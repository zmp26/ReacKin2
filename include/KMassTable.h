#ifndef MASSTABLE_H
#define MASSTABLE_H

#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string.h>

struct MassTableEntry{
	int N, Z, A;
	char Symbol[3];
	float MassExcess;
};

class KMassTable{
private:
	const char* mFileName;
	MassTableEntry mEntries[4000];
	int mNumberOfEntries;
	const float AMU=931.494013;
	bool mInitStatus = false;

public:
	void Init(const char* file);
	int Lookup(int n, int z, int a);
	int Lookup(char* symbol, int a);
	float GetMassExcess(int n, int z, int a);
	float GetMassExcess(const char* symbol, int a);
	const char* GetSymbol(int z);
	int GetZ(char* symbol);
	int GetN(char* symbol, int a);
	void Print(void);
	void PrintEntry(int);
	//added by zmp
	bool GetInitStatus();
	float GetMassMeV(const char* symbol, int a);
	float GetMassMeV(int n, int z, int a);
};

#endif