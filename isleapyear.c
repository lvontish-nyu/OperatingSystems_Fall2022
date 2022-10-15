#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
	isleapyear.c
	Lindsay Von Tish
	Operating Systems
	Fall 2022
	lmv9443@nyu.edu
*/

/* Function Declaration - isLeapYear */
int isLeapYear(int year);


/* Main Method */
int main(int argc, char *argv[]) {
	char *y = argv[1];
	int year = atoi(y);
	int is = isLeapYear(year);
	if(is == 0){
		printf("%d was not a leap year", year);
	}else{
		printf("%d was a leap year", year);
	}
}

/* isLeapYear Function returning boolean int */
int isLeapYear(int year){
	int result = 0;

	//First check if year is divisible by four, return false if it isn't
	if(year%4 != 0){
		return(result);
	}
	// If year is divisible by four, then is it divisible by 100?
	if( year%100 == 0){
		// Is it divisible by 400?
		if(year%400 == 0){
			result = 1;
		}
	}else{
		result = 1;
	}

	return(result);
}