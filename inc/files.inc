{ function returns the players USERNAME }
[psect(id$code)] function get_username : usernam_type;
      var
        user : usernam_type;
        icode : integer;
        jpirec : recj;
 
{ calls GETJPI routine to return the USERNAME }
      function sys$getjpi       (%immed p1 : integer;
                                %immed  p2 : integer;
                                %immed  p3 : integer;
                                var itmlst : recj;
                                %immed  p4 : integer;
                                %immed  p5 : integer;
                                %immed  p6 : integer) : integer;
        external;
 
      BEGIN
        with jpirec do
          BEGIN
            unameinfo.unamelen := 12;
            unameinfo.jpi$_username := %x202;
            new (ptr_uname);
            ptr_uname^ := '            ';
            new (ptr_unamelen);
            ptr_unamelen^ := 0;
            endlist := 0
          END;
        icode := SYS$GETJPI (0,0,0,jpirec,0,0,0);
        if not odd(icode) then
          BEGIN
            writeln('Error in GETJPI process');
            halt
          END
        else
          get_username := jpirec.ptr_uname^
        END;


{Compares Username to Users listed in a file. If in file, returns true. -RLG}
[psect(id$code)] function user_vs_file(boss_file : vtype) : boolean;
    var
        usr, curusr: usernam_type;
        file1 : text;  

BEGIN                                                         {-NBT-}
  user_vs_file := false; 
  usr := username;
  priv_switch(1);
  open(file1,file_name:=boss_file, organization:=sequential,history:=old,
       sharing:=readwrite,error:=continue);
  if (status(file1) = 0) then
          BEGIN
                reset(file1);
                while not(eof(file1)) do begin
                  readln(file1,curusr,error:=continue);
                  if usr = curusr then user_vs_file := true;
                end;
                close(file1,error:=continue);
          END
        else
         BEGIN
           prt ('ERROR: A severe error occurred in ID Section.',1,1);
	   prt ('Please notify the BOSS Master...',2,1);
           Pause(24);
         End;
  END;


  { Function returns true if proper password is given.   -RLG }
[psect(id$code)] function check_pswd   :  boolean;
  var
      i1     : integer;
      x      : char;
      tpw    : packed array [1..12] of char;
 
  BEGIN
        check_pswd := false;
        i1 := 0;
        tpw := '            ';
        prt('Password : ',1,1);
        repeat
            inkey(x);
            case ord(x) of
              13  : ;
              otherwise  
	        begin
                  i1 := i1 + 1;
                  tpw[i1] := x;
                end
            end;
        until ((i1 = 12) or (ord(x) = 13));
        if (tpw = gurgle) then check_pswd := true;
        msg_flag := false;
        erase_line(msg_line,msg_line);
  END;


[psect(id$code)] procedure look_at_userid;

     BEGIN
	username  := get_username;
	validuser := user_vs_file(BOSS_WIZ);
        scumuser  := user_vs_file(BOSS_LOS);
        putzuser  := user_vs_file(BOSS_ANNOY);
     END;
   


	{ Attempt to open the intro file }
[psect(setup$code)] procedure intro(var finam : vtype);
      var
        xpos,i1 : integer;
        dummy   : char;
        day_test,in_line: vtype;
        file1   : text;
        file2   : file of key_type;
        exit_flag       : boolean;
      BEGIN
        exit_flag := false;
        clear(1,1);
{ Attempt to read hours.dat. If it does not exist, then create a standard one.}
        priv_switch(1);
        open(file1,file_name:=BOSS_HOU,
                history:=readonly,sharing:=readonly,error:=continue);
        if (status(file1) = 0) then
          BEGIN
            reset(file1);
            repeat
              readln(file1,in_line,error:=continue);
              if (length(in_line) > 3) then
                BEGIN
                  day_test := substr(in_line,1,4);
                  if      (day_test = 'SUN:') then days[1] := in_line
                  else if (day_test = 'MON:') then days[2] := in_line
                  else if (day_test = 'TUE:') then days[3] := in_line
                  else if (day_test = 'WED:') then days[4] := in_line
                  else if (day_test = 'THU:') then days[5] := in_line
                  else if (day_test = 'FRI:') then days[6] := in_line
                  else if (day_test = 'SAT:') then days[7] := in_line;
                END;
            until(eof(file1));
            close(file1,error:=continue);
            priv_switch(0);
          END
        else    { Create a standard hours file }
          BEGIN
            priv_switch(0);
            open(file1,file_name:=BOSS_HOU,
                organization:=sequential,history:=new,
                sharing:=readwrite,error:=continue);
            if (status(file1) = 0) then
              BEGIN
                rewrite(file1,error:=continue);
                writeln(file1,'    Boss operating hours are:');
                writeln(file1,'    |    AM     |    PM     |');
                writeln(file1,'    1         111         111');
                writeln(file1,'    2123456789012123456789012');
                for i1 := 1 to 7 do
                  writeln(file1,days[i1]);
                writeln(file1,'       (X=Open; .=Closed)');
                close(file1,error:=continue);
                writeln('Created ',BOSS_HOU);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_HOU);
                exit;
              END;
          END;
 
