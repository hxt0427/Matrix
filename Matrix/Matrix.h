#pragma once
#include<iomanip>
#include<iostream>
using namespace std;
template <typename ElemType> //����Ԫ������
struct Matrix
{
	ElemType *data;//Ԫ���׵�ַ
	ElemType *datastart;//Ԫ�������ڴ���׵�ַ
	int rows, cols;//���ڳ����Ķ�ά���󣬴洢������
	int step;//�������е����ײ���
	int * refCount;//���ü���ָ��
	Matrix(int r = 3, int c = 3)
	{
		rows = r;
		cols = c;
		step = sizeof(ElemType)*cols;
		datastart = data = new ElemType[rows*step];
		printf("��þ����ڴ�� %x\n", datastart);
		refCount = new int(0);//�������飬���ü���Ϊ0
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
		retain();//�������ü���
	}
	Matrix &operator = (Matrix &m)//=��������
	{
		if (this != &m)
		{
			//�����ǰMat�Ѿ����ڣ��п���ָ���������ݣ���Ҫ���ͷŶ�������
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
		//�������ü��������߳�ģʽ��Ӧ�ü�������
		(*refCount)++;
		printf("����(%x)���ü�����Ϊ�� %d\n", datastart, refCount[0]);
	}
	void release()
	{
		//�������ü��������߳�ģʽ��Ӧ�ü�������
		if (refCount == NULL)
			return;
		(*refCount)--;
		printf("����(%x)���ü�����Ϊ�� %d\n", datastart, refCount[0]);
		if (*refCount == 0)//���ٱ����þ��ͷ�����
		{
			printf("�ͷž����ڴ�� %x\n", datastart);
			delete[] datastart;
			delete refCount;
			datastart = NULL;
			refCount == NULL;
		}
	}
	ElemType *ptr(int i)
	{
		//��ȡ����ָ��
		return data + i * step;
	}
	void output()
	{
		cout.setf(ios::right);//�Ҷ���
		for (int i = 0; i < rows; i++)//��ѭ����
		{
			ElemType *line = ptr(i);//ȡ���׵�ַ
			for (int j = 0; j < cols; j++)
				cout << setw(3) << line[j];
			cout << endl;
		}
		cout << endl;
	}
	static Matrix eye(int r)
	{
		//���쵥λ����
		Matrix result(r, r);
		for (int i = 0; i < r; i++)		//��ѭ����
		{
			ElemType *line = result.ptr(i);//ȡ���׵�ַ
			memset(line, 0, result.step);//��������
			line[i] = 1;//�Խ���Ԫ�ظ�ֵ1
		}
		return result;
	}
	Matrix clone()
	{
		Matrix result(rows, cols);
		for (int i = 0; i < rows; i++)//���и���
			memcpy(result.ptr(i), ptr(i), result.step);
		return result;
	}
	ElemType &at(int row1, int row2, int col1, int col2)
	{
		int r = row2 - row1 + 1;
		int c = col2 - col1 + 1;
		Matrix result = *this;		//��������
		result.data = &data[step*row1 + col1];	//���޸Ĳ�ͬ����
		result.rows = r;
		result.cols = c;
		return result;
	}
	Matrix operator+(Matrix &b)
	{
		//����ӷ�
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
		//����˷�

	}
};