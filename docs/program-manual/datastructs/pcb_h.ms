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
.ATTR "internkey" "kImage"
The key for the image of this process.
.EATTR
.ATTR "int" "offset"
The current execution offset of the process.
.EATTR
.ATTR "struct pcb *" "pNext"
The next process in the queue this PCB is in.
.EATTR
.ATTR "struct pcb *" "pPrev"
The previous process in the queue this PCB is in.
.EATTR
.EATTRS
.EDATAST
.DATAST
.USAGE
Forward declaration for queues of PCBs.
.EDATAST
.USAGE
Forward declaration for the container of all of the PCB states.
.EDATAST
.BFUNCT "struct pcb *" "makepcb" "struct pcbstate *, char *, char *, enum pcbclass, int" "pcb.h"
.USAGE
Allocate/initialize a new PCB.
.ARGS
.ARG "struct pcbstate *"
The current state for all PCBs.
.EARG
.ARG "char *"
The name of the PCB.
.EARG
.ARG "char *"
The path to the process image for the PCB.
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
.FUNCT "int" "candispatch" "struct pcbstate *" "pcb.h"
.USAGE
Check the number of currently dispatchable summons.
.ARGS
.ARG "struct pcbstate *"
The state to look for PCBs in.
.EARGS
.RETURN
The number of processes that can be dispatched.
.EFUNCT
.FUNCT "struct pcb *" "getdisppcb" "struct pcbstate *" "pcb.h"
.USAGE
Get a process to dispatch, which is the process with the highest priority that
is not suspended.
.ARGS
.ARG "struct pcbstate *"
The state to look for PCBs in.
.EARGS
.RETURN
The dispatchable process, or NULL if there are no dispatchable processes.
.EFUNCT
