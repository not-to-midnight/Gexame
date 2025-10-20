#pragma once
#include <vector>
#include "test4Physics1.h"				// Physics1.h����Physics2.h��ģ���Ǳ����
#include <string>
#include <functional>
bool change = true;

enum class Axis {
	X = 'x',
	Y = 'y',
	Z = 'z',
	Xf='Xf',
	Yf = 'Yf',
	Zf = 'Zf'
};

void log(std::string str) {
	std::cout << str << std::endl;
}

void log(int str) {
	std::cout << str << std::endl;
}

void logfloat(float str) {
	std::cout << str << std::endl;
}

void log(Vector3D a) {
	std::cout << '(' <<a.x<<','<<a.y<<','<<a.z<<')'<< std::endl;
}

class cubeSimulation :public Simulation
{
public:
	//declare
	int numOfMasses;
	Vector3D gravitation;
	Spring** springs;
	float springLength;
	float springConstant;
	float frictionConstant;

		cubeSimulation(//���캯��
			int num,//
			float m,
			float springLength,
			float springConstant,
			float frictionConstant,
			Vector3D gravitation,
			Vector3D ropeConnectionVel,
			Vector3D ropeConnectionPos, Vector3D ropeConnectionVel1,
			Vector3D ropeConnectionPos1, Vector3D ropeConnectionVel2,
			Vector3D ropeConnectionPos2, Vector3D ropeConnectionVel3,
			Vector3D ropeConnectionPos3
			//int x,int y,int z
		) :x(num), 
			y(num), 
			z(num), 
			masses(num*num*num), 
			numOfMasses(num), 
			gravitation(gravitation), 
			springLength(springLength),
			Simulation(),
			ropeConnectionVel(ropeConnectionVel),
			ropeConnectionPos(ropeConnectionPos),
			ropeConnectionVel1(ropeConnectionVel1),
			ropeConnectionPos1(ropeConnectionPos1),
			ropeConnectionVel2(ropeConnectionVel2),
			ropeConnectionPos2(ropeConnectionPos2),
			ropeConnectionVel3(ropeConnectionVel3),
			ropeConnectionPos3(ropeConnectionPos3),
			springConstant(springConstant),
			frictionConstant(frictionConstant)
		{

			//����λ��
			for (int i = 0; i < x; i++) {
				for (int j = 0; j < y; j++) {
					for (int k = 0; k < z; k++) {

						int index = getIndex(i, j, k);
						masses[index] =  Mass(m); // ����һ���������󲢴洢��������

						masses[index].pos = Vector3D(i * springLength, 3+j * springLength, k * springLength); // ����λ��
					}
				}
			}
			addspring();
			//createBendingSprings(pi/2);
		}

		//��λ����i��j��k������mass��
		Mass toIndex(int i, int j, int k) {
			if(i<=numOfMasses&&j <= numOfMasses &&k <= numOfMasses)
				return masses[i * (y * z) + j * z + k];//�����ڸ�λ�õĶ���
			else return Mass(0); // ����Mass��Ĭ�Ϲ�������ʱ���غ���
		}

		//��ȡ��i��j��k�����������������
		int getIndex(int i, int j, int k) {//(0,0,1)->1....(1,0,0)->4*4..(0,1,0)->4
			return i * (y * z) + j * z + k;
		}

