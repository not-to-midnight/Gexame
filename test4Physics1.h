#pragma once
/**************************************************************************

文件: Physics2.h

由Erkin Tunca为nehe.gamedev.net准备

**************************************************************************/
constexpr float pi = 3.14159265358979323846f;  // float版本
#include "test4Physics2.h"									// Physics1.h对于Physics2.h的模拟是必需的

class Spring											// 一个表示带有内部摩擦且连接两个质量的弹簧的对象。该弹簧
	// 有一个正常长度（弹簧不施加任何力时的长度）
{
public:
	Mass* mass1;										// 弹簧一端的第一个质量
	Mass* mass2;										// 弹簧另一端的第二个质量

	float springConstant;								// 表示弹簧刚度的常量
	float springLength;									// 弹簧不施加任何力时的长度
	float frictionConstant;								// 用于弹簧内部摩擦的常量

	Spring(Mass* mass1, Mass* mass2,
		float springConstant, float springLength, float frictionConstant):springConstant(springConstant), springLength(springLength), frictionConstant(frictionConstant)	// 构造函数
	{

		this->mass1 = mass1;													// 设置质量1
		this->mass2 = mass2;													// 设置质量2
	}

	void solve()																	// solve()方法：可以应用力的方法
	{
		Vector3D springVector = mass1->pos - mass2->pos;							// 两个质量之间的向量

		float r = springVector.length();											// 两个质量之间的距离

		Vector3D e, force;															// 力初始化为零值

		if (r != 0)																	// 为了避免除零错误，检查r是否为零
		{
			e = springVector / r;             // 计算单位向量
			force += e * (r - springLength) * (-springConstant);	// 弹簧力被添加到总力中，springlength是弹簧的正常长度，springconstant是弹簧的刚度

			force += -e * (mass1->vel * e - mass2->vel * e) * frictionConstant;						// 摩擦力被添加到总力中
			// 通过这个添加，我们得到弹簧的合力			
		}
		mass1->applyForce(force);													// 力被应用到质量1上
		mass2->applyForce(-force);													// 与力相反的力被应用到质量2上
	}

};



class RopeSimulation : public Simulation				// 一个模拟与平面表面和空气相互作用的绳子的对象
{
public:
	Spring** springs;									// 连接质量的弹簧（数量为[numOfMasses - 1]）

	Vector3D gravitation;								// 重力加速度（重力将应用于所有质量）

	Vector3D ropeConnectionPos;							// 空间中的一个点，用于设置系统中第一个质量（索引为0的质量）的位置

	Vector3D ropeConnectionVel;							// 一个变量，用于移动ropeConnectionPos（通过这个，我们可以摆动绳子）

	RopeSimulation(										// 一个带有11个参数的较长构造函数从这里开始
		int numOfMasses,								// 1. 质量的数量
		float m,										// 2. 每个质量的重量
		float springConstant,							// 3. 弹簧的刚度
		float springLength,								// 4. 弹簧不施加力时的长度
		float springFrictionConstant,					// 5. 弹簧内部摩擦常量
		Vector3D gravitation							// 重力加速度

	) : Simulation(numOfMasses, m)					// 超类创建每个重量为m的质量
	{
		this->gravitation = gravitation;

		int a;
		for (a = 0; a < numOfMasses; ++a)			// 为了设置质量的初始位置，使用for循环
		{
			masses[a]->pos.x = a * springLength;		// 设置质量masses[a]的x位置，与相邻质量的距离为springLength
			masses[a]->pos.y = 0;						// 将y位置设置为0，使其相对于地面水平放置
			masses[a]->pos.z = 0;						// 将z位置设置为0，使其看起来更简单
		}

		springs = new Spring * [numOfMasses - 1];			// 为弹簧创建[numOfMasses - 1]个指针
		// （对于numOfMasses个质量，需要[numOfMasses - 1]个弹簧）

		for (a = 0; a < numOfMasses - 1; ++a)			// 为了创建每个弹簧，开始一个循环
		{
			// 使用索引为“a”的质量和索引为“a + 1”的质量创建索引为“a”的弹簧
			springs[a] = new Spring(masses[a], masses[a + 1],
				springConstant, springLength, springFrictionConstant);
		}
	}

	void release()										// 重写release()方法，因为我们需要删除弹簧
	{
		Simulation::release();							// 让超类释放自身

		for (int a = 0; a < numOfMasses - 1; ++a)		// 为了删除所有弹簧，开始一个循环
		{
			delete(springs[a]);
			springs[a] = NULL;
		}

		delete(springs);
		springs = NULL;
	}

