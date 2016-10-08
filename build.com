$!
$!  BOSS build command file:   example of how to build a boss executable
$!      from the source.  Note that in this example boss is built with
$!      no error checking on.  This greatly increases the efficiency of
$!      the program, but should only be used with de-bugged versions.
$!
$!
$ Setup:
$       on warning then continue
$       on error then goto error_trap
$!
$ Build_paths:
$       cur_path        := 'f$directory()'
$       path_dist       := 'cur_path'
$       cur_len          = 'f$length(cur_path)' - 1
$	cur_path	:= 'f$extract(0,cur_len,cur_path)'
$!       cur_path        := 
$! write sys$output cur_path
$       path_source     := 'cur_path]'
$       path_include    := 'cur_path'.inc]'
$       path_macro      := 'cur_path'.mar]'
$       path_data       := 'cur_path'.dat]'
$       path_execute    := 'cur_path]'
$
$ define_logicals:
$       define/nolog boss_main           'path_dist'
$       define/nolog boss_source         'path_source'
$       define/nolog boss_include        'path_include'
$       define/nolog boss_macro          'path_macro'
$       define/nolog boss_data           'path_data'
$       define/nolog boss_execute        'path_execute'
$!
$ START:
$       if p1.eqs."?"           then goto HELP
$	if p1.eqs."LIBRARY"	then goto HELP_LIBRARY
$	if p1.eqs."LOCK"	then goto LOCK
$	if p1.eqs."UNLOCK"	then goto LOCK
$       if p1.eqs."LINK"        then goto LINK_STEP
$       if p1.eqs."TERMDEF"     then goto COMPILE_TERMDEF
$       if p1.eqs."MACROS"      then goto COMPILE_MACROS
$       if p1.eqs."BOSS"        then goto COMPILE_BOSS
$       if p1.eqs.""            then goto COMPILE_BOSS
$!
$ BAD_PARAM:
$       write sys$output "Unrecognized parameter : ",p1
$       exit
$!
$ HELP:
$       type sys$input
BUILD.COM :     Accepts one optional parameter.  By default, all steps are
                executed.  If parameter is used, only certain steps are
                executed.
 
        Parameters:     P1
                        ?       - display this help
                                - Compile all source, re-link boss
                        LINK    - re-link boss
                        TERMDEF - compile termdef, re-link boss
                        MACROS  - compile all macro routines, re-link boss
                        BOSS    - compile boss & termdef, re-link boss
 
$       exit
$!
$ COMPILE_BOSS:
$       set def boss_source
$       if (f$search ("boss.lis;*") .NES. "") then $delete boss.lis;*
$       if (f$search ("boss.obj;*") .NES. "") then $delete boss.obj;*
$       if (f$search ("boss.env;*") .NES. "") then $delete boss.env;*
$       write sys$output "Compiling BOSS.PAS."
$       pascal boss.pas /nocheck/nodebug
$!      pascal boss.pas /debug /check /nooptimize
$!      pascal boss.pas /nocheck/nodebug/list
$!      pascal boss.pas /nocheck/nodebug/list/show=none
$       write sys$output "BOSS.PAS compiled."
$       if (f$search("boss.exe") .NES. "") then -
	 set protection=(owner:rwed) boss_execute:boss.exe
$!
$ COMPILE_TERMDEF:
$       set def boss_source
$       if (f$search ("termdef.lis;*") .NES. "") then $delete termdef.lis;*
$       if (f$search ("termdef.obj;*") .NES. "") then $delete termdef.obj;*
$!       write sys$output "Compiling TERMDEF.PAS."
$!       pascal termdef /nocheck/nodebug/nolist
$!       write sys$output "TERMDEF.PAS compiled."
$!       if p1.nes."" then goto LINK_STEP
$!
$goto LINK_STEP
$ COMPILE_MACROS:
$       set def boss_macro
$       if (f$search ("bosslib.olb;*") .NES. "") then $delete bosslib.olb;*
$       if (f$search ("*.obj;*") .NES. "") then $delete *.obj;*
$       write sys$output "Compiling MACROS."
$       macro bitpos/nodebug/nolist
$       write sys$output "BITPOS compiled."
$       macro distance/nodebug/nolist
$       write sys$output "DISTANCE compiled."
$       macro insert/nodebug/nolist
$       write sys$output "INSERT compiled."
$       macro maxmin/nodebug/nolist
$       write sys$output "MAXMIN compiled."
$       macro minmax/nodebug/nolist
$       write sys$output "MINMAX compiled."
$       macro putqio/nodebug/nolist
$       write sys$output "PUTQIO compiled."
$       macro randint/nodebug/nolist
$       write sys$output "RANDINT compiled."
$       macro randrep/nodebug/nolist
$       write sys$output "RANDREP compiled."
$       library/create bosslib.olb
$       library bosslib bitpos
$       library bosslib distance
$       library bosslib insert
$       library bosslib maxmin
$       library bosslib minmax
$       library bosslib putqio
$       library bosslib randint
$       library bosslib randrep
$       write sys$output "BOSSLIB.OLB built."
$   set def boss_source
$   exit
$!
$ LINK_STEP:
$       set def boss_source
$       if (f$search ("boss.map;*") .NES. "") then $delete boss.map;*
$       write sys$output "Linking."
$       link/sysshr/notrace/nodebug -
                boss_execute:boss,   -
		boss_macro:bosslib/library
$!
$!      For debugging purposes.
$!      link/sysshr/debug -
$!               boss_execute:boss,boss_macro:bosslib/library
$!
$!      Optional inclusion of termdef.pas  (not sure why)
$!      link/sysshr/notrace/nodebug -
$!	        boss_execute:boss,termdef,boss_macro:bosslib/library
$!
$       write sys$output "BOSS linked."
$       delete boss.obj;*
$	set protection =(system,owner=e,group=e,world=e) boss_execute:boss.exe
$       purge *.exe
$!
$!
$ HELP_LIBRARY:
$       set def boss_data
$	if (f$search("bosshlp.hlb") .EQS. "") then -
$	  library/create/help bosshlp bosshlp
$!
$ THE_END:
$       set def boss_main
$       exit
$!
$ ERROR_TRAP:
$       write sys$output "***Error resulted in termination***"
$       set def boss_main
$ exit
 
$ lock:
$ rename boss_execute:startup.com boss_execute:lock.temp
$ rename boss_execute:lock.com startup.com
$ rename boss_execute:lock.temp lock.com
$ exit
