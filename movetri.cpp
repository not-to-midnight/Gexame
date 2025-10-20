#include<glut.h>
#include<iostream>
using namespace std;
GLfloat red = 1.0, green = 1.0, blue = 1.0;//颜色变量
// Initial square position and size
GLfloat tx1 = 1.0f; GLfloat ty1= 1.0f+25;//左上角的坐标
GLfloat tx2 = 2.0f+25; GLfloat ty2 = 1.0f+25;//右下角的坐标
GLfloat tx3 = -1.0f-25; GLfloat ty3 = -1.0-25;//左下角的坐标


// Step size in x and y directions
// (number of pixels to move each time)
GLfloat stepshu = 1.0f;
GLfloat stepheng= 1.0f;

// Keep track of windows changing width and height
GLfloat windowWidth;
GLfloat windowHeight;

// Called to draw scene  
void RenderScene(void)
{
    // Clear the window with current clearing color  
    glClear(GL_COLOR_BUFFER_BIT);

    // Set current drawing color to red  
    //        R    G    B  
    glColor3f(1.0f, 0.0f, 0.0f);
    glColor3f(red, green, blue);//设置绘图颜色
    // Draw a filled rectangle with current color  
    glBegin(GL_TRIANGLES);//绘制三角形
    glVertex2f(tx1,ty1); // 第一个顶点坐标
    glVertex2f(tx2, ty2); // 第二个顶点坐标
    glVertex2f(tx3, ty3); // 第三个顶点坐标
	glEnd();//结束绘制

    // Flush drawing commands and swap  
    glutSwapBuffers();
}

// Setup the rendering state  
void SetupRC(void)//设置渲染状态
{
    // Set clear color to blue  
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);//设置清屏颜色,清屏颜色相当于画布的颜色或者背景颜色
}

void TimerFunction(int value)//set timer function
{
    // Reverse direction when you reach left or right edge
    //if (x > windowWidth - rsize || x < -windowWidth)//当矩形到达窗口的左右边缘时，改变方向
    //    xstep = -xstep;

    //// Reverse direction when you reach top or bottom edge
    //if (y > windowHeight || y < -windowHeight + rsize)//当矩形到达窗口的上下边缘时，改变方向
    //    ystep = -ystep;

    // Actually move the square


    if(tx3 < -windowWidth || tx2 > windowWidth) {
        stepheng = -stepheng;
    }

    if (ty3 < -windowHeight)
    {

        stepshu = -stepshu;
    }
    if (ty2 > windowHeight)
    {

        stepshu = -stepshu;
    }


tx1 += stepheng;//移动
ty1 += stepshu;
tx2 += stepheng;
ty2 += stepshu;
tx3 += stepheng;
ty3 += stepshu;

    // Check bounds. This is in case the window is made
    // smaller while the rectangle is bouncing and the
    // rectangle suddenly finds itself outside the new
    // clipping volume
    //if (x > (windowWidth - rsize + xstep))//当矩形到达窗的右边缘时，将矩形的x坐标设置为窗口的宽度减去矩形的宽度
    //    x = windowWidth - rsize - 1;
    //else if (x < -(windowWidth + xstep))//当矩形到达 窗口的左边缘时，将矩形的x坐标设置为窗口的宽度加上一个步长，这样矩形就不会在窗口的左边缘消失
    //    x = -windowWidth - 1;

    //if (y > (windowHeight + ystep))
    //    y = windowHeight - 1;
    //else if (y < -(windowHeight - rsize + ystep))
    //    y = -windowHeight + rsize - 1;

    glutPostRedisplay();           
    glutTimerFunc(33, TimerFunction, 1);
}

// Called by GLUT library when the window has changed size
void ChangeSize(int w, int h)
{
    GLfloat aspectRatio;

    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h)
    {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    }
    else
    {
        windowWidth = 100 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == 27)//按下ESC键，退出程序
        exit(0);
}

void processSpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F1:
        red = 1.0;
        green = 0.0;
        blue = 0.0;
        break;
    case GLUT_KEY_F2:
        red = 0.0;
        green = 1.0;
        blue = 0.0;
        break;
    case GLUT_KEY_F3:
        red = 0.0;
        green = 0.0;
        blue = 1.0;
        break;

    case GLUT_KEY_UP:
        stepshu = std::copysign(std::fabs(stepshu) +0.2, stepshu);
        stepheng = std::copysign(std::fabs(stepheng) + 0.2, stepheng);
        break;
    case GLUT_KEY_DOWN:
            stepshu = std::copysign(std::fabs(stepshu) - 0.2, stepshu);
            stepheng = std::copysign(std::fabs(stepheng) - 0.2, stepheng);
		cout << stepshu <<stepheng<< endl;
        break;
    }
}




// Main program entry point
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bounce");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);

    glutKeyboardFunc(processNormalKeys);//键盘回调函数，传入processNormalKeys函数
    glutSpecialFunc(processSpecialKeys);//特殊键盘回调函数，传入processSpecialKeys函数

    SetupRC();

    glutMainLoop();

    return 0;
}