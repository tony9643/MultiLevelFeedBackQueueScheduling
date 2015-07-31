/* Program to implement MLF Scheduling
   No: of Jobs,Queue Level are user inputs
   Program has not implemented round robin scheduling within the lowest queue level 
   Implemened using Codelite in Windows*/		
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define nullptr NULL

typedef struct jobdetails
{
	int ArrivalTime;
	int ServiceTime;
	int JobIndex;
}Job;

typedef struct QueueElement
{
	Job *JobQueued;
	struct QueueElement *next;
}QElement;

typedef struct queuedetails
{
	int TimeSlice;
	QElement *front, *rear;
}Queue;

int main(int argc, char **argv)
{	
	int NoOfJobs,i,QLevels,TimeScheduled,CurrentTime,JobsFinished;
	QElement *temp;
	printf("Enter no: of jobs to be scheduled: ");
	scanf("%d",&NoOfJobs);
	
	Job *job = (Job*)malloc(sizeof(Job)*NoOfJobs);
	for(i=0;i<NoOfJobs;i++)
	{
		printf("Enter Arrival Time Of Job %d :",i);
		scanf("%d",&job[i].ArrivalTime);
		printf("Enter Service Time Of Job %d :",i);
		scanf("%d",&job[i].ServiceTime);
		job[i].JobIndex = -1; // To indicate a job has been scheduled or not
	}
	
	printf("Enter No: of ready queues in MLF: ");
	scanf("%d",&QLevels);
	
	Queue *queue = (Queue*) malloc(sizeof(Queue)*QLevels);
	for(i=0;i<QLevels;i++)
	{
		printf("Enter time slice alloted in Q%d: ",i);
		scanf("%d",&queue[i].TimeSlice);
		queue[i].front=nullptr;
		queue[i].rear=nullptr;
	}
	
	printf("Job details entered are:\nJob\tArrival Time\tService Time\n");
	for(i=0;i<NoOfJobs;i++)
	{
		printf("%d  \t%d\t\t%d\n",(i+1),job[i].ArrivalTime,job[i].ServiceTime);
	}
	printf("Given Queue time slices are:\n");
	for(i=0;i<QLevels;i++)
		printf("Q%d:%d\n",i,queue[i].TimeSlice);
	
	CurrentTime = 0;
	JobsFinished = 0;
	
	printf("\nMLF Scheduling\n=================\nTime     Job scheduled     Queue Level\n");
	while(JobsFinished < NoOfJobs)
	{
		//Check whether any job has arrived and push it to Q0
		for(i=0;i<NoOfJobs;i++)
		{	
			if(job[i].ArrivalTime<=CurrentTime && job[i].JobIndex < 0)
			{	
				job[i].JobIndex = i; // for printing job order
				temp = (QElement*)malloc(sizeof(QElement));
				temp->JobQueued = &job[i];    // insert at rear and delete at front
				temp->next = nullptr;
				if(queue[0].front==nullptr)   //first element in queue
				{
					queue[0].rear = temp;
					queue[0].front = temp;
				}
				else
				{	
					queue[0].rear->next = temp;
					queue[0].rear = temp;
				}
			}
		}
		
		// Execution
		for(i=0;i<QLevels;i++)
		{
			if(queue[i].front != nullptr)
			{
				if(queue[i].TimeSlice < queue[i].front->JobQueued->ServiceTime)
					TimeScheduled = queue[i].TimeSlice;
				else TimeScheduled = queue[i].front->JobQueued->ServiceTime;
				
				printf("%d->%d\t\tJ%d\t\tQ%d\n",CurrentTime,(CurrentTime+TimeScheduled),queue[i].front->JobQueued->JobIndex,i);
				queue[i].front->JobQueued->ServiceTime-=TimeScheduled;
				CurrentTime+=TimeScheduled;
				if(queue[i].front->JobQueued->ServiceTime <= 0)  // job finished execution
				{
					// delete QElement from queue
					temp = queue[i].front->next;
					free(queue[i].front);
					if(temp == nullptr)
						queue[i].rear = nullptr;
					queue[i].front = temp;
					JobsFinished++;    // when front = rear, next ie, new front points to nullptr; won't execute
					break;
				}
				else if(i<(QLevels-1))  // push to lower queue after checking it is not lowest queue
				{
					// Remove from Qi
					temp = queue[i].front;
					queue[i].front=queue[i].front->next;
					temp->next = nullptr;
					if(queue[i].front == nullptr)
						queue[i].rear = nullptr;
					//Add to Qi+1	
					if(queue[i+1].rear==nullptr)  //first job in lower level
					{
						queue[i+1].rear = temp;
						queue[i+1].front = temp;
					}
					else
					{	
						queue[i+1].rear->next = temp;
						queue[i+1].rear = temp;
					}
					break;
				}
	
			}
			else if(i == QLevels-1)  
				CurrentTime+=1;
		}
	}
	free(job);	
	free(queue);
	getch();
	return 0;
}
