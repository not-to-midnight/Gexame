#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "SOIL.h"

///�̻�
//1��2�ı䶯̬ģ��ǿ��
#define MAX_PARTICLES 200        	// �������ӵ��������Ϊ200
#define PI 3.14159f               	// ����Բ���ʵ�ֵ
#define RHO 50                    	// ���������ٶ���ؼ���ĳ���ֵ
#define LIFE 6.0f                 	// ���ӳ�ʼ�������ڵĻ���ֵ
#define TIME 0.01f                	// ʱ�䲽��������ģ������״̬��ʱ��ı仯
#define HISTORY_SIZE 5            	// ��ʷλ�õ�����
GLuint texture[1];                	// �������

float distance = 1;                  	// ��̬ģ���ļ��,������������͸���
float windowHeight, windowWidth;   	// ȫ�ֱ��������ڴ洢���ڵĸ߶ȺͿ��

float planecenterx = -100.0f;
float planecentery = -100.0f;        	// ������������

float planewidth = 50.0f;        	// ������
float planeheight = 50.0f;       	// ����߶�

typedef struct {                   	// �������ӽṹ��
    float life;                    	// ���ӵ���������
    float init_life;               	// ���ӵĳ�ʼ��������
    float speed_aging;             	// ���ӵ��ϻ��ٶ�
    float r;                       	// ������ɫ�ĺ�ɫ����
    float g;                       	// ������ɫ����ɫ����
    float b;                       	// ������ɫ����ɫ����
    float x;                       	// ������X���λ��
    float y;                       	// ������Y���λ��
    float z;                       	// ������Z���λ��
    float v_x;                     	// ������X����ٶ�
    float v_y;                     	// ������Y����ٶ�
    float v_z;                     	// ������Z����ٶ�
    float a_x;                     	// ������X��ļ��ٶ�
    float a_y;                     	// ������Y��ļ��ٶ�
    float a_z;                     	// ������Z��ļ��ٶ�
    float history_x[HISTORY_SIZE]; 	// ��ʷλ��
    float history_y[HISTORY_SIZE]; 	// ��ʷλ��
    float history_z[HISTORY_SIZE]; 	// ��ʷλ��
    int history_index;             	// ��ʷλ������
} particle;

particle particles[MAX_PARTICLES]; 	// �����������飬���ڴ洢�������ӵ���Ϣ



void ChangeSize(int w, int h);   	// �������������ڴ����ڴ�С��
void Update();                   	// �������������ڸ�������ϵͳ״̬
int InitPaticleSystem(void);     	// �������������ڳ�ʼ������ϵͳ
void SetupRC(void);              	// ��������������������Ⱦ״̬
void TimerFunction(int value);   	// �������������ڶ�ʱ���ص�����
int LoadGLTextures();           	// �������������ڼ�������
void RenderScene(void);          	// ����������������Ⱦ����
void InitGLUT(int argc, char** argv);// �������������ڳ�ʼ��GLUT
int InitGL();              	// �������������ڳ�ʼ��OpenGL
void detection(particle& a);
void recover(particle& a);


void drawplane(float centerx,float centery,float width,float height,float deepth)
{
    // ������ɫ
    glColor3f(0.9f, 0.9f, 0.9f);
    // ��ʼ�����ı���
    glBegin(GL_QUADS);
    // ��ά�ı��ζ������꣬z��������Ϊ0

	glVertex3f(centerx, centery , deepth);
	glVertex3f(centerx + width, centery, deepth);
    glVertex3f(centerx + width, centery + height, deepth);
	glVertex3f(centerx , centery+height, deepth);

	// ��������

    glEnd();
}






void ChangeSize(int w, int h) {    	// �����ڸı��Сʱ��GLUT��������õĻص�����
    float aspectRatio;             	// ���ڴ洢���ڿ�߱�
    if (h == 0) h = 1;             	// ��ֹ���ڸ߶�Ϊ0���³���������߶�Ϊ0����Ϊ1
    glViewport(0, 0, w, h);        	// �����ӿڴ�СΪ���ڵĴ�С
    glMatrixMode(GL_PROJECTION);   	// �л���ͶӰ����ģʽ
    glLoadIdentity();              	// ����ͶӰ����Ϊ��λ����
    aspectRatio = (float)w / (float)h; // ���㴰�ڵĿ�߱�
    if (w <= h) {                  	// ���ݴ��ڿ�߱���������ͶӰ����
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0
        ); // ��������ͶӰ�ļ�������
    }
    else {
        windowWidth = 100.0 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }
    glMatrixMode(GL_MODELVIEW);    	// �л���ģ����ͼ����ģʽ
    glLoadIdentity();              	// ����ģ����ͼ����Ϊ��λ����
}



