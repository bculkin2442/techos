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
