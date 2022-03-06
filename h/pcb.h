/*
 * pcb.h
 *
 *  Created on: May 21, 2021
 *      Author: OS1
 */

#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "list.h"
#include "semaphor.h"

#define MIN_STACK_SIZE 2048
#define MAX_STACK_SIZE 65535

class PCB {

public:

	enum State {INITIALIZING, TERMINATING, SUSPENDED, READY, RUNNING};

	///////////////////////////////////

	PCB();
	PCB(StackSize stackSize, Time timeSlice, void (* body)(), Thread*);
	~PCB();
	void start();
	void waitToComplete();

	static void wrapper();

	static void exit();

	///////////////////////////////////

	friend void interrupt timer(...);

	///////////////////////////////////

	unsigned *stack;
	volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;

	List waitingList;

	///////////////////////////////////

	static List pcbList;
	static PCB* idle;
	static PCB initial;
	static volatile PCB* running;

	///////////////////////////////////

	static ID stId;
	ID id;
	volatile State state;
	Time timeSlice;
	StackSize stackSize;
	Thread* myThread;
	int limited;
	volatile int lck;

	///////////////////////////////////

	Time blockedTime;
	int returnValueWait;

	///////////////////////////////////

	PCB* parent;
	PCB* child;
	int childNum;
	List childList;
	int forkReturnValue;

};



#endif /* _pcb_h_ */