		void traverse(std::function<void(Mass&)> a) {
			for (int i = 0; i < x; i++) {
				for (int j = 0; j < y; j++) {
					for (int k = 0; k < z; k++) {
						int index = getIndex(i, j, k);
						a(masses[index]);//������൱��lambta�е�mass������Ȼ���������init����
						//std::string str = "����(" + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k) + ")";
						//std::cout << str << std::endl;
					}
				}
			}
		}
		int a = 0;
		void addspring() {
			springs = new Spring * [(numOfMasses - 1)* (numOfMasses )* (numOfMasses )*3];

			//z��������
			for (int i = 0; i < x; i++) {
				for (int j = 0; j < y; j++) {
					for (int k = 0; k < z-1; k++) {
						int index = getIndex(i, j, k);
							springs[a++] = new Spring(&masses[index], &masses[getIndex(i,j,k + 1 )], springConstant, springLength, frictionConstant);
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i  << ", " << j << ", " << k+1 << ")" << std::endl;
					}
				}
			}
			//x��������
					for (int i = 0; i < x-1 ; i++) {
						for (int j = 0; j < y ; j++) {
							for (int k = 0; k < z ; k++) {
								int index = getIndex(i, j, k);
								springs[a++] = new Spring(&masses[index], &masses[getIndex(i+1 ,j, k)], springConstant,springLength, frictionConstant);
								std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i+1<< ", " << j  << ", " << k << ")" << std::endl;
							}
						}
					}

					//y��������
					for (int i = 0; i < x ; i++) {
						for (int j = 0; j < y-1; j++) {
							for (int k = 0; k < z ; k++) {
								int index = getIndex(i, j, k);
								springs[a++] = new Spring(&masses[index], &masses[getIndex(i, j + 1, k)], springConstant, springLength, frictionConstant);
								std::cout<<"��������(" << i << "," << j << "," << k << ")��" << "(" <<i << ", " <<j+1<<  ", " <<k <<")"<<std::endl;

							}
						}
					}

					log("finish");
		}

