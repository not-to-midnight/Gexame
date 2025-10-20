#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "SOIL.h"

///烟花
//1和2改变动态模糊强度
#define MAX_PARTICLES 200        	// 定义粒子的最大数量为200
#define PI 3.14159f               	// 定义圆周率的值
#define RHO 50                    	// 用于粒子速度相关计算的常量值
#define LIFE 6.0f                 	// 粒子初始生命周期的基础值
#define TIME 0.01f                	// 时间步长，用于模拟粒子状态随时间的变化
#define HISTORY_SIZE 5            	// 历史位置的数量
GLuint texture[1];                	// 纹理对象

float distance = 1;                  	// 动态模糊的间距,大于这个距离后就更新
float windowHeight, windowWidth;   	// 全局变量，用于存储窗口的高度和宽度

float planecenterx = -100.0f;
float planecentery = -100.0f;        	// 地面中心坐标

float planewidth = 50.0f;        	// 地面宽度
float planeheight = 50.0f;       	// 地面高度

typedef struct {                   	// 定义粒子结构体
    float life;                    	// 粒子的生命周期
    float init_life;               	// 粒子的初始生命周期
    float speed_aging;             	// 粒子的老化速度
    float r;                       	// 粒子颜色的红色分量
    float g;                       	// 粒子颜色的绿色分量
    float b;                       	// 粒子颜色的蓝色分量
    float x;                       	// 粒子在X轴的位置
    float y;                       	// 粒子在Y轴的位置
    float z;                       	// 粒子在Z轴的位置
    float v_x;                     	// 粒子在X轴的速度
    float v_y;                     	// 粒子在Y轴的速度
    float v_z;                     	// 粒子在Z轴的速度
    float a_x;                     	// 粒子在X轴的加速度
    float a_y;                     	// 粒子在Y轴的加速度
    float a_z;                     	// 粒子在Z轴的加速度
    float history_x[HISTORY_SIZE]; 	// 历史位置
    float history_y[HISTORY_SIZE]; 	// 历史位置
    float history_z[HISTORY_SIZE]; 	// 历史位置
    int history_index;             	// 历史位置索引
} particle;

particle particles[MAX_PARTICLES]; 	// 创建粒子数组，用于存储所有粒子的信息



void ChangeSize(int w, int h);   	// 函数声明，用于处理窗口大小变
void Update();                   	// 函数声明，用于更新粒子系统状态
int InitPaticleSystem(void);     	// 函数声明，用于初始化粒子系统
void SetupRC(void);              	// 函数声明，用于设置渲染状态
void TimerFunction(int value);   	// 函数声明，用于定时器回调函数
int LoadGLTextures();           	// 函数声明，用于加载纹理
void RenderScene(void);          	// 函数声明，用于渲染场景
void InitGLUT(int argc, char** argv);// 函数声明，用于初始化GLUT
int InitGL();              	// 函数声明，用于初始化OpenGL
void detection(particle& a);
void recover(particle& a);


void drawplane(float centerx,float centery,float width,float height,float deepth)
{
    // 设置颜色
    glColor3f(0.9f, 0.9f, 0.9f);
    // 开始绘制四边形
    glBegin(GL_QUADS);
    // 二维四边形顶点坐标，z坐标设置为0

	glVertex3f(centerx, centery , deepth);
	glVertex3f(centerx + width, centery, deepth);
    glVertex3f(centerx + width, centery + height, deepth);
	glVertex3f(centerx , centery+height, deepth);

	// 结束绘制

    glEnd();
}






void ChangeSize(int w, int h) {    	// 当窗口改变大小时由GLUT函数库调用的回调函数
    float aspectRatio;             	// 用于存储窗口宽高比
    if (h == 0) h = 1;             	// 防止窗口高度为0导致除零错误，若高度为0则设为1
    glViewport(0, 0, w, h);        	// 设置视口大小为窗口的大小
    glMatrixMode(GL_PROJECTION);   	// 切换到投影矩阵模式
    glLoadIdentity();              	// 重置投影矩阵为单位矩阵
    aspectRatio = (float)w / (float)h; // 计算窗口的宽高比
    if (w <= h) {                  	// 根据窗口宽高比设置正交投影参数
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0
        ); // 设置正交投影的剪裁区域
    }
    else {
        windowWidth = 100.0 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }
    glMatrixMode(GL_MODELVIEW);    	// 切换回模型视图矩阵模式
    glLoadIdentity();              	// 重置模型视图矩阵为单位矩阵
}



