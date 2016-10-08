 { Print Moria credits }
[psect(misc1$code)]    procedure game_version;
BEGIN
clear(1,1);
prt('  BOSS : Beyond Moria',1,1);
prt('Version 0.1  : 11/13/90',2,1);
prt('Version 1.0  : 05/01/91',3,1);
prt('Version 2.0  : 03/01/92',4,1);
prt('Robert L. Gulledge                 Jason E. Black',5,1);
prt('raoul@u.washington.edu             cloister@u.washington.edu',6,1);
prt('Univ. of Washington                Univ. of Washington',7,1);
prt('Moria Modules :',8,1);
prt('     V1.0  Dungeon Generator      - RAK',9,1);
prt('           Character Generator    - RAK & JWT',10,1);
prt('           Moria Module           - RAK',11,1);
prt('           Miscellaneous          - RAK & JWT',12,1);
prt('     V2.0  Town Level & Misc      - RAK',13,1);
prt('     V3.0  Internal Help & Misc   - RAK',14,1);
prt('     V4.0  Source Release Version - RAK',15,1);
prt('Robert Alan Koeneke               Jimmey Wayne Todd Jr.',17,1);
prt('Student/University of Oklahoma    Student/University of Oklahoma',18,1);
prt('119 Crystal Bend                  1912 Tiffany Dr.',19,1);
prt('Norman, OK 73069                  Norman, OK  73071',20,1);
prt('(405)-321-2925                    (405) 360-6792',21,1);
        pause(24);
        draw_cave;
      end;
 
 
        { Light up the dungeon                                  -RAK-   }
[psect(wizard$code)]  procedure wizard_light;
    var
        i1,i2,i3,i4     : integer;
        flag            : boolean;
    begin
      if (cave[char_row,char_col].pl) then
        flag := false
      else
        flag := true;
      for i1 := 1 to cur_height do
        for i2 := 1 to cur_width do
          if (cave[i1,i2].fval in floor_set) then
            for i3 := i1-1 to i1+1 do
              for i4 := i2-1 to i2+1 do
                with cave[i3,i4] do
                  begin
                    pl := flag;
                    if (not(flag)) then
                      fm := false;
                  end;
      prt_map;
    end;
 
