#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"
#include <assert.h>

// Constructors-Destructors
ListPtr createList(void (*dataPrinter)(void *data)) {
	ListPtr L = (ListPtr)malloc(sizeof(ListObj));
	if (L) {
		L->head = NULL;
		L->dataPrinter = dataPrinter;
		L->length = 0;
	}
	return L;
}

void destroyList(ListPtr *pL) {
	if (pL && *pL) {
		NodePtr current = (*pL)->head;
		while (current != NULL) {
			NodePtr next = current->next;
			free(current);
			current = next;
		}
		free(*pL);
		*pL = NULL;
	}
}

// Access functions
int lengthList(ListPtr L) {
	if (L == NULL) return -1;
	return L->length;
}

void printList(ListPtr L) {
	if (L == NULL || L->dataPrinter == NULL) return;
	
	NodePtr current = L->head;
	while (current != NULL) {
		L->dataPrinter(current->data);
		current = current->next;
	}
}

void *getList(ListPtr L, int i) {
	if (L == NULL || i < 0 || i >= L->length) return NULL;
	
	NodePtr current = L->head;
	for (int j = 0; j < i; j++) {
		current = current->next;
	}
	return current->data;
}

// Manipulation functions
bool appendList(ListPtr L, void *data) {
	if (L == NULL) return false;
	
	NodePtr newNode = (NodePtr)malloc(sizeof(NodeObj));
	if (newNode == NULL) return false;
	
	newNode->data = data;
	newNode->next = NULL;
	
	if (L->head == NULL) {
		L->head = newNode;
	} else {
		NodePtr current = L->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
	}
	
	L->length++;
	return true;
}

void *deleteList(ListPtr L, int i) {
	if (L == NULL || i < 0 || i >= L->length) return NULL;
	
	void *data;
	NodePtr current = L->head;
	NodePtr prev = NULL;
	
	if (i == 0) {
		data = current->data;
		L->head = current->next;
		free(current);
	} else {
		for (int j = 0; j < i; j++) {
			prev = current;
			current = current->next;
		}
		data = current->data;
		prev->next = current->next;
		free(current);
	}
	
	L->length--;
	return data;
}
