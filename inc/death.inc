{ Handles the gravestone and the top fifty routines, and
  also the 100-day time limit routine. }
[psect(death$code)] procedure upon_death;

var
	points : integer;
   
 
{ Prints the gravestone of the character }
    procedure print_tomb;
      var
        str1,str2,str3,str4,str5,str6,str7,str8 : vtype;
        dstr : array [0..19] of vtype;
        fnam : vtype;
	command : char;
        f1 : text;
        i1 : integer;
        deathdate : packed array [1..11] of char;
        flag : boolean;
      BEGIN
        date(deathdate);
        str1 := fill_str(py.misc.name,31);
        str2 := fill_str(py.misc.title,31);
        str3 := fill_str(py.misc.tclass,31);
        writev(str4,'Level : ',py.misc.lev:1);
        str4 := fill_str(str4,31);
        writev(str5,py.misc.exp:1,' Exp');
        str5 := fill_str(str5,31);
        writev(str6,py.misc.au:1,' Dollars');
        str6 := fill_str(str6,31);
        str7 := 'Died near ' + townlist[town_num].town_name;
        str7 := fill_str(str7,31);
        str8 := fill_str(died_from,31);
dstr[00] := ' ';
dstr[01] := '               _______________________';
dstr[02] := '              /                       \         ___';
dstr[03] := '             /                         \ ___   /   \      ___';
dstr[04] := '            /            RIP            \   \  |   |     /   \';
dstr[05] := '           /                             \  | _|___|_    |   |';
dstr[06] := '          /'+str1+                       '\_|_          _|___|_';
dstr[07] := '         |               the               |   ___';
dstr[08] := '         | '+str2+                       ' |  /   \';
dstr[09] := '         |                                 |  |   |';
dstr[10] := '         | '+str3+                       ' | _|___|_   ____';
dstr[11] := '         | '+str4+                       ' |          /    \';
dstr[12] := '         | '+str5+                       ' |          |    |';
dstr[13] := '         | '+str6+                       ' |          |    |';
dstr[14] := '         | '+str7+                       ' |         _|____|_';
dstr[15] := '         |            killed by            |';
dstr[16] := '         | '+str8+                       ' |';
dstr[17] := '         |           '+deathdate+'           |';
dstr[18] := '        *|   *     *     *    *   *     *  | *';
dstr[19] := '________)/\\_)_/___(\/___(//_\)/_\//__\\(/_|_)_______';
        clear(1,1);
        for i1 := 0 to 19 do
          dprint(dstr[i1],i1+1);
        flush;
        if (get_com('Print to file? (Y/N)',command)) then
          case command of
            'y','Y':  BEGIN
                        prt('Enter Filename:',1,1);
                        flag := false;
                        repeat
                          if (get_string(fnam,1,17,60)) then
                            BEGIN
                              if (length(fnam) = 0) then fnam:='BOSSCHR.DIE';
                              open (f1,file_name:=fnam,error:=continue);
                              if (status(f1) <> 0) then
                                prt('Error creating> ' + fnam,2,1)
                              else
                                BEGIN
                                  flag := true;
                                  rewrite(f1,error:=continue);
                                  for i1 := 0 to 19 do
                                    writeln(f1,dstr[i1],error:=continue);
                                END;
                              close(f1,error:=continue);
                            END
                          else
                            flag := true;
                        until(flag)
                      END;
            otherwise
          END;
        flush;
        if (get_com('Print character sheet to file? (Y/N)',command)) then
          case command of
            'y','Y' :  character_print;
            otherwise;
          end;
      END;
 
 
 
