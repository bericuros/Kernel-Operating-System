/*
 * event.cpp
 *
 *  Created on: Jul 25, 2021
 *      Author: OS1
 */

#include "event.h"
#include "timer.h"
#include "krnlev.h"

Event::Event(IVTNo ivtNo) {
	lock;
	myImpl = new KernelEv(ivtNo, this);
	unlock;
}

Event::~Event() {
	lock;
	if (this->myImpl != 0) {
		delete this->myImpl;
	}
	unlock;
	this->myImpl = 0;
}

void Event::wait() {
	this->myImpl->wait();
}

void Event::signal() {
	this->myImpl->signal();
}


