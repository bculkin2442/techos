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
.DATAST "enum commandtype" "command.h"
.USAGE
Represents the different types of command that exist.
.ATTRS
.ATTR "" "CT_NORMAL"
A normal command, who is represented by a struct command.
.EATTRS
.EDATAST
.DATAST "struct command" "command.h"
.USAGE
The core data structure that represents an executable command.
.ATTRS
.ATTR "enum commandtype" "type"
The type of the command. For this struct, should be CT_NORMAL.
.ATTR "char *" "name"
The name of the command.
.ATTR "char *" "brief"
A brief description of the command.
.ATTR "comfun_t" "comfun"
The handler that executes the command.
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
