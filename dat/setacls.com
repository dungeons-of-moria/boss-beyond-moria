$ set verify
$! this should set the acl's to the proper settings.
$ set file/acl=(ident=game_player,access=read+write) bosslog.dat
$ set file/acl=(ident=game_player,access=read+write) bosschr.dat
$ set file/acl=(ident=game_player,access=read+write) bosstop.dat
$ set file/acl=(ident=game_player,access=read+write) users.dat
$ set file/acl=(ident=game_player,access=read) monsters.dat
$ set file/acl=(ident=game_player,access=read) objects.dat
$ set file/acl=(ident=game_player,access=read) invent.dat
$ set file/acl=(ident=game_player,access=read) quotes.dat
$ set file/acl=(ident=game_player,access=read) bosshlp.hlb
$ set file/acl=(ident=game_player,access=read) bus.dat
$ set file/acl=(ident=game_player,access=read) hours.dat
$ set file/acl=(ident=game_player,access=read) loser.dat
$ set file/acl=(ident=game_player,access=read) message.dat
$ set file/acl=(ident=game_player,access=read) newuser.txt
$ set file/acl=(ident=game_player,access=read) putzs.dat
$ set file/acl=(ident=game_player,access=read) skills.dat
$ set file/acl=(ident=game_player,access=read) wizard.dat
$! there, all done.
$ set noverify
$ exit
