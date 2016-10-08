{ Eat some food.}
[psect(misc2$code)] procedure eat;
      var
         i1 : unsigned;
         i2,i3,item_val : integer;
         out_val : vtype;
         redraw,ident : boolean;
      BEGIN
        reset_flag := true;
        if (inven_ctr > 0) then
          BEGIN
            if (find_range([80],i2,i3)) then
              BEGIN
                redraw := false;
               if (get_item(item_val,'Eat which food?',redraw,i2,i3)) then
                  with inventory[item_val] do
                    BEGIN
                      if (redraw) then draw_cave;
                      reset_flag := false;
                      i1 := flags;
                      ident := false;
                      while (i1 > 0) do
                        BEGIN
                          i2 := bit_pos(i1);
{ Foods }
        CASE (i2) of
          1  :  with py.flags do
                  BEGIN
                    poisoned := poisoned + randint(10) + level;
                    Opusii_vomit(35);
                    ident := true;
                  END;
          2  :  with py.flags do
                  BEGIN
                    blind := blind + randint(250) + 10*level + 100;
                    draw_cave;
                    msg_print('A veil of darkness surrounds you.');
                    ident := true;
                  END;
          3  :  with py.flags do
                  BEGIN
                    afraid := afraid + randint(10) + level;
                    msg_print('You feel terrified!');
                    ident := true;
                  END;
          4  :  with py.flags do
                  BEGIN
                    confused := confused + randint(10) + level;
                    msg_print('You feel drugged.');
                  END;
          5  :  with py.flags do
                  image := image + randint(200) + 25*level + 200;
          6  :  ident := cure_poison;
          7  :  ident := cure_blindness;
          8  :  with py.flags do
                  if (afraid > 1) then
                    BEGIN
                      afraid := 1;
                      ident := true;
                    END;
          9  :  ident := cure_confusion;
          10 :  ident := lose_str;
          11 :  ident := lose_con;
          12 :  ident := lose_int;
          13 :  ident := lose_wis;
          14 :  ident := lose_dex;
          15 :  ident := lose_chr;
          16 :  with py.stat do
                  if (str > cstr) then
                    BEGIN
                      cstr := str;
                      msg_print('You feel your strength returning.');
                      prt_strength;
                      ident := true;
                    END;
          17 :  with py.stat do
                  if (con > ccon) then
                    BEGIN
                      ccon := con;
                      msg_print('You feel your health returning.');
                      prt_constitution;
                      ident := true;
                    END;
          18 :  with py.stat do
                  if (py.stat.int > cint) then
                    BEGIN
                      cint := py.stat.int;
                      msg_print('Your head spins a moment.');
                      prt_intelligence;
                      ident := true;
                    END;
          19 :  with py.stat do
                  if (wis > cwis) then
                    BEGIN
                      cwis := wis;
                      msg_print('You feel your wisdom returning.');
                      prt_wisdom;
                      ident := true;
                    END;
          20 :  with py.stat do
                  if (dex > cdex) then
                    BEGIN
                      cdex := dex;
                      msg_print('You feel more dextrous.');
                      prt_dexterity;
                      ident := true;
                    END;
          21 :  with py.stat do
                  if (chr > cchr) then
                    BEGIN
                      cchr := chr;
                      msg_print('Your skins starts itching.');
                      prt_charisma;
                      ident := true;
                    END;
          22 :  ident := hp_player(randint(3),'food.');
          23 :  ident := hp_player(randint(6),'food.');
          24 :  ident := hp_player(randint(12),'food.');
          25 :  ident := hp_player(damroll('3d6'),'food.');
          26 :  ident := hp_player(damroll('3d12'),'food.');
          27 :  ident := hp_player(-randint(4),'poisonous food.');
          28 :  ident := hp_player(-randint(8),'poisonous food.');

          29 :  ident := hp_player(-damroll('2d8'),'poisonous food.');
          30 :  ident := hp_player(-damroll('3d8'),'poisonous food.');
          31 :  with py.misc do
                  BEGIN
                    mhp := mhp - 1;
                    if (mhp < chp) then
                      chp := mhp;
                    take_hit(1,'poisonous food.');
                    prt_mhp;
                    prt_chp;
                    ident := true;
                  END;
          otherwise ;
        END;
{ End of food actions.}
                        END;
                      if (ident) then
                        identify(inventory[item_val]);
                      if (flags <> 0) then
                        with py.misc do
                          BEGIN
                            exp := exp + round(level/lev);
                            prt_experience;
                          END;
                      add_food(p1);
                      py.flags.status := uand(%X'FFFFFFFC',py.flags.status);
                      prt_hunger;
                      prt_cur_weight;
                      desc_remain(item_val);
                      inven_destroy(item_val);
                    END
                else
                  if (redraw) then draw_cave;
              END
            else       
              msg_print('You are not carrying any food.');
          END
        else
          msg_print('But you are not carrying anything.');
      END;
