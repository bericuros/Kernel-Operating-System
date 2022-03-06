/*
 * krnlsem.h
 *
 *  Created on: Jul 22, 2021
 *      Author: OS1
 */

#ifndef _krnlsem_h_
#define _krnlsem_h_

#include "semaphor.h"
#include "list.h"

class KernelSem {
public:
	KernelSem(int, Semaphore*);
	~KernelSem();

	int wait(Time);
	void signal();

	int block(Time);
	void unblock();

	void update();

	volatile int val;
	Semaphore* mySem;

	volatile List limitedList;
	List unlimitedList;

	static List semList;

};



#endif /* _krnlsem_h_ */
