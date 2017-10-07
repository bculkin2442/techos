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
