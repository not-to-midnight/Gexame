#include <glut.h>
#include <iostream>



///！！！11号

//数字1来切换smooth或者flat





// 定义一个变量来表示着色模式，初始化为 GL_SMOOTH
int shadingMode = GL_SMOOTH;

// 绘制场景的函数
void RenderScene(void)
{
    // 清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);

    // 根据 shadingMode 的值设置着色模式
    glShadeModel(shadingMode);

    // 开始绘制三角形
    glBegin(GL_TRIANGLES);
    // 设置第一个顶点颜色为红色
    glColor3f(1.0f, 0.0f, 0.0f);
    // 设置第一个顶点的坐标
    glVertex3f(0.0f, 1.0f, 0.0f);

    // 设置第二个顶点颜色为绿色
    glColor3f(0.0f, 1.0f, 0.0f);
    // 设置第二个顶点的坐标
    glVertex3f(-1.0f, -1.0f, 0.0f);

    // 设置第三个顶点颜色为蓝色
    glColor3f(0.0f, 0.0f, 1.0f);
    // 设置第三个顶点的坐标
    glVertex3f(1.0f, -1.0f, 0.0f);
    // 结束绘制三角形
    glEnd();

    // 强制刷新缓冲区，将绘制结果显示出来
    glFlush();
}

// 初始化 OpenGL 设置的函数
void SetupRC(void)
{
    // 设置清除颜色为黑色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// 窗口大小改变时的回调函数
void ChangeSize(int w, int h)
{
    GLfloat aspectRatio;

    // 防止除零错误
    if (h == 0)
        h = 1;

    // 设置视口为整个窗口
    glViewport(0, 0, w, h);

    // 切换到投影矩阵模式
    glMatrixMode(GL_PROJECTION);
    // 重置投影矩阵为单位矩阵
    glLoadIdentity();

    // 计算窗口的宽高比
    aspectRatio = (GLfloat)w / (GLfloat)h;

    // 设置正交投影
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, -1.0, 1.0);
    else
        glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, -1.0, 1.0);

    // 切换到模型视图矩阵模式
    glMatrixMode(GL_MODELVIEW);
    // 重置模型视图矩阵为单位矩阵
    glLoadIdentity();
}

// 键盘按键处理函数
void Keyboard(unsigned char key, int x, int y)
{
    if (key == '1') {
        // 按下数字 1 时切换着色模式
        if (shadingMode == GL_SMOOTH) {
            shadingMode = GL_FLAT;
            std::cout << "Switch to GL_FLAT shading mode." << std::endl;
        }
        else {
            shadingMode = GL_SMOOTH;
            std::cout << "Switch to GL_SMOOTH shading mode." << std::endl;
        }
        // 标记窗口需要重新绘制
        glutPostRedisplay();
    }
}

// 主函数
int main(int argc, char* argv[])
{
    // 初始化 GLUT 库
    glutInit(&argc, argv);
    // 设置显示模式为单缓冲和 RGB 颜色模式
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // 创建窗口并设置标题
    glutCreateWindow("Shading Mode Switch Example");
    // 注册绘制场景的回调函数
    glutDisplayFunc(RenderScene);
    // 注册窗口大小改变的回调函数
    glutReshapeFunc(ChangeSize);
    // 注册键盘按键处理的回调函数
    glutKeyboardFunc(Keyboard);

    //glutIdleFunc(RenderScene);
    // 初始化 OpenGL 设置
    SetupRC();

    // 进入 GLUT 主循环，处理事件
    glutMainLoop();

    return 0;
}