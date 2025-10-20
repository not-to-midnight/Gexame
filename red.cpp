#include<glut.h>
#include<iostream>
using namespace std;

// Initial square position and size
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat rsize = 25;

// Step size in x and y directions
// (number of pixels to move each time)
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

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

    // Draw a filled rectangle with current color  
	glRectf(x, y, x + rsize, y - rsize);//绘制矩形,4个参数分别为左下角和右上角的坐标

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
	if (x > windowWidth - rsize || x < -windowWidth)//当矩形到达窗口的左右边缘时，改变方向
        xstep = -xstep;

    // Reverse direction when you reach top or bottom edge
	if (y > windowHeight || y < -windowHeight + rsize)//当矩形到达窗口的上下边缘时，改变方向
        ystep = -ystep;

    // Actually move the square
	x += xstep;//移动矩形的x坐标
	y += ystep;//移动矩形的y坐标

    // Check bounds. This is in case the window is made
    // smaller while the rectangle is bouncing and the
    // rectangle suddenly finds itself outside the new
    // clipping volume
	if (x > (windowWidth - rsize + xstep))//当矩形到达窗的右边缘时，将矩形的x坐标设置为窗口的宽度减去矩形的宽度
        x = windowWidth - rsize - 1;
	else if (x < -(windowWidth + xstep))//当矩形到达 窗口的左边缘时，将矩形的x坐标设置为窗口的宽度加上一个步长，这样矩形就不会在窗口的左边缘消失
        x = -windowWidth - 1;

    if (y > (windowHeight + ystep))
        y = windowHeight - 1;
    else if (y < -(windowHeight - rsize + ystep))
        y = -windowHeight + rsize - 1;

    // Redraw the scene with new coordinates
	glutPostRedisplay();                                                    //重绘的目的是为了不断更新矩形的位置
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

    SetupRC();

    glutMainLoop();

    return 0;
}