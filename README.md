# Interactive-Command-Interpreter-MINIX-OS

Implementation of a configurable shell having subset of ash shell functionalities with enhancements. Enhancements make the shell more user friendly by including features which reduce redundency. The extension is in addition to the regular shell behavior of program execution found in the location given by PATH variable.

**Highlight:** command prediction, sequential/parallel command execution, non-terminating command alert system

**Functionalities -**
* Invoked from the Ash shell provided with Minix.

* Shell first executes a PROFILE file which defines the PATH variable that will allow the access of programs provided in /bin and /usr/bin. Once the PROFILE file is executed, you will be in a HOME directory specified in the PROFILE file. The PATH and HOME variables do not replace those of the Ash shell from which the shell is instantiated.

* In a command line of the shell any executable programs including the utilities provided in /bin and /usr/bin can be executed

* Shell will set an alarm which fires 5 seconds after it has launched a command. After the alarm fired, shell will ask the user whether he/she wants to terminate the command and will terminate the command if the user approves. The user can turn on and off this feature in the PROFILE file or in a command.

* Shell will remember the commands that a user had entered. In the future, when the user wants to enter a command again, she needs to type only the first few characters and then the Tab key, shell will fill in the rest of the command line. If she does not like that suggestion, she can use the up and down arrow keys to look for other suggestions from the shell. If she types any other key, shell will stop suggesting until the next command. If she modifies the suggested command, shell will remember that too. The memory of past commands survives after the shell exits.

* Shell supports a command line with parentheses and the sequence and parallel execution operators (“;” and “&”). A command line is a command or a set of commands connected by the operators. A pair of parentheses encloses a command line which can be treated as a commands. Therefore cmd&(cmd1;cmd2) is a valid command line specifying that cmd will be executed in parallel with (cmd1;cmd2).

**Design**

![Design](https://github.com/Zylog101/Interactive-Command-Interpreter-MINIX-OS/blob/master/Image/aaa.jpg)