		void createBendingSprings(float bendStiffness) {
			int size = numOfMasses;
			// ÿ��������Ҫ������������ƽ�������
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						int centerIdx = getIndex(i, j, k);

						if (i < size - 1 && j < size - 1) {  // ��-��
							int rightIdx = getIndex(i + 1, j, k);
							int upIdx = getIndex(i, j + 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[upIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i +1<< ", " << j << ", " << k << ")" << "(" << i << ", " << j +1<< ", " << k << ")" << std::endl;
						}
						if (i < size - 1 && j > 0) {  // ��-��
							int rightIdx = getIndex(i + 1, j, k);
							int downIdx = getIndex(i, j - 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[downIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i + 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j - 1 << ", " << k << ")" << std::endl;
						}
						if (i > 0 && j < size - 1) {  // ��-��
							int leftIdx = getIndex(i - 1, j, k);
							int upIdx = getIndex(i, j + 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[upIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i - 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j + 1 << ", " << k << ")" << std::endl;
						}
						if (i > 0 && j > 0) {  // ��-��
							int leftIdx = getIndex(i - 1, j, k);
							int downIdx = getIndex(i, j - 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[downIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i - 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j - 1 << ", " << k << ")" << std::endl;
						}

						// x-zƽ���������ĸ�������-ǰ����-����-ǰ����-��
						if (i < size - 1 && k < size - 1) {  // ��-ǰ
							int rightIdx = getIndex(i + 1, j, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i + 1 << ", " << j << ", " << k << ")" << "(" << i << ", " << j  << ", " << k+1 << ")" << std::endl;
						}
						if (i < size - 1 && k > 0) {  // ��-��
							int rightIdx = getIndex(i + 1, j, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[backIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i + 1 << ", " << j << ", " <<k << ")" << "(" << i << ", " << j  << ", " << k-1 << ")" << std::endl;
						}
						if (i > 0 && k < size - 1) {  // ��-ǰ
							int leftIdx = getIndex(i - 1, j, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i -1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j  << ", " << k+1 << ")" << std::endl;
						}
						if (i > 0 && k > 0) {  // ��-��
							int leftIdx = getIndex(i - 1, j, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[backIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i - 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j << ", " << k - 1 << ")" << std::endl;
						}

						// y-zƽ���������ĸ�������-ǰ����-����-ǰ����-��
						if (j < size - 1 && k < size - 1) {  // ��-ǰ
							int upIdx = getIndex(i, j + 1, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[upIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i  << ", " << j +1<< ", " << k  << ")" << "(" << i << ", " << j << ", " << k + 1 << ")" << std::endl;
						}
						if (j < size - 1 && k > 0) {  // ��-��
							int upIdx = getIndex(i, j + 1, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[upIdx], &masses[backIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i << ", " << j + 1 << ", " << k  << ")" << "(" << i << ", " << j << ", " << k - 1 << ")" << std::endl;
						}
						if (j > 0 && k < size - 1) {  // ��-ǰ
							int downIdx = getIndex(i, j - 1, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[downIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i << ", " << j -1 << ", " << k << ")" << "(" << i << ", " << j << ", " << k + 1 << ")" << std::endl;
						}
						if (j > 0 && k > 0) {  // ��-��
							int downIdx = getIndex(i, j - 1, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[downIdx], &masses[backIdx], bendStiffness));
							std::cout << "��������(" << i << "," << j << "," << k << ")��" << "(" << i << ", " << j -1 << ", " << k << ")" << "(" << i << ", " << j << ", " << k - 1 << ")" << std::endl;
						}
						}
					}
				}
			}

		void init() {
			traverse([](Mass& mass) {//û�в�����������Կ���ת����ָ�뺯������
				mass.init();  // ����Mass�����init()����
				});
		}

		void simulate(float dt) {//��Ҫ�������������ʹ��lambda
			traverse([dt](Mass& mass) {
				mass.simulate(dt);  // ����Mass�����simulate()����
				});

			ropeConnectionPos += ropeConnectionVel * dt;	// ����ropeConnectionPos��λ��
		
			masses[getIndex(0,numOfMasses-1,0)].pos = ropeConnectionPos;				// ����Ϊ��0��������Ӧ��λ��ropeConnectionPos��λ��,��ʼλ��
			masses[getIndex(0, numOfMasses - 1, 0)].vel = ropeConnectionVel;				// ���������ٶ�����Ϊ����ropeConnectionVel
			masses[getIndex(0, numOfMasses - 1, numOfMasses-1)].pos = ropeConnectionPos1;
			masses[getIndex(0, numOfMasses - 1, numOfMasses - 1)].vel = ropeConnectionVel1;
			if (change)
			{
				masses[getIndex(numOfMasses - 1, numOfMasses - 1, 0)].pos = ropeConnectionPos2;
				masses[getIndex(numOfMasses - 1, numOfMasses - 1, 0)].vel = ropeConnectionVel2;
				masses[getIndex(numOfMasses - 1, numOfMasses - 1, numOfMasses - 1)].pos = ropeConnectionPos3;
				masses[getIndex(numOfMasses - 1, numOfMasses - 1, numOfMasses - 1)].vel = ropeConnectionVel3;
			}
	}

		void collision() {
			traverse([](Mass& mass) {if (mass.pos.y < -5)mass.pos.y = -5; });
		}

		void applyG() {

			traverse([](Mass& mass) {
				mass.applyForce(Vector3D(0, -9.81f, 0) * mass.m);
				});
		}

		void solve()										// ��дsolve()��������Ϊ������ҪӦ����
		{

			for (a = 0; a< ((numOfMasses - 1) * (numOfMasses) * (numOfMasses) * 3); ++a)		// Ӧ�����е��ɵ���
			{
				springs[a]->solve();						// ����Ϊ��a���ĵ���Ӧ��Ӧ��������
			}

			const size_t bendCount = bendings.size();
			for (size_t i = 0; i < bendCount; ++i) {
				bendings[i]->solve();
			}

			applyG();

		}

		void operate(float dt)					// ģ�����������
		{
			init();										// ���� 1����������Ϊ��
			solve();									// ���� 2��Ӧ����
			simulate(dt);								// ���� 3������ʱ��仯��������
			collision();
		}








private:
	int x, y, z;
	std::vector<Mass> masses;  // �洢�����ʵ�
	std::vector<std::shared_ptr<BendingSpring>> bendings; // �洢������������
	Vector3D ropeConnectionPos;
	Vector3D ropeConnectionVel;
	Vector3D ropeConnectionVel1;
	Vector3D ropeConnectionPos1;
	Vector3D ropeConnectionVel2;
	Vector3D ropeConnectionPos2; 
	Vector3D ropeConnectionVel3;
	Vector3D ropeConnectionPos3;
};

class test :public Simulation
{
public:

	Vector3D gravitation;
	Spring** springs;
	float springLength;
	float springConstant;
	float frictionConstant;
	Vector3D ropeConnectionPos;
	Vector3D ropeConnectionVel;
	std::vector<Mass> masses;  // �洢�����ʵ�
	std::vector<std::shared_ptr<BendingSpring>> bendings;
	test(

		float m,
		float springLength,
		float springConstant,
		float frictionConstant,
		Vector3D gravitation,
		Vector3D ropeConnectionVel,
		Vector3D ropeConnectionPos
		
	) :
		gravitation(gravitation),
		springLength(springLength),
		springConstant(springConstant),
		frictionConstant(frictionConstant),
		ropeConnectionVel(ropeConnectionVel),
		ropeConnectionPos(ropeConnectionPos),
		
		masses(4)
	{
		for (int i = 0; i < 4; i++) {
			masses[i] =  Mass(m);
			
		}
		masses[0].pos = Vector3D(0, -1, 0); // ����λ��
		masses[1].pos = Vector3D(0, -1, 1); // ����λ��
		masses[2].pos = Vector3D(0, -2, 0); // ����λ��
		masses[3].pos = Vector3D(1, -1,0 ); // ����λ��
		springs = new Spring * [3]; 
		springs[0] = new Spring(&masses[0], &masses[1], springConstant, springLength, frictionConstant);
		springs[1] = new Spring(&masses[0], &masses[2], springConstant, springLength, frictionConstant);
		springs[2] = new Spring(&masses[0], &masses[3], springConstant, springLength, frictionConstant);
		
		bendings.push_back(std::make_shared<BendingSpring>(&masses[0],& masses[1],& masses[2], pi / 2));
		bendings.push_back(std::make_shared<BendingSpring>(&masses[0], &masses[1], &masses[3], pi / 2));
		bendings.push_back(std::make_shared<BendingSpring>(&masses[0], &masses[2], &masses[3], pi / 2));

	}
	void solve()										// ��дsolve()��������Ϊ������ҪӦ����
	{
		int a;
		for (a = 0; a < 3; ++a)		// Ӧ�����е��ɵ���
		{
			springs[a]->solve();						// ����Ϊ��a���ĵ���Ӧ��Ӧ��������
		}

		for (a = 0; a < 4; ++a)				// ��ʼһ��ѭ����Ӧ�ö���������ͨ�õ���
		{
			masses[a].applyForce(gravitation * masses[a].m);				// ����			
		}

		for (int i = 0; i < bendings.size(); i++)
		{
			bendings[i]->solve();
			//if (i == 1)
			//{
			//	log("��ʼ�Ƕ�");
			//	logfloat(bendings[i]->restAngle / pi);
			//	log("��ǰ�Ƕ�");
			//	logfloat(bendings[i]->currentAngle / pi);
			//	log("������");
			//	log(bendings[i]->force1);
			//	log("��������С");
			//	logfloat(bendings[i]->force1.length());
			//	logfloat(bendings[i]->force2.length());

			//	log("����");
			//	log(gravitation);

			//}
		}
	}

	 void init()								// �������������ÿ�������� init() ����
	{
		for (int a = 0; a < 4; ++a)		// ���ǽ���ÿ���������� init() ����
			masses[a].init();						// ���������� init() ����
	}

	void simulate(float dt)								// ��дsimulate(float dt)��������Ϊ������Ҫģ��
		// ropeConnectionPos���˶�
	{
		for (int i = 0; i < 4; i++) {
				masses[i].simulate(dt);				// ������������ȡ�µ�λ�ú��µ��ٶ�
		}

		

		ropeConnectionPos += ropeConnectionVel * dt;	// ����ropeConnectionPos��λ��

		masses[0].pos = ropeConnectionPos;				// ����Ϊ��0��������Ӧ��λ��ropeConnectionPos��λ��,��ʼλ��
		masses[0].vel = ropeConnectionVel;				// ���������ٶ�����Ϊ����ropeConnectionVel

	}

 void operate(float dt)					// ģ�����������
	{
		init();										// ���� 1����������Ϊ��
		solve();									// ���� 2��Ӧ����
		simulate(dt);								// ���� 3������ʱ��仯��������
	}
};