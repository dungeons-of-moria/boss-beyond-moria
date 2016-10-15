{ Attempt to open the intro file }
procedure intro(const finam : vtype);
      var
        {xpos,}i1 : integer;
        day_test,in_line: vtype;
        file1   : text;
        file2   : file of key_type;
        exit_flag       : boolean;
      BEGIN
        exit_flag := false;
        clear(1,1);
{ Attempt to read hours.dat. If it does not exist, then create a standard one.}
	assign(file1,BOSS_HOU);
        reset(file1);
        if IOResult = 0 then
          BEGIN
            repeat
              readln(file1,in_line);
              if (length(in_line) > 3) then
                BEGIN
                  day_test := copy(in_line,1,4);
                  if      (day_test = 'SUN:') then days[1] := in_line
                  else if (day_test = 'MON:') then days[2] := in_line
                  else if (day_test = 'TUE:') then days[3] := in_line
                  else if (day_test = 'WED:') then days[4] := in_line
                  else if (day_test = 'THU:') then days[5] := in_line
                  else if (day_test = 'FRI:') then days[6] := in_line
                  else if (day_test = 'SAT:') then days[7] := in_line;
                END;
            until(eof(file1));
            close(file1);
          END
        else    { Create a standard hours file }
          BEGIN
            assign(file1,BOSS_HOU);
            rewrite(file1);
            if IOResult = 0 then
              BEGIN
                writeln(file1,'    Boss operating hours are:');
                writeln(file1,'    |    AM     |    PM     |');
                writeln(file1,'    1         111         111');
                writeln(file1,'    2123456789012123456789012');
                for i1 := 1 to 7 do
                  writeln(file1,days[i1]);
                writeln(file1,'       (X=Open; .=Closed)');
                close(file1);
                writeln('Created ',BOSS_HOU);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_HOU);
                halt(0)
              END;
          END;
 
{ Check the hours, if closed then require password }
        if (not(exit_flag)) then
          BEGIN
            //FIXME?
            //xpos := pos('/WIZARD',finam);
            //if ( xpos > 0) then
{ Opusii }   //if (validuser) then 
              //insert_str(finam,'/WIZARD','');
{ No time checks! -MB }
{
            if (not(check_time)) then
                BEGIN
                  assign(file1,BOSS_HOU);
                  reset(file1);
                  if IOResult = 0 then
                    BEGIN
                      repeat
                        readln(file1,in_line);
                        writeln(in_line);
                      until(eof(file1));
                      close(file1);
                    END;
                  halt(0)
                END;
}
          END;
 
