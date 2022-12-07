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
struct RCB NULLRCB = {.request_id = 0, .arrival_timestamp = 0, .cylinder = 0, .address = 0, process_id= 0};

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


/* Assignment Functions */
/* handle_request_arrival_fcfs */
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
	return(NULLRCB);
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