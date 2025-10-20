#include<iostream>
#include <glut.h>														// 包含GLUT库的头文件，用于OpenGL的窗口管理和事件处理
#include <stdio.h>
#include <stdlib.h>
#include "test4Physics1.h"														// 包含自定义的Physics2.h头文件，可能用于物理模拟相关功能
#include "cube.h"

float camerax = 0.0f, cameray=0.0f,cameraz = 1.0f; // 相机位置
float lookx = 0.0f, looky = 0.0f, lookz = 0.0f; // 观察点位置
float upx = 0.0f, upy = 0.5f, upz = 0.0f; // 
// 摄像机移动速度和旋转灵敏度
float moveSpeed = 0.1f;
float rotateSpeed = 0.01f;
// 摄像机朝向（用于旋转控制）
float yaw = 0.0f;      // 水平旋转角度
float pitch = 0.0f;    // 垂直旋转角度
double sensitivity = 0.005f;  // 鼠标灵敏度
int lastX = 400, lastY = 300;  // 鼠标上一位置
bool firstMouse = true;  // 是否首次捕获鼠标
int windowWidth = 800, windowHeight = 600;
// 移动速度
double speed = 0.2f;
bool rightMouseDown = false;
float timef= 0.002f;
int nummass = 3;
int mutai = 2;



RopeSimulation* ropeSimulation = new RopeSimulation(
	80,						// 80个粒子（质量）
	0.05f,					// 每个粒子的质量为50克
	500.0f,				// 绳子中弹簧的弹性系数
	0.05f,					// 绳子中弹簧的正常长度
	0.2f,					// 弹簧内部摩擦系数
	Vector3D(0, -9.81f, 0)); // 重力加速度

cubeSimulation* cube = new cubeSimulation(
	nummass,						// 10个粒子
	0.05f,					// 每个粒子的质量为50克
	2.0f,					// 绳子中弹簧的正常长度
	1000.0f,				// 绳子中弹簧的弹性系数
	0.2f,					// 弹簧内部摩擦系数
	Vector3D(0, -9.8f, 0),
	Vector3D(0, 0, 0),
	Vector3D(0, 3, 0),
	Vector3D(0, 0, 0),
	Vector3D(0, 3, mutai *nummass),
	Vector3D(0, 0, 0),
	Vector3D(mutai *nummass, 3, 0),
	Vector3D(0, 0, 0),
	Vector3D(mutai *nummass, 3, mutai *nummass)

); // 重力加速度

