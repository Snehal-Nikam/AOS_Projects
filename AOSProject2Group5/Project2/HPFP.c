#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

// Function to create a process statistics object for the given process
processStat *create_ProcessStat(process *proc);

// function to compare priority and arival time for highest priority first preemptive //
int comparePriority(void *data1, void *data2)
{
  processStat *ps1 = (processStat *)data1;
  processStat *ps2 = (processStat *)data2;

  // Compare process priorities; if they're the same, compare their arrival times
  if (((((process *)ps1->proc)->priority)) < ((((process *)ps2->proc)->priority)))
  {
    return -1;
  }
  else if (((((process *)ps1->proc)->priority)) == ((((process *)ps2->proc)->priority)) && (((process *)ps1->proc)->arrivalTime < (((process *)ps2->proc)->arrivalTime)))
  {
    return -1;
  }
  else
  {
    return 1;
  }
}

// Implementation of highest priority first preemptive //

averageStats highestPriorityFirstP(linked_List *processes)
{
  int t = 0;  // Current time quanta
             // Create separate queues for each priority level
  queue *process_Queue_1 = createQueue();
  queue *process_Queue_2 = createQueue();
  queue *process_Queue_3 = createQueue();
  queue *process_Queue_4 = createQueue();
 // Create linked lists to manage the order of processes during preemption for each priority level// Create linked lists to manage the order of processes during preemption for each priority level
  linked_List *ll_1 = createLinkedList();
  linked_List *ll_2 = createLinkedList();
  linked_List *ll_3 = createLinkedList();
  linked_List *ll_4 = createLinkedList();

 // Pointer to traverse the list of processes

  node *process_Pointer = processes->head;

   // Check if there are any processes to schedule
  if (processes->head == NULL)
  {
    fprintf(stderr, "No Process to schedule\n");
  }
  printf("\nHighest Priority First Preemptive:\n");
  processStat *scheduled_Process = NULL;
  // Loop until 100 time quanta or all processes are scheduled
  while (t < 100 || scheduled_Process != NULL)
  // If there's a scheduled process, enqueue it based on its priority
  {
    if (scheduled_Process != NULL)
    {
      if (scheduled_Process->proc->priority == 1)
        enQueue(process_Queue_1, scheduled_Process);
      if (scheduled_Process->proc->priority == 2)
        enQueue(process_Queue_2, scheduled_Process);
      if (scheduled_Process->proc->priority == 3)
        enQueue(process_Queue_3, scheduled_Process);
      if (scheduled_Process->proc->priority == 4)
        enQueue(process_Queue_3, scheduled_Process);
    }
    // check for incoming new process and enQueue it in the queue
    if (process_Pointer != NULL)
    {
      process *newProcess = (process *)(process_Pointer->data);
      while (process_Pointer != NULL && newProcess->arrivalTime <= t)
      {
        if (newProcess->priority == 1)
          enQueue(process_Queue_1, create_ProcessStat(newProcess));
        if (newProcess->priority == 2)
          enQueue(process_Queue_2, create_ProcessStat(newProcess));
        if (newProcess->priority == 3)
          enQueue(process_Queue_3, create_ProcessStat(newProcess));
        if (newProcess->priority == 4)
          enQueue(process_Queue_3, create_ProcessStat(newProcess));
        sort(process_Queue_1, comparePriority);
        sort(process_Queue_2, comparePriority);
        sort(process_Queue_3, comparePriority);
        sort(process_Queue_4, comparePriority);
        process_Pointer = process_Pointer->next;
        if (process_Pointer != NULL)
        {
          newProcess = (process *)(process_Pointer->data);
        }
      }
    }
    // if there is no scheduled process, then check process queue and schedule it //
    if (scheduled_Process == NULL)
    {
      if (process_Queue_1->size > 0)
        scheduled_Process = (processStat *)deQueue(process_Queue_1);
      else if (process_Queue_2->size > 0)
        scheduled_Process = (processStat *)deQueue(process_Queue_2);
      else if (process_Queue_3->size > 0)
        scheduled_Process = (processStat *)deQueue(process_Queue_3);
      else if (process_Queue_4->size > 0)
        scheduled_Process = (processStat *)deQueue(process_Queue_4);
      // If the process has not started before quanta 100, remove the process from the queue and take the next process in queue for execution //
      if (t >= 100 && scheduled_Process->start_Time == -1)
      {
        // free(scheduled_Process);
        scheduled_Process = NULL;
        continue;
      }
    }
    if (scheduled_Process != NULL)
    {
      process *proc = scheduled_Process->proc;

      // add current running process to the time chart
      printf("%c", proc->pid);

      // update current processes stat
      if (scheduled_Process->start_Time == -1)
      {
        scheduled_Process->start_Time = t;
      }

      scheduled_Process->runTime++;

      // printf("Process name %c", proc->pid);

      if (scheduled_Process->runTime >= proc->runTime)
      {
        scheduled_Process->end_Time = t;
        if (scheduled_Process->proc->priority == 1)
          addNode(ll_1, scheduled_Process);
        else if (scheduled_Process->proc->priority == 2)
          addNode(ll_2, scheduled_Process);
        else if (scheduled_Process->proc->priority == 3)
          addNode(ll_3, scheduled_Process);
        else if (scheduled_Process->proc->priority == 4)
          addNode(ll_4, scheduled_Process);
        // addNode(ll,scheduled_Process);
        scheduled_Process = NULL;
        // free(scheduled_Process);
      }
    }
    else
    {
      printf("_");
    }
    // increase the time
    t++;
  }
  // Print Process Stat
  averageStats avg1, avg2, avg3, avg4, avg;
  printf("\nFor Priority Queue 1");
  avg1 = printPolicy_Stat(ll_1);
  printf("\nFor Priority Queue 2");
  avg2 = printPolicy_Stat(ll_2);
  printf("\nFor Priority Queue 3");
  avg3 = printPolicy_Stat(ll_3);
  printf("\nFor Priority Queue 4");
  avg4 = printPolicy_Stat(ll_4);

  avg.averageResponseTime = (avg1.averageResponseTime + avg2.averageResponseTime + avg3.averageResponseTime + avg4.averageResponseTime) / 4;
  avg.averageWaitTime = (avg1.averageWaitTime + avg2.averageWaitTime + avg3.averageWaitTime + avg4.averageWaitTime) / 4;
  avg.averageTurnAround = (avg1.averageTurnAround + avg2.averageTurnAround + avg3.averageTurnAround + avg4.averageTurnAround) / 4;
  avg.avg_Throughput = (avg1.avg_Throughput + avg2.avg_Throughput + avg3.avg_Throughput + avg4.avg_Throughput);

  printf("\nThe average times of High Priority First Preemptive for all queues:\n");
  printf("Average Response Time(RT) : %.1f\n", avg.averageResponseTime);
  printf("Average Wait Time(WT) : %.1f\n", avg.averageWaitTime);
  printf("Average Turn Around Time(TAT) :%.1f\n", avg.averageTurnAround);

// Calculate and print average statistics for each priority queue and overall
    //... (Your logic for calculating and printing average stats)

  return avg;
}
