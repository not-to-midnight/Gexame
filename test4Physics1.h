#pragma once
/**************************************************************************

�ļ�: Physics2.h

��Erkin TuncaΪnehe.gamedev.net׼��

**************************************************************************/
constexpr float pi = 3.14159265358979323846f;  // float�汾
#include "test4Physics2.h"									// Physics1.h����Physics2.h��ģ���Ǳ����

class Spring											// һ����ʾ�����ڲ�Ħ�����������������ĵ��ɵĶ��󡣸õ���
	// ��һ���������ȣ����ɲ�ʩ���κ���ʱ�ĳ��ȣ�
{
public:
	Mass* mass1;										// ����һ�˵ĵ�һ������
	Mass* mass2;										// ������һ�˵ĵڶ�������

	float springConstant;								// ��ʾ���ɸնȵĳ���
	float springLength;									// ���ɲ�ʩ���κ���ʱ�ĳ���
	float frictionConstant;								// ���ڵ����ڲ�Ħ���ĳ���

	Spring(Mass* mass1, Mass* mass2,
		float springConstant, float springLength, float frictionConstant):springConstant(springConstant), springLength(springLength), frictionConstant(frictionConstant)	// ���캯��
	{

		this->mass1 = mass1;													// ��������1
		this->mass2 = mass2;													// ��������2
	}

	void solve()																	// solve()����������Ӧ�����ķ���
	{
		Vector3D springVector = mass1->pos - mass2->pos;							// ��������֮�������

		float r = springVector.length();											// ��������֮��ľ���

		Vector3D e, force;															// ����ʼ��Ϊ��ֵ

		if (r != 0)																	// Ϊ�˱��������󣬼��r�Ƿ�Ϊ��
		{
			e = springVector / r;             // ���㵥λ����
			force += e * (r - springLength) * (-springConstant);	// ����������ӵ������У�springlength�ǵ��ɵ��������ȣ�springconstant�ǵ��ɵĸն�

			force += -e * (mass1->vel * e - mass2->vel * e) * frictionConstant;						// Ħ��������ӵ�������
			// ͨ�������ӣ����ǵõ����ɵĺ���			
		}
		mass1->applyForce(force);													// ����Ӧ�õ�����1��
		mass2->applyForce(-force);													// �����෴������Ӧ�õ�����2��
	}

};



class RopeSimulation : public Simulation				// һ��ģ����ƽ�����Ϳ����໥���õ����ӵĶ���
{
public:
	Spring** springs;									// ���������ĵ��ɣ�����Ϊ[numOfMasses - 1]��

	Vector3D gravitation;								// �������ٶȣ�������Ӧ��������������

	Vector3D ropeConnectionPos;							// �ռ��е�һ���㣬��������ϵͳ�е�һ������������Ϊ0����������λ��

	Vector3D ropeConnectionVel;							// һ�������������ƶ�ropeConnectionPos��ͨ����������ǿ��԰ڶ����ӣ�

	RopeSimulation(										// һ������11�������Ľϳ����캯�������￪ʼ
		int numOfMasses,								// 1. ����������
		float m,										// 2. ÿ������������
		float springConstant,							// 3. ���ɵĸն�
		float springLength,								// 4. ���ɲ�ʩ����ʱ�ĳ���
		float springFrictionConstant,					// 5. �����ڲ�Ħ������
		Vector3D gravitation							// �������ٶ�

	) : Simulation(numOfMasses, m)					// ���ഴ��ÿ������Ϊm������
	{
		this->gravitation = gravitation;

		int a;
		for (a = 0; a < numOfMasses; ++a)			// Ϊ�����������ĳ�ʼλ�ã�ʹ��forѭ��
		{
			masses[a]->pos.x = a * springLength;		// ��������masses[a]��xλ�ã������������ľ���ΪspringLength
			masses[a]->pos.y = 0;						// ��yλ������Ϊ0��ʹ������ڵ���ˮƽ����
			masses[a]->pos.z = 0;						// ��zλ������Ϊ0��ʹ�俴��������
		}

		springs = new Spring * [numOfMasses - 1];			// Ϊ���ɴ���[numOfMasses - 1]��ָ��
		// ������numOfMasses����������Ҫ[numOfMasses - 1]�����ɣ�

		for (a = 0; a < numOfMasses - 1; ++a)			// Ϊ�˴���ÿ�����ɣ���ʼһ��ѭ��
		{
			// ʹ������Ϊ��a��������������Ϊ��a + 1����������������Ϊ��a���ĵ���
			springs[a] = new Spring(masses[a], masses[a + 1],
				springConstant, springLength, springFrictionConstant);
		}
	}

	void release()										// ��дrelease()��������Ϊ������Ҫɾ������
	{
		Simulation::release();							// �ó����ͷ�����

		for (int a = 0; a < numOfMasses - 1; ++a)		// Ϊ��ɾ�����е��ɣ���ʼһ��ѭ��
		{
			delete(springs[a]);
			springs[a] = NULL;
		}

		delete(springs);
		springs = NULL;
	}

	void solve()										// ��дsolve()��������Ϊ������ҪӦ����
	{
		int a;
		for (a = 0; a < numOfMasses - 1; ++a)		// Ӧ�����е��ɵ���
		{
			springs[a]->solve();						// ����Ϊ��a���ĵ���Ӧ��Ӧ��������
		}

		for (a = 0; a < numOfMasses; ++a)				// ��ʼһ��ѭ����Ӧ�ö���������ͨ�õ���
		{
			masses[a]->applyForce(gravitation * masses[a]->m);				// ����			
		}
	}

	void simulate(float dt)								// ��дsimulate(float dt)��������Ϊ������Ҫģ��
		// ropeConnectionPos���˶�
	{
		Simulation::simulate(dt);						// ����Ӧ��ģ���������˶�

		ropeConnectionPos += ropeConnectionVel * dt;	// ����ropeConnectionPos��λ��

		masses[0]->pos = ropeConnectionPos;				// ����Ϊ��0��������Ӧ��λ��ropeConnectionPos��λ��,��ʼλ��
		masses[0]->vel = ropeConnectionVel;				// ���������ٶ�����Ϊ����ropeConnectionVel
	}

	void setRopeConnectionVel(Vector3D ropeConnectionVel)	// ����ropeConnectionVel�ķ���
	{
		this->ropeConnectionVel = ropeConnectionVel;
	}

	 void operate(float dt)					// ģ�����������
	{
		init();										// ���� 1����������Ϊ��
		solve();									// ���� 2��Ӧ����
		simulate(dt);								// ���� 3������ʱ��仯��������
	}

};

float muti = 5.0f;

class BendingSpring {
public:
	Mass* mass1;  // ��һ���ʵ�
	Mass* mass2;  // �м��ʵ�
	Mass* mass3;  // �������ʵ�
	float kb;     // �����ն�ϵ��
	float restAngle; // ��ʼ�Ƕȣ����ȣ�
	float currentAngle;
	Vector3D force1;
	Vector3D force2;
	float angleDiff;

	BendingSpring(Mass* m1, Mass* m2, Mass* m3, float stiffness)
		: mass1(m1), mass2(m2), mass3(m3), kb(stiffness) {
		// ���㲢�洢��ʼ�Ƕ�
		Vector3D v1 = m1->pos - m2->pos;
		Vector3D v2 = m1->pos -m3->pos;
		float dot = v1.unit().dot(v2.unit());
		// ȷ�������[-1,1]��Χ�ڣ�����acos�������
		dot = std::max(-1.0f, std::min(1.0f, dot));
		restAngle = std::acos(dot);
	}
	void solve() {
		// ���㵱ǰ����
		Vector3D v1 = mass1->pos - mass2->pos;
		Vector3D v2 = mass1->pos - mass3->pos;

		// ���㵱ǰ�����ĳ��Ⱥ͹�һ��
		float len1 = v1.length();
		float len2 = v2.length();
		if (len1 < 0.0001f || len2 < 0.0001f) return;

		Vector3D n1 = v1 / len1;
		Vector3D n2 = v2 / len2;

		// ���㵱ǰ�Ƕ�
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

		// ����ǶȲ�
		 angleDiff = currentAngle - restAngle;

		// ����Ť�أ��򻯰棬ʵ��Ӧ���ڹ���������
		float torque = kb * angleDiff;

		// ���㴹ֱ������ƽ��ķ�����
		Vector3D normal = n1.cross(n2);
		float normalMag = normal.length();
		if (normalMag < 0.0001f) return; // �������
		normal = normal / normalMag;

		// �����������ʵ��ϵ���
		// ����ʹ�ü�ģ�ͣ���Ť��ת��Ϊ��ֱ�ڸ��Է������
		 force1 = n1.cross(normal) * (torque / len1)* muti;
		Vector3D force3 = normal.cross(n2) * (torque / len2)* muti;
		 force2 = -(force1 + force3);

		// Ӧ����
		mass2->applyForce(force1);
		mass3->applyForce(force2);
		mass1->applyForce(force3);
	}
};
