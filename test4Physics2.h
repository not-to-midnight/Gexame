/**************************************************************************

文件: Physics1.h

由 Erkin Tunca 为 nehe.gamedev.net 准备

**************************************************************************/

#include <math.h>

// 类 Vector3D		---> 一个表示三维向量或空间中三维点的对象
class Vector3D
{
public:
	float x;									// 这个三维向量的 x 值
	float y;									// 这个三维向量的 y 值
	float z;									// 这个三维向量的 z 值

	Vector3D()									// 构造函数，将 x、y、z 设置为 0
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(float x, float y, float z)			// 构造函数，用指定的 x、y、z 值初始化这个三维向量
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D& operator= (Vector3D v)			// 赋值运算符 =，将 v 的值赋给这个三维向量。例如：v1 = v2 表示将 v2 的值赋给 v1
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3D operator+ (Vector3D v)				// 加法运算符 +，用于将两个三维向量相加。该运算符返回一个新的三维向量
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator- (Vector3D v)				// 减法运算符 -，用于计算两个三维向量的差。该运算符返回一个新的三维向量
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator* (Vector3D v)			// 乘法运算符 *，用于按一个向量缩放这个三维向量。这个值会乘以三维向量的 x、y 和 z
	{
		return Vector3D(x * v.x, y * v.y, z * v.z);
	}

	Vector3D& operator+= (Vector3D v)			// 复合加法运算符 +=，用于将另一个三维向量加到这个三维向量上
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D& operator-= (Vector3D v)			// 复合减法运算符 -=，用于从这个三维向量中减去另一个三维向量
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3D operator* (float value)			// 乘法运算符 *，用于按一个标量缩放这个三维向量。这个值会乘以三维向量的 x、y 和 z
	{ 
		return Vector3D(x * value, y * value, z * value);
	}


	Vector3D operator/ (float value)			// 除法运算符 /，用于按一个标量缩放这个三维向量。这个值会除以三维向量的 x、y 和 z
	{
		return Vector3D(x / value, y / value, z / value);
	}

	Vector3D& operator*= (float value)			// 复合乘法运算符 *=，用于按一个标量缩放这个三维向量
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3D& operator/= (float value)			// 复合除法运算符 /=，用于按一个标量缩放这个三维向量
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	Vector3D operator- ()						// 取反运算符 -，用于将这个三维向量的 x、y 和 z 取反
	{
		return Vector3D(-x, -y, -z);
	}

	float length() const								// length() 方法返回这个三维向量的长度
	{
		return sqrtf(x * x + y * y + z * z);
	};

	void unitize()								// unitize() 方法将这个三维向量归一化，即保持方向不变但长度变为 1
	{
		float length = this->length();

		if (length == 0)
			return;

		x /= length;
		y /= length;
		z /= length;
	}

	Vector3D unit()								// unit() 方法返回一个新的三维向量。返回的值是这个三维向量的归一化版本
	{
		float length = this->length();

		if (length == 0)
			return *this;

		return Vector3D(x / length, y / length, z / length);
	}

	float dot(const Vector3D& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	   Vector3D cross(const Vector3D& v) const {
        return Vector3D(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }


};

// 类 Mass			---> 一个表示质量的对象
class Mass
{
public:
	float m;									// 质量值
	Vector3D pos;								// 空间中的位置
	Vector3D vel;								// 速度
	Vector3D force;								// 某一时刻施加在这个质量上的力
	Vector3D lastpos;
	Mass()
	{
		m = 0;									// 初始化质量值为 0
		pos.x = 0; pos.y = 0; pos.z = 0;			// 初始化位置为 (0, 0, 0)
		vel.x = 0; vel.y = 0; vel.z = 0;			// 初始化速度为 (0, 0, 0)
		force.x = 0; force.y = 0; force.z = 0;	// 初始化力为 (0, 0, 0)
	}

	Mass(float m)								// 构造函数
	{
		this->m = m;
	}

	void applyForce(Vector3D force)
	{
		this->force += force;					// 将外力添加到质量所受的力上
	}

	void init()
	{
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}

	void simulate(float dt)
	{
		vel += (force / m) * dt;				// 将速度变化量添加到速度上。
		// 速度变化量与加速度（力 / 质量）和时间变化量成正比
		lastpos = pos;
		pos += vel * dt;						// 将位置变化量添加到位置上。
		// 位置变化量等于速度乘以时间变化量
	}

};

// 类 Simulation		---> 一个用于模拟多个质量的容器对象，里面有虚函数，所以在编译的时候会生成一个虚函数表
class Simulation
{
public:
	int numOfMasses;								// 这个容器中质量的数量
	Mass** masses;									// 质量由指针的指针来持有。（这里 Mass** 表示一个一维数组）

	Simulation() {
		int numOfMasses = 0;
		masses = NULL;// 这个容器中质量的数量
		std::cout << "Simulation()" << std::endl;
	}
	Simulation(int numOfMasses, float m)			// 构造函数，创建一些质量，质量值为 m
	{
		this->numOfMasses = numOfMasses;

		masses = new Mass * [numOfMasses];			// 创建一个指针数组

		for (int a = 0; a < numOfMasses; ++a)		// 我们将遍历数组中的每个指针
			masses[a] = new Mass(m);				// 创建一个 Mass 指针，并将其放入数组中
	}

	// 删除创建的质量
	virtual void release()							
	{
		for (int a = 0; a < numOfMasses; ++a)		// 我们将删除所有质量
		{
			delete(masses[a]);
			masses[a] = NULL;
		}

		delete(masses);
		masses = NULL;
	}

	Mass* getMass(int index)
	{
		if (index < 0 || index >= numOfMasses)		// 如果索引不在数组范围内
			return NULL;							// 则返回 NULL

		return masses[index];						// 获取指定索引处的质量
	}

	virtual void init()								// 这个方法将调用每个质量的 init() 方法
	{
		for (int a = 0; a < numOfMasses; ++a)		// 我们将对每个质量调用 init() 方法
			masses[a]->init();						// 调用质量的 init() 方法
	}

	virtual void solve()							// 无具体实现，因为在这个基本容器中不需要考虑力的作用
	{
		// 在更高级的容器中，这个方法将被重写，并且会有一些力作用在质量上
	}

	virtual void simulate(float dt)					// 根据时间变化迭代质量
	{
		for (int a = 0; a < numOfMasses; ++a)		// 我们将迭代每个质量
			masses[a]->simulate(dt);				// 迭代质量，获取新的位置和新的速度
	}


	/**
	 * @brief 模拟的完整过程。
	 * @param dt 时间变化量
	 */

	virtual void operate(float dt)					// 模拟的完整过程
	{
		init();										// 步骤 1：将力重置为零
		solve();									// 步骤 2：应用力
		simulate(dt);								// 步骤 3：根据时间变化迭代质量
	}

};