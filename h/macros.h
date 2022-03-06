/*
 * macros.h
 *
 *  Created on: Jul 14, 2021
 *      Author: OS1
 */

#ifndef _macros_h_
#define _macros_h_

#define clearInt asm{\
	pushf;\
	cli;\
}

#define setInt asm popf

#endif /* _macros_h_ */
