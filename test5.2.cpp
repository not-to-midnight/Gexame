#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <math.h>
#include "SOIL.h"
#include <GL/gl.h> 
#include <GL/glu.h>
#include <random>
#include <math.h>   
#include <iostream>



#define PI 3.14159265358979323846
#define RESOLUTION 100
#define MAX_WAVES 5


float camerax = 0.0f, cameray = 0.0f, cameraz = 1.0f; // ���λ��
float lookx = 0.0f, looky = 0.0f, lookz = 0.0f; // �۲��λ��
float upx = 0.0f, upy = 0.5f, upz = 0.0f; // 
// ������ƶ��ٶȺ���ת������
float moveSpeed = 0.1f;
float rotateSpeed = 0.01f;
// ���������������ת���ƣ�
float yaw = 0.0f;      // ˮƽ��ת�Ƕ�
float pitch = 0.0f;    // ��ֱ��ת�Ƕ�
double sensitivity = 0.005f;  // ���������
int lastX = 400, lastY = 300;  // �����һλ��
bool firstMouse = true;  // �Ƿ��״β������
int windowWidth = 800, windowHeight = 600;
// �ƶ��ٶ�
double speedcam = 0.2f;
bool rightMouseDown = false;



int numWaves = 3;
float amplitudes[MAX_WAVES] = { 0.01, 0.008, 0.005 };
float wavelengths[MAX_WAVES] = { 0.3, 0.4, 0.5 };
float speeds[MAX_WAVES] = { -0.2, 0.3, -0.15 };
float centers[MAX_WAVES][2] = {
    {0.0, 0.0},
    {-0.5, 0.5},
    {0.5, -0.5}
};
static float surface[6 * RESOLUTION * (RESOLUTION + 1)];
static float normal[6 * RESOLUTION * (RESOLUTION + 1)];

struct Vector3 {
    float x;
    float y;
    float z;
};

float speed = -0.2;
float rotate_x = 0, rotate_y = 0;
GLuint texture;


float dot(int i, float x, float y) {
    float xc = x - centers[i][0];
    float yc = y - centers[i][1];
    return sqrt(xc * xc + yc * yc);
}

float wave(int i, float x, float y, float time) {
    float frequency = 2 * PI / wavelengths[i];     ///w
    float phase = speeds[i] * frequency;    //fai
    float theta = dot(i, x, y);         //D(x,y)
    float sinValue = sin(theta * frequency + time * phase);
    return 2 * amplitudes[i] * pow((sinValue + 1) / 2, 3);  // k=3  
}//��x��y�㲨�˵ĸ߶�

float waveHeight(float x, float y, float time) {
    float height = 0.0;
    for (int i = 0; i < numWaves; i++)
        height += wave(i, x, y, time);
    return height;
}

float dWavedx(int i, float x, float y, float time) {
    float frequency = 2 * PI / wavelengths[i];
    float phase = speeds[i] * frequency;
    float theta = dot(i, x, y);
    float sinValue = sin(theta * frequency + time * phase);
    float cosValue = cos(theta * frequency + time * phase);

    float k = 3;  // ָ������
    float term = (sinValue + 1) / 2;
    return k * (x - centers[i][0]) * frequency * amplitudes[i] * pow(term, k - 1) * cosValue;
}

float dWavedy(int i, float x, float y, float time) {
    float frequency = 2 * PI / wavelengths[i];
    float phase = speeds[i] * frequency;
    float theta = dot(i, x, y);
    float sinValue = sin(theta * frequency + time * phase);
    float cosValue = cos(theta * frequency + time * phase);

    float k = 3;  // ָ������
    float term = (sinValue + 1) / 2;
    return k * (y - centers[i][1]) * frequency * amplitudes[i] * pow(term, k - 1) * cosValue;
}

Vector3 waveNormal(float x, float y, float time) {//������˵��ӣ�������Ӻ��dx��dy
    float dx = 0.0, dy = 0.0;
    for (int i = 0; i < numWaves; i++) {
        dx += dWavedx(i, x, y, time);
        dy += dWavedy(i, x, y, time);
    }

    Vector3 n;
    n.x = -dx;
    n.y = 1.0;
    n.z = -dy;

    float l = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    if (l != 0) {
        n.x /= l;
        n.y /= l;
        n.z /= l;
    }
    else {
        n.x = 0;
        n.y = 1;
        n.z = 0;
    }
    return n;
}

