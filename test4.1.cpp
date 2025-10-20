#include<iostream>
#include <glut.h>														// ����GLUT���ͷ�ļ�������OpenGL�Ĵ��ڹ�����¼�����
#include <stdio.h>
#include <stdlib.h>
#include "test4Physics1.h"														// �����Զ����Physics2.hͷ�ļ���������������ģ����ع���
#include "cube.h"

float camerax = 0.0f, cameray=0.0f,cameraz = 1.0f; // ���λ��
float lookx = 0.0f, looky = 0.0f, lookz = 0.0f; // �۲��λ��
float upx = 0.0f, upy = 0.5f, upz = 0.0f; // 
// ������ƶ��ٶȺ���ת������
float moveSpeed = 0.1f;
float rotateSpeed = 0.01f;
// ���������������ת���ƣ�
float yaw = 0.0f;      // ˮƽ��ת�Ƕ�
float pitch = 0.0f;    // ��ֱ��ת�Ƕ�
double sensitivity = 0.005f;  // ���������
int lastX = 400, lastY = 300;  // �����һλ��
bool firstMouse = true;  // �Ƿ��״β������
int windowWidth = 800, windowHeight = 600;
// �ƶ��ٶ�
double speed = 0.2f;
bool rightMouseDown = false;
float timef= 0.002f;
int nummass = 3;
int mutai = 2;



RopeSimulation* ropeSimulation = new RopeSimulation(
	80,						// 80�����ӣ�������
	0.05f,					// ÿ�����ӵ�����Ϊ50��
	500.0f,				// �����е��ɵĵ���ϵ��
	0.05f,					// �����е��ɵ���������
	0.2f,					// �����ڲ�Ħ��ϵ��
	Vector3D(0, -9.81f, 0)); // �������ٶ�

cubeSimulation* cube = new cubeSimulation(
	nummass,						// 10������
	0.05f,					// ÿ�����ӵ�����Ϊ50��
	2.0f,					// �����е��ɵ���������
	1000.0f,				// �����е��ɵĵ���ϵ��
	0.2f,					// �����ڲ�Ħ��ϵ��
	Vector3D(0, -9.8f, 0),
	Vector3D(0, 0, 0),
	Vector3D(0, 3, 0),
	Vector3D(0, 0, 0),
	Vector3D(0, 3, mutai *nummass),
	Vector3D(0, 0, 0),
	Vector3D(mutai *nummass, 3, 0),
	Vector3D(0, 0, 0),
	Vector3D(mutai *nummass, 3, mutai *nummass)

); // �������ٶ�

test* test1 = new test(
	0.01f,					// ÿ�����ӵ�����Ϊ50��
	1.0f,					// �����е��ɵ���������
	500.0f,				// �����е��ɵĵ���ϵ��
	0.2f,					// �����ڲ�Ħ��ϵ��
	Vector3D(0, -9.81f, 0),
	Vector3D(0, 0, 0),
	Vector3D(0, -1, 0)
); // �������ٶ�

//void renderText(float x, float y, float z, const char* text) {
//	glRasterPos3f(x, y, z);
//	for (const char* c = text; *c != '\0'; c++) {
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
//	}
//}

void drawrectangle(Vector3D a,Vector3D b,Vector3D c,Vector3D d) {
	glBegin(GL_QUADS);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glVertex3f(d.x, d.y, d.z);
	glEnd();
}


void drawtriangle(Vector3D a, Vector3D b, Vector3D c) {
	glBegin(GL_TRIANGLES);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glEnd();
}


void drawline(Vector3D a, Vector3D b) {
	glColor3f(1.0f, 0.0f, 0.0f); // ������ɫΪ
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glEnd();
}


