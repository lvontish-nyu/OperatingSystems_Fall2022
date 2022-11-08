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

/* Global Variables */

/* Function Signatures */
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
