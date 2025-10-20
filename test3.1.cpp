#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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
        particles[i].init_life = LIFE + rand() % 10 / 10.0;                                         ///生命走

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
    }

    return true;
}

// 渲染场景的函数
void RenderScene(void) {
    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 遍历粒子数组，绘制每个粒子
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // 获取粒子的位置
        float x = particles[i].x;
        float y = particles[i].y;
        float z = particles[i].z;

        // 根据粒子的RGB值和生命周期设置颜色（生命周期作为透明度）
        glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].life);///////////////////////////////life使用

        // 设置绘制点的大小为4.0f
        glPointSize(4.0f);

        // 开始绘制点
        glBegin(GL_POINTS);

        // 指定粒子的顶点位置
        glVertex3f(x, y, z);

        // 结束绘制
        glEnd();
    }

    // 交换缓冲区，显示绘制结果
    glutSwapBuffers();
}

// 更新粒子系统状态的函数
void Update() {
    // 遍历粒子数组，更新每个粒子的状态
	for (int i = 0; i < MAX_PARTICLES; i++) {                //遍历不同的粒子，生成不同的粒子的时间会因遍历的延时而产生差异
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
            theta = (rand() % 90+45) * PI / 180;
            phi = 90 * PI / 180;
            rho = rand() % RHO;
            // 重新初始化粒子在X轴的速度
            particles[i].v_x = float(sinf(phi) * cosf(theta) * rho);
            // 重新初始化粒子在Y轴的速度
            particles[i].v_y = float(sinf(phi) * sin(theta) * rho);
            // 重新初始化粒子在Z轴的速度
            particles[i].v_z = float(cosf(phi) * rho);
        }
    }
}

// 定时器回调函数
void TimerFunction(int value) {
    // 更新粒子系统状态
    Update();
    // 标记窗口需要重新绘制，触发RenderScene函数执行
    glutPostRedisplay();
	//std::cout << "TimerFunction" << std::endl;
    // 再次设置定时器，每隔10毫秒调用一次自身
    glutTimerFunc(10, TimerFunction, 1);///10是10毫秒！！
}

// 设置渲染状态的函数
void SetupRC(void) {
    // 启用OpenGL的混合功能
    glEnable(GL_BLEND);

    // 设置混合函数，实现基于透明度的颜色混合效果
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glutCreateWindow("Particle system");

    // 注册显示回调函数
    glutDisplayFunc(RenderScene);

    // 注册窗口大小改变回调函数
    glutReshapeFunc(ChangeSize);

    // 注册定时器回调函数，每隔10毫秒调用一次
    glutTimerFunc(10, TimerFunction, 1);

    // 设置渲染状态
    SetupRC();
    // 初始化粒子系统
    InitPaticleSystem();

    // 进入GLUT事件处理循环
    glutMainLoop();

    return 0;
}