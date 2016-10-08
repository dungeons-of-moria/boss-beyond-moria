{ play a trick }
[psect(misc2$code)] procedure use_con_skills;
      var
        i1,i2,item_val,dir  : integer;
        choice,chance : integer;
        dumy,y_dumy,x_dumy  : integer;
        redraw  : boolean;
BEGIN
  reset_flag := true;
  if (py.flags.blind > 0) then
    msg_print('Help me, I''m blind!')
  else if (no_light) then
    msg_print('Look buddy, no light, no read, go it?')
  else if (py.flags.confused > 0) then
    msg_print('Your mind is way too messed up right now...')
  else if (py.misc.pskill in [5,6]) then
    if (inven_ctr > 0) then
      BEGIN
        if (find_range([92],i1,i2)) then
          BEGIN
            redraw := false;
            if (get_item(item_val,'Use which con-manual?',
                                                  redraw,i1,i2)) then
              BEGIN
                if (use_skill('Try which trick?',item_val,
                                     choice,chance,redraw)) then
                  with skill_list[choice] do
                    BEGIN
                      reset_flag := false;
                      if (randint(100) < chance) then
                        msg_print('You are not very convincing!')
                      else
                        BEGIN
                          y_dumy := char_row;
                          x_dumy := char_col;
{Tricks. }
  CASE choice of
    1  :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            fire_bolt(0,dir,char_row,char_col,
                                  damroll('2d4')+1,'Spit Ball');
    2  :  detect_monsters;
    3  :  teleport(10);
    4  :  light_area(char_row,char_col);
    5  :  hp_player(damroll('4d4'),'a magic spell.');
    6  :  BEGIN
            detect_sdoor;
            detect_trap;
          END;
    7  :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            fire_ball(2,dir,char_row,char_col,7,2,false,'Stinking Gas');
    8  :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            confuse_monster(dir,char_row,char_col);
    9 :   if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            fire_bolt(1,dir,char_row,char_col,
                                  damroll('3d6')+1,'Terrible Insult');
    10 :  td_destroy;
    11 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            sleep_monster(dir,char_row,char_col);
    12 :  cure_poison;
    13 :  teleport(py.misc.lev*4);
    14 :  remove_curse;
    15 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            fire_bolt(4,dir,char_row,char_col,
                                  damroll('4d6')+1,'Sharp Ice Cube');
    16 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            wall_to_mud(dir,char_row,char_col);
    17 :  ident_spell;
    18 :  recharge(16);
    19 :  sleep_monsters2_3(char_row,char_col,1);
    20 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            poly_monster(dir,char_row,char_col);
    21 :  create_food;
    22 :  sleep_monsters2_3(char_row,char_col,20);
    23 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            fire_bolt(5,dir,char_row,char_col,
                                  damroll('6d6')+1,'Roman Candle');
    24 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            speed_monster(dir,char_row,char_col,-2);		  
    25 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            fire_ball(4,dir,char_row,char_col,
                                  damroll('6d4')+1,2,false,'Deadly Ice Cubes');
    26 :  recharge(40);
    27 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            teleport_monster(dir,char_row,char_col);
    28 :  with py.flags do
            fast := fast + randint(20) + py.misc.lev*2;
    29 :  if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
            fire_ball(5,dir,char_row,char_col,
                                  damroll('7d5')+1,2,false,'Blazing Inferno');
    30 :  destroy_area(char_row,char_col);
    31 :  genocide;
    otherwise ;
  END;
{End of tricks.}
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
                                msg_print('You start to stutter!');
                                py.flags.paralysis :=
                                  randint(5*trunc(smana-cmana));
                                cmana := 0;
                                if (randint(3) = 1) then
				  begin
			  msg_print('You have damaged your reputation!');
				    py.stat.cchr := de_statp(py.stat.cchr);
				    prt_charisma;
				  end;
                              END
                            else
                              cmana := cmana - smana;
                            prt_mana;
                          END;
		      if (randint(10) = 1) then 
		        reset_flag := true;  
                    END
              END
            else
              if (redraw) then draw_cave;
          END
        else
          msg_print('But you are not carrying any con-manuals!');
      END
    else
      msg_print('But you are not carrying any con-manuals!')
  else
    msg_print('You aren''t tricky enough to confuse a tourist!');
END;
 
