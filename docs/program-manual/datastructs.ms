.DATAST "struct cliargs" "libs/argparser.h"
.USAGE
Contains the command-line arguments parsed from an input line.
.ATTRS
.ATTR "int" "argc"
The number of command-line arguments in place.
.ATTR "char **" "argv"
The array of the command-line arguments. Is an array of \fIargc\fP strings.
.EATTR
.EDATAST
.FUNCT "struct cliargs" "parseargs" "char *" "libs/argparser.h"
.USAGE
Parse a set of command-line arguments from the given string. The main thing that
this function does that using \fIstrtok\fP doesn't, is that this will properly
group together things that are inside single/double quoted strings as a single
argument.
.ARGS
.ARG "char *"
The line to parse command-line arguments from. This string is mutated.
.EARGS
.RETURN
The parsed set of command-line arguments.
.EFUNCT
.TYPED "int" "internkey" "libs/intern.h"
.USAGE
The type of a key into an intern table.
Anything less than or equal to zero is a invalid key.
.ETYPED
.DATAST "struct interntab" "libs/intern.h"
.USAGE
Opaque state-type for a intern table.
.br
An intern table is a container for a set of interned strings. The primary
purpose for this is to map a string to a small integer key, for easy indexing of
data structures and string equality checking.
.EDATAST
.FUNCT "struct interntab *" "makeinterntab" "" "libs/intern.h"
.USAGE
Create a new intern table
.RETURN
The new intern table
.EFUNCT
.FUNCT "void" "killinterntab" "struct interntab *" "libs/intern.h"
.USAGE
Destroy an intern table.
.ARGS
.ARG "struct interntab *"
The intern table to destroy
.EARGS
.EFUNCT
.FUNCT "internkey" "internstring" "struct interntab *, const char *" "libs/intern.h"
.USAGE
Enter a string (intern it) into a intern table.
.ARGS
.ARG "struct interntab *"
The intern table to enter the string into.
.ARG "const char *"
The string to intern.
.EARGS
.RETURN
The key corresponding to the string. This may or may not be a new key, depending
on whether the string was interned already or not.
.EFUNCT
.FUNCT "internkey" "lookupstring" "struct interntab *, const char *" "libs/intern.h"
.USAGE
Look up the key for a string in the intern table.
.ARGS
.ARG "struct interntab"
The intern table to enter the string into.
.ARG "const char *"
The string to look up the intern key for.
.EARGS
.RETURN
The intern key corresponding to the strings, or the special key SIINVALID if the
provided string hasn't been interned.
.EFUNCT
.FUNCT "const char *" "lookupkey" "struct interntab *, internkey" "libs/intern.h"
.USAGE
Look up the string for a key in the intern table.
.ARGS
.ARG "struct interntab"
The intern table to enter the string into.
.ARG "internkey"
The intern key to look up the string for.
.EARGS
.RETURN
The string that has the given intern key, or the NULL pointer if no string has
been interned to that key.
.EFUNCT
.FUNCT "unsigned long" "hashstring" "const char *" "libs/intern.c"
.USAGE
Convert a string to a hashcode. This uses the DJB2K33 algorithm, which has been
shown to work well for a wide variety of strings, and not be terribly expensive
to compute.
.ARGS
.ARG "const char *"
The string to hash.
.EARGS
.RETURN
The hashcode for the string.
.EFUNCT
.FUNCT "unsigned long" "hashkey" "const int" "libs/intern.c"
.USAGE
Convert an integer to a hashcode.
.ARGS
.ARG "const int"
The integer to hash.
.EARGS
.RETURN
The hashcode for the integer.
.EFUNCT
.DATAST "struct bucket" "libs/intern.c"
.USAGE
A bucket in one of the two hash-tables the intern table manages.
.br
This bucket is actually a circular linked list of buckets.
.ATTRS
.ATTR "char *" "val"
The string in this bucket.
.ATTR "internkey" "key"
The intern key for this bucket.
.ATTR "struct bucket *" "next"
Pointer to the next bucket in the chain.
.ATTR "struct bucket *" "prev"
Pointer to the previous bucket in the chain.
.EATTRS
.EDATAST
.DATAST "struct interntab" "libs/intern.c"
.USAGE
The implementation of the opaque type in \fIlibs/intern.h\fP.
.br
Contains the value of the next intern key that will be assigned, and two
hashtables. One is keyed off of the interned strings, and the other is keyed off
of the keys for those strings. This is so we can get efficent lookup of both
keys from strings, as well as strings from their corresponding keys.
.ATTRS
.ATTR "nextkey"
The intern key that will be assigned to the next string interned into the table.
.ATTR "strings"
A hash-table keyed off of the string values, for looking up keys based off of
strings.
.ATTR "keys"
A hash-table keyed off of the key values, for looking up strings based off of
keys.
.EATTRS
.EDATAST
.FUNCT "void" "addbucket" "struct bucket *" "libs/intern.c"
.USAGE
Add a bucket to a bucket chain.
.ARGS
.ARG "struct bucket *"
The chain to add a bucket to.
.EARGS
.EFUNCT
.FUNCT "char *" "parsestr" "char *, char, char **" "libs/argparser.c"
.USAGE
Parse a string from a input line.
.ARGS
.ARG "char *"
The initial part of the string, up until the first space.
.ARG "char"
The character that should end this string.
.ARG "char **"
The string being parsed for arguments.
.EARGS
.EFUNCT
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
\# @TODO add a 'BTYPED' to fix this line
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
.FUNCT "void" "initcoms" "" "commands.h"
.USAGE
Initialize global state for commands.
.EFUNCT
.FUNCT "void" "addcommands" "struct comlist *" "commands.h"
.USAGE
Add all of the default commands to a command list.
.ARGS
.ARG "struct comlist *"
The list to add the commands to.
.EARGS
.EFUNCT
.FUNCT "void" "disposecoms" "" "commands.h"
.USAGE
Cleanup global state for commands.
.EFUNCT
.COMDECL "version" "commands.h"
.COMDECL "exit" "commands.h"
.COMDECL "help" "commands.h"
.COMDECL "date" "datecmds.h"
.COMDECL "datefmt" "datecmds.h"
.COMDECL "setdate" "datecmds.h"
.COMDECL "time" "datecmds.h"
.DATAST "struct osstate" "osstate.h"
.USAGE
Contains general state for things the OS requires.
.ATTRS
.ATTR "char *" "in_datefmt"
The current input format for dates.
.EATTR
.ATTR "char *" "out_datefmt"
The current output format for dates.
.EATTR
.ATTR "char *" "time_datefmt"
The current output format for times.
.EATTR
.ATTR "struct tm *" "datetime"
The current date/time.
.EATTR
.ATTR "FILE *" "strem"
The source for reading input from.
.EATTR
.ATTR "FILE *" "output"
The source for writing output to.
.EATTR
.ATTR "struct pcbstate *" "pPCBstat"
The state needed for PCBs.
.EATTR
.EATTRS
.EDATAST
.FUNCT "struct osstate *" "makeosstate" "" "osstate.h" "osstate.h"
.USAGE
Allocate and initialize a OS state struct.
.EFUNCT
.FUNCT "void" "killosstate" "struct osstate *" "osstate.h"
.USAGE
Deinitialize and deallocate an OS state struct.
.EFUNCT
.DATAST "enum pcbclass" "pcb.h"
.USAGE
Represents the classification of a PCB.
.ATTRS
.ATTR "" "PCB_SYSTEM"
This PCB represents a system process.
.ATTR "" "PCB_APPLICATION"
This PCB represents a user process.
.EATTRS
.EDATAST
.DATAST "enum pcbstatus" "pcb.h"
.USAGE
The run status of a PCB.
.ATTRS
.ATTR "" "PCB_BLOCKED"
The PCB is blocked, waiting for something.
.ATTR "" "PCB_READY"
The PCB is ready to run.
.ATTR "" "PCB_RUNNING"
The PCB is currently running.
.EATTRS
.EDATAST
.DATAST "enum pcbsusp" "pcb.h"
.USAGE
The suspension status of a PCB.
.ATTRS
.ATTR "" "PCB_SUSPENDED"
This PCB is currently suspended.
.ATTR "" "PCB_FREE"
This PCB is currently not suspended.
.EATTRS
.EDATAST
.DATAST "struct pcb" "pcb.h"
Represents a running process.
.ATTRS
.ATTR "int" "id"
The unique numeric ID for the process.
.EATTR
.ATTR "internkey" "kName"
The name for the process. Stored as a interned string, for fast equality
checking and space savings.
.EATTR
.ATTR "enum pcbclass" "class"
The classification of this process (System/Application).
.EATTR
.ATTR "int" "priority"
The priority of this process, on a scale from 0 to 9, with higher numbers being
higher priority.
.EATTR
.ATTR "enum pcbstatus" "status"
Indicates whether the PCB is running, ready to run, or blocked.
.EATTR
.ATTR "enum pcbsusp" "susp"
Indicates whether the PCB is suspended or not.
.EATTR
.ATTR "struct pcb *" "pNext"
The next process in the queue this PCB is in.
.EATTR
.ATTR "struct pcb *" "pPrev"
The previous process in the queue this PCB is in.
.EATTR
.EATTRS
.EDATAST
.BFUNCT "struct pcb *" "makepcb" "struct pcbstate *, char *, enum pcbclass, int" "pcb.h"
.USAGE
Allocate/initialize a new PCB.
.ARGS
.ARG "struct pcbstate *"
The current state for all PCBs.
.EARG
.ARG "char *"
The name of the PCB.
.EARG
.ARG "enum pcbclass"
The classification of the PCB.
.EARG
.ARG "int"
The priority of the PCB.
.EARG
.EARGS
.RETURN
An allocated and initialized PCB.
.EFUNCT
.FUNCT "void" "killpcb" "struct pcb *" "pcb.h"
.USAGE
Deinitialize and deallocate a PCB.
.ARGS
.ARG "struct pcb *"
The PCB to deallocate.
.EARG
.EARGS
.EFUNCT
.FUNCT "struct pcb *" "findpcbnum" "struct pcbstate *, int" "pcb.h"
.USAGE
Find a PCB by its numeric ID in a set of queues.
.ARGS
.ARG "struct pcbstate *"
The set of queues to search in.
.EARG
.ARG "int"
The process number to look for.
.EARG
.EARGS
.RETURN
The PCB with the corresponding number, or NULL if no process with that number
exists.
.EFUNCT
.FUNCT "struct pcb *" "findpcbname" "struct pcbstate *, char *" "pcb.h"
.USAGE
Find a PCB by its name in a set of queues.
.IP "NOTE:"
Since process names are not guaranteed to be unique, the queues are searched in
the following order, with the first PCB with a matching name being returned.
.RS
.nr pcbnamenum 0 1
.IP \n+[pcbnamenum].
Ready
.IP \n+[pcbnamenum].
Blocked
.IP \n+[pcbnamenum].
Suspended Ready
.IP \n+[pcbnamenum].
Suspended Blocked
.RE
.ARGS
.ARG "struct pcbstate *"
The set of queues to look in.
.EARG
.ARG "char *"
The process name to look for.
.EARG
.EARGS
.RETURN
The first process with the given name following the above search order, or NULL
if no process by that name exists.
.EFUNCT
.DATAST "enum pcberror" "pcb.h"
.USAGE
The errors possible for \f(BIinsertpcb()\fP
.ATTRS
.ATTR "" "PCBSUCCESS"
No error occured.
.EATTR
.ATTR "" "PCBINVSUSP"
The provided PCB has an invalid suspension status
.EATTR
.ATTR "" "PCBINVSTAT"
The provided PCB has an invalid run status
.EATTR
.ATTR "" "PCBRUNNING"
The provided PCB is currently running.
.EATTR
.ATTR "" "PCBINQUEUE"
The provided PCB is already in a queue.
.EATTR
.EATTRS
.EDATAST
.FUNCT "enum pcberror" "insertpcb" "struct pcbstate *, struct pcb *" "pcb.h"
.USAGE
Insert a PCB into the proper queue, based off of its status.
.ARGS
.ARG "struct pcbstate *"
The set of queues to insert the PCB into.
.EARG
.ARG "struct pcb *"
The PCB to insert into a queue.
.EARG
.EARGS
.RETURN
The status of the insertion operation.
.EFUNCT
.FUNCT "void" "removepcb" "struct pcbstate *, struct pcb *" "pcb.h"
.USAGE
Remove a PCB from the queue it is currently in.
.IP "NOTE:"
Attempting to remove a PCB that is not in a queue from a queue may cause various
degrees of weird things to happen. Don't do it.
.ARGS
.ARG "struct pcbstate *"
The set of queues to remove a PCB from.
.EARG
.ARG "struct pcb *"
The PCB to remove from the queues.
.EARG
.EARGS
.EFUNCT
.COMDECL "script" "scriptcmds.h"
.COMDECL "scriptctl" "scriptcmds.h"
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
.COMDECL "mkpcb" "pcbcmds.h"
.COMDECL "rmpcb" "pcbcmds.h"
.COMDECL "blpcb" "pcbcmds.h"
.COMDECL "ubpcb" "pcbcmds.h"
.COMDECL "sspcb" "pcbcmds.h"
.COMDECL "rspcb" "pcbcmds.h"
.COMDECL "sppcb" "pcbcmds.h"
.COMDECL "shpcb" "pcbcmds.h"
.DATAST "struct pcbqueue" "pcbinternals.h"
.USAGE
Represents a circular, doubly-linked list of PCBs
.ATTRS
.ATTR "int" "nprocs"
The number of processes in this queue.
.EATTR
.ATTR "struct pcb *" "pHead"
The head of the queue.
.EATTR
.EATTRS
.EDATAST
.DATAST "struct pcbstate" "pcbinternals.h"
.USAGE
Contains all of the state needed for PCBs
.ATTRS
.ATTR "struct interntab *" "ptPCBNames"
The table where process names are stored.
.EATTR
.ATTR "int" "nProcid"
The next process ID to be assigned.
.EATTR
.ATTR "struct pcbqueue *" "pqReady"
The queue of ready PCBs.
.EATTR
.ATTR "struct pcbqueue *" "pqBlocked"
The queue of blocked PCBs.
.EATTR
.ATTR "struct pcbqueue *" "pqsReady"
The queue of suspended ready PCBs.
.EATTR
.ATTR "struct pcbqueue *" "pqsBlocked"
The queue of suspended blocked PCBs.
.EATTR
.EATTRS
.EDATAST
.BFUNCT "void" "foreachpcb" "struct pcbqueue *, void (*pcbitr)(struct pcb *, void *), void *" "pcbinternals.h"
.USAGE
Execute a function once for each PCB in a queue.
.ARGS
.ARG "struct pcbqueue *"
The queue of PCBs to process.
.EARG
.ARG "void (*pcbitr)(struct pcb *, void *)"
The function to execute for each PCB. Takes an extra state parameter.
.EARG
.ARG "void *"
The state parameter to pass to the function.
.EARG
.EARGS
.EFUNCT
.DATAST "struct comlist" "comlist.c"
.USAGE
A list of commands, stored in an array indexed by an interned string (the
command name).
.ATTRS
.ATTR "struct command **" "commands"
All of the commands in this list, stored in a dynamically allocated array.
.EATTR
.ATTR "int" "comspace"
The available number of slots in the command array.
.EATTR
.ATTR "int" "comcount"
The total number of slots in the command array.
.EATTR
.ATTR "struct interntab *" "interncoms"
The intern table that turns command names into indexes into the command array.
.EATTR
.EATTRS
.EDATAST
.FUNCT "struct pcbqueue *" "makepcbqueue" "" "osstate.c"
.USAGE
Allocate/initialize a PCB queue.
.RETURN
An allocated and initialized PCB queue.
.EFUNCT
.FUNCT "struct pcbstate *" "makepcbstate" "" "osstate.c"
.USAGE
Allocate and initialize PCB state.
.RETURN
An allocated and initialized set of PCB state.
.EFUNCT
.FUNCT "struct pcb *" "queuefindpcbnum" "struct pcbqueue *, int" "pcb.c"
.USAGE
Find something by process number in a PCB queue.
.ARGS
.ARG "struct pcbqueue *"
The queue to look in.
.EARG
.ARG "int"
The process number to look for.
.EARG
.EARGS
.RETURN
The PCB with that process number, or null if no such PCB exists.
.EFUNCT
.FUNCT "struct pcb *" "queuefindpcbname" "struct pcbqueue *, int" "pcb.c"
.USAGE
Find something by process name in a PCB queue.
.IP "NOTE:"
Since process names aren't guaranteed to be unique, will give the first PCB with
that name.
.ARGS
.ARG "struct pcbqueue *"
The queue to look in.
.EARG
.ARG "int"
The string key for the process name to look for.
.EARG
.EARGS
.RETURN
The first PCB with that process name, or null if no such PCB exists.
.EFUNCT
.FUNCT "int" "fillqueue" "struct pcbqueue *, struct pcb *" "pcb.c"
.USAGE
Fill a PCB queue if it is empty.
.ARGS
.ARG "struct pcbqueue *"
The queue to attempt to fill.
.EARG
.ARG "struct pcb *"
The PCB to attempt to fill with.
.EARG
.EARGS
.RETURN
1 if the queue was succesfully filled, 0 if it already had something in it.
.EFUNCT
.FUNCT "void" "fifoinsertpcb" "struct pcbqueue *" "struct pcb *" "pcb.c"
.USAGE
Insert a PCB into a FIFO queue.
.ARGS
.ARG "struct pcbqueue *"
The queue to insert into.
.EARG
.ARG "struct pcb *"
The PCB to insert into the queue.
.EARG
.EARGS
.EFUNCT
.FUNCT "void" "filoinsertpcb" "struct pcbqueue *" "struct pcb *" "pcb.c"
.USAGE
Insert a PCB into a FILO queue.
.ARGS
.ARG "struct pcbqueue *"
The queue to insert into.
.EARG
.ARG "struct pcb *"
The PCB to insert into the queue.
.EARG
.EARGS
.EFUNCT
.FUNCT "void" "priorinsertpcb" "struct pcbqueue *" "struct pcb *" "pcb.c"
.USAGE
Insert a PCB into a priority queue.
.ARGS
.ARG "struct pcbqueue *"
The queue to insert into.
.EARG
.ARG "struct pcb *"
The PCB to insert into the queue.
.EARG
.EARGS
.EFUNCT
.FUNCT "void" "printpcb" "struct pcb *, void *" "pcbcmds.c"
.USAGE
Print a PCB.
.ARGS
.ARG "struct pcb *"
The PCB to print.
.EARG
.ARG "void *"
The state parameter. Must be valid to cast to a ``struct osstate *''
.EARG
.EARGS
.EFUNCT
.FUNCT "int" "main" "" "techos.c"
.USAGE
Entrance point for TechOS. Setups state, prints a greeting, invokes the command
handler, then cleanups up the result and exits.
.RETURN
The status code for the program
.EFUNCT
