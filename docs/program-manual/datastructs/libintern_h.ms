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
