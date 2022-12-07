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
/*
	This method implements the logic to handle the completion of servicing an IO request in a Shortest-Seek-Time-First Scheduler.
	Takes three inputs:
		1. the request queue (an array of RCB structs)
		2. The number of items in the request queue
		3. the current cylinder.
	The method determines the request to service next and returns its RCB.
*/
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	// Uses handle_arrivals function to avoid duplicate code
	return(handle_arrivals(request_queue, queue_cnt, current_request, new_request, timestamp));
}

/* Handle_request_completion_look */
/*
	This method implements the logic to handle the completion of servicing an IO request in a LOOK Scheduler.
	It takes four input parameters:
		1. the request queue (an array of RCB structs),
		2. The number of items in the request queue
		3. the current cylinder
		4. the scan direction.
	The method determines the request to service next and returns its RCB.
*/
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction){
	// If the request queue is empty:
	if(*queue_cnt == 0){
		// return NULLRCB (indicating that there is no request to service next)
		return(NULLRCB);
	}
	// Otherwise - choose next request to service from queue
	int min_AT = INT_MAX;
	int position = -1;
	int same_cylinder_found = 0;

	int min_DIFF_smaller = INT_MAX * -1;
	int min_DIFF_larger = INT_MAX;
	int pS = -1;
	int pL = -1;
	int min_AT_S = INT_MAX;
	int min_AT_L = INT_MAX;
	int diff;
	int larger_found = 0;
	int smaller_found = 0;
	for(int i = 0; i < *queue_cnt; i++){
		// If there are requests in the queue with the same cylinder as the current cylinder, pick the one among these requests with the earliest arrival time.
		if(request_queue[i].cylinder == current_cylinder && request_queue[i].arrival_timestamp < min_AT){
			min_AT = request_queue[i].arrival_timestamp;
			position = i;
			same_cylinder_found = 1;
		}else if(!same_cylinder_found){
			// Otherwise, if a request on the same cylinder hasn't been found:
			diff = request_queue[i].cylinder - current_cylinder;
			// If the diff is negative, than the cylinder is smaller.  Compare it to the closest request on a smaller cylinder.
			if(diff < 0){
				// If this is closer than the previous closest request on a smaller cylinder, save it
				if(diff > min_DIFF_smaller){
					min_DIFF_smaller = diff;
					min_AT_S = request_queue[i].arrival_timestamp;
					pS = i;
					smaller_found = 1;
				}else if(diff == min_DIFF_smaller && request_queue[i].arrival_timestamp < min_AT_S){
					//  If there are multiple requests with the closest cylinder, then pick the request among these that has the earliest arrival_timestamp
					min_DIFF_smaller = diff;
					min_AT_S = request_queue[i].arrival_timestamp;
					pS = i;
					smaller_found = 1;
				}
			}else if(diff > 0){
				// If the diff is positive, then the cylinder is larger.  Compare it to the closest request on a smaller cylinder
				if(diff < min_DIFF_larger){
					// If this is closer than the previous closest request on a larger cylinder, save it
					min_DIFF_larger = diff;
					min_AT_L = request_queue[i].arrival_timestamp;
					pL = i;
					larger_found = 1;
				}else if(diff == min_DIFF_larger && request_queue[i].arrival_timestamp < min_AT_L){
					// If there are multiple requests with the closest cylinder, then pick the request among these that has the earliest arrival_timestamp
					min_DIFF_larger = diff;
					min_AT_L = request_queue[i].arrival_timestamp;
					pL = i;
					larger_found = 1;
				}
			}
		}
	}
	// If there are requests in the queue with the same cylinder as the current cylinder, pick the one with the earliest arrival_timestamp
	struct RCB nextRCB = NULLRCB;
	int P = 0;
	if(same_cylinder_found){
		nextRCB = request_queue[position];
		P = position;
	} else if(scan_direction){
		// If the scan direction is 1 
		// If there are requests with cylinders larger than the current cylinder: 
		if(larger_found){
			// Pick one among these whose cylinder is closest to the current cylinder
			nextRCB = request_queue[pL];
			P = pL;
		}else{
			// If there are no requests with cylinders larger than the current cylinder:
				// Pick the request whose cylinder is closest to the current cylinder.
			nextRCB = request_queue[pS];
			P = pS;
		}
	}else{
		// Otherwise, if the scan direction is 0:
		// If there are requests with cylinders smaller than the current cylinder:
		if(smaller_found){
			// Pick the one among these whose cylinder is closest to the current cylinder
			nextRCB = request_queue[pS];
			P = pS;
		}else{
			// If there are requests with cylinders larger than the current cylinder:
				// Pick the request whose cylinder is closest to the current cylinder.
			nextRCB = request_queue[pL];
			P = pL;
		}
	}
	// Remove this RCB from the request queue
	remove_RCB(request_queue, queue_cnt, P);
	// Return the removed RCB
	return(nextRCB);
}