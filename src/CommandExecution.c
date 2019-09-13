#include<errno.h> //errno
#include<string.h>//strerror
#include<stdio.h>//fprintf
#include<unistd.h>//unix system calls (fork)
#include<sys/wait.h>//waitpid calls
#include<stdlib.h>//realloc

//need to return <0 if failed to execute
void exec_Command(char *inputCommand)
{
    int counter=0;
	
    char *cmd;
    char **args;
    char *reallocReturnValue;
    int size=2;
    int index=0;
    char *tempArgHolder;
	args=(char**)realloc(NULL,size * sizeof(char*));
	cmd=strtok(inputCommand," ");
	args[index++]=cmd;	
	while((tempArgHolder=strtok(NULL," "))!=NULL)
	{
		args[index++]=tempArgHolder;
		size++;
		reallocReturnValue=(char*)realloc(args,size * sizeof(char*));
		if(reallocReturnValue==NULL)
		{
			fprintf(stderr,"splitting arguments failed Actual error message:%s",strerror(errno));
			exit(0);
		}
	}
	args[index]=NULL;
	if(execvp(cmd,args)<0)
	{
		fprintf(stderr, " Command not executed. Actual errorMessage:%s\n", strerror(errno));
		exit(0);

	}
	//return 0;
	exit(0);
}

//for test purposes
//int main()
//{
//    char *inputCommand;
//    execute_Command( inputCommand);
//}



