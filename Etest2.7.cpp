#include <glut.h>
#include <stdlib.h>
#include<iostream>
#include"SOIL.h"




//�����ʵ����巽��/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//�������ҿ���ת�٣�L���Ƶƹ�


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
    // �����ɫ����������Ȼ�����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ���ñ任����
    glLoadIdentity();
    // ��Z��ƽ��
    glTranslatef(0.0f, 0.0f, z);
    // ��X����ת
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    // ��Y����ת
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    // ������
    glBindTexture(GL_TEXTURE_2D, texture[filter]);//

    // ��ʼ�����ı���
    glBegin(GL_QUADS);
    // ǰ��
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // ����
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    // ����
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // ����
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    // ����
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // ����
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    // ����������
    glutSwapBuffers();

    // ������ת�Ƕ�
    xrot += xspeed;
    yrot += yspeed;
}

void processSpecialKeys(int key, int x, int y) {
    // ������µ������Ϸ�ҳ����PAGE_UP��
    if (key == GLUT_KEY_PAGE_UP) {
        // ��Z���������ƶ�������z��ֵ����ΪzΪ������һ�������൱������Ļ���ƶ���
        z -= 0.02f;
    }
    // ������µ������·�ҳ����PAGE_DOWN��
    if (key == GLUT_KEY_PAGE_DOWN) {
        // ��Z�Ḻ�����ƶ�������z��ֵ������Ļ���ƶ���
        z += 0.02f;
    }
    // ������µ������ϼ�ͷ����UP��
    if (key == GLUT_KEY_UP) {
        // ������X����ת�ٶ�xspeed��ֵ��ʹ������X����ת��������xspeedΪ�����൱�ڷ������ ��
        xspeed -= 0.01f;
    }
    // ������µ������¼�ͷ����DOWN��
    if (key == GLUT_KEY_DOWN) {
        // ������X����ת�ٶ�xspeed��ֵ��ʹ������X����ת���
        xspeed += 0.01f;
    }
    // ������µ��������ͷ����LEFT��
    if (key == GLUT_KEY_LEFT) {
        // ������Y����ת�ٶ�yspeed��ֵ��ʹ������Y����ת��������yspeedΪ�����൱�ڷ������ ��
        yspeed -= 0.01f;
    }
    // ������µ������Ҽ�ͷ����RIGHT��
    if (key == GLUT_KEY_RIGHT) {
        // ������Y����ת�ٶ�yspeed��ֵ��ʹ������Y����ת���
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
            std::cout << "����ʧ";
            return false;

        }

        
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (texture[1] == 0) {
            std::cout << "��ʧ";
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
    // ���ö�ά����ӳ��
    glEnable(GL_TEXTURE_2D);
    // ����ƽ����ɫģʽ
    glShadeModel(GL_SMOOTH);
    // ���������ɫΪ��ɫ��͸����0.5
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    // ������Ȼ��������ֵ
    glClearDepth(1.0f);
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    // ������Ȳ��Ժ���
    glDepthFunc(GL_LEQUAL);
    // ����͸��У����ʾ
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // ���û�����
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    // �����������
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    // ���ù�Դλ��
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    // ���ù�Դ1
    glEnable(GL_LIGHT1);

    return true;
}

// ������ͨ���̰�������
void processNormalKeys(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        // Esc�����º��˳�����
        exit(0);
        break;
    case 'l':
        // �л���Դ����״̬
        light = !light;
        light ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
        break;
    case 'f':
        // ѡ���˲���ʽ
        filter += 1;
        if (filter > 1) filter = 0;
        printf("%d", filter);
        break;
    }
}

// ������
int main(int argc, char** argv) {
    // ��ʼ��GLUT��
    glutInit(&argc, argv);
    // ������ʾģʽ��������Ȳ��ԡ�˫�����RGBA��ɫģʽ
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    // ���ô��ڳ�ʼλ��
    glutInitWindowPosition(100, 100);
    // ���ô��ڳ�ʼ��С
    glutInitWindowSize(640, 640);
    // �������ڲ����ñ���
    glutCreateWindow("Texture");

    // ע��ص�����
	glutDisplayFunc(renderScene); // ע����ʾ�ص�����
	glutIdleFunc(renderScene); // �����ڿ���ʱ����renderScene����
    glutReshapeFunc(changeSize); // �˴�changeSize����δ�������壬�����в���

	// ע����̺�������ص�����
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys); // �˴�processSpecialKeys����δ�������壬�����в���

    if (!InitGL()) {
        printf("Initialization Failed.");
        return 1;
    }

    // ����GLUT�¼�����ѭ��
    glutMainLoop();

    return 0;
}