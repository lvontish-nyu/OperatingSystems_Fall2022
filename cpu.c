#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "oslabs.h"
/*
	cpu.c
	Lindsay Von Tish
	Operating Systems
	Fall 2022
	lmv9443@nyu.edu
*/

/* Struct Declaration - PCB */
/*
Looks like it was already declared in oslabs.h
struct PCB {
	int process_id;
	int arrival_timestamp;
	int total_bursttime;
	int execution_starttime;
	int execution_endtime;
	int remaining_bursttime;
	int process_priority;
}
*/

/* Global Variables */
struct PCB NULLPCB = {.process_id = 0, .arrival_timestamp = 0, .total_bursttime = 0, .execution_starttime = 0, .execution_endtime = 0, .remaining_bursttime = 0, .process_priority = 0};


/* Function Declaration - compare_PCB */
int compare_PCB(struct PCB PCB1, struct PCB PCB2);

/* Function Declaration - print_PCB */
int print_PCB(struct PCB p);

/* Function Declaration - handle_process_arrival_pp */
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp);

/* Function Declaration - handle_process_completion_pp */
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);

/* Function Declaration - handle_process_arrival_srtp */
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp);

/* Function Declaration - handle_process_completion_srtp */
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);

/* Function Declaration - handle_process_arrival_rr */
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum);

/* Function Declaration - handle_process_completion_rr( */
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum);


/* Main Method */
/*
int main(int argc, char *argv[]) {
	

}
*/



/*
	Process to compare the members of two PCBs
*/
int compare_PCB(struct PCB PCB1, struct PCB PCB2){
	if(PCB1.process_id != PCB2.process_id){
		return(0);
	}else if(PCB1.arrival_timestamp != PCB2.arrival_timestamp){
		return(0);
	}else if(PCB1.total_bursttime != PCB2.total_bursttime){
		return(0);
	}else if(PCB1.execution_starttime != PCB2.execution_endtime){
		return(0);
	}else if(PCB1.execution_endtime != PCB2.execution_endtime){
		return(0);
	}else if(PCB1.remaining_bursttime != PCB2.remaining_bursttime){
		return(0);
	}else if(PCB1.process_priority != PCB2.process_priority){
		return(0);
	}else{
		return(1);
	}
}

/*
	Process to print a PCB
*/
int print_PCB(struct PCB p){
	printf("Contents of PCB \n [%d, %d, %d, %d, %d, %d, %d] \n", p.process_id, p.arrival_timestamp, p.total_bursttime, p.execution_starttime, p.execution_endtime, p.remaining_bursttime, p.process_priority);
	return(0);
}

/* handle_process_arrival_pp
	This function implements the logic to handle the arrival of a new process in a Priority-based Preemptive Scheduler. Specifically, it takes five inputs:
		1. the ready queue (an array of PCB structs)
		2. The number of items in the ready queue
		3. the PCB of the currently running process
		4. the PCB of the newly arriving process
		5. the current timestamp.
		The method determines the process to execute next and returns its PCB.

*/
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
	/*
		If there is no currently-running process (i.e., the third argument is the NULLPCB)
			In this case, the PCB of the new process is modified so that
				the execution start time is set to the current timestamp
				the execution end time is set to the sum of the current	timestamp and the total burst time
				the remaining burst time is set to the total burst time.
			the method returns the PCB of the newly-arriving process,
	*/
	if(compare_PCB(current_process, NULLPCB)){
		new_process.execution_starttime = timestamp;
		new_process.execution_endtime = timestamp + new_process.total_bursttime;
		new_process.remaining_bursttime = new_process.total_bursttime;
		return(new_process);
	}

	/*
		If there is a currently-running process
			the method compares the priority of the newly-arriving process with the currently-running process.
	*/
	if(new_process.process_priority >= current_process.process_priority){
		/*
			If the new process has equal or lower priority then
				its execution start time and execution end time are set to 0
				the remaining burst time is the same as its total burst time.
				its PCB is added to the ready queue
			the return value is the PCB of the currently-running process.
		*/
		new_process.execution_starttime = 0;
		new_process.execution_endtime = 0;
		ready_queue[*queue_cnt] = new_process;
		++*queue_cnt;

		return(current_process);
	}else{
		/*
			If the new process has a higher priority then
				the PCB of the new process is modified so that
					execution start time is set to the current timestamp
					execution end time is set to the sum of the current timestamp and the total burst time
					remaining burst time is set to the total burst time.
				the return value is the PCB of the new process.
		*/
		new_process.execution_starttime = timestamp;
		new_process.execution_endtime = timestamp + new_process.total_bursttime;
		new_process.remaining_bursttime = new_process.total_bursttime;
		/*

				The PCB of the currently-running process is added to the ready queue
					after marking its execution end time as 0
					and adjusting its remaining burst time.
		*/
		current_process.execution_endtime = 0;
		current_process.remaining_bursttime --;
		ready_queue[*queue_cnt] = current_process;
		++*queue_cnt;

		return(new_process);

	}


}


struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
	return(NULLPCB);
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
	return(NULLPCB);
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
	return(NULLPCB);
}


struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
	return(NULLPCB);
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum){
	return(NULLPCB);
}