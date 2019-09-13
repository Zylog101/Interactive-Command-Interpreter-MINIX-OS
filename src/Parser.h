

struct CommandList
{
	char *command;
	struct CommandList *nextNode;
};

extern struct CommandList *startNode;

int parse(char *inputCommand);
int destroy();
enum Status{ADD_SUCCESS=1, ADD_FAIL=-1,SUCCESS=2,PARSE_FAIL=-1};
void release();