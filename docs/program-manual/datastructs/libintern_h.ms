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
.TYPED "" "void (*tableitr)(const char *, internkey, void *" "libs/intern.h"
.USAGE
The type of an iterator over an intern table. Takes the following parameters:
.RS
.IP \[bu]
The string in the table.
.IP \[bu]
The key corresponding to the string.
.IP \[bu]
A piece of user-supplied data.
.RE
.ETYPED
.BFUNCT "void" "foreachintern" "struct interntab *, tableitr, void *" "libs/intern.h"
.USAGE
Execute an action over every K-V pair in a table.
.ARGS
.ARG "struct interntab *"
The intern table to iterate over.
.ARG "tableitr"
The iterator to execute.
.ARG "void *"
The data to pass to the iterator.
.EARGS
.EFUNCT
.DATAST "struct internlist" "libs/intern.h"
.USAGE
Incomplete type for an intern-list, which is a map that uses interned strings as
keys.
.EDATAST
.BFUNCT "struct internlist *" "makeinternlist" "int, void (*pfDestroy)(void *)" "libs/intern.h"
.USAGE
Create an intern list.
.ARGS
.ARG "int"
The initial capacity of the list.
.ARG "void (*pfDestroy)(void *)"
The function to call when an item is deleted from the list.
.EARGS
.RETURN
The new intern list.
.EFUNCT
.FUNCT "void" "killinternlist" "struct internlist *" "libs/intern.h"
.USAGE
Destroy and delete an intern list.
.ARGS
.ARG "struct internlist *"
The intern list to destroy.
.EARGS
.EFUNCT
.BFUNCT "void" "putinternlist" "struct internlist *, char *, void *" "libs/intern.h"
.USAGE
Insert an item into an intern list.
.ARGS
.ARG "struct internlist *"
The intern list to add to.
.ARG "char *"
The key to index the data by.
.ARG "void *"
The data to attach to the key.
.EARGS
.EFUNCT
.FUNCT "void *" "getinternlist" "struct internlist *, char *" "libs/intern.h"
.USAGE
Get an item from an intern list. Returns NULL if there is no item for that key.
.ARGS
.ARG "struct internlist *"
The intern list to get the item from.
.ARG "char *"
The key to lookup a value for.
.EARGS
.RETURN
The item in the list, or NULL if there is no item for that key.
.EFUNCT
.FUNCT "void" "deleteinternlist" "struct internlist *, char *" "libs/intern.h"
.USAGE
Delete an item from an intern list. Does nothing if there is no item attached to
that key.
.ARGS
.ARG "struct internlist *"
The intern list to remove the item from.
.ARG "char *"
The key to remove a value for.
.EARGS
.EFUNCT
.FUNCT "int" "containsinternlist" "struct internlist *, char *" "libs/intern.h"
.USAGE
Check if an item is in an intern list. Returns 0 if the item is not in the list,
1 if it is in the list.
.ARGS
.ARG "struct internlist *"
The list to check for membership in.
.ARG "char *"
The key to check for.
.EARGS
.RETURN
1 if the item is in the list, 0 if it isn't.
.EFUNCT
.TYPED "" "void (*internlistitr)(char *, void *, void *)" "libs/intern.h"
.USAGE
Type of an iterator for an intern list. Takes the following arguments:
.RS
.IP \[bu]
The string key of the item.
.IP \[bu]
The value of the item.
.IP \[bu]
A piece of user supplied data.
.RE
.ETYPED
.BFUNCT "void" "foreachinternlist" "struct internlist *, internlistitr, void *" "libs/intern.h"
.USAGE
Execute a function for every key-value pair in a list.
.ARGS
.ARG "struct internlist *"
The list to iterate over.
.ARG "internlistitr"
The function to execute for each key-value pair.
.ARG "void *"
A piece of data to pass to each method.
.EARGS
.EFUNCT