int InitPaticleSystem(void) {      	// 初始化粒子系统的函数
	float theta, phi, rho;// 用于存储粒子的速度和位置
    for (int i = 0; i < MAX_PARTICLES; i++) { // 遍历粒子数组，初始化每个粒子
        particles[i].init_life = LIFE + rand() % 1 ; // 为粒子设置初始生命周期，在基础值上加上一个随机小数
        particles[i].life = particles[i].init_life; // 将初始生命周期赋值给当前生命周期
        particles[i].speed_aging = TIME; // 设置粒子的老化速度为时间步长
        particles[i].r = 1.0f; // 设置粒子颜色为红色（r = 1.0f, g = 0.0f, b = 0.0f）
        particles[i].g = 0.0f;
        particles[i].b = 0.0f;
        particles[i].x = 0.0f; // 设置粒子的初始位置
        particles[i].y = 70.0f;
        particles[i].z = 0.0f;
        theta = (rand()%180 - 270) * PI / 360; // 随机生成角度用于计算速度
        phi = 90 * PI / 180;
        rho = rand() % RHO;
        particles[i].v_x = (float)(sinf(phi) * cosf(theta) * rho); // 计算粒子在X轴的速度
        particles[i].v_y = (float)(sinf(phi) * sinf(theta) * rho); // 计算粒子在Y轴的速度
        particles[i].v_z = (float)(cosf(phi) * rho); // 计算粒子在Z轴的速度
        particles[i].a_x = 0.0f; // 设置粒子在X轴的加速度为0
        particles[i].a_y = -30.0f; // 设置粒子在Y轴的加速度为 -30.0f，模拟类似重力的效果
        particles[i].a_z = 0.0f; // 设置粒子在Z轴的加速度为0
        for (int j = 0; j < HISTORY_SIZE; j++) { // 初始化历史位置
            particles[i].history_x[j] = particles[i].x;
            particles[i].history_y[j] = particles[i].y;
            particles[i].history_z[j] = particles[i].z;
        }
        particles[i].history_index = 0; // 初始化历史位置索引
    }
    return true;
}


void detection(particle& a) {               	// 检测函数
    if (a.x >= -80 && a.x <= 80 && a.y <= -70 && a.y >= -80)
    {
		//std::cout << "反弹前" << a.v_y << std::endl;
        if (abs(a.v_y)-30>=0)
        a.v_y = -a.v_y / abs(a.v_y) * (abs(a.v_y) - 30);  // 反弹
        else
        {
			recover(a);
			//std::cout << "恢复" <<  std::endl;
        }

        
		//std::cout << "反弹后" << a.v_y << std::endl;
    }

    if (a.x >= -80 && a.x <= -70 && a.y <= -30 && a.y >= -80) {
        if (a.x >= -80 && a.x <= -70) {
            a.v_x = -a.v_x; // 反弹
        }
		if (a.y <= -30 && a.y >= -80) {
			a.v_y = -a.v_y/abs(a.v_y)*(abs(a.v_y)-30); // 反弹
		}
        
    }
    if (a.x >= 80 && a.x <= 100 && a.y <= -30 && a.y >= -80)
    {
		if (a.x >= 80 && a.x <= 100) {
			a.v_x = -a.v_x; // 反弹
		}
        
        if (a.y <= -30 && a.y >= -80) {
            a.v_y = -a.v_y; // 反弹
        }
    }

}

