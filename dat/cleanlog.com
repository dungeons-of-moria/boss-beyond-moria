$ if (f$search("bosslog.dat") .NES. "") then delete bosslog.dat.*
$ open/write file bosslog.dat
$ write file "BOSS playlog: started at "+f$time()
$ close file
$ set prot=w:rw bosslog.dat
