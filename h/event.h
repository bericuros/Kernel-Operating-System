/*
 * event.h
 *
 *  Created on: Jul 25, 2021
 *      Author: OS1
 */

// File: event.h
#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};

#include "ivtentry.h"

#define PREPAREENTRY(ivtNo, callOldRoutine)					\
	void interrupt inter##ivtNo(...);						\
	IVTEntry entry##ivtNo(ivtNo, inter##ivtNo);				\
	void interrupt inter##ivtNo(...) {						\
		entry##ivtNo.signal();								\
		if (callOldRoutine) {								\
			entry##ivtNo.oldRoutine();						\
		}													\
		dispatch();											\
	}


#endif
