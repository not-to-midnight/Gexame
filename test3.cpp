#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<cmath>
#include"SOIL.h"

// 定义粒子的最大数量为200
#define	MAX_PARTICLES	200        	
// 定义圆周率的值
#define PI              3.14159f
// 用于粒子速度相关计算的常量值
#define RHO             50
// 粒子初始生命周期的基础值
#define LIFE            3.0f
// 时间步长，用于模拟粒子状态随时间的变化
#define TIME            0.01f
// 历史位置的数量
#define HISTORY_SIZE    5
GLuint texture[1];

//动态模糊的间距,大于这个距离后就更新
int distance = 1;

// 全局变量，用于存储窗口的高度和宽度
float windowHeight, windowWidth;


// 定义粒子结构体
typedef struct {
    // 粒子的生命周期
    float	life;
    // 粒子的初始生命周期
    float   init_life;
    // 粒子的老化速度
    float	speed_aging;

    // 粒子颜色的红色分量
    float	r;
    // 粒子颜色的绿色分量
    float	g;
    // 粒子颜色的蓝色分量
    float	b;

    // 粒子在X轴的位置
    float	x;
    // 粒子在Y轴的位置
    float	y;
    // 粒子在Z轴的位置
    float	z;

    // 粒子在X轴的速度
    float	v_x;
    // 粒子在Y轴的速度
    float	v_y;
    // 粒子在Z轴的速度
    float	v_z;

    // 粒子在X轴的加速度
    float	a_x;
    // 粒子在Y轴的加速度
    float	a_y;
    // 粒子在Z轴的加速度
    float	a_z;

    // 历史位置
    float history_x[HISTORY_SIZE];
    float history_y[HISTORY_SIZE];
    float history_z[HISTORY_SIZE];
    int history_index;
} particle;

// 创建粒子数组，用于存储所有粒子的信息
particle particles[MAX_PARTICLES];

// 当窗口改变大小时由GLUT函数库调用的回调函数
void ChangeSize(int w, int h) {
    // 用于存储窗口宽高比
    float aspectRatio;

    // 防止窗口高度为0导致除零错误，若高度为0则设为1
    if (h == 0) h = 1;

    // 设置视口大小为窗口的大小
    glViewport(0, 0, w, h);

    // 切换到投影矩阵模式
    glMatrixMode(GL_PROJECTION);
    // 重置投影矩阵为单位矩阵
    glLoadIdentity();

    // 计算窗口的宽高比
    aspectRatio = (float)w / (float)h;

    // 根据窗口宽高比设置正交投影参数
    if (w <= h) {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        // 设置正交投影的剪裁区域
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    }
    else {
        windowWidth = 100.0 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }

    // 切换回模型视图矩阵模式
    glMatrixMode(GL_MODELVIEW);
    // 重置模型视图矩阵为单位矩阵
    glLoadIdentity();
}

// 初始化粒子系统的函数
int InitPaticleSystem(void) {
    float theta, phi, rho;

    // 遍历粒子数组，初始化每个粒子
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // 为粒子设置初始生命周期，在基础值上加上一个随机小数
        particles[i].init_life = LIFE + rand() % 10 / 10.0;

        // 将初始生命周期赋值给当前生命周期
        particles[i].life = particles[i].init_life;

        // 设置粒子的老化速度为时间步长
        particles[i].speed_aging = TIME;

        // 设置粒子颜色为红色（r = 1.0f, g = 0.0f, b = 0.0f）
        particles[i].r = 1.0f;
        particles[i].g = 0.0f;
        particles[i].b = 0.0f;

        // 设置粒子的初始位置
        particles[i].x = 0.0f;
        particles[i].y = 50.0f;
        particles[i].z = 0.0f;

        // 随机生成角度用于计算速度
        theta = (rand() % 90 + 45) * PI / 180;
        phi = 90 * PI / 180;
        rho = rand() % RHO;
        // 计算粒子在X轴的速度
        particles[i].v_x = float(sinf(phi) * cosf(theta) * rho);
        // 计算粒子在Y轴的速度
        particles[i].v_y = float(sinf(phi) * sin(theta) * rho);
        // 计算粒子在Z轴的速度
        particles[i].v_z = float(cosf(phi) * rho);

        // 设置粒子在X轴的加速度为0
        particles[i].a_x = 0.0f;
        // 设置粒子在Y轴的加速度为 -30.0f，模拟类似重力的效果
        particles[i].a_y = -30.0f;
        // 设置粒子在Z轴的加速度为0
        particles[i].a_z = 0.0f;

        // 初始化历史位置
        for (int j = 0; j < HISTORY_SIZE; j++) {
            particles[i].history_x[j] = particles[i].x;
            particles[i].history_y[j] = particles[i].y;
            particles[i].history_z[j] = particles[i].z;
        }
        particles[i].history_index = 0;
    }

    return true;
}

// 渲染场景的函数
void RenderScene(void) {
    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D); // 启用二维纹理
    glBindTexture(GL_TEXTURE_2D, texture[0]); // 绑定已加载的纹理

    // 遍历粒子数组，绘制每个粒子及其历史位置
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // 获取粒子的位置
        float x = particles[i].x;
        float y = particles[i].y;
        float z = particles[i].z;

        float size = 1;
        float imga1;
        float alpha = 0.0;
        float step = 0.2;


         //绘制历史位置，透明度依次减弱
        for (int j = 1; j <=HISTORY_SIZE; j++) {
            int loact = (j + particles[i].history_index) % HISTORY_SIZE;//重新定位
            // 调整透明度计算方式，避免alpha过大
            glPointSize(size);
            size = size + 0.1f;
			// 绘制历史位置
            imga1 = size ;
            glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
  /*          glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].life);*/
            alpha += step;
            glColor4f(1.0f, 1.0f,1.0f, alpha); // 设置颜色和透明度
            glTexCoord2d(1, 1); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] + imga1, z); // Top Right
            glTexCoord2d(0, 1); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] + imga1, z); // Top Left
            glTexCoord2d(1, 0); glVertex3f(particles[i].history_x[loact] + imga1, particles[i].history_y[loact] - imga1, z); // Bottom Right
            glTexCoord2d(0, 0); glVertex3f(particles[i].history_x[loact] - imga1, particles[i].history_y[loact] - imga1, z); // Bottom Left
            //glBegin(GL_POINTS);

            //// 指定粒子的顶点位置
            //glVertex3f(particles[i].history_x[loact], particles[i].history_y[loact], particles[i].history_z[loact]);
            glEnd();

        }

        // 绘制当前位置

        glPointSize(1);
        glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
        //glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].life);
        glTexCoord2d(1, 1); glVertex3f(x +imga1, y + imga1,z); // Top Right
        glTexCoord2d(0, 1); glVertex3f(x - imga1, y + imga1, z); // Top Left
        glTexCoord2d(1, 0); glVertex3f(x + imga1, y - imga1,z); // Bottom Right
        glTexCoord2d(0, 0); glVertex3f(x - imga1, y - imga1, z); // Bottom Left
        //glBegin(GL_POINTS);

        //// 指定粒子的顶点位置
        //glVertex3f(x, y, z);
        glEnd();

    }

    //glDisable(GL_TEXTURE_2D); // 绘制完成后，禁用纹理

    // 交换缓冲区，显示绘制结果
    glutSwapBuffers();
}

