#include <glut.h>
#include <stdlib.h>
#include<iostream>

//上下左右控制转速，L控制灯光,change来改变显示颜色的方式

//会切换材质显示方式的方块/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool change = true;
// 用于表示绕X轴旋转角度
GLfloat xrot;
// 用于表示绕Y轴旋转角度
GLfloat yrot;
// 用于表示绕X轴旋转的速度
GLfloat xspeed;
// 用于表示绕Y轴旋转的速度
GLfloat yspeed;
// 表示在屏幕深度方向（Z轴方向）的位置
GLfloat z = -5.0f;
bool light = true;


GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
GLfloat gray[] = { 0.9f, 0.0f, 0.0f, 1.0f };
GLenum face = GL_FRONT_AND_BACK;
GLenum mode = GL_AMBIENT;

void changesize(int w, int h) {
	if (h == 0)h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 100);

	glMatrixMode(GL_MODELVIEW);

}


void renderscene() {
    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 重置变换矩阵为单位矩阵
    glLoadIdentity();
    // 沿Z轴平移，确定物体在屏幕深度方向的位置
    glTranslatef(0.0f, 0.0f, z);
    // 绕X轴旋转，旋转角度为xrot
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    // 绕Y轴旋转，旋转角度为yrot
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    // 设置颜色为红色（RGBA模式，最后一个参数1.0表示透明度为完全不透明）
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    // 绘制一个边长为1.0的实心立方体
    glutSolidCube(1.0);
    // 交换前后缓冲区，显示绘制结果
    glutSwapBuffers();
    // 根据旋转速度更新绕X轴的旋转角度
    xrot += xspeed;
    // 根据旋转速度更新绕Y轴的旋转角度
    yrot += yspeed;
}


void processSpecialKeys(int key, int x, int y) {
    // 如果按下的是向上翻页键（PAGE_UP）
    if (key == GLUT_KEY_PAGE_UP) {
        // 沿Z轴正方向移动，减少z的值（因为z为负，减一个正数相当于向屏幕外移动）
        z -= 0.02f;
    }
    // 如果按下的是向下翻页键（PAGE_DOWN）
    if (key == GLUT_KEY_PAGE_DOWN) {
        // 沿Z轴负方向移动，增加z的值（向屏幕内移动）
        z += 0.02f;
    }
    // 如果按下的是向上箭头键（UP）
    if (key == GLUT_KEY_UP) {
        // 减少绕X轴旋转速度xspeed的值，使物体绕X轴旋转变慢（若xspeed为正，相当于反向加速 ）
        xspeed -= 0.01f;
    }
    // 如果按下的是向下箭头键（DOWN）
    if (key == GLUT_KEY_DOWN) {
        // 增加绕X轴旋转速度xspeed的值，使物体绕X轴旋转变快
        xspeed += 0.01f;
    }
    // 如果按下的是向左箭头键（LEFT）
    if (key == GLUT_KEY_LEFT) {
        // 减少绕Y轴旋转速度yspeed的值，使物体绕Y轴旋转变慢（若yspeed为正，相当于反向加速 ）
        yspeed -= 0.01f;
    }
    // 如果按下的是向右箭头键（RIGHT）
    if (key == GLUT_KEY_RIGHT) {
        // 增加绕Y轴旋转速度yspeed的值，使物体绕Y轴旋转变快
        yspeed += 0.01f;
    }
}
int InitGL(void)
{
    glEnable(GL_LIGHTING);
    // 设置着色模式为平滑着色（Gouraud着色），使图形表面颜色过渡更自然
    glShadeModel(GL_SMOOTH);
    // 设置清除颜色为黑色，透明度为0.5
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    // 设置深度缓冲区的清除值为1.0
    glClearDepth(1.0f);
    // 启用深度测试功能，用于处理三维场景中物体的前后遮挡关系
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试函数，GL_LEQUAL表示当新的深度值小于等于缓冲区中的深度值时，通过测试
    glDepthFunc(GL_LEQUAL);
    // 设置透视校正提示，GL_NICEST表示使用最高质量的透视校正方式
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    if (change) {
        glColorMaterial(face,  mode);
        glEnable(GL_COLOR_MATERIAL);
    }
    else {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
    }

    // 设置光源1的环境光参数
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    // 设置光源1的漫反射光参数
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    // 设置光源1的位置
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    // 启用光源1

    glEnable(GL_LIGHT1);

    return 1; // 这里原代码return true在C语言环境下错误，C中int函数应返回int值，改为1表示初始化成功


}

void processNormalKeys(unsigned char key, int x, int y) {
    // 根据按下的按键进行不同操作
    switch (key) {
        // 当按下的按键ASCII码为27，即Esc键
    case 27:
        // 调用exit函数并传入参数0，用于正常退出程序
        exit(0);
        break;
        // 当按下的按键为字符'l'（小写L）
    case 'l':
        // 对light变量取反，切换光源状态
  
        light = !light;
        std::cout << light;
        // 根据light的值判断是否启用或禁用光照
        light ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
        break;

    case 'f':
        change = !change;
        std::cout << "change";
        break;

    }
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("rotatae cube");
   

    InitGL();

	glutDisplayFunc(renderscene);
	glutIdleFunc(renderscene);
	glutReshapeFunc(changesize);
	glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

	glutMainLoop();
	return 0;
}