int InitPaticleSystem(void) {      	// ��ʼ������ϵͳ�ĺ���
	float theta, phi, rho;// ���ڴ洢���ӵ��ٶȺ�λ��
    for (int i = 0; i < MAX_PARTICLES; i++) { // �����������飬��ʼ��ÿ������
        particles[i].init_life = LIFE + rand() % 1 ; // Ϊ�������ó�ʼ�������ڣ��ڻ���ֵ�ϼ���һ�����С��
        particles[i].life = particles[i].init_life; // ����ʼ�������ڸ�ֵ����ǰ��������
        particles[i].speed_aging = TIME; // �������ӵ��ϻ��ٶ�Ϊʱ�䲽��
        particles[i].r = 1.0f; // ����������ɫΪ��ɫ��r = 1.0f, g = 0.0f, b = 0.0f��
        particles[i].g = 0.0f;
        particles[i].b = 0.0f;
        particles[i].x = 0.0f; // �������ӵĳ�ʼλ��
        particles[i].y = 70.0f;
        particles[i].z = 0.0f;
        theta = (rand()%180 - 270) * PI / 360; // ������ɽǶ����ڼ����ٶ�
        phi = 90 * PI / 180;
        rho = rand() % RHO;
        particles[i].v_x = (float)(sinf(phi) * cosf(theta) * rho); // ����������X����ٶ�
        particles[i].v_y = (float)(sinf(phi) * sinf(theta) * rho); // ����������Y����ٶ�
        particles[i].v_z = (float)(cosf(phi) * rho); // ����������Z����ٶ�
        particles[i].a_x = 0.0f; // ����������X��ļ��ٶ�Ϊ0
        particles[i].a_y = -30.0f; // ����������Y��ļ��ٶ�Ϊ -30.0f��ģ������������Ч��
        particles[i].a_z = 0.0f; // ����������Z��ļ��ٶ�Ϊ0
        for (int j = 0; j < HISTORY_SIZE; j++) { // ��ʼ����ʷλ��
            particles[i].history_x[j] = particles[i].x;
            particles[i].history_y[j] = particles[i].y;
            particles[i].history_z[j] = particles[i].z;
        }
        particles[i].history_index = 0; // ��ʼ����ʷλ������
    }
    return true;
}


void detection(particle& a) {               	// ��⺯��
    if (a.x >= -80 && a.x <= 80 && a.y <= -70 && a.y >= -80)
    {
		//std::cout << "����ǰ" << a.v_y << std::endl;
        if (abs(a.v_y)-30>=0)
        a.v_y = -a.v_y / abs(a.v_y) * (abs(a.v_y) - 30);  // ����
        else
        {
			recover(a);
			//std::cout << "�ָ�" <<  std::endl;
        }

        
		//std::cout << "������" << a.v_y << std::endl;
    }

    if (a.x >= -80 && a.x <= -70 && a.y <= -30 && a.y >= -80) {
        if (a.x >= -80 && a.x <= -70) {
            a.v_x = -a.v_x; // ����
        }
		if (a.y <= -30 && a.y >= -80) {
			a.v_y = -a.v_y/abs(a.v_y)*(abs(a.v_y)-30); // ����
		}
        
    }
    if (a.x >= 80 && a.x <= 100 && a.y <= -30 && a.y >= -80)
    {
		if (a.x >= 80 && a.x <= 100) {
			a.v_x = -a.v_x; // ����
		}
        
        if (a.y <= -30 && a.y >= -80) {
            a.v_y = -a.v_y; // ����
        }
    }

}