{ Check the hours, if closed then require password }
        if (not(exit_flag)) then
          BEGIN
            xpos := index(finam,'/WIZARD');
            if ( xpos > 0) then
{ Opusii }   if (validuser) then 
              insert_str(finam,'/WIZARD','');
            if (not(wizard)) then
              no_controly;
            if (not(check_time)) then
              if (not(validuser)) then
                BEGIN
                  priv_switch(1);
                  open(file1,file_name:=BOSS_HOU,
                        history:=readonly,sharing:=readonly,error:=continue);
                  if (status(file1) = 0) then
                    BEGIN
                      reset(file1);
                      repeat
                        readln(file1,in_line);
                        writeln(in_line);
                      until(eof(file1));
                      close(file1,error:=continue);
                    END;
                  priv_switch(0);
                  exit;
                END;
          END;
 
{ Print the introduction message, news, ect.}
        priv_switch(1);
        open(file1,file_name:=BOSS_MOR,
                organization:=sequential,history:=old,
                sharing:=readwrite,error:=continue);
        if (status(file1) = 0) then
          BEGIN
            if (not(exit_flag)) then
              BEGIN
                reset(file1);
                repeat
                  readln(file1,in_line,error:=continue);
                  writeln(in_line);
                until (eof(file1));
                pause_exit(24);
                close(file1,error:=continue);
              END
            else
              close(file1,error:=continue);
            priv_switch(0);
          END
        else    { Create one.}
          BEGIN
            priv_switch(0);
            open(file1,file_name:=BOSS_MOR,
                organization:=sequential,history:=new,
                sharing:=readwrite,error:=continue);
            if (status(file1) = 0) then
              BEGIN
                rewrite(file1,error:=continue);
