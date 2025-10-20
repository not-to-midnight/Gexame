#pragma once
#include <vector>
#include "test4Physics1.h"				// Physics1.h对于Physics2.h的模拟是必需的
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

		cubeSimulation(//构造函数
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

			//设置位置
			for (int i = 0; i < x; i++) {
				for (int j = 0; j < y; j++) {
					for (int k = 0; k < z; k++) {

						int index = getIndex(i, j, k);
						masses[index] =  Mass(m); // 创建一个质量对象并存储在数组中

						masses[index].pos = Vector3D(i * springLength, 3+j * springLength, k * springLength); // 设置位置
					}
				}
			}
			addspring();
			//createBendingSprings(pi/2);
		}

		//定位到（i，j，k）处的mass点
		Mass toIndex(int i, int j, int k) {
			if(i<=numOfMasses&&j <= numOfMasses &&k <= numOfMasses)
				return masses[i * (y * z) + j * z + k];//返回在该位置的对象
			else return Mass(0); // 假设Mass有默认构造或此临时返回合理
		}

		//获取（i，j，k）处的质量点的索引
		int getIndex(int i, int j, int k) {//(0,0,1)->1....(1,0,0)->4*4..(0,1,0)->4
			return i * (y * z) + j * z + k;
		}

		void traverse(std::function<void(Mass&)> a) {
			for (int i = 0; i < x; i++) {
				for (int j = 0; j < y; j++) {
					for (int k = 0; k < z; k++) {
						int index = getIndex(i, j, k);
						a(masses[index]);//这个就相当于lambta中的mass参数，然后他会进行init操作
						//std::string str = "索引(" + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k) + ")";
						//std::cout << str << std::endl;
					}
				}
			}
		}
		int a = 0;
		void addspring() {
			springs = new Spring * [(numOfMasses - 1)* (numOfMasses )* (numOfMasses )*3];

			//z方向链接
			for (int i = 0; i < x; i++) {
				for (int j = 0; j < y; j++) {
					for (int k = 0; k < z-1; k++) {
						int index = getIndex(i, j, k);
							springs[a++] = new Spring(&masses[index], &masses[getIndex(i,j,k + 1 )], springConstant, springLength, frictionConstant);
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i  << ", " << j << ", " << k+1 << ")" << std::endl;
					}
				}
			}
			//x方向链接
					for (int i = 0; i < x-1 ; i++) {
						for (int j = 0; j < y ; j++) {
							for (int k = 0; k < z ; k++) {
								int index = getIndex(i, j, k);
								springs[a++] = new Spring(&masses[index], &masses[getIndex(i+1 ,j, k)], springConstant,springLength, frictionConstant);
								std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i+1<< ", " << j  << ", " << k << ")" << std::endl;
							}
						}
					}

					//y方向链接
					for (int i = 0; i < x ; i++) {
						for (int j = 0; j < y-1; j++) {
							for (int k = 0; k < z ; k++) {
								int index = getIndex(i, j, k);
								springs[a++] = new Spring(&masses[index], &masses[getIndex(i, j + 1, k)], springConstant, springLength, frictionConstant);
								std::cout<<"链接索引(" << i << "," << j << "," << k << ")到" << "(" <<i << ", " <<j+1<<  ", " <<k <<")"<<std::endl;

							}
						}
					}

					log("finish");
		}

		void createBendingSprings(float bendStiffness) {
			int size = numOfMasses;
			// 每个顶点需要处理三个正交平面的弯曲
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						int centerIdx = getIndex(i, j, k);

						if (i < size - 1 && j < size - 1) {  // 右-上
							int rightIdx = getIndex(i + 1, j, k);
							int upIdx = getIndex(i, j + 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[upIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i +1<< ", " << j << ", " << k << ")" << "(" << i << ", " << j +1<< ", " << k << ")" << std::endl;
						}
						if (i < size - 1 && j > 0) {  // 右-下
							int rightIdx = getIndex(i + 1, j, k);
							int downIdx = getIndex(i, j - 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[downIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i + 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j - 1 << ", " << k << ")" << std::endl;
						}
						if (i > 0 && j < size - 1) {  // 左-上
							int leftIdx = getIndex(i - 1, j, k);
							int upIdx = getIndex(i, j + 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[upIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i - 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j + 1 << ", " << k << ")" << std::endl;
						}
						if (i > 0 && j > 0) {  // 左-下
							int leftIdx = getIndex(i - 1, j, k);
							int downIdx = getIndex(i, j - 1, k);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[downIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i - 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j - 1 << ", " << k << ")" << std::endl;
						}

						// x-z平面弯曲（四个方向：右-前、右-后、左-前、左-后）
						if (i < size - 1 && k < size - 1) {  // 右-前
							int rightIdx = getIndex(i + 1, j, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i + 1 << ", " << j << ", " << k << ")" << "(" << i << ", " << j  << ", " << k+1 << ")" << std::endl;
						}
						if (i < size - 1 && k > 0) {  // 右-后
							int rightIdx = getIndex(i + 1, j, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[rightIdx], &masses[backIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i + 1 << ", " << j << ", " <<k << ")" << "(" << i << ", " << j  << ", " << k-1 << ")" << std::endl;
						}
						if (i > 0 && k < size - 1) {  // 左-前
							int leftIdx = getIndex(i - 1, j, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i -1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j  << ", " << k+1 << ")" << std::endl;
						}
						if (i > 0 && k > 0) {  // 左-后
							int leftIdx = getIndex(i - 1, j, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[leftIdx], &masses[backIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i - 1 << ", " << j << ", " << k  << ")" << "(" << i << ", " << j << ", " << k - 1 << ")" << std::endl;
						}

						// y-z平面弯曲（四个方向：上-前、上-后、下-前、下-后）
						if (j < size - 1 && k < size - 1) {  // 上-前
							int upIdx = getIndex(i, j + 1, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[upIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i  << ", " << j +1<< ", " << k  << ")" << "(" << i << ", " << j << ", " << k + 1 << ")" << std::endl;
						}
						if (j < size - 1 && k > 0) {  // 上-后
							int upIdx = getIndex(i, j + 1, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[upIdx], &masses[backIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i << ", " << j + 1 << ", " << k  << ")" << "(" << i << ", " << j << ", " << k - 1 << ")" << std::endl;
						}
						if (j > 0 && k < size - 1) {  // 下-前
							int downIdx = getIndex(i, j - 1, k);
							int forwardIdx = getIndex(i, j, k + 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[downIdx], &masses[forwardIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i << ", " << j -1 << ", " << k << ")" << "(" << i << ", " << j << ", " << k + 1 << ")" << std::endl;
						}
						if (j > 0 && k > 0) {  // 下-后
							int downIdx = getIndex(i, j - 1, k);
							int backIdx = getIndex(i, j, k - 1);
							bendings.push_back(std::make_shared<BendingSpring>(
								&masses[centerIdx], &masses[downIdx], &masses[backIdx], bendStiffness));
							std::cout << "链接索引(" << i << "," << j << "," << k << ")到" << "(" << i << ", " << j -1 << ", " << k << ")" << "(" << i << ", " << j << ", " << k - 1 << ")" << std::endl;
						}
						}
					}
				}
			}

		void init() {
			traverse([](Mass& mass) {//没有捕获变量，所以可以转换成指针函数传入
				mass.init();  // 调用Mass对象的init()方法
				});
		}

		void simulate(float dt) {//需要捕获变量，不能使用lambda
			traverse([dt](Mass& mass) {
				mass.simulate(dt);  // 调用Mass对象的simulate()方法
				});

			ropeConnectionPos += ropeConnectionVel * dt;	// 迭代ropeConnectionPos的位置
		
			masses[getIndex(0,numOfMasses-1,0)].pos = ropeConnectionPos;				// 索引为“0”的质量应该位于ropeConnectionPos的位置,开始位置
			masses[getIndex(0, numOfMasses - 1, 0)].vel = ropeConnectionVel;				// 该质量的速度设置为等于ropeConnectionVel
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

		void solve()										// 重写solve()方法，因为我们需要应用力
		{

			for (a = 0; a< ((numOfMasses - 1) * (numOfMasses) * (numOfMasses) * 3); ++a)		// 应用所有弹簧的力
			{
				springs[a]->solve();						// 索引为“a”的弹簧应该应用它的力
			}

			const size_t bendCount = bendings.size();
			for (size_t i = 0; i < bendCount; ++i) {
				bendings[i]->solve();
			}

			applyG();

		}

		void operate(float dt)					// 模拟的完整过程
		{
			init();										// 步骤 1：将力重置为零
			solve();									// 步骤 2：应用力
			simulate(dt);								// 步骤 3：根据时间变化迭代质量
			collision();
		}








private:
	int x, y, z;
	std::vector<Mass> masses;  // 存储所有质点
	std::vector<std::shared_ptr<BendingSpring>> bendings; // 存储所有弯曲弹簧
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
	std::vector<Mass> masses;  // 存储所有质点
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
		masses[0].pos = Vector3D(0, -1, 0); // 设置位置
		masses[1].pos = Vector3D(0, -1, 1); // 设置位置
		masses[2].pos = Vector3D(0, -2, 0); // 设置位置
		masses[3].pos = Vector3D(1, -1,0 ); // 设置位置
		springs = new Spring * [3]; 
		springs[0] = new Spring(&masses[0], &masses[1], springConstant, springLength, frictionConstant);
		springs[1] = new Spring(&masses[0], &masses[2], springConstant, springLength, frictionConstant);
		springs[2] = new Spring(&masses[0], &masses[3], springConstant, springLength, frictionConstant);
		
		bendings.push_back(std::make_shared<BendingSpring>(&masses[0],& masses[1],& masses[2], pi / 2));
		bendings.push_back(std::make_shared<BendingSpring>(&masses[0], &masses[1], &masses[3], pi / 2));
		bendings.push_back(std::make_shared<BendingSpring>(&masses[0], &masses[2], &masses[3], pi / 2));

	}
	void solve()										// 重写solve()方法，因为我们需要应用力
	{
		int a;
		for (a = 0; a < 3; ++a)		// 应用所有弹簧的力
		{
			springs[a]->solve();						// 索引为“a”的弹簧应该应用它的力
		}

		for (a = 0; a < 4; ++a)				// 开始一个循环，应用对所有质量通用的力
		{
			masses[a].applyForce(gravitation * masses[a].m);				// 重力			
		}

		for (int i = 0; i < bendings.size(); i++)
		{
			bendings[i]->solve();
			//if (i == 1)
			//{
			//	log("初始角度");
			//	logfloat(bendings[i]->restAngle / pi);
			//	log("当前角度");
			//	logfloat(bendings[i]->currentAngle / pi);
			//	log("弹簧力");
			//	log(bendings[i]->force1);
			//	log("弹簧力大小");
			//	logfloat(bendings[i]->force1.length());
			//	logfloat(bendings[i]->force2.length());

			//	log("重力");
			//	log(gravitation);

			//}
		}
	}

	 void init()								// 这个方法将调用每个质量的 init() 方法
	{
		for (int a = 0; a < 4; ++a)		// 我们将对每个质量调用 init() 方法
			masses[a].init();						// 调用质量的 init() 方法
	}

	void simulate(float dt)								// 重写simulate(float dt)方法，因为我们想要模拟
		// ropeConnectionPos的运动
	{
		for (int i = 0; i < 4; i++) {
				masses[i].simulate(dt);				// 迭代质量，获取新的位置和新的速度
		}

		

		ropeConnectionPos += ropeConnectionVel * dt;	// 迭代ropeConnectionPos的位置

		masses[0].pos = ropeConnectionPos;				// 索引为“0”的质量应该位于ropeConnectionPos的位置,开始位置
		masses[0].vel = ropeConnectionVel;				// 该质量的速度设置为等于ropeConnectionVel

	}

 void operate(float dt)					// 模拟的完整过程
	{
		init();										// 步骤 1：将力重置为零
		solve();									// 步骤 2：应用力
		simulate(dt);								// 步骤 3：根据时间变化迭代质量
	}
};