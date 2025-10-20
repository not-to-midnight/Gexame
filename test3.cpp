#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<cmath>
#include"SOIL.h"

// �������ӵ��������Ϊ200
#define	MAX_PARTICLES	200        	
// ����Բ���ʵ�ֵ
#define PI              3.14159f
// ���������ٶ���ؼ���ĳ���ֵ
#define RHO             50
// ���ӳ�ʼ�������ڵĻ���ֵ
#define LIFE            3.0f
// ʱ�䲽��������ģ������״̬��ʱ��ı仯
#define TIME            0.01f
// ��ʷλ�õ�����
#define HISTORY_SIZE    5
GLuint texture[1];

//��̬ģ���ļ��,������������͸���
int distance = 1;

// ȫ�ֱ��������ڴ洢���ڵĸ߶ȺͿ��
float windowHeight, windowWidth;


// �������ӽṹ��
typedef struct {
    // ���ӵ���������
    float	life;
    // ���ӵĳ�ʼ��������
    float   init_life;
    // ���ӵ��ϻ��ٶ�
    float	speed_aging;

    // ������ɫ�ĺ�ɫ����
    float	r;
    // ������ɫ����ɫ����
    float	g;
    // ������ɫ����ɫ����
    float	b;

    // ������X���λ��
    float	x;
    // ������Y���λ��
    float	y;
    // ������Z���λ��
    float	z;

    // ������X����ٶ�
    float	v_x;
    // ������Y����ٶ�
    float	v_y;
    // ������Z����ٶ�
    float	v_z;

    // ������X��ļ��ٶ�
    float	a_x;
    // ������Y��ļ��ٶ�
    float	a_y;
    // ������Z��ļ��ٶ�
    float	a_z;

    // ��ʷλ��
    float history_x[HISTORY_SIZE];
    float history_y[HISTORY_SIZE];
    float history_z[HISTORY_SIZE];
    int history_index;
} particle;

// �����������飬���ڴ洢�������ӵ���Ϣ
particle particles[MAX_PARTICLES];

// �����ڸı��Сʱ��GLUT��������õĻص�����
void ChangeSize(int w, int h) {
    // ���ڴ洢���ڿ�߱�
    float aspectRatio;

    // ��ֹ���ڸ߶�Ϊ0���³���������߶�Ϊ0����Ϊ1
    if (h == 0) h = 1;

    // �����ӿڴ�СΪ���ڵĴ�С
    glViewport(0, 0, w, h);

    // �л���ͶӰ����ģʽ
    glMatrixMode(GL_PROJECTION);
    // ����ͶӰ����Ϊ��λ����
    glLoadIdentity();

    // ���㴰�ڵĿ�߱�
    aspectRatio = (float)w / (float)h;

    // ���ݴ��ڿ�߱���������ͶӰ����
    if (w <= h) {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        // ��������ͶӰ�ļ�������
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    }
    else {
        windowWidth = 100.0 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }

    // �л���ģ����ͼ����ģʽ
    glMatrixMode(GL_MODELVIEW);
    // ����ģ����ͼ����Ϊ��λ����
    glLoadIdentity();
}

// ��ʼ������ϵͳ�ĺ���
int InitPaticleSystem(void) {
    float theta, phi, rho;

    // �����������飬��ʼ��ÿ������
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // Ϊ�������ó�ʼ�������ڣ��ڻ���ֵ�ϼ���һ�����С��
        particles[i].init_life = LIFE + rand() % 10 / 10.0;

        // ����ʼ�������ڸ�ֵ����ǰ��������
        particles[i].life = particles[i].init_life;

        // �������ӵ��ϻ��ٶ�Ϊʱ�䲽��
        particles[i].speed_aging = TIME;

        // ����������ɫΪ��ɫ��r = 1.0f, g = 0.0f, b = 0.0f��
        particles[i].r = 1.0f;
        particles[i].g = 0.0f;
        particles[i].b = 0.0f;

        // �������ӵĳ�ʼλ��
        particles[i].x = 0.0f;
        particles[i].y = 50.0f;
        particles[i].z = 0.0f;

        // ������ɽǶ����ڼ����ٶ�
        theta = (rand() % 90 + 45) * PI / 180;
        phi = 90 * PI / 180;
        rho = rand() % RHO;
        // ����������X����ٶ�
        particles[i].v_x = float(sinf(phi) * cosf(theta) * rho);
        // ����������Y����ٶ�
        particles[i].v_y = float(sinf(phi) * sin(theta) * rho);
        // ����������Z����ٶ�
        particles[i].v_z = float(cosf(phi) * rho);

        // ����������X��ļ��ٶ�Ϊ0
        particles[i].a_x = 0.0f;
        // ����������Y��ļ��ٶ�Ϊ -30.0f��ģ������������Ч��
        particles[i].a_y = -30.0f;
        // ����������Z��ļ��ٶ�Ϊ0
        particles[i].a_z = 0.0f;

        // ��ʼ����ʷλ��
        for (int j = 0; j < HISTORY_SIZE; j++) {
            particles[i].history_x[j] = particles[i].x;
            particles[i].history_y[j] = particles[i].y;
            particles[i].history_z[j] = particles[i].z;
        }
        particles[i].history_index = 0;
    }

    return true;
}

