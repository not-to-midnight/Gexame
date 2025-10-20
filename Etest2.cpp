#include <glut.h>
#include <iostream>


//上下左右控制移动


//雪人//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // 环境光
GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // 漫反射光
GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // 光源位置

GLfloat matAmbient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat matDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat matShininess[] = { 50.0f };

GLfloat a = 1.0f;


// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

// 定义一个名为ChangeSize的函数，用于在窗口大小改变时进行透视投影的设置
// 参数w和h分别表示窗口的宽度和高度
void ChangeSize(GLsizei w, GLsizei h) // 透视投影
{
    // 定义一个GLfloat类型的变量fAspect，用于存储窗口的宽高比
    GLfloat fAspect;

    // 防止除数为零的情况发生
    // 如果窗口的高度h为0，将其设置为1，避免后续计算宽高比时出现除零错误
    if (h == 0)
        h = 1;

    // 设置OpenGL的视口（Viewport）
    // 视口定义了窗口中用于绘制图形的区域
    // 这里将视口设置为整个窗口，起始坐标为(0, 0)，宽度为w，高度为h
    glViewport(0, 0, w, h);

    // 计算窗口的宽高比
    // 将窗口的宽度w和高度h转换为GLfloat类型后相除，结果存储在fAspect中
    fAspect = (GLfloat)w / (GLfloat)h;

    // 切换到投影矩阵模式
    // OpenGL中有多种矩阵模式，投影矩阵用于定义如何将三维场景投影到二维屏幕上
    glMatrixMode(GL_PROJECTION);

    // 重置投影矩阵为单位矩阵
    // 单位矩阵是一个特殊的矩阵，乘以任何矩阵都不会改变该矩阵的值
    // 这里将投影矩阵重置为单位矩阵，以便后续重新设置投影参数
    glLoadIdentity();

    // 设置透视投影
    // gluPerspective是OpenGL的一个辅助函数，用于创建透视投影矩阵
    // 第一个参数60.0f表示垂直视角，即垂直方向上的视野角度
    // 第二个参数fAspect是窗口的宽高比，用于确定水平视角
    // 第三个参数1.0表示近裁剪面的距离，即离观察者最近的可见距离
    // 第四个参数400.0表示远裁剪面的距离，即离观察者最远的可见距离
    gluPerspective(60.0f, fAspect, 1.0, 400.0);

    // 切换到模型视图矩阵模式
    // 模型视图矩阵用于控制物体的位置、旋转和缩放等变换
    glMatrixMode(GL_MODELVIEW);

    // 重置模型视图矩阵为单位矩阵
    // 这样可以确保后续的物体变换操作是从初始状态开始的
    glLoadIdentity();
}


void drawSnowMan() {
    // 身体 (白色)
    glColor3f(1.0f, a, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);

    // 头部
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    // 眼睛 (黑色)
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

    // 鼻子 (橙色)
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.5f, 10, 2);
    glPopMatrix();
}

void setupRC(void)
{
	glClearColor(0.6f, 0.4f, 0.7f, 1.0f);//设置清屏颜色
	glColor3f(1.0f, 0.0f, 0.0f);//设置绘图颜色
}


void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == 27)//按下ESC键，退出程序
        exit(0);
    if (key == '1') {

        if (a == 0) { a = 1.0f; }
        else { a = 0; }
        glutPostRedisplay();
        std::cout << a;
    }
    switch (key) {
    case '+':
        // 增加环境光亮度，限制在0.0f到1.0f之间
        lightAmbient[0] = lightAmbient[0] + 0.05f > 1.0f ? 1.0f : lightAmbient[0] + 0.05f;
        lightAmbient[1] = lightAmbient[1] + 0.05f > 1.0f ? 1.0f : lightAmbient[1] + 0.05f;
        lightAmbient[2] = lightAmbient[2] + 0.05f > 1.0f ? 1.0f : lightAmbient[2] + 0.05f;
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        break;
    case '-':
        // 减小环境光亮度，限制在0.0f到1.0f之间
        lightAmbient[0] = lightAmbient[0] - 0.05f < 0.0f ? 0.0f : lightAmbient[0] - 0.05f;
        lightAmbient[1] = lightAmbient[1] - 0.05f < 0.0f ? 0.0f : lightAmbient[1] - 0.05f;
        lightAmbient[2] = lightAmbient[2] - 0.05f < 0.0f ? 0.0f : lightAmbient[2] - 0.05f;
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

  void processSpecialKeys(int key, int xx, int yy) {
        float fraction = 0.1f;

        switch (key) {
        case GLUT_KEY_LEFT:
            angle -= 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT:
            angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_UP:
            x += lx * fraction;
            z += lz * fraction;
            break;
        case GLUT_KEY_DOWN:
            x -= lx * fraction;
            z -= lz * fraction;
            break;
        }
    }


void renderScene(void) {//
    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 重置变换矩阵，回到初始状态
    glLoadIdentity();

    // 设置相机位置和方向
    gluLookAt(
        x, 1.0f, z,      // 相机位置
        x + lx, 1.0f, z + lz,  // 相机看向的目标点
        0.0f, 1.0f, 0.0f  // 相机的向上方向
    );

    // 绘制地面
    glColor3f(0.9f, 0.9f, 0.9f);  // 设置地面颜色
    glBegin(GL_QUADS);  // 开始绘制四边形
    glVertex3f(-100.0f, 0.0f, -100.0f);  // 四边形顶点坐标
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();  // 结束绘制四边形

    // 绘制36个雪人
    for (int i = -3; i < 3; i++) {
        for (int j = -3; j < 3; j++) {
            glPushMatrix();  // 保存当前矩阵状态
            glTranslatef(i * 10.0, 0.0, j * 10.0);  // 平移到指定位置
            drawSnowMan();  // 调用函数绘制雪人
            glPopMatrix();  // 恢复之前保存的矩阵状态
        }
    }

    glutSwapBuffers();  // 交换前后缓冲区，显示绘制结果
}

int InitGL(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // 启用光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 设置光源
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // 设置材质
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // 启用颜色追踪
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    return 1;
}


// main函数，程序入口
int main(int argc, char** argv) {
    // 初始化GLUT库
    glutInit(&argc, argv);
    // 设置显示模式，包含深度测试、双缓冲和RGBA颜色模式
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    // 设置窗口初始位置为(100, 100)
    glutInitWindowPosition(100, 100);
    // 设置窗口初始大小为(320, 320)
    glutInitWindowSize(320, 320);
    // 创建窗口并设置标题为"Lighthouse3D - GLUT Tutorial"
    glutCreateWindow("Lighthouse3D - GLUT Tutorial");

    // 注册回调函数
    // 注册显示回调函数，当窗口需要重绘时调用renderScene函数
    glutDisplayFunc(renderScene);
    // 注册窗口大小改变回调函数，当窗口大小改变时调用changeSize函数
    glutReshapeFunc(ChangeSize);
    // 注册空闲回调函数，当程序空闲时调用renderScene函数

    glutIdleFunc(renderScene);//更新摄像机的位置                                                                                    //每秒都在重新绘制画面

    // 注册普通键盘按键回调函数，当有普通键盘按键按下时调用processNormalKeys函数
    glutKeyboardFunc(processNormalKeys);

    // 注册特殊键盘按键回调函数，当有特殊键盘按键按下时调用processSpecialKeys函数
    glutSpecialFunc(processSpecialKeys);
    InitGL();

    //setupRC();
    // 启用OpenGL深度测试


    // 进入GLUT事件处理循环
    glutMainLoop();

    return 1;
}