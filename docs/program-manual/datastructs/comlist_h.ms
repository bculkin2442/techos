.DATAST "struct comlist" "comlist.h"
.USAGE
Incomplete type for a command list.
.br
A command list is exactly what its name implies, a list of commands, with the
ability to get a command by name, or to do something for every command in the
list.
.EDATAST
.FUNCT "struct comlist *" "makecomlist" "" "comlist.h"
.USAGE
Allocate and initialize a command list.
.RETURN
A new command list.
.EFUNCT
.FUNCT "void" "killcomlist" "struct comlist *" "comlist.h"
.USAGE
Destroy a command list
.ARGS
.ARG "struct comlist *"
The command list to destroy.
.EARGS
.EFUNCT
.BFUNCT "void" "addcommand" "struct comlist *, char *, char *, comfun_t" "comlist.h"
.USAGE
Create and add a command to a command list.
.ARGS
.ARG "struct comlist *"
The command list to add the command to.
.ARG "char *"
The name of the command.
.ARG "char *"
A brief description of the command.
.ARG "comfun_t"
A pointer to the function that handles the command.
.EARGS
.EFUNCT
.FUNCT "struct command *" "getcommand" "struct comlist *, char *" "comlist.h"
.USAGE
Retrieve a command from a command list. Will return the \f(BIINVALID_COMMAND\fP
if there is no command by that name in the list.
.ARGS
.ARG "struct comlist *"
The command list to look in.
.ARG "char *"
The name of the command to look for.
.EARGS
.RETURN
The command if it was found in the list, or INVALID_COMMAND if it wasn't.
.EFUNCT
.BFUNCT "void" "foreachcommand" "struct comlist *, void (*comitr)(struct command *)" "comlist.h"
.USAGE
Execute a function for every command in the list.
.ARGS
.ARG "struct comlist *"
The command list to get commands from.
.ARG "void (*comitr)(struct command *)"
The function to execute for every command.
.EARGS
.EFUNCT
.FUNCT "void" "printcommands" "struct comlist *, FILE *" "comlist.h"
.USAGE
Prints a command list.
.ARGS
.ARG "struct comlist *"
The command list to print to.
.EARG
.ARG "FILE *"
The stream to print to.
.EARG
.EARGS
.EFUNCT
