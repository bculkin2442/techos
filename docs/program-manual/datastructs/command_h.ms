\# @TODO add a 'BTYPED' macro to fix this line
.TYPED "" "int (*comfun_t)(int , char **, char *, struct osstate *)" "command.h"
.USAGE
Represents the type of a pointer to a command handler.
.ARGS
.ARG "int"
The number of args the command takes.
.ARG "char **"
An array of command line arguments.
.ARG "char *"
The entire command line, as the user entered it.
.ARG "struct osstate *"
The current OS state.
.EARGS
.RETURN
The status of the command. Should be:
.RS
.IP \[bu]
Zero if the command succeded.
.IP \[bu]
Positive if the command failed in a non-fatal manner.
.IP \[bu]
Negative if the command failed in a fatal manner.
.RE
.ETYPED
.DATAST "struct command" "command.h"
.USAGE
The core data structure that represents an executable command.
.ATTRS
.ATTR "char *" "name"
The name of the command.
.EATTR
.ATTR "char *" "brief"
A brief description of the command.
.EATTR
.ATTR "comfun_t" "comfun"
The handler that executes the command.
.EATTR
.EATTRS
.EDATAST
.DATAST "DECLCOM(name)" "command.h"
.USAGE
Declare the prototype for a command handler.
.br
Declares a \fIcomfun_t\fP that is given the name of the command, prefixed with
handle_.
.EDATAST
.DATAST "HANDLECOM(name)" "command.h"
.USAGE
Declare the implementation for a command handler.
.EDATAST
.FUNCT "int" "checkhelpargs" "int, char **, char *, struct osstate *" "command.h"
.USAGE
Check the arguments of a command that only takes the '-h/--help' argument to see
if it got it as an argument.
.ARGS
.ARG "int"
The number of command line arguments.
.ARG "char **"
An array of the command line arguments.
.ARG "char *"
The command line, just as the user input it.
.ARG "struct osstate *"
The state of the OS.
.EARGS
.RETURN
Always returns zero.
.EFUNCT
.FUNCT "struct command *" "makecommand" "char *, char *, comfun_t" "command.h"
.USAGE
Create a new normal (CT_NORMAL) command.
.ARGS
.ARG "char *"
The name of the command,
.ARG "char *"
A brief description of the command.
.ARG "comfun_t"
The handler function for the command.
.EARGS
.RETURN
A new normal command
.EFUNCT
.FUNCT "void" "killcommand" "struct command *" "command.h"
Destroy a normal (CT_NORMAL) command.
.ARGS
.ARG "struct command *"
The command to destroy.
.EARGS
.EFUNCT
