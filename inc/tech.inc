{ Throw a magic spell } 
[psect(misc2$code)] procedure use_tech_skills;
      var
        i1,i2,item_val,dir  : integer;
        choice,chance : integer;
        dumy,y_dumy,x_dumy  : integer;
        redraw  : boolean;
      BEGIN
        reset_flag := true;
        if (py.flags.blind > 0) then
          msg_print('You can''t see to read your tech manual!')
        else if (no_light) then
          msg_print('You have no light to read by.')
        else if (py.flags.confused > 0) then
          msg_print('You are too confused...')
        else if (py.misc.pskill in [1,2,7]) then
          if (inven_ctr > 0) then
            BEGIN
              if (find_range([90],i1,i2)) then
                BEGIN
                  redraw := false;
                  if (get_item(item_val,'Use which tech manual?',
                                                        redraw,i1,i2)) then
                    BEGIN
                      if (use_skill('Do which skill?',item_val,
                                           choice,chance,redraw)) then
                        with skill_list[choice] do
                          BEGIN
                            reset_flag := false;
                            if (randint(100) < chance) then
                              msg_print('You failed to perform the skill!')
                            else
                              BEGIN
                                y_dumy := char_row;
                                x_dumy := char_col;
{ Spells. }
        CASE choice of
          1  :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  fire_bolt(0,dir,char_row,char_col,
                                        damroll('2d6')+1,'Energy Bolt');
          2  :  detect_monsters;
          3  :  teleport(10);
          4  :  light_area(char_row,char_col);
          5  :  hp_player(damroll('4d4'),'a magic spell.');
          6  :  BEGIN
                  detect_sdoor;
                  detect_trap;
                END;
          7  :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
		  fire_ball(2,dir,char_row,char_col,9,2,false,'Poison Gas');
          8  :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  confuse_monster(dir,char_row,char_col);
          9 :   if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  fire_bolt(1,dir,char_row,char_col,
                                        damroll('3d8')+1,'Electric Shock');
          10 :  td_destroy;
          11 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  sleep_monster(dir,char_row,char_col);
          12 :  cure_poison;
          13 :  teleport(py.misc.lev*5);
          14 :  remove_curse;
          15 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  fire_bolt(4,dir,char_row,char_col,
                                        Damroll('4d8')+1,'Killer Snowball');
          16 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  wall_to_mud(dir,char_row,char_col);
          17 :  ident_spell;
          18 :  recharge(20);
          19 :  sleep_monsters2_3(char_row,char_col,1);
          20 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  poly_monster(dir,char_row,char_col);
          21 :  create_food;
          22 :  sleep_monsters2_3(char_row,char_col,20);
          23 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  fire_bolt(5,dir,char_row,char_col,
                                        damroll('6d8')+1,'Flame-Thrower');
          24 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  speed_monster(dir,char_row,char_col,-1);
	  25 :  py.flags.ring_blast := 10; {Ring of Frost}
          26 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  fire_ball(4,dir,char_row,char_col,
	  			damroll('6d6')+1,2,false,'Frost Ball');
          27 :  recharge(50);
          28 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  teleport_monster(dir,char_row,char_col);
          29 :  with py.flags do
                  fast := fast + randint(20) + py.misc.lev;
	  30 :  py.flags.ring_blast := 9; {Ring of Fire}
          31 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
                  fire_ball(5,dir,char_row,char_col,
	  			damroll('7d7')+1,2,false,'Fire Ball');
          32 :  destroy_area(char_row,char_col);
          33 :  genocide;
          otherwise ;
        END;
 { END of spells.}
                                if (not(reset_flag)) then
                                  with py.misc do
                                    BEGIN
                                      exp := exp + sexp;
                                      prt_experience;
                                      sexp := 0;
                                    END
                              END;
                            with py.misc do
                              if (not(reset_flag)) then
                                BEGIN
                                  if (smana > cmana) then
                                    BEGIN
                                      msg_print('You faint from the effort!');
                                      py.flags.paralysis :=
                                        randint(5*trunc(smana-cmana));
                                      cmana := 0;
                                      if (randint(3) = 1) then
                                BEGIN
                                  msg_print('You have damaged your health!');
                                  py.stat.ccon := de_statp(py.stat.ccon);
                                  prt_constitution;
                                END;
                                    END
                                  else
                                    cmana := cmana - smana;
                                  prt_mana;
                                END;
			    if (randint(10)=1) then 
			      reset_flag := true;
                          END
                    END
                  else
                    if (redraw) then draw_cave;
                END
              else
                msg_print('But you are not carrying any tech manuals!');
            END
          else
            msg_print('But you are not carrying any tech manuals!')
        else
          msg_print('Invalid Command.');
      END;
                  
