/*
 * ivtentry.cpp
 *
 *  Created on: Jul 26, 2021
 *      Author: OS1
 */

#include <dos.h>
#include "ivtentry.h"
#include "event.h"
#include "krnlev.h"
#include "timer.h"
#include "macros.h"

IVTEntry* IVTEntry::ivtEntries[ENTRY_SIZE] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine) {
	lock;
	this->ivtNo = ivtNo;
	this->oldRoutine = 0;
	this->newRoutine = newRoutine;
	this->myKernelEvent = 0;
	IVTEntry::ivtEntries[this->ivtNo] = this;
	unlock;
}

IVTEntry::~IVTEntry() {
	lock;
	IVTEntry::ivtEntries[this->ivtNo] = 0;
	unlock;
}

void IVTEntry::signal() {
	if (myKernelEvent != 0) {
		this->myKernelEvent->signal();
	}
}

void IVTEntry::initialize(KernelEv* kernelEvent) {
	clearInt;
	this->oldRoutine = getvect(this->ivtNo);
	setvect(this->ivtNo, this->newRoutine);
	this->myKernelEvent = kernelEvent;
	setInt;
}

void IVTEntry::restore() {
	clearInt;
	setvect(this->ivtNo, this->oldRoutine);
	IVTEntry::ivtEntries[this->ivtNo] = 0;
	this->myKernelEvent = 0;
	setInt;
}
