/*
 * idlethr.cpp
 *
 *  Created on: Jul 15, 2021
 *      Author: OS1
 */

#include "idlethr.h"

IdleThr::IdleThr() : Thread(defaultStackSize, 1), loopFlag(1) {}

IdleThr::~IdleThr() {
	this->loopFlag = 0;
	this->waitToComplete();
}

void IdleThr::run() {
	while (this->loopFlag == 1) {}
}


