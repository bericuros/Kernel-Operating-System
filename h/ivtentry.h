/*
 * ivtentry.h
 *
 *  Created on: Jul 26, 2021
 *      Author: OS1
 */

#ifndef _ivtentry_h_
#define _ivtentry_h_

#define ENTRY_SIZE 256

#include "timer.h"
#include "macros.h"

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);
class KernelEv;

class IVTEntry {
public:
	IVTEntry(IVTNo, pInterrupt);
	~IVTEntry();

	void signal();
	void initialize(KernelEv*);
	void restore();

	static IVTEntry* ivtEntries[ENTRY_SIZE];

	IVTNo ivtNo;
	pInterrupt oldRoutine;
	pInterrupt newRoutine;
	KernelEv* myKernelEvent;

};


#endif /* _ivtentry_h_ */
