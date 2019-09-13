#include "commandmemory.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

//definition of input command
char **suggestedCommand=NULL;
int sizeOfSuggestedCommand=1;
int indexOfSuggestedCommand=-1;
char *inputCommandLocal;

char *command=NULL;
    int commandSize=2;
    int commandIndex=0;

struct node
{
        char *inputCommand;
        struct node *nextNode;
};

struct node *start=NULL;
//adding at the end of the list
int add(char *inputCommand,int sizeOfInputCommand)
{
    int alreadyExists=0;

//printf("\nEntering Add Method\n");
    struct node *traversedNodePointer;
    if(start!=NULL)
    {
        //handling the case where input command already exists in list
        if(strcmp(inputCommand,start->inputCommand)==0)
        {
            traversedNodePointer=start;
            return ADD_SUCCESS;
        }
        traversedNodePointer=start;
        while(traversedNodePointer->nextNode!=NULL)
        {
            if(strcmp(inputCommand,traversedNodePointer->nextNode->inputCommand)==0)
            break;
            traversedNodePointer=traversedNodePointer->nextNode;

        }

        traversedNodePointer->nextNode = (struct node *) malloc(sizeof(struct node));
        if( traversedNodePointer->nextNode==NULL)
        {
            fprintf(stderr, " Reached out of memory handler could not store new command in command memory. Actual errorMessage:%s\n", strerror(errno));

            return AddFail;

        }
        traversedNodePointer->nextNode->nextNode=NULL;
        traversedNodePointer->nextNode->inputCommand=calloc(1,sizeOfInputCommand);
        strcpy(traversedNodePointer->nextNode->inputCommand,inputCommand);
        //traversedNodePointer->nextNode->inputCommand=inputCommand;
    }
    else
    {
  //      printf("\nEntered addmethod first node code\n");
        start=(struct node *) malloc(sizeof(struct node));
        if( start==NULL)
        {
            fprintf(stderr, " Reached out of memory handler could not store new command in command memory. Actual errorMessage:%s\n", strerror(errno));
            return AddFail;

        }
        start->nextNode=NULL;
        start->inputCommand=calloc(1,sizeOfInputCommand);
        strcpy(start->inputCommand,inputCommand);
        //start->inputCommand=inputCommand;
    //    printf("\n%s\n",start->inputCommand);
    }
return ADD_SUCCESS;
}

void destroy()
{
    struct node *traversedNodePointer;
    traversedNodePointer=start;

    while(start!=NULL)
    {

        start=traversedNodePointer->nextNode;
        free(traversedNodePointer->inputCommand);

        free(traversedNodePointer);

        traversedNodePointer=start;

    }
    //freeing memoryu of suggseted command list
    if(suggestedCommand!=NULL)
    {
        free(suggestedCommand);
    }
}

int searchCommand(char *inputCommand)
//int searchCommand()
{
    char *stringCompareReturn;
    struct node *traversedNodePointer;
    traversedNodePointer=start;
    while(traversedNodePointer!=NULL)
    {

        stringCompareReturn=strstr(traversedNodePointer->inputCommand,inputCommand);
        //printf("node value:%s\n",traversedNodePointer->inputCommand);


        if(stringCompareReturn==NULL)
        {
            continue;
        }
        else
        {
           // printf("entered searchwhile loop\n");

            //adding suggestion to suggestion list
            //TODO: need to handle no allocation case here
                if(suggestedCommand==NULL)
                {
                    suggestedCommand = (char **)realloc( NULL , sizeof(char*) );
                }
                else
                {
                    suggestedCommand = (char **)realloc( suggestedCommand , sizeOfSuggestedCommand * sizeof(char*) );
                }
                indexOfSuggestedCommand++;

                //printf("suggestedCommand:%s ,%d\n",stringCompareReturn,strlen(inputCommand));
                //suggestedCommand[indexOfSuggestedCommand]=stringCompareReturn+strlen(inputCommand);
                suggestedCommand[indexOfSuggestedCommand]=traversedNodePointer->inputCommand;
                sizeOfSuggestedCommand++;
               // printf("%s\n",suggestedCommand[indexOfSuggestedCommand]);

        }
        traversedNodePointer=traversedNodePointer->nextNode;
    }

    if(suggestedCommand!=NULL)
    {
        return SUGGESTIONS_FOUND;
    }
    return NO_SUGGESTIONS;
}

int write_to_file()
{
    struct node *traversedNode;
    FILE *fp;
    fp=fopen("CommandMemoryStore","w");
//printf("write %s",traversedNode->inputCommand);
    traversedNode=start;
	//printf("write %s",traversedNode->inputCommand);
    while(traversedNode!=NULL)
    {
        fprintf(fp,"%s\n",traversedNode->inputCommand);
        traversedNode=traversedNode->nextNode;
    }

    fclose(fp);
    return COMMANDS_STORED;
}

int load_from_file()
{
    FILE *fp;
    char tempChar;
    fp=fopen("CommandMemoryStore","r");
    char *command=NULL;
    int commandSize=2;
    int commandIndex=0;
    char *reallocReturnPointer;
    //deleting the file if it already exists
    if(fp==NULL)
    {
        return NO_COMMAND_FILE;
    }
    else
    {
	

        command=realloc(NULL,commandSize);
        while((tempChar=fgetc(fp))!=EOF)
        {
            if( tempChar == '\n' )
            {
                if(command!=NULL)
                {
                    //adding to the list ,list should take care of creating its own memory for holding the command
                    command[commandIndex]='\0';
                    add(command,commandSize);
                    commandSize=2;
                    commandIndex=0;
                    command=realloc(NULL,commandSize);
                }
            }
            else
            {
                command[commandIndex++]=tempChar;
                commandSize++;
                reallocReturnPointer=realloc(command,commandSize);
                if(reallocReturnPointer==NULL)
                {
                    fprintf(stderr, " Reached out of memory handler in command memory load_from_file. Actual errorMessage:%s\n", strerror(errno));
                    //need to free old memory as it is still valid
                    free(command);
                    return COMMAND_READ_FAIL;
                }
                command=reallocReturnPointer;
                reallocReturnPointer=NULL;
            }
        }

    }
    remove("CommandMemory");
    fclose(fp);
    return LOAD_COMMAND_MEMORY_SUCCESS;
}

/*
int main()
{
   // char *ch=realloc(NULL,7*sizeof(char));
    //scanf("%s",ch);
       // ch ="ls -la";
    //add(ch);
   // printf("start node %s\n",start->inputCommand);
   // char *ach=realloc(NULL,3*sizeof(char));
   //     ach ="ls";

    //char *cha=realloc(NULL,7*sizeof(char));
    //    cha =strcpy(cha,"ls -lr");
    //add(cha);

  // load_from_file();

    //int success=search(ach);

    if(success==2)
    printf("search found result YIPIKAYIIII!!!!!!!!!!!!\n");
    else
    printf("No search result STILL WORKSSS OHHHH MYYYY GODDDDDDDDDDD");

  char *ach=realloc(NULL,sizeof(char*));
        *ach ='k';
    add(ach);
    printf("start node %s\n",start->nextNode->inputCommand);

    //getchar();
   // destroy();
   // if (start==NULL)
    {
    printf("Memory freed");
    }
    */
