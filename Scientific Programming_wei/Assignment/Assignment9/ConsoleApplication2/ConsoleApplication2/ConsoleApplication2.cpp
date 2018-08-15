// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


/* 稀疏矩阵乘法，三元组法实现*/

#include <iostream>
#include <iomanip>
using namespace std;
const int MAX_SIZE = 100;     // 矩阵中非零元素的最多个数
const int LEN = 3;       // 矩阵中元素的对齐长度
						 // 当矩阵元素因为位数导致不能对齐的时候调整该值
struct Node
{
	int nRow, nCol;       // 三元组的行、列号
	int nVal;        // 元素的值
};
struct SparMatrix
{
	int nRows, nCols;      // 矩阵的行、列数
	int nTerms;        // 矩阵的非零元个数
	struct Node arrayData[MAX_SIZE];  // 存放矩阵非零元素的三元组数组
	void PrintMatrix();      // 输出矩阵
	int GetElement(int m, int n);   // 获得矩阵对应的元素
	void PrintInit();      // 矩阵的输入初始化 
	void AddElement(int m, int n, int nVal); // 增加非零元素
};

// 矩阵乘法函数(*new)记得释放内存。
SparMatrix* MatrixMulti(SparMatrix* pM1, SparMatrix* pM2);

void main()
{
	SparMatrix matrix1;
	cout << "The 1st matrix:" << endl;
	matrix1.PrintInit();
	SparMatrix matrix2;
	cout << "The 2nd matrix:" << endl;
	matrix2.PrintInit();
	cout << "Multiplication:" << endl;
	matrix1.PrintMatrix();
	cout << "*" << endl;
	matrix2.PrintMatrix();
	cout << "=" << endl;
	SparMatrix* pMatrixPro;
	pMatrixPro = MatrixMulti(&matrix1, &matrix2);
	if (pMatrixPro == NULL)
	{
		cout << "Error!" << endl;
	}
	else
	{
		pMatrixPro->PrintMatrix();
	}
	if (pMatrixPro != NULL)
	{
		delete pMatrixPro;
		pMatrixPro = NULL;
	}

}

void SparMatrix::PrintMatrix()
{
	// cout << "Print Matrix:" << endl;
	for (int m = 1; m <= nRows; m++)
	{
		cout << "|" << setw(LEN);
		for (int n = 1; n <= nCols; n++)
		{
			cout << GetElement(m, n);
			if (n == nCols)
			{
				break;
			}
			cout << "," << setw(LEN);
		}
		cout << "|" << endl;
	}
}

int SparMatrix::GetElement(int m, int n)
{
	for (int i = 0; i < nTerms; i++)
	{
		if (arrayData[i].nRow == m && arrayData[i].nCol == n)
		{
			return arrayData[i].nVal;
		}
	}
	return 0;
}

void SparMatrix::AddElement(int m, int n, int nVal)
{
	Node element = { m,n,nVal };
	arrayData[nTerms] = element;
	nTerms++;
}

void SparMatrix::PrintInit()
{
	nTerms = 0;
	int m, n;
	while (1)
	{
		cout << "Plz input the row and column num, using space to separate them:" << endl;
		cin >> m >> n;
		if (m > 0 && n > 0)
		{
			break;
		}
		cout << "The num is wrong. Plz input again." << endl;
		cin.clear();
		cin.sync();
	}

	nRows = m;
	nCols = n;

	cout << "Matrix: ( " << m << " , " << n << " )" << endl;
	int num = 0;
	int nVal = 0;
	char chTmp[10] = { 0 };
	while (1)
	{
		cout << "Plz input the " << num + 1 << " element. * to end:" << endl;
		cin >> chTmp;
		if (*chTmp == '*')
		{
			cout << "Over" << endl;
			break;
		}
		else
		{
			nVal = atoi(chTmp);
		}
		while (1)
		{
			cout << "Then input the row and colum of this element, using space to separate them:" << endl;
			cin >> m >> n;
			if (m > 0 && n > 0 && m <= nRows && n <= nCols)
			{
				break;
			}
			cout << "The num is wrong. Plz input again." << endl;
			cin.clear();
			cin.sync();
		}
		AddElement(m, n, nVal);
	}
}

SparMatrix* MatrixMulti(SparMatrix* pM1, SparMatrix* pM2)
{
	if (pM1->nCols != pM2->nRows)
	{
		return NULL;
	}
	SparMatrix* pro = new SparMatrix;
	pro->nTerms = 0;
	pro->nRows = pM1->nRows;
	pro->nCols = pM2->nCols;
	for (int m = 1; m <= pM1->nRows; m++)
	{
		for (int n = 1; n <= pM2->nCols; n++)
		{
			int nVol = 0;
			for (int p = 1; p <= pM1->nCols; p++)
			{
				int nPro = pM1->GetElement(m, p)*pM2->GetElement(p, n);
				nVol += nPro;
			}
			if (nVol != 0)
			{
				pro->AddElement(m, n, nVol);
			}
		}
	}

	return pro;
}
