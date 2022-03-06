/*
 * semaphor.cpp
 *
 *  Created on: Jul 22, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "krnlsem.h"
#include "timer.h"

Semaphore::Semaphore(int init) {
	lock;
	this->myImpl = new KernelSem(init, this);
	unlock;
}
Semaphore::~Semaphore() {
	lock;
	if (this->myImpl != 0) {
		delete this->myImpl;
	}
	unlock;
}

int Semaphore::wait(Time maxTimeToWait) {
	return this->myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	this->myImpl->signal();
}

int Semaphore::val() const {
	return this->myImpl->val;
}
