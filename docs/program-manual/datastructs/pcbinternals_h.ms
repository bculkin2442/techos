.DATAST "enum queuetype" "pcbinternals.h"
.USAGE
Represents the various types of PCB queues that exist.
.ATTRS
.ATTR "" "QT_NORMAL"
Represents a normal, FIFO PCB queue. The corresponding struct is \f(CWstruct
pcbqueue\fP.
.EATTR
.ATTR "" "QT_PRIORITY"
Represents a priority FIFO PCB queue. The corresponding struct is \f(CWstruct
pcbqueueprior.\fP
.EATTR
.EATTRS
.EDATAST
.DATAST "struct pcbqueue" "pcbinternals.h"
.USAGE
Represents a circular, doubly-linked list of PCBs
.ATTRS
.ATTR "enum pcbtype" "type"
The type of this PCB. Indicates the actual struct type of this PCB, which may
not be struct pcbqueue. Should be QT_NORMAL if this is actually a \f(CWstruct
pcbqueue\fP.
.EATTR
.ATTR "int" "nprocs"
The number of processes in this queue.
.EATTR
.ATTR "struct pcb *" "pHead"
The head of the queue.
.EATTR
.EATTRS
.EDATAST
.DATAST "struct pcbqueueprior" "pcbinternals.h"
.USAGE
Represents a priority queue of PCBs.
.ATTRS
.ATTR "enum queuetype" "type"
The type of queue this is. Should always be QT_PRIOR.
.EATTR
.ATTR "int" "nprocs"
The number of processes in this queue.
.EATTR
.ATTR "struct pcbqueue **" "apqQueues"
The list of queues: one for each priority level.
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
