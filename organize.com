$ set verify  ! so you can see what's going on as it happens.
$!
$! let's have some directories.
$ create/dir [.boss]     
$ create/dir [.boss.etc]
$ create/dir [.boss.inc]
$ create/dir [.boss.dat]
$ create/dir [.boss.mar]
$!
$! move files into the main boss directory
$ if f$search("mailinstruct.txt") .nes. "" ! this file might not exist, so
$ then                                     ! don't freak if it's not there.
$   rename MAILINSTRUCT.TXT [.boss]MAILINSTRUCT.TXT
$ endif
$ rename BOSS.PAS [.boss]BOSS.PAS 
$ rename BUILD.COM [.boss]BUILD.COM 
$ rename INSTALL.DOC [.boss]INSTALL.DOC 
$ rename STARTUP.COM [.boss]STARTUP.COM 
$ rename LOCK.COM [.boss]LOCK.COM
$!
$! dat directory: 
$ rename SETACLS.COM [.boss.dat]SETACLS.COM
$ rename BOSSHLP.HLP [.boss.dat]BOSSHLP.HLP 
$ rename BOSSTOP.DAT [.boss.dat]BOSSTOP.DAT 
$ rename BUS.DAT [.boss.dat]BUS.DAT 
$ rename CLEANLOG.COM [.boss.dat]CLEANLOG.COM 
$ rename HOURS.DAT [.boss.dat]HOURS.DAT 
$ rename LOSER.DAT [.boss.dat]LOSER.DAT 
$ rename MONSTERS.DAT [.boss.dat]MONSTERS.DAT 
$ rename NEWUSER.TXT [.boss.dat]NEWUSER.TXT 
$ rename PUTZS.DAT [.boss.dat]PUTZS.DAT 
$ rename USERS.DAT [.boss.dat]USERS.DAT 
$ rename MESSAGE.DAT [.boss.dat]MESSAGE.DAT 
$ rename WIZARD.DAT [.boss.dat]WIZARD.DAT 
$ rename OBJECTS.DAT [.boss.dat]OBJECTS.DAT
$ rename INVENT.DAT [.boss.dat]INVENT.DAT
$ rename QUOTES.DAT [.boss.dat]QUOTES.DAT
$ rename SKILLS.DAT [.boss.dat]SKILLS.DAT
$!
$! etc directory: 
$ rename BUGS.TXT [.boss.etc]BUGS.TXT 
$ rename CREATURE.DOC [.boss.etc]CREATURE.DOC 
$ rename ITEMFLAGS.DOC [.boss.etc]ITEMFLAGS.DOC 
$ rename OBJECT.DOC [.boss.etc]OBJECT.DOC 
$ rename WISH.LIST [.boss.etc]WISH.LIST 
$!
$! inc directory: 
$ rename CONSTANTS.INC [.boss.inc]CONSTANTS.INC 
$ rename CREATE.INC [.boss.inc]CREATE.INC 
$ rename CREATURE.INC [.boss.inc]CREATURE.INC 
$ rename DATAFILES.INC [.boss.inc]DATAFILES.INC 
$ rename DEATH.INC [.boss.inc]DEATH.INC 
$ rename DESC.INC [.boss.inc]DESC.INC 
$ rename DEVICE.INC [.boss.inc]DEVICE.INC 
$ rename DISPLAY.INC [.boss.inc]DISPLAY.INC 
$ rename EAT.INC [.boss.inc]EAT.INC 
$ rename FILES.INC [.boss.inc]FILES.INC 
$ rename FLOPPY.INC [.boss.inc]FLOPPY.INC 
$ rename GENERATE.INC [.boss.inc]GENERATE.INC 
$ rename HELP.INC [.boss.inc]HELP.INC 
$ rename IO.INC [.boss.inc]IO.INC 
$ rename MAIN.INC [.boss.inc]MAIN.INC 
$ rename MISC.INC [.boss.inc]MISC.INC 
$ rename OBJECTS.INC [.boss.inc]OBJECTS.INC 
$ rename POTIONS.INC [.boss.inc]POTIONS.INC 
$ rename PRAYER.INC [.boss.inc]PRAYER.INC 
$ rename RAYGUN.INC [.boss.inc]RAYGUN.INC 
$ rename SAVE.INC [.boss.inc]SAVE.INC 
$ rename SKILLS.INC [.boss.inc]SKILLS.INC 
$ rename STORE1.INC [.boss.inc]STORE1.INC 
$ rename STORE2.INC [.boss.inc]STORE2.INC 
$ rename TECH.INC [.boss.inc]TECH.INC 
$ rename TERMDEF.INC [.boss.inc]TERMDEF.INC 
$ rename TRAIN.INC [.boss.inc]TRAIN.INC 
$ rename TREASURE.INC [.boss.inc]TREASURE.INC 
$ rename TRICK.INC [.boss.inc]TRICK.INC 
$ rename TYPES.INC [.boss.inc]TYPES.INC 
$ rename VALUES.INC [.boss.inc]VALUES.INC 
$ rename VARIABLES.INC [.boss.inc]VARIABLES.INC 
$ rename WIERD.INC [.boss.inc]WIERD.INC 
$ rename WIZARD.INC [.boss.inc]WIZARD.INC 
$!
$! mar directory: 
$ rename BITPOS.MAR [.boss.mar]BITPOS.MAR 
$ rename DISTANCE.MAR [.boss.mar]DISTANCE.MAR 
$ rename INSERT.MAR [.boss.mar]INSERT.MAR 
$ rename MAXMIN.MAR [.boss.mar]MAXMIN.MAR 
$ rename MINMAX.MAR [.boss.mar]MINMAX.MAR 
$ rename PUTQIO.MAR [.boss.mar]PUTQIO.MAR 
$ rename RANDINT.MAR [.boss.mar]RANDINT.MAR 
$ rename RANDREP.MAR [.boss.mar]RANDREP.MAR 
$ set noverify
$ write sys$output "Ok.  Now move into your new [.boss] subdirectory "
$ write sys$output "and follow the instructions in install.doc"
