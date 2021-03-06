#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <algorithm> 

#define QUANTITY_MAX 10000000
#define SIZE_LIST 50000
#define THREADS 16
using namespace std;

vector<double> shelSort(vector<double> a);
void ThreadProc(LPVOID lpdwThreadParam);
vector<double> vectorMult, vectorSingle;

struct LISTA {
	int posi,posf;
	vector<double> lista;
}LISTA;

void main() {
	const int size = SIZE_LIST;	
	cout << "Inicio preenchimento vetor...";
	clock_t tStartPreenchimento = clock();
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		vectorSingle.push_back(rand() % QUANTITY_MAX);
//		vectorMult.push_back(vectorSingle[i]);
	}
	vectorMult = vectorSingle;

	cout << endl << "Tempo preenchimento Vetor: " << (double)(clock() - tStartPreenchimento) / CLOCKS_PER_SEC << " segundos";

	cout << endl << "Vetor SingleThread e multi Desordenado = [10]";
	for (int i = 0; i < 10; i++) {
		cout << vectorMult[i] << " ";
	}

	cout << endl << endl << "Inicio processamento Single Thread...";

	clock_t tStartSingle = clock();
	vectorSingle = shelSort(vectorSingle);
	cout << endl << "Tempo processamento Single Thread: " << (double)(clock() - tStartSingle) / CLOCKS_PER_SEC << " segundos";

	clock_t tStartMult = clock();

	DWORD *ThreadIds = new DWORD[THREADS];
	HANDLE *ThreadHandles = new HANDLE[THREADS];
	struct LISTA *data = new struct LISTA[THREADS];
	
	cout << endl << endl;
	
	int inicioDivisao = 0, fimDivisao = 0;
	int tamanho = SIZE_LIST / THREADS;
	for (int i = 0; i < THREADS; i++) {
		data[i].posi = inicioDivisao;
		data[i].posf = inicioDivisao + tamanho - 1;
		inicioDivisao = inicioDivisao + tamanho;
		ThreadHandles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, &data[i], 0, &ThreadIds[i]);
	}

	WaitForMultipleObjects(THREADS, ThreadHandles, TRUE, INFINITE);

	cout << endl << "Tempo processamento MultThread: " << (double)(clock() - tStartMult) / CLOCKS_PER_SEC << " segundos" << endl << endl;	

	cout << endl << "Vetor SingleThread Ordenado[20] = ";
	for (int i = 0; i < 20; i++) {
		cout << vectorSingle[i] << " ";
	}
		
	cout << endl << endl << endl << "Vetor MultiThread";
	for (int i = 0; i < THREADS; i++) {
		cout << endl<<"Thread: " << i << " Ordenadao :" ;
		for(int j=0;j<10;j++)
			cout << "  " << data[i].lista[j];
	}
		
	cout << endl << endl << "Vetor Multhread ordenado == ";	
	vectorMult.clear();	
	for (int i = 0; i < tamanho-1; i++)	{
		vector<double> tempVector;
		for (int j = 0; j < THREADS; j++) {			
			tempVector.push_back(data[j].lista[i]);
		}
		for (int p = 0; p < tempVector.size(); p++)
			vectorMult.push_back(tempVector[p]);		
	}

	sort(vectorMult.begin(), vectorMult.end());

	cout << endl << endl << "Vetor mult ordenado e junto [50]: " << endl;
	for (int i = 0; i < 50; i++)
		cout<< "  " << vectorMult[i];

	for (int i = 0; i<THREADS; i++)
		CloseHandle(ThreadHandles[i]);
	cout << endl;
	system("pause");
}

void ThreadProc(LPVOID lpdwThreadParam) {
	struct LISTA *input = (struct LISTA *)lpdwThreadParam;
	vector<double> a;
	for (int i = input->posi; i < input->posf; i++) {
		a.push_back(vectorMult[i]);
	}
	a = shelSort(a);
	input->lista = a;	
}

vector<double> shelSort(vector<double> a) {
	int i, j, increment, temp;
	int array_size = a.size();
	increment = 3;	
	while (increment > 0)
	{
		for (i = 0; i < array_size; i++)
		{
			j = i;
			temp = a[i];
			while ((j >= increment) && (a[j - increment] > temp))
			{
				a[j] = a[j - increment];
				j = j - increment;
			}
			a[j] = temp;
		}
		if (increment / 2 != 0)
			increment = increment / 2;
		else if (increment == 1)
			increment = 0;
		else
			increment = 1;
	}	
	return a;
}