void generateSurface(float time) {
    int index = 0;
    for (int i = 0; i < RESOLUTION; i++) {
        for (int j = 0; j <= RESOLUTION; j++) {
            float x = (j / (float)RESOLUTION) * 2.0f - 1.0f;  // ˮ������� x ����
            float z = (i / (float)RESOLUTION) * 2.0f - 1.0f;  // ˮ������� z ����

            // ��������
            surface[index++] = x;
            surface[index++] = waveHeight(x, z, time);  // y Ϊ���˸߶�
            surface[index++] = z;

            // ����
            Vector3 n = waveNormal(x, z, time);
            normal[index++] = n.x;
            normal[index++] = n.y;
            normal[index++] = n.z;
        }
    }
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        camerax, cameray, cameraz,	// ���λ��
        lookx, looky, lookz,		// �۲��λ��
        upx, upy, upz);			// ��������

    glRotatef(rotate_y, 1.0f, 0.0f, 0.0f);
    glRotatef(rotate_x, 0.0f, 1.0f, 0.0f);


    // �������������ֶ�ָ�����꣩
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    for (int i = 0; i < RESOLUTION - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= RESOLUTION; j++) {
            int idx1 = (i * (RESOLUTION + 1) + j) * 6;
            int idx2 = ((i + 1) * (RESOLUTION + 1) + j) * 6;

            // ɾ���ֶ�������������꣨�����Զ����ɣ�
            glNormal3f(normal[idx1 + 3], normal[idx1 + 4], normal[idx1 + 5]);
            glVertex3f(surface[idx1], surface[idx1 + 1], surface[idx1 + 2]);

            glNormal3f(normal[idx2 + 3], normal[idx2 + 4], normal[idx2 + 5]);
            glVertex3f(surface[idx2], surface[idx2 + 1], surface[idx2 + 2]);
        }

        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);


    glColor3f(1.0f, 1.0f, 1.0f);  // �ָ�Ĭ����ɫ
    glutSwapBuffers();
}









void updateCameraVectors() {

    lookx = camerax + cosf(yaw) * cosf(pitch);
    looky = cameray + sinf(pitch);
    lookz = cameraz + sinf(yaw) * cosf(pitch);


    upx = 0.0f;
    upy = 1.0f;
    upz = 0.0f;
}

void mouseButtonCallback(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            std::cout << "mouseButtonCallback" << std::endl;
            rightMouseDown = true;
            firstMouse = true;  // �������λ�ã���������
        }
        else {
            std::cout << "mouseButtonCallback2" << std::endl;
            rightMouseDown = false;
        }
    }
}

// ͳһ������ƶ�������
void mouseMovementHandler(int x, int y) {
    if (!rightMouseDown) return;  // ֻ���Ҽ�����ʱ����
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 1.2f) pitch = 1.2f;
    if (pitch < -1.2f) pitch = -1.2f;
    updateCameraVectors();
    glutPostRedisplay();
}

void passiveMouseCallback(int x, int y) {
    if (!rightMouseDown) return;  // ֻ���Ҽ�����ʱ�����ƶ�
    mouseMovementHandler(x, y);
}

// ����ƶ��ص����а�ť����ʱ��
void activeMouseCallback(int x, int y) {
    if (!rightMouseDown) return;  // ֻ���Ҽ�����ʱ�����ƶ�
    mouseMovementHandler(x, y);
}

