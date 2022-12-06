#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "oslabs.h"
/*
	memory.c
	Lindsay Von Tish
	Operating Systems
	Fall 2022
	lmv9443@nyu.edu
*/

/*
	struct MEMORY_BLOCK {
		int start_address;
		int end_address;
		int segment_size;
		int process_id; //0 indicates a free block
	}
*/

/* Global Variables */
struct MEMORY_BLOCK NULLBLOCK = {.start_address = 0, .end_address = 0, .segment_size = 0, .process_id = 0};

/* Function Signatures */

/* My Functions */
/* print_BLOCK */
int print_BLOCK(struct MEMORY_BLOCK b);
/* print_MAP - Prints memory map*/
int print_MAP(struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt);
/* split_BLOCK */
int split_Block(struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int position, int request_size);
/* compare_BLOCK */
int compare_BLOCK(struct MEMORY_BLOCK M1, struct MEMORY_BLOCK M2);
/* merge_BLOCKS( */
int merge_BLOCKS(struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int start_position, int end_position);

/* Assignment Functions */
/* best_fit_allocate */
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
/* first_fit_allocate */
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
/* worst_fit_allocate */
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
/* next_fit_allocate */
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address);
/* release_memory */
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt);


/* Main Method */
/*
int main(int argc, char *argv[]) {

}
*/

/* My functions */

/* print_BLOCK */
int print_BLOCK(struct MEMORY_BLOCK b){
	printf("[%d, %d, %d, %d]\n", b.start_address, b.end_address, b.segment_size, b.process_id);
	return(0);
}

/* print_MAP - Prints memory map*/
int print_MAP(struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt){
	for(int i = 0; i < *map_cnt; i++){
		print_BLOCK(memory_map[i]);
	}
	return(0);
}

int split_Block(struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int position, int request_size){
	// Shift BLOCKs after the split forward
	for(int i = *map_cnt; i > position; i--){
		memory_map[i] = memory_map[i -1];
	}
	// Split block at position
	struct MEMORY_BLOCK og_BLOCK = memory_map[position];
	memory_map[position].segment_size = request_size;
	memory_map[position].end_address = memory_map[position].start_address + request_size - 1;

	memory_map[position + 1].segment_size -= request_size;
	memory_map[position + 1].start_address = memory_map[position].end_address + 1;

	++*map_cnt;

	return(0);
}


/* compare_BLOCK */
int compare_BLOCK(struct MEMORY_BLOCK M1, struct MEMORY_BLOCK M2){
	if(M1.segment_size != M2.segment_size){
		return(0);
	}else if(M1.start_address != M2.start_address){
		return(0);
	}else if(M1.end_address != M2.end_address){
		return(0);
	}else if(M1.process_id != M2.process_id){
		return(0);
	}else{
		return(1);
	}
}

/* merge_BLOCKS( */
int merge_BLOCKS(struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int start_position, int end_position){
	// Create one block that will contain the others
	int s = memory_map[start_position].start_address;
	int e = memory_map[end_position].end_address;
	int n = end_position - start_position;

	struct MEMORY_BLOCK mega_BLOCK = {.start_address = s, .end_address = e, .segment_size = e - s + 1, .process_id = 0};
	//printf("Created new mega_BLOCK!\n");
	//print_BLOCK(mega_BLOCK);

	// Place megaBlock in the map and shift additional blocks backwards
	memory_map[start_position] = mega_BLOCK;
	for(int i = end_position + 1; i < *map_cnt - n; i++){
		memory_map[i] = memory_map[i + n];
	}
	//printf("map_cnt = %d", *map_cnt);
	*map_cnt = *map_cnt - n;
	//printf("map_cnt = %d", *map_cnt);
	return(0);
}

/* Assignment Functions */
/*
best_fit_allocate
	This method allocates memory according to the Best Fit scheme.
	Inputs
		process id of the requesting process
		size of the memory being requested
		the memory map
	It finds the candidate memory blocks that can be allocated and chooses the one whose size is closest to the requested size.
*/
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
	// Find memory block
	int sizeDiff;
	int bestDiff = memory_map[0].segment_size - request_size;
	int position = 0;

	for(int i = 0; i < *map_cnt; i++){
		sizeDiff = memory_map[i].segment_size - request_size;
		// Only care about free blocks w/ a PID of 0 with enough memory to fit the new block
		if(memory_map[i].process_id == 0 && sizeDiff >= 0){
			if(sizeDiff == 0){
				/*
					If the free block found is exactly of the same size as the requested size
						the method updates the process id to allocate it
						returns this memory block.
					Can return first exact fit memory block
				*/
				memory_map[i].process_id = process_id;
				return(memory_map[i]);
			}else if(sizeDiff < bestDiff && bestDiff >= 0){
				position = i;
				bestDiff = sizeDiff;
			}else if(bestDiff < 0){
				position = i;
				bestDiff = sizeDiff;
			}
		}
	}

	if(bestDiff < 0){
		// if there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK.
		return(NULLBLOCK);
	}else{
		/*
			If the free block found is larger than the requested size
			the block is split into two pieces
				the first piece allocated
				the second piece becoming a free block in the memory map
		*/

		split_Block(memory_map, map_cnt, position, request_size);
		memory_map[position].process_id = process_id;
		return(memory_map[position]);
	}
}


