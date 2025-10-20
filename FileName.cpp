#include <glut.h>
#include <iostream>
using namespace std;

GLfloat red = 1.0, green = 1.0, blue = 1.0;//颜色变量
GLfloat angle = 0;//旋转角度
GLfloat a = 2.0;
void renderscene()
{
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲区
	glPushMatrix();//保存矩阵，目的是为了不影响其图形的绘制
	glRotatef(angle, 0.0, 0.0, 1.0);//旋转角度
	glColor3f(red,green,blue);//设置绘图颜色

	glBegin(GL_TRIANGLES);//绘制三角形
	glVertex3f(-1.0, -1.0, 0.0);//定义三角形的三个顶点，范围在-1到1之间
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();//结束绘制
	//glFlush();//强制刷新缓冲区
	glPopMatrix();//恢复矩阵，目的是为了不影响其图形的绘制
	glutSwapBuffers();//交换缓冲区,目的是避免闪烁,闪烁是因为绘制的图形在缓冲区中，当绘制完成后，将缓冲区中的图形显示到屏幕上，这个过程是有时间的，所以会出现闪烁
}

void setupRC(void)
{
	glClearColor(0.6f, 0.4f, 0.7f, 1.0f);//设置清屏颜色
	glColor3f(1.0f, 0.0f, 0.0f);//设置绘图颜色
}

void changesize(int w, int h)//改变窗口大小时调用的函数
{
	GLfloat aspectRatio;//定义宽高比
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);//设置视口
	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影模式
	glLoadIdentity();//重置投影矩阵

	aspectRatio = (GLfloat)w / (GLfloat)h;//计算窗口的宽高比（建立裁剪区域）
	if (w <= h)
		glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, 1.0, -1.0);//设置正交投影
	else
		glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, 1.0, -1.0);//设置正交投影，六个参数分别为左、右、下、上、近、远裁剪面的位置
	glMatrixMode(GL_MODELVIEW);//设置矩阵模式为模型视图模式
	glLoadIdentity();//重置模型视图矩阵
}

//void changesize(int w, int h)
//{
//
//	if (h == 0)
//		h = 1;
//	float ratio = 1.0 * w / h;//计算窗口的宽高比
//	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影模式
//	glLoadIdentity();//重置投影矩阵
//	glViewport(0, 0, w, h);//设置视口
//	gluPerspective(45, ratio, 1, 1000);//设置透视投影
//	glMatrixMode(GL_MODELVIEW);//设置矩阵模式为模型视图模式
//	glLoadIdentity();//重置模型视图矩阵
//	gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);//设置视点、观察点、上方向
//
//}

void timerfunction(int value)
{
	angle =angle+a;//每次旋转2度，效果是图像不断旋转
	glutPostRedisplay();//重绘的目的是为了不断更新旋转角度
	glutTimerFunc(23, timerfunction, 1);//设置定时器
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
		a += 0.1;
		break;
	case GLUT_KEY_DOWN:
		a -= 0.1;
		break;
	}
}

int main(int argc,char **argv) {
//	init GLUT and create window
	glutInit(&argc, argv);//初始化GLUT库
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//设置显示模式,参数是位掩码，GLUT_DOUBLE表示双缓冲，GLUT_RGB表示使用RGB颜色模式，还有其他模式，GLUT_SINGLE表示单缓冲，GLUT_INDEX表示使用索引颜色模式
	glutInitWindowPosition(100, 100);//设置窗口位置
	glutInitWindowSize(400, 400);//设置窗口大小
	glutCreateWindow("Hello, World!"); //创建窗口并设置窗口标题为"Hello, World!"


	//	register callbacks
	glutDisplayFunc(renderscene);//显示回调函数，传入reherscene函数
	glutReshapeFunc(changesize);//监测窗口大小变化
	glutTimerFunc(23, timerfunction, 1);//设置定时器，每23毫秒调用一次timerfunction函数

	glutKeyboardFunc(processNormalKeys);//键盘回调函数，传入processNormalKeys函数
	glutSpecialFunc(processSpecialKeys);//特殊键盘回调函数，传入processSpecialKeys函数

	setupRC();//初始化渲染环境

	glutMainLoop();//进入主循环
	return 0;
}