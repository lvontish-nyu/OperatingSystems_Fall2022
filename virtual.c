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
	struct PTE {
		int is_valid;
		int frame_number;
		int arrival_timestamp;
		int last_access_timestamp;
		int reference_count;
	}
*/


/* Assignment Function Signatures */
/* process_page_access_fifo */
int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
/* count_page_faults_fifo */
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
/* process_page_access_lru */
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
/* count_page_faults_lru */
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
/* process_page_access_lfu */
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
/* count_page_faults_lfu */
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);



/* Assignment Functions */
/* process_page_access_fifo */
/*
	This function implements the logic to process a page access in a system that uses the First-In First-Out (FIFO) policy for page replacement
		It takes four inputs:
		1. process page table
		2. logical page number
		3. process frame pool
		4. current timestamp.
	The function determines the memory frame number for the logical page and returns this number.
*/
int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
	int fn;
	/* First the function checks if the page being referenced is already in memory (i.e., the page-table entry has the valid bit true). */
	if(page_table[page_number].is_valid){
		/*
		If so:
			Modifies the last_access_timestamp and the reference_count fields of the page-table entry.
			returns the frame number
		*/
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count ++;
		return(page_table[page_number].frame_number);
	}
	/*
	If the page being referenced is not in memory:
		the function checks if there are any free frames (i.e., the process frame pool is not empty)
	*/
	if(*frame_cnt > 0){
		/*
		If so, a frame is removed from the process frame pool
			the frame number is inserted into the page-table entry corresponding to the logical page number.
			the other fields of the page-table entry are set appropriately.
			The function returns the frame number associated with the page-table entry.
		*/

		fn = frame_pool[*frame_cnt - 1];

		page_table[page_number].frame_number = fn;
		page_table[page_number].arrival_timestamp = current_timestamp;
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count = 1;
		page_table[page_number].is_valid = 1;

		(*frame_cnt)--;
		return(fn);
	}
	/*
	If the page being referenced is not in memory and there are no free frames for the process
		a page needs to be replaced.
	*/
	
	// The function selects among all the pages of the process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest arrival_timestamp.
	int min_ts = INT_MAX;
	int position = -1;
	for(int i = 0; i < *table_cnt; i++){
		if(page_table[i].is_valid && page_table[i].arrival_timestamp < min_ts){
			min_ts = page_table[i].arrival_timestamp;
			position = i;
		}
	}
	// Get the frame number used by that page;
	fn = page_table[position].frame_number;
	// It marks that page_table entry as invalid, along with setting the frame_number, arrival_timestamp, last_access_timestamp and reference_count to -1.
	page_table[position].is_valid = 0;
	page_table[position].frame_number = -1;
	page_table[position].arrival_timestamp = -1;
	page_table[position].last_access_timestamp = -1;
	page_table[position].reference_count = -1;
	// It then sets the frame_number of the page-table entry of the newly-referenced page to the newly freed frame.
	page_table[page_number].frame_number = fn;
	// It also sets the arrival_timestamp, the last_access_timestamp and the reference_count fields of the page-table entry appropriately.
	page_table[page_number].arrival_timestamp = current_timestamp;
	page_table[page_number].last_access_timestamp = current_timestamp;
	page_table[page_number].reference_count = 1;
	page_table[page_number].is_valid = 1;
	// Finally, the function returns this frame number.
	return(fn);
		
}

/* count_page_faults_fifo */
/*
It takes three inputs:
	1. process page table
	2. sequence of logical page numbers
	3. process frame pool.
The function returns the estimated number of page faults for the reference string, with respect to the pool of frames allocated to the process.
For each logical page number (in the sequence), the function simulates the processing of the page access in the FIFO system.
It keeps track of the number of page faults that occur in the system as it simulates the processing of the entire sequence of logical page numbers.
*/
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
	// In order to simulate timestamps, the function starts with a timestamp of 1 and increments it whenever the processing of a new page access is begun.
	int current_timestamp = 1;
	// # Page Faults encountered
	int page_faults = 0;
	int fn;
	int min_ts = INT_MAX;
	int position = -1;

	// Processing each page in the reference string
	for(int i = 0; i < reference_cnt; i++){
		// Get logical page number of the current page
		int logical_page_number = refrence_string[i];
	
		//Check if the page being referenced is already in memory (i.e., the page-table entry has the valid bit true).
		if(page_table[logical_page_number].is_valid){
			// modify the last_access_timestamp and the reference_count fields of the page-table entry.
			page_table[logical_page_number].last_access_timestamp = current_timestamp;
			page_table[logical_page_number].reference_count++;
		}else{
			// Otherwise, the page is not in memory
			// Check for free frames (i.e., the process frame pool is not empty)
			page_faults++;
			if(frame_cnt > 0){
				//  Insert the frame number into page-table entry coorresponding to the logical page
				// Set fields of page-table entry
				page_table[logical_page_number].is_valid = 1;
				page_table[logical_page_number].frame_number = frame_pool[frame_cnt - 1];
				page_table[logical_page_number].arrival_timestamp = current_timestamp;
				page_table[logical_page_number].last_access_timestamp = current_timestamp;
				page_table[logical_page_number].reference_count = 1;
				//Remove frame from process frame pool
				frame_pool[frame_cnt - 1] = -1;
				frame_cnt--;
				// Count the page fault
				//page_faults++;
			} else {
				// Otherwise, there are no free frames in the process frame pool
				// The function selects among all the pages of the process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest arrival_timestamp.
				min_ts = INT_MAX;
				position = -1;
				for(int i = 0; i < table_cnt; i++){
					if(page_table[i].is_valid && page_table[i].arrival_timestamp < min_ts){
						min_ts = page_table[i].arrival_timestamp;
						position = i;
					}
				}
				// Get the frame number used by that page;
				fn = page_table[position].frame_number;
				// It marks that page_table entry as invalid, along with setting the frame_number, arrival_timestamp, last_access_timestamp and reference_count to -1.
				page_table[position].is_valid = 0;
				page_table[position].frame_number = -1;
				page_table[position].arrival_timestamp = -1;
				page_table[position].last_access_timestamp = -1;
				page_table[position].reference_count = -1;
				// It then sets the frame_number of the page-table entry of the newly-referenced page to the newly freed frame.
				page_table[logical_page_number].frame_number = fn;
				// It also sets the arrival_timestamp, the last_access_timestamp and the reference_count fields of the page-table entry appropriately.
				page_table[logical_page_number].arrival_timestamp = current_timestamp;
				page_table[logical_page_number].last_access_timestamp = current_timestamp;
				page_table[logical_page_number].reference_count = 1;
				page_table[logical_page_number].is_valid = 1;
				// Count the page fault
				//page_faults++;
			}
		}
		current_timestamp++
	}
	return(page_faults);
}


/* process_page_access_lru */
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
	return(0);
}
/* count_page_faults_lru */
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
	return(0);
}
/* process_page_access_lfu */
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
	return(0);
}
/* count_page_faults_lfu */
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
	return(0);
}
