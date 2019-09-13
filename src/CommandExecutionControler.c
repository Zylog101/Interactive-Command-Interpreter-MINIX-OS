#include "CommandExecutionControler.h"
#include "CommandExecution.h"
#include "Parser.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>//unix system calls (fork)
#include<sys/wait.h>//waitpid calls
#include<signal.h>



int *pidList = NULL;
int pidIndex=0;
int size=1;
int alarmSet=0;
int alarms=0;

void onAlarm()
{	
	if(alarms==0)
	{
		return;
	}
	int i=0;
	char userSection[3];
	printf("Command executed for too long!!!\nDo you want to terminate the command???????\nyes or no\n");
	scanf("%s",userSection);
	if(strcmp(userSection,"yes")==0)
	{
		for(i=0;i<pidIndex;i++)
		{
			kill(pidList[i],SIGKILL);
		}
	}
	alarms=0;
	//alarm(0);
	printf("Successfully Terminated the command\n");
	return;
	
}
void addPid(int id)
{
	int *memRetPtr=NULL;
	if(pidList==NULL)
	{
		pidList=calloc(1,sizeof(int));
	}
	else
	{
		memRetPtr=realloc(pidList,size*sizeof(int));
		if(memRetPtr==NULL)
		{
			fprintf(stderr, " Add to pid list fail. Actual errorMessage:%s\n", strerror(errno));
			return;
		}
		else
		{
			pidList=memRetPtr;
			pidList[pidIndex++]=id;
			size++;
		}
		
	}
}

//need to deallocate and shrink list but not handleled yet
void removePid(int id)
{
	for(int i=0;i<=pidIndex;i++)
	{
		if(pidList[i]==id)
		{
			pidList[i]=0;
			break;
		}
	}
}

//need to handle zombie children case
void execute_Command(char *inputCommand)
{
	if(strcmp(getenv("SetAlarm"),"yes")==0)
	{
		alarms=1;
		if(alarmSet==0)
		{
			signal(SIGALRM,onAlarm);
			alarmSet=1;
		}
		alarm(5);
	}
	struct CommandList *traversedNode;
	
	//parse the string
	if((parse(inputCommand))!=SUCCESS)
	{
		fprintf(stderr, " Parser failed. Actual errorMessage:%s\n", strerror(errno));
		return;
	}
	
	traversedNode=startNode;
	//the case of a single command
	
	while(traversedNode!=NULL)
	{
		if(traversedNode->nextNode==NULL)
		{
			pid_t pid=fork();
			if(pid==-1)
			{
				//error case
				fprintf(stderr,"oops fork didnt work!! Actual error message:%s",strerror(errno));
			}
			else if(pid!=0)
			{
				int status;
				//parent
				//waitpid( -1, &status , 0);
				addPid(pid);
				waitpid(pid,&status,0);
				if(WIFEXITED(status))
				{
					removePid(pid);
				}
				traversedNode=NULL;
				continue;
			}
			else
			{
				//child
				exec_Command(traversedNode->command);
				/*
				if(<0)
				{	printf("child");
					fprintf(stderr, " Command not executed. Actual errorMessage:%s\n", strerror(errno));
				}
				else
				{
					_exit(2);
				}
				*/

			}
			return;
		}
		else if(strcmp(traversedNode->nextNode->command,";") == 0 ) 
		{
			pid_t pid=fork();
			if(pid==-1)
			{
				//error case
				fprintf(stderr,"oops fork didnt work!! Actual error message:%s",strerror(errno));
			}
			else if(pid!=0)
			{
				int status;
				//parent
				addPid(pid);
				waitpid(pid,&status,0);
				if(WIFEXITED(status))
				{
					//removePid(pid);
				}
				//skipping the ; node to get to next command
				traversedNode=traversedNode->nextNode->nextNode;
				continue;
			}
			else
			{
				//child
				exec_Command(traversedNode->command);
				/*
				if(<0)
				{	printf("child");
					fprintf(stderr, " Command not executed. Actual errorMessage:%s\n", strerror(errno));
				}
				else
				{
					_exit(2);
				}
				*/
			}
		}
		else if(strcmp(traversedNode->nextNode->command,"&") == 0 )
		{

			pid_t pid=fork();
			if(pid==-1)
			{
				//error case
				fprintf(stderr,"oops fork didnt work!! Actual error message:%s",strerror(errno));
			}
			else if(pid!=0)
			{
				int status;
				//parent
				waitpid( -1, &status , 0);
				addPid(pid);
				//skipping the & node to get to next command
				traversedNode=traversedNode->nextNode->nextNode;
				continue;
			}
			else
			{
				pid_t pid=fork();
				if(pid!=0)
				{
					exit(0);
				}
				else
				{
					exec_Command(traversedNode->command);
				}
				
				//child
				//if((exec_Command(traversedNode->command))==-1)
				//{
				//	fprintf(stderr, " Command not executed. Actual errorMessage:%s\n", strerror(errno));
				//}
				//else
				//{
				//	_exit(2);
				//}
			}
		}	
		
		
	}
	alarms=0;
	release();
	
}