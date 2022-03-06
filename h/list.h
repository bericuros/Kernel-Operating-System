/*
 * list.h
 *
 *  Created on: Jul 17, 2021
 *      Author: OS1
 */

#ifndef _list_h_
#define _list_h_


class List {
public:

	class Node {
	public:
		friend class List;

		void* data;
		Node* prev;
		Node* next;

		Node();
		Node(void*);
	};
	friend class Node;

	////////////////

	List();
	~List();

	void insert(void*);  			// insert before head
	void insert(void*, Node*);		// insert before the given node, if node == 0 then insert
	void push(void*);				// push after tail
	void push(void*, Node*);		// push after the given node, if node == 0 then push
	void* delet();					// delete before head
	void* delet(Node*);				// delete before the given node, if node == 0 or node->prev == 0 then delet
	void* pop();					// pop after tail
	void* pop(Node*);				// pop after the given node, if node == 0 or node->next == 0 then pop
	void* take(Node*);				// take the given node
	void clear();					// delete whole list

	////////////////

	Node* head;
	Node* tail;
	int size;

};




#endif /* _list_h_ */
