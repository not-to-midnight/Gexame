#include <glut.h>
#include <iostream>


//�������ҿ����ƶ�


//ѩ��//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // ������
GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // �������
GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // ��Դλ��

GLfloat matAmbient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat matDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat matShininess[] = { 50.0f };

GLfloat a = 1.0f;


// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

// ����һ����ΪChangeSize�ĺ����������ڴ��ڴ�С�ı�ʱ����͸��ͶӰ������
// ����w��h�ֱ��ʾ���ڵĿ�Ⱥ͸߶�
void ChangeSize(GLsizei w, GLsizei h) // ͸��ͶӰ
{
    // ����һ��GLfloat���͵ı���fAspect�����ڴ洢���ڵĿ�߱�
    GLfloat fAspect;

    // ��ֹ����Ϊ����������
    // ������ڵĸ߶�hΪ0����������Ϊ1��������������߱�ʱ���ֳ������
    if (h == 0)
        h = 1;

    // ����OpenGL���ӿڣ�Viewport��
    // �ӿڶ����˴��������ڻ���ͼ�ε�����
    // ���ｫ�ӿ�����Ϊ�������ڣ���ʼ����Ϊ(0, 0)�����Ϊw���߶�Ϊh
    glViewport(0, 0, w, h);

    // ���㴰�ڵĿ�߱�
    // �����ڵĿ��w�͸߶�hת��ΪGLfloat���ͺ����������洢��fAspect��
    fAspect = (GLfloat)w / (GLfloat)h;

    // �л���ͶӰ����ģʽ
    // OpenGL���ж��־���ģʽ��ͶӰ�������ڶ�����ν���ά����ͶӰ����ά��Ļ��
    glMatrixMode(GL_PROJECTION);

    // ����ͶӰ����Ϊ��λ����
    // ��λ������һ������ľ��󣬳����κξ��󶼲���ı�þ����ֵ
    // ���ｫͶӰ��������Ϊ��λ�����Ա������������ͶӰ����
    glLoadIdentity();

    // ����͸��ͶӰ
    // gluPerspective��OpenGL��һ���������������ڴ���͸��ͶӰ����
    // ��һ������60.0f��ʾ��ֱ�ӽǣ�����ֱ�����ϵ���Ұ�Ƕ�
    // �ڶ�������fAspect�Ǵ��ڵĿ�߱ȣ�����ȷ��ˮƽ�ӽ�
    // ����������1.0��ʾ���ü���ľ��룬����۲�������Ŀɼ�����
    // ���ĸ�����400.0��ʾԶ�ü���ľ��룬����۲�����Զ�Ŀɼ�����
    gluPerspective(60.0f, fAspect, 1.0, 400.0);

    // �л���ģ����ͼ����ģʽ
    // ģ����ͼ�������ڿ��������λ�á���ת�����ŵȱ任
    glMatrixMode(GL_MODELVIEW);

    // ����ģ����ͼ����Ϊ��λ����
    // ��������ȷ������������任�����Ǵӳ�ʼ״̬��ʼ��
    glLoadIdentity();
}


void drawSnowMan() {
    // ���� (��ɫ)
    glColor3f(1.0f, a, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);

    // ͷ��
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    // �۾� (��ɫ)
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

    // ���� (��ɫ)
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.5f, 10, 2);
    glPopMatrix();
}

void setupRC(void)
{
	glClearColor(0.6f, 0.4f, 0.7f, 1.0f);//����������ɫ
	glColor3f(1.0f, 0.0f, 0.0f);//���û�ͼ��ɫ
}


