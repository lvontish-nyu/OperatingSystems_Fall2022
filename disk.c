#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "oslabs.h"
/*
	virtual.c
	Lindsay Von Tish
	Operating Systems
	Fall 2022
	lmv9443@nyu.edu
*/

/*
struct RCB {
	int request_id;
	int arrival_timestamp;
	int cylinder;
	int address;
	int process_id;
}
/* Global Variables */
struct RCB NULLRCB = {.request_id = 0, .arrival_timestamp = 0, .cylinder = 0, .address = 0, .process_id= 0};

/* My Function Declarations */
int compare_RCB(struct RCB rcb1, struct RCB rcb2);

/* Assignment Function Declarations*/
/* handle_request_arrival_fcfs */
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
/* handle_request_completion_fcfs */
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt);
/* handle_request_arrival_sst */
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
/* Handle_request_completion_sstf */
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder);
/* Handle_request_arrival_look */
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
/* Handle_request_completion_look */
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction);


/* My Functions */
int compare_RCB(struct RCB rcb1, struct RCB rcb2){
	if(rcb1.request_id != rcb2.request_id){
		return(0);
	}else if(rcb1.arrival_timestamp != rcb2.arrival_timestamp){
		return(0);
	}else if(rcb1.cylinder != rcb2.cylinder){
		return(0);
	}else if(rcb1.address != rcb2.address){
		return(0);
	}else if(rcb1.process_id != rcb2.process_id){
		return(0);
	}else{
		return(1);
	}
}

/* Assignment Functions */
/* handle_request_arrival_fcfs */
/*
	This method implements the logic to handle the arrival of a new IO request in a First-Come-First-Served Scheduler.
	Takes five inputs:
		1. the request queue (an array of RCB structs)
		2. The number of items in the request queue
		3. the RCB of the currently-serviced request
		4. the RCB of the newly-arriving request
		5. the current timestamp.
*/
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	// If the disk is free (indicated by the third parameter being a NULLRCB):
	if(compare_RCB(current_request, NULLRCB)){
		// The method returns the RCB of the newly-arriving request
		return(new_request);
	}
	// Otherwise
	// Add new request to the request queue
	request_queue[*queue_cnt] = new_request;
	(*request_cnt)++;
	// Return the RCB of the currently-serviced request
	return(current_request);
}
/* handle_request_completion_fcfs */
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
	return(NULLRCB);
}
/* handle_request_arrival_sst */
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	return(NULLRCB);
}
/* Handle_request_completion_sstf */
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
	return(NULLRCB);
}
/* Handle_request_arrival_look */
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	return(NULLRCB);
}
/* Handle_request_completion_look */
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction){
	return(NULLRCB);
}