void RenderScene(void) {           	// ��Ⱦ�����ĺ���
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����ɫ����������Ȼ�����
    glEnable(GL_TEXTURE_2D);       	// ���ö�ά����
    glBindTexture(GL_TEXTURE_2D, texture[0]); // ���Ѽ��ص�����

    drawplane(-80, -80, 10, 50,-1);//��
	drawplane(-80,-80 ,160 ,10,-1); // ��
	drawplane(80, -80, 20, 50,-1); // ��
    drawplane(80, -80, 5, 160,1); // 
    drawplane(0, 80, 90,5 ,1); // ���Ƶ���
    drawplane(-10, 70, 20, 10,1); // ���Ƶ���



    for (int i = 0; i < MAX_PARTICLES; i++) { // �����������飬����ÿ�����Ӽ�����ʷλ��
        float x = particles[i].x; // ��ȡ���ӵ�λ��
        float y = particles[i].y;
        float z = particles[i].z;
        float size = 1;
        float imga1;
        float alpha = 0.0;
        float step = 0.2;
        for (int j = 1; j <= HISTORY_SIZE; j++) { // ������ʷλ�ã�͸�������μ���
            int loact = (j + particles[i].history_index) % HISTORY_SIZE; // ���¶�λ��ʷλ������
            glPointSize(size); // ���õ��С
            size += 0.1f; // ��������ʷ���Ӵ�С
            imga1 = size; // ��������ӳ��ߴ�
            glBegin(GL_TRIANGLE_STRIP); // ���������δ������ı���
            alpha += step; // ������͸����
            // ������ɫ��͸���ȣ���ɫ��͸����
            glColor4f(0.0f, 0.0f, 1.0f, alpha);
            glTexCoord2d(1, 1); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] + imga1, z); // ���϶���
            glTexCoord2d(0, 1); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] + imga1, z); // ���϶���
            glTexCoord2d(1, 0); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] - imga1, z); // ���¶���
            glTexCoord2d(0, 0); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] - imga1, z); // ���¶���
            glEnd(); // ��������
        }
        glPointSize(1); // ���õ�ǰ���Ӵ�С
        glBegin(GL_TRIANGLE_STRIP); // ���������δ������ı���
        glTexCoord2d(1, 1); glVertex3f(x + imga1, y + imga1, z); // ���϶���
        glTexCoord2d(0, 1); glVertex3f(x - imga1, y + imga1, z); // ���϶���
        glTexCoord2d(1, 0); glVertex3f(x + imga1, y - imga1, z); // ���¶���
        glTexCoord2d(0, 0); glVertex3f(x - imga1, y - imga1, z); // ���¶���
        glEnd(); // ��������
    }
    glutSwapBuffers();             	// ��������������ʾ���ƽ��
}

void Update() {                    	// ��������ϵͳ״̬�ĺ���
    for (int i = 0; i < MAX_PARTICLES; i++) { // �����������飬����ÿ�����ӵ�״̬

        if (abs(particles[i].history_x[particles[i].history_index] - particles[i].x) >= distance) { // ���浱ǰλ�õ���ʷλ�ã���̬ģ�������жϣ�
            particles[i].history_index = (particles[i].history_index + 1) % HISTORY_SIZE; // ѭ��������ʷλ������
            particles[i].history_x[particles[i].history_index] = particles[i].x; // �洢��ǰX���굽��ʷ����
            particles[i].history_y[particles[i].history_index] = particles[i].y; // �洢��ǰY���굽��ʷ����
            particles[i].history_z[particles[i].history_index] = particles[i].z; // �洢��ǰZ���굽��ʷ����
        }

        particles[i].x += particles[i].v_x * TIME; // �����ٶȺ�ʱ�䲽���������ӵ�λ��
        particles[i].y += particles[i].v_y * TIME;
        //particles[i].z += particles[i].v_z * TIME;
        particles[i].v_x += particles[i].a_x * TIME; // ���ݼ��ٶȺ�ʱ�䲽���������ӵ��ٶ�
        particles[i].v_y += particles[i].a_y * TIME;
        //particles[i].v_z += particles[i].a_z * TIME;
        particles[i].life -= particles[i].speed_aging; // �����ϻ��ٶȼ������ӵ���������
		detection(particles[i]); // ���������������ײ

        if (particles[i].x >= 150 || particles[i].x <= -150 || particles[i].y <= -150 || particles[i].y >= 150)
        {
            recover(particles[i]);
        }
        //if (particles[i].life < 0.0f) 
        //if(particles[i].x>=150|| particles[i].x<=-150||particles[i].y<=-150||particles[i].y>=150)
        //{                                                                                                                          // ������ӵ���������С��0����ʾ�����Ѵﵽ�������ڽ���
        //    particles[i].init_life = LIFE + rand() % 10 / 10.0; // Ϊ���Ӹ����µ���������
        //    particles[i].life = particles[i].init_life;
        //    particles[i].x = 0.0f; // �������ӵ�λ��
        //    particles[i].y = 70.0f;
        //    particles[i].z = 0.0f;

        //    theta = (rand() % 180 - 270) * PI / 360; // ������ɽǶ����ڼ����ٶ�
        //    phi = 90 * PI / 180;
        //    rho = rand() % RHO;
        //    particles[i].v_x = (float)(sinf(phi) * cosf(theta) * rho); // ���³�ʼ��������X����ٶ�
        //    particles[i].v_y = (float)(sinf(phi) * sin(theta) * rho); // ���³�ʼ��������Y����ٶ�
        //    particles[i].v_z = (float)(cosf(phi) * rho); // ���³�ʼ��������Z����ٶ�

        //    for (int j = 0; j < HISTORY_SIZE; j++) { // ������ʷλ��
        //        particles[i].history_x[j] = particles[i].x;
        //        particles[i].history_y[j] = particles[i].y;
        //        particles[i].history_z[j] = particles[i].z;
        //    }
        //    particles[i].history_index = 0; // ������ʷλ������
        
    }
}

