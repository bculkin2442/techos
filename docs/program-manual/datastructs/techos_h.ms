.FUNCT "void" "comhan" "struct osstate *" "techos.h"
.USAGE
The main command handler.
.PP
Loops reading commands and handling them until either an EOF is reached, an exit
command is executed, or a command fails in a fatal manner.
.ARGS
.ARG "struct osstate *"
The state of the OS during command execution.
.EARG
.EARGS
.EFUNCT
.FUNCT "int" "handleline" "struct osstate *, char *" "techos.h"
.USAGE
Executes a command from a line of input.
.ARGS
.ARG "struct osstate *"
The state of the OS for the line.
.EARG
.ARG "char *"
The line to execute.
.EARG
.EARGS
.RETURN
The return status of the command. This is zero for success, positive for
non-fatal failure, and negative for fatal failure.
.EFUNCT
.BFUNCT "int" "execcom" "struct command *, struct cliargs, char *, struct osstate *" "techos.h"
.USAGE
Execute a command, given its arguments.
.ARGS
.ARG "struct command *"
The command to execute.
.EARG
.ARG "struct cliargs"
The arguments to the command.
.EARG
.ARG "char *"
The command-line as the user entered it.
.EARG
.ARG "struct osstate *"
The state of the OS for the command.
.EARG
.EARGS
.RETURN
The return status of the command, the meaning of which is documented above.
.EFUNCT