// 更新粒子系统状态的函数
void Update() {
    // 遍历粒子数组，更新每个粒子的状态
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // 保存当前位置到历史位置
        if (abs(particles[i].history_x[particles[i].history_index] - particles[i].x) >= distance) {
            particles[i].history_index = (particles[i].history_index + 1) % HISTORY_SIZE;//循环更新历史位置索引，从1更新到5,index是几，哪个位置就是最近的历史位置
            particles[i].history_x[particles[i].history_index] = particles[i].x;
            particles[i].history_y[particles[i].history_index] = particles[i].y;
            particles[i].history_z[particles[i].history_index] = particles[i].z;
            
        }

        // 根据速度和时间步长更新粒子的位置
        particles[i].x += particles[i].v_x * TIME;
        particles[i].y += particles[i].v_y * TIME;
        particles[i].z += particles[i].v_z * TIME;

        // 根据加速度和时间步长更新粒子的速度
        particles[i].v_x += particles[i].a_x * TIME;
        particles[i].v_y += particles[i].a_y * TIME;
        particles[i].v_z += particles[i].a_z * TIME;

        // 根据老化速度减少粒子的生命周期
        particles[i].life -= particles[i].speed_aging;

        float theta, phi, rho;
        // 如果粒子的生命周期小于0，表示粒子已达到生命周期结束
        if (particles[i].life < 0.0f) {
            // 为粒子赋予新的生命周期
            particles[i].init_life = LIFE + rand() % 10 / 10.0;
            particles[i].life = particles[i].init_life;

            // 重置粒子的位置
            particles[i].x = 0.0f;
            particles[i].y = 50.0f;
            particles[i].z = 0.0f;

            // 重新随机生成角度用于计算速度
            theta = (rand() % 90 + 45) * PI / 180;
            phi = 90 * PI / 180;
            rho = rand() % RHO;
            // 重新初始化粒子在X轴的速度
            particles[i].v_x = float(sinf(phi) * cosf(theta) * rho);
            // 重新初始化粒子在Y轴的速度
            particles[i].v_y = float(sinf(phi) * sin(theta) * rho);
            // 重新初始化粒子在Z轴的速度
            particles[i].v_z = float(cosf(phi) * rho);

            // 重置历史位置
            for (int j = 0; j < HISTORY_SIZE; j++) {
                particles[i].history_x[j] = particles[i].x;
                particles[i].history_y[j] = particles[i].y;
                particles[i].history_z[j] = particles[i].z;
            }
            particles[i].history_index = 0;
        }
    }
}

// 定时器回调函数
void TimerFunction(int value) {
    // 更新粒子系统状态
    Update();
    // 标记窗口需要重新绘制，触发RenderScene函数执行
    glutPostRedisplay();
    // 再次设置定时器，每隔10毫秒调用一次自身
    glutTimerFunc(10, TimerFunction, 1);
}

// 设置渲染状态的函数
void SetupRC(void) {
    // 启用OpenGL的混合功能
    glEnable(GL_BLEND);

    // 设置混合函数，实现基于透明度的颜色混合效果
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



// 加载纹理函数
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


    // 设置清屏颜色为黑色
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 设置深度缓冲区的清除值为1.0
    glClearDepth(1.0f);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试的类型为GL_LEQUAL
    glDepthFunc(GL_LEQUAL);
    // 设置视口大小为窗口的大小
    glViewport(0, 0, windowWidth, windowHeight);
    // 设置投影矩阵模式
    glMatrixMode(GL_PROJECTION);
    // 重置投影矩阵为单位矩阵
    glLoadIdentity();
    // 设置透视投影参数
    gluPerspective(45.0f, windowWidth / windowHeight, 1.0f, 100.0f);
    // 切换回模型视图矩阵模式
    glMatrixMode(GL_MODELVIEW);
    
    std::cout << "纹理加载" << std::endl;
    if (!LoadGLTextures())
    {
        return false;
    }
    else {
		std::cout << "纹理加载成功" << std::endl;
        // 启用OpenGL的混合功能
        glEnable(GL_BLEND);

        // 设置混合函数，实现基于透明度的颜色混合效果
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    return true;
}

int main(int argc, char** argv) {

    // 初始化GLUT库
    glutInit(&argc, argv);
    // 设置显示模式为带深度缓冲区、双缓冲和RGBA颜色模式
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // 设置窗口的初始位置
    glutInitWindowPosition(100, 100);

    // 设置窗口的初始大小
    glutInitWindowSize(640, 640);

    // 创建标题为“Particle system”的窗口
    glutCreateWindow("FireWorks");

    // 注册显示回调函数
    glutDisplayFunc(RenderScene);

    // 注册窗口大小改变回调函数
    glutReshapeFunc(ChangeSize);

    // 注册定时器回调函数，每隔10毫秒调用一次
    glutTimerFunc(10, TimerFunction, 1);

   
    InitGL();

    // 设置渲染状态
    SetupRC();
    // 初始化粒子系统
    InitPaticleSystem();

    // 进入GLUT事件处理循环
    glutMainLoop();

    return 0;
}