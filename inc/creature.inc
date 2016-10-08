{ Creatures movement and attacking are done from here }
  procedure creatures;
      VAR
           i1,i2,i3,moldy,moldx    : integer;
 
 
{ Updates screen when monsters move about  }
      PROCEDURE update_mon(monptr : integer);
        VAR
                flag : boolean;
        BEGIN
          with m_list[monptr] do
            with cave[fy,fx] do
              BEGIN
                flag := false;
                if (cdis <= max_sight) then   { all this stuff means: can you }
                  if (py.flags.blind < 1) then       { see the monster?       }
                    if (panel_contains(fy,fx)) then
                      if (los(char_row,char_col,fy,fx)) then
                        with c_list[mptr] do
                          BEGIN		  	{we can see him?  cool! }
{ Normal sight. }
                            if ((pl) or (tl)) then
                              BEGIN
                                if (py.flags.see_inv) then
                                  flag := true
                                else if (uand(%X'10000',cmove) = 0) then
                                  flag := true
                              END
{ Infra vision. }
                              else if (py.flags.see_infra > 0) then
                                if (cdis <= py.flags.see_infra) then
                                  if (uand(%X'2000',cdefense) <> 0) then
                                    flag := true
                          END;
{ Light it up. }
                if (flag) then
                  BEGIN
                    if (not(ml)) then
                      BEGIN
                        print(c_list[mptr].cchar,fy,fx);
                        ml := true;
                        if (search_flag) then
                          search_off;
                        if (py.flags.rest > 0) then
                          rest_off;
                        flush;
                        if (find_flag) then
                          BEGIN
                            find_flag := false;
                            move_char(5)
                          END
                      END
                  END
{ Turn it off.}
                else if (ml) then
                  BEGIN
                    ml := false;
                    if ((tl) or (pl)) then
                      lite_spot(fy,fx)
                    else
                      unlite_spot(fy,fx)
                  END
              END
        END;
 
 
{ Move the critters about the dungeon }
      FUNCTION mon_move(monptr : integer) : boolean;
        TYPE
                mm_type = array [1..5] of integer;
        VAR
                i1,i2,i3 : integer;
                mm : mm_type;
                out_val : vtype;
                move_test : boolean;
 
