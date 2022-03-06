/*
 * list.cpp
 *
 *  Created on: Jul 17, 2021
 *      Author: OS1
 */

/*
 * list.cpp
 *
 *  Created on: Jul 17, 2021
 *      Author: OS1
 */

#include "list.h"
#include "pcb.h"
#include "timer.h"
#include <iostream.h>

List::Node::Node() : data(0), prev(0), next(0) {}
List::Node::Node(void *aData) : data(aData), prev(0), next(0) {}

List::List() : head(0), tail(0), size(0) {}
List::~List() {
	this->clear();
}

void List::insert(void* aData) {
	Node* newNode = new Node(aData);

	newNode->prev = 0;
	newNode->next = this->head;

	if (this->head) this->head->prev = newNode;
	else this->tail = newNode;

	this->head = newNode;

	this->size++;
}

void List::insert(void* aData, Node* aNode) {
	if (aNode) {
		Node* newNode = new Node(aData);

		newNode->prev = aNode->prev;
		newNode->next = aNode;

		if (newNode->prev) newNode->prev->next = newNode;
		else this->head = newNode;

		aNode->prev = newNode;

		this->size++;
	}
	else this->insert(aData);
}

void List::push(void* aData) {
	Node* newNode = new Node(aData);

	newNode->prev = this->tail;
	newNode->next = 0;

	if (this->tail) this->tail->next = newNode;
	else this->head = newNode;

	this->tail = newNode;

	this->size++;
}

void List::push(void* aData, Node* aNode) {
	if (aNode) {
		Node* newNode = new Node(aData);

		newNode->prev = aNode;
		newNode->next = aNode->next;

		if (newNode->next) newNode->next->prev = newNode;
		else this->tail = newNode;

		aNode->next = newNode;

		this->size++;
	}
	else this->push(aData);
}

void* List::delet() {
	void* data = 0;
	if (this->head) {
		Node* oldNode = this->head;
		data = oldNode->data;

		this->head = this->head->next;
		if (this->head) this->head->prev = 0;
		else this->tail = 0;

		delete oldNode;
		this->size--;
	}
	return data;
}

void* List::delet(Node* aNode) {
	if (aNode && aNode->prev) {
		Node* oldNode = aNode->prev;
		void* data = oldNode->data;

		aNode->prev = oldNode->prev;
		if (aNode->prev) aNode->prev->next = aNode;
		else this->head = aNode;

		delete oldNode;
		this->size--;

		return data;
	}
	return this->delet();
}

void* List::pop() {
	void* data = 0;
	if (this->tail) {
		Node* oldNode = this->tail;
		data = oldNode->data;

		this->tail = this->tail->prev;
		if (this->tail) this->tail->next = 0;
		else this->head = 0;

		delete oldNode;
		this->size--;
	}
	return data;
}

void* List::pop(Node* aNode) {
	if (aNode && aNode->next) {
		Node* oldNode = aNode->next;
		void* data = oldNode->data;

		aNode->next = oldNode->next;
		if (aNode->next) aNode->next->prev = aNode;
		else this->tail = aNode;

		delete oldNode;
		this->size--;

		return data;
	}
	return this->pop();
}

void* List::take(Node* aNode) {
	void* data = 0;
	if (aNode) {
		data = aNode->data;

		if (aNode->prev) aNode->prev->next = aNode->next;
		else this->head = aNode->next;

		if (aNode->next) aNode->next->prev = aNode->prev;
		else this->tail = aNode->prev;

		delete aNode;
		this->size--;
	}
	return data;
}

void List::clear() {
	while (this->delet());
}