{ Print the introduction message, news, ect.}
        full_clear;
        assign(file1,BOSS_MOR);
        reset(file1);
        if IOResult = 0 then
          BEGIN
            clear(1,1);
            repeat
              readln(file1,in_line);
              writeln(in_line)
            until (eof(file1));
            pause_exit(24);
            close(file1)
          END
        else   { Create one.}
          BEGIN
            assign(file1,BOSS_MOR);
            rewrite(file1);
            if IOResult = 0 then
              BEGIN
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
                close(file1);
                writeln('Created ',BOSS_MOR);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_MOR);
                halt(0)
              END;
          END;
{ Check for MASTER.DAT }
        assign(file2,BOSS_MAS);
        reset(file2);
        if IOResult <> 0 then
          BEGIN
            rewrite(file2);
            if IOResult = 0 then
              BEGIN
                writeln('Created ',BOSS_MAS);
                close(file2);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_MAS);
                halt(0)
              END;
          END
        else
          BEGIN
            close(file2);
          END;
{ Check for TOPTWENTY.DAT }
        assign(file1,BOSS_TOP);
        reset(file1);
        if IOResult <> 0 then
          BEGIN
            rewrite(file1);
            if IOResult = 0 then
              BEGIN
                writeln('Created ',BOSS_TOP);
                close(file1);
                exit_flag := true;
              END
            else
              BEGIN
                writeln('Error in creating ',BOSS_TOP);
                halt(0)
              END;
          END
        else
          BEGIN
            close(file1);
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
            halt(0)
          END;
      END;
 
 
{ Prints dungeon map to external file }
procedure print_map;
    var
        i1,i2,i3,i4,i5,i6,i7,i8 : integer;
        dun_line: string[133];
        filename1       : string[80];
        tmp     : char;
        file1   : text;
    BEGIN
      prt('File name: ',1,1);
      if (get_string(filename1,1,12,64)) then
        BEGIN
          if (length(filename1) = 0) then filename1 := 'BOSSMAP.DAT';
          assign(file1,filename1);
          rewrite(file1);
          if IOResult = 0 then
            BEGIN
              prt('Writing Boss Dungeon Map...',1,1);
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
                  writeln(file1,chr(12));
                  write(file1,'Section[',i7:1,',',i8:1,'];     ');
                  writeln(file1,'Depth : ',(dun_level*50):1,' (meters)');
                  writeln(file1,' ');
                  write(file1,'   ');
                  for i5 := i2 to i4 do
                    BEGIN
                      i6 := trunc(i5/100);
                      write(file1,i6:1);
                    END;
                  writeln(file1);
                  write(file1,'   ');
                  for i5 := i2 to i4 do
                    BEGIN
                      i6 := trunc(i5/10) - trunc(i5/100)*10;
                      write(file1,i6:1);
                    END;
                  writeln(file1);
                  write(file1,'   ');
                  for i5 := i2 to i4 do
                    BEGIN
                      i6 := i5 - trunc(i5/10)*10;
                      write(file1,i6:1);
                    END;
                  writeln(file1);
                  for i5 := i1 to i3 do
                    BEGIN
                      dun_line := padleft(inttostr(i5),3);
                      for i6 := i2 to i4 do
                        BEGIN
                          if (test_light(i5,i6)) then
                            loc_symbol(i5,i6,tmp)
                          else
                            tmp := ' ';
                          dun_line := dun_line + tmp;
                        END;
                      writeln(file1,dun_line);
                    END;
                  i2 := i2 + outpage_width;
                until (i2 >= cur_width);
                i1 := i1 + outpage_height;
              until (i1 >= cur_height);
              close(file1);
              prt('Completed.',1,1);
            END;
        END
    END;
 
 
{ Prints a list of random objects to a file.  Note that the object file
  is a sampling of objects which be expected to appear on that level. }
procedure print_objects;
    var
        nobj,i1,i2,level : integer;
        filename1,tmp_str : string[80];
        file1   : text;
    BEGIN
      prt('Produce objects on what level?: ',1,1);
      get_string(tmp_str,1,33,10);
      level := 0;
      level := strtoint(tmp_str);
      prt('Produce how many objects?: ',1,1);
      get_string(tmp_str,1,28,10);
      nobj := 0;
      nobj := strtoint(tmp_str);
      if ((nobj > 0) and (level > -1) and (level < 1201)) then
        BEGIN
          if (nobj > 9999) then nobj := 9999;
          prt('File name: ',1,1);
          if (get_string(filename1,1,12,64)) then
            BEGIN
              if (length(filename1) = 0) then filename1 := 'BOSSOBJ.DAT';
              assign(file1,filename1);
              rewrite(file1);
              if IOResult = 0 then
                BEGIN
                  tmp_str := inttostr(nobj);
                  prt(tmp_str + ' random objects being produced...',1,1);
                  writeln(file1,'*** Random Object Sampling:');
                  writeln(file1,'*** ',nobj:1,' objects');
                  writeln(file1,'*** For Level ',level:1);
                  writeln(file1,'');
                  writeln(file1,'');
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
                      writeln(file1,tmp_str);
                    END;
                  pusht(i2);
                  close(file1);
                  prt('Completed.',1,1);
                END
              else
                prt('File could not be created.',1,1);
            END;
        END;
    END;
 
 
{ Prints a listing of monsters }
procedure print_monsters;
    var
        i1,i2,xpos : integer;
	atype, adesc : system.integer; { sscanf secure }
        file1   : text;
        out_val,new_val,filename1       : vtype;
        attstr,attx     : vtype;
        damstr : ansistring;
 
    BEGIN
      prt('File name: ',1,1);
      if (get_string(filename1,1,12,64)) then
        BEGIN
          if (length(filename1) = 0) then filename1 := 'BOSSMON.DAT';
          assign(file1,filename1);
          rewrite(file1);
          if IOResult = 0 then
            BEGIN
              prt('Writing Monster Dictionary...',1,1);
              for i1 := 1 to max_creatures do
                with c_list[i1] do
                  BEGIN
{ BEGIN writing to file }
writeln(file1,'--------------------------------------------');
out_val := name + '                              ';
writeln(file1,i1:3,'  ',out_val:30,'     (',cchar:1,')');
writeln(file1,'     Speed =',speed:2,'  Level     =',level:2,'  Exp =',mexp:5);
writeln(file1,'     AC    =',ac:2,   '  Eye-sight =',aaf:2,'  HD  =',hd:5);
if (($80000000 and cmove) <> 0) then
  writeln(file1,'     Creature is a ***Win Creature***');
