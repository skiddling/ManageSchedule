#include "interruptflag.h"

thread_local InterruptFlag this_thread_interrupt_flag;

void interruption_point(){
	if (this_thread_interrupt_flag.is_set()) {
		throw thread_interrupted();
	}
}