writeln(file1,'                          ***********************');
writeln(file1,'                          **    BOSS ',cur_version:7:2,'   **');
writeln(file1,'                          ***********************');
writeln(file1,'                      COPYRIGHT (c) Robert Alan Koeneke');
writeln(file1,' ');
writeln(file1,'Programers : Robert Alan Koeneke / University of Oklahoma');
writeln(file1,'             Jimmey Wayne Todd   / University of Oklahoma');
writeln(file1,' ');
writeln(file1,' ');
writeln(file1,'Modified by: Bill Palter         / State University of New York');
writeln(file1,'             Nick B Triantos     / State University of New York');
writeln(file1,'         						');
writeln(file1,'BOSS Modifications by: Robert Gulledge / University of Washington');
writeln(file1,'                       jason black     / University of Washington');
writeln(file1,' ');
writeln(file1,' ');
writeln(file1,'             Check NewsInc in the office building for new features!.');
                close(file1,error:=continue);
                writeln('Created ',BOSS_MOR);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_MOR);
                exit;
              END;
          END;
{ Check for MASTER.DAT }
        priv_switch(1);
        open (file2,file_name:=boss_mas,
                access_method:=keyed,organization:=indexed,history:=readonly,
                sharing:=readwrite,error:=continue);
        if (status(file2) <> 0) then
          BEGIN
            priv_switch(0);
            open (file2,file_name:=boss_mas,
                access_method:=keyed,organization:=indexed,history:=new,
                sharing:=readwrite,error:=continue);
            if (status(file2) = 0) then
              BEGIN
                writeln('Created ',BOSS_MAS);
                close(file2,error:=continue);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_MAS);
                exit;
              END;
          END
        else
          BEGIN
            close(file2,error:=continue);
            priv_switch(0);
          END;
{ Check for TOPTWENTY.DAT }
        priv_switch(1);
        open (file1,file_name:=boss_top,
                organization:=sequential,history:=readonly,
                sharing:=readwrite,error:=continue);
        if ((status(file1) <> 0) and (status(file1) <> 2)) then
          BEGIN
            priv_switch(0);
            open (file1,file_name:=boss_top,
                organization:=sequential,history:=new,
                sharing:=readwrite,error:=continue);
            if (status(file1) = 0) then
              BEGIN
                writeln('Created ',BOSS_TOP);
                close(file1,error:=continue);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_TOP);
                exit;
              END;
          END
        else
          BEGIN
            close(file1,error:=continue);
            priv_switch(0);
          END;
        if (exit_flag) then
          BEGIN
            writeln('Notice: System BOSS wizard should set the protection');
            writeln('        on files just created. See INSTALL.DOC for');
            writeln('        help on setting protection on the files.');
            writeln('Notice: File HOURS.DAT may be edited to set operating');
            writeln('        hours for BOSS.');
            writeln('Notice: File MESSAGE.DAT may be edited to contain news');
            writeln('        items, etc...');
            exit;
          END;
      END;
 
 
{ Prints dungeon map to external file }
[psect(misc2$code)] procedure print_map;
    var
        i1,i2,i3,i4,i5,i6,i7,i8 : integer;
        dun_line: varying [133] of char;
        filename1       : varying [80] of char;
        tmp     : char;
        file1   : text;
    BEGIN
      prt('File name: ',1,1);
      if (get_string(filename1,1,12,64)) then
        BEGIN
          if (length(filename1) = 0) then filename1 := 'BOSSMAP.DAT';
          open(file1,filename1,error:=continue);
          if (status(file1) = 0) then
            BEGIN
              prt('Writing Boss Dungeon Map...',1,1);
              put_qio;
              rewrite(file1,error:=continue);
              i1 := 1;
              i7 := 0;
              repeat
                i2 := 1;
                i3 := i1 + outpage_height - 1;
                if (i3 > cur_height) then
                  i3 := cur_height;
                i7 := i7 + 1;
                i8 := 0;
                repeat
                  i4 := i2 + outpage_width - 1;
                  if (i4 > cur_width) then
                    i4 := cur_width;
                  i8 := i8 + 1;
                  writeln(file1,chr(12),error:=continue);
                  write(file1,'Section[',i7:1,',',i8:1,'];     ',
                                                        error:=continue);
                  writeln(file1,'Depth : ',(dun_level*50):1,' (meters)',
                                                        error:=continue);
                  writeln(file1,' ',error:=continue);
                  write(file1,'   ',error:=continue);
                  for i5 := i2 to i4 do
                    BEGIN
                      i6 := trunc(i5/100);
                      write(file1,i6:1,error:=continue);
                    END;
                  writeln(file1,error:=continue);
                  write(file1,'   ',error:=continue);
                  for i5 := i2 to i4 do
                    BEGIN
                      i6 := trunc(i5/10) - trunc(i5/100)*10;
                      write(file1,i6:1,error:=continue);
                    END;
                  writeln(file1,error:=continue);
                  write(file1,'   ',error:=continue);
                  for i5 := i2 to i4 do
                    BEGIN
                      i6 := i5 - trunc(i5/10)*10;
                      write(file1,i6:1,error:=continue);
                    END;
                  writeln(file1,error:=continue);
                  for i5 := i1 to i3 do
                    BEGIN
                      writev(dun_line,i5:3);
                      for i6 := i2 to i4 do
                        BEGIN
                          if (test_light(i5,i6)) then
                            loc_symbol(i5,i6,tmp)
                          else
                            tmp := ' ';
                          dun_line := dun_line + tmp;
                        END;
                      writeln(file1,dun_line,error:=continue);
                    END;
                  i2 := i2 + outpage_width;
                until (i2 >= cur_width);
                i1 := i1 + outpage_height;
              until (i1 >= cur_height);
              close(file1,error:=continue);
              prt('Completed.',1,1);
            END;
        END
    END;
 
 
{ Prints a list of random objects to a file.  Note that the object file
  is a sampling of objects which be expected to appear on that level. }
