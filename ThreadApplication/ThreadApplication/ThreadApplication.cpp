#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <time.h>

#define QUANTITY_MAX 10000
#define SIZE_LIST 10000
#define PRINT_RESULT_ORDENATION true
using namespace std;

void selSort(int a[], int n);


struct LISTA {
	int tamanho;
	int lista[SIZE_LIST];
}LISTA;
int position;
int *num_of_thr = new int;
int thread_index = 0;
void ThreadProc(LPVOID lpdwThreadParam);

void main() {
	const int size = SIZE_LIST;
	int vectorSingle[size], vectorMult[size];
	cout << "Inicio preenchimento vetor...";
	clock_t tStartPreenchimento = clock();
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		vectorSingle[i] = rand() % QUANTITY_MAX;
		vectorMult[i] = vectorSingle[i];
	}

	cout << endl << "Tempo preenchimento Vetor: " << (double)(clock() - tStartPreenchimento) / CLOCKS_PER_SEC << " segundos";

	if (PRINT_RESULT_ORDENATION) {
		cout << endl << "Vetor SingleThread e multi Desordenado = ";
		for (int i = 0; i < 10; i++) {
			cout << vectorSingle[i] << " ";
		}
	}

	cout << endl << endl <<"Inicio processamento Single Thread...";


	//Organização vetor, SellSort.
	clock_t tStartSingle = clock();
	selSort(vectorSingle, size);
	cout << endl << "Tempo processamento Single Thread: " << (double)(clock() - tStartSingle) / CLOCKS_PER_SEC << " segundos";
	//Fim organização Single Thread
	
	clock_t tStartMult = clock();
	//Quantidade thread e inicio do processamento.
	int *numThreads = new int;
	*numThreads = 6;
	int n = *numThreads;

	DWORD *ThreadIds = new DWORD[n];
	HANDLE *ThreadHandles = new HANDLE[n];
	struct LISTA *data = new struct LISTA[n];
	cout << endl<< endl;
	
	
	int inicioDivisao = 0, fimDivisao = 0;	
	const int tamanho = 250;	
	for (int q = 0; q < *numThreads; q++) {
		int listaNova[tamanho];		
		fimDivisao = fimDivisao + tamanho;
		for (inicioDivisao; inicioDivisao < fimDivisao; inicioDivisao++) {
			data[q].lista[inicioDivisao] = vectorMult[inicioDivisao];			
		}		
		data[q].tamanho = q;

		inicioDivisao = fimDivisao; 
		
		
		ThreadHandles[q] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, &data[q], 0, &ThreadIds[q]);		
	}


	*num_of_thr = n;

	WaitForMultipleObjects(*num_of_thr, ThreadHandles, TRUE, INFINITE);
	
	cout << endl << "Tempo processamento MultThread: " << (double)(clock() - tStartMult) / CLOCKS_PER_SEC << " segundos";

	if (PRINT_RESULT_ORDENATION) {
		cout << endl<<"Vetor SingleThread Ordenado = ";
		for (int i = 0; i < 10; i++) {
			cout << vectorSingle[i] << " ";
		}
	}

	if (PRINT_RESULT_ORDENATION) {
		cout << endl << "Vetor MultiThread Ordenado = ";
		for (int i = 0; i < 10; i++) {
			cout << vectorMult[i] << " ";
		}
	}

	for (int i = 0; i<n; i++) 
		CloseHandle(ThreadHandles[i]);
	cout << endl;
	system("pause");
}

void ThreadProc(LPVOID lpdwThreadParam) {
	struct LISTA *input = (struct LISTA *)lpdwThreadParam;		
	cout << endl;
	for (int i = 0; i < 10; i++) {
		cout << input->lista[i] << " ";
	}
	selSort(&input->lista[input->tamanho], input->tamanho);
	cout << endl;
	cout << "Após ordenação";
	for (int i = 0; i < 10; i++) {
		cout << input->lista[i] << " ";
	}

}

void selSort(int a[], int n) {
	for (int j = 0; j < n; j++) {
		int smalle = a[j];
		int current = a[j];
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