void RenderScene(void) {           	// 渲染场景的函数
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲区和深度缓冲区
    glEnable(GL_TEXTURE_2D);       	// 启用二维纹理
    glBindTexture(GL_TEXTURE_2D, texture[0]); // 绑定已加载的纹理

    drawplane(-80, -80, 10, 50,-1);//左
	drawplane(-80,-80 ,160 ,10,-1); // 下
	drawplane(80, -80, 20, 50,-1); // 右
    drawplane(80, -80, 5, 160,1); // 
    drawplane(0, 80, 90,5 ,1); // 绘制地面
    drawplane(-10, 70, 20, 10,1); // 绘制地面



    for (int i = 0; i < MAX_PARTICLES; i++) { // 遍历粒子数组，绘制每个粒子及其历史位置
        float x = particles[i].x; // 获取粒子的位置
        float y = particles[i].y;
        float z = particles[i].z;
        float size = 1;
        float imga1;
        float alpha = 0.0;
        float step = 0.2;
        for (int j = 1; j <= HISTORY_SIZE; j++) { // 绘制历史位置，透明度依次减弱
            int loact = (j + particles[i].history_index) % HISTORY_SIZE; // 重新定位历史位置索引
            glPointSize(size); // 设置点大小
            size += 0.1f; // 逐步增大历史粒子大小
            imga1 = size; // 计算纹理映射尺寸
            glBegin(GL_TRIANGLE_STRIP); // 构建三角形带绘制四边形
            alpha += step; // 逐步增加透明度
            // 设置颜色和透明度（黄色半透明）
            glColor4f(0.0f, 0.0f, 1.0f, alpha);
            glTexCoord2d(1, 1); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] + imga1, z); // 右上顶点
            glTexCoord2d(0, 1); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] + imga1, z); // 左上顶点
            glTexCoord2d(1, 0); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] - imga1, z); // 右下顶点
            glTexCoord2d(0, 0); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] - imga1, z); // 左下顶点
            glEnd(); // 结束绘制
        }
        glPointSize(1); // 设置当前粒子大小
        glBegin(GL_TRIANGLE_STRIP); // 构建三角形带绘制四边形
        glTexCoord2d(1, 1); glVertex3f(x + imga1, y + imga1, z); // 右上顶点
        glTexCoord2d(0, 1); glVertex3f(x - imga1, y + imga1, z); // 左上顶点
        glTexCoord2d(1, 0); glVertex3f(x + imga1, y - imga1, z); // 右下顶点
        glTexCoord2d(0, 0); glVertex3f(x - imga1, y - imga1, z); // 左下顶点
        glEnd(); // 结束绘制
    }
    glutSwapBuffers();             	// 交换缓冲区，显示绘制结果
}

void Update() {                    	// 更新粒子系统状态的函数
    for (int i = 0; i < MAX_PARTICLES; i++) { // 遍历粒子数组，更新每个粒子的状态

        if (abs(particles[i].history_x[particles[i].history_index] - particles[i].x) >= distance) { // 保存当前位置到历史位置（动态模糊距离判断）
            particles[i].history_index = (particles[i].history_index + 1) % HISTORY_SIZE; // 循环更新历史位置索引
            particles[i].history_x[particles[i].history_index] = particles[i].x; // 存储当前X坐标到历史数组
            particles[i].history_y[particles[i].history_index] = particles[i].y; // 存储当前Y坐标到历史数组
            particles[i].history_z[particles[i].history_index] = particles[i].z; // 存储当前Z坐标到历史数组
        }

        particles[i].x += particles[i].v_x * TIME; // 根据速度和时间步长更新粒子的位置
        particles[i].y += particles[i].v_y * TIME;
        //particles[i].z += particles[i].v_z * TIME;
        particles[i].v_x += particles[i].a_x * TIME; // 根据加速度和时间步长更新粒子的速度
        particles[i].v_y += particles[i].a_y * TIME;
        //particles[i].v_z += particles[i].a_z * TIME;
        particles[i].life -= particles[i].speed_aging; // 根据老化速度减少粒子的生命周期
		detection(particles[i]); // 检测粒子与地面的碰撞

        if (particles[i].x >= 150 || particles[i].x <= -150 || particles[i].y <= -150 || particles[i].y >= 150)
        {
            recover(particles[i]);
        }
        //if (particles[i].life < 0.0f) 
        //if(particles[i].x>=150|| particles[i].x<=-150||particles[i].y<=-150||particles[i].y>=150)
        //{                                                                                                                          // 如果粒子的生命周期小于0，表示粒子已达到生命周期结束
        //    particles[i].init_life = LIFE + rand() % 10 / 10.0; // 为粒子赋予新的生命周期
        //    particles[i].life = particles[i].init_life;
        //    particles[i].x = 0.0f; // 重置粒子的位置
        //    particles[i].y = 70.0f;
        //    particles[i].z = 0.0f;

        //    theta = (rand() % 180 - 270) * PI / 360; // 随机生成角度用于计算速度
        //    phi = 90 * PI / 180;
        //    rho = rand() % RHO;
        //    particles[i].v_x = (float)(sinf(phi) * cosf(theta) * rho); // 重新初始化粒子在X轴的速度
        //    particles[i].v_y = (float)(sinf(phi) * sin(theta) * rho); // 重新初始化粒子在Y轴的速度
        //    particles[i].v_z = (float)(cosf(phi) * rho); // 重新初始化粒子在Z轴的速度

        //    for (int j = 0; j < HISTORY_SIZE; j++) { // 重置历史位置
        //        particles[i].history_x[j] = particles[i].x;
        //        particles[i].history_y[j] = particles[i].y;
        //        particles[i].history_z[j] = particles[i].z;
        //    }
        //    particles[i].history_index = 0; // 重置历史位置索引
        
    }
}

