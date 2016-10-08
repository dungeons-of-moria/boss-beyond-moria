{ Potions for the quaffing }
[psect(misc2$code)] procedure quaff;
      var
        i1 	  	 : unsigned;
	i2,i3,i4,i5,i7   : integer;
	item_val,nhp,nst : integer;
        out_val		 : vtype;
        redraw,ident	 : boolean;
      begin
        reset_flag := true;
        if (inven_ctr > 0) then
          begin
            if (find_range([75,76],i2,i3)) then
              begin
                redraw := false;
               if (get_item(item_val,'Quaff which potion?',redraw,i2,i3)) then
                  with inventory[item_val] do
                    begin
                      if (redraw) then draw_cave;
                      reset_flag := false;
                      i1 := flags;
                      ident := false;
                      while (i1 > 0) do
                        begin
                          i2 := bit_pos(i1);
                          if (tval = 76) then i2 := i2 + 32;
{ Potions }
        case (i2) of
          1  :  with py.stat do
		  Begin
		    if (tohit > 0) then {Increase Strength x (tohit) }
	              for i7 := 1 to tohit do
			begin  
                          cstr := in_statp(cstr);
                          ident := true;
                          if (cstr > str) then
                            str := cstr;
                          case (randint(3)) of
                            1 : msg_print('Wow!  What bulging muscles!');
                            2 : msg_print('Damn!  You''re getting buff!');
                            3 : msg_print('I bet you pump iron in your spare time.');
                          end;
			end
		    else if (tohit < 0) then {Lose Strength}
		      for i7 := 1 to abs(tohit) do
		        begin	    
		          ident := lose_str;
			  case randint(3) of
			    1 : msg_print('Your burden feels heavier.');
			    2 : msg_print('Your muscles shrivel up.');
			    3 : msg_print('You resemble a 98 pound weakling.');
			  end;
		        end;
                    prt_strength;
                    prt_max_weight;
                  End;
          2  :  if (py.flags.diseased = 0) then
		  begin
                    py.stat.cstr := py.stat.str;
                    msg_print('You feel warm all over.');
                    prt_strength;
                    prt_max_weight;
                    ident := true;
		  end
		else msg_print('Your disease nullifies the potion!');
          3  :  with py.stat do
                  Begin
		    if (tohit > 0) then { Inc. Intellegence x (tohit) }
		      for i7 := 1 to tohit do
		        begin
			  ident := true;
                          cint := in_statp(cint);
                          if (cint > py.stat.int) then
                          py.stat.int := cint;
                          msg_print('Imagine how it would feel if your brain started growing.');
			end
		    else if (tohit < 0) then  {Dec. Int}
		      for i7 := 1 to abs(tohit) do
		        begin
			  case (randint(3)) of
			    1: msg_print('This is your brain on drugs.');
			    2: msg_print('You accidentally snort this acidic potion up your nose.');
			    3: msg_print('You hear something rattle in your head.');
			  end;
		        end;
                    prt_intelligence;
                  End;
          4  :  if (py.flags.diseased = 0) then
		  begin
                    py.stat.cint := py.stat.int;
                    msg_print('Your brain throbs for a moment');
                    prt_intelligence;
                    ident := true;
		  end
		else msg_print('Your disease nullifies the potion!');
          5  :  with py.stat do
                  Begin
		    if (tohit > 0) then {Increase Wisdom x (tohit) }
		      for i7 := 1 to tohit do
		        begin
			  ident := true;
			  cwis := in_statp(cwis);
			  if (cwis > wis) then
			    wis := cwis;
			  case (randint(3)) of
		  1 : msg_print('You suddenly have a profound thought!');
		  2 : msg_print('You feel closer to the Buddah consciousness');
		  3 : msg_print('Wisdom becomes you, brave adventurer.');
			  end;
			end
		    else if (tohit < 0) then {Dec. Wis}
		      for i7 := 1 to abs(tohit) do
		        begin
			  ident := lose_wis;
			  msg_print('The Buddah slips from your mind''s eye.');
			end;
                    prt_wisdom;
                  End;
          6  :	  if (py.flags.diseased = 0) then
		    begin
                      py.stat.cwis := py.stat.wis;
		      case (randint(3)) of
		        1: msg_print('The Buddah forgives you.');
		        2: msg_print('You feel the Buddah return to you.');
		        3: msg_print('You realize how foolish you''ve been.');
		      end;
                      prt_wisdom;
                      ident := true;
		    end
		  else msg_print('Your disease nullifies the potion!');
          7  :  with py.stat do
                  Begin
                    if (tohit > 0) then {Increase Dex }
		      for i7 := 1 to tohit do
		        begin
			  ident := true;
		          cdex := in_statp(cdex);
			  if (cdex > dex) then
			    dex := cdex;
			  case (randint(4)) of
			    1 : msg_print('You grow some new fingers.');
			    2 : msg_print('You feel more limber!');
			    3 : if (dex > 17) then
			          msg_print('You are suddenly able to juggle.')
			        else
				  msg_print('You feel nearly able to juggle.');
			    4 : begin
                               msg_print('You grow another limb.');
			       msg_print('But I''m not sure what to call it.');
			        end;
                          end;
			end
		    else if (tohit < 0) then {Dec. Dex}  
		      for i7 := 1 to abs(tohit) do
		        begin
			  case (randint(3)) of
			    1:  msg_print('You feel your joints stiffen.');
			    2:  msg_print('Butterfingers!');
			    3:  msg_print('Your fingers tie in knots.');
			  end;
			  ident := lose_dex;
			end;
                    prt_dexterity;
                  End;
          8  :    if (py.flags.diseased = 0) then
		    begin
                      py.stat.cdex := py.stat.dex;
                      msg_print('You feel at ease with your body.');
                      prt_dexterity;
                      ident := true;
		    end
		  else msg_print('Your disease nullifies the potion!');
           9  :  with py.misc do
                  Begin
                    if (tohit > 0) then {Increase Con }
		      for i7 := 1 to tohit do
		        begin
			  ident := true;
			  py.stat.ccon := in_statp(py.stat.ccon);
			  if (py.stat.ccon > py.stat.con) then
			    py.stat.con := py.stat.ccon;
			  case (randint(3)) of
			    1: msg_print('You feel like you''ve just inhaled a vial of amiyl nitrate.');
			    2: msg_print('You feel _really_ healthy.');
			    3: msg_print('Your constitution just went up.');
			  end;
			end
		    else if (tohit < 0) then {Decrease Con}  
		      for i7 := 1 to abs(tohit) do
		        begin
			  ident := lose_con;
			  msg_print('..........');
			end;
                    prt_constitution;
                  End;
         10  :  if (py.flags.diseased = 0) then
	          begin 
                    py.stat.ccon := py.stat.con;
                    msg_print('You feel your grip on life strengthen.');
                    prt_constitution;
                    ident := true;
                  end
		else msg_print('Your disease nullifies the potion!');
         11  :  with py.stat do
                  Begin
		    if (tohit > 0) then {Increase Charisma x (tohit) }
		      for i7 := 1 to tohit do
		        begin
			  ident := true;
			  cchr := in_statp(cchr);
			  if (cchr > chr) then
			    chr := cchr;
                          case (randint(3)) of
			    1 : msg_print('Gee, ain''t you cute!');
			    2 : msg_print('Ooh!  What a face!');
			    3 : msg_print('Quick!  Where''s my camera?');
			  end;
			end
		    else if (tohit < 0) then {Decrease Chr}  
		      for i7 := 1 to abs(tohit) do
		        begin
			  ident := lose_chr;
			  msg_print('Bleah!  Sourpuss!');
			end;
                    prt_charisma;
                  End;
          12 :  if (py.flags.diseased = 0) then
		  begin
                    py.stat.cchr := py.stat.chr;
                    msg_print('You feel your looks returning.');
                    prt_charisma;
                    ident := true;
                  end
		else msg_print('Your disease nullifies the potion!');
{Healing} 13 :  ident := hp_player(damroll('2d7'),'a potion.');
          14 :  ident := hp_player(damroll('4d7'),'a potion.');
          15 :  ident := hp_player(damroll('6d7'),'a potion.');
          16 :  ident := hp_player(1000,'a potion.');
          17 : ;
          18 :  with py.misc do
                  begin
                    i5 := (exp div 2) + 10;
                    if (i5 > 100000) then i5 := 100000;
                    exp := exp + i5;
                    msg_print('You feel more experienced.');
                    prt_experience;
                    ident := true;
                  end;
          19 :  with py.flags do
                  if (not (py.flags.free_act)) then
                    begin
                      msg_print('Sheesh!  Falling asleep on the job!');
                      py.flags.paralysis:=py.flags.paralysis+randint(4)+4;
                      ident := true;
                    end;
          20 :  with py.flags do
                  begin
                    msg_print('Your eyelids close and fuse shut!');
                    blind := blind + randint(100) + 100;
                    ident := true;
                  end;
          21 :  with py.flags do
                  begin
                    msg_print('Hey!  This is good stuff!  * Hick! *');
                    confused := confused + randint(20) + 12;
                    ident := true;
                  end;
          22 :  with py.flags do
                  begin
                    msg_print('You feel very sick.');
                    poisoned := poisoned + randint(15) + 10;
                    ident := true;
                  end;
          23 :  begin
                  py.flags.fast := py.flags.fast + randint(25) + 15;
                  ident := true;
                end;
          24 :  begin
                  py.flags.slow := py.flags.slow + randint(25) + 15;
                  ident := true;
                end;
          25 :  ident := detect_monsters;
          26 :  msg_print('hm.  nothing happened that you can see...');
          27 :  msg_print('hm.  nothing happened that you can see...');
          28 :  msg_print('hm.  nothing happened that you can see...');
          29 :  cure_blindness;
          30 :  cure_confusion;
          31 :  cure_poison;
	  32 :; {Cursed bit- do nothing}
          33 :  with py.misc do
		  begin
                    if (pskill in [1,2,7]) then
                      begin
                        ident := learn_spell(redraw);
                        if (redraw) then draw_cave;
                      end;
                    if (pskill in [3,4]) then
                      ident := learn_prayer;
                    if (pskill in [5,6]) then
                      begin
                        ident := learn_extra(redraw);
                        if (redraw) then draw_cave;
                      end;
		  end;
          34 :  begin
                  case (randint(5)) of
                    1 : msg_print('You feel your memories fade.');
                    2 : msg_print('You cannot remember your mother''s name.');
                    3 : msg_print('You cannot remember your second-grade teacher''s name.');
                    4 : msg_print('You cannot remember your own name.');
                    5 : msg_print('You can''t remember anyone''s name.');
                  end;
                  msg_print('');
                  i4 := trunc(py.misc.exp/5.0);
                  lose_exp(randint(i4)+i4);
                  ident := true;
                end;
          35 :  with py.flags do
                  begin
                    poisoned := 0;
                    if (food > 150) then food := 150;
                    paralysis := 4;
           msg_print('Does the phrase "spew chunks" mean anything to you?');
                    ident := true;
                  end;
          36 :  begin
                  py.flags.invuln := py.flags.invuln + 50;
                  msg_print('Your skin turns into a high tech, very');
                  msg_print('hard polymerized laminate material.');
                  py.misc.pac := py.misc.pac + 100;
                  py.misc.dis_ac := py.misc.dis_ac + 100;
                  prt_pac;
                  ident := true;
                end;
          37 :  begin
                  py.flags.hero := py.flags.hero + randint(25) + 25;
                  ident := true;
                end;
          38 :  begin
                  py.flags.shero := py.flags.shero + randint(25) + 25;
                  ident := true;
                end;
          39 :  ident := remove_fear;
          40 :  ident := restore_level;
          41 :  with py.flags do
                  resist_heat := resist_heat + randint(10) + 10;
          42 :  with py.flags do
                  resist_cold := resist_cold + randint(10) + 10;
          43 :  detect_inv2(randint(12)+12);
          44 :  ident := slow_poison;
          45 :  ident := cure_poison;
          46 :  with py.misc do
                  if (cmana < mana) then
                    begin
                      cmana := mana;
                      ident := true;
                      msg_print('Power courses through your veins!');
                      prt_mana;
                    end;
          47 :  with py.flags do
                  begin
                    tim_infra := tim_infra + 100 + randint(100);
                    ident := true;
                    msg_print('Your eyes shift into low-frequency mode');
                  end;
          48 :  with py.misc do
                  begin
                    msg_print('You feel able to withstand more damage.');
                    msg_print('Now don''t get cocky...');
                    nhp := randint(2)*get_hitdie;
                    mhp := mhp + nhp;
                    chp := chp + nhp;
                    prt_mhp;
                    prt_chp;
                  end;
          49 :  with py.misc do
                  begin
                    msg_print('You feel more confident.');
                    gain_mana(int_adj);
                    gain_mana(wis_adj);
                    prt_mana;
                  end;
          50 : with py.misc do  {longevity}
	         begin               
		   if ((prace = 8) and (age>120)) then
		     i7 := 100
		   else if (age > 20) then
		     i7 := 20
		   else
		     i7 := 0;
		   if (i7 <> 0) then
		     begin
                       age := age - randint(i7);
		       age := greater(age,16);
		       msg_print('You feel much younger.');
		     end
		   else
		     msg_print('You feel nothing happen.');
                 end;
{     51..64 :  msg_print('The bottle was empty.'); }
          otherwise msg_print('The bottle was empty.');
        end;
{ End of Potions... }   end;
                      if (ident) then
                        identify(inventory[item_val]);
                      if (flags <> 0) then
                        begin
                          with py.misc do
                            exp := exp + round(level/lev);
                          prt_experience;
                        end;
                      add_food(p1);
                      desc_remain(item_val);
                      inven_destroy(item_val);
                    end
                else
                  if (redraw) then draw_cave;
              end
            else
              msg_print('You are not carrying any potions.');
          end
        else
          msg_print('But you are not carrying anything.');
      end;

