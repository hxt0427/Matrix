#pragma once
#include<iostream>
#include<iomanip>
#include<memory>
#include"../../SeqList/SeqList/SeqList.h"
using namespace std;

template <typename ElemType>
struct Triplet {
	int row, col;//�����±�
	ElemType e;//����
	Triplet() {}//�����ṩĬ�Ϲ��캯����һ�ߴ����������
	Triplet(int r, int c, ElemType v)
	{
		row = r;
		col = c;
		e = v;
	}
};

template <typename ElemType>
struct SparseMatrix :SeqList<Triplet<ElemType>>
{
	int rows, cols;//���д�С
	SparseMatrix(int r, int c, int n = 10)
	{		//���캯��
		rows = r;
		cols = c;
	}
	int getIndex(int r, int c)
	{		//�����ܾ�����±�ת��Ϊϡ������е��±꣬�Ա����
		int i = 0;
		for (int i = 0; i < this->length; i++)
		{
			Triplet<ElemType> &t = this->items[i];
			if (t.row > r || t.row == r && t.col >= c)
				break;
		}
		return i;
	}
	ElemType get(int r, int c)
	{
		int index = getIndex(r, c);
		if (index < this->length &&  this->items[index].row == r && this->items[index].cols == c)
			return  this->items[index];
		else
			return ElemType();
	}
	void set(int r, int c, ElemType v)
	{
		int index = getIndex(r, c);
		if (index < this->length &&  this->items[index].row == r && this->items[index].cols == c)
			this->items[index] = v;
		else
			this->insertAt(index, Triplet<ElemType>(r, c, v));
	}
	void  output()
	{
		int k = 0;
		cout.setf(ios::right);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				ElemType v = ElemType();
				if (k < this->length && i == this->items[k].row && j == this->items[k].col)
				{
					v = this->items[k].e;
					k++;
				}
				cout << setw(4) << v;
			}
			cout << endl;
		}
		cout << endl;
	}
	void transpose()
	{
		int *pinshu = new int[sizeof(int)*cols];
		int *pinshu2 = new int[sizeof(int)*cols];		//�ۼ�
		for (int i = 0; i < cols; i++)	//��ʼ��
		{
			pinshu[i] = 0;
			pinshu2[i] = 0;
		}
			
		for (int i = 0; i < this->length; i++)		//Ƶ��
		{
			int c = this->items[i].col;
			pinshu[c]++;
		}
		for (int i = 0; i < cols-1; i++)
		{
			int c = pinshu[i];
			pinshu[i + 1] += c;
		}
		for (int i = 0; i < cols - 1; i++)
		{
			pinshu2[i + 1] += pinshu[i];
		}
		Triplet<ElemType> * news = new Triplet<ElemType>[sizeof(Triplet<ElemType>)*this->length];
		for (int i = 0; i < this->length; i++)
		{
			news[pinshu2[this->items[i].col]] = this->items[i];
			pinshu2 [this->items[i].col]++;
		}
		for (int i = 0; i < this->length; i++)
		{
			int cc = news[i].row;
			news[i].row = news[i].col;
			news[i].col = cc;
		}
		memcpy(this->items, news, this->length * sizeof(Triplet<ElemType>));
	}
};