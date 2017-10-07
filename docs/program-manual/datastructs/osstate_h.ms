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
