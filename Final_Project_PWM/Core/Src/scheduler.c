/*
 * scheduler.c
 *
 *  Created on: Nov 28, 2022
 *      Author: nguye
 */

#include "scheduler.h"
#include "global.h"
#include <stdio.h>

sTasks SCH_tasks_G[SCH_MAX_TASKS];
int current_index_task = 0;
int Error_code_G = 0;
int min_index = 0;
int min_delay = 0;

void SCH_Init() {
	for (int i = 0; i < SCH_MAX_TASKS; i++) {
		SCH_Delete_Task(i);
	}
	Error_code_G = 0;
	//current_index_task = -1;
	//Timer_init();
	//Watchdog_init();
}

unsigned char SCH_Add_Task ( void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD) {
	// If user init SCH before, then assign current_index_task to zero (first index)
	unsigned char Return_code;

	if(current_index_task < SCH_MAX_TASKS){

		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY / TIMER_CYCLE;
		SCH_tasks_G[current_index_task].Period =  PERIOD / TIMER_CYCLE;
		SCH_tasks_G[current_index_task].RunMe = 0;

		SCH_tasks_G[current_index_task].TaskID = current_index_task;

		current_index_task++;

		Return_code = RETURN_NORMAL;

	} else {
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		Return_code = RETURN_ERROR;
	}

	return Return_code;
}

void SCH_Dispatch_Tasks(void) {
	int i = min_index;
	// check flag of min_index
	if (SCH_tasks_G[i].RunMe > 0) {
		update_other_tasks();
		(*SCH_tasks_G[i].pTask)();
		SCH_tasks_G[i].RunMe--;
		// If it's one-shot task, remove it form the array
		if (SCH_tasks_G[i].Period == 0) {
			SCH_Delete_Task(i);
		}
	}
	// Report system status
	SCH_Report_Status();
	// The scheduler enters idle mode at this point
	//SCH_Go_To_Sleep();
}


/* // Update function with O(n)
void SCH_Update(void) {
	for (int i = 0; i < current_index_task; i++) {
		if (SCH_tasks_G[i].Delay > 0) SCH_tasks_G[i].Delay--;
		else {
			 SCH_tasks_G[i].Delay =  SCH_tasks_G[i].Period;
			 SCH_tasks_G[i].RunMe += 1;
		}
	}
}*/

/* My new SCH_Update function */
void SCH_Update() {
	// if task is ready to run
	if (SCH_tasks_G[min_index].Delay > 0) SCH_tasks_G[min_index].Delay--;
	else {
		 SCH_tasks_G[min_index].Delay =  SCH_tasks_G[min_index].Period;
		 SCH_tasks_G[min_index].RunMe += 1;
	}
}

void Shift_Task_To_Left(int index_a, int index_b) {
	SCH_tasks_G[index_a].pTask = SCH_tasks_G[index_b].pTask;
	SCH_tasks_G[index_a].Delay = SCH_tasks_G[index_b].Delay;
	SCH_tasks_G[index_a].Period =  SCH_tasks_G[index_b].Period;
	SCH_tasks_G[index_a].RunMe = SCH_tasks_G[index_b].RunMe;

	SCH_tasks_G[index_a].TaskID = SCH_tasks_G[index_b].TaskID;
}

unsigned char SCH_Delete_Task(uint16_t TASK_INDEX) {
	unsigned char Return_code;
	if (current_index_task < 0 || current_index_task >= SCH_MAX_TASKS) {
		// No task at this location...
		// Set the global error variable
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		Return_code = RETURN_ERROR;
		return Return_code;
	} else {
		Return_code = RETURN_NORMAL;
	}
	// Shift task from TASK_INDEX + 1 to the left
	for (int i = TASK_INDEX; i < current_index_task; i++) {
		Shift_Task_To_Left(i, i + 1);
	}
	// Remove last index task
	SCH_tasks_G[current_index_task].pTask = 0x0000 ;
	SCH_tasks_G[current_index_task].Delay = 0;
	SCH_tasks_G[current_index_task].Period = 0;
	SCH_tasks_G[current_index_task].RunMe = 0;
	current_index_task--;

	return Return_code;
}

void update_other_tasks() {
	for (int i = 0; i < current_index_task; i++) {
		if (i == min_index) {
			continue;
		}

		SCH_tasks_G[i].Delay -= min_delay;
	}

	find_new_min_task();
}

void find_new_min_task() {
	// find new min index of schedule array
	min_index = 0;
	min_delay = SCH_tasks_G[min_index].Delay;

	for (int i = min_index + 1; i < current_index_task; i++) {
		if (SCH_tasks_G[i].Delay < min_delay) {
			min_index = i;
			min_delay = SCH_tasks_G[i].Delay;
		}
	}
}
void SCH_Report_Status(void) {
#ifdef SCH_REPORT_ERRORS
	// ONLY APPLIES IF WE ARE REPORTING ERRORS
	// Check for a new  error code
	if (Error_code_G != Last_error_code_G) {
		// Negative logic on LEDs assumed
		Error_port = 255 - Error_code_G;
		Last_error_code_G = Error_code_G;

		if (Error_code_G != 0) {
			Error_tick_count_G = 60000;
		} else {
			Error_tick_count_G = 0;
		}
	} else {
		if (Error_tick_count_G != 0) {
			if (--Error_tick_count_G == 0) {
				Error_code_G = 0; // Reset error code
			}
		}
	}
#endif
}
