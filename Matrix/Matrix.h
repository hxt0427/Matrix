#pragma once
#include<iomanip>
#include<iostream>
using namespace std;
template <typename ElemType> //矩阵元素类型
struct Matrix
{
	ElemType *data;//元素首地址
	ElemType *datastart;//元素所在内存块首地址
	int rows, cols;//对于常见的二维矩阵，存储行列数
	int step;//相邻两行的行首步长
	int * refCount;//引用计数指针
	Matrix(int r = 3, int c = 3)
	{
		rows = r;
		cols = c;
		step = sizeof(ElemType)*cols;
		datastart = data = new ElemType[rows*step];
		printf("获得矩阵内存块 %x\n", datastart);
		refCount = new int(0);//对象数组，引用计数为0
		retain();
	}
	~Matrix()
	{
		release();
	}
	Matrix(Matrix &m)
	{
		rows = m.rows;
		cols = m.cols;
		step = m.step;
		data = m.data;
		datastart = m.datastart;
		refCount = m.refCount;
		retain();//增加引用计数
	}
	Matrix &operator = (Matrix &m)//=拷贝函数
	{
		if (this != &m)
		{
			//如果当前Mat已经存在，有可能指向其他数据，需要先释放对齐引用
			release();
			rows = m.rows;
			cols = m.cols;
			step = m.step;
			data = m.data;
			datastart = m.datastart;
			refCount = m.refCount;
			retain();
		}
		return *this;
	}
	void retain()
	{
		//增加引用计数，多线程模式下应该加锁访问
		(*refCount)++;
		printf("矩阵(%x)引用计数增为： %d\n", datastart, refCount[0]);
	}
	void release()
	{
		//减少引用计数，多线程模式下应该加锁访问
		if (refCount == NULL)
			return;
		(*refCount)--;
		printf("矩阵(%x)引用计数减为： %d\n", datastart, refCount[0]);
		if (*refCount == 0)//不再被引用就释放数据
		{
			printf("释放矩阵内存块 %x\n", datastart);
			delete[] datastart;
			delete refCount;
			datastart = NULL;
			refCount == NULL;
		}
	}
	ElemType *ptr(int i)
	{
		//获取行首指针
		return data + i * step;
	}
	void output()
	{
		cout.setf(ios::right);//右对齐
		for (int i = 0; i < rows; i++)//先循环行
		{
			ElemType *line = ptr(i);//取行首地址
			for (int j = 0; j < cols; j++)
				cout << setw(3) << line[j];
			cout << endl;
		}
		cout << endl;
	}
	static Matrix eye(int r)
	{
		//构造单位矩阵
		Matrix result(r, r);
		for (int i = 0; i < r; i++)		//先循环行
		{
			ElemType *line = result.ptr(i);//取行首地址
			memset(line, 0, result.step);//整行清零
			line[i] = 1;//对角线元素赋值1
		}
		return result;
	}
	Matrix clone()
	{
		Matrix result(rows, cols);
		for (int i = 0; i < rows; i++)//逐行复制
			memcpy(result.ptr(i), ptr(i), result.step);
		return result;
	}
	ElemType &at(int row1, int row2, int col1, int col2)
	{
		int r = row2 - row1 + 1;
		int c = col2 - col1 + 1;
		Matrix result = *this;		//先做拷贝
		result.data = &data[step*row1 + col1];	//在修改不同部分
		result.rows = r;
		result.cols = c;
		return result;
	}
	Matrix operator+(Matrix &b)
	{
		//矩阵加法
		Matrix a = clone();
		for (int i = 0; i < rows; i++)
		{
			ElemType *pa = a.ptr(i);
			ElemType *pb = b.ptr(i);
			for (int j = 0; j < cols; j++)
				pa[j] += pb[j];
		}
		return a;
	}
	Matrix operator*(Matrix & b)
	{
		//矩阵乘法

	}
};