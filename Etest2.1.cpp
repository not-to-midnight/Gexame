#include <glut.h>
#include <iostream>



///������11��

//����1���л�smooth����flat





// ����һ����������ʾ��ɫģʽ����ʼ��Ϊ GL_SMOOTH
int shadingMode = GL_SMOOTH;

// ���Ƴ����ĺ���
void RenderScene(void)
{
    // �����ɫ������
    glClear(GL_COLOR_BUFFER_BIT);

    // ���� shadingMode ��ֵ������ɫģʽ
    glShadeModel(shadingMode);

    // ��ʼ����������
    glBegin(GL_TRIANGLES);
    // ���õ�һ��������ɫΪ��ɫ
    glColor3f(1.0f, 0.0f, 0.0f);
    // ���õ�һ�����������
    glVertex3f(0.0f, 1.0f, 0.0f);

    // ���õڶ���������ɫΪ��ɫ
    glColor3f(0.0f, 1.0f, 0.0f);
    // ���õڶ������������
    glVertex3f(-1.0f, -1.0f, 0.0f);

    // ���õ�����������ɫΪ��ɫ
    glColor3f(0.0f, 0.0f, 1.0f);
    // ���õ��������������
    glVertex3f(1.0f, -1.0f, 0.0f);
    // ��������������
    glEnd();

    // ǿ��ˢ�»������������ƽ����ʾ����
    glFlush();
}

// ��ʼ�� OpenGL ���õĺ���
void SetupRC(void)
{
    // ���������ɫΪ��ɫ
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// ���ڴ�С�ı�ʱ�Ļص�����
void ChangeSize(int w, int h)
{
    GLfloat aspectRatio;

    // ��ֹ�������
    if (h == 0)
        h = 1;

    // �����ӿ�Ϊ��������
    glViewport(0, 0, w, h);

    // �л���ͶӰ����ģʽ
    glMatrixMode(GL_PROJECTION);
    // ����ͶӰ����Ϊ��λ����
    glLoadIdentity();

    // ���㴰�ڵĿ�߱�
    aspectRatio = (GLfloat)w / (GLfloat)h;

    // ��������ͶӰ
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, -1.0, 1.0);
    else
        glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, -1.0, 1.0);

    // �л���ģ����ͼ����ģʽ
    glMatrixMode(GL_MODELVIEW);
    // ����ģ����ͼ����Ϊ��λ����
    glLoadIdentity();
}

// ���̰���������
void Keyboard(unsigned char key, int x, int y)
{
    if (key == '1') {
        // �������� 1 ʱ�л���ɫģʽ
        if (shadingMode == GL_SMOOTH) {
            shadingMode = GL_FLAT;
            std::cout << "Switch to GL_FLAT shading mode." << std::endl;
        }
        else {
            shadingMode = GL_SMOOTH;
            std::cout << "Switch to GL_SMOOTH shading mode." << std::endl;
        }
        // ��Ǵ�����Ҫ���»���
        glutPostRedisplay();
    }
}

// ������
int main(int argc, char* argv[])
{
    // ��ʼ�� GLUT ��
    glutInit(&argc, argv);
    // ������ʾģʽΪ������� RGB ��ɫģʽ
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // �������ڲ����ñ���
    glutCreateWindow("Shading Mode Switch Example");
    // ע����Ƴ����Ļص�����
    glutDisplayFunc(RenderScene);
    // ע�ᴰ�ڴ�С�ı�Ļص�����
    glutReshapeFunc(ChangeSize);
    // ע����̰�������Ļص�����
    glutKeyboardFunc(Keyboard);

    //glutIdleFunc(RenderScene);
    // ��ʼ�� OpenGL ����
    SetupRC();

    // ���� GLUT ��ѭ���������¼�
    glutMainLoop();

    return 0;
}