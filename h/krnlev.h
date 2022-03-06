/*
 * krnlev.h
 *
 *  Created on: Jul 25, 2021
 *      Author: OS1
 */

#ifndef _krnlev_h_
#define _krnlev_h_

class PCB;
class Semaphore;
class Event;
typedef unsigned char IVTNo;

class KernelEv {
public:
	KernelEv(IVTNo, Event*);
	~KernelEv();

	void wait();
	void signal();

	PCB* myPCB;
	Event* myEvent;
	IVTNo ivtNo;
	int val;
	int isBlocked;
};


#endif /* _krnlev_h_ */
