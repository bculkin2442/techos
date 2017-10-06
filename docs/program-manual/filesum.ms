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
.FILESUM "pcb.h"
Header file for declarations of PCBs (Process Control Blocks).
.FILESUM "pcb.c"
Implementation of PCBs.
.FILESUM "pcbcmds.h"
Header file for declaration of PCB commands.
.FILESUM "pcbcmds.c"
Implementation of PCB commands: mkpcb, rmpcb, blpcb, ubpcb, sspcb, rspcb, sppcb,
shpcb
.FILESUM "pcbinternals.h"
Header file for implementation details on how PCBs and PCB queues work.
.FILESUM "pcbinternals.c"
Implementation for any PCB internals.
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
.FILESUM "test.tsh"
A test file for the script command.
.FILESUM "help/blpcb.1"
Manual for the blpcb command.
.FILESUM "help/date.1"
Manual for the date command
.FILESUM "help/datefmt.1"
Manual for the datefmt command
.FILESUM "help/exit.1"
Manual for the exit command
.FILESUM "help/help.1"
Manual for the help command
.FILESUM "help/mkpcb.1"
Manual for the mkpcb command.
.FILESUM "help/rmpcb.1"
Manual for the rmpcb command.
.FILESUM "help/rspcb.1"
Manual for the rspcb command.
.FILESUM "help/script.1"
Manual for the script command.
.FILESUM "help/setdate.1"
Manual for the setdate command
.FILESUM "help/sppcb.1"
Manual for the sppcb command.
.FILESUM "help/sspcb.1"
Manual for the sspcb command.
.FILESUM "help/time.1"
Manual for the time command.
.FILESUM "help/ubpcb.1"
Manual for the ubpcb command.
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