[psect(misc2$code)] procedure print_objects;
    var
        nobj,i1,i2,level : integer;
        filename1,tmp_str : varying [80] of char;
        file1   : text;
    BEGIN
      prt('Produce objects on what level?: ',1,1);
      get_string(tmp_str,1,33,10);
      level := 0;
      readv(tmp_str,level,error:=continue);
      prt('Produce how many objects?: ',1,1);
      get_string(tmp_str,1,28,10);
      nobj := 0;
      readv(tmp_str,nobj,error:=continue);
      if ((nobj > 0) and (level > -1) and (level < 1201)) then
        BEGIN
          if (nobj > 9999) then nobj := 9999;
          prt('File name: ',1,1);
          if (get_string(filename1,1,12,64)) then
            BEGIN
              if (length(filename1) = 0) then filename1 := 'BOSSOBJ.DAT';
              open(file1,filename1,error:=continue);
              if (status(file1) = 0) then
                BEGIN
                  writev(tmp_str,nobj:1);
                  prt(tmp_str + ' random objects being produced...',1,1);
                  put_qio;
                  rewrite(file1,error:=continue);
        writeln(file1,'*** Random Object Sampling:',error:=continue);
        writeln(file1,'*** ',nobj:1,' objects',error:=continue);
        writeln(file1,'*** For Level ',level:1,error:=continue);
        writeln(file1,'',error:=continue);
        writeln(file1,'',error:=continue);
                  popt(i2);
                  for i1 := 1 to nobj do
                    BEGIN
                      t_list[i2] := object_list[get_obj_num(level)];
                      magic_treasure(t_list[i2],level);
                      temporary_slot := t_list[i2];
                      with temporary_slot do
                        BEGIN
                          unquote(name);
                          known1(name);
                          known2(name);
                        END;
                      objdes(tmp_str,1,true,'t');
                      writeln(file1,tmp_str,error:=continue);
                    END;
                  pusht(i2);
                  close(file1,error:=continue);
                  prt('Completed.',1,1);
                END
              else
                prt('File could not be opened.',1,1);
            END;
        END;
    END;
 
 
{ Prints a listing of monsters }
[psect(wizard$code)] procedure print_monsters;
    var
        i1,i2,xpos,atype,adesc  : integer;
        file1   : text;
        out_val,new_val,filename1       : vtype;
        attstr,attx     : vtype;
        damstr  : dtype;
 
    BEGIN
      prt('File name: ',1,1);
      if (get_string(filename1,1,12,64)) then
        BEGIN
          if (length(filename1) = 0) then filename1 := 'BOSSMON.DAT';
          open(file1,filename1,error:=continue);
          if (status(file1) = 0) then
            BEGIN
              prt('Writing Monster Dictionary...',1,1);
              put_qio;
              rewrite(file1,error:=continue);
              for i1 := 1 to max_creatures do
                with c_list[i1] do
                  BEGIN
{ BEGIN writing to file }
writeln(file1,'--------------------------------------------',error:=continue);
out_val := name + '                              ';
writeln(file1,i1:3,'  ',out_val:30,'     (',cchar:1,')',error:=continue);
writeln(file1,'     Speed =',speed:2,'  Level     =',level:2,'  Exp =',mexp:5,
                                                        error:=continue);
writeln(file1,'     AC    =',ac:2,   '  Eye-sight =',aaf:2,'  HD  =',hd:5,
                                                        error:=continue);
if (uand(%X'80000000',cmove) <> 0) then
  writeln(file1,'     Creature is a ***Win Creature***',error:=continue);
if (uand(%X'00080000',cmove) <> 0) then
  writeln(file1,'     Creature Eats/kills other creatures.',error:=continue);
if (uand(%X'0001',cdefense) <> 0) then
  writeln(file1,'     Creature is a dragon.',error:=continue);
if (uand(%X'0002',cdefense) <> 0) then
  writeln(file1,'     Creature is a monster.',error:=continue);
if (uand(%X'0004',cdefense) <> 0) then
  writeln(file1,'     Creature is evil.',error:=continue);
if (uand(%X'0008',cdefense) <> 0) then
  writeln(file1,'     Creature is undead.',error:=continue);
if (uand(%X'0010',cdefense) <> 0) then
  writeln(file1,'     Creature harmed by cold.',error:=continue);
if (uand(%X'0020',cdefense) <> 0) then
  writeln(file1,'     Creature harmed by fire.',error:=continue);
if (uand(%X'0040',cdefense) <> 0) then
  writeln(file1,'     Creature harmed by poison.',error:=continue);
