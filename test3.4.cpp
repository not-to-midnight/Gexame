#include <glut.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>

constexpr int MAX_PARTICLES = 100;
constexpr int MAX_FIREWORKS = 5;
constexpr int MAX_SPARKS = 200;
constexpr int MAX_SMOKE = 200;
constexpr int MAX_FLAMES = 300;
constexpr float PI = 3.14159265f;
constexpr float gravity = -0.0005f;
constexpr int IGNITION_THRESHOLD = 20;

struct Particle {
    float x, y;
    float vx, vy;
    float r, g, b;
    float life;
    bool active;
};

struct Firework {
    float x, y;
    float vy;
    bool exploded;
    std::vector<Particle> particles;
};

struct Wood {
    float x, y, width, height;
    int hitCount;
    bool onFire;
    bool isBurnedOut;
    std::vector<Particle> sparks;
    std::vector<Particle> smoke;
    std::vector<Particle> flames;
    float burnLevel;
};

std::vector<Firework> fireworks;
Wood wood;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

// 预生成随机数数组
constexpr int PREGEN_SIZE = 1000;
float pregenAngles[PREGEN_SIZE];
float pregenSpeeds[PREGEN_SIZE];
int pregenIndex = 0;

void initPregen() {
    for (int i = 0; i < PREGEN_SIZE; ++i) {
        pregenAngles[i] = static_cast<float>(dis(gen) * 2 * PI);
        pregenSpeeds[i] = static_cast<float>(0.005 + dis(gen) * 0.025);
    }
}

inline float randomFloat(float a, float b) {
    return a + (b - a) * static_cast<float>(dis(gen));
}

inline void getRandomWoodPosition(float& x, float& y) {
    x = wood.x + wood.width * static_cast<float>(dis(gen));
    y = wood.y + wood.height * static_cast<float>(dis(gen));
}

void createFirework() {
    if (fireworks.size() >= MAX_FIREWORKS) return;

    Firework fw;
    fw.x = randomFloat(-0.8f, 0.8f);
    fw.y = -1.0f;
    fw.vy = randomFloat(0.03f, 0.04f);
    fw.exploded = false;
    fw.particles.reserve(MAX_PARTICLES);
    fireworks.push_back(fw);
}

void explode(Firework& fw) {
    fw.particles.clear();
    fw.particles.reserve(MAX_PARTICLES);

    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle p;
        const float angle = pregenAngles[(pregenIndex + i) % PREGEN_SIZE];
        const float speed = pregenSpeeds[(pregenIndex + i) % PREGEN_SIZE];

        p.vx = std::cos(angle) * speed;
        p.vy = std::sin(angle) * speed;
        p.x = fw.x;
        p.y = fw.y;
        p.r = randomFloat(0.7f, 1.0f);
        p.g = randomFloat(0.7f, 1.0f);
        p.b = randomFloat(0.7f, 1.0f);
        p.life = 1.5f;
        p.active = true;
        fw.particles.push_back(p);
    }
    pregenIndex = (pregenIndex + MAX_PARTICLES) % PREGEN_SIZE;
    fw.exploded = true;
}

bool allParticlesInactive(const Firework& fw) {
    for (const auto& p : fw.particles)
        if (p.active) return false;
    return true;
}

void createParticle(std::vector<Particle>& container, float r, float g, float b,
    float life, float speedMultiplier, bool useGravity) {
    // 查找可复用的粒子
    for (auto& p : container) {
        if (!p.active) {
            float x, y;
            getRandomWoodPosition(x, y);
            const float angle = pregenAngles[pregenIndex];
            const float speed = pregenSpeeds[pregenIndex] * speedMultiplier;

            p.x = x;
            p.y = y;
            p.vx = std::cos(angle) * speed;
            p.vy = std::sin(angle) * speed;
            p.r = r;
            p.g = g;
            p.b = b;
            p.life = life;
            p.active = true;

            pregenIndex = (pregenIndex + 1) % PREGEN_SIZE;
            return;
        }
    }

    // 没有可复用则新建（如果容器还有空间）
    if (container.size() < container.capacity()) {
        Particle p;
        float x, y;
        getRandomWoodPosition(x, y);
        const float angle = pregenAngles[pregenIndex];
        const float speed = pregenSpeeds[pregenIndex] * speedMultiplier;

        p.x = x;
        p.y = y;
        p.vx = std::cos(angle) * speed;
        p.vy = std::sin(angle) * speed;
        p.r = r;
        p.g = g;
        p.b = b;
        p.life = life;
        p.active = true;

        container.push_back(p);
        pregenIndex = (pregenIndex + 1) % PREGEN_SIZE;
    }
}

