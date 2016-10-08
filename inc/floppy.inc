        { Running Disk Programs              -RAK- (RLG)  }
	{Player Must have a computer (tval=29) wielded}
[psect(misc2$code)] procedure read;
      var
                i1	                           : unsigned;
                i2,i3,i4,item_val,y,x,failure_rate : integer;
                redraw,ident,first,flag		   : boolean;
     BEGIN
       first := true;
       reset_flag := true;
       if (inven_ctr > 0) then
        if (equipment[35].tval = 29) then
          begin
            if (find_range([70,71],i2,i3)) then
              if (py.flags.blind > 0) then
                msg_print('Help me, I''m blind!')
              else if (no_light) then
                msg_print('You have no light to see by.')
              else if (py.flags.confused > 0) then
                begin
                  msg_print('The text seems to swim about the screen!');
                  msg_print('You are too confused to type...');
                end
              else
                begin
                  redraw := false;
                  if (get_item(item_val,'Run which floppy disk?',redraw,
                                                                i2,i3)) then
                    with inventory[item_val] do
                      begin
                        if (redraw) then draw_cave;
		        failure_rate := (equipment[35].subval - 300);
                        reset_flag := false;
                        i1 := flags;
                        ident := false;
		        if ((failure_rate > 90) and (randint(3)=3)) then
			  begin
			    ident := false;
			    first := false;
			    msg_print('Sparks fly from the computer!');
			  end
                        else while (i1 > 0) do
                          begin
                            i2 := bit_pos(i1);
                            if (tval = 71) then i2 := i2 + 32;
                            if (first) then
                              if (not(i2 in [4,25])) then
                                first := false;
                                
        { Disks... }
        case (i2) of                 
          1  :  ident := enchant_weapon('tohit');
          2  :  ident := enchant_weapon('todam');
          3  :  ident := enchant_armor('up');
          4  :  begin
                  identify(inventory[item_val]);
                  msg_print('This is an identify program');
                  msg_print(' ');
                  if (ident_spell) then first := false;
                end;
          5  :  if (remove_curse) then
                  begin
                    msg_print('You feel as if someone is watching over you.');
                    ident := true;
                  end;        
          6  :  ident := light_area(char_row,char_col);
          7  :  begin
                  for i3 := 1 to randint(3) do
                    begin
                      y := char_row;
                      x := char_col;
                      summon_monster(y,x,false);
                    end;
                  ident := true;
                end;
          8  :  begin             
                  teleport(10);
                  ident := true;
                end;
          9  :  begin
                  teleport(100);
                  ident := true;                          
                end;
          10 :  begin
                  msg_print('That disk appeared to be blank.');
                  ident := true;
                end;
          11 :  begin
                  msg_print('Your hands begin to glow.');
                  py.flags.confuse_monster := true;
                  ident := true;
                end;
          12 :  ident := map_area;
          13 :  ident := sleep_monsters2_3(char_row,char_col,1);
          14 :  ident := warding_glyph;
          15 :  ident := detect_treasure;
          16 :  ident := detect_object;
          17 :  ident := detect_trap;
          18 :  ident := detect_sdoor;
          19 :  begin
                  msg_print('This is a mass genocide program.');
                  msg_print(' ');
                  ident := mass_genocide;
                end;
          20 :  ident := detect_invisible;
          21 :  begin
                  ident := aggravate_monster(20);
                  msg_print('There is a high pitched humming noise');
                end;
          22 :  ident := trap_creation;
          23 :  ident := td_destroy;
          24 :  ident := door_creation;
          25 :  begin
                  identify(inventory[item_val]);
                  msg_print('This is a Recharge program.');
                  msg_print(' ');
                  if (recharge(60)) then first := false;
                end;
          26 :  begin
                  msg_print('This is a genocide program.');
                  msg_print(' ');
                  ident := genocide;
                end;
          27 :  ident := unlight_area(char_row,char_col);
          28 :  ident := protect_evil;
          29 :  ident := create_food;
          30 :  ident := dispell_creature(%X'0008',60);
          31 :  begin
                  msg_print('That disk appeared to be blank.');
                  ident := true;
                end;

	  32 :; {Cursed bit- do nothing}
          33 :	for i3 := 1 to randint(2) do
		  ident := enchant_weapon('both');
          34 :  ident := enchant_weapon('curse');
          35 :  for i3 := 1 to randint(2) do
	          ident := enchant_armor('up');
          36 :  ident := enchant_armor('curse');
          37 :  begin
                  for i3 := 1 to randint(3) do
                    begin
                      y := char_row;
                      x := char_col;
                      summon_undead(y,x);
                    end;
                  ident := true;
                end;
          38 :  ident := bless(randint(12)+6);
          39 :  ident := bless(randint(24)+12);
          40 :  ident := bless(randint(48)+24);
          41 :  begin
                  ident := true;
                  py.flags.word_recall := 25 + randint(30);
                  msg_print('The air about you becomes charged...');
                end;
          42 :  ident := destroy_area(char_row,char_col);
          43 :  begin	
		  ident := true;
		  random_wierdness(0,true);
		end;
          44 :  begin
                  for i3 := 1 to randint(3) do
                    begin
                      y := char_row;
                      x := char_col;
                      summon_monster(y,x,false);
                    end;
                  ident := true;
                end;
          45 :  with py.flags do
	          begin
		    ident := true;
		    ring_blast := 9; {odd}
		    msg_print('You are surrounded by a strange ring of fire!');
		  end;		  
          46 :  with py.flags do
	          begin
		    ident := true;
		    ring_blast := 10; {even}
		    msg_print('A strange ring of frost encircles you!');
		  end;		  
{     47..64 :  msg_print('bogus disk.  send bug report to max::raoul');}
          otherwise msg_print('bogus disk.  send bug report to max::raoul');
        end;
        { End of Disks... }
                          end;
                        if (not(reset_flag)) then
                          begin
                            if (ident) then
                              identify(inventory[item_val]);
                            if (not(first)) then
                              Begin   {Disk Eaten?}
				if (i2 = 4) then 
				  failure_rate := failure_rate div 2;
				if (randint(100) < failure_rate) then
				  begin
                                    msg_print('Your computer eats the disk.');
                                    desc_remain(item_val);
                                    inven_destroy(item_val);
                                  end;
			      End;
                            if (flags <> 0) then
                              with py.misc do
                                begin
                                  exp := exp + round(level/lev);
                                  prt_experience;
                                end;
                          end;
                      end
                  else
                    if (redraw) then draw_cave;
                end
            else
              msg_print('You are not carrying any disks.');
          end
	 else 
	   msg_print('But, you are not wielding a computer!')
      else
         msg_print('But you are not carrying anything.')
     END;