{Activates either print_monsters  or  print_objects}
[psect(wizard$code)]  procedure print_stuff_to_file;

  BEGIN
    If (get_com('Print to File:  (M)onsters or (O)bjects ?',command)) then
      case command of
        'o','O'  :  print_objects;
        'm','M'  :  print_monsters;
        otherwise;
      end;	
  END;
 
 
{ Wizard routine for gaining on stats }
[psect(wizard$code)]  procedure change_character;
    var
        tmp_val                 : integer;
        tmp_str                 : vtype;
    begin
      with py.stat do
        begin
          prt('(3 - 50) Strength     = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > 2) and (tmp_val < 51)) then
            begin
              str  := tmp_val;
              cstr := tmp_val;
              prt_strength;
              prt_max_weight;
            end;
          prt('(3 - 50) Intelligence = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > 2) and (tmp_val < 51)) then
            begin
              int  := tmp_val;
              cint := tmp_val;
              prt_intelligence;
            end;
          prt('(3 - 50) Wisdom       = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > 2) and (tmp_val < 51)) then
            begin
              wis  := tmp_val;
              cwis := tmp_val;
              prt_wisdom;
            end;
          prt('(3 - 50) Dexterity    = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > 2) and (tmp_val < 51)) then
            begin
              dex  := tmp_val;
              cdex := tmp_val;
              prt_dexterity;
            end;
          prt('(3 - 50) Constitution = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > 2) and (tmp_val < 51)) then
            begin
              con  := tmp_val;
              ccon := tmp_val;
              prt_constitution;
            end;
          prt('(3 - 50) Charisma     = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > 2) and (tmp_val < 51)) then
            begin
              chr  := tmp_val;
              cchr := tmp_val;
              prt_charisma;
            end;
        end;
      with py.misc do
        begin
          prt('(1 - 32767) Hit points = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -1;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > 0) and (tmp_val < 32768)) then
            begin
              mhp  := tmp_val;
              chp  := tmp_val;
              prt_mhp;
              prt_chp;
            end;
     if ( (py.misc.tclass <> 'Ninja') and
          (py.misc.tclass <> 'Wrestler') ) then
       begin
          prt('(0 - 32767) Resources  = ',1,1);
          get_string(tmp_str,1,26,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > -1) and (tmp_val < 32768)) then
            begin
              mana  := tmp_val;
              cmana := tmp_val;
              prt_mana;
            end;
       end;
          writev(tmp_str,'Current=',srh:1,'  (0-100) Searching = ');
          tmp_val := length(tmp_str);
          prt(tmp_str,1,1);
          get_string(tmp_str,1,tmp_val+1,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > -1) and (tmp_val < 201)) then
            srh  := tmp_val;
          writev(tmp_str,'Current=',stl:1,'  (0-10) Stealth = ');
          tmp_val := length(tmp_str);
          prt(tmp_str,1,1);
          get_string(tmp_str,1,tmp_val+1,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > -1) and (tmp_val < 11)) then
            stl  := tmp_val;
          writev(tmp_str,'Current=',disarm:1,'  (0-100) Disarming = ');
          tmp_val := length(tmp_str);
          prt(tmp_str,1,1);
          get_string(tmp_str,1,tmp_val+1,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > -1) and (tmp_val < 201)) then
            disarm := tmp_val;
          writev(tmp_str,'Current=',save:1,'  (0-100) Save = ');
          tmp_val := length(tmp_str);
          prt(tmp_str,1,1);
          get_string(tmp_str,1,tmp_val+1,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > -1) and (tmp_val < 201)) then
            save := tmp_val;
          writev(tmp_str,'Current=',bth:1,'  (0-200) Base to hit = ');
          tmp_val := length(tmp_str);
          prt(tmp_str,1,1);
          get_string(tmp_str,1,tmp_val+1,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > -1) and (tmp_val < 201)) then
            bth  := tmp_val;
          writev(tmp_str,'Current=',bthb:1,'  (0-200) Guns/Grenade = ');
          tmp_val := length(tmp_str);
          prt(tmp_str,1,1);
          get_string(tmp_str,1,tmp_val+1,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if ((tmp_val > -1) and (tmp_val < 201)) then
            bthb := tmp_val;
          writev(tmp_str,'Current=',au:1,'  Money = ');
          tmp_val := length(tmp_str);
          prt(tmp_str,1,1);
          get_string(tmp_str,1,tmp_val+1,10);
          tmp_val := -999;
          readv(tmp_str,tmp_val,error:=continue);
          if (tmp_val > -1) then
            begin
              au := tmp_val;
              prt_gold;
            end;
        end;
      erase_line(msg_line,msg_line);
      py_bonuses(blank_treasure,0);
    end;
 
        { Wizard routine for creating objects                   -RAK-   }
[psect(wizard$code)]  procedure wizard_create;
    var
        tmp_val                 : integer;
        tmp_str                 : vtype;
        flag                    : boolean;
    begin
      msg_print('Warning: This routine can cause fatal error.');
      msg_print(' ');
      msg_flag := false;
      with temporary_slot do
        begin
          prt('Name   : ',1,1);
          if (get_string(tmp_str,1,10,60)) then
            name := tmp_str
          else
            name := '& Wizard Object!';
          repeat
            prt('Tval   : ',1,1);
            get_string(tmp_str,1,10,10);
            tmp_val := 0;
            readv(tmp_str,tmp_val,error:=continue);
            flag := true;
            case tmp_val of
              1,13,15 : tchar := '~';
              2  :      tchar := '&';
              4  :      tchar := ';';
              10 :      tchar := '{';
              11 :      tchar := '{';
              12 :      tchar := '{';
              20 :      tchar := '}';
              21 :      tchar := '/';
              22 :      tchar := '\';
              23 :      tchar := '|';
              25 :      tchar := '\';
	      29 :	tchar := '%';
              30 :      tchar := ']';
              31 :      tchar := ']';
              32 :      tchar := '(';
              33 :      tchar := ']';
              34 :      tchar := ')';
              35 :      tchar := '[';
              36 :      tchar := '(';
              40 :      tchar := '"';
              45 :      tchar := '=';
              55 :      tchar := '_';
              60 :      tchar := '-';
              65 :      tchar := '-';
              70,71 :   tchar := '?';
              75,76 : tchar   := '!';
              80 :      tchar := ',';
	      82 :	tchar := '|';
	      84 :	tchar := '*';
        90,91,92 :      tchar := '?';
              otherwise flag := false;
            end;
          until (flag);
          tval := tmp_val;
          prt('Subval : ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 1;
          readv(tmp_str,tmp_val,error:=continue);
          subval := tmp_val;
          prt('Weight : ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 1;
          readv(tmp_str,tmp_val,error:=continue);
          weight := tmp_val;
          prt('Number : ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 1;
          readv(tmp_str,tmp_val,error:=continue);
          number := tmp_val;
          prt('Damage : ',1,1);
          get_string(tmp_str,1,10,5);
          damage := tmp_str;
          prt('+To hit: ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 0;
          readv(tmp_str,tmp_val,error:=continue);
          tohit := tmp_val;
          prt('+To dam: ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 0;
          readv(tmp_str,tmp_val,error:=continue);
          todam := tmp_val;
          prt('AC     : ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 0;
          readv(tmp_str,tmp_val,error:=continue);
          ac := tmp_val;
          prt('+To AC : ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 0;
          readv(tmp_str,tmp_val,error:=continue);
          toac := tmp_val;
          prt('P1     : ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 0;
          readv(tmp_str,tmp_val,error:=continue);
          p1 := tmp_val;
          prt('Flags (In HEX): ',1,1);
          flags := get_hex_value(1,17,8);
          prt('Cost : ',1,1);
          get_string(tmp_str,1,10,10);
          tmp_val := 0;
          readv(tmp_str,tmp_val,error:=continue);
          cost := tmp_val;
          if (get_com('Allocate? (Y/N)',command)) then
            case command of
                'y','Y':  begin
                            popt(tmp_val);
                            t_list[tmp_val] := temporary_slot;
                            with cave[char_row,char_col] do
                              begin
                                if (tptr > 0) then
                                  delete_object(char_row,char_col);
                                tptr := tmp_val;
                              end;
                            msg_print('Allocated...');
                          end;
                otherwise msg_print('Aborted...');
            end;
          temporary_slot := blank_treasure;
        end;
    end;

[psect(wizard$code)]  procedure make_nice_character;

        BEGIN
          with py.misc do
	    if (lev < 20) then
	      Begin
                max_exp := 7000; exp := max_exp;
                au := 30000;     
                bth := 100;      ptohit := 20;   ptodam := 20;
		pac := 100;      ptoac := 50;     
		dis_th := 20;   dis_td := 20;   dis_tac := 50;
                with py.stat do
                  begin
                    str := 30; int := 30; wis := 30; 
                    dex := 30; con := 30; chr := 30; 
                  end;
	      End
	    else
	      Begin
                max_exp := 200000; exp := max_exp;
                au := 90000;     
                bth := 200; 
                ptohit := 50;    ptodam := 50;  
		pac := 100;      ptoac := 100;
		dis_th := 50;    dis_td := 50;  dis_tac := 100;
		with py.stat do
                  begin
                    str := 50; int := 50; wis := 50;
		    dex := 50; con := 50; chr := 50; 
                  end;
                with py.flags do
                  begin
                    fire_resist := true;  cold_resist := true;
                    acid_resist := true;  regenerate  := true;
                    lght_resist := true;  free_act    := true;
                    sustain_str := true;  sustain_int := true; 
		    sustain_wis := true;  sustain_dex := true; 
		    sustain_con := true;  sustain_chr := true;
                    see_inv     := true;  ffall       := true;
                  end;
	      End;
          draw_cave;
        END;

[psect(wizard$code)]  procedure change_location;

var
    i1,i2	: integer;
    tmp_str	: vtype;

BEGIN
   prt_num('Go to which level (0-',townlist[town_num].max_depth,1,1); 
   prt(') ? ',1,24);
   get_string(tmp_str,1,29,5);
   i1 := -1;
   readv(tmp_str,i1,error:=continue);
   i2 := (i1 div 100);
   if (i1 > 99) then  {change town}
     begin
       if ((i2 <> town_num) and (i2 in [1..8])) then
         begin
           msg_print('You beam yourself to '+townlist[i2].town_name);
           msg_print('');
           town_num := i2;
           town_seed := get_seed;
           store_init;  store_maint;
           py.misc.max_lev := 0;
           office_char_row := -1; office_char_col := -1;
           moria_flag := true;
         end;
     end
   else if (i1 > -1) then
     begin
       dun_level := i1;
       if (dun_level>townlist[town_num].max_depth) then
         dun_level := townlist[town_num].max_depth;
       moria_flag := true;
     end
   else
     erase_line(msg_line,msg_line);
END;


[psect(wizard$code)]  procedure really_heal_character;
     BEGIN 
       remove_curse;
       cure_blindness;
       cure_confusion;
       cure_poison;
       remove_fear;
       py.misc.chp := py.misc.mhp;
       with py.stat do
         begin
           cstr := str;  cint := int;  cwis := wis;
           cdex := dex;  ccon := con;  cchr := chr;
         end;
       zero_pyflag;
       prt_stat_block;
     END;


[psect(wizard$code)]  procedure show_values;

      var
	i1,i2,i3,row,col			    : integer;
Begin
	clear(1,1);
	with py.misc do
	  begin 
	    i1    := trunc(expfact*100);
	    prt_num('Real +to armor   : ',ptoac,3,2);
	    prt_num('Hit Die          : ',hitdie,4,2);
	    prt_num('Exp. Factor %    : ',i1,5,2);
	    prt_num('Real +to hit     : ',ptohit,6,2);
	    prt_num('Real +to dam     : ',ptodam,7,2);
	    prt_num('Speed            : ',py.flags.speed,8,2);
	    prt_num('Base to Hit      : ',bth,3,27);
	    prt_num('BTH with Guns    : ',bthb,4,27);
	    prt_num('BaseSearching    : ',srh,5,27);
	    prt_num('BasePerception   : ',perc,6,27);
	    prt_num('BaseDisarming    : ',disarm,7,27);
	    prt_num('BaseSavingThrow  : ',save,8,27);
	    prt_num('BaseStealth      : ',stl,9,27);
	    prt_num('BaseWeird        : ',wierd,10,27);
	    prt_num('Infravision      : ',py.flags.see_infra,11,27);
	    prt_num('Melee Fights     : ',xbth,3,50);
	    prt_num('Guns/Grenades    : ',xbthb,4,50);
	    prt_num('Searching        : ',xsrh,5,50);
	    prt_num('Perception       : ',xperc,6,50);
	    prt_num('Disarming        : ',xdis,7,50);
	    prt_num('Saving Throw     : ',xsave,8,50);
	    prt_num('Stealth          : ',xstl,9,50);
	    prt_num('Wierd Device     : ',xdev,10,50);
	    prt_num('Dun_Level        : ',dun_level,12,2);
	    prt_num('Town Lev_plus    : ',townlist[town_num].lev_plus,13,2);
	    prt_num('Dun_power        : ',dun_power,14,2);
	  end;
        pause(24);
	clear(1,1);
	with py.flags do
	  begin
	        prt('Counters',3,5);
	    prt_num('Blind       : ',blind,5,5);     
	    prt_num('Confusion   : ',confused,6,5); 
	    prt_num('Paralysis   : ',paralysis,7,5); 
	    prt_num('Fear        : ',afraid,8,5);    
	    prt_num('Posion      : ',poisoned,9,5);  
	    prt_num('Hallucinate : ',image,10,5);   
	    prt_num('Prot. Evil  : ',protevil,11,5); 
	    prt_num('Invulnerable: ',invuln,12,5); 
	    prt_num('Blessed     : ',blessed,13,5);  
	    prt_num('Detect Inv. : ',detect_inv,14,5);
	    prt_num('Resist Heat : ',resist_heat,15,5);
	    prt_num('Resist Cold : ',resist_cold,16,5);
	        prt('More Counters',3,25);
	    prt_num('Food        : ',food,5,25);
	    prt_num('Food Rate   : ',food_digested,6,25);
	    prt_num('Fast        : ',fast,7,25);
	    prt_num('Slow        : ',slow,8,25);
	    prt_num('Hero        : ',hero,9,25);	
	    prt_num('Super Hero  : ',shero,10,25);
	    prt_num('Recall      : ',word_recall,11,25);
	    prt_num('Infravsion  : ',tim_infra,12,25);
	    prt_num('Ring Blast  : ',ring_blast,13,25);
	    prt_num('Special     : ',special,14,25);
	    prt_num('Petrified   : ',petrified,15,25);
	    prt_num('Diseased    : ',diseased,16,25);
	  end;
	pause(24);
	clear(1,1);
	i1 := muptr;
	i3 := 0;
	while ((i1 <> 0) and (i3 < 60)) do
	  begin
	    prt(c_list[m_list[i1].mptr].name,(i3 mod 20)+2,(i3 div 20)*27 + 1);
	    i1 := m_list[i1].nptr;
	    i3 := i3 + 1;
	  end;
	pause(24);
        draw_cave;
  End;

[psect(wizard$code)]  procedure item_stats;

var
   i1			: unsigned;
   item_val,counter,i2	: integer;
   redraw		: boolean;
   out_val		: vtype;

 BEGIN     	
   redraw := false;
   if (get_item(item_val,'Examine which item?',redraw,1,inven_ctr)) then
     with inventory[item_val] do
       begin
         clear(1,1);
	 redraw := true;
	 objdes(out_val,item_val,true,'i');
	     prt('Item : '+name,2,1);
	     prt('Desc : '+out_val,3,1);
	 prt_num('Type Value   : ',tval	   ,4,5);
             prt('Character    : '+tchar   ,5,5);        
	 prt_num('P1 Value     : ',p1	   ,6,5);
	 prt_num('Price        : ',cost	   ,7,5);
	 prt_num('Sub Value    : ',subval  ,8,5);
	 prt_num('Weight       : ',weight  ,9,5);
	 prt_num('Number       : ',number ,10,5);
	 prt_num('To hit       : ',tohit  ,11,5);
	 prt_num('To Damage    : ',todam  ,12,5);
	 prt_num('Base AC      : ',ac	  ,13,5);
	 prt_num('Plus to AC   : ',toac	  ,14,5);
	     prt('Damage       : '+damage ,15,5);
         prt_num('Power Level  : ',level  ,16,5);
	 prt('Flags:',5,30);
	 i1 := flags;
	 counter := 5;
	 while (i1 > 0) do
	   begin
	     counter := counter+1;
	     i2 := bit_pos(i1);
	     prt_num('',i2,counter,31);	 
	   end;	   	 
	 pause(24);
       end;
   if (redraw) then draw_cave;
 END;


[psect(wizard$code)]  procedure beckon_creature;
  VAR
       monster,y,x	: integer;
       tmp_str		: vtype;	 

  BEGIN
    prt_num('Summon Which Creature (1-',max_creatures,1,1); 
    prt(') ? ',1,29);
    get_string(tmp_str,1,33,4);
    monster := -1;
    readv(tmp_str,monster,error:=continue);
    if (monster > 0) then
      Begin
        repeat
          y := char_row + randint(10) - 5;
          x := char_col + randint(10) - 5;
        until ((cave[y,x].cptr = 0) and (cave[y,x].fopen));
 	place_monster(y,x,monster,true);
	creatures(false);
      End;
  END;

