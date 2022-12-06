/*
 * scheduler.h
 *
 *  Created on: Nov 28, 2022
 *      Author: nguye
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>


typedef struct{
	void (*pTask)(void);
	uint32_t 	Delay;
	uint32_t 	Period;
	uint8_t 	RunMe;

	uint32_t 	TaskID;
}sTasks;

#define SCH_MAX_TASKS	40

#define ERROR_SCH_TOO_MANY_TASKS 		0
#define ERROR_SCH_CANNOT_DELETE_TASK	1
#define RETURN_ERROR					2
#define RETURN_NORMAL					3


void SCH_Init(void);

unsigned char SCH_Add_Task ( void (*pFunction)() ,
					uint32_t DELAY,
					uint32_t PERIOD);

void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Delete_Task(uint16_t TASK_INDEX);
void SCH_Report_Status(void);
void Watchdog();
// my extra functions
void find_new_min_task(void);
void update_other_tasks(void);
#endif /* INC_SCHEDULER_H_ */

