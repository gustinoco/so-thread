// VidaDeFilosofo.cpp : define o ponto de entrada para o aplicativo do console.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include<random>
#define QUANTITY_MAX 7
#define THREADS 5
using namespace std;
struct HACHI {
	int esquerda, direita, filosofo;
	bool status = false;
};
void VidaDeFilosofo(LPVOID thread);
void dormir(int filosofo);
void acordar(int filosofo);
void comer(HACHI *hachi);
void pegaHachi(HACHI *hachi);
void devolveHachi(HACHI *hachi);
int tempo();
int hachi[THREADS];
HANDLE hMutex;

void main()
{
	//Seta estado hachi = Disponível
	for (int i = 0; i < THREADS; i++)
		hachi[i] = 0;

	DWORD ThreadId;
	HANDLE *ThreadHandles = new HANDLE[THREADS];
	struct HACHI *data = new struct HACHI[THREADS];
	hMutex = CreateMutex(NULL, FALSE, NULL);
	//Passa Struct HACHI com valor do filoso e hachi direita e esquerda que ele deve pegar.
	for (int i = 0; i < THREADS; i++) {
		data[i].filosofo = i;
		if (i == 0) {
			data[i].direita = THREADS - 1;
			data[i].esquerda = i;
		}
		else {
			data[i].direita = i - 1;
			data[i].esquerda = i;
		}
		//Cria as threads com o data
		ThreadHandles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VidaDeFilosofo, &data[i], 0, &ThreadId);
	}

	WaitForMultipleObjects(THREADS, ThreadHandles, TRUE, INFINITE);
	cout << endl;


	system("pause");
}

void VidaDeFilosofo(LPVOID thread) {
	while (true) {
		struct HACHI *input = (struct HACHI *)thread;
		dormir(input->filosofo);
		acordar(input->filosofo);
		pegaHachi(input);
		comer(input);
		devolveHachi(input);
	}
}

void dormir(int filosofo) {
	int tempoDormir = tempo();
	cout << endl << "Filosofo " << filosofo << " vai dormir por " << (double)(tempoDormir * 1000) / CLOCKS_PER_SEC << " segundos" << endl;
	Sleep(tempoDormir * 1000);
}

void acordar(int filosofo) {
	cout << endl << "Filosofo " << filosofo << " acordou" << endl;
}

void comer(HACHI *mHachi) {
	int tempoDormir = tempo();
	cout << endl << "Filosofo " << mHachi->filosofo << " esta comendo na direita:  " << hachi[mHachi->direita] << " na esquerda: " << hachi[mHachi->esquerda] << " por " << (double)(tempoDormir * 1000) / CLOCKS_PER_SEC << " segundos" << endl;
	Sleep(tempoDormir*1000);
}

void pegaHachi(HACHI *mHachi) {
	mHachi->status = true;
	while (true) {
		if (mHachi->status) {
			if (WaitForSingleObject(hMutex, INFINITE) != WAIT_FAILED) {
				if ((hachi[mHachi->direita] == 0) && (hachi[mHachi->esquerda] == 0)) {
					cout << endl << "Filosofo " << mHachi->filosofo << " pegou o hachi da direita: " << mHachi->direita << " = " << hachi[mHachi->direita] << " e o da esquerda: " << mHachi->esquerda << " = " << hachi[mHachi->esquerda] << endl;
					hachi[mHachi->esquerda] = 1;
					hachi[mHachi->direita] = 1;
					mHachi->status = true;
					break;
				}
				else {
					cout << endl << "*******************Filosofo " << mHachi->filosofo << " nao pegou hachi pois direita = " << hachi[mHachi->direita] << " e esquerda = " << hachi[mHachi->esquerda] << endl;
					//ReleaseMutex(hMutex);
					//Sleep(1000);
				}
			}
			else
				mHachi->status = false;
		}
	}
	ReleaseMutex(hMutex);
}

void devolveHachi(HACHI *mHachi) {
	while (true) {
		if (WaitForSingleObject(hMutex, INFINITE) != WAIT_FAILED) {
			hachi[mHachi->esquerda] = 0;
			hachi[mHachi->direita] = 0;
			cout << endl << "Filosofo " << mHachi->filosofo << " devolveu o hachi da direita: " << mHachi->direita << " = " << hachi[mHachi->direita] << " e o da esquerda: " << mHachi->esquerda << " = " << hachi[mHachi->esquerda] << endl;
			break;
		}
	}
	ReleaseMutex(hMutex);
}

int tempo() {
	std::random_device rd;   
	std::mt19937 rng(rd()); 
	std::uniform_int_distribution<int> uni(1, QUANTITY_MAX); 
	auto random_integer = uni(rng);
	return random_integer;
}
