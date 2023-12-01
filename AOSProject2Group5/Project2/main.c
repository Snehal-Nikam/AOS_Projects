#include <stdio.h>
#include "utility.h"
#include "process.h"
#include "stat.h"

int main(int argc, char **argv)
{
	int ctr = 0;

	// Arrays to store statistics for each of the scheduling algorithms

	averageStats fcfs[6], sjf[6], srf[6], rr[6], hpfp[6], hpfnp[6], final[6];

	// Loop to simulate the scheduling algorithms 5 times

	while (ctr < 5)
	{
		// Print header for this round
		printf("\n*********************************************************************************\n");
		printf("\t\t\t\tRound %d\n", ctr + 1);
		printf("*********************************************************************************\n");

		// Generate a list of processes

		linked_List *processList = generate_Processes(52);
		node *ptr = processList->head;

		// Print the generated processes

		printf("-------------------------------------------------\n");
		printf("Process ID | Arrival Time | Run Time | Priority |\n");
		printf("-------------------------------------------------\n");
		while (ptr != NULL)
		{
			process *proc = (process *)ptr->data;
			printf("%10c | %12.1f | %8.1f | %8d |\n", proc->pid, proc->arrivalTime, proc->runTime, proc->priority);
			ptr = ptr->next;
		}
		printf("--------------------------------------------------\n");
		printf("Total No. of Processes : %d\n", processList->size);
		printf("--------------------------------------------------\n");

		// First Come First Serve
		fcfs[ctr] = firstComeFirstServeNP(processList);
		// Round Robin
		rr[ctr] = roundRobinP(processList, 1);
		// Shortest Job First
		sjf[ctr] = shortestJobFirstNP(processList);
		// Shortest Remaining Time First
		srf[ctr] = shortestRemainingTimeP(processList);
		// Highest Priority First Preemptive
		hpfp[ctr] = highestPriorityFirstP(processList);
		// Highest Priority First Non-Preemptive
		hpfnp[ctr] = highestPriorityFirstNP(processList);
		ctr++;
	}

	for (int i = 0; i < 6; i++)
	{
		final[i].averageResponseTime = 0;
		final[i].averageWaitTime = 0;
		final[i].averageTurnAround = 0;
		final[i].avg_Throughput = 0;
	}

	// Initialize average statistics for final results
	for (int i = 0; i < 5; i++)
	{
		final[0].averageResponseTime += fcfs[i].averageResponseTime;
		final[1].averageResponseTime += sjf[i].averageResponseTime;
		final[2].averageResponseTime += srf[i].averageResponseTime;
		final[3].averageResponseTime += rr[i].averageResponseTime;
		final[4].averageResponseTime += hpfp[i].averageResponseTime;
		final[5].averageResponseTime += hpfnp[i].averageResponseTime;

		final[0].averageWaitTime += fcfs[i].averageWaitTime;
		final[1].averageWaitTime += sjf[i].averageWaitTime;
		final[2].averageWaitTime += srf[i].averageWaitTime;
		final[3].averageWaitTime += rr[i].averageWaitTime;
		final[4].averageWaitTime += hpfp[i].averageWaitTime;
		final[5].averageWaitTime += hpfnp[i].averageWaitTime;

		final[0].averageTurnAround += fcfs[i].averageTurnAround;
		final[1].averageTurnAround += sjf[i].averageTurnAround;
		final[2].averageTurnAround += srf[i].averageTurnAround;
		final[3].averageTurnAround += rr[i].averageTurnAround;
		final[4].averageTurnAround += hpfp[i].averageTurnAround;
		final[5].averageTurnAround += hpfnp[i].averageTurnAround;

		final[0].avg_Throughput += fcfs[i].avg_Throughput;
		final[1].avg_Throughput += sjf[i].avg_Throughput;
		final[2].avg_Throughput += srf[i].avg_Throughput;
		final[3].avg_Throughput += rr[i].avg_Throughput;
		final[4].avg_Throughput += hpfp[i].avg_Throughput;
		final[5].avg_Throughput += hpfnp[i].avg_Throughput;
	}

	// Average for all the algorithms
	for (int i = 0; i < 6; i++)
	{
		final[i].averageResponseTime /= 5;
		final[i].averageWaitTime /= 5;
		final[i].averageTurnAround /= 5;
		final[i].avg_Throughput /= 5;
	}

	// Print the averaged results for each scheduling algorithm

	printf("\n\n\n");
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("The average of the 5 runs of all algorithms is as follows:\n");
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("ALGORITHM 01: FIRST COME FIRST SERVE:\n\n");
	printf("Average Response Time(RT) : %.1f\n", final[0].averageResponseTime);
	printf("Average Wait Time(WT) : %.1f\n", final[0].averageWaitTime);
	printf("Average Turn Around Time(TAT) :%.1f\n", final[0].averageTurnAround);
	printf("Average throughput(tr) :%.1f\n", final[0].avg_Throughput);
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("ALGORITHM 02: ROUND ROBIN [PREEMPTIVE]:\n\n");
	printf("Average Response Time(RT) : %.1f\n", final[3].averageResponseTime);
	printf("Average Wait Time(WT) : %.1f\n", final[3].averageWaitTime);
	printf("Average Turn Around Time(TAT) :%.1f\n", final[3].averageTurnAround);
	printf("Average throughput(tr) :%.1f\n", final[3].avg_Throughput);
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("ALGORITHM 03: SHORTEST JOB FIRST [NON PREEMPTIVE]:\n\n");
	printf("Average Response Time(RT) : %.1f\n", final[1].averageResponseTime);
	printf("Average Wait Time(WT) : %.1f\n", final[1].averageWaitTime);
	printf("Average Turn Around Time(TAT) :%.1f\n", final[1].averageTurnAround);
	printf("Average throughput(tr) :%.1f\n", final[1].avg_Throughput);
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("ALGORITHM 04: SHORTEST REMAINING TIME FIRST [PREEMPTIVE]:\n\n");
	printf("Average Response Time(RT) : %.1f\n", final[2].averageResponseTime);
	printf("Average Wait Time(WT) : %.1f\n", final[2].averageWaitTime);
	printf("Average Turn Around Time(TAT) :%.1f\n", final[2].averageTurnAround);
	printf("Average throughput(tr) :%.1f\n", final[2].avg_Throughput);
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("ALGORITHM 05: HIGHEST PRIORITY FIRST [PREEMPTIVE]:\n\n");
	printf("Average Response Time(RT) : %.1f\n", final[4].averageResponseTime);
	printf("Average Wait Time(WT) : %.1f\n", final[4].averageWaitTime);
	printf("Average Turn Around Time(TAT) :%.1f\n", final[4].averageTurnAround);
	printf("Average throughput(tr) :%.1f\n", final[4].avg_Throughput);
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("ALGORITHM: HIGHEST PRIORITY FIRST [NON PREEMPTIVE]:\n\n");
	printf("Average Response Time(RT) : %.1f\n", final[5].averageResponseTime);
	printf("Average Wait Time(WT) : %.1f\n", final[5].averageWaitTime);
	printf("Average Turn Around Time(TAT) :%.1f\n", final[5].averageTurnAround);
	printf("Average throughput(tr) :%.1f\n", final[5].avg_Throughput);
	printf("\n");
}
