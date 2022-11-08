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
int main(int argc, char *argv[]) {
	// Initialize memory map for testing
	struct MEMORY_BLOCK B0 = {.start_address = 0, .end_address = 3, .segment_size = 4, .process_id = 1};
	struct MEMORY_BLOCK B1 = {.start_address = 4, .end_address = 20, .segment_size = 17, .process_id = 0};
	struct MEMORY_BLOCK B2 = {.start_address = 21, .end_address = 30, .segment_size = 10, .process_id = 0};
	struct MEMORY_BLOCK B3 = {.start_address = 0, .end_address = 3, .segment_size = 4, .process_id = 2};

	int c;
	int *map_cnt = &c;
	*map_cnt = 4;

	struct MEMORY_BLOCK memory_map[MAPMAX] = {B0, B1, B2, B3};
	printf("Map Initialized\n");
	printf("Map Size = %d\n", *map_cnt);
	print_MAP(memory_map, map_cnt);

	// Add a new block with PID 3 and size 10
	best_fit_allocate(10, memory_map, map_cnt, 3);
	printf("Added new block\nMap:\n");
	printf("Map Size = %d\n", *map_cnt);
	print_MAP(memory_map, map_cnt);

	// Add a new block that is too big
	best_fit_allocate(30, memory_map, map_cnt, 4);
	printf("Added new block\nMap:\n");
	printf("Map Size = %d\n", *map_cnt);
	print_MAP(memory_map, map_cnt);

	//Add a block to split the map
	best_fit_allocate(7, memory_map, map_cnt, 5);
	printf("Added new block\nMap:\n");
	printf("Map Size = %d\n", *map_cnt);
	print_MAP(memory_map, map_cnt);

}

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
	printf("Original Block\n");
	print_BLOCK(memory_map[position]);

	for(int i = *map_cnt; i > position; i--){
		memory_map[i] = memory_map[i -1];
	}
	printf("Blocks shifted\n");
	// Split block at position
	struct MEMORY_BLOCK og_BLOCK = memory_map[position];
	memory_map[position].segment_size = request_size;
	memory_map[position].end_address = memory_map[position].start_address + request_size - 1;

	memory_map[position + 1].segment_size -= request_size;
	memory_map[position + 1].start_address = memory_map[position].end_address + 1;

	printf("Block 1\n");
	print_BLOCK(memory_map[position]);
	printf("\nBlock 2\n");
	print_BLOCK(memory_map[position + 1]);

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
		printf("Comparing:\n	bestDiff = %d at position %d");
		if(memory_map[i].process_id == 0 && sizeDiff >= 0){
			if(sizeDiff == 0){
				/*
					If the free block found is exactly of the same size as the requested size
						the method updates the process id to allocate it
						returns this memory block.
					Can return first exact fit memory block
				*/
				printf("Blocks are the same size\n Returning BLOCK of size %d at position %d\n", memory_map[i].segment_size, i);
				memory_map[i].process_id = process_id;
				return(memory_map[i]);
			}else if(sizeDiff < bestDiff && bestDiff >= 0){
				printf("New BLOCK of size %d at position %d is better fit\n", sizeDiff, i);
				position = i;
				bestDiff = sizeDiff;
			}
		}
	}

	printf("Best fit found!\n SizeDiff = %d\n Size = %d\n, position = %d\n", bestDiff, memory_map[position].segment_size, position);

	if(bestDiff < 0){
		// if there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK.
		printf("No free block.  Returning NULL\n");
		return(NULLBLOCK);
	}else{
		/*
			If the free block found is larger than the requested size
			the block is split into two pieces
				the first piece allocated
				the second piece becoming a free block in the memory map
		*/
		printf("Reallocating block at position %d of size %d\n", position, memory_map[position].segment_size);
		printf("Original Map:\n");
		print_MAP(memory_map, map_cnt);

		split_Block(memory_map, map_cnt, request_size, position);
		memory_map[position].process_id = process_id;

		printf("\nNew Map\n");
		print_MAP(memory_map, map_cnt);

	}
}


/* first_fit_allocate */
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
/* worst_fit_allocate */
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
/* next_fit_allocate */
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address);
/* release_memory */
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt);