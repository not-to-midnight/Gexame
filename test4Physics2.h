/**************************************************************************

�ļ�: Physics1.h

�� Erkin Tunca Ϊ nehe.gamedev.net ׼��

**************************************************************************/

#include <math.h>

// �� Vector3D		---> һ����ʾ��ά������ռ�����ά��Ķ���
class Vector3D
{
public:
	float x;									// �����ά������ x ֵ
	float y;									// �����ά������ y ֵ
	float z;									// �����ά������ z ֵ

	Vector3D()									// ���캯������ x��y��z ����Ϊ 0
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(float x, float y, float z)			// ���캯������ָ���� x��y��z ֵ��ʼ�������ά����
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D& operator= (Vector3D v)			// ��ֵ����� =���� v ��ֵ���������ά���������磺v1 = v2 ��ʾ�� v2 ��ֵ���� v1
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3D operator+ (Vector3D v)				// �ӷ������ +�����ڽ�������ά������ӡ������������һ���µ���ά����
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator- (Vector3D v)				// ��������� -�����ڼ���������ά�����Ĳ�����������һ���µ���ά����
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator* (Vector3D v)			// �˷������ *�����ڰ�һ���������������ά���������ֵ�������ά������ x��y �� z
	{
		return Vector3D(x * v.x, y * v.y, z * v.z);
	}

	Vector3D& operator+= (Vector3D v)			// ���ϼӷ������ +=�����ڽ���һ����ά�����ӵ������ά������
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D& operator-= (Vector3D v)			// ���ϼ�������� -=�����ڴ������ά�����м�ȥ��һ����ά����
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3D operator* (float value)			// �˷������ *�����ڰ�һ���������������ά���������ֵ�������ά������ x��y �� z
	{ 
		return Vector3D(x * value, y * value, z * value);
	}


	Vector3D operator/ (float value)			// ��������� /�����ڰ�һ���������������ά���������ֵ�������ά������ x��y �� z
	{
		return Vector3D(x / value, y / value, z / value);
	}

	Vector3D& operator*= (float value)			// ���ϳ˷������ *=�����ڰ�һ���������������ά����
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3D& operator/= (float value)			// ���ϳ�������� /=�����ڰ�һ���������������ά����
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	Vector3D operator- ()						// ȡ������� -�����ڽ������ά������ x��y �� z ȡ��
	{
		return Vector3D(-x, -y, -z);
	}

	float length() const								// length() �������������ά�����ĳ���
	{
		return sqrtf(x * x + y * y + z * z);
	};

	void unitize()								// unitize() �����������ά������һ���������ַ��򲻱䵫���ȱ�Ϊ 1
	{
		float length = this->length();

		if (length == 0)
			return;

		x /= length;
		y /= length;
		z /= length;
	}

	Vector3D unit()								// unit() ��������һ���µ���ά���������ص�ֵ�������ά�����Ĺ�һ���汾
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

// �� Mass			---> һ����ʾ�����Ķ���
class Mass
{
public:
	float m;									// ����ֵ
	Vector3D pos;								// �ռ��е�λ��
	Vector3D vel;								// �ٶ�
	Vector3D force;								// ĳһʱ��ʩ������������ϵ���
	Vector3D lastpos;
	Mass()
	{
		m = 0;									// ��ʼ������ֵΪ 0
		pos.x = 0; pos.y = 0; pos.z = 0;			// ��ʼ��λ��Ϊ (0, 0, 0)
		vel.x = 0; vel.y = 0; vel.z = 0;			// ��ʼ���ٶ�Ϊ (0, 0, 0)
		force.x = 0; force.y = 0; force.z = 0;	// ��ʼ����Ϊ (0, 0, 0)
	}

	Mass(float m)								// ���캯��
	{
		this->m = m;
	}

	void applyForce(Vector3D force)
	{
		this->force += force;					// ��������ӵ��������ܵ�����
	}

	void init()
	{
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}

	void simulate(float dt)
	{
		vel += (force / m) * dt;				// ���ٶȱ仯����ӵ��ٶ��ϡ�
		// �ٶȱ仯������ٶȣ��� / ��������ʱ��仯��������
		lastpos = pos;
		pos += vel * dt;						// ��λ�ñ仯����ӵ�λ���ϡ�
		// λ�ñ仯�������ٶȳ���ʱ��仯��
	}

};

// �� Simulation		---> һ������ģ�������������������������麯���������ڱ����ʱ�������һ���麯����
class Simulation
{
public:
	int numOfMasses;								// �������������������
	Mass** masses;									// ������ָ���ָ�������С������� Mass** ��ʾһ��һά���飩

	Simulation() {
		int numOfMasses = 0;
		masses = NULL;// �������������������
		std::cout << "Simulation()" << std::endl;
	}
	Simulation(int numOfMasses, float m)			// ���캯��������һЩ����������ֵΪ m
	{
		this->numOfMasses = numOfMasses;

		masses = new Mass * [numOfMasses];			// ����һ��ָ������

		for (int a = 0; a < numOfMasses; ++a)		// ���ǽ����������е�ÿ��ָ��
			masses[a] = new Mass(m);				// ����һ�� Mass ָ�룬���������������
	}

	// ɾ������������
	virtual void release()							
	{
		for (int a = 0; a < numOfMasses; ++a)		// ���ǽ�ɾ����������
		{
			delete(masses[a]);
			masses[a] = NULL;
		}

		delete(masses);
		masses = NULL;
	}

	Mass* getMass(int index)
	{
		if (index < 0 || index >= numOfMasses)		// ��������������鷶Χ��
			return NULL;							// �򷵻� NULL

		return masses[index];						// ��ȡָ��������������
	}

	virtual void init()								// �������������ÿ�������� init() ����
	{
		for (int a = 0; a < numOfMasses; ++a)		// ���ǽ���ÿ���������� init() ����
			masses[a]->init();						// ���������� init() ����
	}

	virtual void solve()							// �޾���ʵ�֣���Ϊ��������������в���Ҫ������������
	{
		// �ڸ��߼��������У��������������д�����һ���һЩ��������������
	}

	virtual void simulate(float dt)					// ����ʱ��仯��������
	{
		for (int a = 0; a < numOfMasses; ++a)		// ���ǽ�����ÿ������
			masses[a]->simulate(dt);				// ������������ȡ�µ�λ�ú��µ��ٶ�
	}


	/**
	 * @brief ģ����������̡�
	 * @param dt ʱ��仯��
	 */

	virtual void operate(float dt)					// ģ�����������
	{
		init();										// ���� 1����������Ϊ��
		solve();									// ���� 2��Ӧ����
		simulate(dt);								// ���� 3������ʱ��仯��������
	}

};