if (($00080000 and cmove) <> 0) then
  writeln(file1,'     Creature Eats/kills other creatures.');
if (($0001 and cdefense) <> 0) then
  writeln(file1,'     Creature is a dragon.');
if (($0002 and cdefense) <> 0) then
  writeln(file1,'     Creature is a monster.');
if (($0004 and cdefense) <> 0) then
  writeln(file1,'     Creature is evil.');
if (($0008 and cdefense) <> 0) then
  writeln(file1,'     Creature is undead.');
if (($0010 and cdefense) <> 0) then
  writeln(file1,'     Creature harmed by cold.');
if (($0020 and cdefense) <> 0) then
  writeln(file1,'     Creature harmed by fire.');
if (($0040 and cdefense) <> 0) then
  writeln(file1,'     Creature harmed by poison.');
if (($0080 and cdefense) <> 0) then
  writeln(file1,'     Creature harmed by acid.');
if (($0100 and cdefense) <> 0) then
  writeln(file1,'     Creature harmed by blue light.');
if (($0200 and cdefense) <> 0) then
  writeln(file1,'     Creature harmed by Stone-to-Mud.');
if (($0400 and cdefense) <> 0) then
  writeln(file1,'     Creature harmed by Kill Robot.');
if (($0800 and cdefense) <> 0) then
  writeln(file1,'     Negative Experience if Killed.');
if (($1000 and cdefense) <> 0) then
  writeln(file1,'     Creature cannot be charmed or slept.');
if (($2000 and cdefense) <> 0) then
  writeln(file1,'     Creature seen with Infra-Vision.');
if (($4000 and cdefense) <> 0) then
  writeln(file1,'     Creature has MAX hit points.');
if (($00010000 and cmove) <> 0) then
  writeln(file1,'     Creature is invisible.');
if (($00100000 and cmove) <> 0) then
  writeln(file1,'     Creature picks up objects.');
if (($00200000 and cmove) <> 0) then
  writeln(file1,'     Creature multiplies.');
if (($01000000 and cmove) <> 0) then
  writeln(file1,'     Carries object(s).');
if (($02000000 and cmove) <> 0) then
  writeln(file1,'     Carries gold, gems, ect.');
if (($04000000 and cmove) <> 0) then
  writeln(file1,'       Has object/gold 60% of time.');
if (($08000000 and cmove) <> 0) then
  writeln(file1,'       Has object/gold 90% of time.');
if (($10000000 and cmove) <> 0) then
  writeln(file1,'       Has 1d2 object(s)/gold.');
if (($20000000 and cmove) <> 0) then
  writeln(file1,'       Has 2d2 object(s)/gold.');
if (($40000000 and cmove) <> 0) then
  writeln(file1,'       Has 4d2 object(s)/gold.');
{ Creature casts spells / Breathes Dragon breath.}
if (spells > 0) then
  BEGIN
    writeln(file1,'   --Spells/Dragon Breath =');
    writeln(file1,'       Casts spells 1 out of ',($F and spells):1,
                ' turns.');
    if (($00000010 and spells) <> 0) then
      writeln(file1,'       Can teleport short.');
    if (($00000020 and spells) <> 0) then
      writeln(file1,'       Can teleport long.');
    if (($00000040 and spells) <> 0) then
      writeln(file1,'       Teleport player to itself.');
    if (($00000080 and spells) <> 0) then
      writeln(file1,'       Cause light wounds.');
    if (($00000100 and spells) <> 0) then
      writeln(file1,'       Cause serious wounds.');
    if (($00000200 and spells) <> 0) then
      writeln(file1,'       Hold person.');
    if (($00000400 and spells) <> 0) then
      writeln(file1,'       Cause blindness.');
    if (($00000800 and spells) <> 0) then
      writeln(file1,'       Cause confusion.');
    if (($00001000 and spells) <> 0) then
      writeln(file1,'       Cause fear.');
    if (($00002000 and spells) <> 0) then
      writeln(file1,'       Summon a monster.');
    if (($00004000 and spells) <> 0) then
      writeln(file1,'       Summon an undead.');
    if (($00008000 and spells) <> 0) then
      writeln(file1,'       Slow person.');
    if (($00010000 and spells) <> 0) then
      writeln(file1,'       Drains mana for healing.');
    if (($00020000 and spells) <> 0) then
      writeln(file1,'       **Unknown spell value**');
    if (($00040000 and spells) <> 0) then
      writeln(file1,'       **Unknown spell value**');
    if (($00080000 and spells) <> 0) then
      writeln(file1,'       Breathes Lightning Dragon Breath.');
    if (($00100000 and spells) <> 0) then
      writeln(file1,'       Breathes Gas Dragon Breath.');
    if (($00200000 and spells) <> 0) then
      writeln(file1,'       Breathes Acid Dragon Breath.');
    if (($00400000 and spells) <> 0) then
      writeln(file1,'       Breathes Frost Dragon Breath.');
    if (($00800000 and spells) <> 0) then
      writeln(file1,'       Breathes Fire Dragon Breath.');
  END;
{ Movement for creature }
writeln(file1,'   --Movement =');
if (($00000001 and cmove) <> 0) then
  writeln(file1,'       Move only to attack.');
