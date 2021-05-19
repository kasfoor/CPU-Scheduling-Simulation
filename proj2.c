#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>

void FCFS();
void SRTF();
void RR(int);

int pid[20];
int arrivalTime[20];
int burstTime[20];
int processes = 0;

int queue[20];
int count = 0;

void enqueue(int x)
{
	queue[count] = x;
	count++;
}

int dequeue()
{
	int result = queue[0];
	int i;
	for (i = 0; i < count - 1; i++)
	{
		queue[i] = queue[i + 1];
	}
	count--;
	return result;
}

void printQueue()
{
	int i;
	for (i = 0; i < processes; i++)
	{
		printf("%d ", queue[i]);
	}
}

int main(int argc, char *argv[])
{

	FILE *f = fopen(argv[1], "r");
	char buffer[50];
	while (fgets(buffer, sizeof(buffer), f) != NULL)
	{
		processes++;
	}
	rewind(f);

	int i;
	for (i = 0; i < processes; i++)
	{
		fscanf(f, "%d %d %d", &pid[i], &arrivalTime[i], &burstTime[i]);
	}
	fclose(f);

	if (strcmp(argv[2], "FCFS") == 0)
	{

		printf("Scheduling algorithm: FCFS\nTotal %d tasks are read from %s. press "
			   "enter"
			   " to start...\n",
			   processes, argv[1]);
		printf("==============================================================\n");
		FCFS();
	}
	else if (strcmp(argv[2], "SRTF") == 0)
	{

		printf("Scheduling algorithm: SRTF\nTotal %d tasks are read from %s. press "
			   "enter"
			   " to start...\n",
			   processes, argv[1]);
		printf("==============================================================\n");
		SRTF();
	}
	else if (strcmp(argv[2], "RR") == 0)
	{

		printf("Scheduling algorithm: RR\nTotal %d tasks are read from %s. press "
			   "enter"
			   " to start...\n",
			   processes, argv[1]);
		printf("==============================================================\n");
		RR(atoi(argv[3]));
	}
}

void FCFS()
{

	int time = 0;
	float waitTime = 0;
	float turnAroundTime = 0;
	float responseTime = 0;

	int id[20];
	float at[20];
	float bt[20];
	float wt[20];
	float tt[20];
	float rt[20];

	wt[0] = 0;
	rt[0] = 0;

	int i, j;

	for (i = 0; i < processes; i++)
	{
		id[i] = pid[i];
		at[i] = arrivalTime[i];
		bt[i] = burstTime[i];
	}

	for (i = 0; i < processes; i++)
	{
		j = bt[i];
		while (j > 0)
		{
			printf("<system time %d>process\t %d is running\n", time, id[i]);
			time++;
			j--;
		}
		wt[i] = time - at[i] - bt[i];
		rt[i] = time - at[i] - bt[i];
		tt[i] = time - at[i];
		printf("<system time %d>process\t %d is finished.........\n", time, id[i]);
	}
	printf("<system time %d> All processes finish...............\n", time);

	for (i = 0; i < processes; i++)
	{
		waitTime += wt[i];
		responseTime += rt[i];
		turnAroundTime += tt[i];
	}

	waitTime /= processes;
	responseTime /= processes;
	turnAroundTime /= processes;

	printf("===============================================================\n");
	printf("Average cpu usage\t : 100%%\n");
	printf("Average waiting time\t : %.2f\n", waitTime);
	printf("Average response time\t : %.2f\n", responseTime);
	printf("Average turnaround time\t : %.2f\n", turnAroundTime);
	printf("===============================================================\n");
}