// ���̰����ص�����
void keyboardCallback(unsigned char key, int x, int y) {
    // �������ǰ����������һ����
    float forwardX = lookx - camerax;
    float forwardY = looky - cameray;
    float forwardZ = lookz - cameraz;
    float len = sqrtf(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    forwardX /= len;
    forwardY /= len;
    forwardZ /= len;

    // �����������������ǰ���������������
    float rightX, rightY, rightZ;
    rightX = forwardY * upz - forwardZ * upy;
    rightY = forwardZ * upx - forwardX * upz;
    rightZ = forwardX * upy - forwardY * upx;
    len = sqrtf(rightX * rightX + rightY * rightY + rightZ * rightZ);
    rightX /= len;
    rightY /= len;
    rightZ /= len;

    // ���ݰ����������λ��
    switch (key) {
    case 'w':  // ǰ��
    case 'W':
        camerax += forwardX * speedcam;
        cameray += forwardY * speedcam;
        cameraz += forwardZ * speedcam;
        
        break;
    case 's':  // ����
    case 'S':
        camerax -= forwardX * speedcam;
        cameray -= forwardY * speedcam;
        cameraz -= forwardZ * speedcam;
        break;
    case 'a':  // ����
    case 'A':
        camerax -= rightX * speedcam;
        cameray -= rightY * speedcam;
        cameraz -= rightZ * speedcam;
        break;
    case 'd':  // ����
    case 'D':
        camerax += rightX * speedcam;
        cameray += rightY * speedcam;
        cameraz += rightZ * speedcam;
        break;
    case'q':
    case'Q':
        cameray += speedcam * 0.5;
        break;
    case'e':
    case'E':
        cameray -= speedcam * 0.5;
        break;

    case'1':
        speed += 0.01;
        std::cout << "�ƶ��ٶ�:" << speed << std::endl;
        break;
    case'2':
        speed -= 0.01;
        std::cout << "�ƶ��ٶ�:" << speed << std::endl;
        break;
    case'3':
        sensitivity += 0.0001;
        std::cout << "���������:" << sensitivity << std::endl;
        break;
    case'4':
        sensitivity -= 0.0001;
        std::cout << "���������:" << sensitivity << std::endl;
        break;
    }

    // ���¹۲��λ�ã��������������Է���
    lookx = camerax + forwardX;
    looky = cameray + forwardY;
    lookz = cameraz + forwardZ;

    // ǿ���ػ�
    glutPostRedisplay();
}

void timerCallback(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);  // Լ60FPS
}

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0 * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int loadTexture() {
    texture = SOIL_load_OGL_texture(
        "reflection.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (texture == 0) return 0;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // �޸�Ϊ GL_CLAMP��ָ����Ҫ��
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // ����Զ������������ɣ�ָ����ؼ��޸ģ�
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    return 1;
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // ���ñ�����ɫΪ��ɫ
    glClearDepth(1.0f);                      // ������Ȼ���
    glEnable(GL_DEPTH_TEST);                 // ������Ȳ���
    glDepthFunc(GL_LEQUAL);                  // ������Ȳ��ԵıȽϺ���
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // ����͸������

    // ��ӹ���
    GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // ������
    GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // �������
    GLfloat LightPosition[] = { 1.0f, 1.0f, -0.5f, 0.0f };  // ��Դλ��
    GLfloat mat_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };   // �߹ⷴ��
    GLfloat mat_shininess[] = { 100.0f };                    // �߹�ָ��

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);      // ���ò��ʵĸ߹�
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);    // ���ò��ʵĸ߹�ָ��
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);         // ���ù�Դ�Ļ�����
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);         // ���ù�Դ���������
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);       // ���ù�Դ��λ��
    glEnable(GL_LIGHT1);                                  // ���ù�Դ1
    glEnable(GL_LIGHTING);                                 // ���ù���
    glColorMaterial(GL_FRONT, GL_DIFFUSE);                 // ���ò�����ɫ
    glEnable(GL_COLOR_MATERIAL);                           // ���ò�����ɫ


    // ��������
    if (!loadTexture()) {
        printf("Failed to load texture\n");
        exit(1);
    }
}


void reshapeCallback(int width, int height) {
    if (height == 0) height = 1;
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Water Simulation");

    initGL();

    glutDisplayFunc(renderScene);

    updateCameraVectors();

    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutPassiveMotionFunc(passiveMouseCallback);  // �ް�ť����ʱ���ƶ�
    glutMotionFunc(activeMouseCallback);          // �а�ť����ʱ���ƶ�
    glutMouseFunc(mouseButtonCallback);           // ��갴��
    glutTimerFunc(0, timerCallback, 0);    // ��ʱ���ص�

    glutIdleFunc([]() {
        static float time = 0;
        time += 0.01;
        generateSurface(time);
        glutPostRedisplay();
        });

    glutMainLoop();
    return 0;
}