// ��Ⱦ�����ĺ���
void RenderScene(void) {
    // �����ɫ����������Ȼ�����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D); // ���ö�ά����
    glBindTexture(GL_TEXTURE_2D, texture[0]); // ���Ѽ��ص�����

    // �����������飬����ÿ�����Ӽ�����ʷλ��
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // ��ȡ���ӵ�λ��
        float x = particles[i].x;
        float y = particles[i].y;
        float z = particles[i].z;

        float size = 1;
        float imga1;
        float alpha = 0.0;
        float step = 0.2;


         //������ʷλ�ã�͸�������μ���
        for (int j = 1; j <=HISTORY_SIZE; j++) {
            int loact = (j + particles[i].history_index) % HISTORY_SIZE;//���¶�λ
            // ����͸���ȼ��㷽ʽ������alpha����
            glPointSize(size);
            size = size + 0.1f;
			// ������ʷλ��
            imga1 = size ;
            glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
  /*          glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].life);*/
            alpha += step;
            glColor4f(1.0f, 1.0f,1.0f, alpha); // ������ɫ��͸����
            glTexCoord2d(1, 1); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] + imga1, z); // Top Right
            glTexCoord2d(0, 1); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] + imga1, z); // Top Left
            glTexCoord2d(1, 0); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] - imga1, z); // Bottom Right
            glTexCoord2d(0, 0); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] - imga1, z); // Bottom Left
            //glBegin(GL_POINTS);

            //// ָ�����ӵĶ���λ��
            //glVertex3f(particles[i].history_x[loact], particles[i].history_y[loact], particles[i].history_z[loact]);
            glEnd();

        }

        // ���Ƶ�ǰλ��

        glPointSize(1);
        glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
        //glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].life);
        glTexCoord2d(1, 1); glVertex3f(x +imga1, y + imga1,z); // Top Right
        glTexCoord2d(0, 1); glVertex3f(x - imga1, y + imga1, z); // Top Left
        glTexCoord2d(1, 0); glVertex3f(x + imga1, y - imga1,z); // Bottom Right
        glTexCoord2d(0, 0); glVertex3f(x - imga1, y - imga1, z); // Bottom Left
        //glBegin(GL_POINTS);

        //// ָ�����ӵĶ���λ��
        //glVertex3f(x, y, z);
        glEnd();

    }

    //glDisable(GL_TEXTURE_2D); // ������ɺ󣬽�������

    // ��������������ʾ���ƽ��
    glutSwapBuffers();
}