{ Choose correct directions for monster movement }
        PROCEDURE get_moves(monptr : integer; var mm : mm_type);
          VAR
                y,ay,x,ax,move_val : integer;
          BEGIN
            y := m_list[monptr].fy - char_row;
            x := m_list[monptr].fx - char_col;
            ay := abs(y);
            ax := abs(x);
            if (y < 0) then
              move_val := 8
            else
              move_val := 0;
            if (x > 0) then
              move_val := move_val + 4;
            if (ay > (ax*1.7321)) then   { pi/2, in case you were wondering }
              move_val := move_val + 2
            else if (ax > (ay*1.7321)) then
              move_val := move_val + 1;
            CASE move_val of
              0     :   BEGIN
                          mm[1] := 9;
                          if (ay > ax) then
                            BEGIN
                              mm[2] := 8;
                              mm[3] := 6;
                              mm[4] := 7;
                              mm[5] := 3
                            END
                          else
                            BEGIN
                              mm[2] := 6;
                              mm[3] := 8;
                              mm[4] := 3;
                              mm[5] := 7
                            END
                        END;
              1,9   :   BEGIN
                          mm[1] := 6;
                          if (y < 0) then
                            BEGIN
                              mm[2] := 3;
                              mm[3] := 9;
                              mm[4] := 2;
                              mm[5] := 8
                            END
                          else
                            BEGIN
                              mm[2] := 9;
                              mm[3] := 3;
                              mm[4] := 8;
                              mm[5] := 2
                            END
                        END;
              2,6   :   BEGIN
                          mm[1] := 8;
                          if (x < 0) then
                            BEGIN
                              mm[2] := 9;
                              mm[3] := 7;
                              mm[4] := 6;
                              mm[5] := 4
                            END
                          else
                            BEGIN
                              mm[2] := 7;
                              mm[3] := 9;
                              mm[4] := 4;
                              mm[5] := 6
                            END
                        END;
              4     :   BEGIN
                          mm[1] := 7;
                          if (ay > ax) then
                            BEGIN
                              mm[2] := 8;
                              mm[3] := 4;
                              mm[4] := 9;
                              mm[5] := 1
                            END
                          else
                            BEGIN
                              mm[2] := 4;
                              mm[3] := 8;
                              mm[4] := 1;
                              mm[5] := 9
                            END
                        END;
              5,13  :   BEGIN
                          mm[1] := 4;
                          if (y < 0) then
                            BEGIN
                              mm[2] := 1;
                              mm[3] := 7;
                              mm[4] := 8;
                              mm[5] := 2
                            END
                          else
                            BEGIN
                              mm[2] := 7;
                              mm[3] := 1;
                              mm[4] := 2;
                              mm[5] := 8
                            END
                        END;
              8     :   BEGIN
                          mm[1] := 3;
                          if (ay > ax) then
                            BEGIN
                              mm[2] := 2;
                              mm[3] := 6;
                              mm[4] := 1;
                              mm[5] := 9
                            END
                          else
                            BEGIN
                              mm[2] := 6;
                              mm[3] := 2;
                              mm[4] := 9;
                              mm[5] := 1
                            END
                        END;
              10,14 :   BEGIN
                          mm[1] := 2;
                          if (x < 0) then
                            BEGIN
                              mm[2] := 1;
                              mm[3] := 3;
                              mm[4] := 4;
                              mm[5] := 6
                            END
                          else
                            BEGIN
                              mm[2] := 3;
                              mm[3] := 1;
                              mm[4] := 6;
                              mm[5] := 4
                            END
                        END;
              12    :   BEGIN
                          mm[1] := 1;
                          if (ay > ax) then
                            BEGIN
                              mm[2] := 2;
                              mm[3] := 4;
                              mm[4] := 3;
                              mm[5] := 7
                            END
                          else
                            BEGIN
                              mm[2] := 4;
                              mm[3] := 2;
                              mm[4] := 7;
                              mm[5] := 3
                            END
                        END
            END
          END;
 
 
{ Make an attack on the player (chuckle...) }
        PROCEDURE make_attack(monptr : integer);
          VAR
                xpos,atype,adesc,dam,i1,i2,i3,i4,i7 : integer;
                attstr,attx,cdesc,mdesc,ddesc : vtype;
                damstr : dtype;
                flag : boolean;
          BEGIN
            with m_list[monptr] do
            with c_list[mptr] do
              BEGIN
                attstr := damage;
                if ((uand(%X'10000',cmove) <> 0) and
                    (not(py.flags.see_inv)))then
                  cdesc := 'It '
                else if (py.flags.blind > 0) then
                  cdesc := 'It '
                else if (not(ml)) then
                  cdesc := 'It '
                else
                  cdesc := 'The ' + name + ' ';
{ For 'DIED_FROM' string }
                if (uand(%X'80000000',cmove) <> 0) then
                  ddesc := 'The ' + name
                else
                  ddesc := '& ' + name;
                temporary_slot.name := ddesc;
                temporary_slot.number := 1;
                objdes(ddesc,1,true,'t');
{ End DIED_FROM }
                while (length(attstr) > 0) do
                  BEGIN
                    xpos := index(attstr,'|');
                    if (xpos > 0) then
                      BEGIN
                        attx := substr(attstr,1,xpos-1);
                        attstr := substr(attstr,xpos+1,length(attstr)-xpos)
                      END
                    else
                      BEGIN
                        attx := attstr;
                        attstr := ''
                      END;
                    readv(attx,atype,adesc,damstr);
                    flag := false;
                    if (py.flags.protevil > 0) then
                      if (uand(cdefense,%X'0004') <> 0) then
                        if ((py.misc.lev+1) > level) then
                          BEGIN
                            atype := 99;
                            adesc := max_attack_desc;
                          END;
                    with py.misc do
                    CASE atype of  { hit conditions for various attacks }
{Normal attack }     1  :      if (test_hit(60+intel,level,0,pac+ptoac)) then
                                  flag := true;
{Poison Strength}    2  :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Confusion attack}   3  :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Fear attack }       4  :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Fire attack }       5  :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Acid attack }       6  :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Cold attack }       7  :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Lightning attack}   8  :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Corrosion attack}   9  :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Blindness attack}   10 :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Paralysis attack}   11 :      if (test_hit(4,level,0,pac+ptoac)) then
                                  flag := true;
{Steal Money }       12 :      if (test_hit(8,level,0,py.misc.lev)) then
                                  if (py.misc.au > 0) then
                                    flag := true;
{Steal Object }      13 :      if (test_hit(5,level,0,py.misc.lev)) then
                                  if (inven_ctr > 0) then
                                    flag := true;
{Poison }            14 :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Lose dexterity}     15 :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Lose constitution}  16 :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Lose intelligence}  17 :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Lose wisdom}        18 :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Lose experience}    19 :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Aggravate monsters} 20 :      flag := true;
{Disenchant }        21 :      if (test_hit(7,level,0,pac+ptoac)) then
                                  flag := true;
{Eat food }          22 :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Eat light }         23 :      if (test_hit(0,level,0,pac+ptoac)) then
                                  flag := true;
{Eat charges }       24 :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Vampire bite }      25 :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Turn to stone }     26 :      if (test_hit(10,level,0,pac+ptoac)) then
                                  flag := true;
{Vargouillian bite } 29 :      if (test_hit(3,level,0,pac+ptoac)) then 
				  flag := true;
{Cause disease}	     30 :      if ((randint(124) > py.stat.ccon) or
				   (randint(100) > 95)) then
			          flag := true;
{Lose charisma}	     31 :      if (test_hit(0,level,0,pac+ptoac)) then
			          flag := true;
                     99 :      flag := true;
                      OTHERWISE
                    END;
                    if (flag) then
                      BEGIN   
		        if (adesc <> 19) then
			  msg_print(cdesc + attack_desc[adesc])
			else
{Eddie attacks}           Begin
                            if (py.flags.see_inv) then
                              i7 := 11
                            else
                              i7 := 7;
			     case randint(i7) of
			      1: msg_print(cdesc+'insults you.');
			      2: msg_print(cdesc+'insults your mother.');
			      3: msg_print(cdesc+'wets on your leg.');
			      4: msg_print(cdesc+'defiles you.');
			      5: msg_print(cdesc+'humiliates you.');
			      6: msg_print(cdesc+'laughs at you.');
			      7: msg_print(cdesc+'curses you!');
			      8: msg_print(cdesc+'moons you!');
			      9: msg_print(cdesc+'gives you the finger.');
			     10: msg_print(cdesc+'makes obscene gestures.');
			     11: msg_print(cdesc+'dances around you.');
                            end
			  End;
                        CASE atype of
{Normal attack  }         1  :  BEGIN
                                  dam := damroll(damstr);
                                  with py.misc do
                                    dam :=dam - round(((pac+ptoac)/300.0)*dam);
                                  take_hit(dam,ddesc);
                                  prt_chp
                                END;
{Poison Strength}         2  :  BEGIN
                                  take_hit(damroll(damstr),ddesc);
                                  if (py.flags.sustain_str) then
                msg_print('You feel weaker for a moment, then it passes.')
                                    else if (randint(2) = 1) then
                                      BEGIN
                                        msg_print('You feel weaker.');
                                        py.stat.cstr := de_statp(py.stat.cstr);
                                        prt_strength
                                      END;
                                  prt_chp
                                END;
{Confusion attack}        3  :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (randint(50) > py.stat.cint) then
                                      BEGIN
                                        if (confused < 1) then
                                          BEGIN
                                            msg_print('You feel confused.');
                                            confused:=confused+randint(intel);
                                          END;
                                        confused := confused + 3;
                                      END;
                                    prt_chp;
                                  END;
{Fear attack }            4  :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (player_saves(wis_adj)) then
                                      msg_print('You resist the effects!')
                                    else if (afraid < 1) then
                                      BEGIN
                                        msg_print('You are suddenly afraid!');
                                        afraid := afraid + 3 + randint(level)
                                      END
                                    else
                                      afraid := afraid + 3;
                                    prt_chp
                                  END;
{Fire attack }            5  :  BEGIN
                                  msg_print('You are enveloped in flames!');
                                  fire_dam(damroll(damstr),ddesc)
                                END;
{Acid attack }            6  :  BEGIN
                                  msg_print('You are covered in acid!');
                                  acid_dam(damroll(damstr),ddesc)
                                END;
{Cold attack }            7  :  BEGIN
                                  msg_print('You are covered with frost!');
                                  cold_dam(damroll(damstr),ddesc)
                                END;
{Lightning attack}        8  :  BEGIN
                                  msg_print('Lightning strikes you!');
                                  light_dam(damroll(damstr),ddesc)
                                END;
{Corrosion attack}        9  :  BEGIN
                        msg_print('A stinging red gas swirls about you.');
                                  corrode_gas(ddesc);
                                  take_hit(damroll(damstr),ddesc);
                                  prt_chp
                                END;
{Blindness attack}        10 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (blind < 1) then
                                      BEGIN
                                        blind := blind + 10 + randint(level);
                                        msg_print('Your eyes begin to sting.');
                                        msg_print(' ')
                                      END;
                                    blind := blind + 5;
                                    prt_chp
                                  END;
{Paralysis attack}        11 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (player_saves(con_adj)) then
                                      msg_print('You resist the effects!')
                                    else if (paralysis < 1) then
                                      BEGIN
                                        if (free_act) then
                                          msg_print('You are unaffected.')
                                        else
                                          begin
                                            paralysis:=randint(level) + 3;
                                            msg_print('You are paralyzed.')
                                          END
                                      END;
                                    prt_chp
                                  END;
{Steal Money }           12 :  with py.misc do
                                  BEGIN
{ 5% guaranteed success. }           if (((randint(100) > 95) or
				          (randint(60) < py.stat.cdex))
                                     and (py.flags.paralysis < 1)) then
                msg_print('You quickly protect your money pouch!')
                                    else
                                      BEGIN
                                        i1 := trunc(au/10) + randint(25);
                                        if (i1 > au) then
                                          au := 0
                                        else
                                          au := au - i1;
                                        msg_print('Your purse feels lighter.');
                                        prt_gold
                                      END;
                                    if (randint(2) = 1) then
                                      BEGIN
                                        msg_print('There is a puff of smoke!');
                                        teleport_away(monptr,max_sight)
                                      END
                                  END;
{Steal Object }           13 :  with py.stat do
                                  BEGIN
{ 5% guaranteed sucess. }           if (((randint(100) > 95) or
				         (randint(60) < py.stat.cdex))
                                    and (py.flags.paralysis < 1)) then
                msg_print('You grab hold of your backpack!')
                                    else
                                      BEGIN
                                        i1 := randint(inven_ctr);
                                        inven_destroy(i1);
                                msg_print('Your backpack feels lighter.')
                                      END;
                                    if (randint(2) = 1) then
                                      BEGIN
                                        msg_print('There is a puff of smoke!');
                                        teleport_away(monptr,max_sight)
                                      END
                                  END;
{Poison }                 14 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    prt_chp;
                                    msg_print('You feel very sick.');
                                    poisoned := poisoned+randint(level)+5;
                                    Opusii_vomit(randint(level)+5);
                                  END;
{Lose dexterity }         15 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (sustain_dex) then
                msg_print('You feel clumsy for a moment, then it passes.')
                                    else
                                      BEGIN
                                        msg_print('You feel more clumsy.');
                                        py.stat.cdex := de_statp(py.stat.cdex);
                                        prt_dexterity
                                      END;
                                    prt_chp
                                  END;
{Lose constitution }      16 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (sustain_con) then
                msg_print('Your body resists the effects of the disease.')
                                    else
                                      BEGIN
                                        msg_print('Your health is damaged!');
                                        py.stat.ccon := de_statp(py.stat.ccon);
                                        prt_constitution
                                      END;
                                    prt_chp
                                  END;
{Lose intelligence }      17 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (sustain_int) then
                                      BEGIN
                msg_print('You feel your memories fading...');
                msg_print('Your memories are suddenly restored!')
                                      END
                                    else
                                      BEGIN
                msg_print('You feel your memories fading...');
                                        py.stat.cint:=de_statp(py.stat.cint);
                                        prt_intelligence
                                      END;
                                    prt_chp;
                                  END;
{Lose wisdom  }           18 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (sustain_wis) then
                                      msg_print('Your wisdom is sustained.')
                                    else
                                      BEGIN
                                        msg_print('You feel less astute.');
                                        py.stat.cwis := de_statp(py.stat.cwis);
                                        prt_wisdom
                                      END;
                                    prt_chp
                                  END;
{Lose experience  }       19:   BEGIN
                                msg_print('You feel your life draining away!');
                                  i1:=damroll(damstr)+
                                        (py.misc.exp div 100)*mon$drain_life;
                                  lose_exp(i1)
                                END;
{Aggravate monster}       20:   aggravate_monster(5);
{Disenchant }             21:   BEGIN
                                  flag := false;
                                  CASE randint(6) of
                                    1 : i1 := 23;
                                    2 : i1 := 26;
                                    3 : i1 := 27;
                                    4 : i1 := 32;
                                    5 : i1 := 28;
                                    6 : i1 := 24
                                  END;
                                  with equipment[i1] do
                                    BEGIN
                                      if (tohit > 0) then
                                        BEGIN
                                          tohit := tohit - randint(2);
                                          flag := true
                                        END;
                                      if (todam > 0) then
                                        BEGIN
                                          todam := todam - randint(2);
                                          flag := true
                                        END;
                                      if (toac > 0) then
                                        BEGIN
                                          toac  := toac  - randint(2);
                                          flag := true
                                        END
                                    END;
                                  if (flag) then
                                    BEGIN
                    msg_print('There is a static feeling in the air...');
                                      py_bonuses(blank_treasure,1)
                                    END
                                END;
{Eat food }               22:   BEGIN
                                  if (find_range([80],i1,i2)) then
                                    inven_destroy(i1)
				  else 
				    msg_print(cdesc+'is unsatiated.');
                                END;
{Eat light }              23:   BEGIN
                                  with equipment[33] do
                                    if (p1 > 0) then
                                      BEGIN
                                        p1 := p1 - 250 - randint(250);
                                        if (p1 < 1) then p1 := 1;
                                        msg_print('Your light dims...')
                                      END    
                                END;
{Eat charges }            24:   if (inven_ctr > 0) then
                                  BEGIN
                                    i1 := randint(inven_ctr);
                                    with inventory[i1] do
                                      if (tval in [55,60,65]) then
                                        if (p1 > 0) then
                                          BEGIN
                                            hp := hp +5*p1;
                                            p1 := 0;
                                msg_print('Energy drains from your pack!');
                                          END
                                  END;       
{Vampire Bite -MWK}        25: BEGIN
                                {Lose experience}
                                msg_print('You feel your life draining away!');
                                  i1:=damroll(damstr)+
                                        (py.misc.exp div 100)*mon$drain_life;
                                lose_exp(i1);
                                {Poison Strength}
                                take_hit(damroll(damstr),ddesc);
                                if (py.flags.sustain_str) then
                msg_print('You feel weaker for a moment, then it passes.')
                                else
                                  if (randint(2) = 1) then
                                    BEGIN
                                      msg_print('You feel weaker.');
                                      py.stat.cstr := de_statp(py.stat.cstr);
                                      prt_strength
                                    END;
                                prt_chp
                              END;
		          26 : if (randint(100) > 50) then {turn to stone -jeb}
                                BEGIN
                                 py.flags.petrified := py.flags.petrified +
				   randint(20); 
				 if (py.flags.petrified < 40) then
				    msg_print('Your blood thickens!')
				 else if (py.flags.petrified < 80) then
				    msg_print('You need a rubdown!')
				 else if (py.flags.petrified < 120) then
				    msg_print('Your joints stiffen...')
				 else if (py.flags.petrified < 160) then
				    msg_print('Your skin turns stoney grey!')
				 else if (py.flags.petrified < 200) then
				    msg_print('You can barely move!')
				 else
				    msg_print('You are thoroughly stoned.');
				 if (py.flags.petrified > 200) then
	  			  begin
				    py.misc.chp := -1;
				    take_hit(0,ddesc);
				  end;
                                END; 
{Mind Flayer}		  27 :if ((randint(60) < py.stat.cdex) and 
				  (randint(5) = 5)) then
                                Begin
                          msg_print('The Mind Flayer''s tentacles touch you.');
                          msg_print('Your neurons have been sucked out.');
                                 py.stat.cint := 3; prt_intelligence;
				 msg_print('Your head feels lighter.');
                                 msg_print('');
                                 take_hit(0,ddesc)
                                End; 
{Hell Horse/Nightmare}    28 :  begin
                                  with py.flags do
                                    begin
                                      blind := blind + randint(5);
                                    end;
                                  msg_print('Your eyes sting.');
                                  take_hit(damroll(damstr),ddesc);
                                end; 
{Vargouillian Attack-NBT} 29 :  BEGIN
				  { Lose Max H.P. by 1d6 }
                                  take_hit(damroll(damstr),ddesc);
                                  if (py.flags.sustain_str) then

