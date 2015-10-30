#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "quick3.h"
#include "profile.h"
using namespace std;

int cmp(int* a, int* b)
{
	return *a - *b;
}

int main()
{
	string junk;
	srand(time(NULL));
	double time;
	int* p = new int[10000000];
	
	vector<int> A1, A2, A3, B1, B2, B3, C1, C2, C3, D1, D2, D3, E1, E2, E3;
	for (int i = 0; i < 10000000; i++)
	{
		int num = rand()%1000000;
		A1.emplace_back(num);
		A2.emplace_back(num);
		A3.emplace_back(num);
		p[i] = num;
		B1.emplace_back(i);
		B2.emplace_back(i);
		B3.emplace_back(i);
		C1.emplace_back(100000-i);
		C2.emplace_back(100000-i);
		C3.emplace_back(100000-i);
		D1.emplace_back(num%100);
		D2.emplace_back(num%100);
		D3.emplace_back(num%100);
		E1.emplace_back(1);
		E2.emplace_back(1);
		E3.emplace_back(1);
	}
	cout << "Random:\n";
	Timer a1;
	a1.Start();
	quick(A1.begin(), A1.end());
	time = a1.End();
	cout << "Quicksort time: " << time << endl;
	Timer a2;
	a2.Start();
	quick3(A2.begin(), A2.end());
	time = a2.End();
	cout << "Quick3 time:    " << time << endl;
	for (int i = 1; i < A2.size(); i++) if (A2[i-1] > A2[i]) cout << "not sorted!!\n";
	Timer a3;
	a3.Start();
	sort(A3.begin(), A3.end());
	time = a3.End();
	cout << "std::sort time: " << time << endl;
	
	Timer aa;
	aa.Start();
	qsort(p, 1000000, sizeof(int), (int (*)(const void*, const void*))cmp);
	time = aa.End();
	cout << "qsort time:     " << time << endl;
	
	cout << "\nSorted:\n";
	Timer b1;
	b1.Start();
	quick(B1.begin(), B1.end());
	time = b1.End();
	cout << "Quicksort time: " << time << endl;
	Timer b2;
	b2.Start();
	quick3(B2.begin(), B2.end());
	time = b2.End();
	cout << "Quick3 time:    " << time << endl;
	Timer b3;
	b3.Start();
	sort(B3.begin(), B3.end());
	time = b3.End();
	cout << "std::sort time: " << time << endl;
	
	cout << "\nReverse-Sorted:\n";
	Timer c1;
	c1.Start();
	quick(C1.begin(), C1.end());
	time = c1.End();
	cout << "Quicksort time: " << time << endl;
	Timer c2;
	c2.Start();
	quick3(C2.begin(), C2.end());
	time = c2.End();
	cout << "Quick3 time:    " << time << endl;
	Timer c3;
	c3.Start();
	sort(C3.begin(), C3.end());
	time = c3.End();
	cout << "std::sort time: " << time << endl;
	
	cout << "\nFew Unique Keys:\n";
	Timer d1;
	d1.Start();
	quick(D1.begin(), D1.end());
	time = d1.End();
	cout << "Quicksort time: " << time << endl;
	Timer d2;
	d2.Start();
	quick3(D2.begin(), D2.end());
	time = d2.End();
	cout << "Quick3 time:    " << time << endl;
	Timer d3;
	d3.Start();
	sort(D3.begin(), D3.end());
	time = d3.End();
	cout << "std::sort time: " << time << endl;
	
	cout << "\nOne Key:\n";
	Timer e1;
	e1.Start();
	quick(E1.begin(), E1.end());
	time = e1.End();
	cout << "Quicksort time: " << time << endl;
	Timer e2;
	e2.Start();
	quick3(E2.begin(), E2.end());
	time = e2.End();
	cout << "Quick3 time:    " << time << endl;
	Timer e3;
	e3.Start();
	sort(E3.begin(), E3.end());
	time = e3.End();
	cout << "std::sort time: " << time << endl;
	
	delete p;
	
	getline(cin, junk);
}