void locatecube() {
		for (int i = 0; i < cube->numOfMasses-1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // ������ɫΪ��ɫ
				drawrectangle((cube->toIndex(0, i, j)).pos, (cube->toIndex(0, i, j + 1)).pos, (cube->toIndex(0, i + 1, j + 1)).pos, (cube->toIndex(0, i + 1, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // ������ɫΪ��ɫ
				drawrectangle((cube->toIndex(i, 0, j)).pos, (cube->toIndex(i, 0, j + 1)).pos, (cube->toIndex(i+1, 0, j + 1)).pos, (cube->toIndex(i+1, 0, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // ������ɫΪ��ɫ
				drawrectangle((cube->toIndex( i, j,0)).pos, (cube->toIndex( i, j + 1,0)).pos, (cube->toIndex( i + 1, j + 1,0)).pos, (cube->toIndex(i + 1, j,0)).pos);
			}
		}
	
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // ������ɫΪ��ɫ
				drawrectangle((cube->toIndex(cube->numOfMasses-1, i, j)).pos, (cube->toIndex(cube->numOfMasses-1, i, j + 1)).pos, (cube->toIndex(cube->numOfMasses-1, i + 1, j + 1)).pos, (cube->toIndex(cube->numOfMasses-1, i + 1, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // ������ɫΪ��ɫ
				drawrectangle((cube->toIndex(i, cube->numOfMasses - 1, j)).pos, (cube->toIndex(i, cube->numOfMasses - 1, j + 1)).pos, (cube->toIndex(i + 1, cube->numOfMasses - 1, j + 1)).pos, (cube->toIndex(i + 1, cube->numOfMasses - 1, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // ������ɫΪ��ɫ
				drawrectangle((cube->toIndex(i, j,cube->numOfMasses - 1)).pos, (cube->toIndex(i, j + 1, cube->numOfMasses - 1)).pos, (cube->toIndex(i + 1, j + 1,cube->numOfMasses - 1)).pos, (cube->toIndex( i + 1, j, cube->numOfMasses - 1)).pos);
			}
		}

}


void locateline() {
	for (int i = 0; i < cube->numOfMasses-1; i++)
	{
		drawline((cube->toIndex(0, 0, i)).pos, (cube->toIndex(0, 0, i + 1)).pos);
		drawline((cube->toIndex(( cube->numOfMasses-1), 0, i)).pos, (cube->toIndex((cube->numOfMasses - 1), 0, i + 1)).pos);
		drawline((cube->toIndex(0, (cube->numOfMasses - 1), i)).pos, (cube->toIndex(0, (cube->numOfMasses - 1), i + 1)).pos);
		drawline((cube->toIndex((cube->numOfMasses - 1), (cube->numOfMasses - 1), i)).pos, (cube->toIndex((cube->numOfMasses - 1), (cube->numOfMasses - 1), i + 1)).pos);
		drawline((cube->toIndex(0, i,0)).pos, (cube->toIndex(0,  i + 1,0)).pos);
		drawline((cube->toIndex(0, i, (cube->numOfMasses - 1))).pos, (cube->toIndex(0,  i + 1, (cube->numOfMasses - 1))).pos);
		drawline((cube->toIndex((cube->numOfMasses - 1), i,0)).pos, (cube->toIndex((cube->numOfMasses - 1), i + 1,0)).pos);
		drawline((cube->toIndex((cube->numOfMasses - 1), i, (cube->numOfMasses - 1))).pos, (cube->toIndex((cube->numOfMasses - 1), i + 1, (cube->numOfMasses - 1))).pos);
		drawline((cube->toIndex(i,0,0)).pos, (cube->toIndex(i + 1,0,0)).pos);
		drawline((cube->toIndex(i, (cube->numOfMasses - 1), (cube->numOfMasses - 1))).pos, (cube->toIndex(i + 1, (cube->numOfMasses - 1), (cube->numOfMasses - 1))).pos);
		drawline((cube->toIndex(i, (cube->numOfMasses - 1),0)).pos, (cube->toIndex( i + 1, (cube->numOfMasses - 1),0)).pos);
		drawline((cube->toIndex(i,0, (cube->numOfMasses - 1))).pos, (cube->toIndex( i + 1,0, (cube->numOfMasses - 1))).pos);
	}
}


void enableLighting() {
	// ����OpenGL����
	GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // ���ͻ�����ǿ��
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // ��Ӿ��淴���
	GLfloat LightPosition[] = { 2.0f, 2.0f, 2.0f, 1.0f };  // ������Դλ�õ�б�Ϸ�
	GLfloat red[] = { 0.9f, 0.0f, 0.0f, 1.0f };            // ��׼ȷ�ı�����
	GLenum face = GL_FRONT_AND_BACK;
	GLenum mode = GL_AMBIENT;

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // ���ò�͸������
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if (change) {
		glColorMaterial(face, mode);
		glEnable(GL_COLOR_MATERIAL);
	}
	else {
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);

		// ��Ӳ��ʵľ��淴������
		GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat matShininess[] = { 100.0f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	}

	// ���ù�Դ����
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);  // ��Ӿ��淴��
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	// ��ӹ�Դ˥��
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);

	glEnable(GL_LIGHT1);
}


int InitGL(GLvoid)										// ����OpenGL�����ö����������
{
	//ropeSimulation->getMass(ropeSimulation->numOfMasses - 1)->vel.z = 10.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);									// ���ú�ɫ����
	glClearDepth(1.0f);													// ������Ȼ�����
	glShadeModel(GL_SMOOTH);												// ѡ��ƽ����ɫ
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);						// ����͸�Ӽ���Ϊ�ȷ

	return true;										// ��ʼ���ɹ�
}


void updateCameraVectors() {
	// ����۲췽�������������ָ��۲�㣩
	lookx = camerax + cosf(yaw) * cosf(pitch);
	looky = cameray + sinf(pitch);
	lookz = cameraz + sinf(yaw) * cosf(pitch);

	// ���������������򻯰棬ʵ��Ӧ���п�����Ҫ�����ӵļ��㣩
	// ������������Ϸ�����Ϊ(0,1,0)
	upx = 0.0f;
	upy = 1.0f;
	upz = 0.0f;
}


void Deinitialize(void)													// �κ��û��Զ���ķ���ʼ�����������������
{
	ropeSimulation->release();												// �ͷ�ropeSimulation����
	delete(ropeSimulation);													// ɾ��ropeSimulation����
	ropeSimulation = NULL;
}


void changeSize(int w, int h) {

	// ��ֹ���ڸ߶�Ϊ��ʱ���ֳ������
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// ʹ��ͶӰ����
	glMatrixMode(GL_PROJECTION);

	// ���þ���
	glLoadIdentity();

	// ���ӿ�����Ϊ��������
	glViewport(0, 0, w, h);

	// ������ȷ��͸��
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// �ص�ģ����ͼ����
	glMatrixMode(GL_MODELVIEW);
}


void Update()											// ������ִ���˶�����
{
	cube->operate(timef);
	test1->operate(timef);
}

void drawscene()
{
	glColor3f(1.0f, 1.0f, 1.0f);							
	drawrectangle(Vector3D(-10, -5, -10), Vector3D(10, -5, -10), Vector3D(10, -5, 10), Vector3D(-10, -5, 10)); // ���Ƶ���
	//std::cout << "drawscene" << std::endl;
	locatecube();
	locateline();
	drawline(test1->masses[0].pos, test1->masses[1].pos);
	drawline(test1->masses[0].pos, test1->masses[3].pos);
	drawline(test1->masses[0].pos, test1->masses[2].pos);
	drawline(test1->masses[1].pos, test1->masses[1].pos+(test1->masses[1].force/ test1->masses[1].force.length()/3));
	drawline(test1->masses[2].pos, test1->masses[2].pos + (test1->masses[2].force / test1->masses[2].force.length() / 3));
	drawline(test1->masses[3].pos, test1->masses[3].pos + (test1->masses[3].force / test1->masses[3].force.length() / 3));
}

void renderScene(void) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();														// ����ģ����ͼ����

	// �����λ��������Z������40�״�
	// ������������ΪY����ʹ��+Xָ�򴰿��Ҳ࣬+Yָ�򴰿��Ϸ�
	gluLookAt(
		camerax, cameray, cameraz,	// ���λ��
		lookx, looky, lookz,		// �۲��λ��
		upx, upy, upz);			// ��������
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					// �����ɫ����������Ȼ�����
	drawscene();
	// ����
	Update();
	
	//Update();
	// ��ʼ��������
	//glColor3ub(255, 255, 0);												// ������ɫΪ��ɫ
	//for (int a = 0; a < ropeSimulation->numOfMasses - 1; ++a)
	//{
	//	Mass* mass1 = ropeSimulation->getMass(a);
	//	Vector3D* pos1 = &mass1->pos;
	//	Mass* mass2 = ropeSimulation->getMass(a + 1);
	//	Vector3D* pos2 = &mass2->pos;
	//	glLineWidth(4);      // �����߿�
	//	glBegin(GL_LINES);
	//	glVertex3f(pos1->x, pos1->y, pos1->z);
	//	glVertex3f(pos2->x, pos2->y, pos2->z);
	//	glEnd();
	//}
	// �������ӽ���

	glFlush();																// ˢ��GL��Ⱦ�ܵ�

	glutSwapBuffers();
}

void mouseButtonCallback(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			std::cout << "mouseButtonCallback" << std::endl;
			rightMouseDown = true;
			firstMouse = true;  // �������λ�ã���������
		}
		else {
			std::cout << "mouseButtonCallback2" << std::endl;
			rightMouseDown = false;
		}
	}
}

// ͳһ������ƶ�������
void mouseMovementHandler(int x, int y) {
	if (!rightMouseDown) return;  // ֻ���Ҽ�����ʱ����

	// ԭ������ƶ��߼�...
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
	if (!rightMouseDown) return;  // ֻ���Ҽ�����ʱ�����ƶ�
	mouseMovementHandler(x, y);
}

// ����ƶ��ص����а�ť����ʱ��
void activeMouseCallback(int x, int y) {
	if (!rightMouseDown) return;  // ֻ���Ҽ�����ʱ�����ƶ�
	mouseMovementHandler(x, y);
}

// ���̰����ص�����
void keyboardCallback(unsigned char key, int x, int y) {
	// �������ǰ����������һ����
	float forwardX = lookx - camerax;
	float forwardY = looky - cameray;
	float forwardZ = lookz - cameraz;
	float len = sqrtf(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
	forwardX /= len;
	forwardY /= len;
	forwardZ /= len;

	// �����������������ǰ���������������
	float rightX, rightY, rightZ;
	rightX = forwardY * upz - forwardZ * upy;
	rightY = forwardZ * upx - forwardX * upz;
	rightZ = forwardX * upy - forwardY * upx;
	len = sqrtf(rightX * rightX + rightY * rightY + rightZ * rightZ);
	rightX /= len;
	rightY /= len;
	rightZ /= len;

	// ���ݰ����������λ��
	switch (key) {
	case 'w':  // ǰ��
	case 'W':
		camerax += forwardX * speed;
		cameray += forwardY * speed;
		cameraz += forwardZ * speed;
		break;
	case 's':  // ����
	case 'S':
		camerax -= forwardX * speed;
		cameray -= forwardY * speed;
		cameraz -= forwardZ * speed;
		break;
	case 'a':  // ����
	case 'A':
		camerax -= rightX * speed;
		cameray -= rightY * speed;
		cameraz -= rightZ * speed;
		break;
	case 'd':  // ����
	case 'D':
		camerax += rightX * speed;
		cameray += rightY * speed;
		cameraz += rightZ * speed;
		break;
	case'q':
	case'Q':
		cameray += speed*0.5;
		break;
	case'e':
	case'E':
		cameray -= speed*0.5;
		break;

	case'1':
		speed += 0.01;
		std::cout <<"�ƶ��ٶ�:" << speed << std::endl;
		break;
	case'2':
		speed -= 0.01;
		std::cout <<"�ƶ��ٶ�:" << speed << std::endl;
		break;
	case'3':
		sensitivity += 0.0001;
		std::cout <<"���������:" << sensitivity << std::endl;
		break;
	case'4':
		sensitivity -= 0.0001;
		std::cout << "���������:" << sensitivity << std::endl;
		break;

	case'f':
		if (change) {
			change = false;
			log((int)change);
		}
		else {
			change = true;
			log((int)change);
		}
		break;

	case'5':
		muti += 0.05;
		log("�˻�");
		logfloat(muti);
		break;
	case	'6':
		muti -= 0.05;
		log("�˻�");
		logfloat(muti);
		break;
	case'c':
		timef += 0.0001;
		log("ʱ��");
		logfloat(timef);
		break;
	case'x':
		timef -= 0.0001;
		log("ʱ��");
		logfloat(timef);
	}

	// ���¹۲��λ�ã��������������Է���
	lookx = camerax + forwardX;
	looky = cameray + forwardY;
	lookz = cameraz + forwardZ;

	// ǿ���ػ�
	glutPostRedisplay();
}

void timerCallback(int value) {
	glutPostRedisplay();
	glutTimerFunc(16, timerCallback, 0);  // Լ60FPS
}

// ���ڴ�С�ı�ص�����
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
	// ��ʼ��GLUT����������
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Rope Simulation");

	// ע��ص�����
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);//�����������λ��

	if (!InitGL()) {
		printf("Initialization Failed.");
		return false;
	}

	enableLighting();

	glEnable(GL_DEPTH_TEST);

	// ��ʼ�����
	updateCameraVectors();

	// ���ûص�����
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutPassiveMotionFunc(passiveMouseCallback);  // �ް�ť����ʱ���ƶ�
	glutMotionFunc(activeMouseCallback);          // �а�ť����ʱ���ƶ�
	glutMouseFunc(mouseButtonCallback);           // ��갴��
	glutTimerFunc(0, timerCallback, 0);    // ��ʱ���ص�

	// ����GLUT�¼�����ѭ��
	glutMainLoop();

	return 0;
}

