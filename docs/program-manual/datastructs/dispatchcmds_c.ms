.FUNCT "int" "dodispatch" "struct osstate *, int" "dispatchcmds.c"
.USAGE
Do the actual work of dispatching processes.
.ARGS
.ARG "struct osstate *"
The state of the OS.
.ARG "int"
The level of verbosity, or the amount of output to generate. Should be 0, 1 or
2, with higher numbers giving more output.
.EARGS
.RETURN
The return status of the command
.EFUNCT
