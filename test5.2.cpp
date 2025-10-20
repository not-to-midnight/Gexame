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


float camerax = 0.0f, cameray = 0.0f, cameraz = 1.0f; // 相机位置
float lookx = 0.0f, looky = 0.0f, lookz = 0.0f; // 观察点位置
float upx = 0.0f, upy = 0.5f, upz = 0.0f; // 
// 摄像机移动速度和旋转灵敏度
float moveSpeed = 0.1f;
float rotateSpeed = 0.01f;
// 摄像机朝向（用于旋转控制）
float yaw = 0.0f;      // 水平旋转角度
float pitch = 0.0f;    // 垂直旋转角度
double sensitivity = 0.005f;  // 鼠标灵敏度
int lastX = 400, lastY = 300;  // 鼠标上一位置
bool firstMouse = true;  // 是否首次捕获鼠标
int windowWidth = 800, windowHeight = 600;
// 移动速度
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
}//在x、y点波浪的高度

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

    float k = 3;  // 指数参数
    float term = (sinValue + 1) / 2;
    return k * (x - centers[i][0]) * frequency * amplitudes[i] * pow(term, k - 1) * cosValue;
}

float dWavedy(int i, float x, float y, float time) {
    float frequency = 2 * PI / wavelengths[i];
    float phase = speeds[i] * frequency;
    float theta = dot(i, x, y);
    float sinValue = sin(theta * frequency + time * phase);
    float cosValue = cos(theta * frequency + time * phase);

    float k = 3;  // 指数参数
    float term = (sinValue + 1) / 2;
    return k * (y - centers[i][1]) * frequency * amplitudes[i] * pow(term, k - 1) * cosValue;
}

Vector3 waveNormal(float x, float y, float time) {//多个波浪叠加，计算叠加后的dx和dy
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
            float x = (j / (float)RESOLUTION) * 2.0f - 1.0f;  // 水面网格的 x 坐标
            float z = (i / (float)RESOLUTION) * 2.0f - 1.0f;  // 水面网格的 z 坐标

            // 顶点坐标
            surface[index++] = x;
            surface[index++] = waveHeight(x, z, time);  // y 为波浪高度
            surface[index++] = z;

            // 法线
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
        camerax, cameray, cameraz,	// 相机位置
        lookx, looky, lookz,		// 观察点位置
        upx, upy, upz);			// 向上向量

    glRotatef(rotate_y, 1.0f, 0.0f, 0.0f);
    glRotatef(rotate_x, 0.0f, 1.0f, 0.0f);


    // 启用纹理（无需手动指定坐标）
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    for (int i = 0; i < RESOLUTION - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= RESOLUTION; j++) {
            int idx1 = (i * (RESOLUTION + 1) + j) * 6;
            int idx2 = ((i + 1) * (RESOLUTION + 1) + j) * 6;

            // 删除手动计算的纹理坐标（改用自动生成）
            glNormal3f(normal[idx1 + 3], normal[idx1 + 4], normal[idx1 + 5]);
            glVertex3f(surface[idx1], surface[idx1 + 1], surface[idx1 + 2]);

            glNormal3f(normal[idx2 + 3], normal[idx2 + 4], normal[idx2 + 5]);
            glVertex3f(surface[idx2], surface[idx2 + 1], surface[idx2 + 2]);
        }

        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);


    glColor3f(1.0f, 1.0f, 1.0f);  // 恢复默认颜色
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
            firstMouse = true;  // 重置鼠标位置，避免跳变
        }
        else {
            std::cout << "mouseButtonCallback2" << std::endl;
            rightMouseDown = false;
        }
    }
}

// 统一的鼠标移动处理函数
void mouseMovementHandler(int x, int y) {
    if (!rightMouseDown) return;  // 只在右键按下时处理
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
    if (!rightMouseDown) return;  // 只在右键按下时处理移动
    mouseMovementHandler(x, y);
}

// 鼠标移动回调（有按钮按下时）
void activeMouseCallback(int x, int y) {
    if (!rightMouseDown) return;  // 只在右键按下时处理移动
    mouseMovementHandler(x, y);
}