test* test1 = new test(
	0.01f,					// 每个粒子的质量为50克
	1.0f,					// 绳子中弹簧的正常长度
	500.0f,				// 绳子中弹簧的弹性系数
	0.2f,					// 弹簧内部摩擦系数
	Vector3D(0, -9.81f, 0),
	Vector3D(0, 0, 0),
	Vector3D(0, -1, 0)
); // 重力加速度

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
	glColor3f(1.0f, 0.0f, 0.0f); // 设置颜色为
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
				glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
				drawrectangle((cube->toIndex(0, i, j)).pos, (cube->toIndex(0, i, j + 1)).pos, (cube->toIndex(0, i + 1, j + 1)).pos, (cube->toIndex(0, i + 1, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
				drawrectangle((cube->toIndex(i, 0, j)).pos, (cube->toIndex(i, 0, j + 1)).pos, (cube->toIndex(i+1, 0, j + 1)).pos, (cube->toIndex(i+1, 0, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
				drawrectangle((cube->toIndex( i, j,0)).pos, (cube->toIndex( i, j + 1,0)).pos, (cube->toIndex( i + 1, j + 1,0)).pos, (cube->toIndex(i + 1, j,0)).pos);
			}
		}
	
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
				drawrectangle((cube->toIndex(cube->numOfMasses-1, i, j)).pos, (cube->toIndex(cube->numOfMasses-1, i, j + 1)).pos, (cube->toIndex(cube->numOfMasses-1, i + 1, j + 1)).pos, (cube->toIndex(cube->numOfMasses-1, i + 1, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
				drawrectangle((cube->toIndex(i, cube->numOfMasses - 1, j)).pos, (cube->toIndex(i, cube->numOfMasses - 1, j + 1)).pos, (cube->toIndex(i + 1, cube->numOfMasses - 1, j + 1)).pos, (cube->toIndex(i + 1, cube->numOfMasses - 1, j)).pos);
			}
		}
		for (int i = 0; i < cube->numOfMasses - 1; i++) {
			for (int j = 0; j < cube->numOfMasses - 1; j++)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
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
	// 基础OpenGL设置
	GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // 降低环境光强度
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 添加镜面反射光
	GLfloat LightPosition[] = { 2.0f, 2.0f, 2.0f, 1.0f };  // 调整光源位置到斜上方
	GLfloat red[] = { 0.9f, 0.0f, 0.0f, 1.0f };            // 更准确的变量名
	GLenum face = GL_FRONT_AND_BACK;
	GLenum mode = GL_AMBIENT;

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // 设置不透明背景
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

		// 添加材质的镜面反射属性
		GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat matShininess[] = { 100.0f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	}

	// 设置光源参数
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);  // 添加镜面反射
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	// 添加光源衰减
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);

	glEnable(GL_LIGHT1);
}


int InitGL(GLvoid)										// 所有OpenGL的设置都在这里进行
{
	//ropeSimulation->getMass(ropeSimulation->numOfMasses - 1)->vel.z = 10.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);									// 设置黑色背景
	glClearDepth(1.0f);													// 设置深度缓冲区
	glShadeModel(GL_SMOOTH);												// 选择平滑着色
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);						// 设置透视计算为最精确

	return true;										// 初始化成功
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


void Deinitialize(void)													// 任何用户自定义的反初始化操作都在这里进行
{
	ropeSimulation->release();												// 释放ropeSimulation对象
	delete(ropeSimulation);													// 删除ropeSimulation对象
	ropeSimulation = NULL;
}


void changeSize(int w, int h) {

	// 防止窗口高度为零时出现除零错误
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// 使用投影矩阵
	glMatrixMode(GL_PROJECTION);

	// 重置矩阵
	glLoadIdentity();

	// 将视口设置为整个窗口
	glViewport(0, 0, w, h);

	// 设置正确的透视
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// 回到模型视图矩阵
	glMatrixMode(GL_MODELVIEW);
}


void Update()											// 在这里执行运动更新
{
	cube->operate(timef);
	test1->operate(timef);
}

void drawscene()
{
	glColor3f(1.0f, 1.0f, 1.0f);							
	drawrectangle(Vector3D(-10, -5, -10), Vector3D(10, -5, -10), Vector3D(10, -5, 10), Vector3D(-10, -5, 10)); // 绘制地面
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
	glLoadIdentity();														// 重置模型视图矩阵

	// 将相机位置设置在Z方向上40米处
	// 设置向上向量为Y方向，使得+X指向窗口右侧，+Y指向窗口上方
	gluLookAt(
		camerax, cameray, cameraz,	// 相机位置
		lookx, looky, lookz,		// 观察点位置
		upx, upy, upz);			// 向上向量
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					// 清除颜色缓冲区和深度缓冲区
	drawscene();
	// 更新
	Update();
	
	//Update();
	// 开始绘制绳子
	//glColor3ub(255, 255, 0);												// 设置颜色为黄色
	//for (int a = 0; a < ropeSimulation->numOfMasses - 1; ++a)
	//{
	//	Mass* mass1 = ropeSimulation->getMass(a);
	//	Vector3D* pos1 = &mass1->pos;
	//	Mass* mass2 = ropeSimulation->getMass(a + 1);
	//	Vector3D* pos2 = &mass2->pos;
	//	glLineWidth(4);      // 设置线宽
	//	glBegin(GL_LINES);
	//	glVertex3f(pos1->x, pos1->y, pos1->z);
	//	glVertex3f(pos2->x, pos2->y, pos2->z);
	//	glEnd();
	//}
	// 绘制绳子结束

	glFlush();																// 刷新GL渲染管道

	glutSwapBuffers();
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
		camerax += forwardX * speed;
		cameray += forwardY * speed;
		cameraz += forwardZ * speed;
		break;
	case 's':  // 后退
	case 'S':
		camerax -= forwardX * speed;
		cameray -= forwardY * speed;
		cameraz -= forwardZ * speed;
		break;
	case 'a':  // 左移
	case 'A':
		camerax -= rightX * speed;
		cameray -= rightY * speed;
		cameraz -= rightZ * speed;
		break;
	case 'd':  // 右移
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
		std::cout <<"移动速度:" << speed << std::endl;
		break;
	case'2':
		speed -= 0.01;
		std::cout <<"移动速度:" << speed << std::endl;
		break;
	case'3':
		sensitivity += 0.0001;
		std::cout <<"鼠标灵敏度:" << sensitivity << std::endl;
		break;
	case'4':
		sensitivity -= 0.0001;
		std::cout << "鼠标灵敏度:" << sensitivity << std::endl;
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
		log("乘积");
		logfloat(muti);
		break;
	case	'6':
		muti -= 0.05;
		log("乘积");
		logfloat(muti);
		break;
	case'c':
		timef += 0.0001;
		log("时间");
		logfloat(timef);
		break;
	case'x':
		timef -= 0.0001;
		log("时间");
		logfloat(timef);
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

// 窗口大小改变回调函数
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
	// 初始化GLUT并创建窗口
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Rope Simulation");

	// 注册回调函数
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);//更新摄像机的位置

	if (!InitGL()) {
		printf("Initialization Failed.");
		return false;
	}

	enableLighting();

	glEnable(GL_DEPTH_TEST);

	// 初始化相机
	updateCameraVectors();

	// 设置回调函数
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutPassiveMotionFunc(passiveMouseCallback);  // 无按钮按下时的移动
	glutMotionFunc(activeMouseCallback);          // 有按钮按下时的移动
	glutMouseFunc(mouseButtonCallback);           // 鼠标按键
	glutTimerFunc(0, timerCallback, 0);    // 定时器回调

	// 进入GLUT事件处理循环
	glutMainLoop();

	return 0;
}

