/*
 * userthr.cpp
 *
 *  Created on: Aug 15, 2021
 *      Author: OS1
 */

#include "userthr.h"

extern int ret;
extern int userMain(int argc, char** argv);

UserThr::UserThr(int aArgc, char** aArgv) : Thread(), argc(aArgc), argv(aArgv) {}

UserThr::~UserThr() {
	this->waitToComplete();
}

Thread * UserThr::clone() const {
	return new UserThr(argc, argv);
}

void UserThr::run() {
	ret = userMain(this->argc, this->argv);
}