void SRTF()
{
	int time = 0;
	float waitTime = 0;
	float turnAroundTime = 0;
	float responseTime = 0;

	int id[20];
	float at[20];
	float bt[20];
	float wt[20];
	float tt[20];
	float rt[20];

	int seen[20];

	int i;

	for (i = 0; i < processes; i++)
	{
		id[i] = pid[i];
		at[i] = arrivalTime[i];
		bt[i] = burstTime[i];
		seen[i] = 0;
	}

	int completed = 0;
	int minimumBurst = bt[0];
	int smallestProcess = 0;
	int earliestArtivalTime = at[0];
	int completionTime;
	bool flag = false;

	while (completed != processes)
	{
		for (i = 0; i < processes; i++)
		{
			if (at[i] <= time && bt[i] <= minimumBurst && bt[i] > 0)
			{
				smallestProcess = i;
				flag = true;
				minimumBurst = bt[i];
				earliestArtivalTime = at[i];
			}
		}

		if (flag == false)
		{
			time++;
		}

		if (seen[smallestProcess] == 0)
		{
			rt[smallestProcess] = time;
			seen[smallestProcess] = 1;
		}

		printf("<system time %d>process\t %d is running\n", time, id[smallestProcess]);
		bt[smallestProcess]--;
		time++;
		minimumBurst = bt[smallestProcess];

		if (bt[smallestProcess] == 0)
		{
			completed++;
			printf("<system time %d>process\t %d is finished.........\n", time, id[smallestProcess]);
			flag = false;
			wt[smallestProcess] = time - burstTime[smallestProcess] - at[smallestProcess];
			tt[smallestProcess] = wt[smallestProcess] + burstTime[smallestProcess];
			minimumBurst = INT_MAX;
		}
	}

	for (i = 0; i < processes; i++)
	{
		waitTime += wt[i];
		responseTime += rt[i] - at[i];
		turnAroundTime += tt[i];
	}

	waitTime /= processes;
	turnAroundTime /= processes;
	responseTime = responseTime / processes;

	printf("<system time %d> All processes finish...............\n", time);

	printf("===============================================================\n");
	printf("Average cpu usage\t : 100%%\n");
	printf("Average waiting time\t : %.2f\n", waitTime);
	printf("Average response time\t : %.2f\n", responseTime);
	printf("Average turnaround time\t : %.2f\n", turnAroundTime);
	printf("===============================================================\n");
}

void RR(int quantum)
{

	int time = 0;
	float waitTime = 0;
	float turnAroundTime = 0;
	float responseTime = 0;

	int id[20];
	float at[20];
	float bt[20];
	float wt[20];
	float tt[20];
	float rt[20];

	int i, j;
	int count = 0;
	int waitingQueue[20];
	int seen[20];

	for (i = 0; i < processes; i++)
	{
		id[i] = pid[i];
		at[i] = arrivalTime[i];
		bt[i] = burstTime[i];
		waitingQueue[i] = 0;
		seen[i] = 0;
	}

	int completed = 0;

	for (i = 0; i < processes; i++)
	{
		if (at[i] <= time && waitingQueue[i] == 0)
		{
			enqueue(i);
			waitingQueue[i] = 1;
			count++;
		}
	}

	while (completed != processes)
	{

		int currProcess = dequeue();

		for (j = 0; j < quantum && bt[currProcess] != 0; j++)
		{
			if (seen[currProcess] == 0)
			{
				rt[currProcess] = time - at[currProcess];
				seen[currProcess] = 1;
			}

			printf("<system time %d>process\t %d is running\n", time, id[currProcess]);
			bt[currProcess]--;
			time++;
		}

		for (i = 0; i < processes; i++)
		{
			if (at[i] <= time && waitingQueue[i] == 0)
			{
				enqueue(i);
				waitingQueue[i] = 1;
				count++;
			}
		}

		if (bt[currProcess] != 0)
		{
			enqueue(currProcess);
		}

		else if (bt[currProcess] == 0 || bt[currProcess] < 0)
		{
			printf("<system time %d>process\t %d is finished.........\n", time, id[currProcess]);
			tt[currProcess] = time;
			completed++;
		}
	}

	for (i = 0; i < processes; i++)
	{
		turnAroundTime += tt[i] - at[i];
		waitTime += tt[i] - at[i] - burstTime[i];
		responseTime += rt[i];
	}

	turnAroundTime /= processes;
	waitTime /= processes;
	responseTime /= processes;

	printf("<system time %d> All processes finish...............\n", time);
	printf("===============================================================\n");
	printf("Average cpu usage\t : 100%%\n");
	printf("Average waiting time\t : %.2f\n", waitTime);
	printf("Average response time\t : %.2f\n", responseTime);
	printf("Average turnaround time\t : %.2f\n", turnAroundTime);
	printf("===============================================================\n");
}