{ Potions for the mixing!!}
[psect(misc2$code)] procedure mix_potions;

  VAR
        blegga1,blegga2     : treasure_type;
	i1,i2,i3,val1,val2  : integer;
	redraw,redraw2,dummy: boolean;

 {sub-procedures}
  procedure   mixture_blows_up(explode : dtype);
  
  Begin
    msg_print('The mixture blows up!!!');
    dummy := hp_player(-damroll(explode),'an exploding potion');
    msg_print('You are damaged by your feeble attempts at chemistry.');
  End;


  procedure   make_new_potion(result : integer);

  var  
    power,p1,p2,i1   : integer;
    out_val	     : vtype;
    new_potion	     : treasure_type;

  Begin
    msg_print('You successfully mix the two potions together!');
    power := (blegga1.level + blegga2.level)*result;
    p1 := power - 10;
    if (p1 < 0) then p1 := 0;
    if (p1 > max_obj_level) then p1 := max_obj_level-20;
    p2 := power + 10;
    if (p2 > max_obj_level) then p2 := max_obj_level;

    repeat 
      repeat
        new_potion := object_list[randint(max_objects)];
      until (new_potion.tval in [75,76]);
    until (new_potion.level in [p1..p2]);

    temporary_slot := new_potion;
    inven_carry(i1);
    objdes(out_val,i1,true,'i');
    out_val := 'You have created '+out_val+' ('+raoul_label(i1)+cur_char1(i1); 
    msg_print(out_val);
    msg_print('Congratulations.');
  End;