// 键盘按键回调函数
void keyboardCallback(unsigned char key, int x, int y) {
    // 计算相机前向向量（归一化）
    float forwardX = lookx - camerax;
    float forwardY = looky - cameray;
    float forwardZ = lookz - cameraz;
    float len = sqrtf(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    forwardX /= len;
    forwardY /= len;
    forwardZ /= len;

    // 计算相机右向量（与前向和上向量正交）
    float rightX, rightY, rightZ;
    rightX = forwardY * upz - forwardZ * upy;
    rightY = forwardZ * upx - forwardX * upz;
    rightZ = forwardX * upy - forwardY * upx;
    len = sqrtf(rightX * rightX + rightY * rightY + rightZ * rightZ);
    rightX /= len;
    rightY /= len;
    rightZ /= len;

    // 根据按键更新相机位置
    switch (key) {
    case 'w':  // 前进
    case 'W':
        camerax += forwardX * speedcam;
        cameray += forwardY * speedcam;
        cameraz += forwardZ * speedcam;
        
        break;
    case 's':  // 后退
    case 'S':
        camerax -= forwardX * speedcam;
        cameray -= forwardY * speedcam;
        cameraz -= forwardZ * speedcam;
        break;
    case 'a':  // 左移
    case 'A':
        camerax -= rightX * speedcam;
        cameray -= rightY * speedcam;
        cameraz -= rightZ * speedcam;
        break;
    case 'd':  // 右移
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
        std::cout << "移动速度:" << speed << std::endl;
        break;
    case'2':
        speed -= 0.01;
        std::cout << "移动速度:" << speed << std::endl;
        break;
    case'3':
        sensitivity += 0.0001;
        std::cout << "鼠标灵敏度:" << sensitivity << std::endl;
        break;
    case'4':
        sensitivity -= 0.0001;
        std::cout << "鼠标灵敏度:" << sensitivity << std::endl;
        break;
    }

    // 更新观察点位置（保持与相机的相对方向）
    lookx = camerax + forwardX;
    looky = cameray + forwardY;
    lookz = cameraz + forwardZ;

    // 强制重绘
    glutPostRedisplay();
}

void timerCallback(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);  // 约60FPS
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
    // 修改为 GL_CLAMP（指导书要求）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // 添加自动纹理坐标生成（指导书关键修改）
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    return 1;
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // 设置背景颜色为黑色
    glClearDepth(1.0f);                      // 设置深度缓存
    glEnable(GL_DEPTH_TEST);                 // 启用深度测试
    glDepthFunc(GL_LEQUAL);                  // 设置深度测试的比较函数
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // 设置透视修正

    // 添加光照
    GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // 环境光
    GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // 漫反射光
    GLfloat LightPosition[] = { 1.0f, 1.0f, -0.5f, 0.0f };  // 光源位置
    GLfloat mat_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };   // 高光反射
    GLfloat mat_shininess[] = { 100.0f };                    // 高光指数

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);      // 设置材质的高光
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);    // 设置材质的高光指数
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);         // 设置光源的环境光
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);         // 设置光源的漫反射光
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);       // 设置光源的位置
    glEnable(GL_LIGHT1);                                  // 启用光源1
    glEnable(GL_LIGHTING);                                 // 启用光照
    glColorMaterial(GL_FRONT, GL_DIFFUSE);                 // 启用材质颜色
    glEnable(GL_COLOR_MATERIAL);                           // 启用材质颜色


    // 加载纹理
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
    glutPassiveMotionFunc(passiveMouseCallback);  // 无按钮按下时的移动
    glutMotionFunc(activeMouseCallback);          // 有按钮按下时的移动
    glutMouseFunc(mouseButtonCallback);           // 鼠标按键
    glutTimerFunc(0, timerCallback, 0);    // 定时器回调

    glutIdleFunc([]() {
        static float time = 0;
        time += 0.01;
        generateSurface(time);
        glutPostRedisplay();
        });

    glutMainLoop();
    return 0;
}