if (($00000002 and cmove) <> 0) then
  writeln(file1,'       Move and attack normally.');
if (($00000008 and cmove) <> 0) then
  writeln(file1,'       20% random movement.');
if (($00000010 and cmove) <> 0) then
  writeln(file1,'       40% random movement.');
if (($00000020 and cmove) <> 0) then
  writeln(file1,'       75% random movement.');
if (($00020000 and cmove) <> 0) then
  writeln(file1,'       Can open doors.');
if (($00040000 and cmove) <> 0) then
  writeln(file1,'       Can phase through walls.');
writeln(file1,'   --Creature attacks =');
attstr := damage;
while (length(attstr) > 0) do
  BEGIN
    xpos := pos('|',attstr);
    if (xpos > 0) then
      BEGIN
        attx := copy(attstr,1,xpos-1);
        attstr := copy(attstr,xpos+1,length(attstr)-xpos);
      END
    else
      BEGIN
        attx := attstr;
        attstr := '';
      END;
    sscanf(attx, '%d%d%s', [@atype, @adesc, @damstr]);
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
    writeln(file1,out_val);
  END;
  for i2 := 1 to 2 do writeln(file1,' ');
END;
{ end writing to file }
              close(file1);
              prt('Completed.',1,1);
            END;
        END
    END;
 
 
{ Print the character to a file or device }
procedure character_print;
    var
        i1,i2: integer;
        file1   : text;
        out_val,filename1,prt1,prt2 : vtype;
        out_str,out_int,out_wis,out_dex,out_con,out_chr : stat_type;
 
    BEGIN
      filename1 := py.misc.name + '.txt';
          assign(file1,filename1);
{ Append. There is tombstone already in the file and perhaps a crown. -MB }
          append(file1);
          if IOResult = 0 then
            BEGIN
              prt('Writing character sheet...',1,1);
