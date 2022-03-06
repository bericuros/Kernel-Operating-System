/*
 * thread.cpp
 *
 *  Created on: May 21, 2021
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "macros.h"
#include "timer.h"
#include <string.h>
#include <dos.h>
#include <iostream.h>

void Thread::start() {
	this->myPCB->start();
}

void Thread::waitToComplete() {
	this->myPCB->waitToComplete();
}

Thread::~Thread() {
	lock;
	this->waitToComplete();
	if (this->myPCB != 0) {
		delete this->myPCB;
	}
	unlock;
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread * Thread::getThreadById(ID id) {
	Thread* thread = 0;
	lock;
	for (List::Node* p = PCB::pcbList.head; p != 0; p = p->next) {
		if (((PCB*)p->data)->id == id) {
			thread = ((PCB*)p->data)->myThread;
			break;
		}
	}
	unlock;
	return thread;
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock;
	myPCB = new PCB(stackSize, timeSlice, PCB::wrapper, this);
	unlock;
}

static int tss;
static int tbp;
static int parentStackOffset;
static int childStackOffset;
static int oldDistanceOffset;
static int newDistanceOffset;
static int parentOffset;

void interrupt copyContext() {
	PCB::running->child->lck = PCB::running->lck;
	memcpy(PCB::running->child->stack, PCB::running->stack, PCB::running->stackSize);

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov tss, ss
		mov tbp, bp
	}
#endif

	parentStackOffset = FP_OFF(PCB::running->stack);
	oldDistanceOffset = tbp - parentStackOffset;
	childStackOffset = FP_OFF(PCB::running->child->stack);
	PCB::running->child->ss = FP_SEG(PCB::running->child->stack);
	PCB::running->child->sp = childStackOffset + oldDistanceOffset;
	PCB::running->child->bp = PCB::running->child->sp;

	parentOffset = *((unsigned*)MK_FP(tss, tbp));
	while (parentOffset != 0 && parentOffset != parentStackOffset) {
		newDistanceOffset = parentOffset - parentStackOffset;
		PCB::running->child->stack[oldDistanceOffset] = childStackOffset + newDistanceOffset;
		oldDistanceOffset = newDistanceOffset;
		tbp = parentOffset;
		parentOffset = *((unsigned*)MK_FP(tss, tbp));
	}
}

static Thread* childThread;

ID Thread::fork() {
	lock;
	childThread = PCB::running->myThread->clone();
	if (childThread == 0 || childThread->myPCB == 0 || childThread->myPCB->stack == 0) {
		unlock;
		return -1;
	}

	PCB::running->child = childThread->myPCB;
	PCB::running->child->parent = (PCB*)PCB::running;

	PCB::running->forkReturnValue = PCB::running->child->id;
	PCB::running->child->forkReturnValue = 0;

	PCB::running->childNum++;
	PCB::running->childList.push((void*)(PCB::running->child));

	copyContext();

	PCB::running->child->myThread->start();

	unlock;

	return PCB::running->forkReturnValue;

}

void Thread::exit() {
	PCB::exit();
}

void Thread::waitForForkChildren() {
	lock;
	for (List::Node* p = PCB::running->childList.head; p != 0; p = p->next) {
		((PCB*)(p->data))->waitToComplete();
	}
	unlock;
}

Thread* Thread::clone() const {
	return 0;
}





