void updateParticles(std::vector<Particle>& particles, bool checkCollision) {
    for (auto& p : particles) {
        if (!p.active) continue;

        p.x += p.vx;
        p.y += p.vy;
        p.vy += gravity;
        p.life -= 0.03f;

        if (p.life <= 0.0f) {
            p.active = false;
            continue;
        }

        if (p.x < -1.0f || p.x > 1.0f) p.vx *= -0.8f;
        if (p.y < -1.0f || p.y > 1.0f) p.vy *= -0.8f;

        if (checkCollision && !wood.onFire && !wood.isBurnedOut) {
            if (p.x >= wood.x && p.x <= wood.x + wood.width &&
                p.y >= wood.y && p.y <= wood.y + wood.height) {
                p.active = false;
                if (++wood.hitCount >= IGNITION_THRESHOLD)
                    wood.onFire = true;
            }
        }
    }
}

void updateFlames() {
    for (auto& p : wood.flames) {
        if (!p.active) continue;

        p.y += p.vy;
        p.life -= 0.03f;
        if (p.life <= 0.0f)
            p.active = false;
    }
}

void updateWood() {
    if (!wood.onFire || wood.isBurnedOut) return;

    wood.burnLevel += 0.005f;
    if (wood.burnLevel >= 1.0f) {
        wood.isBurnedOut = true;
        return;
    }

    for (int i = 0; i < 5; ++i)
        createParticle(wood.flames, randomFloat(0.9f, 1.0f), randomFloat(0.5f, 0.7f), 0.0f,
            randomFloat(0.5f, 1.0f), 0.03f, false);

    for (int i = 0; i < 3; ++i)
        createParticle(wood.sparks, 1.0f, 0.5f, 0.0f, 0.5f, 0.03f, true);

    for (int i = 0; i < 2; ++i)
        createParticle(wood.smoke, 0.3f, 0.3f, 0.3f, 1.0f, 0.01f, true);
}

void update(int value) {
    // 更新烟花
    for (auto& fw : fireworks) {
        if (!fw.exploded) {
            fw.y += fw.vy;
            if ((fw.vy += gravity) <= 0.0f)
                explode(fw);
        }
        else {
            updateParticles(fw.particles, true);
        }
    }

    // 清理失效烟花
    fireworks.erase(std::remove_if(fireworks.begin(), fireworks.end(),
        [](const Firework& fw) { return fw.exploded && allParticlesInactive(fw); }),
        fireworks.end());

    // 更新燃烧系统
    updateWood();
    updateParticles(wood.sparks, false);
    updateParticles(wood.smoke, false);
    updateFlames();

    // 概率生成新烟花
    if (fireworks.size() < MAX_FIREWORKS && dis(gen) < 0.1)
        createFirework();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void drawParticles(const std::vector<Particle>& particles, float pointSize, bool useAlpha) {
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        if (!p.active) continue;
        if (useAlpha)
            glColor4f(p.r, p.g, p.b, p.life);
        else
            glColor3f(p.r, p.g, p.b);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // 绘制木头
    if (!wood.isBurnedOut) {
        glColor3f(0.5f * (1.0f - wood.burnLevel), 0.2f * (1.0f - wood.burnLevel), 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(wood.x, wood.y);
        glVertex2f(wood.x + wood.width, wood.y);
        glVertex2f(wood.x + wood.width, wood.y + wood.height * (1.0f - wood.burnLevel));
        glVertex2f(wood.x, wood.y + wood.height * (1.0f - wood.burnLevel));
        glEnd();
    }

    // 合并绘制调用
    drawParticles(wood.sparks, 2.0f, false);
    drawParticles(wood.smoke, 2.0f, false);
    drawParticles(wood.flames, 3.0f, true);

    // 绘制烟花
    glPointSize(3.0f);
    for (const auto& fw : fireworks) {
        if (!fw.exploded) {
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_POINTS);
            glVertex2f(fw.x, fw.y);
            glEnd();
        }
        else {
            drawParticles(fw.particles, 3.0f, true);
        }
    }

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initPregen();
    fireworks.reserve(MAX_FIREWORKS);

    wood.x = -0.8f;
    wood.y = -0.9f;
    wood.width = 1.6f;
    wood.height = 0.1f;
    wood.sparks.reserve(MAX_SPARKS);
    wood.smoke.reserve(MAX_SMOKE);
    wood.flames.reserve(MAX_FLAMES);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Optimized Fireworks");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
