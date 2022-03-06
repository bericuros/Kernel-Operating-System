/*
 * krnlev.cpp
 *
 *  Created on: Jul 25, 2021
 *      Author: OS1
 */

#include "krnlev.h"
#include "event.h"
#include "pcb.h"
#include "timer.h"
#include "ivtentry.h"
#include "SCHEDULE.H"
#include "semaphor.h"
#include "krnlsem.h"

KernelEv::KernelEv(IVTNo aIvtNo, Event *aMyEvent) : myEvent(aMyEvent), ivtNo(aIvtNo), val(0), isBlocked(0) {
	lock;
	this->myPCB = (PCB*)PCB::running;
	IVTEntry::ivtEntries[this->ivtNo]->initialize(this);
	unlock;
}

KernelEv::~KernelEv() {
	lock;
	IVTEntry::ivtEntries[this->ivtNo]->restore();
	unlock;
}

void KernelEv::wait() {
	lock;

	if (PCB::running == this->myPCB) {
		if (this->val == 0) {
			this->myPCB->state = PCB::SUSPENDED;
			this->isBlocked = 1;
			dispatch();
		}
		else {
			this->val = 0;
		}
	}

	unlock;
}

void KernelEv::signal() {
	lock;

	if (this->isBlocked == 1) {
		this->myPCB->state = PCB::READY;
		this->isBlocked = 0;
		Scheduler::put(this->myPCB);
	}
	else {
		this->val = 1;
	}

	unlock;
}