	void solve()										// 重写solve()方法，因为我们需要应用力
	{
		int a;
		for (a = 0; a < numOfMasses - 1; ++a)		// 应用所有弹簧的力
		{
			springs[a]->solve();						// 索引为“a”的弹簧应该应用它的力
		}

		for (a = 0; a < numOfMasses; ++a)				// 开始一个循环，应用对所有质量通用的力
		{
			masses[a]->applyForce(gravitation * masses[a]->m);				// 重力			
		}
	}

	void simulate(float dt)								// 重写simulate(float dt)方法，因为我们想要模拟
		// ropeConnectionPos的运动
	{
		Simulation::simulate(dt);						// 超类应该模拟质量的运动

		ropeConnectionPos += ropeConnectionVel * dt;	// 迭代ropeConnectionPos的位置

		masses[0]->pos = ropeConnectionPos;				// 索引为“0”的质量应该位于ropeConnectionPos的位置,开始位置
		masses[0]->vel = ropeConnectionVel;				// 该质量的速度设置为等于ropeConnectionVel
	}

	void setRopeConnectionVel(Vector3D ropeConnectionVel)	// 设置ropeConnectionVel的方法
	{
		this->ropeConnectionVel = ropeConnectionVel;
	}

	 void operate(float dt)					// 模拟的完整过程
	{
		init();										// 步骤 1：将力重置为零
		solve();									// 步骤 2：应用力
		simulate(dt);								// 步骤 3：根据时间变化迭代质量
	}

};

float muti = 5.0f;

class BendingSpring {
public:
	Mass* mass1;  // 第一个质点
	Mass* mass2;  // 中间质点
	Mass* mass3;  // 第三个质点
	float kb;     // 弯曲刚度系数
	float restAngle; // 初始角度（弧度）
	float currentAngle;
	Vector3D force1;
	Vector3D force2;
	float angleDiff;

	BendingSpring(Mass* m1, Mass* m2, Mass* m3, float stiffness)
		: mass1(m1), mass2(m2), mass3(m3), kb(stiffness) {
		// 计算并存储初始角度
		Vector3D v1 = m1->pos - m2->pos;
		Vector3D v2 = m1->pos -m3->pos;
		float dot = v1.unit().dot(v2.unit());
		// 确保点积在[-1,1]范围内，避免acos计算错误
		dot = std::max(-1.0f, std::min(1.0f, dot));
		restAngle = std::acos(dot);
	}
	void solve() {
		// 计算当前向量
		Vector3D v1 = mass1->pos - mass2->pos;
		Vector3D v2 = mass1->pos - mass3->pos;

		// 计算当前向量的长度和归一化
		float len1 = v1.length();
		float len2 = v2.length();
		if (len1 < 0.0001f || len2 < 0.0001f) return;

		Vector3D n1 = v1 / len1;
		Vector3D n2 = v2 / len2;

		// 计算当前角度
		float dot = n1.dot(n2);
		dot = std::max(-1.0f, std::min(1.0f, dot));
		currentAngle = std::acos(dot);

		if (currentAngle >= pi * 3 / 4 || currentAngle <= pi / 4)
		{
			if (abs(angleDiff) >= currentAngle - restAngle)
			{
				angleDiff = currentAngle - restAngle;
			}
			else
			{
				mass1->pos = mass1->lastpos;
				mass2->pos = mass2->lastpos;
				mass3->pos = mass3->lastpos;
				return;
			}

		}

		// 计算角度差
		 angleDiff = currentAngle - restAngle;

		// 计算扭矩（简化版，实际应基于惯性张量）
		float torque = kb * angleDiff;

		// 计算垂直于弯曲平面的法向量
		Vector3D normal = n1.cross(n2);
		float normalMag = normal.length();
		if (normalMag < 0.0001f) return; // 共线情况
		normal = normal / normalMag;

		// 计算作用在质点上的力
		// 这里使用简化模型：将扭矩转换为垂直于各自方向的力
		 force1 = n1.cross(normal) * (torque / len1)* muti;
		Vector3D force3 = normal.cross(n2) * (torque / len2)* muti;
		 force2 = -(force1 + force3);

		// 应用力
		mass2->applyForce(force1);
		mass3->applyForce(force2);
		mass1->applyForce(force3);
	}
};
