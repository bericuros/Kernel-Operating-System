/*
 * main.cpp
 *
 *  Created on: May 12, 2021
 *      Author: OS1
 */

#include "timer.h"
#include "userthr.h"

int ret;

extern int userMain(int argc, char** argv);

int main(int argc, char** argv){

	Timer::init();

	UserThr userthr(argc, argv);
	userthr.start();
	userthr.waitToComplete();

	Timer::restore();

	return ret;
}

