\##############################################################################
\# MACROS AND OTHER GENERAL THINGS
\##############################################################################
\# Typeset in 12pt font
.nr PS 12p
\# Set an author from WVU Tech
.de WVUAU
.AU
\\$1
.AI
West Virginia University Institute of Technology
..
\# Print a file summary
.de FILESUM
.IP "\fB\\$1\fP"
.IX "File Summary: \\$1"
.XS
.I "\t\\$1"
.XE
..
\# Begin an internal data structure description
.de IDATAST
.KS
.IP "\fB\\$1\fP"
.rj 1
\# This line is typeset on the same line as the structure name
.I "\r(File: \\$2)"
.RS
..
\# End a internal data structure description
.de EIDATAST
.KE
.RE
.sp 1n
..
\# Begin a data structure description
.de DATAST
.IDATAST "\\$1" "\\$2"
.IX "Start Data Structure: \\$1"
.XS
\t\fI(\\$2)\fP\tData Structure: \\$1
.XE
..
\# Begin the attributes of a data structure
.de ATTRS
.SUBH "Attributes"
.RS
..
\# Print an attribute
.de ATTR
.BULT "\f(BI\\$1\fB \\$2\fP:"
..
\# End the attributes of a data structure
.de EATTRS
.RE
..
\# End a data structure description
.de EDATAST
.EIDATAST
.IX "End Data Structure"
..
\# Print usage header for a data structure
.de USAGE
.SUBH "Usage"
..
\# Function item
.de FUNCT
.IDATAST "\f(BI\\$1\fB \\$2(\f(BI\\$3)\fP" "\\$4"
.IX "Start Function: \\$2"
.XS
\t\fI(\\$4)\fP\tFunction: \\$2
.XE
..
\# A function whose file entry needs to go on a new line
.de BFUNCT
.KS
.IP "\f(BI\\$1\fB \\$2(\f(BI\\$3)\fP"
.rj 1
\# This line is typeset on the same line as the structure name
.I "(File: \\$4)"
.IX "Start Function: \\$2"
.XS
\t\fI(\\$4)\fP\tFunction: \\$2
.XE
.RS
..
\# Start function arguments
.de ARGS
.SUBH "Arguments"
.RS
..
\# Print function argument
.de ARG
.IBULT "\\$1"
..
\# End function arguments.
.de EARGS
.RE
..
\# Print return header
.de RETURN
.SUBH "Return"
..
\# End function
.de EFUNCT
.IX "End Function"
.EIDATAST
..
\# Start typedef
.de TYPED
.IDATAST "typedef \f(BI\\$1\fB \\$2\fP" "\\$3"
.IX "Typedef: \\$2"
.XS
\t\fI(\\$3)\fP\tTypedef: \\$2
.XE
..
\# End typedef
.de ETYPED
.EIDATAST
..
\# Print a command declaration
.de COMDECL
.DATAST "DECLCOM(\\$1)" "\\$2"
.USAGE
Implement the \fB\\$1\fP command.
.EDATAST
..
\# Begin a section in the table of contents
.de XSH
.SH
\\$1
.XS
.B "\\$1"
.XE
.IX "Start Section: \\$1"
.RS
..
\# End a section
.de ESH
.IX "End Section"
.RE
..
\# Bulleted list item
.de BULT
.IP "\[bu] \\$1"
..
\# Italic bullet list item
.de IBULT
.BULT "\fI\\$1\fP"
..
\# Hanging subheader
.de SUBH
.IP "\fB\\$1\fP:"
..
\# Print page number on first page
.P1
\# Don't print date on title page.
.ND ""
\##############################################################################
\# PAPER CONTENTS START HERE
\##############################################################################
\# Print a report title page
.RP no
\&
.sp 2in
.TL
TechOS Programmers Manual
.sp 2in
.WVUAU "Ben Culkin"
.WVUAU "Jared Miller"
.WVUAU "Lucas Darnell"
\# No abstract needed
.AB no
.AE
.XSH "Overview"
.PP
Our TechOS is a command-line OS that is being made so that we can get a good
perspective on how Operating Systems are made/operate. The current state is very
simple, with this being only the first module, and having only a few commands
implemented. The project is written in C, and we are using Git to keep track of
files.
.PP
For this first module, it involves only the command handler, and the basic
looping structure for reading commands. The commands that can currently be used
are: date, datefmt, exit, help, setdate, script, time and version.
.ESH
.XSH "File Summaries"
.FILESUM "comlist.h"
Header file for declaration of command lists, and the operations upon them.
.FILESUM "comlist.c"
Implementation of command lists, which is where available commands are stored.
.FILESUM "command.h"
Header file for declaration of the command structure.
.FILESUM "commands.c"
Implementation of the command structure.
.FILESUM "datecmds.h"
Header file for declaration of date commands.
.FILESUM "datecmds.c"
Implementation of the date commands: date, datefmt, setdate and time
.FILESUM "osstate.h"
Header file for declaration of OS state, which is where all the things we want
to keep track of get put to cut down on globals.
.FILESUM "osstate.c"
Implementation of OS state.
.FILESUM "scriptcmds.h"
Header file for declaration of script commands.
.FILESUM "scriptcmds.c"
Implementation of the script commands: script
.FILESUM "techos.h"
Main header file for the OS. Contains prototypes for the interface, as well as
necessary global data.
.FILESUM "techos.c"
Implemenation of the interface for the OS. Is responsible for getting commands
from the user, and then executing them.
.FILESUM "help/date.1"
Manual for the date command
.FILESUM "help/datefmt.1"
Manual for the datefmt command
.FILESUM "help/exit.1"
Manual for the exit command
.FILESUM "help/help.1"
Manual for the help command
.FILESUM "help/setdate.1"
Manual for the setdate command
.FILESUM "help/time.1"
Manual for the time command.
.FILESUM "help/version.1"
Manual for the version command.
.FILESUM "libs/argparser.h"
Header file for the command-line argument parser.
.FILESUM "libs/argparser.c"
Implementation of the command-line argument parser.
.FILESUM "libs/intern.h"
Header file for a string-interning system.
.FILESUM "libs/intern.c"
Implemenation of the string-interning system.
.ESH
.XSH "Data Structures"
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
.TYPED "" "int (*comfun_t)(int , char **, char *, struct osstate *)\0" "command.h"
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
.ESH
\# Print the table of contents. This'll get moved to a different position after
\# the pdf has been created
.bp
.TC
