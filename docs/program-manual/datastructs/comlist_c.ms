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
