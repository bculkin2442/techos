.FUNCT "unsigned long" "hashstring" "const char *" "libs/intern.c"
.USAGE
Convert a string to a hashcode. This uses the DJB2K33 algorithm, which has been
shown to work well for a wide variety of strings, and not be terribly expensive
to compute.
.ARGS
.ARG "const char *"
The string to hash.
.EARGS
.RETURN
The hashcode for the string.
.EFUNCT
.FUNCT "unsigned long" "hashkey" "const int" "libs/intern.c"
.USAGE
Convert an integer to a hashcode.
.ARGS
.ARG "const int"
The integer to hash.
.EARGS
.RETURN
The hashcode for the integer.
.EFUNCT
.DATAST "struct bucket" "libs/intern.c"
.USAGE
A bucket in one of the two hash-tables the intern table manages.
.br
This bucket is actually a circular linked list of buckets.
.ATTRS
.ATTR "char *" "val"
The string in this bucket.
.ATTR "internkey" "key"
The intern key for this bucket.
.ATTR "struct bucket *" "next"
Pointer to the next bucket in the chain.
.ATTR "struct bucket *" "prev"
Pointer to the previous bucket in the chain.
.EATTRS
.EDATAST
.DATAST "struct interntab" "libs/intern.c"
.USAGE
The implementation of the opaque type in \fIlibs/intern.h\fP.
.br
Contains the value of the next intern key that will be assigned, and two
hashtables. One is keyed off of the interned strings, and the other is keyed off
of the keys for those strings. This is so we can get efficent lookup of both
keys from strings, as well as strings from their corresponding keys.
.ATTRS
.ATTR "nextkey"
The intern key that will be assigned to the next string interned into the table.
.EATTR
.ATTR "strings"
A hash-table keyed off of the string values, for looking up keys based off of
strings.
.EATTR
.ATTR "keys"
A hash-table keyed off of the key values, for looking up strings based off of
keys.
.EATTR
.EATTRS
.EDATAST
.FUNCT "void" "addbucket" "struct bucket *" "libs/intern.c"
.USAGE
Add a bucket to a bucket chain.
.ARGS
.ARG "struct bucket *"
The chain to add a bucket to.
.EARGS
.EFUNCT
.DATAST "struct internlist" "libs/intern.c"
.USAGE
The implementation for the opaque data type of the same name in libs/intern.h. A
simple array-based mapping from strings to void pointers.
.ATTRS
.ATTR "paData"
The actual data storage for the array.
.EATTR
.ATTR  "dataspace"
The amount of space available in the data array.
.EATTR
.ATTR "datausage"
The amount of space used in the data array.
.EATTR
.ATTR "ptKeys"
The mapping from string keys to indexes into the array.
.EATTR
.ATTR "void (*pfDestroy)(void *)"
The function to call on data items that are being deleted.
.EATTR
.EATTRS
.EDATAST
.DATAST "struct listitrdata" "libs/intern.c"
.USAGE
Storage for data used in dointernlistitr.
.ATTRS
.ATTR "plList"
The list being iterated over.
.EATTR
.ATTR "pfFunc"
The function to call for each pair.
.EATTR
.ATTR "pvData"
The data to pass to pfFunc.
.EATTR
.EATTRS
.EDATAST
.BFUNCT "void" "dointernlistitr" "const char *, internkey, void *" "libs/intern.c"
.USAGE
Internal function for using foreachintern to implement foreachinternlist.
.ARGS
.ARG "const char *"
The name of the entry in the intern list.
.ARG "internkey"
The key of the entry in the intern list.
.ARG "void *"
The listitrdata we use for data storage.
.EARGS
.EFUNCT
