/*
 * krnlsem.cpp
 *
 *  Created on: Jul 22, 2021
 *      Author: OS1
 */

#include "krnlsem.h"
#include "semaphor.h"
#include "timer.h"
#include "list.h"
#include "pcb.h"
#include "macros.h"
#include "SCHEDULE.H"
#include <iostream.h>

List KernelSem::semList;

KernelSem::KernelSem(int init, Semaphore* aMySem) : val(init >= 0 ? init : 1), mySem(aMySem) {
	lock;
	semList.push((void*)this);
	unlock;
}

KernelSem::~KernelSem() {

	// because of the limitedList
	clearInt;
	//lock;

	List::Node* p = KernelSem::semList.head;
	for (; p != 0 && ((KernelSem*)(p->data)) != this; p = p->next);
	KernelSem::semList.take(p);

	//unlock;
	setInt;

	PCB* unblockedPCB = 0;
	while (this->limitedList.size > 0) {
		unblockedPCB = (PCB*)(this->limitedList.pop());
		unblockedPCB->state = PCB::READY;
		unblockedPCB->returnValueWait = 0;
		Scheduler::put(unblockedPCB);
	}

	while (this->unlimitedList.size > 0) {
		unblockedPCB = (PCB*)(this->unlimitedList.pop());
		unblockedPCB->state = PCB::READY;
		unblockedPCB->returnValueWait = 0;
		Scheduler::put(unblockedPCB);
	}

}

int KernelSem::wait(Time maxTimeToWait) {
	lock;

	if (--this->val < 0) {
		return this->block(maxTimeToWait);
	}

	unlock;

	return 1;
}

void KernelSem::signal() {
	lock;

	if (++this->val <= 0) {
		this->unblock();
	}

	unlock;
}

// block must have only the unlock !!!
int KernelSem::block(Time maxTimeToWait) {
	PCB::running->state = PCB::SUSPENDED;

	if (maxTimeToWait > 0) {

		// because of the limitedList
		clearInt;

		List::Node*p = this->limitedList.head;
		for (; p != 0 && maxTimeToWait >= ((PCB*)(p->data))->blockedTime; p = p->next) {
			maxTimeToWait -= ((PCB*)(p->data))->blockedTime;
		}

		PCB::running->blockedTime = maxTimeToWait;

		if (p != 0) {
			((PCB*)(p->data))->blockedTime -= PCB::running->blockedTime;
			this->limitedList.insert((void*)PCB::running, p);
		}
		else this->limitedList.push((void*)PCB::running);

		setInt;

	}
	else this->unlimitedList.push((void*)PCB::running);

	dispatch();

	// the lock is in the wait method !!!
	unlock;

	return PCB::running->returnValueWait;
}

void KernelSem::unblock() {
	PCB* unblockedPCB = 0;

	if (this->unlimitedList.size > 0) unblockedPCB = (PCB*)(this->unlimitedList.delet());
	else {
		// because of the limitedList
		clearInt;
		unblockedPCB = (PCB*)(this->limitedList.pop());
		setInt;
	}

	if (unblockedPCB != 0) {
		unblockedPCB->state = PCB::READY;
		unblockedPCB->returnValueWait = 1;
		Scheduler::put(unblockedPCB);
	}
}

void KernelSem::update() {
	int add = 0;
	if (this->limitedList.size > 0) {
		((PCB*)(this->limitedList.head->data))->blockedTime--;
		while (this->limitedList.size > 0 && ((PCB*)(this->limitedList.head->data))->blockedTime == 0) {
			PCB* unblockedPCB = (PCB*)(this->limitedList.delet());
			unblockedPCB->state = PCB::READY;
			unblockedPCB->returnValueWait = 0;
			Scheduler::put(unblockedPCB);
			add++;
		}
	}
	this->val += add;
}
