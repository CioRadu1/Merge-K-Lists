/*
*  Ciobanu Radu-Rares
*  Grupa 30222
	*In acest cod am testat algoritmul de interclasare a k liste sortate
	cu ajutorul constructiei unui heap ce contine pointeri la primul element din fiecare lista

	 - pentru generarea acestor liste soratate avem o complexitate de O(n), iar numarul comparatiilor este 0 deoarece 
	   nu avem nevoie de aceste pentru ca functia FillRandomArray genereaza automat un vector sortat pe care l-am 
	   pus apoi intr-o lista
	 - pentru interclasarea acestora avem o complexitate de O(n*logk), aceasta luand cel mai mic prim element al fiecarei liste
	   si inserandu-l la sfarsitul listei rezultat.

*/
#include <stdio.h>
#include "Profiler.h"	
using namespace std;

Profiler p("Interclasare");

#define MAX_SIZE 10000
#define MAX_SIZE1 500
#define STEP_SIZE 100
#define STEP_SIZE1 10

typedef struct node {

	int x;
	struct node* next;
	
}NodeT;

NodeT* create(int givenKey)
{
	NodeT* q = ((NodeT*)malloc(sizeof(NodeT)));
	q->x = givenKey;
	q->next = NULL;
	return q;
}

void insert_last(NodeT **first, NodeT **last, int givenKey)
{
	NodeT* q = create(givenKey);
	if (*first == NULL)
	{
		*first = q;
		*last = q;
	}
	else
	{
		(*last)->next = q;
		*last = q;
	}
}

void heapify(NodeT* a[], int index, int k, int n) {

	Operation attr = p.createOperation("Atribuiri", n);
	Operation comp = p.createOperation("Comparatii", n);

	int left = (2 * index) + 1;
	int right = (2 * index) + 2;
	int min = index;
	if (left < k) {
		comp.count();
		if (a[left]->x < a[index]->x) {

			min = left;
		}
	}
	if (right < k){
		comp.count();
		if (a[right]->x < a[min]->x) {
			
			min = right;
		}
	}
	if (min != index)
	{
		attr.count(3);
		swap(a[index], a[min]); 
		heapify(a, min, k, n);
	}

}

void bottom_up(NodeT* a[], int k) {

	for (int i = ((k / 2) - 1); i >= 0; i--) {
		heapify(a, i, k, 0);
	}
}

NodeT* mergeListe(NodeT* a[], int k, int n) {

	Operation attr = p.createOperation("Atribuiri", k);
	Operation comp = p.createOperation("Comparatii", k);

	int l = k;
	int m = n;
	NodeT** heap = ((NodeT**)malloc(n * sizeof(NodeT)));
	for (int i = 0; i < k; i++) {
		attr.count();
		heap[i] = &a[i][0]; 
	}
	bottom_up(heap, l);
	
	NodeT* rezIn = nullptr;
	NodeT* rezSf = nullptr;

	while (l > 0){	
		comp.count();
		if (rezSf != nullptr) {
			attr.count(2);
			rezSf->next = heap[0];
			rezSf = heap[0];
		}
		else {
			attr.count(2);
			rezIn = heap[0];
			rezSf = heap[0];
		}
		attr.count();
		heap[0] = heap[0]->next;
		comp.count();
		if (heap[0] == NULL) {

			l--;
			attr.count();
			heap[0] = heap[l];
		}
		if (l > 0) {
			heapify(heap, 0, l,k);
		}
	}
	return rezIn;
}