// ��������ϵͳ״̬�ĺ���
void Update() {
    // �����������飬����ÿ�����ӵ�״̬
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // ���浱ǰλ�õ���ʷλ��
        if (abs(particles[i].history_x[particles[i].history_index] - particles[i].x) >= distance) {
            particles[i].history_index = (particles[i].history_index + 1) % HISTORY_SIZE;//ѭ��������ʷλ����������1���µ�5,index�Ǽ����ĸ�λ�þ����������ʷλ��
            particles[i].history_x[particles[i].history_index] = particles[i].x;
            particles[i].history_y[particles[i].history_index] = particles[i].y;
            particles[i].history_z[particles[i].history_index] = particles[i].z;
            
        }

        // �����ٶȺ�ʱ�䲽���������ӵ�λ��
        particles[i].x += particles[i].v_x * TIME;
        particles[i].y += particles[i].v_y * TIME;
        particles[i].z += particles[i].v_z * TIME;

        // ���ݼ��ٶȺ�ʱ�䲽���������ӵ��ٶ�
        particles[i].v_x += particles[i].a_x * TIME;
        particles[i].v_y += particles[i].a_y * TIME;
        particles[i].v_z += particles[i].a_z * TIME;

        // �����ϻ��ٶȼ������ӵ���������
        particles[i].life -= particles[i].speed_aging;

        float theta, phi, rho;
        // ������ӵ���������С��0����ʾ�����Ѵﵽ�������ڽ���
        if (particles[i].life < 0.0f) {
            // Ϊ���Ӹ����µ���������
            particles[i].init_life = LIFE + rand() % 10 / 10.0;
            particles[i].life = particles[i].init_life;

            // �������ӵ�λ��
            particles[i].x = 0.0f;
            particles[i].y = 50.0f;
            particles[i].z = 0.0f;

            // ����������ɽǶ����ڼ����ٶ�
            theta = (rand() % 90 + 45) * PI / 180;
            phi = 90 * PI / 180;
            rho = rand() % RHO;
            // ���³�ʼ��������X����ٶ�
            particles[i].v_x = float(sinf(phi) * cosf(theta) * rho);
            // ���³�ʼ��������Y����ٶ�
            particles[i].v_y = float(sinf(phi) * sin(theta) * rho);
            // ���³�ʼ��������Z����ٶ�
            particles[i].v_z = float(cosf(phi) * rho);

            // ������ʷλ��
            for (int j = 0; j < HISTORY_SIZE; j++) {
                particles[i].history_x[j] = particles[i].x;
                particles[i].history_y[j] = particles[i].y;
                particles[i].history_z[j] = particles[i].z;
            }
            particles[i].history_index = 0;
        }
    }
}

// ��ʱ���ص�����
void TimerFunction(int value) {
    // ��������ϵͳ״̬
    Update();
    // ��Ǵ�����Ҫ���»��ƣ�����RenderScene����ִ��
    glutPostRedisplay();
    // �ٴ����ö�ʱ����ÿ��10�������һ������
    glutTimerFunc(10, TimerFunction, 1);
}

// ������Ⱦ״̬�ĺ���
void SetupRC(void) {
    // ����OpenGL�Ļ�Ϲ���
    glEnable(GL_BLEND);

    // ���û�Ϻ�����ʵ�ֻ���͸���ȵ���ɫ���Ч��
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



// ����������
int LoadGLTextures()
{
    // load an image file directly as a new OpenGL texture
    texture[0] = SOIL_load_OGL_texture
    (
        "test.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (texture[0] == 0)
        return false;

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    return true;
}

int InitGL()
{


    // ����������ɫΪ��ɫ
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // ������Ȼ����������ֵΪ1.0
    glClearDepth(1.0f);
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    // ������Ȳ��Ե�����ΪGL_LEQUAL
    glDepthFunc(GL_LEQUAL);
    // �����ӿڴ�СΪ���ڵĴ�С
    glViewport(0, 0, windowWidth, windowHeight);
    // ����ͶӰ����ģʽ
    glMatrixMode(GL_PROJECTION);
    // ����ͶӰ����Ϊ��λ����
    glLoadIdentity();
    // ����͸��ͶӰ����
    gluPerspective(45.0f, windowWidth / windowHeight, 1.0f, 100.0f);
    // �л���ģ����ͼ����ģʽ
    glMatrixMode(GL_MODELVIEW);
    
    std::cout << "�������" << std::endl;
    if (!LoadGLTextures())
    {
        return false;
    }
    else {
		std::cout << "������سɹ�" << std::endl;
        // ����OpenGL�Ļ�Ϲ���
        glEnable(GL_BLEND);

        // ���û�Ϻ�����ʵ�ֻ���͸���ȵ���ɫ���Ч��
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    return true;
}

int main(int argc, char** argv) {

    // ��ʼ��GLUT��
    glutInit(&argc, argv);
    // ������ʾģʽΪ����Ȼ�������˫�����RGBA��ɫģʽ
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // ���ô��ڵĳ�ʼλ��
    glutInitWindowPosition(100, 100);

    // ���ô��ڵĳ�ʼ��С
    glutInitWindowSize(640, 640);

    // ��������Ϊ��Particle system���Ĵ���
    glutCreateWindow("FireWorks");

    // ע����ʾ�ص�����
    glutDisplayFunc(RenderScene);

    // ע�ᴰ�ڴ�С�ı�ص�����
    glutReshapeFunc(ChangeSize);

    // ע�ᶨʱ���ص�������ÿ��10�������һ��
    glutTimerFunc(10, TimerFunction, 1);

   
    InitGL();

    // ������Ⱦ״̬
    SetupRC();
    // ��ʼ������ϵͳ
    InitPaticleSystem();

    // ����GLUT�¼�����ѭ��
    glutMainLoop();

    return 0;
}