/* first_fit_allocate */
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
	// Find memory block
	int sizeDiff;

	for(int i = 0; i < *map_cnt; i++){/* release_memory */
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt){
	//Find the free block in the map
	int position = -1;
	for(int i = 0; i < *map_cnt; i++){
		if(compare_BLOCK(freed_block, memory_map[i])){
			position = i;
			break;
		}
	}

	// Set memory block PID to 0
	memory_map[position].process_id = 0;

	//printf("Map before merge:\n");
	//print_MAP(memory_map, map_cnt);

	if(memory_map[position - 1].process_id == 0 && memory_map[position - 1].process_id == 0){
		merge_BLOCKS(memory_map, map_cnt, position - 1, position + 1);
	}else if(memory_map[position - 1].process_id == 0){
		merge_BLOCKS(memory_map, map_cnt, position - 1, position);
	}else if(memory_map[position + 1].process_id == 0){
		merge_BLOCKS(memory_map, map_cnt, position, position + 1);
	}

	//printf("Map after merge:\n");
	//print_MAP(memory_map, map_cnt);

}
		// Only care about free blocks w/ a PID of 0 with enough memory to fit the new block
		if(memory_map[i].process_id == 0){
			sizeDiff = memory_map[i].segment_size - request_size;
			if(sizeDiff == 0){
				// Perfect fit found!
				memory_map[i].process_id = process_id;
				return(memory_map[i]);
			}else if(sizeDiff > 0){
				// Good enough fit found!
				split_Block(memory_map, map_cnt, i, request_size);
				memory_map[i].process_id = process_id;
				return(memory_map[i]);
			}
		}
	}
	// Otherwise, no fit was found, return NULL
	return(NULLBLOCK);
}

/* worst_fit_allocate */
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
	// Find largest block of size 0
	int largest = -32;
	int position = -1;
	for(int i = 0; i < *map_cnt; i++){
		if(memory_map[i].segment_size > largest && memory_map[i].process_id == 0){
			largest = memory_map[i].segment_size;
			position = i;
		}
	}
	// Is the largest spot big enough to fit the new segment?
	if(request_size == largest){
		// Perfect Fit
		memory_map[position].process_id = process_id;
		return(memory_map[position]);
	}else if(request_size < largest){
		split_Block(memory_map, map_cnt, position, request_size);
		memory_map[position].process_id = process_id;
		return(memory_map[position]);
	}else{
		// Otherwise, the request is larger than the largest remaining memory block
		return(NULLBLOCK);
	}
}

//* next_fit_allocate */
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address){
	// Find the next block with enough memory
	int sizeDiff;
	
	for(int i = 0; i < *map_cnt; i++){
		if(memory_map[i].start_address >= last_address && memory_map[i].process_id == 0){
			// Only care about blocks AFTER the last_address with PID of 0
			if(memory_map[i].segment_size == request_size){
				// Perfect fit
				memory_map[i].process_id = process_id;
				return(memory_map[i]);
			}else if(memory_map[i].segment_size > request_size){
				// Good enough fit
				split_Block(memory_map, map_cnt, i, request_size);
				memory_map[i].process_id = process_id;
				return(memory_map[i]);
			}
		}
	}
	//Otherwise, there is no fit after the last address that is big enough
	return(NULLBLOCK);
}


	/* release_memory */
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt){
	//Find the free block in the map
	int position = -1;
	for(int i = 0; i < *map_cnt; i++){
		if(compare_BLOCK(freed_block, memory_map[i])){
			position = i;
			break;
		}
	}

	// Set memory block PID to 0
	memory_map[position].process_id = 0;

	//printf("Map before merge:\n");
	//print_MAP(memory_map, map_cnt);
	

	if(position != 0 && position != *map_cnt){
		if(memory_map[position - 1].process_id == 0 && memory_map[position - 1].process_id == 0){
			//printf("Blocks on either side are free\n");
			merge_BLOCKS(memory_map, map_cnt, position - 1, position + 1);
		}else if(memory_map[position - 1].process_id == 0){
			//printf("Block before is free\n");
			merge_BLOCKS(memory_map, map_cnt, position - 1, position);
		}else if(memory_map[position + 1].process_id == 0){
			//printf("block after is free\n");
			merge_BLOCKS(memory_map, map_cnt, position, position + 1);
		}
	}else if(position != 0 && memory_map[position + 1].process_id == 0){
		//printf("block after is free\n");
		merge_BLOCKS(memory_map, map_cnt, position, position + 1);
	}else if(position != *map_cnt && memory_map[position - 1].process_id == 0){
		//printf("Block before is free\n");
		merge_BLOCKS(memory_map, map_cnt, position - 1, position);
	}

	//printf("Map after merge:\n");
	//print_MAP(memory_map, map_cnt);

}