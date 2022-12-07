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
/* compare_RCB */
int compare_RCB(struct RCB rcb1, struct RCB rcb2);
/* remove_RCB */
int remove_RCB(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int position);
/* handle_arrivals */
struct RCB handle_arrivals(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);

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
/* compare_RCB */
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

/* remove_RCB */
int remove_RCB(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int position){
	for(int i = position; i < *queue_cnt - 1; i++){
		request_queue[i] = request_queue[i + 1];
	}
	(*queue_cnt)--;
	request_queue[*queue_cnt] = NULLRCB;
	return(1);
}

/* handle_arrivals */
struct RCB handle_arrivals(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	// Update arrival timestamp
	new_request.arrival_timestamp = timestamp;	
	// If the disk is free (indicated by the third parameter being a NULLRCB):
	if(compare_RCB(current_request, NULLRCB)){
		// The method returns the RCB of the newly-arriving request
		return(new_request);
	}
	// Otherwise
	// Add new request to the request queue
	request_queue[*queue_cnt] = new_request;
	(*queue_cnt)++;
	// Return the RCB of the currently-serviced request
	return(current_request);
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
	// Uses handle_arrivals function to avoid duplicate code
	return(handle_arrivals(request_queue, queue_cnt, current_request, new_request, timestamp));
}

/* handle_request_completion_fcfs */
/*
	This method implements the logic to handle the completion of servicing an IO request in a First-Come-First-Served Scheduler. 
	It takes two inputs:
		1. the request queue (an array of RCB structs).
		2. The number of items in the request queue
	The method determines the request to service next and returns its RCB.
*/
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
	// If the request queue is empty:
	if(*queue_cnt == 0){
		// return NULLRCB (indicating that there is no request to service next)
		return(NULLRCB);
	}
	// Otherwise:
	// Find the RCB in the request queue that has the earliest arrival time
	int min_AT = INT_MAX;
	int position = -1;
	for(int i = 0; i < *queue_cnt; i++){
		if(request_queue[i].arrival_timestamp < min_AT){
			min_AT = request_queue[i].arrival_timestamp;
			position = i;
		}
	}
	struct RCB nextRCB = request_queue[position];
	// Remove this RCB from the request queue
	remove_RCB(request_queue, queue_cnt, position);
	// Return the removed RCB
	return(nextRCB);
}

/* handle_request_arrival_sst */
/*
	This method implements the logic to handle the arrival of a new IO request in a Shortest-Seek-First (also known as Shortest-Seek-Time-First) Scheduler.
	Takes five inputs:
		1. the request queue (an array of RCB structs)
		2. The number of items in the request queue
		3. the RCB of the currently-serviced request
		4. the RCB of the newly-arriving request
		5. the current timestamp.
*/
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	// Uses handle_arrivals function to avoid duplicate code
	return(handle_arrivals(request_queue, queue_cnt, current_request, new_request, timestamp));

}
/* Handle_request_completion_sstf */
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
	// If the request queue is empty:
	if(*queue_cnt == 0){
		// return NULLRCB (indicating that there is no request to service next)
		return(NULLRCB);
	}
	// Otherwise:
	int min_AT = INT_MAX;
	int min_DIFF = INT_MAX;
	int position = -1;
	int diff;
	// Find the RCB in the request queue who's cylinder is closest to the current cylinder.
	for(int i = 0; i < *queue_cnt; i++){
		diff = abs(request_queue[i].cylinder - current_cylinder);
		if(diff < min_DIFF){
			min_DIFF = diff;
			min_AT = request_queue[i].arrival_timestamp;
			position = i;
		}else if(diff == min_DIFF && request_queue[i].arrival_timestamp < min_AT){
			//  If there are multiple requests with the closest cylinder:
				// The method picks the request among these that has the earliest arrival_timestamp
			min_DIFF = diff;
			min_AT = request_queue[i].arrival_timestamp;
			position = i;
		}
	}
	struct RCB nextRCB = request_queue[position];
	// Remove this RCB from the request queue
	remove_RCB(request_queue, queue_cnt, position);
	// Return the removed RCB
	return(nextRCB);
}

/* Handle_request_arrival_look */
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	// Uses handle_arrivals function to avoid duplicate code
	return(handle_arrivals(request_queue, queue_cnt, current_request, new_request, timestamp));
}
/* Handle_request_completion_look */
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction){
	return(NULLRCB);
}