#include <glut.h>
#include <iostream>
using namespace std;

GLfloat red = 1.0, green = 1.0, blue = 1.0;//��ɫ����
GLfloat angle = 0;//��ת�Ƕ�
GLfloat a = 2.0;
void renderscene()
{
	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
	glPushMatrix();//�������Ŀ����Ϊ�˲�Ӱ����ͼ�εĻ���
	glRotatef(angle, 0.0, 0.0, 1.0);//��ת�Ƕ�
	glColor3f(red,green,blue);//���û�ͼ��ɫ

	glBegin(GL_TRIANGLES);//����������
	glVertex3f(-1.0, -1.0, 0.0);//���������ε��������㣬��Χ��-1��1֮��
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();//��������
	//glFlush();//ǿ��ˢ�»�����
	glPopMatrix();//�ָ�����Ŀ����Ϊ�˲�Ӱ����ͼ�εĻ���
	glutSwapBuffers();//����������,Ŀ���Ǳ�����˸,��˸����Ϊ���Ƶ�ͼ���ڻ������У���������ɺ󣬽��������е�ͼ����ʾ����Ļ�ϣ������������ʱ��ģ����Ի������˸
}

void setupRC(void)
{
	glClearColor(0.6f, 0.4f, 0.7f, 1.0f);//����������ɫ
	glColor3f(1.0f, 0.0f, 0.0f);//���û�ͼ��ɫ
}

void changesize(int w, int h)//�ı䴰�ڴ�Сʱ���õĺ���
{
	GLfloat aspectRatio;//�����߱�
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);//�����ӿ�
	glMatrixMode(GL_PROJECTION);//���þ���ģʽΪͶӰģʽ
	glLoadIdentity();//����ͶӰ����

	aspectRatio = (GLfloat)w / (GLfloat)h;//���㴰�ڵĿ�߱ȣ������ü�����
	if (w <= h)
		glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, 1.0, -1.0);//��������ͶӰ
	else
		glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, 1.0, -1.0);//��������ͶӰ�����������ֱ�Ϊ���ҡ��¡��ϡ�����Զ�ü����λ��
	glMatrixMode(GL_MODELVIEW);//���þ���ģʽΪģ����ͼģʽ
	glLoadIdentity();//����ģ����ͼ����
}

//void changesize(int w, int h)
//{
//
//	if (h == 0)
//		h = 1;
//	float ratio = 1.0 * w / h;//���㴰�ڵĿ�߱�
//	glMatrixMode(GL_PROJECTION);//���þ���ģʽΪͶӰģʽ
//	glLoadIdentity();//����ͶӰ����
//	glViewport(0, 0, w, h);//�����ӿ�
//	gluPerspective(45, ratio, 1, 1000);//����͸��ͶӰ
//	glMatrixMode(GL_MODELVIEW);//���þ���ģʽΪģ����ͼģʽ
//	glLoadIdentity();//����ģ����ͼ����
//	gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);//�����ӵ㡢�۲�㡢�Ϸ���
//
//}

void timerfunction(int value)
{
	angle =angle+a;//ÿ����ת2�ȣ�Ч����ͼ�񲻶���ת
	glutPostRedisplay();//�ػ��Ŀ����Ϊ�˲��ϸ�����ת�Ƕ�
	glutTimerFunc(23, timerfunction, 1);//���ö�ʱ��
}

void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)//����ESC�����˳�����
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
	glutInit(&argc, argv);//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//������ʾģʽ,������λ���룬GLUT_DOUBLE��ʾ˫���壬GLUT_RGB��ʾʹ��RGB��ɫģʽ����������ģʽ��GLUT_SINGLE��ʾ�����壬GLUT_INDEX��ʾʹ��������ɫģʽ
	glutInitWindowPosition(100, 100);//���ô���λ��
	glutInitWindowSize(400, 400);//���ô��ڴ�С
	glutCreateWindow("Hello, World!"); //�������ڲ����ô��ڱ���Ϊ"Hello, World!"


	//	register callbacks
	glutDisplayFunc(renderscene);//��ʾ�ص�����������reherscene����
	glutReshapeFunc(changesize);//��ⴰ�ڴ�С�仯
	glutTimerFunc(23, timerfunction, 1);//���ö�ʱ����ÿ23�������һ��timerfunction����

	glutKeyboardFunc(processNormalKeys);//���̻ص�����������processNormalKeys����
	glutSpecialFunc(processSpecialKeys);//������̻ص�����������processSpecialKeys����

	setupRC();//��ʼ����Ⱦ����

	glutMainLoop();//������ѭ��
	return 0;
}