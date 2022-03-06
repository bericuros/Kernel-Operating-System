/*
 * userthr.h
 *
 *  Created on: Aug 15, 2021
 *      Author: OS1
 */

#ifndef _userthr_h_
#define _userthr_h_

#include "thread.h"

class UserThr : public Thread {
public:

	UserThr(int, char**);
	~UserThr();
	Thread * clone() const;

	void run();

	int argc;
	char **argv;
};


#endif /* _userthr_h_ */
