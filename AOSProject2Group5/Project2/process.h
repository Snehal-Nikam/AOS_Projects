#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "utility.h"

struct process_S
{
	unsigned char pid;
	float arrivalTime;
	float runTime;
	unsigned char priority;
};

typedef struct process_S process;

process *create_Process(char pid, float arrivalTime, float runTime, unsigned char priority);

linked_List *generate_Processes(int n);

#endif