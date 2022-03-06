/*
 * timer.cpp
 *
 *  Created on: Jul 13, 2021
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include "timer.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "macros.h"
#include "thread.h"
#include "krnlsem.h"

volatile int Timer::timeSlice = defaultTimeSlice;
volatile int Timer::requestedContextChange = 0;
volatile int Timer::waitingContextChange = 0;
volatile int Timer::lck = 0;

volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;

unsigned oldTimerOFF;
unsigned oldTimerSEG;

extern void tick();

void interrupt timer(...) {
	if (Timer::requestedContextChange == 0) {
		tick();

		for (List::Node* p = KernelSem::semList.head; p != 0; p = p->next) {
			((KernelSem*)(p->data))->update();
		}

		if (Timer::timeSlice > 0){
			Timer::timeSlice--;
		}
	}

	if ((Timer::timeSlice == 0 && PCB::running->limited == 1 && Timer::lck == 0) || Timer::requestedContextChange == 1) {
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;
		PCB::running->lck = Timer::lck;

		if (PCB::running->state != PCB::TERMINATING &&
				PCB::running->state != PCB::SUSPENDED &&
				PCB::running != PCB::idle) {
			PCB::running->state = PCB::READY;
			Scheduler::put((PCB*)PCB::running);
		}

		PCB::running = Scheduler::get();
		if (PCB::running == 0) {		// empty scheduler
			PCB::running = PCB::idle;
		}

		PCB::running->state = PCB::RUNNING;

		Timer::lck = PCB::running->lck;
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		Timer::timeSlice = PCB::running->timeSlice;

		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}

	if (Timer::timeSlice == 0 && PCB::running->limited == 1 && Timer::lck > 0) Timer::waitingContextChange = 1;
	else Timer::waitingContextChange = 0;

	if(Timer::requestedContextChange == 0) asm int 60h;

	Timer::requestedContextChange = 0;
}

void Timer::init(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		//sti
	}

	// the rest is done in the constructor
	// ss, sp, bp are assigned with the first context switch
	Timer::timeSlice = PCB::initial.timeSlice;
	PCB::initial.state = PCB::RUNNING;
	PCB::running = &PCB::initial;

	asm sti;
}

// vraca staru prekidnu rutinu
void Timer::restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}

	delete PCB::idle->myThread;
}


void dispatch(){
	clearInt;
	Timer::requestedContextChange = 1;
	timer();
	setInt;
}

