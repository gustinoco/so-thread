#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <time.h>

#define QUANTITY_MAX 10000000
#define SIZE_LIST 500000
#define THREADS 8
#define PRINT_RESULT_ORDENATION true
using namespace std;

void selSort(double a[], int n);
void ThreadProc(LPVOID lpdwThreadParam);
double vectorSingle[SIZE_LIST], vectorMult[SIZE_LIST];
double list[THREADS][SIZE_LIST];

struct LISTA {
	int tamanho, posi,posf,thread;
}LISTA;


void main() {
	const int size = SIZE_LIST;
	
	cout << "Inicio preenchimento vetor...";
	clock_t tStartPreenchimento = clock();
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		vectorSingle[i] = rand() % QUANTITY_MAX;
		vectorMult[i] = vectorSingle[i];
	}

	cout << endl << "Tempo preenchimento Vetor: " << (double)(clock() - tStartPreenchimento) / CLOCKS_PER_SEC << " segundos";

	if (PRINT_RESULT_ORDENATION) {
		cout << endl << "Vetor SingleThread e multi Desordenado = [10]";
		for (int i = 0; i < 10; i++) {
			cout << vectorMult[i] << " ";
		}
	}

	cout << endl << endl << "Inicio processamento Single Thread...";


	//Organização vetor, SellSort.
	clock_t tStartSingle = clock();
	selSort(vectorSingle, size);
	cout << endl << "Tempo processamento Single Thread: " << (double)(clock() - tStartSingle) / CLOCKS_PER_SEC << " segundos";
	//Fim organização Single Thread


	clock_t tStartMult = clock();
	//Quantidade thread e inicio do processamento.	

	DWORD *ThreadIds = new DWORD[THREADS];
	HANDLE *ThreadHandles = new HANDLE[THREADS];
	struct LISTA *data = new struct LISTA[THREADS];
	cout << endl << endl;


	int inicioDivisao = 0, fimDivisao = 0;
	int tamanho = SIZE_LIST / THREADS;
	for (int i = 0; i < THREADS; i++) {
		data[i].posi = inicioDivisao;
		data[i].thread = i;
		data[i].posf = inicioDivisao + tamanho - 1;
		inicioDivisao = inicioDivisao + tamanho;
		data[i].tamanho = tamanho;		
		ThreadHandles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, &data[i], 0, &ThreadIds[i]);
	}	

	WaitForMultipleObjects(THREADS, ThreadHandles, TRUE, INFINITE);

	cout << endl << "Tempo processamento MultThread: " << (double)(clock() - tStartMult) / CLOCKS_PER_SEC << " segundos";

	cout << endl << endl;

	if (PRINT_RESULT_ORDENATION) {
		cout << endl << "Vetor SingleThread Ordenado = ";
		for (int i = 0; i < 10; i++) {
			cout << vectorSingle[i] << " ";
		}
	}

	cout << endl << endl;
	
	if (PRINT_RESULT_ORDENATION) {
		cout << endl << "Vetor MultiThread Ordenado = ";
		for (int i = 0; i < THREADS; i++) {
			for(int j =i;j<10;j++)
				cout << " " <<list[i][j] << " ";
		}	
		cout << endl << endl;		
	
	}
	for (int i = 0; i<THREADS; i++)
		CloseHandle(ThreadHandles[i]);
	cout << endl;
	system("pause");
}

void ThreadProc(LPVOID lpdwThreadParam) {
	struct LISTA *input = (struct LISTA *)lpdwThreadParam;
	for (int i = input->posi; i < input->posf; i++) {
		list[input->thread][i] = vectorMult[i];		
	}	
	selSort(list[input->thread], input->tamanho);
}

void selSort(double a[], int n) {
	for (int j = 0; j < n; j++) {
		double smalle = a[j];
		double current = a[j];
		int index = j;
		for (int i = j; i < n; i++) {
			if (a[i] < smalle) {
				smalle = a[i];
				index = i;
			}
		}
		a[j] = smalle;
		a[index] = current;
	}
}