cnv_stat(py.stat.cstr,out_str);
cnv_stat(py.stat.cint,out_int);
cnv_stat(py.stat.cwis,out_wis);
cnv_stat(py.stat.cdex,out_dex);
cnv_stat(py.stat.ccon,out_con);
cnv_stat(py.stat.cchr,out_chr);
writeln(file1);
writeln(file1);
writeln(file1);
write(file1,  '  Name  :',padleft(py.misc.name,25));
write(file1,  '  Age         :',py.misc.age:4);
writeln(file1,'     Strength     :',out_str:6);
write(file1,  '  Qual  :',padleft(py.misc.race,25));
write(file1,  '  Height      :',py.misc.ht:4);
writeln(file1,'     Intelligence :',out_int:6);
write(file1,  '  Sex   :',padleft(py.misc.sex,25));
write(file1,  '  Weight      :',py.misc.wt:4);
writeln(file1,'     Wisdom       :',out_wis:6);
write(file1,  '  Class :',padleft(py.misc.tclass,25));
write(file1,  '  Social Class:',py.misc.sc:4);
writeln(file1,'     Dexterity    :',out_dex:6);
write(file1,  '  Title :',padleft(py.misc.title,25));
write(file1,  '               ','    ');
writeln(file1,'     Constitution :',out_con:6);
write(file1,  '         ',' ':30);
write(file1,  '              ');
writeln(file1,'     Charisma     :',out_chr:6);
writeln(file1);
writeln(file1);
writeln(file1);
writeln(file1);
write(file1,  '  + To Hit    :',py.misc.dis_th:6);
write(file1,  '     Level      :',py.misc.lev:6);
writeln(file1,'     Max Hit Points :',py.misc.mhp:6);
write(file1,  '  + To Damage :',py.misc.dis_td:6);
write(file1,  '     Experience :',py.misc.exp:6);
writeln(file1,'     Cur Hit Points :',trunc(py.misc.chp):6);
write(file1,  '  + To AC     :',py.misc.dis_tac:6);
write(file1,  '     Cash       :',py.misc.au:6);
writeln(file1,'     Max Resources  :',py.misc.mana:6);
write(file1,  '    Total AC  :',py.misc.dis_ac:6);
write(file1,  '                       ');
writeln(file1,'     Current Rsrc   :',py.misc.mana:6);
writeln(file1);
writeln(file1);
writeln(file1,'(Miscellaneous Abilities)':40);
writeln(file1,' ');
write(file1,  '  Melee Fights: ',padleft(likert(xbth ,20,1),10));
write(file1,  '  Stealth     : ',padleft(likert(xstl ,10,1),10));
writeln(file1,'  Perception  : ',padleft(likert(xperc,10,1),10));
write(file1,  '  Guns/Grenade: ',padleft(likert(xbthb,20,1),10));
write(file1,  '  Disarming   : ',padleft(likert(xdis ,10,1),10));
writeln(file1,'  Searching   : ',padleft(likert(xsrh ,10,1),10));
write(file1,  '  Saving Throw: ',padleft(likert(xsave,10,1),10));
write(file1,  '  Weird Device: ',padleft(likert(xdev ,10,1),10));
writeln(file1,'  Infra-Vision: ',padleft(xinfra,10));
{ Write out the character's history }
writeln(file1);
writeln(file1);
writeln(file1,'Character Background':45);
for i1 := 1 to 5 do writeln(file1,padleft(py.misc.history[i1],71):76);
{ Write out the equipment list.}
i2 := 0;
writeln(file1);
writeln(file1);
writeln(file1,'  [Character''s Equipment List]');
writeln(file1);
if (equip_ctr = 0) then
  writeln(file1,'  Character has no equipment in use.')
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
          out_val := '  ' + chr(i2+96) + prt1 + prt2;
          writeln(file1,out_val);
        END;
{ Write out the character's inventory.}
              writeln(file1);
              writeln(file1);
              writeln(file1);
              writeln(file1,'  [General Inventory List]');
              writeln(file1);
if (inven_ctr = 0) then
  writeln(file1,'  Character has no objects in inventory.')
else
  BEGIN
    for i1 := 1 to inven_ctr do
      BEGIN
        objdes(prt1,i1,true,'i');
        out_val := chr(i1+96) + ') ' + prt1;
        writeln(file1,out_val);
      END
  END;
              close(file1);
              prt('Completed.',1,1);
            END
    END;


procedure read_text_data( position : integer; 
					      filename :   vtype);
VAR
  num,starts,msg_lines : integer;
  infile	       : text;
  astring	       : vtype;

BEGIN
  assign(infile,filename);
  reset(infile);
  readln(infile,num);
  If (position > num) then
    begin
       astring := 'Error: Message number ' + padleft(inttostr(position),3) + 
         ' in file ' + filename;
       msg_print(astring);
       msg_print('Please report error to your local BOSS wizard.');
    end
  Else
    begin   
      if (position = 0) then  
        position := randint(num);
      starts := 1;
      while(starts < position) do
        begin
	  readln(infile,astring);
	  if (astring = '**') then
	    starts := starts + 1;
        end;
      msg_lines := 0;
      readln(infile,astring);
      while (astring <> '**') do
      begin
        msg_lines := msg_lines + 1;
	msg_print(astring);
	readln(infile,astring);
      end;
      if (msg_lines = 0) then
        begin
          astring := 'Error: blank message number ' + 
            padleft(inttostr(position),3) + ' in file ' + filename;
	  msg_print(astring);
	  msg_print('Please report error to your local BOSS wizard.');
        end;
    end;
    close(infile);
  End;