if (uand(%X'0080',cdefense) <> 0) then
  writeln(file1,'     Creature harmed by acid.',error:=continue);
if (uand(%X'0100',cdefense) <> 0) then
  writeln(file1,'     Creature harmed by blue light.',error:=continue);
if (uand(%X'0200',cdefense) <> 0) then
  writeln(file1,'     Creature harmed by Stone-to-Mud.',error:=continue);
if (uand(%X'0400',cdefense) <> 0) then
  writeln(file1,'     Creature harmed by Kill Robot.',error:=continue);
if (uand(%X'0800',cdefense) <> 0) then
  writeln(file1,'     Negative Experience if Killed.',error:=continue);
if (uand(%X'1000',cdefense) <> 0) then
  writeln(file1,'     Creature cannot be charmed or slept.',error:=continue);
if (uand(%X'2000',cdefense) <> 0) then
  writeln(file1,'     Creature seen with Infra-Vision.',error:=continue);
if (uand(%X'4000',cdefense) <> 0) then
  writeln(file1,'     Creature has MAX hit points.',error:=continue);
if (uand(%X'00010000',cmove) <> 0) then
  writeln(file1,'     Creature is invisible.',error:=continue);
if (uand(%X'00100000',cmove) <> 0) then
  writeln(file1,'     Creature picks up objects.',error:=continue);
if (uand(%X'00200000',cmove) <> 0) then
  writeln(file1,'     Creature multiplies.',error:=continue);
if (uand(%X'01000000',cmove) <> 0) then
  writeln(file1,'     Carries object(s).',error:=continue);
if (uand(%X'02000000',cmove) <> 0) then
  writeln(file1,'     Carries gold, gems, ect.',error:=continue);
if (uand(%X'04000000',cmove) <> 0) then
  writeln(file1,'       Has object/gold 60% of time.',error:=continue);
if (uand(%X'08000000',cmove) <> 0) then
  writeln(file1,'       Has object/gold 90% of time.',error:=continue);
if (uand(%X'10000000',cmove) <> 0) then
  writeln(file1,'       Has 1d2 object(s)/gold.',error:=continue);
if (uand(%X'20000000',cmove) <> 0) then
  writeln(file1,'       Has 2d2 object(s)/gold.',error:=continue);
if (uand(%X'40000000',cmove) <> 0) then
  writeln(file1,'       Has 4d2 object(s)/gold.',error:=continue);
{ Creature casts spells / Breathes Dragon breath.}
if (spells > 0) then
  BEGIN
    writeln(file1,'   --Spells/Dragon Breath =',error:=continue);
    writeln(file1,'       Casts spells 1 out of ',uand(%X'F',spells):1,
                ' turns.',error:=continue);
    if (uand(%X'00000010',spells) <> 0) then
      writeln(file1,'       Can teleport short.',error:=continue);
    if (uand(%X'00000020',spells) <> 0) then
      writeln(file1,'       Can teleport long.',error:=continue);
    if (uand(%X'00000040',spells) <> 0) then
      writeln(file1,'       Teleport player to itself.',error:=continue);
    if (uand(%X'00000080',spells) <> 0) then
      writeln(file1,'       Cause light wounds.',error:=continue);
    if (uand(%X'00000100',spells) <> 0) then
      writeln(file1,'       Cause serious wounds.',error:=continue);
    if (uand(%X'00000200',spells) <> 0) then
      writeln(file1,'       Hold person.',error:=continue);
    if (uand(%X'00000400',spells) <> 0) then
      writeln(file1,'       Cause blindness.',error:=continue);
    if (uand(%X'00000800',spells) <> 0) then
      writeln(file1,'       Cause confusion.',error:=continue);
    if (uand(%X'00001000',spells) <> 0) then
      writeln(file1,'       Cause fear.',error:=continue);
    if (uand(%X'00002000',spells) <> 0) then
      writeln(file1,'       Summon a monster.',error:=continue);
    if (uand(%X'00004000',spells) <> 0) then
      writeln(file1,'       Summon an undead.',error:=continue);
    if (uand(%X'00008000',spells) <> 0) then
      writeln(file1,'       Slow person.',error:=continue);
    if (uand(%X'00010000',spells) <> 0) then
      writeln(file1,'       Drains mana for healing.',error:=continue);
    if (uand(%X'00020000',spells) <> 0) then
      writeln(file1,'       **Unknown spell value**',error:=continue);
    if (uand(%X'00040000',spells) <> 0) then
      writeln(file1,'       **Unknown spell value**',error:=continue);
    if (uand(%X'00080000',spells) <> 0) then
      writeln(file1,'       Breathes Lightning Dragon Breath.',error:=continue);
    if (uand(%X'00100000',spells) <> 0) then
      writeln(file1,'       Breathes Gas Dragon Breath.',error:=continue);
    if (uand(%X'00200000',spells) <> 0) then
      writeln(file1,'       Breathes Acid Dragon Breath.',error:=continue);
    if (uand(%X'00400000',spells) <> 0) then
      writeln(file1,'       Breathes Frost Dragon Breath.',error:=continue);
    if (uand(%X'00800000',spells) <> 0) then
      writeln(file1,'       Breathes Fire Dragon Breath.',error:=continue);
  END;
{ Movement for creature }
writeln(file1,'   --Movement =',error:=continue);
if (uand(%X'00000001',cmove) <> 0) then
  writeln(file1,'       Move only to attack.',error:=continue);