void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == 27)//����ESC�����˳�����
        exit(0);
    if (key == '1') {

        if (a == 0) { a = 1.0f; }
        else { a = 0; }
        glutPostRedisplay();
        std::cout << a;
    }
    switch (key) {
    case '+':
        // ���ӻ��������ȣ�������0.0f��1.0f֮��
        lightAmbient[0] = lightAmbient[0] + 0.05f > 1.0f ? 1.0f : lightAmbient[0] + 0.05f;
        lightAmbient[1] = lightAmbient[1] + 0.05f > 1.0f ? 1.0f : lightAmbient[1] + 0.05f;
        lightAmbient[2] = lightAmbient[2] + 0.05f > 1.0f ? 1.0f : lightAmbient[2] + 0.05f;
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        break;
    case '-':
        // ��С���������ȣ�������0.0f��1.0f֮��
        lightAmbient[0] = lightAmbient[0] - 0.05f < 0.0f ? 0.0f : lightAmbient[0] - 0.05f;
        lightAmbient[1] = lightAmbient[1] - 0.05f < 0.0f ? 0.0f : lightAmbient[1] - 0.05f;
        lightAmbient[2] = lightAmbient[2] - 0.05f < 0.0f ? 0.0f : lightAmbient[2] - 0.05f;
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

  void processSpecialKeys(int key, int xx, int yy) {
        float fraction = 0.1f;

        switch (key) {
        case GLUT_KEY_LEFT:
            angle -= 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT:
            angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_UP:
            x += lx * fraction;
            z += lz * fraction;
            break;
        case GLUT_KEY_DOWN:
            x -= lx * fraction;
            z -= lz * fraction;
            break;
        }
    }


void renderScene(void) {//
    // �����ɫ����������Ȼ�����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ���ñ任���󣬻ص���ʼ״̬
    glLoadIdentity();

    // �������λ�úͷ���
    gluLookAt(
        x, 1.0f, z,      // ���λ��
        x + lx, 1.0f, z + lz,  // ��������Ŀ���
        0.0f, 1.0f, 0.0f  // ��������Ϸ���
    );

    // ���Ƶ���
    glColor3f(0.9f, 0.9f, 0.9f);  // ���õ�����ɫ
    glBegin(GL_QUADS);  // ��ʼ�����ı���
    glVertex3f(-100.0f, 0.0f, -100.0f);  // �ı��ζ�������
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();  // ���������ı���

    // ����36��ѩ��
    for (int i = -3; i < 3; i++) {
        for (int j = -3; j < 3; j++) {
            glPushMatrix();  // ���浱ǰ����״̬
            glTranslatef(i * 10.0, 0.0, j * 10.0);  // ƽ�Ƶ�ָ��λ��
            drawSnowMan();  // ���ú�������ѩ��
            glPopMatrix();  // �ָ�֮ǰ����ľ���״̬
        }
    }

    glutSwapBuffers();  // ����ǰ�󻺳�������ʾ���ƽ��
}

int InitGL(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // ���ù���
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // ���ù�Դ
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // ���ò���
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // ������ɫ׷��
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    return 1;
}


// main�������������
int main(int argc, char** argv) {
    // ��ʼ��GLUT��
    glutInit(&argc, argv);
    // ������ʾģʽ��������Ȳ��ԡ�˫�����RGBA��ɫģʽ
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    // ���ô��ڳ�ʼλ��Ϊ(100, 100)
    glutInitWindowPosition(100, 100);
    // ���ô��ڳ�ʼ��СΪ(320, 320)
    glutInitWindowSize(320, 320);
    // �������ڲ����ñ���Ϊ"Lighthouse3D - GLUT Tutorial"
    glutCreateWindow("Lighthouse3D - GLUT Tutorial");

    // ע��ص�����
    // ע����ʾ�ص���������������Ҫ�ػ�ʱ����renderScene����
    glutDisplayFunc(renderScene);
    // ע�ᴰ�ڴ�С�ı�ص������������ڴ�С�ı�ʱ����changeSize����
    glutReshapeFunc(ChangeSize);
    // ע����лص����������������ʱ����renderScene����

    glutIdleFunc(renderScene);//�����������λ��                                                                                    //ÿ�붼�����»��ƻ���

    // ע����ͨ���̰����ص�������������ͨ���̰�������ʱ����processNormalKeys����
    glutKeyboardFunc(processNormalKeys);

    // ע��������̰����ص�����������������̰�������ʱ����processSpecialKeys����
    glutSpecialFunc(processSpecialKeys);
    InitGL();

    //setupRC();
    // ����OpenGL��Ȳ���


    // ����GLUT�¼�����ѭ��
    glutMainLoop();

    return 1;
}