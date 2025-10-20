#include <glut.h>
#include <stdlib.h>
#include<iostream>

//�������ҿ���ת�٣�L���Ƶƹ�,change���ı���ʾ��ɫ�ķ�ʽ

//���л�������ʾ��ʽ�ķ���/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool change = true;
// ���ڱ�ʾ��X����ת�Ƕ�
GLfloat xrot;
// ���ڱ�ʾ��Y����ת�Ƕ�
GLfloat yrot;
// ���ڱ�ʾ��X����ת���ٶ�
GLfloat xspeed;
// ���ڱ�ʾ��Y����ת���ٶ�
GLfloat yspeed;
// ��ʾ����Ļ��ȷ���Z�᷽�򣩵�λ��
GLfloat z = -5.0f;
bool light = true;


GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
GLfloat gray[] = { 0.9f, 0.0f, 0.0f, 1.0f };
GLenum face = GL_FRONT_AND_BACK;
GLenum mode = GL_AMBIENT;

void changesize(int w, int h) {
	if (h == 0)h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 100);

	glMatrixMode(GL_MODELVIEW);

}


void renderscene() {
    // �����ɫ����������Ȼ�����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ���ñ任����Ϊ��λ����
    glLoadIdentity();
    // ��Z��ƽ�ƣ�ȷ����������Ļ��ȷ����λ��
    glTranslatef(0.0f, 0.0f, z);
    // ��X����ת����ת�Ƕ�Ϊxrot
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    // ��Y����ת����ת�Ƕ�Ϊyrot
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    // ������ɫΪ��ɫ��RGBAģʽ�����һ������1.0��ʾ͸����Ϊ��ȫ��͸����
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    // ����һ���߳�Ϊ1.0��ʵ��������
    glutSolidCube(1.0);
    // ����ǰ�󻺳�������ʾ���ƽ��
    glutSwapBuffers();
    // ������ת�ٶȸ�����X�����ת�Ƕ�
    xrot += xspeed;
    // ������ת�ٶȸ�����Y�����ת�Ƕ�
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
int InitGL(void)
{
    glEnable(GL_LIGHTING);
    // ������ɫģʽΪƽ����ɫ��Gouraud��ɫ����ʹͼ�α�����ɫ���ɸ���Ȼ
    glShadeModel(GL_SMOOTH);
    // ���������ɫΪ��ɫ��͸����Ϊ0.5
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    // ������Ȼ����������ֵΪ1.0
    glClearDepth(1.0f);
    // ������Ȳ��Թ��ܣ����ڴ�����ά�����������ǰ���ڵ���ϵ
    glEnable(GL_DEPTH_TEST);
    // ������Ȳ��Ժ�����GL_LEQUAL��ʾ���µ����ֵС�ڵ��ڻ������е����ֵʱ��ͨ������
    glDepthFunc(GL_LEQUAL);
    // ����͸��У����ʾ��GL_NICEST��ʾʹ�����������͸��У����ʽ
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    if (change) {
        glColorMaterial(face,  mode);
        glEnable(GL_COLOR_MATERIAL);
    }
    else {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
    }

    // ���ù�Դ1�Ļ��������
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    // ���ù�Դ1������������
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    // ���ù�Դ1��λ��
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    // ���ù�Դ1

    glEnable(GL_LIGHT1);

    return 1; // ����ԭ����return true��C���Ի����´���C��int����Ӧ����intֵ����Ϊ1��ʾ��ʼ���ɹ�


}

void processNormalKeys(unsigned char key, int x, int y) {
    // ���ݰ��µİ������в�ͬ����
    switch (key) {
        // �����µİ���ASCII��Ϊ27����Esc��
    case 27:
        // ����exit�������������0�����������˳�����
        exit(0);
        break;
        // �����µİ���Ϊ�ַ�'l'��СдL��
    case 'l':
        // ��light����ȡ�����л���Դ״̬
  
        light = !light;
        std::cout << light;
        // ����light��ֵ�ж��Ƿ����û���ù���
        light ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
        break;

    case 'f':
        change = !change;
        std::cout << "change";
        break;

    }
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("rotatae cube");
   

    InitGL();

	glutDisplayFunc(renderscene);
	glutIdleFunc(renderscene);
	glutReshapeFunc(changesize);
	glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

	glutMainLoop();
	return 0;
}