if (uand(%X'00000002',cmove) <> 0) then
  writeln(file1,'       Move and attack normally.',error:=continue);
if (uand(%X'00000008',cmove) <> 0) then
  writeln(file1,'       20% random movement.',error:=continue);
if (uand(%X'00000010',cmove) <> 0) then
  writeln(file1,'       40% random movement.',error:=continue);
if (uand(%X'00000020',cmove) <> 0) then
  writeln(file1,'       75% random movement.',error:=continue);
if (uand(%X'00020000',cmove) <> 0) then
  writeln(file1,'       Can open doors.',error:=continue);
if (uand(%X'00040000',cmove) <> 0) then
  writeln(file1,'       Can phase through walls.',error:=continue);
writeln(file1,'   --Creature attacks =',error:=continue);
attstr := damage;
while (length(attstr) > 0) do
  BEGIN
    xpos := index(attstr,'|');
    if (xpos > 0) then
      BEGIN
        attx := substr(attstr,1,xpos-1);
        attstr := substr(attstr,xpos+1,length(attstr)-xpos);
      END
    else
      BEGIN
        attx := attstr;
        attstr := '';
      END;
    readv(attx,atype,adesc,damstr);
    CASE atype of
{Normal attack  }     1  : new_val := 'normal damage.';
{Poison Strength}     2  : new_val := 'lowering strength.';
{Confusion attack}    3  : new_val := 'confusion.';
{Fear attack    }     4  : new_val := 'fear.';
{Fire attack    }     5  : new_val := 'fire damage.';
{Acid attack    }     6  : new_val := 'acid damage.';
{Cold attack    }     7  : new_val := 'cold damage.';
{Lightning attack}    8  : new_val := 'lightning damage.';
{Corrosion attack}    9  : new_val := 'corrosion damage.';
{Blindness attack}    10 : new_val := 'blindness.';
{Paralysis attack}    11 : new_val := 'paralyzation.';
{Steal Money    }     12 : new_val := 'stealing money.';
{Steal Object   }     13 : new_val := 'stealing object.';
{Poison         }     14 : new_val := 'poison damage.';
{Lose Dex       }     15 : new_val := 'lose dexterity.';
{Lose Con       }     16 : new_val := 'lose constitution.';
{Lose Int       }     17 : new_val := 'lose intelligence.';
{Lose Wis       }     18 : new_val := 'lose wisdom.';
{Lose Exp       }     19 : new_val := 'lose experience.';
{Aggravation    }     20 : new_val := 'aggravates monsters.';
{Disenchant     }     21 : new_val := 'disenchants objects.';
{Eats food      }     22 : new_val := 'eating food.';
{Eats light     }     23 : new_val := 'eating light source.';
{Eats charges   }     24 : new_val := 'absorbing charges.';
{Vampire Bite   }     25 : new_val := 'losing strength and experience';
{Turn to Stone  }     26 : new_val := 'turning to stone.';
{Mind Flayer    }     27 : new_val := 'sucking your brain out.';
{Hell Horse	}     28 : new_val := 'NightMare blindness.';
{Vargouille	}     29 : new_val := 'draining max hit points.';
{Diseases       }     30 : new_val := 'causes disease.';
{Lose charisma	}     31 : new_val := 'lose charisma.';
{Special        }     99 : new_val := 'blank message.';
                 otherwise new_val := '**Unknown value** AAAAUUUGH!!!!';
    END;
    out_val := attack_desc[adesc] + new_val + ' (' + damstr + ')';
    writeln(file1,out_val,error:=continue);
  END;
  for i2 := 1 to 2 do writeln(file1,' ',error:=continue);
