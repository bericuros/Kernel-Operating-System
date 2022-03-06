/*
 * pcb.cpp
 *
 *  Created on: May 21, 2021
 *      Author: OS1
 */

#include "pcb.h"
#include "macros.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>
#include "thread.h"
#include "idlethr.h"
#include "list.h"
#include "timer.h"

ID PCB::stId = 1;		// the idle thread will have 1 as its id, the initial will gave 2
List PCB::pcbList;
PCB* PCB::idle = (new IdleThr())->myPCB;
PCB PCB::initial;
volatile PCB* PCB::running = &PCB::initial;

// only for initial thread
PCB::PCB() : id(stId++), state(RUNNING), stackSize(defaultStackSize), timeSlice(0), limited(defaultTimeSlice != 0 ? 1 : 0),
		myThread(0), stack(0), blockedTime(0), returnValueWait(0), lck(0), childNum(0), parent(0), child(0), forkReturnValue(0) {
	lock;
	PCB::pcbList.push((void*)this);
	unlock;
}

PCB::PCB(StackSize stackSize, Time timeSlice, void (* body)(), Thread *thread) :
	id(stId++), state(INITIALIZING), timeSlice(timeSlice), limited(timeSlice != 0 ? 1 : 0),
	myThread(thread), blockedTime(0), returnValueWait(0), lck(0), childNum(0), parent(0), child(0), forkReturnValue(0) {

	lock;
	stackSize = stackSize <= MAX_STACK_SIZE ? stackSize : MAX_STACK_SIZE;
	stackSize = stackSize >= MIN_STACK_SIZE ? stackSize : MIN_STACK_SIZE;

	this->stackSize = stackSize;
	stackSize /= sizeof(unsigned);
	this->stack = new unsigned[stackSize];

	if (this->stack != 0) {
		this->stack[stackSize - 1] = 0x200;
		this->stack[stackSize - 2] = FP_SEG(body);
		this->stack[stackSize - 3] = FP_OFF(body);

		this->stack[stackSize - 12] = 0; // for fork

		this->sp = FP_OFF(this->stack + stackSize - 12);
		this->ss = FP_SEG(this->stack + stackSize - 12);
		this->bp = FP_OFF(this->stack + stackSize - 12);

		PCB::pcbList.push((void*)this);
	}

	unlock;
}

PCB::~PCB() {
	//lock;
	if (this->stack != 0) {
		delete []this->stack;
	}
	//unlock;
}

void PCB::start() {
	lock;

	if (this->state == INITIALIZING) {
		this->state = READY;
		Scheduler::put(this);
	}

	unlock;
}

void PCB::waitToComplete() {
	lock;
	if (this != PCB::running && this->state != PCB::TERMINATING) {
		PCB::running->state = PCB::SUSPENDED;
		this->waitingList.push((void*)PCB::running);
		dispatch();
	}
	unlock;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	PCB::exit();
}

void PCB::exit() {
	lock;

	PCB::running->state = PCB::TERMINATING;
	for (List::Node* p = PCB::running->waitingList.head; p != 0; p = p->next) {
		PCB* pcbThr = (PCB*)(p->data);
		pcbThr->state = PCB::READY;
		Scheduler::put(pcbThr);
	}
	PCB::running->waitingList.clear();

	unlock;
	dispatch();
}