{ Enters a players name on the top forty list }
    procedure top_fifty(pts : integer);
      var
        list 			  : array [1..50] of vtype;
        blank			  : packed array [1..13] of char;
        i1,i2,i4,n1,mwk5,trys     : integer;
        o1,o2,o3,o4		  : vtype;
        f1			  : text;
        flag,file_flag		  : boolean;
	command			  : char;
      BEGIN
        if (wizard) then exit;
        clear(1,1);
        for i1 := 1 to 50 do
          list[i1] := '';
        n1 := 1;
        priv_switch(1);
        trys := 0;
        file_flag := false;
        repeat
          open (f1,file_name:=boss_top,
                organization:=sequential,history:=old,
                sharing:=none,error:=continue);
          if (status(f1) = 2) then
            BEGIN
              trys := trys + 1;
              if (trys > 5) then
                file_flag := true
              else
                sleep(2);
            END
          else
            file_flag := true;
        until(file_flag);
        if ((status(f1) <> 0) and (status(f1) <> 2)) then
          open (f1,file_name:=boss_top,
                organization:=sequential,history:=new,
                sharing:=none,error:=continue);
        if (status(f1) <> 0) then
          BEGIN
            writeln('Error in opening ',boss_top);
            writeln('Please contact local BOSS Wizard.');
            exit;
          END;
        reset(f1);
        while ((not eof(f1)) and (n1 <= 50)) do
          BEGIN
            readln(f1,list[n1],error:=continue);
            n1 := n1 + 1;
          END;
        n1 := n1 - 1;                
        i1 := 1;
        flag := false;
        while ((i1 <= n1) and (not flag)) do
          BEGIN
            readv(list[i1],blank,i4);
            if (i4 < pts) then
              flag := true
            else
              i1 := i1 + 1;
          END;
        if ((flag) or (n1 = 0) or (n1 < 50)) then
          BEGIN
            for i2 := 49 downto i1 do
              list[i2+1] := list[i2];
            o1 := username;
	    o2 := pad(py.misc.name,' ',24);
	    o3 := pad(py.misc.race,' ',12);
	    o4 := pad(py.misc.tclass,' ',11);	    
            writev(list[i1],pad(o1,' ',13),pts:7,'  ',o2,'  ',
		                 py.misc.lev:2,'    ',o3,'  ',o4);
            if (n1 < 50) then
              n1 := n1 + 1;
            flag := false;
          END;
        rewrite(f1);
        for i1 := 1 to n1 do
          writeln(f1,list[i1]);
        close(f1);    
        priv_switch(0);
	writeln;
	writeln;
	writeln;
        prt('Username      Points  Character               Level   Quality       Class',3,1);
        for i1 := 1 to n1 do
          BEGIN
             writeln(list[i1]);
             if ((i1 mod 20) = 0) then
               begin
	         msg_flag := true;
		 msg_print('');
                 clear(4,1);
               end;
             mwk5:=i1
          END;
        mwk5:=mwk5+2;
        writeln;
        put_qio
      END;
 
 
{ Change the player into a King! }
    procedure kingly;
      BEGIN
{ Change the character attributes.}
        dun_level := 0;
        died_from := 'Ripe Old Age';
        with py.misc do
          BEGIN
	    if (lev <= max_player_level) then
              lev := lev + max_player_level;
            max_exp := 1000000*total_winner_num + randint(1000);
            exp := max_exp
          END;
{ Let the player know that he did good.}
        clear(1,1);
        dprint('                                  #',2);
        dprint('                                #####',3);
        dprint('                                  #',4);
        dprint('                            ,,,  $$$  ,,,',5);
        dprint('                        ,,=$   "$$$$$"   $=,,',6);
        dprint('                      ,$$        $$$        $$,',7);
        dprint('                      *>         <*>         <*',8);
        dprint('                      $$         $$$         $$',9);
        dprint('                      "$$        $$$        $$"',10); 
        dprint('                       "$$       $$$       $$"',11);
        dprint('                        *#########*#########*',12);
        dprint('                        *#########*#########*',13);
        dprint('                          Veni, Vidi, Vici!',16);
        dprint('                      You have slain the BOSS!',17);
        dprint('                        And saved the world!',18);
        flush;
        pause(24);
      END;
 
 
{ What happens upon dying...}
  BEGIN
    with py.misc do
      begin
        points := trunc(max_exp/expfact) +
           100*(max_lev + townlist[town_num].lev_plus) - principal;
        if (total_winner_num > 0) then
          begin      
	    kingly;
            lev := lev + max_player_level;
            points := points + 1000000*total_winner_num + 1000*(100-day);
            exp := max_exp
          end
      end;
    print_tomb;
    top_fifty(points);
    exit;
  END;

[psect(death$code)] procedure time_out;
   
BEGIN
  clear(1,1);
  prt('Midnight, the bewitching hour.  It has been 100 days since you',1,3);
  prt('started your quest.  You have been busy, and so has the Boss.',2,3);
  prt('Sadly for you and the world you were supposed to save, the Boss',3,3);
  prt('has amassed enough power to take over the world.  He has seized',4,3);
  prt('control of all major world governments and the world economy.',5,3);
  prt('A simple pack of chewing gum now costs over 400 dollars, and',6,3);
  prt('there is only one brand on the market.  All over the world,',7,3);
  prt('people cry out in vain for freedom and brand name choices.',8,3);
  prt('You were their last hope, and you have let them down.  The Boss',9,3);
  prt('has vacated the Earth and now resides in an ultra-secure satel-',10,3);
  prt('lite orbiting in geosynchronous orbit over Sumatra.  You lose,',11,3);
  prt('although you may explore the Boss'' demense to your heart''s',12,3);
  prt('content.  But it will do absolutely no good at all.',13,3);
  pause(24);
  out_of_time := true;
  draw_cave;
END;


