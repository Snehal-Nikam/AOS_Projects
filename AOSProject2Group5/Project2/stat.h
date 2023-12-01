#ifndef STAT_H_
#define STAT_H_
#include "process.h"

enum State
{
	UNUSED,
	EMBRYO,
	SLEEPING,
	RUNNING,
	ZOMBIE
};

struct processStat_S
{
	process *proc;
	float wait_Time;
	float turnAround_time;
	float responseTime;

	float start_Time;
	float end_Time;
	float runTime;

	enum State state;
};
typedef struct processStat_S processStat;

struct averageStats_s
{
	float averageResponseTime;
	float averageWaitTime;
	float averageTurnAround;
	float avg_Throughput;
};

typedef struct averageStats_s averageStats;

averageStats firstComeFirstServeNP(linked_List *processes);
averageStats shortestJobFirstNP(linked_List *processes);
averageStats shortestRemainingTimeP(linked_List *processes);
averageStats roundRobinP(linked_List *processes, int time_slice);
averageStats highestPriorityFirstP(linked_List *processes);
averageStats highestPriorityFirstNP(linked_List *processes);

// Print Stat//
averageStats printPolicy_Stat(linked_List *ll);
#endif