void recover(particle& a)  // 恢复粒子状态的函数
{
    float theta, phi, rho;
                                                                                                                      // 如果粒子的生命周期小于0，表示粒子已达到生命周期结束
        a.init_life = LIFE + rand() % 10 / 10.0; // 为粒子赋予新的生命周期
        a.life = a.init_life;
        a.x = 0.0f; // 重置粒子的位置
        a.y = 70.0f;
        a.z = 0.0f;

        theta = (rand() % 180 - 270) * PI / 360; // 随机生成角度用于计算速度
        phi = 90 * PI / 180;
        rho = rand() % RHO;
        a.v_x = (float)(sinf(phi) * cosf(theta) * rho); // 重新初始化粒子在X轴的速度
        a.v_y = (float)(sinf(phi) * sin(theta) * rho); // 重新初始化粒子在Y轴的速度
        a.v_z = (float)(cosf(phi) * rho); // 重新初始化粒子在Z轴的速度

        for (int j = 0; j < HISTORY_SIZE; j++) { // 重置历史位置
            a.history_x[j] = a.x;
            a.history_y[j] = a.y;
            a.history_z[j] = a.z;
        }
        a.history_index = 0; // 重置历史位置索引
    
}

void TimerFunction(int value) {    	// 定时器回调函数
    Update(); // 更新粒子系统状态
    glutPostRedisplay(); // 标记窗口需要重新绘制，触发RenderScene函数执行
    glutTimerFunc(10, TimerFunction, 1); // 再次设置定时器，每隔10毫秒调用一次自身
}

void SetupRC(void) {               	// 设置渲染状态的函数
    glEnable(GL_BLEND); // 启用OpenGL的混合功能
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置混合函数，实现基于透明度的颜色混合效果
}

int LoadGLTextures() {             	// 加载纹理函数
    texture[0] = SOIL_load_OGL_texture( // 加载纹理文件
        "water.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    if (texture[0] == 0) return false; // 纹理加载失败返回false
    glBindTexture(GL_TEXTURE_2D, texture[0]); // 绑定纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 设置纹理缩小过滤
    return true; // 纹理加载成功返回true
}

int InitGL() {                     	// 初始化OpenGL函数
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 设置清屏颜色为黑色
    glClearDepth(1.0f);            	// 设置深度缓冲区清除值
    glEnable(GL_DEPTH_TEST);       	// 启用深度测试
    glDepthFunc(GL_LEQUAL);        	// 设置深度测试函数

    std::cout << "纹理加载" << std::endl; // 输出纹理加载提示
    if (!LoadGLTextures()) return false; // 加载纹理失败返回false
    else {
        std::cout << "纹理加载成功" << std::endl; // 输出加载成功提示
        glEnable(GL_BLEND);        	// 启用混合功能
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置混合函数
    }
    return true; // 初始化成功返回true
}

void InitGLUT(int argc, char** argv) { // 初始化GLUT函数
	glutInit(&argc, argv);          	// 初始化GLUT库
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 设置显示模式
	glutInitWindowSize(640, 480);   	// 设置窗口大小
	glutCreateWindow("FireWorks"); // 创建窗口并设置标题
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

int main(int argc, char** argv) {  	// 主函数

	srand(time(NULL));          	// 设置随机数种子
 
	InitGLUT(argc, argv);          	// 初始化GLUT

    glutDisplayFunc(RenderScene);  	// 注册显示回调函数
    glutReshapeFunc(ChangeSize);   	// 注册窗口大小改变回调函数
    glutTimerFunc(10, TimerFunction, 1); // 注册定时器回调函数

    InitGL();                      	// 初始化OpenGL环境
    SetupRC();                     	// 设置渲染状态
    InitPaticleSystem();           	// 初始化粒子系统
	glutKeyboardFunc(Keyboard);    	// 注册键盘输入回调函数

    glutMainLoop();                	// 进入GLUT事件处理循环

    return 0; // 程序正常退出返回0
}