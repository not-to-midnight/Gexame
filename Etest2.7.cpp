#include <glut.h>
#include <stdlib.h>
#include<iostream>
#include"SOIL.h"




//待材质的立体方块/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//上下左右控制转速，L控制灯光


bool light; // Lighting ON/OFF
GLfloat xrot; // X Rotation
GLfloat yrot; // Y Rotation
GLfloat xspeed; // X Rotation Speed
GLfloat yspeed; // Y Rotation Speed
GLfloat z = -5.0f; // Depth Into The Screen
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
GLuint filter; // Which Filter To Use
GLuint texture[2]; // Storage For 2 Textures

void changeSize(int w, int h) {
    if (h == 0)h = 1;

    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45, ratio, 1, 100);

    glMatrixMode(GL_MODELVIEW);

}


void renderScene(void) {
    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 重置变换矩阵
    glLoadIdentity();
    // 沿Z轴平移
    glTranslatef(0.0f, 0.0f, z);
    // 绕X轴旋转
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    // 绕Y轴旋转
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture[filter]);//

    // 开始绘制四边形
    glBegin(GL_QUADS);
    // 前面
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // 后面
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    // 顶面
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // 底面
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    // 右面
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // 左面
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    // 交换缓冲区
    glutSwapBuffers();

    // 更新旋转角度
    xrot += xspeed;
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


int LoadGLTextures()
{
    for (int i = 0; i < 2; i++)
    {
        /* load an image file directly as a new OpenGL texture */
        texture[i] = SOIL_load_OGL_texture
        (
            "Glass.bmp",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y
        );

        if (texture[0] == 0) {
            std::cout << "纹理丢失";
            return false;

        }

        
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (texture[1] == 0) {
            std::cout << "丢失";
            return false;
        }
        // Create Linear Filtered Texture
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    return true; // Return Success
}


int InitGL(void) {
    LoadGLTextures();
    // 启用二维纹理映射
    glEnable(GL_TEXTURE_2D);
    // 设置平滑着色模式
    glShadeModel(GL_SMOOTH);
    // 设置清除颜色为黑色，透明度0.5
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    // 设置深度缓冲区清除值
    glClearDepth(1.0f);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试函数
    glDepthFunc(GL_LEQUAL);
    // 设置透视校正提示
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // 设置环境光
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    // 设置漫反射光
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    // 设置光源位置
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    // 启用光源1
    glEnable(GL_LIGHT1);

    return true;
}

// 处理普通键盘按键函数
void processNormalKeys(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        // Esc键按下后退出程序
        exit(0);
        break;
    case 'l':
        // 切换光源开关状态
        light = !light;
        light ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
        break;
    case 'f':
        // 选择滤波方式
        filter += 1;
        if (filter > 1) filter = 0;
        printf("%d", filter);
        break;
    }
}

// 主函数
int main(int argc, char** argv) {
    // 初始化GLUT库
    glutInit(&argc, argv);
    // 设置显示模式，包含深度测试、双缓冲和RGBA颜色模式
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    // 设置窗口初始位置
    glutInitWindowPosition(100, 100);
    // 设置窗口初始大小
    glutInitWindowSize(640, 640);
    // 创建窗口并设置标题
    glutCreateWindow("Texture");

    // 注册回调函数
	glutDisplayFunc(renderScene); // 注册显示回调函数
	glutIdleFunc(renderScene); // 当窗口空闲时调用renderScene函数
    glutReshapeFunc(changeSize); // 此处changeSize函数未给出定义，需自行补充

	// 注册键盘和特殊键回调函数
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys); // 此处processSpecialKeys函数未给出定义，需自行补充

    if (!InitGL()) {
        printf("Initialization Failed.");
        return 1;
    }

    // 进入GLUT事件处理循环
    glutMainLoop();

    return 0;
}