/*
 * idlethr.h
 *
 *  Created on: Jul 15, 2021
 *      Author: OS1
 */

#ifndef _idlethr_h_
#define _idlethr_h_

#include "thread.h"

class IdleThr : public Thread {
public:

	IdleThr();
	~IdleThr();
	virtual void run();

	int loopFlag;

};

#endif /* _idlethr_h_ */
