$!
$ define datadir disk$a:[cloister.foo.boss.dat]
$ define bossdir disk$a:[cloister.foo.boss]
$
$ if p1 .eqs. "SCORE" then goto score
$ if p1 .eqs. "HELP" then goto help
$
$ normal:
$! set mess/nof/nos/noi/not
$
$ !update playlog
$ open/append file datadir:bosslog.dat
$ string = "New Character"
$ if p1 .nes. "" then string = p1
$ usernam = f$getjpi("","username")
$ line = f$edit(f$getsyi("nodename"),"trim") + "::" + usernam
$ line = line + "  started at:"+f$time()+" : "+string
$ write file line
$ close file
$
$ !check user file.
$ open/read/write/share userfile datadir:users.dat
$
$ userloop:
$ read/end=newuser userfile line
$ if line .eqs. usernam
$ then
$   close userfile
$   goto start
$ endif
$ goto userloop
$
$ newuser:
$ write userfile usernam
$ close userfile
$ type/page datadir:newuser.txt
$ inquire hitpage "			Press <return> to play."
$
$
$ !---start the game---
$ start:
$ commandline = f$trnlnm ("MAS$DRVCOM")
$ define /nolog sys$input tt
$ define /nolog/user sys$command tt
$ write sys$output commandline
$ mcr bossdir:boss.exe 'commandline 'p1
$
$
$ !update the playlog and exit
$ open/append file datadir:bosslog.dat
$ line = f$edit(f$getsyi("nodename"),"trim")+"::"+usernam
$ if f$extract(0,4,line) .eqs. "MAX:" then line = line + "  "
$ if f$extract(0,4,line) .eqs. "VAX1" then line = line + " "
$ line = line + " finished at:"+f$time()+" : "+string
$ write file line
$ close file
$ set mess/s/f/i/t
$ goto exiting

$ score:
$ n = 0
$ write sys$output "Username      Points  Character              Level    Quality       Class"
$ open/read scorefile datadir:bosstop.dat
$ scoreloop:
$ read/end=done scorefile line
$ write sys$output line
$ n = n + 1
$ if n .eq. 20 then goto pause
$ goto scoreloop

$ pause:
$ n = 0
$ inquire line "press enter to continue"
$ if line .eqs. " " then goto done
$ goto scoreloop

$ done:
$ close scorefile
$ goto exiting
 
$ help:
$ spawn/nowait help/prompt/page/noliblist/library=datadir:bosshlp.hlb
$
$ exiting:
$ deassign datadir
$ deassign bossdir
$ exit

