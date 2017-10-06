\# @TODO write some kinda script to auto-generate this
.CREF "main" "techos.c"
.SUBH "Calls"
initcoms(), makeosstate(), makecomlist(), addcommands(), comhan(), killosstate()
disposecoms(), 
.SUBH "Called By"
None
.ECREF
.CREF "addcommand" "comlist.c"
.SUBH "Calls"
internstring()
.SUBH "Called By"
addcommands()
.ECREF
.CREF "addcommands" "commands.c"
.SUBH "Calls"
addcommand()
.SUBH "Called By"
main()
.ECREF
.CREF "checkhelpargs" "command.c"
.SUBH "Called By"
handle_exit(), handle_version(), handle_help(), handle_date(), handle_time(),
handle_setdate()
.ECREF
.CREF "comhan" "techos.c"
.SUBH "Calls"
handleline()
.SUBH "Called By"
main()
.ECREF
.CREF "disposecoms" "commands.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
main()
.ECREF
.CREF "execcom" "commands.c"
.SUBH "Calls"
All the handle_*() functions through com.comfun
.SUBH "Called By"
handleline()
.ECREF
.CREF "findpcbname" "pcb.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
handle_rmpcb(), handle_blpcb(), handle_ubpcb(), handle_sspcb(), handle_rspcb(),
handle_shpcb()
.ECREF
.CREF "findpcbnum" "pcb.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
handle_rmpcb(), handle_blpcb(), handle_shpcb()
.ECREF
.CREF "foreachcommand" "comlist.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
Nothing
.ECREF
.CREF "foreachpcb" "pcbinternals.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
handle_shpcb()
.ECREF
.CREF "getcommand" "comlist.c"
.SUBH "Calls"
getcommand()
.SUBH "Called By"
handleline()
.ECREF
.CREF "handle_*" "*cmds.c"
.SUBH "Called By"
execcom() through com.comfun
.ECREF
.CREF "handleline" "techos.c"
.SUBH "Calls"
parseargs(), getcommand(), execcom()
.SUBH "Called By"
handle_script(), comhan()
.ECREF
.CREF "initcoms" "commands.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
main()
.ECREF
.CREF "insertpcb" "pcb.c"
.SUBH "Calls"
priorinsertpcb(), fifoinsertpcb()
.SUBH "Called By"
handle_mkpcb(), handle_blpcb(), handle_ubpcb(), handle_sspcb(), handle_rspcb()
.ECREF
.CREF "internstring" "libs/intern.c"
.SUBH "Calls"
lookupstring(), hashstring(), hashkey(), addbucket()
.SUBH "Called By"
addcommand()
.ECREF
.CREF "killcomlist" "comlist.c"
.SUBH "Calls"
killinterntab()
.SUBH "Called By"
Nothing
.ECREF
.CREF "killinterntab" "libs/intern.c"
.SUBH "Called By"
killcomlist()
.ECREF
.CREF "killosstate" "osstate.c"
.SUBH "Called By"
main()
.ECREF
.CREF "killpcb" "pcb.c"
.SUBH "Called By"
handle_rmpcb()
.ECREF
.CREF "lookupkey" "libs/intern.c"
.SUBH "Calls"
hashkey()
.SUBH "Called By"
printpcb()
.ECREF
.CREF "lookupstring" "libs/intern.c"
.SUBH "Calls"
hashstring()
.SUBH "Called By"
lookupstrings()
.ECREF
.CREF "makecomlist" "comlist.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
main()
.ECREF
.CREF "makeinterntab" "libs/intern.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
Nothing
.ECREF
.CREF "makeosstate" "osstate.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
main()
.ECREF
.CREF "makepcb" "pcb.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
handle_mkpcb()
.ECREF
.CREF "parseargs" "libs/argparser.c"
.SUBH "Calls"
Nothing
.SUBH "Called by"
handleline()
.ECREF
.CREF "printcommands" "comlist.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
printcommands()
.ECREF
.CREF "removepcb" "pcb.c"
.SUBH "Calls"
Nothing
.SUBH "Called By"
hadle_rmpcb(), handle_blpcb(), handle_ubpcb(), handle_sspcb(), handle_rspcb()
.ECREF
