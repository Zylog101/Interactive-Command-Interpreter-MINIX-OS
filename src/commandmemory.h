extern char **suggestedCommand;
extern int indexOfSuggestedCommand;
extern char *inputCommandLocal;

enum CommandMemStatus{  AddFail=1,ADD_SUCCESS=5, SUGGESTIONS_FOUND=2, COMMANDS_STORED=3, COMMAND_READ_FAIL=-1,LOAD_COMMAND_MEMORY_SUCCESS=4,NO_SUGGESTIONS=-2, NO_COMMAND_FILE=-3};

//adds a command to memory
int add(char *inputCommand,int sizeOfInputCommand);
//int add();

//returns success of search result and updates the command completion result in a shared string
int searchCommand(char *inputCommand);

//free all memory taken up
void destroy();

//returns success of store
int write_to_file();
//load the list of commands into memory
int load_from_file();