{ If player fails save vs poison, nail the muther }
  
              msg_print('You feel weaker for a moment, then it passes.')
                                    else if (randint(3) > 1) then
                                      BEGIN
              			msg_print('Your hit points are drained!');
					with py.misc do
					BEGIN
					  mhp := mhp - randint(6);
					  if (mhp < chp) then
					    chp := mhp;
					  prt_mhp;
					  prt_chp;
					END;
                                      END;
                                  prt_chp
                                END;
			  30 :  begin  {infect player with disease(s)  --jeb}
			          py.flags.diseased := py.flags.diseased + 1;
			      msg_print(cdesc+'gives you a social disease.');
				end;
{Lose charisma  }         31 :  with py.flags do
                                  BEGIN
                                    take_hit(damroll(damstr),ddesc);
                                    if (sustain_chr) then
                                      msg_print('Your charisma is sustained.')
                                    else
                                      BEGIN
                                        msg_print('Your charisma is drained.');
                                        py.stat.cchr := de_statp(py.stat.cchr);
                                        prt_charisma
                                      END;
                                    prt_chp
                                  END;
                          99 :   ;
                          OTHERWISE
                        END
                      END
                    else
                      CASE adesc of
                          1,2,3,6  : msg_print(cdesc + 'misses you.');
                          OTHERWISE
                      END
                  END
              END
          END;
 
 
{ Make the move if possible, five choices }
        FUNCTION make_move(monptr : integer; mm : mm_type) : boolean;
          VAR
                i1,i2,newy,newx : integer;
                movebits : unsigned;
                flag,tflag : boolean;
          BEGIN
            i1 := 1;
            flag := false;
            make_move := false;
            movebits := c_list[m_list[monptr].mptr].cmove;
            repeat
{ Get new positon }
              newy := m_list[monptr].fy;
              newx := m_list[monptr].fx;
              move(mm[i1],newy,newx);
              with cave[newy,newx] do
                if (fval <> 15) then
                  BEGIN
                    tflag := false;
{ Floor is open? }
                    if (fopen) then
                      tflag := true
{ Creature moves through walls? }
                    else if (uand(movebits,%X'40000') <> 0) then
                      tflag := true
{ Creature can open doors? }
                    else if (tptr > 0) then
        with t_list[tptr] do
          with m_list[monptr] do
            if (uand(movebits,%X'20000') <> 0) then
              BEGIN     
{ Creature can open doors. }
                CASE tval of
                  105 : BEGIN   
{ Closed doors. }      if (p1 = 0) then
                            BEGIN
                              tflag := true;
                              if (fm) then
                                if (los(char_row,char_col,newy,newx)) then
                                  BEGIN
                                    t_list[tptr] := door_list[1];
                                    fopen := true;
                                    lite_spot(newy,newx);
                                    tflag := false
                                  END
                            END
{ Locked door  }           else if (p1 > 0) then 
                            BEGIN
                              if (randint(100-level) < 5) then
                                p1 := 0
                            END
{ Stuck doors }            else if (p1 < 0) then
                            BEGIN
                              if (randint(hp) > (10+abs(p1))) then
                                p1 := 0
                            END
                        END;
{ Secret doors..} 109 : BEGIN
                          tflag := true;
                          if (fm) then
                            if (los(char_row,char_col,newy,newx)) then
                              BEGIN
                                t_list[tptr] := door_list[1];
                                fopen := true;
                                lite_spot(newy,newx);
                                tflag := false
                              END
                        END;
                  OTHERWISE
                END
              END
            else
              BEGIN     { Creature can not open doors, must bash them }
                CASE tval of
                  105 : BEGIN
                          i2 := abs(p1) + 20;
                          if (randint(hp) > i2) then
                            BEGIN
                              tflag := true;
                              if (fm) then
                                if (los(char_row,char_col,newy,newx)) then
                                  BEGIN
                                    t_list[tptr] := door_list[1];
                                    t_list[tptr].p1 := randint(2) - 1;
                                    fopen := true;
                                    lite_spot(newy,newx);
                                    tflag := false
                                  END
                            END
                        END;
                  109 : ;
                  OTHERWISE
                END
              END;
{ Glyph of warding present?
Scare Monster trap} if (tflag) then
                      if (tptr > 0) then
                        if (t_list[tptr].tval = 102) then
                          if (t_list[tptr].subval = 99) then
                            tflag := false;
{ Creature has attempted to move on player?  }
                    if (tflag) then
                      if (cptr = 1) then
                        BEGIN
                          if (not (m_list[monptr].ml)) then
                            update_mon(monptr);
                          if (find_flag) then
                            BEGIN
                              find_flag := false;
                              move_char(5)
                            END;
                          make_attack(monptr);
{ Player has read a Confuse Monster. Monster gets a saving throw... }
                          if (py.flags.confuse_monster) then
                            with m_list[monptr] do
                              with c_list[mptr] do
                                BEGIN
                                  msg_print('Your hands stop glowing.');
                                  py.flags.confuse_monster := false;
                                  if ((randint(max_mons_level) < level) or
                                      (uand(%X'1000',cdefense) <> 0)) then
                                    BEGIN
                msg_print('The ' + name + ' is unaffected.')
                                    END
                                  else
                                    BEGIN
                msg_print('The ' + name + ' appears confused.');
                                      confused := true
                                    END
                                END;
                          tflag := false;
                          flag  := true
                        END
{ Creature is attempting to move on other creature? }
                      else if ((cptr > 1) and
                        ((newy <> m_list[monptr].fy) or
                         (newx <> m_list[monptr].fx))) then
                        BEGIN
{ Creature eats other creatures? }
                          if (uand(movebits,%X'80000') <> 0) then
                            delete_monster(cptr)
                          else
                            tflag := false
                        END;
{ Creature has been allowed move }
                    if (tflag) then
                      with m_list[monptr] do
                        BEGIN
{ Pick up or eat an object }
                          if (uand(movebits,%X'100000') <> 0) then
                            with cave[newy,newx] do
                              if (tptr > 0) then
                                if (t_list[tptr].tval < 100) then
                                  delete_object(newy,newx);
{ Move creature record }
                          move_rec(fy,fx,newy,newx);
                          fy := newy;
                          fx := newx;
                          flag := true;
                          make_move := true
                        END
                  END;
              i1 := i1 + 1;
{ Up to 5 attempts at moving, then give up }
            until ((flag) or (i1 > 5))
          END;
 
 
{ Creatures can cast spells too. 
cast_spell := true if creature changes position
took_turn  := true if creature casts a spell }
     FUNCTION cast_spell( monptr : integer; VAR took_turn : boolean) : boolean;
          VAR
                i1 : unsigned;
                i2,i3,y,x,chance,thrown_spell : integer;
                r1 : real;
                spell_choice : array [1..31] of integer;
                cdesc,ddesc,outval : vtype;
                flag : boolean;
          BEGIN
            with m_list[monptr] do
              with c_list[mptr] do
                BEGIN
                  chance := int(uand(spells,%X'0000000F'));
{ 1 in x chance of casting spell }
                  if (randint(chance) <> 1) then
                    BEGIN
                      cast_spell := false;
                      took_turn  := false
                    END
{ Must be within certain range }
                  else if (cdis > max_spell_dis) then
                    BEGIN
                      cast_spell := false;
                      took_turn  := false
                    END
{ Must have unobstructed Line-Of-Sight }
                  else if (not(los(char_row,char_col,fy,fx))) then
                    BEGIN
                      cast_spell := false;
                      took_turn  := false
                    END
                  else  
{ Creature is going to cast a spell }
                    BEGIN
                      took_turn  := true;
                      cast_spell := false;
{ Describe the attack }
                      flag := true;
                      if (not(ml)) then
                        flag := false
                      else if ((uand(%X'10000',c_list[mptr].cmove) <> 0) and
                          (not(py.flags.see_inv))) then
                        flag := false
                      else if (py.flags.blind > 0) then
                        flag := false;
                      if (flag) then
                        cdesc := 'The ' + name + ' '
                      else
                        cdesc := 'It ';
{ For 'DIED_FROM' string }
                      if (uand(%X'80000000',cmove) <> 0) then
                        ddesc := 'The ' + name
                      else
                        ddesc := '& ' + name;
                      temporary_slot.name   := ddesc;
                      temporary_slot.number := 1;
                      objdes(ddesc,1,true,'t');
{ End DIED_FROM. Extract all possible spells into spell_choice }
                      i1 := uand(spells,%X'FFFFFFF0');
                      i3 := 0;
                      while (i1 <> 0) do
                        BEGIN
                          i2 := bit_pos(i1);
                          i3 := i3 + 1;
                          spell_choice[i3] := i2
                        END;
{ Choose a spell to cast }
                      thrown_spell := spell_choice[randint(i3)];
{ Cast the spell... }
                      CASE thrown_spell of
{Teleport Short} 5 :    teleport_away(monptr,4);
{Teleport Long } 6 :    teleport_away(monptr,max_sight);
{Teleport To   } 7 :    BEGIN
                          cdesc := cdesc + 'beckons you near.';
                          msg_print(cdesc);
                          msg_print(' ');
                          teleport_to(fy,fx)
                        END;
{Light Wound   } 8 :    BEGIN
                          cdesc := cdesc + 'hurls a frisbee at you';
                          msg_print(cdesc);
                          if (player_saves(wis_adj+py.misc.lev)) then
                            msg_print('You dodge out of the way...')
                          else
                            take_hit(damroll('2d8'),ddesc)
                        END;
{Serious Wound } 9 :    BEGIN
                          cdesc := cdesc + 'sends an energy bolt your way.';
                          msg_print(cdesc);                     
                          if (player_saves(wis_adj+py.misc.lev)) then
                            msg_print('You duck just in time...')
                          else
                            take_hit(damroll('8d8'),ddesc)
                        END;
{Hold Person   }10 :    BEGIN
                          cdesc := cdesc + 'shoots a freeze ray at you.';
                          msg_print(cdesc);
                          if (py.flags.free_act) then
                            msg_print('You are unaffected...')
                          else if (player_saves(wis_adj+py.misc.lev)) then
                            msg_print('You shake out of it...')
                          else if (py.flags.paralysis > 0) then
			    begin
                              py.flags.paralysis:=py.flags.paralysis+2;
			      msg_print('You are stuck even more!')
			    end
                          else
			    begin
                              py.flags.paralysis:=randint(5)+4;
			      msg_print('You are held in place!')
			    end;
                        END;
{Cause Blindnes}11 :    BEGIN
                          cdesc := cdesc + 'throws sand in your face.';
                          msg_print(cdesc);
                          if (player_saves(wis_adj+py.misc.lev)) then
                            msg_print('You grit your teeth and bear it...')
                          else if (py.flags.blind > 0) then
                            py.flags.blind := py.flags.blind + 5
                          else
                            BEGIN
                              py.flags.blind := py.flags.blind+12+randint(3);
                              msg_print(' ')
                            END
                        END;
{Cause Confuse }12 :    BEGIN
                          cdesc := cdesc + 'tries to confuse you.';
                          msg_print(cdesc);
                          if (player_saves(wis_adj+py.misc.lev)) then
                            msg_print('You resist the attempt...')
                          else if (py.flags.confused > 0) then
                            py.flags.confused := py.flags.confused + 2
                          else
                            py.flags.confused := randint(5) + 3
                        END;
{Cause Fear}    13 :    BEGIN
                          cdesc := cdesc + 'tries to scare you.';
                          msg_print(cdesc);
                          if (player_saves(wis_adj+py.misc.lev)) then
                            msg_print('Your bravery holds true...')
                          else if (py.flags.afraid > 0) then
                            py.flags.afraid := py.flags.afraid + 2
                          else
                            py.flags.afraid := randint(5) + 3
                        END;
{Summon Monster}14 :    BEGIN
                          cdesc := cdesc + 'summons an underling!';
                          msg_print(cdesc);
                          y := char_row;
                          x := char_col;
                          summon_monster(y,x,false);
                          check_mon_lite(y,x)
                        END;
{Summon Undead} 15 :    BEGIN
                          cdesc := cdesc + 'summons a mutant!';
                          msg_print(cdesc);
                          y := char_row;
                          x := char_col;
                          summon_undead(y,x);
                          check_mon_lite(y,x)
                        END;
{Slow Person  } 16 :    BEGIN
                          cdesc := cdesc + 'attempts to tangle you in taffy.';
                          msg_print(cdesc);
                          if (py.flags.free_act) then
                            msg_print('You are unaffected...')
                          else if (player_saves(wis_adj+py.misc.lev)) then
                            msg_print('You lick the taffy right off!.')
                          else if (py.flags.slow > 0) then
                            py.flags.slow := py.flags.slow + 2
                          else
                            py.flags.slow := randint(5) + 3
                        END;
{Drain Mana }   17 :    if (trunc(py.misc.cmana) > 0) then
                          BEGIN
                            outval := cdesc + 'drains resources from you!';
                            msg_print(outval);
                            outval := cdesc + 'appears more powerful...';
                            msg_print(outval);
                            r1 := randint(level);
                            if (r1 > py.misc.cmana) then r1 := py.misc.cmana;
                            py.misc.cmana := py.misc.cmana - r1;
                            hp := hp + 5*trunc(r1)
                          END;
{Breathe Light} 20 :    BEGIN
                          cdesc := cdesc + 'shoots lightning at you!';
                          msg_print(cdesc);
			  explosion(1,char_row,char_col,
			    trunc(hp/3.0),2,true,'lightning')
                        END;
{Breathe Gas }  21 :    BEGIN
                          cdesc := cdesc + 'throws a gas grenade at you!';
                          msg_print(cdesc);
                          explosion(2,char_row,char_col,
			    trunc(hp/3.0),2,true,'poisonous gas')
                        END;
{Breath Acid }  22 :    BEGIN
                          cdesc := cdesc + 'douses you with acid.';
                          msg_print(cdesc);
			  explosion(3,char_row,char_col,
			    trunc(hp/3.0),2,true,'acid')
                        END;
{Breath Frost } 23 :    BEGIN
                          cdesc := cdesc + 'covers you with frost.';
                          msg_print(cdesc);
			  explosion(4,char_row,char_col,
			    trunc(hp/3.0),2,true,'frost')
                        END;
{Breath Fire }  24 :    BEGIN
                          cdesc := cdesc + 'blasts you with fire.';
                          msg_print(cdesc);
			  explosion(5,char_row,char_col,
			    trunc(hp/3.0),2,true,'fire')
                        END;
                OTHERWISE BEGIN
                            msg_print('Creature cast unknown spell.');
                            cdesc := ''
                          END
                      END
{ End of spells }
                    END
                END
          END;
 
 
{ Main procedure for monster movement (MON_MOVE) }
        BEGIN
          mon_move := false;
          with c_list[m_list[monptr].mptr] do
            BEGIN
{ Does the critter multiply? }
              if (m_list[monptr].cur_reprod > 0) then
                if (max_mon_mult >= mon_tot_mult) then
                  if ((py.flags.rest mod mon_mult_adj) = 0) then
                    with m_list[monptr] do
                      BEGIN
                        i3 := 0;
                        for i1 := fy-1 to fy+1 do
                          for i2 := fx-1 to fx+1 do
                            if (in_bounds(i1,i2)) then
                              if (cave[i1,i2].cptr > 1) then
                                i3 := i3 + 1;
                        if (i3 < 4) then
                          if (randint(i3*mon_mult_adj) = 1) then
			    begin
			      cur_reprod := cur_reprod - 1;
                              multiply_monster(fy,fx,mptr,
			      		randint(cur_reprod),false);
			    end;
                      END;
{ Creature is confused?  Chance it becomes un-confused  }
              move_test := false;
              if (m_list[monptr].confused) then
                BEGIN
                  mm[1] := randint(9);
                  mm[2] := randint(9);
                  mm[3] := randint(9);
                  mm[4] := randint(9);
                  mm[5] := randint(9);
                  mon_move := make_move(monptr,mm);
                  if (randint(8) = 1) then
                    m_list[monptr].confused := false;
                  move_test := true
                END
{ Creature may cast a spell }
              else if (spells > 0) then
                mon_move := cast_spell(monptr,move_test);
              if (not(move_test)) then
                BEGIN
{ 75% random movement }
                  if ((randint(100) < 75) and
                    (uand(cmove,%X'00000020') <> 0)) then
                    BEGIN
                      mm[1] := randint(9);
                      mm[2] := randint(9);
                      mm[3] := randint(9);
                      mm[4] := randint(9);
                      mm[5] := randint(9);
                      mon_move := make_move(monptr,mm)
                    END
{ 40% random movement }
                  else if ((randint(100) < 40) and
                    (uand(cmove,%X'00000010') <> 0))  then
                    BEGIN
                      mm[1] := randint(9);
                      mm[2] := randint(9);
                      mm[3] := randint(9);
                      mm[4] := randint(9);
                      mm[5] := randint(9);
                      mon_move := make_move(monptr,mm)
                    END
{ 20% random movement }
                  else if ((randint(100) < 20) and
                    (uand(cmove,%X'00000008') <> 0))  then
                    BEGIN
                      mm[1] := randint(9);
                      mm[2] := randint(9);
                      mm[3] := randint(9);
                      mm[4] := randint(9);
                      mm[5] := randint(9);
                      mon_move := make_move(monptr,mm)
                    END
{ Normal movement }
                  else if (uand(cmove,%X'00000002') <> 0) then
                    BEGIN
                      if (randint(200) = 1) then
                        BEGIN
                          mm[1] := randint(9);
                          mm[2] := randint(9);
                          mm[3] := randint(9);
                          mm[4] := randint(9);
                          mm[5] := randint(9)
                        END
                      else
                        get_moves(monptr,mm);
                      mon_move := make_move(monptr,mm);
                    END
{ Attack, but don't move }
                  else if (uand(cmove,%X'00000001') <> 0) then
                    if (m_list[monptr].cdis < 2) then
                      BEGIN
                        get_moves(monptr,mm);
                        mon_move := make_move(monptr,mm)
                      END
                END
            END
        END;
 
 
{ Main procedure for creatures }
      BEGIN
        if (muptr > 0) then
          BEGIN
{ Process the monsters  }
        i1 := muptr;
        repeat
          with m_list[i1] do
            BEGIN
              cdis := distance(char_row,char_col,fy,fx);
              if (attack) then  { Attack is argument passed to CREATURE}
                BEGIN
                  i3 := movement_rate(cspeed);
                  if (i3 > 0) then
                    for i2 := 1 to movement_rate(cspeed) do
                      BEGIN
                        if ((cdis <= c_list[mptr].aaf) or (ml)) then
                          BEGIN
                            if (csleep > 0) then
                              if (py.flags.aggravate) then
                                csleep := 0
                              else if (py.flags.rest < 1) then
                                if (randint(10) > py.misc.stl) then
                                  csleep := csleep - trunc(75.0/cdis);
                              if (stuned > 0) then
                                stuned := stuned - 1;
                            if ((csleep <= 0) and (stuned <= 0)) then
                              BEGIN
                                moldy := fy;
                                moldx := fx;
                                if (mon_move(i1)) then
                                  if (ml) then
                                    BEGIN
                                      ml := false;
                                      if (test_light(moldy,moldx)) then
                                        lite_spot(moldy,moldx)
                                      else
                                        unlite_spot(moldy,moldx)
                                    END
                              END
                          END;
                        update_mon(i1)
                      END
                  else
                    update_mon(i1)
                END
              else
                update_mon(i1)
            END;
          i1 := m_list[i1].nptr;
        until ((i1 = 0) or (moria_flag))
{ End processing monsters }
          END
      END;
