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
