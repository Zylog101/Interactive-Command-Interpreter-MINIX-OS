
/*

Input : character pointer
Funtion: reads user input
Returns: user input status and data (character pointer)

*/


#include"UserInput.h"
#include<stdlib.h> //for realloc
#include<stdio.h> //getchar and printf
#include<errno.h> //errno
#include<string.h>//strerror
#include "conio.h"
#include "commandmemory.h"

//move it to UserInput.h header
enum InputStatus{ ENTER_PRESSED = 0,  ENTER_NOT_PRESSED = 1, USER_INPUT_ERROR =-1};
char *inputCommand=NULL;
int collect_input_user_data();
void LoadMemory()
{
	int load_from_file();
}

int get_user_input()
{
    int userEnteredSomething=1;
    char enteredChar;
    char *reallocReturnPointer;
    enum InputStatus inputStatus;
    int sizeOfInputCommand;
    int commandInputIndex;
    int tabSet=0;
    int searchIndex=indexOfSuggestedCommand;
    int backTrackCounter=0;
    int suggested=0;

	load_from_file();
    printf("[#]");
    inputStatus=ENTER_NOT_PRESSED;
    //allocating initial memory for the command char string
    sizeOfInputCommand=2;
    commandInputIndex=0;
    //case where the inputCommand is already used and needs to be freed
    if(inputCommand!=NULL)
    {
        free(inputCommand);
        inputCommand=NULL;
    }
    inputCommand=calloc(2,1);
    while (inputStatus==ENTER_NOT_PRESSED)
    {
        enteredChar=getch();
        //new code start
        //handling the case of arrow press
        if( enteredChar == '\033' )
        {

            getch();
            enteredChar=getch();
            if(tabSet != 1)
            {
                continue;
            }
            if(enteredChar=='A')
            {
                //reached a place where you cant go any further up or this is the only suggestion
                if(searchIndex==0)
                {
                    continue;
                }
                else
                {
                    //printf("\n%d\n",backTrackCounter);
                    //deleting all the entered values
                    while(backTrackCounter!=0)
                    {
                        printf("\b");
                        printf(" ");
                        printf("\b");
                        backTrackCounter--;
                    }
                    --searchIndex;
                    printf("%s",suggestedCommand[searchIndex]);
                    suggested=1;
                    backTrackCounter=strlen(suggestedCommand[searchIndex]);
                    commandInputIndex=backTrackCounter;
                    sizeOfInputCommand=backTrackCounter+1;
                    continue;
                }
            }
            else if(enteredChar=='B')
            {
                //reached a place where you cant go any further down suggestion list or there is only on suggestion
                if(searchIndex==indexOfSuggestedCommand)
                {
                    continue;
                }
                else
                {
                    //deleting all the entered values
                    while(backTrackCounter!=0)
                    {
                        printf("\b");
                        printf(" ");
                        printf("\b");
                        backTrackCounter--;
                    }
                    printf("%s",suggestedCommand[++searchIndex]);
                    suggested=1;
                    backTrackCounter=strlen(suggestedCommand[searchIndex]);
                    commandInputIndex=backTrackCounter;
                    sizeOfInputCommand=backTrackCounter+1;
                    continue;
                    //debugging purposses
                    //printf("%s",suggestedCommand[searchIndex]);
                }
            }
        }

        if(  enteredChar == '\t'  )
        {
            //handling the case when multiple tabs are pressed
            if(tabSet == 1)
            {
                continue;
            }
            if(searchCommand(inputCommand)==SUGGESTIONS_FOUND)
            {

                backTrackCounter=strlen(inputCommand);
                //deleting all the entered values
                while(backTrackCounter!=0)
                {
                    printf("\b");
                    printf("_");
                    printf("\b");
                    backTrackCounter--;
                }
                tabSet=1;
                //suggested
                printf("%s",suggestedCommand[indexOfSuggestedCommand]);
                suggested=1;
                searchIndex=indexOfSuggestedCommand;
                //this is required as in UP handling code this is used to backtrack and print new suggestion
                //this is nothing but the size of new input command now
                backTrackCounter=strlen(suggestedCommand[indexOfSuggestedCommand]);
                commandInputIndex=backTrackCounter;
                sizeOfInputCommand=backTrackCounter+1;
               // printf("\n%d\n",backTrackCounter);
                continue;
            }
            else
            {
                tabSet=0;
                continue;
            }

        }
//new code end

        if (enteredChar == '\n')
        {
            //tabSet=0;
            inputStatus= ENTER_PRESSED;
            //case where nothing is entered
            if(userEnteredSomething!=2)
            {
               userEnteredSomething=0;
            }
            //something was entered
            else
            {

                //new code
                //if(suggested)
                if(tabSet)
                {
                    free(inputCommand);
                    inputCommand=NULL;
                    //int size=strlen(suggestedCommand[searchIndex]);
                    reallocReturnPointer=realloc(inputCommand, sizeOfInputCommand);
                    //handling exception cases
                    //Case:out of memory to allocate
                    if (reallocReturnPointer==NULL)
                    {
                        fprintf(stderr, " Reached out of memory handler. Actual errorMessage:%s\n", strerror(errno));
                        //need to free old memory as it is user has already input char's
                        //free(inputCommand);
                        inputStatus= USER_INPUT_ERROR;
                    }
                    else
                    {
                        inputCommand=reallocReturnPointer;
                       // sizeOfInputCommand+=size;

                        //TODO:is this really required?
                        reallocReturnPointer=NULL;
                        //adding suggestion to the command string
                        inputCommand=strcpy(inputCommand,suggestedCommand[searchIndex]);
                        free(suggestedCommand);
                        suggestedCommand=NULL;
                        tabSet=0;
                        commandInputIndex++;
                    }
                }
            //suggested=0;
        //new code end
                inputCommand[commandInputIndex]='\0';
                add(inputCommand,sizeOfInputCommand);
		write_to_file();
            }
        }
        // TODO: handle other conditions such as "up" "down" "tab" keys pressed here
        else
        {
            //Something was suggested
            if(tabSet)
            {
                free(inputCommand);
                inputCommand=NULL;
                reallocReturnPointer=realloc(inputCommand, sizeOfInputCommand);

                //handling exception cases
                //Case:out of memory to allocate
                if (reallocReturnPointer==NULL)
                {
                    fprintf(stderr, " Reached out of memory handler. Actual errorMessage:%s\n", strerror(errno));
                    inputStatus= USER_INPUT_ERROR;
                }
                else
                {
                    inputCommand=reallocReturnPointer;

                    //TODO:is this really required?
                    reallocReturnPointer=NULL;
                    inputCommand=strcpy(inputCommand,suggestedCommand[searchIndex]);
                    free(suggestedCommand);
                    suggestedCommand=NULL;
                    tabSet=0;
                    sizeOfInputCommand++;

                }

            }
        //new code end
            tabSet=0;
            userEnteredSomething=2;
            inputCommand[commandInputIndex]=enteredChar;
		
            printf("%c",enteredChar);
            sizeOfInputCommand++;
            commandInputIndex++;
            //allocating additional memory for the inputCommand char string
            reallocReturnPointer=realloc(inputCommand, sizeOfInputCommand);


            //handling exception cases
            //Case:out of memory to allocate
            if (reallocReturnPointer==NULL)
            {
                fprintf(stderr, " Reached out of memory handler. Actual errorMessage:%s\n", strerror(errno));
                //need to free old memory as it is still valid
                free(inputCommand);
                inputStatus= USER_INPUT_ERROR;
            }
            else
            {
                inputCommand=reallocReturnPointer;

                //TODO:is this really required?
                reallocReturnPointer=NULL;
                inputStatus = ENTER_NOT_PRESSED;
            }
        }

        //based on the status program loop control is decided here
        if(inputStatus == ENTER_NOT_PRESSED)
        {
            continue;
        }
        if(inputStatus == ENTER_PRESSED)
        {
            printf("\n");
            break;
        }
        else if(inputStatus == USER_INPUT_ERROR )
        {
            break;
        }
    }
return inputStatus;
}


void free_memory()
{
    write_to_file();
    destroy();
}
 /*
//Test code
int main()
{
   char *inputCommand;
    enum InputStatus inputStatus;
   // load_from_file();
    inputStatus = get_user_input();
    switch(inputStatus)
    {
        case ENTER_PRESSED:
                            printf("\n\n ENTER_PRESSED \n\n");
                            break;
        case USER_INPUT_ERROR:
                            printf("\n\n USER INPUT ERROR \n\n");
                            break;
        default:
            printf("\n\n %d \n\n", inputStatus);
    }
   // write_to_file();
  
while (1)
{
    get_user_input();
}
    exit(1);
}
 */
