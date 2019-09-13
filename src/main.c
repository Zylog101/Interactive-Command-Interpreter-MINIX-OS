/*
Convention followed
Structure name -> PascalCase
Structure members-> camelCase
local variables->camelCase
global variables->
global constants->ALL_CAPS
Functions using structures->function_using_structures(pascalCase)
*/
//for testing adding conio.h here
#include "conio.h"
#include<stdio.h>
#include<stdlib.h>
#include "UserInput.h"
#include "CommandExecutionControler.h"
#include<errno.h> //errno
#include<string.h>//strerror
#include<unistd.h>//chdir

//move it to UserInput.h header
enum InputStatus{ ENTER_PRESSED = 0,  ENTER_NOT_PRESSED = 1, USER_INPUT_ERROR = -1};
enum ShellStatus { SHELL_OK = 1 , SHELL_INIT_FAIL=2};


//remove all the headers present in headers later
//int get_user_input(char *inputCommand);
int initializeShell();
int execute_profile_file(FILE *profileFp,char *profileFileName);
void execute_Command(char *inputCommand);
int main()
{

    enum ShellStatus shellStatus;
    
    enum InputStatus inputStatus;
   

    //TODO: handle the case when intializeShell resutls in an error
    shellStatus = initializeShell();
	LoadMemory();

    while( shellStatus == SHELL_OK )
    {

	inputStatus = get_user_input();
        if( inputStatus != ENTER_PRESSED )
        {
            continue;
        }
        //TODO:parse
        
        execute_Command(inputCommand);

    }
	free_memory();

    return 0;
}

int initializeShell()
{
    FILE *ifp;
    int initStatus;
    char profileFileName[] = "PROFILE";

    initStatus = SHELL_OK;

    //open profile file

	ifp = fopen(profileFileName,"r");
	if ( ifp == NULL )
	{
		fprintf(stderr," Unable to read %s file ",profileFileName);
		//TODO: is there a way to handle this case
		initStatus = SHELL_INIT_FAIL;
	}
	else
	{
        initStatus = execute_profile_file(ifp,profileFileName);
	
        //handling the case when environmental variable set fails and case where everything is ok
        if((initStatus == SHELL_INIT_FAIL && ifp != NULL)||( initStatus == SHELL_OK ) )
        {
            fclose(ifp);
        }
	}
	return initStatus;
}

int execute_profile_file(FILE *profileFp , char *profileFileName )
{//
    enum ProfileExecState { EnvName =1, EnvVal=0, NothingEntered=-1} flag;
    int setEnvStatus;
    char ch;
    //initializing variables to read from profile file
    char *envName;
    int envNameIndex=0;
    int envNameSize=2;

    char *envVal;
    int envValSize=2;
    int envValIndex=0;
    flag=NothingEntered;

    char *reallocReturnPointer;

    envName=realloc(NULL,envNameSize);
    envVal=realloc(NULL,envNameSize);

    //checking if the profile file is empty
    while((ch=fgetc(profileFp))!=EOF)
    {
//
        if(ch==' ')
        {
	    if(flag==NothingEntered)
	    {
		continue;
	    }
	    else if( flag == EnvName )
	    {
		envName[envNameIndex]='\0';
           	flag=EnvVal;
	    }
            
        }
        else if(ch=='\n')
        {
	    if(flag==NothingEntered)
	    {
		continue;
	    }
	    //exception case handled
	    else if(flag==EnvName)
	    {
            	fprintf(stderr," Enteries in Profile file not in - KEY VALUE - format \n");
            	return SHELL_INIT_FAIL;
	    }
	    else if(flag==EnvVal)
	    {
		envVal[envValIndex]='\0';
            	flag=EnvName;
            	setEnvStatus=1;
            	//Environmental variables are set
            	setEnvStatus = setenv(envName,envVal,1);
            	//Environment variable not set
            	if(setEnvStatus == -1)
            	{
            	    fprintf(stderr," unable to set %s environment variable \n",envName);
            	    return SHELL_INIT_FAIL;
            	}
//
	    	printf("EnvVar Name : %s = %s\n",envName,envVal);
            	free(envName);
            	free(envVal);
            	envNameSize=2;
            	envNameIndex=0;
            	envName=realloc(NULL,envNameSize);
            	envValSize=2;
            	envValIndex=0;
            	envVal=realloc(NULL,envNameSize);
	    }
            
        }
        else if(flag==EnvVal) //EnvVal case
        {
            envVal[envValIndex++]=ch;
            envValSize++;
            reallocReturnPointer=realloc(envVal,envValSize);
            if(reallocReturnPointer==NULL)
                {
                    fprintf(stderr, " Reached out of memory handler. Actual errorMessage:%s\n", strerror(errno));
                    //need to free old memory as it is still valid
                    free(envVal);
                    return SHELL_INIT_FAIL;
                }
            envVal=reallocReturnPointer;
            reallocReturnPointer=NULL;

        }
	else //EnvName case
        {
	    flag=EnvName;
            envName[envNameIndex++]=ch;
            envNameSize++;
            reallocReturnPointer=realloc(envName,envNameSize);
            if(reallocReturnPointer==NULL)
                {
                    fprintf(stderr, " Reached out of memory handler. Actual errorMessage:%s\n", strerror(errno));
                    //need to free old memory as it is still valid
                    free(envName);
                    return SHELL_INIT_FAIL;
                }
            envName=reallocReturnPointer;
            reallocReturnPointer=NULL;

        }
    }
    //changing the present working directory to HOME
    if(chdir(getenv("HOME"))== -1)
    {
	fprintf(stderr, " Could not change working directory actual errorMessage:%s\n", strerror(errno));
	return SHELL_INIT_FAIL;
    }
    return SHELL_OK;
}