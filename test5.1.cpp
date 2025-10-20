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

// 相机相关变量
float camerax = 0.0f, cameray = 0.0f, cameraz = 1.0f;
float lookx = 0.0f, looky = 0.0f, lookz = 0.0f;
float upx = 0.0f, upy = 0.5f, upz = 0.0f;
float moveSpeed = 0.1f;
float rotateSpeed = 0.01f;
float yaw = 0.0f;
float pitch = 0.0f;
double sensitivity = 0.005f;
int lastX = 400, lastY = 300;
bool firstMouse = true;
int windowWidth = 800, windowHeight = 600;
double speedcam = 0.2f;
bool rightMouseDown = false;

// 波浪相关变量
int numWaves = 3;
float amplitudes[MAX_WAVES] = { 0.01, 0.008, 0.005 };
float wavelengths[MAX_WAVES] = { 0.3, 0.4, 0.5 };
float speeds[MAX_WAVES] = { -0.2, 0.3, -0.15 };
float centers[MAX_WAVES][2] = {
    {0.0, 0.0},
    {-0.5, 0.5},
    {0.5, -0.5}
};
struct Vector3 {
    float x;
    float y;
    float z;
};
float amplitude = 0.01;
float wavelength = 0.3;
float speed = -0.2;
float center[2] = { 0.0, 0.0 };
int wire_frame = 0;
int show_normals = 0;
int left_click = 0, right_click = 0;
int xold = 0, yold = 0;
float rotate_x = 0, rotate_y = 0;
float translate_z = 5.0;
GLuint texture;

// 新增变量用于Gerstner Wave模型
float Q[MAX_WAVES] = { 0.5, 0.5, 0.5 };     // 控制波浪陡峭程度的参数

float wi[MAX_WAVES];                      // 波数
float Ai[MAX_WAVES];                      // 振幅

float Di_x[MAX_WAVES] = { 1.0, 0.8, 0.6 };  // 波在x方向的向量分量
float Di_y[MAX_WAVES] = { 0.0, 0.6, 0.8 };  // 波在y方向的向量分量
float phi[MAX_WAVES];                     // 相位参数

static float surface[6 * RESOLUTION * (RESOLUTION + 1)];
static float normal[6 * RESOLUTION * (RESOLUTION + 1)];

// 初始化Gerstner Wave参数
void initWaveParams() {
    for (int i = 0; i < numWaves; i++) {
        wi[i] = 2 * PI / wavelengths[i];  // 波长
        Ai[i] = amplitudes[i];            // 波幅
        phi[i] = speeds[i];               // 传播速率
    }
}

// Gerstner Wave模型计算单个波浪
Vector3 gerstnerWave(int i, float x, float y, float time) {
    float wx = wi[i] * Di_x[i];
    float wy = wi[i] * Di_y[i];
    float arg = wx * x + wy * y + phi[i] * time;
    float cosValue = cos(arg);
    float sinValue = sin(arg);

    // 计算偏移后的坐标
    float newX =  - Q[i] * Ai[i] * Di_x[i] * cosValue;
    float newY =  - Q[i] * Ai[i] * Di_y[i] * cosValue;
    float newZ = Ai[i] * sinValue;

    return { newX, newY, newZ };
}

// 计算所有波浪叠加后的坐标
Vector3 gerstnerWaveHeight(float x, float y, float time) {
    Vector3 result = { x, y, 0.0f };

    for (int i = 0; i < numWaves; i++) {
        Vector3 wave = gerstnerWave(i, x, y, time);
        result.x += wave.x; //减去原来的值
        result.y += wave.y;
        result.z += wave.z;
    }

    return result;
}

// 计算Gerstner Wave的法线
Vector3 gerstnerWaveNormal(float x, float y, float time) {
    float Bx = 1.0f, By = 0.0f, Bz = 0.0f;  // 切线向量B
    float Tx = 0.0f, Ty = 1.0f, Tz = 0.0f;  // 切线向量T
    float Nx = 0.0f, Ny = 0.0f, Nz = 1.0f;  // 法线向量N

    for (int i = 0; i < numWaves; i++) {
        float wx = wi[i] * (x - centers[i][0]);
        float wy = wi[i] * (x - centers[i][0]);
        float arg = wx * x + wy * y + phi[i] * time;
        float cosArg = cos(arg);
        float sinArg = sin(arg);

        // 计算偏导数
        float dx = -Q[i] * Ai[i] * wi[i] * (x - centers[i][0]) * (x - centers[i][0]) * sinArg;
        float dy = -Q[i] * Ai[i] * wi[i] * (x - centers[i][0]) * (y - centers[i][1]) * sinArg;
        float dz = -Ai[i] * wi[i] * (x - centers[i][0]) * cosArg;

        Bx += dx;
        By += dy;
        Bz += dz;

        dx = -Q[i] * Ai[i] * wi[i] * (x - centers[i][0]) * (y - centers[i][1]) * sinArg;
        dy = -Q[i] * Ai[i] * wi[i] * (y - centers[i][1]) * (y - centers[i][1]) * sinArg;
        dz = -Ai[i] * wi[i] * (y - centers[i][1]) * cosArg;

        Tx += dx;
        Ty += dy;
        Tz += dz;
    }

    // 计算法线 (B x T)
    Nx = By * Tz - Bz * Ty;
    Ny = Bz * Tx - Bx * Tz;
    Nz = Bx * Ty - By * Tx;

    // 归一化法线
    float len = sqrtf(Nx * Nx + Ny * Ny + Nz * Nz);
    if (len > 0.0f) {
        Nx /= len;
        Ny /= len;
        Nz /= len;
    }

    return { Nx, Ny, Nz };
}

void generateSurface(float time) {
    int index = 0;
    for (int i = 0; i < RESOLUTION; i++) {
        for (int j = 0; j <= RESOLUTION; j++) {
            float x = (j / (float)RESOLUTION) * 2.0f - 1.0f;  // 水面网格的 x 坐标
            float z = (i / (float)RESOLUTION) * 2.0f - 1.0f;  // 水面网格的 z 坐标

            // 使用Gerstner Wave计算顶点坐标
            Vector3 wavePos = gerstnerWaveHeight(x, z, time);

            surface[index++] = wavePos.x;
            surface[index++] = wavePos.z;  
            surface[index++] = wavePos.y; 

            // 使用Gerstner Wave计算法线
            Vector3 n = gerstnerWaveNormal(x, z, time);
            normal[index++] = n.x;
            normal[index++] = n.z; 
            normal[index++] = n.y;  
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


    if (wire_frame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

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
    // 计算观察方向向量（从相机指向观察点）
    lookx = camerax + cosf(yaw) * cosf(pitch);
    looky = cameray + sinf(pitch);
    lookz = cameraz + sinf(yaw) * cosf(pitch);

    // 计算向上向量（简化版，实际应用中可能需要更复杂的计算）
    // 这里假设世界上方向量为(0,1,0)
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

    // 原有鼠标移动逻辑...
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
    glutInitWindowSize(800, 600);
    glutCreateWindow("Gerstner Wave Water Simulation");

    initGL();
    initWaveParams();  // 初始化Gerstner Wave参数

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