BEGIN
  reset_flag := true;
  if (inven_ctr = 0) then
    msg_print('You are not carrying anything.')
  else 
    begin
      if (not(find_range([75,76],i2,i3))) then
        msg_print('You are not carrying any potions.')
      else
        begin
          redraw := false;
          if (get_item(val1,'Start with which potion?',redraw,i2,i3)) then
            begin
      if (get_item(val2,'And mix it with which potion?',redraw2,i2,i3)) then
	        begin
		  if ((val1 = val2) and (inventory[val1].number = 1)) then
		    begin
		      msg_print('You can''t mix a potion with itself!');
		      msg_print('');
		    end
		  else
		    begin
		      if (redraw or redraw2) then draw_cave;
		      redraw := false;
		      redraw2 := false;
		      reset_flag := false;
		      blegga1 := inventory[val1];
		      blegga2 := inventory[val2];
		      if (val1 >= val2) then
		        begin
		          inven_destroy(val1);
		          inven_destroy(val2);		      
			end
		      else
		        begin
		          inven_destroy(val2);
		          inven_destroy(val1);		      
			end;
		      i1 := randint(20)+int_adj;
		      if (i1 = 1) then
		        mixture_blows_up('2d20')
		      else if (i1 < 6) then
		        mixture_blows_up('1d10')
		      else if (i1 < 16) then
		        msg_print('The mixture dissolves into the air.')
		      else if (i1 < 20) then
		        make_new_potion(1)
		      else
		        make_new_potion(2);    
		    end
		end
	    end;
	  if (redraw or redraw2) then draw_cave;
	end
    end
END;
