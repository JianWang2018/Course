// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


/* ϡ�����˷�����Ԫ�鷨ʵ��*/

#include <iostream>
#include <iomanip>
using namespace std;
const int MAX_SIZE = 100;     // �����з���Ԫ�ص�������
const int LEN = 3;       // ������Ԫ�صĶ��볤��
						 // ������Ԫ����Ϊλ�����²��ܶ����ʱ�������ֵ
struct Node
{
	int nRow, nCol;       // ��Ԫ����С��к�
	int nVal;        // Ԫ�ص�ֵ
};
struct SparMatrix
{
	int nRows, nCols;      // ������С�����
	int nTerms;        // ����ķ���Ԫ����
	struct Node arrayData[MAX_SIZE];  // ��ž������Ԫ�ص���Ԫ������
	void PrintMatrix();      // �������
	int GetElement(int m, int n);   // ��þ����Ӧ��Ԫ��
	void PrintInit();      // ����������ʼ�� 
	void AddElement(int m, int n, int nVal); // ���ӷ���Ԫ��
};

// ����˷�����(*new)�ǵ��ͷ��ڴ档
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
