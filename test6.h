#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float * tmp=x0;x0=x;x=tmp;}
#define FOR_EACH_CELL for ( i=1 ; i<=N ; i++ ) { for ( j=1 ; j<=N ; j++ ) {
#define END_FOR }}

//x是当前时刻的速度场，s是上一个时刻的速度场
void add_source(int N, float* x, float* s, float dt)
{
	int i, size = (N + 2) * (N + 2);
	for (i = 0; i < size; i++) x[i] += dt * s[i];
}

void set_bnd(int N, int b, float* x)
{
	int i, j;
	int block_x1 = N * 0.4, block_x2 = N * 0.6;  // 方块障碍物的x范围
	int block_y1 = N * 0.2, block_y2 = N * 0.8;  // 方块障碍物的y范围

	// 处理四个边界（左右上下）
	for (i = 1; i <= N; i++) {
		x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];  // 左边界
		x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];  // 右边界
		x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];  // 下边界
		x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];  // 上边界
	}

	// 处理四个角点
	x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);

	// 设置方块障碍物内部的值为0（速度场）或外部值（密度场）
	if (b == 1 || b == 2) {  // 速度场
		for (i = block_x1; i <= block_x2; i++) {
			for (j = block_y1; j <= block_y2; j++) {
				x[IX(i, j)] = 0;  // 障碍物内部速度为0（无滑移）
			}
		}
		// 设置障碍物边界的速度（反弹效果）
		for (i = block_x1; i <= block_x2; i++) {
			x[IX(i, block_y1 - 1)] = -x[IX(i, block_y1)];  // 下边界
			x[IX(i, block_y2 + 1)] = -x[IX(i, block_y2)];  // 上边界
		}
		for (j = block_y1; j <= block_y2; j++) {
			x[IX(block_x1 - 1, j)] = -x[IX(block_x1, j)];  // 左边界
			x[IX(block_x2 + 1, j)] = -x[IX(block_x2, j)];  // 右边界
		}
	}
	else {  // 密度场或压力场
		for (i = block_x1; i <= block_x2; i++) {
			for (j = block_y1; j <= block_y2; j++) {
				// 障碍物内部密度保持外部值（可选：设为0表示完全阻挡）
				x[IX(i, j)] = x[IX(block_x1 - 1, j)];
			}
		}
	}
}

/* 线性方程组求解（Gauss-Seidel迭代法） */
void lin_solve(int N, int b, float* x, float* x0, float a, float c)
{
	int i, j, k;

	for (k = 0; k < 20; k++) {
		FOR_EACH_CELL
			x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] + x[IX(i, j - 1)] + x[IX(i, j + 1)])) / c;
		END_FOR
			set_bnd(N, b, x);
	}
}

//x0是要添加的速度场，x是上一个时刻的速度场
void diffuse(int N, int b, float* x, float* x0, float diff, float dt)
{
	float a = dt * diff * N * N;
	lin_solve(N, b, x, x0, a, 1 + 4 * a);
}

void advect(int N, int b, float* d, float* d0, float* u, float* v, float dt)
{
	int i, j, i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;

	dt0 = dt * N;
	FOR_EACH_CELL
		x = i - dt0 * u[IX(i, j)]; y = j - dt0 * v[IX(i, j)];
	if (x < 0.5f) x = 0.5f; if (x > N + 0.5f) x = N + 0.5f; i0 = (int)x; i1 = i0 + 1;
	if (y < 0.5f) y = 0.5f; if (y > N + 0.5f) y = N + 0.5f; j0 = (int)y; j1 = j0 + 1;
	s1 = x - i0; s0 = 1 - s1; t1 = y - j0; t0 = 1 - t1;
	d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
		s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
	END_FOR
		set_bnd(N, b, d);
}

void project(int N, float* u, float* v, float* p, float* div)
{
	int i, j;

	FOR_EACH_CELL
		div[IX(i, j)] = -0.5f * (u[IX(i + 1, j)] - u[IX(i - 1, j)] + v[IX(i, j + 1)] - v[IX(i, j - 1)]) / N;
	p[IX(i, j)] = 0;
	END_FOR
		set_bnd(N, 0, div); set_bnd(N, 0, p);

	lin_solve(N, 0, p, div, 1, 4);

	FOR_EACH_CELL
		u[IX(i, j)] -= 0.5f * N * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
	v[IX(i, j)] -= 0.5f * N * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
	END_FOR
		set_bnd(N, 1, u); set_bnd(N, 2, v);
}

//x是当前时刻的密度场，x0是要加的密度场
void dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt)
{
	add_source(N, x, x0, dt);
	SWAP(x0, x); diffuse(N, 0, x, x0, diff, dt);
	SWAP(x0, x); advect(N, 0, x, x0, u, v, dt);
}

//u0和v0是上一个时刻的速度场
void vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt)
{
	add_source(N, u, u0, dt); add_source(N, v, v0, dt);  // 添加外部力源

	// 扩散步骤（处理流体粘性）
	SWAP(u0, u); diffuse(N, 1, u, u0, visc, dt);
	SWAP(v0, v); diffuse(N, 2, v, v0, visc, dt);

	//u是当前时刻的速度场，u0是上一个时刻的速度场
	// 第一次投影（确保初始速度场无散度）
	project(N, u, v, u0, v0);

	// 对流步骤（速度场自我输送）
	SWAP(u0, u); SWAP(v0, v);
	advect(N, 1, u, u0, u0, v0, dt); advect(N, 2, v, v0, u0, v0, dt);

	// 第二次投影（确保最终速度场无散度）
	project(N, u, v, u0, v0);
}

