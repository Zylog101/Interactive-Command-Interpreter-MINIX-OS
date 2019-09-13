#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include "Parser.h"

//enum Status{ADD_SUCCESS=1, ADD_FAIL=-1,SUCCESS=2}

struct CommandList *startNode=NULL;

struct CommandList * createNode();

void release()
{
	struct CommandList *traversedNodePointer;
	traversedNodePointer=startNode;
	while(traversedNodePointer!=NULL)
	{
		free(traversedNodePointer->command);
		free(traversedNodePointer);
		traversedNodePointer=traversedNodePointer->nextNode;
	}
	startNode=NULL;
}

int addNode(char *ch,int sizeOfInputCommand)
{

	struct CommandList *traversedNodePointer;
	if(startNode==NULL)
	{
       		if((startNode=createNode(startNode))==NULL)
		{
			fprintf(stderr, " Couldnt Add Node. Actual errorMessage:%s\n", strerror(errno));
			return ADD_FAIL;
		}
		else
		{
			startNode->nextNode=NULL;
			startNode->command=calloc(1,sizeOfInputCommand+1);
			strcpy(startNode->command,ch);
			startNode->command[sizeOfInputCommand]='\0';

		}
	}
	else
	{
		traversedNodePointer=startNode;
        while(traversedNodePointer->nextNode!=NULL)
        {
            traversedNodePointer=traversedNodePointer->nextNode;
        }
		if((traversedNodePointer->nextNode=createNode())==NULL)
		{
			fprintf(stderr, " Couldnt Add Node. Actual errorMessage:%s\n", strerror(errno));
			return ADD_FAIL;
		}
		traversedNodePointer->nextNode->nextNode=NULL;
        	traversedNodePointer->nextNode->command=calloc(1,sizeOfInputCommand+1);
       		strcpy(traversedNodePointer->nextNode->command,ch);
		traversedNodePointer->nextNode->command[sizeOfInputCommand]='\0';
        
	}
	return ADD_SUCCESS;
}

struct CommandList * createNode()
{
	
	struct CommandList *tempNode=(struct CommandList *) calloc(1,sizeof(struct CommandList));

        if( tempNode==NULL)
        {
            fprintf(stderr, " Reached out of memory handler could not Parse in Parser. Actual errorMessage:%s\n", strerror(errno));
            return NULL;

        }

	return tempNode;
}

int parse(char *inputCommand)
{
	struct CommandList *traversedNode;
	char ch;
	int inputCommandIndex=0;
	int commandLength=strlen(inputCommand);
	inputCommand[commandLength+1]='\0';
	ch=inputCommand[inputCommandIndex];

	while(ch!='\0')
	{
		
		if(ch == ';'|| ch == '&' )
		{
			if(addNode(&ch,1)==ADD_SUCCESS)
			{printf("");
				inputCommandIndex++;
				ch=inputCommand[inputCommandIndex];
				continue;
			}
			else
			{
				return ADD_FAIL;
			}
		}
		else
		{
			int size=2;
			int index=0;
			int cmdStartIndex=inputCommandIndex;
			int cmdEndIndex=inputCommandIndex;
			char *cmd;
			cmd=calloc(1,size);
			while((ch!='\0')&&(ch!=';')&&(ch!='&')&&inputCommandIndex<=commandLength)
			{
				cmd[index++]=ch;
				cmdEndIndex++;
				inputCommandIndex++;
				ch=inputCommand[cmdEndIndex];
				

			}
			
			if(addNode(cmd,strlen(cmd))==ADD_SUCCESS)
			{
				continue;
			}
			else
			{
				return ADD_FAIL;
			}
			
		}
		
	}
	return SUCCESS;
}