NodeT** generare_liste(int k, int n,NodeT **first, NodeT **last) {
	
	Operation attr = p.createOperation("Atribuiri_generare", n);
	Operation comp = p.createOperation("Comparatii_generare", n);

	int nrNumere = n;
	int nrListe = k;
	int suma = 0;
	int** a = ((int**)malloc(k * sizeof(int*)));
	for (int i = 0; i < k; i++) {
		if (suma != n) {
			int lung = 0;
		   	if ((nrNumere % nrListe) == 0) {
				lung = (nrNumere / nrListe);
				nrNumere -= lung; 
				suma += lung;
				nrListe--;
			 }
			else if ((nrNumere % nrListe) != 0) {
				lung = (nrNumere / nrListe) + (nrNumere % nrListe);
				nrNumere -= lung; 
				suma += lung;
				nrListe--;
			 }
			a[i] = ((int*)malloc(lung * sizeof(int)));
			attr.count(lung);
			FillRandomArray(a[i], lung, 10, 10000, false, ASCENDING);
			for (int j = 0; j < lung; j++) {

				insert_last(&first[i], &last[i], a[i][j]);
			}
		}

	}
	return first;
}

void afisareLista(NodeT *first)
{
	NodeT* q = first;
	while (q != NULL)
	{
		printf("%d ", q->x);
		q = q->next;
	}
	printf("\n");
}

void afisareListe(NodeT **first, int k , int n)
{
	for (int i = 0; i < k; i++) {
		printf("%d :", i + 1);
		afisareLista(first[i]);
	}
}

void demo() {
	
	int k = 4;
	int l = k;
	int n = 20;
	int m = n;
	NodeT** first = ((NodeT**)malloc(k * sizeof(NodeT*)));
	for (int i = 0; i < k; i++) {
		first[i] = nullptr;
	}
	NodeT** last = ((NodeT**)malloc(k * sizeof(NodeT*)));
	for (int i = 0; i < k; i++) {
		last[i] = nullptr; 
	}
	NodeT* last1 = NULL;
	NodeT** liste = generare_liste(k, n, first, last);
	afisareListe(liste, k, n);
	printf("\n");
	NodeT* rezultat = mergeListe(liste, l, m);
	printf("Lista rezultata: ");
	afisareLista(rezultat);

}

void grafice() {
	int k1 = 5;
	int k2 = 10;
	int k3 = 100;
	int n;

	NodeT** first = ((NodeT**)malloc(k3 * sizeof(NodeT*)));
	for (int i = 0; i < k3; i++) {
		first[i] = nullptr;
	}
	NodeT** last = ((NodeT**)malloc(k3 * sizeof(NodeT*)));
	for (int i = 0; i < k3; i++) {
		last[i] = nullptr;
	}

	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE) {
		NodeT **liste1 = generare_liste(k1, n, first, last);
		 p.addSeries("k1_Liste", "Atribuiri_generare", "Comparatii_generare");
	}

	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE) {
		NodeT** liste2 = generare_liste(k2, n, first, last);
		p.addSeries("k2_Liste", "Atribuiri_generare", "Comparatii_generare");
	}
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE) {
		NodeT** liste3 = generare_liste(k3, n, first, last);
		p.addSeries("k3_Liste", "Atribuiri_generare", "Comparatii_generare");
	}
	p.createGroup("Total", "k1_Liste", "k2_Liste", "k3_Liste");
	p.createGroup("Atribuiri_Comparatii_Caz1", "Atribuiri_generare", "Comparatii_generare");
}

void grafic_n() {
	int n = 10000;
	
	for (int k = STEP_SIZE1; k < MAX_SIZE1; k += STEP_SIZE1) {
		NodeT** first1 = ((NodeT**)malloc(k * sizeof(NodeT*))); 
		for (int i = 0; i < k; i++) { 
			first1[i] = nullptr;
		}
		NodeT** last1 = ((NodeT**)malloc(k * sizeof(NodeT*))); 
		for (int i = 0; i < k; i++) { 
			last1[i] = nullptr;  
		}
		NodeT** liste4 = generare_liste(k, n, first1, last1);
		mergeListe(liste4, k, n); 
	}
	p.addSeries("TotalCaz2", "Atribuiri", "Comparatii");
	p.createGroup("Atribuiri_Comparatii_n_fix","Atribuiri", "Comparatii");
}

void grafice_all() {
	grafice();
	grafic_n();
	p.showReport();
}

int main() {

	demo();
	//grafice_all();

	return 0;
}