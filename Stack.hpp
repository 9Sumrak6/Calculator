#pragma once
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class Stack{
	struct Node{
		double value;
		Node *next;
	};

	Node *top;
	int size;
public: 
	Stack();
	void Push(double value);
	void Pop();
	double Top();
	bool IsEmpty();
	int GetSize();
};

Stack::Stack() {
	top = NULL;
	size = 0;
}

void Stack::Push(double value) {
	Node *node = new Node;
	node->value = value;
	node->next = top;
	top = node;
	size++;
}

void Stack::Pop() {
	Node *tmp = top;
	top = top->next;
	delete tmp;
	size--;
}

double Stack::Top() {
	return top->value;
}

bool Stack::IsEmpty() {
	return top == NULL;
}

int Stack::GetSize() {
	return size;
}