END;
{ end writing to file }
              close(file1,error:=continue);
              prt('Completed.',1,1);
            END;
        END
    END;
 
 
{ Print the character to a file or device }
[psect(misc2$code)] procedure character_print;
    var
        i1,i2,i3: integer;
        file1   : text;
        out_val,filename1,prt1,prt2,new_page    : vtype;
        out_str,out_int,out_wis,out_dex,out_con,out_chr : stat_type;
 
    BEGIN
      prt('File name: ',1,1);
      if (get_string(filename1,1,12,64)) then
        BEGIN
          if (length(filename1) = 0) then filename1 := 'BOSSCHR.DAT';
          open(file1,filename1,error:=continue);
          if (status(file1) = 0) then
            BEGIN
              prt('Writing character sheet...',1,1);
              put_qio;
              rewrite(file1,error:=continue);
              new_page := chr(12);
              writeln(file1,new_page,error:=continue);
              cnv_stat(py.stat.cstr,out_str);
              cnv_stat(py.stat.cint,out_int);
              cnv_stat(py.stat.cwis,out_wis);
              cnv_stat(py.stat.cdex,out_dex);
              cnv_stat(py.stat.ccon,out_con);
              cnv_stat(py.stat.cchr,out_chr);
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
write(file1,  '  Name  :',pad(py.misc.name,' ',25),error:=continue);
write(file1,  '  Age         :',py.misc.age:4,error:=continue);
writeln(file1,'     Strength     :',out_str:6,error:=continue);
write(file1,  '  Qual  :',pad(py.misc.race,' ',25),error:=continue);
write(file1,  '  Height      :',py.misc.ht:4,error:=continue);
writeln(file1,'     Intelligence :',out_int:6,error:=continue);
write(file1,  '  Sex   :',pad(py.misc.sex,' ',25),error:=continue);
write(file1,  '  Weight      :',py.misc.wt:4,error:=continue);
writeln(file1,'     Wisdom       :',out_wis:6,error:=continue);
write(file1,  '  Class :',pad(py.misc.tclass,' ',25),error:=continue);
write(file1,  '  Social Class:',py.misc.sc:4,error:=continue);
writeln(file1,'     Dexterity    :',out_dex:6,error:=continue);
write(file1,  '  Title :',pad(py.misc.title,' ',25),error:=continue);
write(file1,  '               ','    ',error:=continue);
writeln(file1,'     Constitution :',out_con:6,error:=continue);
write(file1,  '         ',' ':30,error:=continue);
write(file1,  '              ',error:=continue);
writeln(file1,'     Charisma     :',out_chr:6,error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
write(file1,  '  + To Hit    :',py.misc.dis_th:6,error:=continue);
write(file1,  '     Level      :',py.misc.lev:6,error:=continue);
writeln(file1,'     Max Hit Points :',py.misc.mhp:6,error:=continue);
write(file1,  '  + To Damage :',py.misc.dis_td:6,error:=continue);
write(file1,  '     Experience :',py.misc.exp:6,error:=continue);
writeln(file1,'     Cur Hit Points :',trunc(py.misc.chp):6,error:=continue);
write(file1,  '  + To AC     :',py.misc.dis_tac:6,error:=continue);
write(file1,  '     Cash       :',py.misc.au:6,error:=continue);
writeln(file1,'     Max Resources  :',py.misc.mana:6,error:=continue);
write(file1,  '    Total AC  :',py.misc.dis_ac:6,error:=continue);
write(file1,  '                       ',error:=continue);
writeln(file1,'     Current Rsrc   :',py.misc.mana:6,error:=continue);
 
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
writeln(file1,'(Miscellaneous Abilities)':40,error:=continue);
writeln(file1,' ',error:=continue);
write(file1,  '  Melee Fights: ',pad(likert(xbth ,20,1),' ',10),error:=continue);
write(file1,  '  Stealth     : ',pad(likert(xstl ,10,1),' ',10),error:=continue);
writeln(file1,'  Perception  : ',pad(likert(xperc,10,1),' ',10),error:=continue);
write(file1,  '  Guns/Grenade: ',pad(likert(xbthb,20,1),' ',10),error:=continue);
write(file1,  '  Disarming   : ',pad(likert(xdis ,10,1),' ',10),error:=continue);
writeln(file1,'  Searching   : ',pad(likert(xsrh ,10,1),' ',10),error:=continue);
write(file1,  '  Saving Throw: ',pad(likert(xsave,10,1),' ',10),error:=continue);
write(file1,  '  Weird Device: ',pad(likert(xdev ,10,1),' ',10),error:=continue);
writeln(file1,'  Infra-Vision: ',pad(xinfra,' ',10),error:=continue);
{ Write out the character's history }
writeln(file1,' ');
writeln(file1,' ');
writeln(file1,'Character Background':45);
for i1 := 1 to 5 do writeln(file1,pad(py.misc.history[i1],' ',71):76);
{ Write out the equipment list.}
              i2 := 0;
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,'  [Character''s Equipment List]',error:=continue);
              writeln(file1,' ',error:=continue);
if (equip_ctr = 0) then
  writeln(file1,'  Character has no equipment in use.',error:=continue)
else
  for i1 := 23 to equip_max do
    with equipment[i1] do
      if (tval > 0) then
        BEGIN
          CASE i1 of
            23 :        prt1 := ') You are wielding   : ';
            24 :        prt1 := ') Worn on head       : ';
            25 :        prt1 := ') Worn around neck   : ';
            26 :        prt1 := ') Worn on body       : ';
            27 :        prt1 := ') Worn on shield arm : ';
            28 :        prt1 := ') Worn on hands      : ';
            29 :        prt1 := ') Right ring finger  : ';
            30 :        prt1 := ') Left  ring finger  : ';
            31 :        prt1 := ') Worn on feet       : ';
            32 :        prt1 := ') Worn about body    : ';
            33 :        prt1 := ') Light source is    : ';
            34 :        prt1 := ') Secondary weapon   : ';
	    35 :        prt1 := ') Computer Used      : ';
            otherwise   prt1 := ') *Unknown value*    : ';
          END;
          i2 := i2 + 1;
          objdes(prt2,i1,true,'e');
          writev(out_val,'  ',chr(i2+96),prt1,prt2);
          writeln(file1,out_val,error:=continue);
        END;
{ Write out the character's inventory.}
              writeln(file1,new_page,error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,' ',error:=continue);
              writeln(file1,'  [General Inventory List]',error:=continue);
              writeln(file1,' ',error:=continue);
if (inven_ctr = 0) then
  writeln(file1,'  Character has no objects in inventory.',error:=continue)
else
  BEGIN
    for i1 := 1 to inven_ctr do
      BEGIN
        objdes(prt1,i1,true,'i');
        writev(out_val,chr(i1+96),') ',prt1);
        writeln(file1,out_val,error:=continue);
      END
  END;
              writeln(file1,new_page,error:=continue);
              close(file1,error:=continue);
              prt('Completed.',1,1);
            END;
        END
    END;


[psect(moria$code)] procedure read_text_data( position : integer; 
					      filename :   vtype);
VAR
  num,starts,msg_lines : integer;
  infile	       : text;
  string	       : vtype;

BEGIN
  open(infile,file_name:=filename, organization:=sequential,history:=old,
       sharing:=readonly,error:=continue);
  reset(infile);
  readln(infile,num);
  If (position > num) then
    begin
       writev(string,'Error: Message number ',position:3,' in file ',filename);
       msg_print(string);
       msg_print('Please report error to your local BOSS wizard.');
    end
  Else
    begin   
      if (position = 0) then  
        position := randint(num);
      starts := 1;
      while(starts < position) do
        begin
	  readln(infile,string);
	  if (string = '**') then
	    starts := starts + 1;
        end;
      msg_lines := 0;
      readln(infile,string);
      while (string <> '**') do
      begin
        msg_lines := msg_lines + 1;
	msg_print(string);
	readln(infile,string);
      end;
      if (msg_lines = 0) then
        begin
          writev(string,
           'Error: blank message number ',position:3,' in file ',filename);
	  msg_print(string);
	  msg_print('Please report error to your local BOSS wizard.');
        end;
    end;
    close(infile);
  End;
