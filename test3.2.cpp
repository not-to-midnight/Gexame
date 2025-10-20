#include <glut.h>
#include "SOIL.h"
#include <cmath>
#include <ctime>

#define MAX_PARTICLES 1000
#define WATER_TEXTURE "water.png"  // ��׼��һ��ˮ������ͼ

// ���ӽṹ��
struct Particle {
    float pos[3];     // λ�� (x,y,z)
    float speed[3];   // �ٶ�
    float life;       // ��������
    float fade;       // ˥���ٶ�
    float color[4];   // RGBA��ɫ
    float size;       // ���Ӵ�С
};

Particle particles[MAX_PARTICLES];
GLuint textureID;
float angle = 0.0f;

// �������
float cameraPos[3] = { 0, 5, 15 };
float lookAt[3] = { 0, 0, 0 };

void initParticles() {
    srand(time(NULL));
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].pos[0] = 0;  // ��ʼXλ�ü���
        particles[i].pos[1] = 10.0; // ��ʼYλ�ã��ٲ�������
        particles[i].pos[2] = 0;  // ��ʼZλ��

        // �����ʼ�ٶ�
        particles[i].speed[0] = (rand() % 100 - 50) / 200.0f; // X������΢���
        particles[i].speed[1] = -(rand() % 100) / 50.0f;    // Y��������
        particles[i].speed[2] = (rand() % 100 - 50) / 200.0f; // Z������΢���

        particles[i].life = 1.0f;
        particles[i].fade = (rand() % 100) / 1000.0f + 0.003f;
        particles[i].color[0] = 0.3f;  // R
        particles[i].color[1] = 0.5f;  // G
        particles[i].color[2] = 1.0f;  // B
        particles[i].color[3] = 0.7f;  // A
        particles[i].size = (rand() % 100) / 100.0f + 0.1f;
    }
}

void loadTexture() {
    textureID = SOIL_load_OGL_texture(
        WATER_TEXTURE,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void initGL() {
    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // �򵥹���
    GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat lightDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat lightPosition[] = { 5.0, 10.0, 5.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    loadTexture();
    initParticles();
}

void updateParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            // �������
            particles[i].speed[1] -= 0.098f * 0.2f;  // ����
            particles[i].pos[0] += particles[i].speed[0];
            particles[i].pos[1] += particles[i].speed[1];
            particles[i].pos[2] += particles[i].speed[2];

            // ��������˥��
            particles[i].life -= particles[i].fade;

            // ��ײ��⣨�򵥵��棩
            if (particles[i].pos[1] < 0) {
                particles[i].speed[1] = -particles[i].speed[1] * 0.3f;
                particles[i].pos[1] = 0;
            }

            // ��������������
            if (particles[i].life < 0) {
                particles[i].pos[0] = (rand() % 100 - 50) / 50.0f;
                particles[i].pos[1] = 10.0;
                particles[i].pos[2] = (rand() % 100 - 50) / 50.0f;

                particles[i].life = 1.0f;
                particles[i].speed[0] = (rand() % 100 - 50) / 200.0f;
                particles[i].speed[1] = -(rand() % 100) / 50.0f;
                particles[i].speed[2] = (rand() % 100 - 50) / 200.0f;
            }
        }
    }
}

void drawParticle(Particle p) {
    glPushMatrix();
    glTranslatef(p.pos[0], p.pos[1], p.pos[2]);

    // ʼ�����������Billboard������
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    // �Ƴ���ת����
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            modelview[i * 4 + j] = (i == j) ? 1.0f : 0.0f;

    glLoadMatrixf(modelview);

    glColor4fv(p.color);
    glBegin(GL_QUADS);
    glTexCoord2d(1, 1); glVertex3f(p.size, p.size, 0);
    glTexCoord2d(0, 1); glVertex3f(-p.size, p.size, 0);
    glTexCoord2d(0, 0); glVertex3f(-p.size, -p.size, 0);
    glTexCoord2d(1, 0); glVertex3f(p.size, -p.size, 0);
    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // �������
    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
        lookAt[0], lookAt[1], lookAt[2],
        0, 1, 0);

    // ���Ƶ���
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_QUADS);
    glVertex3f(-20, 0, -20);
    glVertex3f(-20, 0, 20);
    glVertex3f(20, 0, 20);
    glVertex3f(20, 0, -20);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    // ������������
    glBindTexture(GL_TEXTURE_2D, textureID);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0)
            drawParticle(particles[i]);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    // ���������
    float speed = 0.5f;
    switch (key) {
    case 'a': angle -= 0.1f; break;
    case 'd': angle += 0.1f; break;
    case 'w': cameraPos[2] -= speed; break;
    case 's': cameraPos[2] += speed; break;
    case 27: exit(0);
    }
    lookAt[0] = cameraPos[0] + sin(angle) * 10;
    lookAt[2] = cameraPos[2] - cos(angle) * 10;
    glutPostRedisplay();
}

void idle() {
    updateParticles();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Waterfall Particle System");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}