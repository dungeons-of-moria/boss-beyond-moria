 {Following are skill procedure/functions
  These routines are commonly used in the scroll, potion, wands, and
  staves routines, and are occassionly called from other areas.
  Now included are creature spells also.
 
 Sleep creatures within radius from player,
 combining sleep II and sleep III spells - mwk,opusii 6/28/87}
[psect(misc6$code)] function sleep_monsters2_3(y,x,rad : integer) : boolean;
      VAR
         i1,i2,mk1,mk2,mk3,mk4: integer;
      BEGIN
        sleep_monsters2_3 := false;
        mk1:=y-rad;  mk2:=y+rad;  mk3:=x-rad;  mk4:=x+rad;
        if (mk1 < 1) then mk1:=1;
        if (mk2 >  max_height) then mk2:= max_height;
        if (mk3 < 1) then mk3:=1;
        if (mk4 > max_width) then mk4:=max_width;  {stay inbounds}
        for i1 := mk1 to mk2 do {last statement of procedure}
          for i2 := mk3 to mk4 do
            with cave[i1,i2] do        
              if (cptr > 1) then
	        with m_list[cptr], c_list[mptr] do
                    if (ml) then {visible}
{can(will) be slept}  if (uand(%X'1000',cdefense) = 0) then
                        BEGIN
                          if ( rad > 5 ) then
                            prt('',1,1) {let them guess for sleep III}
                          else
                            msg_print('The ' + name + ' snoozes for a while.');
                          sleep_monsters2_3 := true;
                          csleep := 500
                        END
{can't (quite) sleep} else
{give small chance}     if ( randint(20) > 18 ) then
                          BEGIN
                            if ( rad > 5 ) then
{let 'em guess for sleep III} prt('',1,1)
                            else
                              msg_print('The ' + name + ' goes into dreamland.');
                            sleep_monsters2_3 := true;
{better than none!}         csleep := 13 + randint(7)
                          END
                        else
                          if ( rad > 5 ) then
                            prt('',1,1)
                          else
                            msg_print('The ' + name + ' is unaffected.')
      END;
 
{ Detect any treasures on the current panel }
[psect(misc6$code)] function detect_treasure : boolean;
      VAR                                                        
         i1,i2  : integer;
      BEGIN
        detect_treasure := false;
        for i1 := panel_row_min to panel_row_max do
          for i2 := panel_col_min to panel_col_max do
            with cave[i1,i2] do
              if (tptr > 0) then
                if (t_list[tptr].tval = 100) then
                  if (not (test_light(i1,i2))) then
                    BEGIN
                      lite_spot(i1,i2);
                      tl := true;
                      detect_treasure := true
                    END
      END;
 
{ Detect all objects on the current panel }
[psect(misc6$code)] function detect_object : boolean;
      VAR
         i1,i2  : integer;
      BEGIN
        detect_object := false;
        for i1 := panel_row_min to panel_row_max do
          for i2 := panel_col_min to panel_col_max do
            with cave[i1,i2] do
              if (tptr > 0) then
                if (t_list[tptr].tval < 100) then
                  if (not (test_light(i1,i2))) then
                    BEGIN
                      lite_spot(i1,i2);
                      tl := true;
                      detect_object := true
                    END
      END;
 
{ Locates and displays traps on current panel }
[psect(misc6$code)] function detect_trap : boolean;
      VAR
         i1,i2 : integer;
      BEGIN
        detect_trap := false;
        for i1 := panel_row_min to panel_row_max do
          for i2 := panel_col_min to panel_col_max do
            with cave[i1,i2] do
              if (tptr > 0) then
                if (t_list[tptr].tval = 101) then
                  BEGIN
                    change_trap(i1,i2);
                    fm := true;
                    detect_trap := true
                  END
                else if (t_list[tptr].tval = 2) then
                  with t_list[tptr] do
                    known2(name)
      END;
 
{ Locates and displays all secret doors on current panel }
[psect(misc6$code)] function detect_sdoor : boolean;    
      VAR
         i1,i2 : integer;
      BEGIN
        detect_sdoor := false;
        for i1 := panel_row_min to panel_row_max do
          for i2 := panel_col_min to panel_col_max do
            with cave[i1,i2] do
              if (tptr > 0) then
{ Secret doors }
                if (t_list[tptr].tval = 109) then
                  BEGIN
                    fval := corr_floor3.ftval;
                    change_trap(i1,i2);
                    fm := true;
                    detect_sdoor := true
                  END
{ staircases }  else if (t_list[tptr].tval in [107,108]) then
                  if (not(fm)) then
                    BEGIN
                      fm := true;
                      lite_spot(i1,i2);
                      detect_sdoor := true
                    END
      END;
              
{ Locates and displays all invisible creatures on current panel }
[psect(misc6$code)] function detect_invisible : boolean;
      VAR
         i1 : integer;
         dummy : char;
         flag : boolean;
      BEGIN   
        flag := false;
        i1 := muptr;
        repeat
          with m_list[i1] do
            if (panel_contains(fy,fx)) then
              if (uand(%X'10000',c_list[mptr].cmove) <> 0) then
                BEGIN
                  ml := true;
                  print(c_list[mptr].cchar,fy,fx);
                  flag := true
                END;
          i1 := m_list[i1].nptr;
        until(i1 = 0);
        if (flag) then
          BEGIN
            msg_print('You sense the presence of invisible creatures!');
            msg_print(' ');
            msg_flag := false
          END;
        detect_invisible := flag
      END;
{ lights/darkens a circular area around the [y,x] of radius r.  --jeb}
[psect(misc6$code)] procedure make_light(y,x,r : integer; light : boolean);

      var d,ty,tx   : real;    {d = distance, ty,tx=temp real x,y}
	  i1,i2	    : integer; {i1,i2 = cave co-ordinates}
	  row,col   : integer;
	  sym_char  : char;
	  temp_char : dtype;
	  
      BEGIN
        ty := 1.0*y;
	tx := 1.0*x;
        for i1 := y-r to y+r do
	  for i2 := x-2*r to x+2*r do
	    if (in_bounds(i1,i2)) then
	      begin
	        d := sqrt(sqr(1.0*i1-ty)+sqr(0.7*(1.0*i2-tx)));  
	        if ((trunc(d) <= r) and 
	           (cave[i1,i2].fval in (pwall_set+floor_set))) then
	          begin
	            cave[i1,i2].pl := light;
		    row := i1 - panel_row_prt;  {convert [i1,i2] to screen pos}
		    col := i2 - panel_col_prt;
		    if ((row in [2..23]) and (col in [15..80])) then 
		      begin
		        if (light) then
		          loc_symbol(i1,i2,sym_char)
			else
			  sym_char := ' ';
			used_line[row] := true;
			temp_char := sym_char; 
			put_buffer(temp_char,row,col);
		      end;
	    	  end;
	      end;
      END;

{ Light an area: 1.  If corridor then light immediate area
                 2.  If room then light entire room. }
[psect(misc6$code)] function light_area(y,x : integer) : boolean;
      VAR
         i1,i2 		: integer;
	 radius		: integer;
	 out_val	: vtype;
      BEGIN
        writev(out_val,
     'You are surrounded by a ',colors[randint(max_colors)],' light.');
        msg_print(out_val);
        light_area := true;
        if ((cave[y,x].fval in [1,2]) and (dun_level > 0)) then
	  begin
	    radius := trunc((1.0*py.misc.lev) / 3.0);
	    if (radius < 3) then radius := 3;
	    if ((dun_level mod 10) in open_cave_set) then
              make_light(y,x,radius,true)
	    else
 	      light_room(y,x);
	    creatures(false);
	  end
        else
          for i1 := y-1 to y+1 do
            for i2 := x-1 to x+1 do
              if (in_bounds(i1,i2)) then
                BEGIN
                  if (not (test_light(i1,i2))) then
                    lite_spot(i1,i2);
                  cave[i1,i2].pl := true;
                END
      END;
 
{ Darken an area, opposite of light area }
[psect(misc6$code)] function unlight_area(y,x : integer) : boolean;
      VAR
        i1,i2,i3,tmp1,tmp2,start_row,start_col,END_row,END_col : integer;
        flag : boolean;
      BEGIN
        flag := false;
        if ((cave[y,x].fval in [1,2]) and (dun_level > 0)) then
          BEGIN
            tmp1 := trunc(screen_height/2);
            tmp2 := trunc(screen_width /2);
            start_row := trunc(y/tmp1)*tmp1 + 1;
            start_col := trunc(x/tmp2)*tmp2 + 1;
            end_row := start_row + tmp1 - 1;
            end_col := start_col + tmp2 - 1;
            for i1 := start_row to END_row do
              BEGIN
                out_val := '';
                i3 := 0;
                for i2 := start_col to END_col do
                  with cave[i1,i2] do
                    if (fval in [1,2]) then
                      BEGIN
                        pl := false;
                        fval := 1;
                        if (not(test_light(i1,i2))) then
                          BEGIN
                            if (i3 = 0) then
                              i3 := i2;
                            out_val := out_val + ' '
                          END
                        else if (i3 > 0) then
                          BEGIN
                            flag := true;
                            print(out_val,i1,i3);
                            out_val := '';
                            i3 := 0
                          END
                      END
                    else if (i3 > 0) then
                      BEGIN
                        flag := true;
                        print(out_val,i1,i3);
                        out_val := '';
                        i3 := 0
                      END;
                if (i3 > 0) then
                  BEGIN
                    flag := true;
                    print(out_val,i1,i3)
                  END
              END
          END
        else
          for i1 := y-1 to y+1 do
            for i2 := x-1 to x+1 do
              if (in_bounds(i1,i2)) then
                with cave[i1,i2] do
                  if (fval in [4,5,6]) then
                    if (pl) then
                      BEGIN
                        pl := false;
                        flag := true
                      END;
        if (flag) then
          BEGIN
            msg_print('Darkness pervades the area around you...');
            unlight_area := true
          END
        else          
          unlight_area := false
      END;
 
{ Map the current area plus some }
[psect(misc6$code)] function map_area : boolean;
      VAR
         i1,i2,i3,i4,i5,i6,i7,i8 : integer;
      BEGIN
        map_area := true;
        i1 := panel_row_min - randint(10);
        i2 := panel_row_max + randint(10);
        i3 := panel_col_min - randint(20);
        i4 := panel_col_max + randint(20);
        for i5 := i1 to i2 do
          for i6 := i3 to i4 do
            if (in_bounds(i5,i6)) then
              if (cave[i5,i6].fval in floor_set) then
                for i7 := i5-1 to i5+1 do
                  for i8 := i6-1 to i6+1 do
                    with cave[i7,i8] do
                      if (fval in pwall_set) then
                        pl := true
                      else if (tptr > 0) then
                        if (t_list[tptr].tval in light_set) then
                          fm := true;
        prt_map
      END;
 
{ Identify an object }
[psect(misc6$code)] function ident_spell : boolean;
      var
        item_val : integer;
        out_val : vtype;
        redraw : boolean;
      BEGIN
        ident_spell := false;
        redraw := false;
        if (get_item(item_val,'Identify which item?', redraw,1,inven_ctr)) then
          with inventory[item_val] do
            BEGIN
              ident_spell := true;
              identify(inventory[item_val]);
              known2(name);
              objdes(out_val,item_val,true,'i');
              msg_print(chr(item_val+96) + cur_char2(item_val)+' ' + out_val)
            END;
        if (redraw) then
          BEGIN
            msg_print(' ');
            draw_cave
          END
      END;    
 
{ Get all the monsters on the level pissed off. }
[psect(misc6$code)] function aggravate_monster(dis_affect : integer ) : boolean;
      VAR
         i1 : integer;
      BEGIN
        aggravate_monster := true;
        i1 := muptr;
        repeat
          with m_list[i1] do
            BEGIN
              csleep := 0;
              if (cdis <= dis_affect) then
                if (cspeed < 2) then
                  cspeed := cspeed + 1
            END;
          i1 := m_list[i1].nptr;
        until(i1 = 0)
      END;
 
{ Surround the fool with traps (chuckle) }
[psect(misc6$code)] function trap_creation : boolean;
      VAR
         i1,i2 : integer;
      BEGIN
        trap_creation := true;
        for i1 := char_row-1 to char_row+1 do
          for i2 := char_col-1 to char_col+1 do
            with cave[i1,i2] do
              if (fval in floor_set) then
                BEGIN
                  if (tptr > 0) then
                    delete_object(i1,i2);
                  place_trap(i1,i2,1,randint(max_trapa))
                END
      END;
 
{ Surround the player with doors. }
[psect(misc6$code)] function door_creation : boolean;
      VAR
         i1,i2,i3 : integer;
      BEGIN
        door_creation := true;
        for i1 := char_row-1 to char_row+1 do
          for i2 := char_col-1 to char_col+1 do
            if ((i1 <> char_row) or (i2 <> char_col)) then
              with cave[i1,i2] do
                if (fval in floor_set) then
                  BEGIN
                    popt(i3);
                    if (tptr > 0) then
                      delete_object(i1,i2);
                    fopen := false;
                    tptr := i3;
                    t_list[i3] := door_list[2];
                    if (test_light(i1,i2)) then
                      lite_spot(i1,i2)
                  END
      END;
 
{ Destroys any adjacent door(s)/trap(s) }
[psect(misc6$code)] function td_destroy : boolean;
      VAR
         i1,i2 : integer;
      BEGIN
        td_destroy := false;
        for i1 := char_row-1 to char_row+1 do
         for i2 := char_col-1 to char_col+1 do
          with cave[i1,i2] do
           if (tptr > 0) then
            if (t_list[tptr].tval in [101,102,104,105,109]) then
             BEGIN
              if (delete_object(i1,i2)) then
              td_destroy := true
             END
      END;
             
{ Display all creatures on the current panel }
[psect(misc6$code)] function detect_monsters : boolean;
      VAR
         i1,i2 : integer;
         flag : boolean;
      BEGIN
       flag := false;
       i1 := muptr;
       if (i1<>0) then
         repeat
           with m_list[i1] do
             if (panel_contains(fy,fx)) then
               if (uand(%X'10000',c_list[mptr].cmove) = 0) then
                 BEGIN
                   ml := true;
                   print(c_list[mptr].cchar,fy,fx);
                   flag := true
                 END;
             i1 := m_list[i1].nptr;
         until(i1 = 0);
       if (flag) then
        BEGIN
         msg_print('You detect life forms nearby...');
         msg_print(' ');
         msg_flag := false;
         detect_monsters := true
        END;
        detect_monsters := flag
      END;
 
 
{ Leave a line of light in given dir, blue light can sometimes hurt creatures.}
[psect(misc6$code)] function light_line(dir,y,x,dmg  : integer;
					     laser   : boolean) : boolean;
 BEGIN
   light_line := true;
   while (cave[y,x].fopen) do
     BEGIN
      with cave[y,x] do
        BEGIN
         if (panel_contains(y,x)) then
          BEGIN
           if (not((tl) or (pl))) then
             lite_spot(y,x);
           if (cptr > 1) then
              with m_list[cptr], c_list[mptr] do
               if (uand(%X'0100',cdefense) <> 0) then
                 begin
                   msg_print('The ' + name + ' wails out in pain!');
                   i1 := mon_take_hit(cptr,dmg);
                   if (i1 > 0) then
                     msg_print('The ' + name + ' buys the proverbial farm.');
                 end
	       else if (laser) then
	         begin
                   msg_print('The ' + name + ' is zapped by the laser!');
                   i1 := mon_take_hit(cptr,dmg);
                   if (i1 > 0) then
                     msg_print('The ' + name + ' sizzles and melts away.');
		 end;
          END;
         pl := true
        END;
      move(dir,y,x);
     END
 END;
 
 
{ Light line in all directions }
[psect(misc6$code)] function starlite(y,x : integer) : boolean;
      VAR
        i1 : integer;
      BEGIN
       msg_print('The end of the device bursts into a blue shimmering light.');
       starlite := true;
       for i1 := 1 to 9 do
        if (i1 <> 5) then
         light_line(i1,y,x,damroll('2d8'),false);
      END;
 
 
{ Disarms all traps/chests in a given direction }
[psect(misc6$code)] function disarm_all(dir,y,x : integer) : boolean;
      VAR
        i1,oldy,oldx : integer;
      BEGIN
       disarm_all := false;
       repeat
         with cave[y,x] do
           BEGIN
             if (tptr > 0) then
               with t_list[tptr] do
                 if (tval in [101,102]) then
                   BEGIN
                     if (delete_object(y,x)) then disarm_all := true;
                   END
                 else
                   if (tval = 105) then
                     BEGIN
                       p1 := 0
                     END
                   else
                     if (tval = 109) then
                       BEGIN
                         fval := corr_floor3.ftval;
                         change_trap(y,x);
                         fm := true;
                         disarm_all := true
                       END
                     else
                       if (tval = 2) then
                         if (flags > 0) then
                           BEGIN
                             msg_print('Click!');
                             flags := 0;
                             disarm_all := true;
                             i1 := index(name,' (');
                             if (i1 > 0) then
                               name := substr(name,1,i1-1);
                             name := name + ' (Unlocked)';
                             known2(name)
                           END
           END;
           oldy := y;
           oldx := x;
           move(dir,y,x);
       until(not(cave[oldy,oldx].fopen))

      END;
 
{ Return flags for given type area affect }
[psect(misc6$code)] procedure get_flags (
                        typ : integer; VAR weapon_type,harm_type : integer;
                        VAR destroy : obj_set );
      BEGIN
       CASE typ of
{ Lightning }1  :  BEGIN
               weapon_type := %X'00080000';
               harm_type   := %X'0100';
               destroy     := [45,60,65]
              END;
{ Gas }      2  :  BEGIN
               weapon_type := %X'00100000';
               harm_type   := %X'0040';
               destroy     := []
              END;
{ Acid }     3  :  BEGIN
               weapon_type := %X'00200000';
               harm_type   := %X'0080';
              destroy := [12,20,21,22,30,31,32,33,34,35,36,55,70,71,80,104,105]
              END;
{ Frost }    4  :  BEGIN
               weapon_type := %X'00400000';
               harm_type   := %X'0010';
               destroy     := [75,76]
              END;
{ Fire }     5  :  BEGIN
               weapon_type := %X'00800000';
               harm_type   := %X'0020';
               destroy := [12,20,21,22,30,31,32,36,55,70,71,75,76,80,104,105]
              END;
{ Anti-Mut } 6  :  BEGIN
               weapon_type := %X'00000000';
               harm_type   := %X'0004';
               destroy     := []
              END;
        otherwise BEGIN
                   weapon_type := 0;
                   harm_type   := 0;
                   destroy     := []
                  END
       END
      END;
 
 
{ Shoot a bolt in a given direction }
[psect(misc6$code)] function fire_bolt  (
                        typ,dir,y,x,dam : integer;
                        bolt_typ        : ctype ) : boolean;
      VAR
        i1,oldy,oldx,dist,weapon_type,harm_type : integer;
        flag : boolean;
        mwkchar: char;
        dummy : obj_set;
        diestr: vtype;
      BEGIN
       fire_bolt := true;
       flag := false;
       get_flags(typ,weapon_type,harm_type,dummy);
       oldy := y;
       oldx := x;
       dist := 0;
       repeat
        move(dir,y,x);
        if (test_light(oldy,oldx)) then
         lite_spot(oldy,oldx)
        else
         unlite_spot(oldy,oldx);
         dist := dist + 1;
         if (dist > obj$bolt_range ) then
          flag := true
         else
          BEGIN
          with cave[y,x] do
           BEGIN
           if (fopen) then
            BEGIN
            if (cptr > 1) then
             BEGIN
             flag := true;
             with m_list[cptr], c_list[mptr] do
               BEGIN
                msg_print('The '+bolt_typ+' strikes the '+ c_list[mptr].name);
                if (uand(harm_type,cdefense) <> 0) then
                 dam := dam*2
                else if (uand(weapon_type,spells) <> 0) then
                      dam := trunc(dam/4.0);
                      i1 := mon_take_hit(cptr,dam);
                      if (i1 > 0) then
		        begin
		          case (randint(5)) of
                            1: diestr := ' ceases to live.';
			    2: diestr := ' is no longer.';
			    3: diestr := ' dies agonizingly.';
			    4: diestr := ' expires with a sigh.';
			    5: diestr := ' collapses in a heap.';
			  end;
                          msg_print('The '+c_list[i1].name + diestr);
			end
                      else
                       BEGIN
                          if (panel_contains(y,x)) then
                        BEGIN
                        print(c_list[mptr].cchar,y,x);
                        m_list[cptr].ml := true
                        END
               END
             END
            END
            else if (panel_contains(y,x)) then
                  BEGIN
                  CASE dir of
                    1,9: mwkchar:='/';
                    2,8: mwkchar:='|';
                    3,7: mwkchar:='\';
                    4,6: mwkchar:='-'
                   END;
                   print(mwkchar,y,x)
                   END
             END
            else
             flag := true
           END;
           oldy := y;
           oldx := x
          END;
       until (flag)
      END;

  {Declared forward in Main.inc}
  {Blow up a certain (y,x) Painful is true when it hurts the player.}
{[psect(misc6$code)]} procedure explosion; {(typ,y,x,dam_hp,radius : integer;
					 painful	       : boolean;
				         descrip	       : ctype);}
   VAR
        i1,i2,i3,dam,thit,tkill		: integer;
        weapon_type,harm_type		: integer;
        flag				: boolean;
        destroy				: obj_set;
   BEGIN
        thit   := 0;
	tkill  := 0;
	get_flags(typ,weapon_type,harm_type,destroy);
        for i1 := y-radius to y+radius do
          for i2 := x-radius to x+radius do
            if (in_bounds(i1,i2)) then
              if (distance(y,x,i1,i2) <= radius) then
	        if (los(y,x,i1,i2)) then
                  with cave[i1,i2] do
                    BEGIN
                      if (tptr > 0) then
                        if (t_list[tptr].tval in destroy) then
			  delete_object(i1,i2);
                        if (fopen) then
			  BEGIN
			    if (panel_contains(i1,i2)) then 
			      print('*',i1,i2);
                            if ((painful) and (cptr = 1)) then
                              BEGIN
                                dam := trunc(dam_hp/(distance(i1,i2,y,x)+1));
                                  CASE typ of
				    0 : take_hit(dam,descrip);
                                    1 : light_dam(dam,descrip);
				    2 : poison_gas(dam,descrip);
				    3 : acid_dam(dam,descrip);
				    4 : cold_dam(dam,descrip);
				    5 : fire_dam(dam,descrip)
				  END;
			      END;
			    if (cptr > 1) then
			      with m_list[cptr], c_list[mptr] do
				BEGIN
				 thit := thit + 1;
				 dam := dam_hp;
				 if (uand(harm_type,cdefense) <> 0) then
				   dam := dam*2
				 else if (uand(weapon_type,spells) <> 0) then
				   dam := dam div 2;
				 dam := trunc(dam/(distance(i1,i2,y,x)+1));
				 i3 := mon_take_hit(cptr,dam);
				 if (i3 > 0) then
				   tkill := tkill + 1
				 else
				   BEGIN
				     if (panel_contains(i1,i2)) then
				       begin
				         print(cchar,i1,i2);
					 ml := true
				       end
				     END
				  END
                          END
                    END;
        for i1 := (y - radius) to (y + radius) do
	  for i2 := (x - radius) to (x + radius) do
	    if (in_bounds(i1,i2)) then
	      if (panel_contains(i1,i2)) then
	        if (distance(y,x,i1,i2) <= radius) then
		  BEGIN
		    with cave[i1,i2] do
		      if (test_light(i1,i2)) then
		        lite_spot(i1,i2)
		      else if (cptr = 1) then
                        lite_spot(i1,i2)
                      else if (cptr > 1) then
                        if (m_list[cptr].ml) then
                          lite_spot(i1,i2)
                        else
                          unlite_spot(i1,i2)
                      else
                        unlite_spot(i1,i2);
	          END;
        if (thit = 1) then
          msg_print('The ' + descrip + ' envelopes a creature!')
        else if (thit > 1) then
          msg_print('The ' + descrip + ' envelopes several creatures!');
        if (tkill = 1) then
          msg_print('The ' + descrip + ' kills a creature!')
        else if (tkill > 1) then
          msg_print('Several monsters expire in the attack!');
   END;
 
 
{ Shoot a ball in a given direction.  Note that balls have an area affect. }
[psect(misc6$code)] function fire_ball (typ,dir,y,x     : integer;
				        dam_hp,radius	: integer;
				        painful		: boolean;
				        descrip		: ctype ) : boolean;
      VAR
        dam,oldy,oldx,dist	: integer;
        flag			: boolean;
      BEGIN
        fire_ball := true;
	flag := false;
	oldy := y;
	oldx := x;
	dist := 0;
	repeat
	  move(dir,y,x);
	  dist := dist + 1;
	  if (test_light(oldy,oldx)) then
	    lite_spot(oldy,oldx)
	  else
	    unlite_spot(oldy,oldx);
	  if (dist > obj$bolt_range) then
	    flag := true
	  else
	    BEGIN
	      with cave[y,x] do
	        BEGIN
		  if ((not(fopen)) or (cptr > 1)) then
		    begin
		      flag := true;
		      if (not(fopen)) then
		      begin
		        y := oldy;
			x := oldx
		      end;
		      explosion(typ,y,x,dam_hp,2,painful,descrip);
		   end
                 else if (panel_contains(y,x)) then
                   print('*',y,x)
                END;
              oldy := y;
	      oldx := x
          END;
        until (flag)
      END;
 
 
{ Recharge a wand, staff, or rod.  Sometimes the item breaks. }
[psect(misc6$code)] function recharge(num : integer) : boolean;
      VAR
         item_val :integer;
         redraw : boolean;
      BEGIN
        recharge := false;
        redraw := false;
        if (get_item(item_val,'Recharge which item?',redraw,1,inven_ctr)) then
          with inventory[item_val] do
            if (tval in [55,60,65]) then
              if (randint(8) = 1) then
                BEGIN
                  recharge := true;
                  msg_print('The item melts in your hand...');
                  inven_destroy(item_val)
                END
              else
                BEGIN
                  recharge := true;
                  num := trunc(num/(level+2));
                  p1 := p1 + 2 + randint(num);
                  if (index(name,'^') = 0) then {obj already identified}
                    insert_str(name,' (%P1',' (%P1')
{obj not ident'd} else
                    insert_str(name,'^ (%P1','^ (%P1')
                END;
        if (redraw) then
          BEGIN
            msg_print(' ');
            draw_cave
          END
      END;
 
{ Increase or decrease a creatures hit points }
[psect(misc6$code)] function hp_monster(dir,y,x,dam : integer) : boolean;
      VAR
         i1 : integer;
         flag : boolean;
      BEGIN
        hp_monster := false;
        flag := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            BEGIN
              if (fopen) then
                BEGIN
                  if (cptr > 1) then
                    BEGIN
                      flag := true;
 		      with m_list[cptr], c_list[mptr] do
                          BEGIN
                            hp_monster := true;
                            i1 := mon_take_hit(cptr,dam);
                            if (i1 > 0) then
        msg_print('The ' + c_list[i1].name + ' suffers from a major coronary.')
                            else
                              msg_print('The '+name+' says "thanks, buddy!"')
                          END
                    END
                END
              else
                flag := true
            END;
        until (flag)
      END;

{ Drains life }              
[psect(misc6$code)] function drain_life(dir,y,x : integer) : boolean;
      VAR
         i1 : integer;
         flag : boolean;
      BEGIN
        drain_life := false;
        flag := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            BEGIN
              if (fopen) then
                BEGIN
                  if (cptr > 1) then
                    BEGIN
                      flag := true;
 		      with m_list[cptr], c_list[mptr] do
                          if (uand(cdefense,%X'0008') = 0) then
                            BEGIN
                              drain_life := true;
                              i1 := mon_take_hit(cptr,50);
                              if (i1 > 0) then
        msg_print('The ' + c_list[i1].name + ' dies smiling.')
                              else
        msg_print('The ' + name + ' gets a boo-boo too big for a band-aid.')
                            END
                    END
                END
              else
                flag := true
            END;
        until (flag)
      END;
 
{ Increase or decrease a creatures speed }
[psect(misc6$code)] function speed_monster(dir,y,x,spd : integer) : boolean;
      VAR
         flag : boolean;
      BEGIN
        speed_monster := false;
        flag := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            BEGIN
              if (fopen) then
                BEGIN
                  if (cptr > 1) then
                    BEGIN
                      flag := true;
 		      with m_list[cptr], c_list[mptr] do
                          if (spd > 0) then
                            BEGIN
                              cspeed := cspeed + spd;
                              csleep := 0;
                            END
                          else if (randint(max_mons_level) > level) then
                            BEGIN
                              cspeed := cspeed + spd;
                              csleep := 0;
			      msg_print('The '+name+' looks blurry.');
                            END
                          else
                            msg_print('The ' + name + ' is unaffected.');
                      speed_monster := true;
                    END
                END
              else
                flag := true
            END;
        until (flag)
      END;
 
{ Confuse a creature }
[psect(misc6$code)] function confuse_monster(dir,y,x : integer) : boolean;
      VAR
          flag : boolean;
      BEGIN
        confuse_monster := false;
        flag := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            if (fopen) then
              BEGIN
                if (cptr > 1) then
 		  with m_list[cptr], c_list[mptr] do
                      BEGIN
                        confuse_monster := true;
                        flag := true;
                        if ((randint(max_mons_level) < level) or
                         (uand(%X'1000',cdefense) <> 0)) then
                          msg_print('The ' + name + ' is unaffected.')
                        else
                          BEGIN
                            confused := true;
                            csleep := 0;
                            msg_print('The ' + name + ' appears confused.')
                          END
                      END
              END
            else
              flag := true;
        until (flag)
      END;
 
{ Sleep a creature. }
[psect(misc6$code)] function sleep_monster(dir,y,x : integer) : boolean;
      VAR
          flag : boolean;
      BEGIN
        sleep_monster := false;
        flag := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            if (fopen) then
              BEGIN
                if (cptr > 1) then
 		  with m_list[cptr], c_list[mptr] do
                      BEGIN
                        sleep_monster := true;
                        flag := true;
                        if ((randint(max_mons_level) < level) or
                         (uand(%X'1000',cdefense) <> 0)) then
                          msg_print('The ' + name + ' is unaffected.')
                        else
                          BEGIN
                            csleep := 500;
                            msg_print('The ' + name + ' snores blissfuly...')
                          END
                      END
              END
            else
              flag := true;
        until (flag)
      END;
 
{ Turn stone to mud, delete wall. }
[psect(misc6$code)] function wall_to_mud(dir,y,x : integer) : boolean;
      VAR
          i1 : integer; 
          out_val : vtype;
          flag : boolean;
      BEGIN
        wall_to_mud := false;
        flag := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            if (in_bounds(y,x)) then
              BEGIN
                if (fval in wall_set) then
                  BEGIN
                    flag := true;
                    twall(y,x,1,0);
                    if (test_light(y,x)) then
                      BEGIN
                        msg_print('The wall erodes away');
                        wall_to_mud := true
                      END
                  END
                else if ((tptr > 0) and (not(fopen))) then
                  BEGIN
                    flag := true;
                    if (panel_contains(y,x)) then
                      if (test_light(y,x)) then
                        BEGIN
                          temporary_slot := t_list[tptr];
                          objdes(out_val,1,false,'t');
                          msg_print('The ' + out_val + ' erodes away.');
                          wall_to_mud := true
                        END;
                    delete_object(y,x)
                  END;
                if (cptr > 1) then
 		  with m_list[cptr], c_list[mptr] do
                      if (uand(%X'0200',cdefense) <> 0) then
                        BEGIN
                          i1 := mon_take_hit(cptr,100);
                          flag := true;
                          if (ml) then
                            if (i1 > 0) then
                        msg_print('The ' + name + ' flops once, and dies.')
                            else
                              msg_print('The ' + name + ' melts away.');
                        END
              END
            else
              flag := true;
        until (flag)
      END;
 
 
{ Destroy all traps and doors in a given direction }
[psect(misc6$code)] function td_destroy2(dir,y,x : integer) : boolean;
      BEGIN
        td_destroy2 := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            if (tptr > 0) then
              with t_list[tptr] do
                if (tval in [2,101,102,105,109]) then
                  BEGIN
                    if (delete_object(y,x)) then
                      BEGIN
                        msg_print('There is a bright flash of light!');
                        fopen := true;
                        td_destroy2 := true
                      END
                  END;
        until (not(cave[y,x].fopen))
      END;
 
{ Polymorph a monster }
[psect(misc6$code)] function poly_monster(dir,y,x : integer) : boolean;
      VAR
          dist : integer;
          flag : boolean;
      BEGIN
        poly_monster := false;
        flag := false;
        dist := 0;
        repeat
          move(dir,y,x);
          dist := dist + 1;
          if (dist < obj$bolt_range) then
            BEGIN
              with cave[y,x] do
                BEGIN
                  if (fopen) then
                    BEGIN
                      if (cptr > 1) then
                        with c_list[m_list[cptr].mptr] do
                          if (randint(max_mons_level) > level) then
                            BEGIN
                              flag := true;
                              delete_monster(cptr);
                              place_monster(y,x,
                        randint(m_level[max_mons_level])+ m_level[0],false);
                              if (panel_contains(y,x)) then
                                if (test_light(y,x)) then
                                  poly_monster := true;
                            END
                          else
                            msg_print('The ' + name + ' is unaffected.')
                    END
                  else
                    flag := true
                END
            END
          else
            flag := true;
        until (flag)
      END;
 
{ Create a wall. }
[psect(misc6$code)] function build_wall(dir,y,x : integer) : boolean;
      VAR
          i1 : integer;
      BEGIN
        build_wall := false;
        i1 := 0;
        move(dir,y,x);
        while ((cave[y,x].fopen) and (i1 < 10)) do
          with cave[y,x] do
            BEGIN
              if (tptr > 0) then
                delete_object(y,x);
              if (cptr > 1) then
                mon_take_hit(cptr,damroll('2d8'));
              fval  := rock_wall2.ftval;
              fopen := rock_wall2.ftopen;
              fm := false;
              if (test_light(y,x)) then
                lite_spot(y,x);
              i1 := i1 + 1;
              build_wall := true;
              move(dir,y,x)
            END
      END;           
 
{ Replicate a creature }
[psect(misc6$code)] function clone_monster(dir,y,x : integer) : boolean;
      VAR
        flag : boolean;
      BEGIN
        flag := false;
        clone_monster := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            if (cptr > 1) then
	      with m_list[cptr] do
                Begin
                  multiply_monster(y,x,mptr,cur_reprod,false);
                  if (panel_contains(y,x)) then
                    if (ml) then
                      clone_monster := true;
                  flag := true
                End
        until ((not(cave[y,x].fopen)) or (flag))
      END;
 
{ Move the creature record to a new location }
[psect(misc6$code)] function teleport_away(monptr,dis : integer) : boolean;
      VAR
          yn,xn,ctr : integer;
      BEGIN
        with m_list[monptr] do
          BEGIN
            ctr := 0;
            repeat
              repeat
                yn := fy + (randint(2*dis+1) - (dis + 1));
                xn := fx + (randint(2*dis+1) - (dis + 1));
              until(in_bounds(yn,xn));
              ctr := ctr + 1;
              if (ctr > 9) then
                BEGIN
                  ctr := 0;
                  dis := dis + 5
                END;
            until ((cave[yn,xn].fopen) and (cave[yn,xn].cptr = 0));
            move_rec(fy,fx,yn,xn);
            if (test_light(fy,fx)) then
              lite_spot(fy,fx);
            fy := yn;
            fx := xn;
            ml := false;
            teleport_away := true
          END
      END;
 
{ Teleport player to spell casting creature }
[psect(misc6$code)] function teleport_to(ny,nx : integer) : boolean;
      VAR
        dis,ctr,y,x,i1,i2 : integer;
      BEGIN
        teleport_to := true;
        dis := 1;
        ctr := 0;
        repeat
          y := ny + (randint(2*dis+1) - (dis + 1));
          x := nx + (randint(2*dis+1) - (dis + 1));
          ctr := ctr + 1;
          if (ctr > 9) then
            BEGIN
              ctr := 0;
              dis := dis + 1
            END;
        until ((cave[y,x].fopen) and (cave[y,x].cptr < 2));
        move_rec(char_row,char_col,y,x);
        for i1 := char_row-1 to char_row+1 do
          for i2 := char_col-1 to char_col+1 do
            with cave[i1,i2] do
              BEGIN
                tl := false;
                if (not(test_light(i1,i2))) then
                  unlite_spot(i1,i2)
              END;
        if (test_light(char_row,char_col)) then
          lite_spot(char_row,char_col);
        char_row := y;
        char_col := x;
        move_char(5);
        creatures(false)
      END;
 
{ Teleport all creatures in a given direction away }
[psect(misc6$code)] function teleport_monster(dir,y,x : integer) : boolean;
      VAR
          yn,xn : integer;
          flag : boolean;
      BEGIN
        flag := false;
        teleport_monster := false;
        repeat
          move(dir,y,x);
          with cave[y,x] do
            if (cptr > 1) then
              BEGIN
                teleport_away(cptr,max_sight);
                teleport_monster := true
              END;
        until ((not(cave[y,x].fopen)) or (flag))
      END;
              
{ Delete all creatures within MAX_SIGHT distance }
[psect(misc6$code)] function mass_genocide : boolean;
      VAR     
         i1,i2 : integer;
      BEGIN
        mass_genocide := false;
        i1 := muptr;
        repeat
          with m_list[i1], c_list[mptr] do
              BEGIN
                i2 := nptr;
                if (cdis <= max_sight) then
                  if (uand(cdefense,%X'80000000') = 0) then
                    BEGIN
                      delete_monster(i1);
                      mass_genocide := true
                    END
              END;
          i1 := i2;
        until(i1 = 0)
      END;
 
{ Delete all creatures of a given type from level. }
[psect(misc6$code)] function genocide : boolean;
      VAR
          i1,i2 : integer;
          typ : char;
      BEGIN
        genocide := true;
        i1 := muptr;
        if (get_com('Which type of creature do wish exterminated?',typ)) then
          repeat
	    with m_list[i1], c_list[mptr] do
                BEGIN
                  i2 := nptr;
                  if (typ = c_list[mptr].cchar) then
                    if (uand(cdefense,%X'80000000') = 0) then
                      delete_monster(i1)
                    else
                      msg_print('The ' + name + ' is unaffected.')
                END;
            i1 := i2;
          until(i1 = 0)
      END;
 
{ Change speed of any creature player can see. }
[psect(misc6$code)] function speed_monsters(spd : integer) : boolean;
      VAR
         i1,i2 : integer;
      BEGIN
        i1 := muptr;
        speed_monsters := false;
        repeat
          with m_list[i1] do
            BEGIN
              i2 := nptr;
              if (ml) then
                with c_list[mptr] do
                  if (spd > 0) then
                    BEGIN
                      cspeed := cspeed + spd;
                      csleep := 0;
                      speed_monsters := true
                    END
                  else if (randint(max_mons_level) > level) then
                    BEGIN
                      cspeed := cspeed + spd;
                      csleep := 0;
                      speed_monsters := true
                    END
                  else
                    msg_print('The ' + name + ' changes speed...');
            END;
          i1 := i2;
        until(i1 = 0)
      END;
                       
{ Polymorph any creature that player can see. }
[psect(misc6$code)] function mass_poly : boolean;
      VAR
          i1,i2,y,x : integer;
      BEGIN
        i1 := muptr;
        mass_poly := false;
        repeat
          with m_list[i1] do
            BEGIN
              i2 := nptr;
              if (cdis < max_sight) then
                with c_list[mptr] do
                  if (uand(cdefense,%X'80000000') = 0) then
                    BEGIN
                      y := fy;
                      x := fx;
                      delete_monster(i1);
       place_monster(y,x,randint(m_level[max_mons_level]) + m_level[0],false);
                      mass_poly := true
                    END
            END;
          i1 := i2;
        until(i1 = 0)
      END;
 
{ Display evil creatures on current panel }
[psect(misc6$code)] function detect_evil : boolean;
      VAR
          i1 : integer;
          dummy : char;
          flag : boolean;
      BEGIN
        flag := false;
        i1 := muptr;
        repeat
          with m_list[i1] do
            if (panel_contains(fy,fx)) then
              if (uand(%X'0004',c_list[mptr].cdefense) <> 0) then
                BEGIN
                  ml := true;
                  print(c_list[mptr].cchar,fy,fx);
                  flag := true
                END;
          i1 := m_list[i1].nptr;
        until(i1 = 0);
        if (flag) then
          BEGIN
            msg_print('You detect scum nearby!');
            msg_print(' ');
            msg_flag := false
          END;
        detect_evil := flag
      END;
 
{ Change players hit points in some manner }
[psect(misc6$code)] function hp_player(num : integer; kind : vtype) : boolean;
      BEGIN
        hp_player := false;
        with py.misc do
          BEGIN
            if (num < 0) then
              BEGIN
                take_hit(abs(num),kind);
                if (chp < 0) then
                  msg_print('You feel your life slipping away!');
                hp_player := true
              END
            else if (chp < mhp) then
              BEGIN
                chp := chp + num;
                if (chp > mhp) then
                  chp := mhp;
                prt_chp;
                CASE trunc(num/5) of
                  0      :  msg_print('You feel a little better.');
                  1,2    :  msg_print('You feel better.');
                  3,4,5,6:  msg_print('You feel much better.');
                  otherwise msg_print('You feel very good.')
                END;
                hp_player := true
              END
          END
      END;
 
{ Cure players confusion }
[psect(misc6$code)] function cure_confusion : boolean;
      BEGIN
        cure_confusion := false;
        with py.flags do
          if (confused > 1) then
            BEGIN
              confused := 1;
              cure_confusion := true
            END
      END;
 
{ Cure players blindness }
[psect(misc6$code)] function cure_blindness : boolean;
      BEGIN
        cure_blindness := false;
        with py.flags do
          if (blind > 1) then
            BEGIN
              blind := 1;
              cure_blindness := true
            END
      END;
 
{ Cure poisoning }
[psect(misc6$code)] function cure_poison : boolean;
      BEGIN
        cure_poison := false;
        with py.flags do
          if (poisoned > 1) then
            BEGIN
              poisoned := 1;
              cure_poison := true
            END
      END;

{ Cure disease. - RLG}
[psect(misc6$code)] function cure_disease : boolean;
      BEGIN
        cure_disease := false;
        with py.flags do
          if (diseased > 1) then
            BEGIN
              diseased := 1;
              cure_disease := true
            END
      END;
 
{ Cure the players fear }
[psect(misc6$code)] function remove_fear : boolean;
      BEGIN
        remove_fear := false;
        with py.flags do
          if (afraid > 1) then
            BEGIN
              afraid := 1;
              remove_fear := true
            END
      END;
 
{ This is a fun one.  In a given block, pick some walls and
  turn them into open spots.  Pick some open spots and turn
  them into walls.  An "Earthquake" effect.}
[psect(misc6$code)] function earthquake : boolean;
      VAR
          i1,i2 : integer;
      BEGIN
        earthquake := true;
        for i1 := char_row-8 to char_row+8 do
          for i2 := char_col-8 to char_col+8 do
            if ((i1 <> char_row) or (i2 <> char_col)) then
              if (in_bounds(i1,i2)) then
                if (randint(8) = 1) then
                  BEGIN
                    with cave[i1,i2] do
                      BEGIN
                        if (tptr > 0) then
                          delete_object(i1,i2);
                        if (cptr > 1) then
                          mon_take_hit(cptr,damroll('2d8'));
                        if (fval in wall_set) then
                          BEGIN
                            if (next_to4(i1,i2,[1,2]) > 0) then
                              BEGIN
                                fval  := corr_floor2.ftval;
                                fopen := corr_floor2.ftopen
                              END
                            else
                              BEGIN
                                fval  := corr_floor1.ftval;
                                fopen := corr_floor1.ftopen
                              END;
                            if (test_light(i1,i2)) then unlite_spot(i1,i2);
                            pl := false;
                            fm := false;
                            if (tl) then lite_spot(i1,i2)
                          END
                        else if (fval in floor_set) then
                          BEGIN
                            CASE randint(10) of
                              1,2,3,4,5 : BEGIN
                                            fval  := rock_wall3.ftval;
                                            fopen := rock_wall3.ftopen
                                          END;
                              6,7,8     : BEGIN
                                            fval  := rock_wall2.ftval;
                                            fopen := rock_wall2.ftopen
                                          END;
                              9,10      : BEGIN
                                            fval  := rock_wall1.ftval;
                                            fopen := rock_wall1.ftopen
                                          END
                            END;
                            fm := false
                          END
                      END;
                    if (test_light(i1,i2)) then
                      lite_spot(i1,i2)
                  END
      END;
 
{ Evil creatures don't like this.}
[psect(misc6$code)] function protect_evil : boolean;
      BEGIN
        protect_evil := true;
        with py.flags do
          protevil := protevil + randint(25) + 3*py.misc.lev
      END;
 
{ Create some high quality mush for the player. }
[psect(misc6$code)] function create_food : boolean;
      BEGIN
        create_food := true;
        with cave[char_row,char_col] do
          BEGIN
            if (tptr > 0) then
              delete_object(char_row,char_col);
            place_object(char_row,char_col);
            t_list[tptr] := mush
          END
      END;
 
                
{ Attempts to destroy a type of creature.  Success depends on
  the creatures level VS. the player's level}
[psect(misc6$code)] function dispell_creature(
                                cflag   : integer;
                                damage  : integer) : boolean;
      VAR
        i1,m_next : integer;
      BEGIN
        i1 := muptr;
        dispell_creature := false;
        repeat
          m_next := m_list[i1].nptr;
          with m_list[i1] do
            if (ml) then
              if (uand(cflag,c_list[mptr].cdefense) <> 0) then
                  BEGIN
                    hp := hp - randint(damage);
                    csleep := 0;
                    if (hp < 0) then
                      BEGIN
                        msg_print('The ' + c_list[mptr].name + ' dissolves!');
                        monster_death(fy,fx,c_list[mptr].cmove);
                        with c_list[mptr] do
                          with py.misc do
                            exp := exp + round(mexp*(level/lev));
                        delete_monster(i1)
                      END
                    else
                      msg_print('The ' + c_list[mptr].name + ' shudders.');
                    dispell_creature := true
                  END;
          i1 := m_next;
        until(i1 = 0)
      END;
 
{ Attempt to turn (confuse) undead creatures.}
[psect(misc6$code)] function turn_undead : boolean;
      VAR
          i1 : integer;
      BEGIN
        i1 := muptr;
        turn_undead := false;
        repeat
          with m_list[i1] do
            if (panel_contains(fy,fx)) then
              if (ml) then
                if (uand(%X'0008',c_list[mptr].cdefense) <> 0) then
                  BEGIN
                    if (((py.misc.lev+1) > c_list[mptr].level) or
                        (randint(5) = 1)) then
                      BEGIN
                  msg_print('The ' + c_list[mptr].name + ' runs frantically!');
                        confused := true
                      END
                    else
                      msg_print('The ' + c_list[mptr].name +' is unaffected.');
                    turn_undead := true
                  END;
          i1 := m_list[i1].nptr;
        until(i1 = 0)
      END;
 
{ Leave a glyph of warding... Creatures will not pass over!}
[psect(misc6$code)] function warding_glyph : boolean;
      VAR
          i1 : integer;
      BEGIN
        warding_glyph := true;
        with cave[char_row,char_col] do
          if (tptr = 0) then
            BEGIN
              popt(i1);
              tptr := i1;
              t_list[i1] := scare_monster
            END
      END;
 
{ Lose a strength point. }
[psect(misc6$code)] function lose_str : boolean;
      BEGIN
        lose_str := true;
        if (not(py.flags.sustain_str)) then
          BEGIN
            with py.stat do
              cstr := de_statp(cstr);
            msg_print('You feel rather sick.');
            prt_strength
          END
        else
          msg_print('You feel sick for a moment, then it passes.')
      END;
 
{ Lose an intelligence point. }
[psect(misc6$code)] function lose_int : boolean;
      BEGIN
        lose_int := true;
        if (not(py.flags.sustain_int)) then
          BEGIN
            with py.stat do
              cint := de_statp(cint);
            msg_print('You become very dizzy.');
            prt_intelligence
          END
        else
          msg_print('You become dizzy for a moment, then it passes.')
      END;
 
{ Lose a wisdom point. }
[psect(misc6$code)] function lose_wis : boolean;
      BEGIN
        lose_wis := true;
        if (not(py.flags.sustain_wis)) then
          BEGIN
            with py.stat do
              cwis := de_statp(cwis);
            msg_print('You feel more naive.');
            prt_wisdom
          END
        else
          msg_print('You feel senseless for a moment, then it passes.')
      END;
 
{ Lose a dexterity point.}
[psect(misc6$code)] function lose_dex : boolean;
      BEGIN
        lose_dex := true;
        if (not(py.flags.sustain_dex)) then
          BEGIN
            with py.stat do
              cdex := de_statp(cdex);
            msg_print('You feel like a klutz.');
            prt_dexterity
          END
        else
          msg_print('You feel like a klutz for a moment, then it passes.')
      END;
 
{ Lose a constitution point.}
[psect(misc6$code)] function lose_con : boolean;
      BEGIN
        lose_con := true;
        if (not(py.flags.sustain_con)) then
          BEGIN
            with py.stat do
              ccon := de_statp(ccon);
            msg_print('You feel very sick.');
            prt_constitution
          END
        else
          msg_print('You feel sick for a moment, then it passes.')
      END;
 
{ Lose a charisma point.}
[psect(misc6$code)] function lose_chr : boolean;
      BEGIN
        lose_chr := true;
        if (not(py.flags.sustain_chr)) then
          BEGIN
            with py.stat do
              cchr := de_statp(cchr);
            msg_print('You get a bad case of acne.');
            prt_charisma
          END
        else
          msg_print('Your skin starts to itch, but feels better now.')
      END;
 
{ Lose experience }
[psect(misc6$code)] procedure lose_exp(amount : integer);
    var
        i1,i2,av_hp,lose_hp,av_mn,lose_mn : integer;
        flag : boolean;
    BEGIN
      if (py.misc.lev > 40) then 
        msg_print('But the feeling is only temporary!')
      else with py.misc do
        BEGIN
          if (amount > exp) then
            exp := 0
          else
            exp := exp - amount;
          i1 := 1;
          while (trunc(player_exp[i1]*expfact) <= exp) do
            i1 := i1 + 1;
          i2 := lev - i1;
          while (i2 > 0) do
            BEGIN
              av_hp := trunc(mhp/lev);
              av_mn := trunc(mana/lev);
              lev   := lev - 1;
              i2    := i2 - 1;
              lose_hp := randint(av_hp*2-1);
              lose_mn := randint(av_mn*2-1);
              mhp  := mhp  - lose_hp;
              mana := mana - lose_mn;
              if (mhp  < 1) then mhp  := 1;
              if (mana < 0) then mana := 0;
              if (pskill <> 0) then
                BEGIN
                  i1 := max_skills;
                  flag := false;
                  repeat
                    i1 := i1 - 1;                 
		    flag := skill_list[i1].learned;
                  until((flag) or (i1 < 2));
                  if (flag) then
                    BEGIN
		      begin
		        skill_list[i1].learned := false;
                        if (pskill in [1,2,7]) then
                          msg_print('You have forgotten a tech skill!')
                        else if (pskill in [3,4]) then
                          msg_print('You have forgotten a prayer!')
			else if (pskill in [5,6]) then
			  msg_print ('You have forgotten a trick');
		     end;
                   END;
              END;
            END;
          if (chp   > mhp)  then chp   := mhp;
          if (cmana > mana) then cmana := mana;
          title := player_title[pclass,lev];
          prt_experience;
          prt_mhp;
          prt_chp;
          prt_mana;
          prt_level;
          prt_title
        END
    END;
 
{ Slow Poison }
[psect(misc6$code)] function slow_poison : boolean;
    BEGIN
      slow_poison := false;
      with py.flags do
        if (poisoned > 0) then
          BEGIN
            poisoned := trunc(poisoned/2.0);
            if (poisoned < 1) then poisoned := 1;
            slow_poison := true;
            msg_print('The effects of the poison has been reduced.')
          END
    END;
 
{ Bless}
[psect(misc6$code)] function bless(amount : integer) : boolean;
    BEGIN
      bless := true;
      with py.flags do
        blessed := blessed + amount
    END;
 
{ Detect Invisible for period of time }
[psect(misc6$code)] function detect_inv2(amount : integer) : boolean;
    BEGIN
      detect_inv2 := true;
      with py.flags do
        detect_inv := detect_inv + amount
    END;
 
{ The spell of destruction.}
[psect(misc6$code)] function destroy_area(y,x : integer) : boolean;
    VAR
        i1,i2,i3 : integer;
 
    PROCEDURE replace_spot(y,x,typ : integer);
      BEGIN
        with cave[y,x] do
          BEGIN
            CASE typ of
              1,2,3  :  BEGIN
                          fval  := corr_floor1.ftval;
                          fopen := corr_floor1.ftopen
                        END;
              4,7,10 :  BEGIN
                          fval  := rock_wall1.ftval;
                          fopen := rock_wall1.ftopen
                        END;
              5,8,11 :  BEGIN
                          fval  := rock_wall2.ftval;
                          fopen := rock_wall2.ftopen
                        END;
              6,9,12 :  BEGIN
                          fval  := rock_wall3.ftval;
                          fopen := rock_wall3.ftopen
                        END
            END;
            pl := false;
            fm := false;
            if (tptr > 0) then
              delete_object(y,x);
            if (cptr > 1) then
              delete_monster(cptr)
          END
      END;
 
    BEGIN
      destroy_area := true;
      if (dun_level > 0) then
        BEGIN
          for i1 := (y-15) to (y+15) do
            for i2 := (x-15) to (x+15) do
              if (in_bounds(i1,i2)) then
                if (cave[i1,i2].fval <> 15) then
                  BEGIN
                    i3 := distance(i1,i2,y,x);
                    if (i3 < 13) then
                      replace_spot(i1,i2,randint(6))
                    else if (i3 < 16) then
                      replace_spot(i1,i2,randint(9))
                  END
        END;
      msg_print('There is a searing blast of light!');
      py.flags.blind := py.flags.blind + 10 + randint(10)
    END;
 
 
{ Enchants a plus onto an item.}
[psect(misc6$code)] function enchant(var pluses : worlint) : boolean;
    VAR
        chance : integer;
    BEGIN
      chance := 0;
      enchant := false;
      if (pluses > 0) then
        CASE pluses of
          1  :  chance := 040;
          2  :  chance := 100;
          3  :  chance := 200;
          4  :  chance := 400;
          5  :  chance := 600;
          6  :  chance := 700;
          7  :  chance := 800;
          8  :  chance := 900;
          9  :  chance := 950;
          otherwise chance := 995
        END;
      if (randint(1000) > chance) then
        BEGIN
          pluses := pluses + 1;
          enchant := true
        END
    END;
 
 
{ Removes curses from items in equipment}
[psect(misc6$code)] function remove_curse : boolean;
    VAR
        i1 : integer;
    BEGIN
      remove_curse := false;
      for i1 := 23 to 34 do
        with equipment[i1] do
          if (uand(%X'80000000',flags) <> 0) then
            BEGIN
              flags := uand(%X'7FFFFFFF',flags);
              py_bonuses(blank_treasure,0);
              remove_curse := true
            END
    END;
 
 
{ Restores any drained experience }
[psect(misc6$code)] function restore_level : boolean;
    BEGIN
      restore_level := false;
      with py.misc do
        if (max_exp > exp) then
          BEGIN
            restore_level := true;
            msg_print('You feel your life energies returning...');
            while (exp < max_exp) do
              BEGIN
                exp := max_exp;
                prt_experience
              END
          END
    END;

{Complex routine to handle modifications on a player's wielded weapon}
[psect(misc6$code)] function enchant_weapon(how : dtype) : boolean;

      VAR
           out_val	: vtype;
	   failure	: boolean;      

      BEGIN
        enchant_weapon := true;
        failure := true;
        with equipment[23] do
          if (tval > 0) then
   	    if (how = 'curse') then
              begin
                temporary_slot := equipment[23];
                objdes(out_val,23,false,'e');
                msg_print('Your ' + out_val + ' glows black, then fades.');
		if (tohit > 0) then
		  tohit := 0
		else
		  tohit := tohit-1;
		if (todam > 0) then
		  todam := 0
		else
		  todam := todam-1;
                py_bonuses(temporary_slot,-1);
              end
	    else	       
              begin                 
                objdes(out_val,23,false,'e');
                msg_print('Your ' + out_val + ' glows faintly!');
		if ((how = 'tohit') or (how = 'both')) then
                  if (enchant(tohit)) then
                    begin
                      flags := uand(%X'7FFFFFFF',flags);
                      py_bonuses(blank_treasure,0);
		      failure := false; 
                    end;
		if ((how = 'todam') or (how = 'both')) then
                  if (enchant(todam)) then
                    begin
                      flags := uand(%X'7FFFFFFF',flags);
                      py_bonuses(blank_treasure,0);
		      failure := false;
                    end;
                if (failure) then
                  msg_print('It appears to be unaffected...');
              end;
    END;


[psect(misc6$code)] function enchant_armor(how : dtype) : boolean;

      VAR
      	   tmp		: array [1..6] of integer;
      	   i3,i4	: integer;
           out_val	: vtype;
	   failure	: boolean;      

      BEGIN
        enchant_armor := true;
        i3 := 0;
        i4 := 0;
        if (equipment[26].tval > 0) then
          begin
            i3 := i3 + 1;
            tmp[i3] := 26;
          end;
        if (equipment[27].tval > 0) then
          begin
            i3 := i3 + 1;
            tmp[i3] := 27;
          end;
        if (equipment[31].tval > 0) then
          begin
            i3 := i3 + 1;
            tmp[i3] := 31;
          end;
        if (equipment[32].tval > 0) then
          begin
            i3 := i3 + 1;
            tmp[i3] := 32;
          end;
        if (equipment[28].tval > 0) then
          begin
            i3 := i3 + 1;
            tmp[i3] := 28;
          end;
        if (equipment[24].tval > 0) then
          begin
            i3 := i3 + 1;
            tmp[i3] := 24;
          end;
        if (i3 > 0) then i4 := tmp[randint(i3)];
	if (how <> 'curse') then
	  begin
            if      (uand(%X'80000000',equipment[26].flags) <> 0) then
              i4 := 26
            else if (uand(%X'80000000',equipment[27].flags) <> 0) then
              i4 := 27
            else if (uand(%X'80000000',equipment[31].flags) <> 0) then
              i4 := 31
            else if (uand(%X'80000000',equipment[32].flags) <> 0) then
              i4 := 32
            else if (uand(%X'80000000',equipment[24].flags) <> 0) then
              i4 := 24
            else if (uand(%X'80000000',equipment[28].flags) <> 0) then
              i4 := 28;
	  end;
          if (i4 > 0) then
            with equipment[i4] do
	      if (how = 'curse') then
		begin
                  temporary_slot := equipment[i3];
                  objdes(out_val,i3,false,'e');
                  msg_print('Your ' + out_val + ' glows black, then fades.');
                  flags := %X'80000000';
                  toac := -randint(5) - randint(5);
                  py_bonuses(temporary_slot,-1);
		end
	      else
                begin
                  objdes(out_val,i4,false,'e');
                  msg_print('Your ' + out_val + ' glows faintly!');
                  if (enchant(toac)) then
                    begin
                      flags := uand(%X'7FFFFFFF',flags);
                      py_bonuses(blank_treasure,0);
                    end
                  else
                    msg_print('It appears unaffected...');
                end;
      END;