void recover(particle& a)  // �ָ�����״̬�ĺ���
{
    float theta, phi, rho;
                                                                                                                      // ������ӵ���������С��0����ʾ�����Ѵﵽ�������ڽ���
        a.init_life = LIFE + rand() % 10 / 10.0; // Ϊ���Ӹ����µ���������
        a.life = a.init_life;
        a.x = 0.0f; // �������ӵ�λ��
        a.y = 70.0f;
        a.z = 0.0f;

        theta = (rand() % 180 - 270) * PI / 360; // ������ɽǶ����ڼ����ٶ�
        phi = 90 * PI / 180;
        rho = rand() % RHO;
        a.v_x = (float)(sinf(phi) * cosf(theta) * rho); // ���³�ʼ��������X����ٶ�
        a.v_y = (float)(sinf(phi) * sin(theta) * rho); // ���³�ʼ��������Y����ٶ�
        a.v_z = (float)(cosf(phi) * rho); // ���³�ʼ��������Z����ٶ�

        for (int j = 0; j < HISTORY_SIZE; j++) { // ������ʷλ��
            a.history_x[j] = a.x;
            a.history_y[j] = a.y;
            a.history_z[j] = a.z;
        }
        a.history_index = 0; // ������ʷλ������
    
}

void TimerFunction(int value) {    	// ��ʱ���ص�����
    Update(); // ��������ϵͳ״̬
    glutPostRedisplay(); // ��Ǵ�����Ҫ���»��ƣ�����RenderScene����ִ��
    glutTimerFunc(10, TimerFunction, 1); // �ٴ����ö�ʱ����ÿ��10�������һ������
}

void SetupRC(void) {               	// ������Ⱦ״̬�ĺ���
    glEnable(GL_BLEND); // ����OpenGL�Ļ�Ϲ���
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ���û�Ϻ�����ʵ�ֻ���͸���ȵ���ɫ���Ч��
}

int LoadGLTextures() {             	// ����������
    texture[0] = SOIL_load_OGL_texture( // ���������ļ�
        "water.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    if (texture[0] == 0) return false; // �������ʧ�ܷ���false
    glBindTexture(GL_TEXTURE_2D, texture[0]); // ������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // ��������Ŵ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ����������С����
    return true; // ������سɹ�����true
}

int InitGL() {                     	// ��ʼ��OpenGL����
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ����������ɫΪ��ɫ
    glClearDepth(1.0f);            	// ������Ȼ��������ֵ
    glEnable(GL_DEPTH_TEST);       	// ������Ȳ���
    glDepthFunc(GL_LEQUAL);        	// ������Ȳ��Ժ���

    std::cout << "�������" << std::endl; // ������������ʾ
    if (!LoadGLTextures()) return false; // ��������ʧ�ܷ���false
    else {
        std::cout << "������سɹ�" << std::endl; // ������سɹ���ʾ
        glEnable(GL_BLEND);        	// ���û�Ϲ���
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ���û�Ϻ���
    }
    return true; // ��ʼ���ɹ�����true
}

void InitGLUT(int argc, char** argv) { // ��ʼ��GLUT����
	glutInit(&argc, argv);          	// ��ʼ��GLUT��
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ������ʾģʽ
	glutInitWindowSize(640, 480);   	// ���ô��ڴ�С
	glutCreateWindow("FireWorks"); // �������ڲ����ñ���
}

void Keyboard(unsigned char key, int x, int y)
{
    if (key == '1') {
   
            distance += 0.1;
            std::cout << distance << std::endl;
		}
    if (key == '2') {

        distance -= 0.1;
        std::cout << distance << std::endl;
    }


        glutPostRedisplay();
    
}

int main(int argc, char** argv) {  	// ������

	srand(time(NULL));          	// �������������
 
	InitGLUT(argc, argv);          	// ��ʼ��GLUT

    glutDisplayFunc(RenderScene);  	// ע����ʾ�ص�����
    glutReshapeFunc(ChangeSize);   	// ע�ᴰ�ڴ�С�ı�ص�����
    glutTimerFunc(10, TimerFunction, 1); // ע�ᶨʱ���ص�����

    InitGL();                      	// ��ʼ��OpenGL����
    SetupRC();                     	// ������Ⱦ״̬
    InitPaticleSystem();           	// ��ʼ������ϵͳ
	glutKeyboardFunc(Keyboard);    	// ע���������ص�����

    glutMainLoop();                	// ����GLUT�¼�����ѭ��

    return 0; // ���������˳�����0
}