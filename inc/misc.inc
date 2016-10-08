{ Use date and time to produce random seed }
[psect(setup$code)] function get_seed : unsigned;
    type
        $quad = [quad,unsafe] record
                l0      : unsigned;
                l1      : unsigned;
        end;
    var
        time     : $quad;
        seed_val : unsigned;
 
    [asynchronous,external (SYS$GETTIM)] function get_time(
                var time : $quad) : integer;
                external;

    begin
      get_time(time);                           { Current time  }
      seed_val := uor(time.l0,time.l1);         { Random number }
      get_seed := uor(seed_val,%X'00000001');   { Odd number    }
    end;
 

{ returns true if there is a line of sight from [y1,x1] to [y2,x2] -jeb }
{ NOTE:  THIS ROUTINE IS NOT AT ALL EVEN A LITTLE BIT DEBUGGED OR TESTED}
{ But it has lasted awhile without complaint.... -RLG}
[psect(misc4$code)] function clear_path(y1,x1,y2,x2 : integer) : boolean;
      var 
        i3		  : integer;  {loop counter                }
        i1,i2  		  : integer;  {current place under scrutiny}
	r1,r2		  : real;     {real equivalents of i1,i2   }	
	dist		  : real;     {distance from pos1 to pos2  }
	x_vector,y_vector : real;     {amount to move per step     }
      begin
        clear_path := true;
	dist := sqrt(sqr(1.0*(y1-y2))+sqr(1.0*(x1-x2)));
	y_vector := (y2-y1)/dist;
	x_vector := (x2-x1)/dist;
	for i3 := 1 to trunc(dist) do
	  begin
	    i1 := y1 + trunc(1.0*y1+1.0*i3*y_vector);
	    i2 := x1 + trunc(1.0*x1+1.0*i3*x_vector);
	    if (not (((i1 = y1) and (i2 = x1)) or 
	       ((i1 = y2) and (i2 = x2)))) then
	       if (cave[i1,i2].fval in wall_set) then clear_path := false;
	  end;
      end;

{ Centers a string within a #field length character string }
[psect(misc1$code)] function fill_str (p1 : vtype; field : integer) : vtype;
      var
        s1 	: vtype;
        i1,i2	: integer;
      BEGIN
        s1 := '';
        i1 := trunc(length(p1) / 2);
	i2 := field div 2;
        fill_str := substr(pad(s1,' ',i2-i1) + pad(p1,' ',field),1,field);
      END;
 
{ An often needed function.  Returns the Lesser of two integers }
[psect(misc4$code)] function lesser(a,b : integer) : integer;
   begin
     if (a >= b) then
       lesser := b
     else
       lesser := a;
   end;

{ Returns the Greater of two integers. }
[psect(misc4$code)] function greater(a,b : integer) : integer;
   begin
     if (a >= b) then
       greater := a
     else
       greater := b;
   end;


{ Routine that draws the bus.  See also 'take_bus' in train.inc }
[psect(misc4$code)] procedure draw_the_bus;
var
	str  :  vtype;
	f1   :  text;
	i1   : integer;
BEGIN
  open (f1,file_name:=BUS_PIC,record_length:=80,history:=old,error:=continue);
  reset(f1);
  if (status(f1) <> 0) then
    begin
      writev(str,'You endure a long bus ride and arrive in ',
		   townlist[town_num].town_name);
      msg_print(str);
    end
  else
    begin
      clear(1,1);
      for i1 := 1 to 24 do
	begin
	  readln(f1,str);
	  prt(str,i1,1);
	end;
      prt( fill_str(townlist[town_num].town_name,21) + '|',17,52);
      pause(24);
    end;
  close(f1);
END;

{ Computes current weight limit }
[psect(misc4$code)] function weight_limit : integer;
   begin
     weight_limit := lesser(3000, py.stat.cstr*player_weight_cap + py.misc.wt);
   end;
 
{ Returns the day number; 1=Sunday...7=Saturday }
[psect(setup$code)] function day_num : integer;
    var
        i1       : integer;
    [external(LIB$DAY)] function getday(
        var daynum       : integer;
        dum1      : integer := %immed 0;
        dum2      : integer := %immed 0) : integer;
        external;
    begin
      getday(i1);
      day_num := ((i1+3) mod 7) + 1;
    end;
 
 
{ Returns the hour number; 0=midnight...23=11 PM }
[psect(setup$code)] function hour_num : integer;
    var
        hour : integer;
        time_str : packed array [1..11] of char;
    begin
      time(time_str);
      readv(substr(time_str,1,2),hour);
      hour_num := hour;
    end;
 
 
{ Check the day-time strings to see if open }
[psect(setup$code)] function check_time : boolean;
    begin
      case days[day_num,(hour_num+5)] of
        '.'  :  check_time := false;    { Closed }
        'X'  :  check_time := true;     { Normal hours }
        otherwise check_time := false;  { Other, assumed closed }
      end;
    end;
 
 
{ Generates a random integer x where 1<=X<=MAXVAL }
  function randint ( %immed maxval : integer ) : integer;
        external;
 
  function rand_rep ( %immed num : integer; %immed die : integer ) : integer;
        external;
 
 
{ Generates a random integer number of NORMAL distribution }
[psect(misc1$code)] function randnor(mean,stand : integer) : integer;
    begin
      randnor :=  trunc(sqrt(-2.0*ln(randint(9999999)/10000000.0))*
                  cos(6.283*(randint(9999999)/10000000.0))*stand) + mean;
    end;
 
 
{ Returns position of first set bit }
  function bit_pos ( %ref  test : unsigned ) : integer;
        external;
 
 
{ Checks a co-ordinate for in bounds status }
[psect(misc1$code)] function in_bounds(y,x : integer) : boolean;
    begin
      if ((y > 1) and (y < cur_height) and
          (x > 1) and (x < cur_width)) then
        in_bounds := true
      else
        in_bounds := false;
    end;
 
 
{ Distance between two points }
  function distance (
                      %immed y1       : integer;
                      %immed x1       : integer;
                      %immed y2       : integer;
                      %immed x2       : integer  ) : integer;
        external;
 
 
{ Checks points north, south, east, and west for a type }
[psect(misc1$code)] function next_to4 ( y,x : integer; group_set : obj_set ) : integer;
    var
        i1 : integer;
    begin
      i1 := 0;
      if (y > 1) then
        if (cave[y-1,x].fval in group_set) then
          i1 := i1 + 1;
      if (y < cur_height) then
        if (cave[y+1,x].fval in group_set) then
          i1 := i1 + 1;
      if (x > 1) then
        if (cave[y,x-1].fval in group_set) then
          i1 := i1 + 1;
      if (x < cur_width) then
        if (cave[y,x+1].fval in group_set) then
          i1 := i1 + 1;
      next_to4 := i1
    end;
 
 
{ Checks all adjacent spots for elements }
[psect(misc1$code)] function next_to8 (
                        y,x : integer;
                        group_set : obj_set ) : integer;
    var
        i1,i2,i3 : integer;
    begin
      i1 := 0;
      for i2 := (y - 1) to (y + 1) do
        for i3 := (x - 1) to (x + 1) do
          if (in_bounds(i2,i3)) then
            if (cave[i2,i3].fval in group_set) then
              i1 := i1 + 1;
      next_to8 := i1
    end;
 
 
{ Link all free space in treasure list together }
[psect(generate$code)] procedure tlink;
      var
        i1 : integer;
      begin
        for i1 := 1 to max_talloc do
          begin
            t_list[i1] := blank_treasure;
            t_list[i1].p1 := i1 - 1;
          end;
        tcptr := max_talloc;
      end;
 
 
{ Link all free space in monster list together }
[psect(generate$code)] procedure mlink;
      var
        i1 : integer;
      begin
        for i1 := 1 to max_malloc do
          begin
            m_list[i1] := blank_monster;
            m_list[i1].nptr := i1 - 1;
          end;
        m_list[2].nptr := 0;
        muptr := 0;
        mfptr := max_malloc;
      end;
 
 
{ Initializes M_LEVEL array for use with PLACE_MONSTER }
[psect(setup$code)] procedure init_m_level;
    var
        i1,i2,i3 : integer;
    begin
      i1 := 1;   {main counter, goes by step through c_list}
      i2 := 0;          {counter for level}
      i3 := max_creatures - extra_monsters;  {max value for main counter}
      repeat
        m_level[i2] := 0;
        while ((i1 <= i3) and (c_list[i1].level = i2)) do
          begin
            m_level[i2] := m_level[i2] + 1;
            i1 := i1 + 1;
          end;
        i2 := i2 + 1;
      until (i2 > max_mons_level);
      for i1 := 2 to max_mons_level do
        m_level[i1] := m_level[i1] + m_level[i1-1];
    end;
 
 
{ Initializes T_LEVEL array for use with PLACE_OBJECT }
[psect(setup$code)] procedure init_t_level;
    var
        i1,i2 : integer;
    begin
      i1 := 1;
      i2 := 0;
      repeat
        while ((i1 <= max_objects) and (object_list[i1].level = i2)) do
          begin
            t_level[i2] := t_level[i2] + 1;
            i1 := i1 + 1;
          end;
        i2 := i2 + 1;
      until ((i2 > max_obj_level) or (i1 > max_objects));
      for i1 := 1 to max_obj_level do
        t_level[i1] := t_level[i1] + t_level[i1-1];
    end;
 
 { Adjust prices of objects }
  procedure price_adjust;
    var
        i1 : integer;
    begin
      for i1 := 1 to max_objects do
        with object_list[i1] do
          cost := trunc(cost*cost_adj + 0.99);
      for i1 := 1 to inven_init_max do
        with inventory_init[i1] do
          cost := trunc(cost*cost_adj + 0.99);
    end;
 
 
{ Converts input string into a dice roll
  Normal input string will look like '2d6', '3d8'... ect. }
[psect(misc1$code)] function damroll(dice : dtype) : integer;
    var
        i1,num,sides : integer;
    begin
      for i1 := 1 to length(dice) do
        if (dice[i1] = 'd') then
          dice[i1] := ' ';
      num := 0;
      sides := 0;
      readv(dice,num,sides,error:=continue);
      damroll := rand_rep(num,sides);
    end;
 
 
{ Returns true if no obstructions between two given points }
[psect(misc1$code)] function los(y1,x1,y2,x2 : integer) : boolean;
    var
        ty,tx,stepy,stepx,p1,p2 : integer;
        slp,tmp : real;
        flag : boolean;
    begin
      ty := (y1 - y2);
      tx := (x1 - x2);
      flag := true;
      if ((ty <> 0) or (tx <> 0)) then
        begin
          if (ty < 0) then
            stepy := -1
          else
            stepy := 1;
          if (tx < 0) then
            stepx := -1
          else
            stepx := 1;
          if (ty = 0) then
            repeat
              x2 := x2 + stepx;
              flag := cave[y2,x2].fopen;
            until((x1 = x2) or (not (flag)))
          else if (tx = 0) then
            repeat
              y2 := y2 + stepy;
              flag := cave[y2,x2].fopen;
            until((y1 = y2) or (not (flag)))
          else if (abs(ty) > abs(tx)) then
            begin
              slp := abs(tx/ty)*stepx;
              tmp := x2;
              repeat
                y2 := y2 + stepy;
                tmp := tmp + slp;
                p1 := round(tmp - 0.1);
                p2 := round(tmp + 0.1);
                if (not ((cave[y2,p1].fopen) or (cave[y2,p2].fopen))) then
                  flag := false;
              until((y1 = y2) or (not (flag)))
            end
          else
            begin
              slp := abs(ty/tx)*stepy;
              tmp := y2;
              repeat
                x2 := x2 + stepx;
                tmp := tmp + slp;
                p1 := round(tmp - 0.1);
                p2 := round(tmp + 0.1);
                if (not ((cave[p1,x2].fopen) or (cave[p2,x2].fopen))) then
                  flag := false;
              until((x1 = x2) or (not (flag)))
            end;
        end;
      los := flag;
    end;
 
 
{ Returns symbol for given row, column }
[psect(misc5$code)] procedure loc_symbol(y,x : integer; var sym : char);
    Begin
      with cave[y,x] do
        if ((cptr = 1) and (not(find_flag))) then
          sym := '@'
        else if (py.flags.blind > 0) then
          sym := ' '
        else
          Begin
            if (tptr > 0) then
              if (t_list[tptr].tval <> 109) then
		sym := t_list[tptr].tchar
	      else if (wall_flag) then
	        sym := wall_chr[(dun_level mod 10) +1]
	      else 
	        sym := '#'
            else if (fval < 10) then
              sym := '.'
            else if (wall_flag) then
	      sym := wall_chr[(dun_level mod 10) +1]
	    else
	      sym := '#';
            if (cptr > 1) then
              begin
                with m_list[cptr] do
                  if ((ml) and
                      ((uand(c_list[mptr].cmove,%X'00010000') = 0) or
                       (py.flags.see_inv))) then
                    sym := c_list[mptr].cchar
              end;
          End;
    END;
 
 
{ Tests a spot for light or field mark status }
[psect(misc1$code)] function test_light(y,x : integer) : boolean;
    begin
      with cave[y,x] do
        if ((pl) or (fm) or (tl)) then
          test_light := true
        else
          test_light := false;
    end;
 
 
{ Prints the map of the dungeon }
[psect(misc2$code)] procedure prt_map;
    var
        i1,i2,i3,i4,i5,ypos,xpos,isp   : integer;
        floor_str       : vtype;
        tmp_char : char;
        flag     : boolean;
    begin
      redraw := false;  { Screen has been redrawn   }
      i3 := 1;          { Used for erasing dirty lines  }
      i4 := 14;         { Erasure starts in this column }
      for i1 := panel_row_min to panel_row_max do       { Top to bottom }
        begin
          i3 := i3 + 1;                 { Increment dirty line ctr }
          if (used_line[i3]) then       { If line is dirty... }
            begin
              erase_line(i3,i4);        { erase it. }
              used_line[i3] := false;   { Now it's a clean line }
            end;
          floor_str := ''; { Floor_str is string to be printed}
          ypos := i1;      { Save row }
          flag := false;   { False until floor_str <> ''   }
          isp := 0;        { Number of blanks encountered  }
          for i2 := panel_col_min to panel_col_max do   { Left to right }
            with cave[i1,i2] do
              begin        { Get character for location    }
                if (test_light(i1,i2)) then
                  loc_symbol(i1,i2,tmp_char)
                else if ((cptr = 1) and (not(find_flag))) then
                  tmp_char := '@'
                else if (cptr > 1) then
                  if (m_list[cptr].ml) then
                    loc_symbol(i1,i2,tmp_char)
                  else
                    tmp_char := ' '
                else
                  tmp_char := ' ';
                if (tmp_char = ' ') then{ If blank... }
                  begin
                    if (flag) then      { If floor_str <> '' then }
                      begin
                        isp := isp + 1; { Increment blank ctr }
                        if (isp > 3) then       { Too many blanks, print}
                          begin                 { floor_str and reset }
                            print(floor_str,ypos,xpos);
                            flag := false;
                            isp := 0;
                          end;
                      end
                  end
                else
                  begin
                    if (flag) then      { Floor_str <> '' }
                      begin
                        if (isp > 0) then       { Add on the blanks }
                          begin
                            for i5 := 1 to isp do
                              floor_str := floor_str + ' ';
                            isp := 0;
                          end;                  { Add on the character  }
                        floor_str := floor_str + tmp_char;
                      end
                    else
                      begin             { Floor_str = '' }
                        xpos := i2;     { Save column for printing }
                        flag := true;   { Set flag to true }
                        floor_str := tmp_char;  { Floor_str <> '' }
                      end;
                  end;
              end;
          if (flag) then   { Print remainder, if any }
            print(floor_str,ypos,xpos);
        end;
    end;
 
 
{ Compact monsters }
[psect(misc2$code)] procedure compact_monsters;
    var
        i1,i2,i3,ctr,cur_dis : integer;
        delete_1,delete_any  : boolean;
    begin
      cur_dis := 66;
      delete_any := false;
      repeat
        i1 := muptr;
        i2 := 0;
        repeat
          delete_1 := false;
          i3 := m_list[i1].nptr;
          with m_list[i1] do
            if (cur_dis > cdis) then
              if (randint(3) = 1) then
                begin
                  if (i2 = 0) then
                    muptr := i3
                  else
                    m_list[i2].nptr := i3;
                  cave[fy,fx].cptr := 0;
                  m_list[i1] := blank_monster;
                  m_list[i1].nptr := mfptr;
                  mfptr := i1;
                  ctr := ctr + 1;
                  delete_1 := true;
                  delete_any := true;
                end;
          if (not(delete_1)) then i2 := i1;
          i1 := i3;
        until (i1 = 0);
        if (not(delete_any)) then cur_dis := cur_dis - 6;
      until (delete_any);
      if (cur_dis < 66) then prt_map;
    end;
 
 
{ Returns a pointer to next free space }
[psect(misc3$code)] procedure popm(var x : integer);
    begin
      if (mfptr < 1) then compact_monsters;
      x := mfptr;
      mfptr := m_list[x].nptr;
    end;
 
 
{ Pushs a record back onto free space list }
[psect(misc3$code)] procedure pushm(x : integer);
    begin
      m_list[x] := blank_monster;
      m_list[x].nptr := mfptr;
      mfptr := x;
    end;
 
 
{ Gives Max hit points }
[psect(misc3$code)] function max_hp(hp_str : dtype) : integer;
    var
        i1,num,die : integer;
    begin
      for i1 := 1 to length(hp_str) do
        if (hp_str[i1] = 'd') then
          hp_str[i1] := ' ';
      readv(hp_str,num,die);
      max_hp := num*die;
    end;
 
 
{ Put varying monster info from creature list into monster list.}
{ The procedure is called bungi because he was there......  -RLG}
[psect(misc3$code)] procedure bungi(y,x,z,cur_pos : integer);
    Begin
      with m_list[cur_pos] do       
        begin
          fy := y;
          fx := x;
          mptr := z;
          nptr := muptr;
          muptr := cur_pos;
          if (uand(c_list[z].cdefense,%X'4000') <> 0) then
            hp := max_hp(c_list[z].hd)
          else
            hp := damroll(c_list[z].hd);
          cspeed := c_list[z].speed + py.flags.speed;
          stuned := 0;
          cdis := distance(char_row,char_col,y,x);
          cave[y,x].cptr := cur_pos;
	end;	  
    End;

{ Places a monster at given location }
[psect(misc3$code)] procedure place_monster(y,x,z : integer; slp : boolean);
    var
        i1,cur_pos : integer;
    begin
      popm(cur_pos);
      bungi(y,x,z,cur_pos);
      with m_list[cur_pos] do
        begin
	  cur_reprod := c_list[z].reprod;
          if (slp) then
              csleep := trunc(c_list[z].sleep/5.0) + randint(c_list[z].sleep)
          else
            csleep := 0
        end
    end;

{ Places a policeman on town level. }
[psect(misc3$code)] procedure place_policeman(y1,x1,cop_num: integer);
    var
      y,x,z,cur_pos : integer;
    begin
      repeat
        y := y1 + randint(7)-4;
        x := x1 + randint(7)-4;
      until ((in_bounds(y,x)) and (cave[y,x].fval in [1,2,4]) and
             (cave[y,x].cptr = 0));
      z := m_level[0] + m_level[max_mons_level] + cop_num;
      popm(cur_pos);
      bungi(y,x,z,cur_pos);
      with m_list[cur_pos] do
        begin
	  cur_reprod := c_list[z].reprod;
          csleep := 0
        end
    end;
 
{ Places multiplying/reproducing monster at given location. -RLG}
[psect(misc3$code)] procedure place_mult_monster(y,x,z,repro : integer; 
						         slp : boolean);
    var
        i1,cur_pos : integer;
    Begin
      popm(cur_pos);
      bungi(y,x,z,cur_pos);
      with m_list[cur_pos] do
        begin
	  cur_reprod := repro;
          if (slp) then
              csleep := trunc(c_list[z].sleep/5.0) + randint(c_list[z].sleep)
          else
            csleep := 0
        end
    End;

[psect(misc3$code)] procedure get_random_location(var y,x : integer);
      Begin
        repeat
          y := randint(cur_height-2)+1;
          x := randint(cur_width-2)+1;
          until ((cave[y,x].fval in [1,2,4]) and
                (cave[y,x].cptr = 0)        and
                (cave[y,x].tptr = 0)        and
                (distance(y,x,char_row,char_col) > max_sight));
      End;

{ Places badguy at random location. -RLG}
[psect(misc3$code)] procedure place_badguy_monster(monster : integer);
      VAR
        cur_pos,y,x,z: integer;
      BEGIN
	  { Finds exact spot of badguy in creature list}
	  get_random_location(y,x);
          z := m_level[0] + m_level[max_mons_level] + num_of_police + monster;
          popm(cur_pos);
	  bungi(y,x,z,cur_pos);
          with m_list[cur_pos] do
            begin
	      cur_reprod := c_list[z].reprod;
              csleep := 10;
            end;
      END;
 
{ Places the Boss at random location }
[psect(misc3$code)] procedure place_the_boss;
      var
          cur_pos,y,x,z : integer;
      Begin
      	  get_random_location(y,x);
	  z := max_creatures;
	  popm(cur_pos);
	  bungi(y,x,z,cur_pos);
          with m_list[cur_pos] do
            begin
	      cur_reprod := c_list[z].reprod;
              csleep := 0;
            end;
      End;
 
 
{ Allocates a random monster }
[psect(misc3$code)] procedure alloc_monster(alloc_set : obj_set;
                          num,dis : integer;
                          slp : boolean );
    var
        y,x,a,b,i1,i2,i3 : integer;
    begin
      for i1 := 1 to num do
        begin
          repeat
            y := randint(cur_height-2)+1;
            x := randint(cur_width-2)+1;
          until ((cave[y,x].fval in alloc_set)  and
                 (cave[y,x].cptr = 0)           and
                 (cave[y,x].fopen)              and
                 (distance(y,x,char_row,char_col) > dis));
          if (dun_level <= 0) then
            i2 := randint(m_level[0])
          else
            if (dun_power > max_mons_level) then
              i2 := randint(m_level[max_mons_level]) + m_level[0]
            else
              if (randint(mon_nasty) = 1) then
                begin
                  i2 := dun_power + abs(randnor(0,4)) + 1;
                  if (i2 > max_mons_level) then i2 := max_mons_level;
                  i3 := m_level[i2] - m_level[i2-1];
                  i2 := randint(i3) + m_level[i2-1]
                end
              else
                i2 := randint(m_level[dun_power]) + m_level[0];
          place_monster(y,x,i2,slp);
        end
    end;
 
 
{ Places creature adjacent to given location }
[psect(misc3$code)] function summon_monster(
                                var y,x : integer;
                                slp     : boolean  ) : boolean;
    var
        i1,i2,i3,i4,i5 : integer;
    begin
      summon_monster := false;
      i1 := 0;
      i5 := dun_power - mon$summon_adj;
      if (i5 < 1) then
        i4 := 1
      else if (i5 > max_mons_level) then
        i4 := max_mons_level
      else
        i4 := i5;
      if (dun_level <= 0) then
        i4 := randint(m_level[0])
      else
        i4 := randint(m_level[i4]) + m_level[0];
      repeat
        i2 := y - 2 + randint(3);
        i3 := x - 2 + randint(3);
        if (in_bounds(i2,i3)) then
          with cave[i2,i3] do
            if (fval in [1,2,4,5]) then
              if (cptr = 0) then
                if (fopen) then
                  begin
                    place_monster(i2,i3,i4,slp);
                    summon_monster := true;
		    i1 := 9;
                    y := i2;
                    x := i3;
                  end;
        i1 := i1 + 1;
      until (i1 > 9);
    end;
 
 
{ Places undead adjacent to given location }
[psect(misc3$code)] function summon_undead(var y,x : integer) : boolean;
    var
        i1,i2,i3,i4,i5,ctr      : integer;
    begin
      i1 := 0;
      summon_undead := false;
      i4 := m_level[max_mons_level];
      repeat
        i5 := randint(i4) + m_level[0];
        ctr := 0;
        repeat
          if (uand(c_list[i5].cdefense,%X'0008') <> 0) then
            begin
              ctr := 20;
              i4  := 0;
            end
          else
            begin
              i5 := i5 + 1;
              if (i5 > i4) then
                ctr := 20
              else
                ctr := ctr + 1;
            end;
        until(ctr > 19)
      until(i4 = 0);
      repeat
        i2 := y - 2 + randint(3);
        i3 := x - 2 + randint(3);
        if (in_bounds(i2,i3)) then
          with cave[i2,i3] do
            if (fval in [1,2,4,5]) then
              if ((cptr = 0) and (fopen)) then
                begin
                  place_monster(i2,i3,i5,false);
                  summon_undead := true;
                  i1 := 9;
                  y := i2;
                  x := i3;
                end;
        i1 := i1 + 1;
      until (i1 > 9);
    end;
 
 
{ If too many objects on floor level, delete some of them   }
[psect(misc2$code)] procedure compact_objects;
    var
        i1,i2,ctr,cur_dis : integer;
        flag       : boolean;
    begin
      ctr := 0;
      cur_dis := 66;
      repeat
        for i1 := 1 to cur_height do
          for i2 := 1 to cur_width do
            with cave[i1,i2] do
              if (tptr > 0) then
                if (distance(i1,i2,char_row,char_col) > cur_dis) then
                  begin
                    flag := false;
                    with t_list[tptr] do
                      case tval of
                        102     : if (subval in [1,6,9]) then
                                    flag := true
                                  else if (randint(4) = 1) then
                                    flag := true;
                        103     : flag := true;
                        104,105 : if (randint(4) = 1) then flag := true;
                        107,108 : ;
                        otherwise if (randint(8) = 1) then flag := true;
                      end;
                    if (flag) then
                      begin
                        fopen := true;
                        t_list[tptr] := blank_treasure;
                        t_list[tptr].p1 := tcptr;
                        tcptr := tptr;
                        tptr := 0;
                        ctr := ctr + 1;
                      end;
                  end;
          if (ctr = 0) then cur_dis := cur_dis - 6;
        until (ctr > 0);
        if (cur_dis < 66) then prt_map;
    end;
 
 
{ Gives pointer to next free space  }
[psect(misc4$code)] procedure popt(var x : integer);
    var
        i1 : integer;
    begin
      if (tcptr < 1) then compact_objects;
      x := tcptr;
      tcptr := t_list[x].p1;
    end;
 
 
{ Pushs a record back onto free space list  }
[psect(misc4$code)] procedure pusht(x : integer);
    begin
      t_list[x] := blank_treasure;
      t_list[x].p1 := tcptr;
      tcptr := x;
    end;
 
 
{ Order the treasure list by level  }
[psect(setup$code)] procedure sort_objects;
    var
        i1,i2,i3,gap     : integer;
        tmp       : treasure_type;
    begin
      gap := max_objects div 2;
      while (gap > 0) do
        begin
          for i1 := gap+1 to max_objects do
            begin
              i2 := i1 - gap;
              while (i2 > 0) do
                begin
                  i3 := i2 + gap;
                  if (object_list[i2].level > object_list[i3].level) then
                    begin
                      tmp := object_list[i2];
                      object_list[i2] := object_list[i3];
                      object_list[i3] := tmp;
                    end
                  else
                    i2 := 0;
                  i2 := i2 - gap;
                end;
            end;
          gap := gap div 2;
        end;
    end;
 
 
{places a trap at a given location}
[psect(misc4$code)] procedure place_trap(y,x,typ,subval : integer);
    var
        cur_pos   : integer;
        cur_trap  : treasure_type;
    begin
      if (typ = 1) then
        cur_trap := trap_lista[subval]
      else
        if (typ = 2) then
          cur_trap := trap_listb[subval]
        else
          cur_trap := blk_mkt_trap; {typ=3, one per level - mwk}
      popt(cur_pos);
      cave[y,x].tptr  := cur_pos;
      t_list[cur_pos] := cur_trap;
    end;
 
 
{ Places rubble at location y,x }
[psect(misc4$code)] procedure place_rubble(y,x : integer);
    var
        cur_pos   : integer;
    begin
      popt(cur_pos);
      with cave[y,x] do
        begin
          tptr := cur_pos;
          fopen := false;
        end;
      t_list[cur_pos] := rubble;
    end;
 
 
[psect(misc4$code)] procedure place_open_door(y,x : integer);
    var
        cur_pos   : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
        tptr := cur_pos;
        t_list[cur_pos] := door_list[1];
        fval  := corr_floor3.ftval;
        fopen := true;
      end;
  end;
 
 
[psect(misc4$code)] procedure place_broken_door(y,x : integer);
    var
        cur_pos   : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
        tptr := cur_pos;
        t_list[cur_pos] := door_list[1];
        fval  := corr_floor3.ftval;
        fopen := true;
        t_list[cur_pos].p1 := 1;
      end;
  end;
 
 
[psect(misc4$code)] procedure place_closed_door(y,x : integer);
    var
        cur_pos   : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
        tptr := cur_pos;
        t_list[cur_pos] := door_list[2];
        fval  := corr_floor3.ftval;
        fopen := false;
      end;
  end;
 
 
[psect(misc4$code)] procedure place_locked_door(y,x : integer);
    var
        cur_pos   : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
        tptr := cur_pos;
        t_list[cur_pos] := door_list[2];
        fval  := corr_floor3.ftval;
        fopen := false;
        t_list[cur_pos].p1 := randint(10) + 10;
      end;
  end;
 
 
[psect(misc4$code)] procedure place_stuck_door(y,x : integer);
    var
        cur_pos   : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
        tptr := cur_pos;
        t_list[cur_pos] := door_list[2];
        fval  := corr_floor3.ftval;
        fopen := false;
        t_list[cur_pos].p1 := -randint(10) - 10;
      end;
  end;
 
 
[psect(misc4$code)] procedure place_secret_door(y,x : integer);
    var
        cur_pos   : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
        tptr := cur_pos;
        t_list[cur_pos] := door_list[3];
        fval  := corr_floor4.ftval;
        fopen := false;
      end;
  end;
 
 
[psect(misc4$code)] procedure place_door(y,x : integer);
    var
        cur_pos   : integer;
  begin
    case randint(3) of
      1 : case randint(4) of
            1    : place_broken_door(y,x);
            otherwise     place_open_door(y,x);
          end;
      2 : case randint(12) of
            1,2  : place_locked_door(y,x);
            3    : place_stuck_door(y,x);
            otherwise     place_closed_door(y,x);
          end;
      3 : place_secret_door(y,x);
    end;
  end;
 
 
 
{ Place an up staircase at given y,x    }
[psect(misc4$code)] procedure place_up_stairs(y,x : integer);
    var
        cur_pos,i3    : integer;
    begin
      with cave[y,x] do
        if (tptr <> 0) then
          begin
            pusht(tptr);
            tptr := 0;
            fopen := true;
          end;
      popt(cur_pos);
      cave[y,x].tptr := cur_pos;
      case randint(10) of
      1,2,3,4,5: i3 := 1;
	  6,7,8: i3 := 2;
	   9,10: i3 := 3;
      end;
      t_list[cur_pos] := up_stair[i3];
    end;
 
{ Place a down staircase at given y,x   }
[psect(misc4$code)] procedure place_down_stairs(y,x : integer);
    var
        cur_pos,i3   	 : integer;
    begin
      with cave[y,x] do
        if (tptr <> 0) then
          begin
            pusht(tptr);
            tptr := 0;
            fopen := true;
          end;
      popt(cur_pos);
      cave[y,x].tptr := cur_pos;
      case randint(10) of 
      1,2,3,4,5: i3 := 1;
	  6,7,8: i3 := 2;
           9,10: i3 := 3;
      end;
      t_list[cur_pos] := down_stair[i3];
    end;
 
{ Places a staircase 1=up, 2=down   }
[psect(misc4$code)] procedure place_stairs(typ,num,walls : integer);
    var
        i1,i2,y1,x1,y2,x2 : integer;
        flag       : boolean;
    begin
      for i1 := 1 to num do
          begin
            flag := false;
            repeat
              i2 := 0;
              repeat
                y1 := randint(cur_height - 12);
                x1 := randint(cur_width  - 12);
                y2 := y1 + 12;
                x2 := x1 + 12;
                repeat
                  repeat
                    with cave[y1,x1] do
                      if (fval in [1,2,4]) then
                        if (tptr = 0) then
                          if (next_to4(y1,x1,wall_set) >= walls) then
                            begin
                              flag := true;
                              case typ of
                                1 : place_up_stairs(y1,x1);
                                2 : place_down_stairs(y1,x1);
                              end;
                            end;
                    x1 := x1 + 1;
                  until ((x1 = x2) or (flag));
                  x1 := x2 - 12;
                  y1 := y1 + 1;
                until ((y1 = y2) or (flag));
                i2 := i2 + 1;
              until ((flag) or (i2 > 30));
              walls := walls - 1;
            until(flag);
          end;
    end;
 
 
{ Places a treasure (Gold or Gems) at given row, column }
[psect(misc4$code)] procedure place_gold(y,x : integer);
    var
        cur_pos,i1       : integer;
    begin
      popt(cur_pos);
      i1 := trunc((randint(dun_power+2)+2)/2.0);
      if (randint(obj_great) = 1) then
        i1 := i1 + randint(dun_power);
      if (i1 > max_gold) then
        i1 := randint(max_gold-8)+8;
      cave[y,x].tptr := cur_pos;
      t_list[cur_pos] := gold_list[i1];
      with t_list[cur_pos] do
        cost := randint(6*cost) + cost;
    end;
 
 
{ Returns the array number of a random object   }
[psect(misc4$code)] function get_obj_num(level : integer) : integer;
    var
        i1      : integer;
    begin
      if (level > max_obj_level)  then level := max_obj_level;
      if (randint(obj_great) = 1) then level := max_obj_level;
      if (level = 0) then
        i1 := randint(t_level[0])
      else
        i1 := randint(t_level[level]);
      get_obj_num := i1;
    end;
 

{ Pre-declaration. Code located in TREASURE.INC }
[psect(misc4$code)] procedure magic_treasure(var item  : treasure_type;
						 power :       integer); 
  forward;

{ Places an object at given row, column co-ordinate }
[psect(misc4$code)] procedure place_object(y,x : integer);
    var
        cur_pos  : integer;
    begin
      popt(cur_pos);
      cave[y,x].tptr := cur_pos;
      t_list[cur_pos] := object_list[get_obj_num(dun_power)];
      magic_treasure(t_list[cur_pos],dun_power);
    end;
 
 
{ Allocates an object for tunnels and rooms }
[psect(misc4$code)] procedure alloc_object(alloc_set : obj_set;
					   typ,num   : integer);
    var
        i1,i2,i3  : integer;
    begin
      for i3 := 1 to num do
        begin
          repeat
            i1 := randint(cur_height);
            i2 := randint(cur_width);
          until ((cave[i1,i2].fval in alloc_set) and
                 (cave[i1,i2].tptr = 0));
          case typ of
            1 : place_trap(i1,i2,1,randint(max_trapa));
            2 : place_trap(i1,i2,2,randint(max_trapb));
            3 : place_rubble(i1,i2);
            4 : place_gold(i1,i2);
            5 : place_object(i1,i2)
          end
        end
    end;
 
 
{ Creates objects nearby the coordinates given  }
[psect(misc4$code)] procedure random_object(y,x,num : integer);
    var
        i1,i2,i3  : integer;
    begin
      repeat
        i1 := 0;
        repeat
          i2 := y - 3 + randint(5);
          i3 := x - 4 + randint(7);
          with cave[i2,i3] do
            if (fval in floor_set) then
              if (tptr = 0) then
                begin
                  if (randint(100) < 75) then
                    place_object(i2,i3)
                  else
                    place_gold(i2,i3);
                    i1 := 9;
                end;
          i1 := i1 + 1;
        until (i1 > 10);
        num := num - 1;
      until (num = 0);
    end;
 
 
{ Converts stat num into string }
[psect(misc5$code)] procedure cnv_stat  (
                        stat        : byteint;
                        var out_val : stat_type
                                        );
    var
        tmp_str     : vtype;
        part1,part2 : integer;
    begin
      if (stat > 18) then
        begin
          part1 := 18;
          part2 := stat - 18;
          writev(tmp_str,part1:2,'/',part2:1);
        end
      else
        writev(tmp_str,stat:2);
      if (length(tmp_str) < 6) then tmp_str := pad(tmp_str,' ',6);
      out_val := tmp_str;
    end;
 
 
{ Print character info in given row, column }
[psect(misc5$code)] procedure prt_field(info : vtype; row,column : integer);

      begin
        put_buffer(pad(info,' ',14),row,column);
      end;
 
 
{ Print number with header at given row, column }
[psect(misc5$code)] procedure prt_num   ( header   	 : vtype;
					  num,row,column : integer);
      var
                out_val   : vtype;
      begin
        writev(out_val,header,num:1,' ');
        put_buffer(out_val,row,column);
      end;

 
{ Print character stat in given row, column }
[psect(misc5$code)] procedure prt_stat  ( stat_name   :   vtype;
					  stat        : byteint;
					  row,column  : integer);
      var
                stat_val  : stat_type;
                out_val   : vtype;
      begin
        prt_num(stat_name,stat,row,column);

       { cnv_stat(stat,stat_val);
         out_val := stat_name + stat_val;  ((the old system....))
         put_buffer(out_val,row,column);   }

      end;
 
 
{ prints game time in stat block. --jeb}
[psect(misc5$code)] procedure prt_time;
	var
          hours   : integer;
	  minutes : integer;
	  weekday : dtype;
	  out_val : ctype;
	  str	  : ctype;
  BEGIN
	  case (day mod 7) of
	      0: weekday := 'SUN';
	      1: weekday := 'MON';
	      2: weekday := 'TUE';
	      3: weekday := 'WED';
	      4: weekday := 'THU';
	      5: weekday := 'FRI';
	      6: weekday := 'SAT';
	  end;
	  writev(out_val,weekday,day:3,' ');
	  minutes := (turn mod 720) div 12;
	  hours := turn div 720;
	  if (hours < 10) then
	    out_val := out_val + '0';	    
	  writev(str,out_val,hours:1,':');
	  if (minutes < 10) then
	    str := str + '0';
	  writev(out_val,str,minutes:1);
	  put_buffer(out_val,23,1)
  END;


{Decrease py.flags after mass turn increment - RLG}
[psect(misc2$code)] procedure zero_pyflag;
     
     BEGIN		   
       with py.flags do
         Begin
	   if (blind>0) then blind := 1;
	   if (paralysis>0) then paralysis := 1;
	   if (confused>0) then confused := 1;
	   if (protection>0) then protection := 1;
	   if (fast>0) then fast := 1;
	   if (slow>0) then slow := 1;
	   if (afraid>0) then afraid := 1;
	   if (poisoned>0) then poisoned := 1;
	   if (image>0) then image := 1;
	   if (protevil>0) then protevil := 1;
	   if (invuln>0) then invuln := 1;
	   if (hero>0) then hero := 1;
	   if (shero>0) then shero := 1;
	   if (blessed>0) then blessed := 1;
	   if (resist_heat>0) then resist_heat := 1;
	   if (resist_cold>0) then resist_cold := 1;
	   if (detect_inv>0) then detect_inv := 1;
	   if (tim_infra>0) then tim_infra := 1;
 	   word_recall := 0;
	 End;
     END;

{ Adjustment for wisdom }
[psect(misc2$code)] function wis_adj : integer;
    begin
      if (py.stat.cwis > 49) then
        wis_adj := 10
      else if (py.stat.cwis > 44) then
        wis_adj := 9
      else if (py.stat.cwis > 39) then
        wis_adj := 8
      else if (py.stat.cwis > 34) then
        wis_adj := 7
      else if (py.stat.cwis > 29) then
        wis_adj := 6
      else if (py.stat.cwis > 24) then
        wis_adj := 5
      else if (py.stat.cwis > 19) then
        wis_adj := 4
      else if (py.stat.cwis > 17) then
        wis_adj := 3
      else if (py.stat.cwis > 14) then
        wis_adj := 2
      else if (py.stat.cwis > 7) then
        wis_adj := 1
      else
        wis_adj := 0;
    end;
 
 
{ adjustment for intellegence   }
[psect(misc2$code)] function int_adj : integer;
    begin
      if (py.stat.cint > 49) then
        int_adj := 10
      else if (py.stat.cint > 44) then
        int_adj := 9
      else if (py.stat.cint > 39) then
        int_adj := 8
      else if (py.stat.cint > 34) then
        int_adj := 7
      else if (py.stat.cint > 29) then
        int_adj := 6
      else if (py.stat.cint > 24) then
        int_adj := 5
      else if (py.stat.cint > 19) then
        int_adj := 4
      else if (py.stat.cint > 17) then
        int_adj := 3
      else if (py.stat.cint > 14) then
        int_adj := 2
      else if (py.stat.cint > 7) then
        int_adj := 1
      else
        int_adj := 0;
    end;
 
 
{ Adjustment for charisma }
[psect(misc2$code)] function chr_adj : integer;
    begin
      if (py.stat.cchr > 49) then
        chr_adj := 10
      else if (py.stat.cchr > 44) then
        chr_adj := 9
      else if (py.stat.cchr > 39) then
        chr_adj := 8
      else if (py.stat.cchr > 34) then
        chr_adj := 7
      else if (py.stat.cchr > 29) then
        chr_adj := 6
      else if (py.stat.cchr > 24) then
        chr_adj := 5
      else if (py.stat.cchr > 19) then
        chr_adj := 4
      else if (py.stat.cchr > 17) then
        chr_adj := 3
      else if (py.stat.cchr > 14) then
        chr_adj := 2
      else if (py.stat.cchr > 7 ) then
        chr_adj := 1
      else
        chr_adj := 0;
    end;
 
{ Charisma adjustment for buy/sell prices  -RLG }                      
[psect(misc2$code)] function chr_padj : real;  
    begin
       if (py.stat.cchr > 49) then
         chr_padj := -0.20
       else if (py.stat.cchr > 44) then
         chr_padj := -0.18
       else if (py.stat.cchr > 39) then
         chr_padj := -0.16
       else if (py.stat.cchr > 34) then
         chr_padj := -0.14
       else if (py.stat.cchr > 29) then
         chr_padj := -0.12
       else if (py.stat.cchr > 24) then
         chr_padj := -0.10
       else if (py.stat.cchr > 19) then
         chr_padj := -0.08
       else if (py.stat.cchr > 17) then
         chr_padj := -0.04
       else if (py.stat.cchr > 15) then
         chr_padj := -0.02
       else if (py.stat.cchr > 12) then
         chr_padj :=  0.05
       else if (py.stat.cchr >  9) then
         chr_padj := 0.10
       else if (py.stat.cchr >  6) then
         chr_padj := 0.20
       else if (py.stat.cchr >  3) then
         chr_padj := 0.25
       else
         chr_padj := 0.30
    end;
 
{ Returns a character's adjustment to hit points    }
[psect(misc2$code)] function con_adj : integer;
  begin
    with py.stat do
      if      (ccon <   4) then con_adj := -3
      else if (ccon <   6) then con_adj := -2
      else if (ccon <   9) then con_adj := -1
      else if (ccon <  13) then con_adj :=  0
      else if (ccon <  16) then con_adj :=  1
      else if (ccon <  18) then con_adj :=  2
      else if (ccon <  20) then con_adj :=  3
      else if (ccon <  25) then con_adj :=  4
      else if (ccon <  30) then con_adj :=  5
      else if (ccon <  35) then con_adj :=  6
      else if (ccon <  40) then con_adj :=  7
      else if (ccon <  45) then con_adj :=  8
      else if (ccon <  50) then con_adj :=  9
      else                      con_adj := 10
  end;
 
 
{ Calculates hit points for each level that is gained.  }
[psect(misc2$code)] function get_hitdie : integer;
    var
        i1      : integer;
    begin
      get_hitdie := randint(py.misc.hitdie) + con_adj;
    end;
 
 
{ Prints title of character }
[psect(misc5$code)] procedure prt_title;
    begin
      prt_field(py.misc.title,5,stat_column);
    end;
 
 
{ Prints strength   }
[psect(misc5$code)] procedure prt_strength;
    begin
      prt_stat('',py.stat.cstr,7,stat_column+6);
    end;
 
 
{ Prints intelligence   }
[psect(misc5$code)] procedure prt_intelligence;
    begin
      prt_stat('',py.stat.cint,8,stat_column+6);
    end;
 
 
{ Prints wisdom }
[psect(misc5$code)] procedure prt_wisdom;
    begin
      prt_stat('',py.stat.cwis,9,stat_column+6);
    end;
 
 
{ Prints dexterity  }
[psect(misc5$code)] procedure prt_dexterity;
    begin
      prt_stat('',py.stat.cdex,10,stat_column+6);
    end;
 
 
{ Prints constitution   }
[psect(misc5$code)] procedure prt_constitution;
    begin
      prt_stat('',py.stat.ccon,11,stat_column+6);
    end;
 
 
{ Prints charisma   }
[psect(misc5$code)] procedure prt_charisma;
    begin
      prt_stat('',py.stat.cchr,12,stat_column+6);
    end;
 
 
 
{ Prints level  }
[psect(misc5$code)] procedure prt_level;
    begin
      prt_num( '',py.misc.lev,14,stat_column+6);
    end;
 
 
{ Prints players current (a real number...) and max mana points }
[psect(misc1$code)] procedure prt_mana;
var mwkdiv : real;
    indx   : integer;
begin
   mwkdiv:=py.misc.cmana/10;
   if (mwkdiv < 1) then
     indx:=2
   else
     if (mwkdiv < 10) then
       indx:=3
     else
       indx:=4;
   prt_num( '',trunc(py.misc.cmana),18,stat_column+6);
   prt_num( '/',py.misc.mana,18,stat_column+5+indx);
end;
 
 
{ Prints Max hit points }
[psect(misc5$code)] procedure prt_mhp;
    begin
      prt_num( '',py.misc.mhp,17,stat_column+6);
    end;
 
 
{ Prints players current hit points (a real number...)  }
[psect(misc1$code)] procedure prt_chp;
    begin
      prt_num( '',trunc(py.misc.chp),16,stat_column+6);
    end;
 
 
{ prints current AC }
[psect(misc5$code)] procedure prt_pac;
    begin
      prt_num( '',py.misc.dis_ac,19,stat_column+6);
    end;
 
 
{ Prints current gold   }
[psect(misc5$code)] procedure prt_gold;
    begin
      prt_num( '',py.misc.au,20,stat_column+6);
    end;
 
{ Prints current weight   }
[psect(misc5$code)] procedure prt_cur_weight;
    begin
      prt_num( '',inven_weight,21,stat_column+6);
    end;
 
{ Prints weight capacity }
[psect(misc5$code)] procedure prt_max_weight;
    begin
      max_weight:=weight_limit;
      prt_num( '',max_weight,22,stat_column+6);
    end;
 
{ Prints depth in stat area }
[psect(misc5$code)] procedure prt_depth;
      var
                depths  : vtype;
                depth   : integer;
      begin
        if (dun_level = 0) then
	  depths := townlist[town_num].town_name
	else if (dun_level < 0) then
	  depths := 'Office Building'
        else
	  begin
            depth := dun_level*50;
            writev(depths,'Depth: ',depth:1,' (meters)');
	  end;
        prt(depths,24,60);
      end;
 
 
{ Prints status of hunger   }
[psect(misc5$code)] procedure prt_hunger;
    begin
      if (uand(%X'000002',py.flags.status) <> 0) then
        put_buffer('Weak    ',24,1)
      else if (uand(%X'000001',py.flags.status) <> 0) then
        put_buffer('Hungry  ',24,1)
      else
        put_buffer('        ',24,1);
    end;
 
 
{ Prints Blind status   }
[psect(misc5$code)] procedure prt_blind;
    begin
      if (uand(%X'000004',py.flags.status) <> 0) then
        put_buffer('Blind  ',24,9)
      else
        put_buffer('       ',24,9);
    end;
 
 
{ Prints Confusion status   }
[psect(misc5$code)] procedure prt_confused;
    begin
      if (uand(%X'000008',py.flags.status) <> 0) then
        put_buffer('Confused  ',24,16)
      else
        put_buffer('          ',24,16);
    end;
 
 
{ Prints Fear status    }
[psect(misc5$code)] procedure prt_afraid;
    begin
      if (uand(%X'000010',py.flags.status) <> 0) then
        put_buffer('Afraid  ',24,26)
      else
        put_buffer('        ',24,26);
    end;
 
 
{ Prints Poisoned status }
[psect(misc5$code)] procedure prt_poisoned;
    begin
      if (uand(%X'000020',py.flags.status) <> 0) then
        put_buffer('Poisoned  ',24,34)
      else
        put_buffer('          ',24,34);
    end;
 
 
{ Prints Searching status   }
[psect(misc5$code)] procedure prt_search;
    begin
      if (uand(%X'000100',py.flags.status) <> 0) then
        put_buffer('Searching  ',24,44)
      else
        put_buffer('           ',24,44);
    end;
 
 
{ Prints Resting status }
[psect(misc5$code)] procedure prt_rest;
    begin
      if (uand(%X'000200',py.flags.status) <> 0) then
        put_buffer('Resting    ',24,44)
      else
        put_buffer('           ',24,44);
    end;
 
 
{ Prints winner status on display   }
[psect(misc5$code)] procedure prt_winner;
    begin
      put_buffer('*Winner*',23,1);
    end;
 
 
{ Increases a stat by one randomized level  }
[psect(misc2$code)] function in_statp(stat : byteint) : byteint;
    begin
      if (stat < 35) then
        stat := stat + 1
      else if (stat < 50) then
        stat := stat + randint(2) - 1;
      in_statp := stat;
    end;
 
{ Decreases a stat by one randomized level  }
[psect(misc2$code)] function de_statp(stat : byteint) : byteint;
    begin
      if (stat > 34) then
        begin
          stat := stat - 1;
	  if (randint(5)=1) then    {Hose the sucker...!}
	    stat := stat -1;      
	end
      else if (stat > 3) then
        stat := stat - 1;
      de_statp := stat;
    end;
 
{ Increases a stat by one true level    }
[psect(misc2$code)] function in_statt(stat : byteint) : byteint;
    begin
      if (stat < 50) then
        stat := stat + 1;
      in_statt := stat;
    end;
 
 
{ Decreases a stat by true level    }
[psect(misc2$code)] function de_statt(stat : byteint) : byteint;
    begin
      if (stat > 3) then
        stat := stat - 1;
      de_statt := stat;
    end;
 
 
{ Returns a character's adjustment to hit.  }
[psect(misc2$code)] function tohit_adj : integer;
  var
        total     : integer;
  begin
    with py.stat do
      begin
        if      (cdex <   4) then total := -3
        else if (cdex <   6) then total := -2
        else if (cdex <   8) then total := -1
        else if (cdex <  16) then total :=  0
        else if (cdex <  17) then total :=  1
        else if (cdex <  18) then total :=  2
        else if (cdex <  20) then total :=  3
        else if (cdex <  30) then total :=  4
        else if (cdex <  40) then total :=  5
        else if (cdex <  50) then total :=  6
        else                      total :=  7;
        if      (cstr <   4) then total := total - 3
        else if (cstr <   5) then total := total - 2
        else if (cstr <   7) then total := total - 1
        else if (cstr <  18) then total := total + 0
        else if (cstr <  25) then total := total + 1
        else if (cstr <  30) then total := total + 2
        else if (cstr <  40) then total := total + 3
        else if (cstr <  50) then total := total + 4
        else                      total := total + 5;
    end;
    tohit_adj := total;
  end;
 
 
{ Returns a character's adjustment to armor class   }
[psect(misc2$code)] function toac_adj : integer;
  begin
    with py.stat do
      if      (cdex <   4) then toac_adj :=  -4
      else if (cdex <   6) then toac_adj :=  -3
      else if (cdex <   9) then toac_adj :=  -2
      else if (cdex <  13) then toac_adj :=  -1
      else if (cdex <  16) then toac_adj :=   0
      else if (cdex <  18) then toac_adj :=   1
      else if (cdex <  20) then toac_adj :=   2
      else if (cdex <  25) then toac_adj :=   3
      else if (cdex <  30) then toac_adj :=   4
      else if (cdex <  35) then toac_adj :=   5
      else if (cdex <  40) then toac_adj :=   6
      else if (cdex <  50) then toac_adj :=   7
      else                      toac_adj :=   8
  end;
 
 
{ Returns a character's adjustment to disarm    }
[psect(misc2$code)] function todis_adj : integer;
  begin
    with py.stat do
      if      (cdex <   4) then todis_adj :=  -8
      else if (cdex <   6) then todis_adj :=  -6
      else if (cdex <   9) then todis_adj :=  -4
      else if (cdex <  13) then todis_adj :=  -2
      else if (cdex <  16) then todis_adj :=   0
      else if (cdex <  18) then todis_adj :=   1
      else if (cdex <  20) then todis_adj :=   2
      else if (cdex <  25) then todis_adj :=   3
      else if (cdex <  30) then todis_adj :=   4
      else if (cdex <  35) then todis_adj :=   5
      else if (cdex <  40) then todis_adj :=   6
      else if (cdex <  45) then todis_adj :=   7
      else if (cdex <  50) then todis_adj :=   8
      else                      todis_adj :=   9
  end;
 
 
{ Returns a character's adjustment to damage    }
[psect(misc2$code)] function todam_adj : integer;
  begin
    with py.stat do
      if      (cstr <   4) then todam_adj := -2
      else if (cstr <   6) then todam_adj := -1
      else if (cstr <   9) then todam_adj :=  0
      else if (cstr <  13) then todam_adj :=  1
      else if (cstr <  16) then todam_adj :=  2
      else if (cstr <  18) then todam_adj :=  3
      else if (cstr <  20) then todam_adj :=  4
      else if (cstr <  25) then todam_adj :=  5
      else if (cstr <  30) then todam_adj :=  6
      else if (cstr <  35) then todam_adj :=  7
      else if (cstr <  40) then todam_adj :=  8
      else if (cstr <  50) then todam_adj :=  9
      else                      todam_adj := 10;
  end;
 
 
{ Pre-declaration of PRT_EXPERIENCE }
[psect(misc5$code)] procedure prt_experience;
        forward;
 
 
{ Prints character-screen info  }
[psect(misc5$code)] procedure prt_stat_block;
    VAR
     indx : integer;
     mwkdiv : real;
    begin
      prt_field(py.misc.race,                 3,stat_column);
      prt_field(py.misc.tclass,               4,stat_column);
      prt_field(py.misc.title,                5,stat_column);
      prt_stat('STR : ',py.stat.cstr,         7,stat_column);
      prt_stat('INT : ',py.stat.cint,         8,stat_column);
      prt_stat('WIS : ',py.stat.cwis,         9,stat_column);
      prt_stat('DEX : ',py.stat.cdex,        10,stat_column);
      prt_stat('CON : ',py.stat.ccon,        11,stat_column);
      prt_stat('CHR : ',py.stat.cchr,        12,stat_column);
      prt_num( 'LEV : ',py.misc.lev,         14,stat_column);
      prt_num( 'EXP : ',py.misc.exp,         15,stat_column);
      prt_num( 'CHP : ',trunc(py.misc.chp),  16,stat_column);
      prt_num( 'MHP : ',py.misc.mhp,         17,stat_column);
 if ((py.misc.tclass <> 'Ninja') and (py.misc.tclass <> 'Wrestler')) then
   BEGIN
      mwkdiv:=py.misc.cmana/10;
      if (mwkdiv < 1) then
        indx:=2
      else
        if (mwkdiv < 10) then
          indx:=3
        else
          indx:=4;
      prt_num( 'RSRC: ',trunc(py.misc.cmana),18,stat_column);
      prt_num( '/',py.misc.mana,18,stat_column+5+indx);
   END;
      prt_num( 'AC  : ',py.misc.dis_ac,      19,stat_column);
      prt_num( 'CASH: ',py.misc.au,          20,stat_column);
      prt_num( 'WGHT: ',inven_weight,        21,stat_column);
      max_weight := weight_limit;
      prt_num( 'MXWT: ',max_weight,          22,stat_column);
      prt_time;
      if (total_winner_num > 0) then prt_winner;
      if (uand(%X'000003',py.flags.status) <> 0) then
        prt_hunger;
      if (uand(%X'000004',py.flags.status) <> 0) then
        prt_blind;
      if (uand(%X'000008',py.flags.status) <> 0) then
        prt_confused;
      if (uand(%X'000010',py.flags.status) <> 0) then
        prt_afraid;
      if (uand(%X'000020',py.flags.status) <> 0) then
        prt_poisoned;
      if (uand(%X'000100',py.flags.status) <> 0) then
        prt_search;
      if (uand(%X'000200',py.flags.status) <> 0) then
        prt_rest;
    end;
 
 
{ Draws entire screen   }
[psect(misc5$code)] procedure draw_cave;
    begin
      clear(1,1);
      prt_stat_block;
      prt_map;
      prt_depth;
    end;
{ 
[external] procedure cdclra( minlin,mincol,maxlin,maxcol : integer );
     external;

[psect(misc5$code)] procedure clear_area(row1,col1,row2,col2 : integer );
  begin
	cdclra( row1, col1, row2, col2 );
  end;


[external] procedure cdbox( lin1, col1, lin2, col2 : integer );
     external;

[psect(misc5$code)] procedure draw_box( r1, c1, r2, c2 : integer );
  begin
	cdbox( r1, c1, r2, c2 );
  end;
}

{ Prints the following information on the screen.   }
[psect(create$code)] procedure put_character;
    begin
      clear(1,1);
      with py.misc do
        begin
          prt('Name  : ' + name,3,3);
          prt('Qual  : ' + race,4,3);
          prt('Sex   : ' + sex,5,3);
          prt('Class : ' + tclass,6,3)
        end
    end;
 
 
{ Prints the following information on the screen. }
[psect(create$code)] procedure put_stats;
    begin
      with py do
        begin
          prt('Cur',2,62);
          prt('Max',2,71);
          prt_stat('STR : ',stat.cstr,3,56);
          prt_stat('| ',stat.str,3,69);
          prt_stat('INT : ',stat.cint,4,56);
          prt_stat('| ',stat.int,4,69);
          prt_stat('WIS : ',stat.cwis,5,56);
          prt_stat('| ',stat.wis,5,69);
          prt_stat('DEX : ',stat.cdex,6,56);
          prt_stat('| ',stat.dex,6,69);
          prt_stat('CON : ',stat.ccon,7,56);
          prt_stat('| ',stat.con,7,69);
          prt_stat('CHR : ',stat.cchr,8,56);
          prt_stat('| ',stat.chr,8,69);
          prt_num('+ To Hit : ',misc.dis_th,10,4);
          prt_num('+ To Dam : ',misc.dis_td,11,4);
          prt_num('+ To AC  : ',misc.dis_tac,12,4);
          prt_num('Total AC : ',misc.dis_ac,13,4);
        end
    end;
 
 
{ Returns a rating of x depending on y }
[psect(create$code)] function likert(x,y,i1 : integer) : btype;
      begin
        case i1 of
	     1:	case trunc(x/y) of
	     	      -1,0,1 : likert := 'VERY BAD';
	                 2,3 : likert := 'Bad';
	                 4   : likert := 'Poor';
			 5,6 : likert := 'Average';
			 7   : likert := 'Good';
		         8   : likert := 'Very Good';
			 9   : likert := 'Superb';
		     otherwise likert := 'EXCELLENT';
	        end; 
	     2:	case trunc(x/y) of
		      -1,0,1  : likert := 'Repugnant';
		         2    : likert := 'Mediocre';
	                 3    : likert := 'Sub-Par';
			4,5   : likert := 'Average';
			 6    : likert := 'Notable';
			 7    : likert := 'Honorable';
                         8    : likert := 'Remarkable';
                         9    : likert := 'Exemplary';
                     otherwise  likert := 'PERFECTION';
	        end; 
	     3:	case trunc(x/y) of
	              -1,0,1  : likert := 'Bogus';
		         2    : likert := 'Awful';
	                 3    : likert := 'Cool Minus';
			4,5   : likert := 'Dude';
			 6    : likert := 'Cool';
                         7    : likert := 'Mass Cool';
                        8,9   : likert := 'Awesome';
                     otherwise  likert := 'EXCELLENT';
	        end; 
	     4:	case trunc(x/y) of
	              -1,0,1  : likert := 'very bad';
		         2    : likert := 'bad';
	                 3    : likert := 'not bad';
		       4,5,6  : likert := 'okay';
                       7,8,9  : likert := 'good';
                     otherwise  likert := 'very good';
	        end; 
	     5:	case trunc(x/y) of
	              -1,0,1 : likert := '  :6    ';
		         2   : likert := '  :0    ';
	                 3   : likert := '  :(    ';
			4,5  : likert := '  :}    ';
			 6   : likert := '  :)    ';
                         7   : likert := '  :]    ';
                        8,9  : likert := '  :>    ';
                     otherwise likert := '  :D    ';
	        end; 
          end;
      end;
 
 
{ Prints age, height, weight, and SC }
[psect(create$code)] procedure put_misc1;
    begin
      with py do
        begin
          prt_num('Age          : ',misc.age,4,27);
          prt_num('Height/Weight: ',misc.ht ,5,27);
          prt_num('/',misc.wt ,5,44);
          prt_num('Social Class : ',misc.sc ,6,27);
        end;
    end;
 
 
{ Prints the following information on the screen. }
[psect(create$code)] procedure put_misc2;
  
    var
        out_val     :     ctype;

    begin
      with py.misc do
        begin
          prt_num('Level          : ',lev,10,27);
          prt_num('Cash           : ',au,11,27);
          prt_num('Savings Account: ',act_bal,12,27);
          prt_num('Bank Loan      : ',principal,13,27);
	  
          prt_num('Experience : ',exp,10,54);
          prt_num('Needed Exp : ',trunc(player_exp[lev]*expfact),11,54);
	  writev(out_val,'Hit Points : ',trunc(chp):1,'/',mhp:1);
          prt(out_val,12,54);
	  writev(out_val,'Resources  : ',trunc(cmana):1,'/',mana:1);
          prt(out_val,13,54);
        end
    end;
 
 
{ It's about time someone added this code!  The skill system is a complete
  mess and it's needed to be fixed since I got a hold of the code.  -RLG}

[psect(misc2$code)] function xbth : integer;
   begin
     with py.misc do
       xbth := lesser(200, bth + lev*bth_lev_adj + ptohit*bth_plus_adj);
   end;

[psect(misc2$code)] function xbthb : integer;
   begin
     with py.misc do
       xbthb := lesser(200, bthb + lev*bth_lev_adj + ptohit*bth_plus_adj);
   end;

[psect(misc2$code)] function xperc : integer;
   begin
     with py.misc do
       xperc  := lesser(200, perc + (lev div 2) + int_adj);
   end;

[psect(misc2$code)] function xsrh : integer;
   begin
     with py.misc do
       xsrh  := lesser(100, srh + (lev div 2) + int_adj);
   end;

[psect(misc2$code)] function xstl : integer;
   begin
     with py.misc do
       xstl := lesser(10, stl + (py.misc.lev div 8));
   end;

[psect(misc2$code)] function xdis : integer;
   begin
     with py.misc do
       xdis := lesser(100, disarm + (lev div 2) + 2*todis_adj);
   end;

[psect(misc2$code)] function xsave : integer;
   begin
     with py.misc do
       xsave := lesser(100, save + (lev div 2) + wis_adj);
   end;

[psect(misc2$code)] function xdev : integer;
   begin
     with py.misc do
       xdev := lesser(100, wierd + (lev div 2) + int_adj);
   end;

[psect(misc2$code)] function xinfra : vtype;
   var
      out_val : vtype;
   begin
     with py.flags do
       if (see_infra = 0) then
         xinfra := 'None'
       else if (see_infra = 1) then
	 xinfra := '1 Meter'
       else
         begin
           writev(out_val,py.flags.see_infra:1,' Meters');
	   xinfra := out_val;
	 end;
   end;


{ Prints ratings on certain abilities }
[psect(create$code)] procedure put_misc3;
      var 
          i1 : integer;
      begin
	clear(16,1);
	i1 := randint(5);   {random variation for likert tables}
        prt('(Miscellaneous Abilities)',16,24);
        put_buffer('Melee Fights: '+likert(xbth,20,i1) ,17, 2);
        put_buffer('Guns/Grenade: '+likert(xbthb,20,i1) ,18, 2);
        put_buffer('Saving Throw: '+likert(xsave,10,i1) ,19, 2);
        put_buffer('Stealth     : '+likert(xstl, 1,i1) ,17,27);
        put_buffer('Disarming   : '+likert(xdis,10,i1) ,18,27);
        put_buffer('Weird Device: '+likert(xdev,10,i1) ,19,27);
        put_buffer('Perception  : '+likert(xperc,10,i1) ,17,52);
        put_buffer('Searching   : '+likert(xsrh,10,i1) ,18,52);
        put_buffer('Infra-Vision: '+xinfra,              19,52);
      end;
 
 { Used to display the character on the screen. }
[psect(create$code)] procedure display_char;
      var
        dummy   : char;
      begin
        put_character;
        put_misc1;
        put_stats;
        put_misc2;
        put_misc3;
      end;
 
 { Gets a name for the character }
[psect(create$code)] procedure get_name;
    begin
      prt('Enter your player''s name  [press <RETURN> when finished]',22,3);
      get_string(py.misc.name,3,11,24);
      clear(21,1);
    end;
 
{ Changes the name of the character }
[psect(create$code)] procedure change_name;
    var
        c       : char;
        flag    : boolean;
    begin
      flag := false;
      display_char;
      repeat
        prt('<c>hange character name.     <ESCAPE> to continue.',22,3);
        inkey(c);
        case ord(c) of
          99     : get_name;
          0,3,25,26,27  : flag := true;
          otherwise;
        end;
      until (flag);
    end;
 
 
[psect(misc4$code)] procedure display_kill_list;

    VAR
	i1,i2,total	: integer;
	tmp		: array [1..52] of integer;
	c_chr		: dtype;
	out_val		: vtype;
    BEGIN
      clear(1,1);
      total := 0;
      for i1 := 1 to 52 do
        tmp[i1] := 0;
      for i1 := 1 to max_creatures do
        begin
	  i2 := index(alpha_set,c_list[i1].cchar);
	  tmp[i2] := tmp[i2] + kill_list[i1];
	  total := total + kill_list[i1];
	end;
      prt(
   'Total creatures killed for each on-screen character representation:',1,5);
      for i2 := 1 to 13 do
        prt_num(chr(i2+96)+' :  ',tmp[i2],i2+2,13); 
      for i2 := 14 to 26 do
        prt_num(chr(i2+96)+' :  ',tmp[i2],i2+2-13,28); 
      for i2 := 1 to 13 do
        prt_num(chr(i2+64)+' :  ',tmp[i2+26],i2+2,43); 
      for i2 := 14 to 26 do
        prt_num(chr(i2+64)+' :  ',tmp[i2+26],i2+2-13,58); 
      prt_num('Total Creatures Killed : ',total,17,24);
      if (total <= 21) then
         out_val := '(you must be a pacifist.)'
      else if (total <= 100) then
         out_val := '(a good beginning.)'
      else if (total <= 400) then
         out_val := '(you sure fight a lot.)'
      else if (total <= 800) then
         out_val := '(KILL!  KILL!  KILL!)'
      else if (total <= 1600) then
         out_val := '(the rest are waiting for you.)'
      else out_val := '(the Boss is gonna get you.)';
      prt(out_val,18,24);
      pause(24);
      draw_cave;
    END;


{ Destroy an item in the inventory }
[psect(misc4$code)] procedure inven_destroy(item_val : integer);
      var
           i2      : integer;
      begin
        temporary_slot := inventory[item_val];
        with inventory[item_val] do
          begin
            if ((number > 1) and (subval < 512))  then
              begin
                number := number - 1;
                inven_weight := inven_weight - weight;
                temporary_slot.number := 1;
              end
            else
              begin
                inven_weight := inven_weight - weight*number;
                for i2 := item_val to inven_ctr-1 do
                  inventory[i2] := inventory[i2+1];
                inventory[inven_ctr] := blank_treasure;
                inven_ctr := inven_ctr - 1;
              end;
          end
      end;
 
 
{ Drops an item from inventory to given location }
[psect(misc4$code)] procedure inven_drop(item_val,y,x : integer);
      var
        i1  : integer;
      begin
        with cave[y,x] do
          begin
            if (tptr > 0) then pusht(tptr);
            inven_destroy(item_val);
            popt(i1);
            t_list[i1] := temporary_slot;
            tptr := i1;
          end;
      end;
 
 
{ Destroys a type of item on a given percent chance }
[psect(misc4$code)] function inven_damage(
                        typ      :       obj_set;
                        perc     :       integer
                                        ) : integer;
      var
                i1,i2    : integer;
      begin
        i2 := 0;
        for i1 := 1 to inven_ctr do
          with inventory[i1] do
            if (tval in typ) then
              if (randint(100) < perc) then
                begin
                  inven_destroy(i1);
                  i2 := i2 + 1;
                end;
        inven_damage := i2;
      end;
 
 
{ Check inventory for too much weight   }
[psect(misc4$code)] function inven_check_weight : boolean;
      var
        item_wgt : integer;
      begin
        inven_check_weight := false;
        max_weight := weight_limit;
        with temporary_slot do
          item_wgt := number*weight;
{ Now, check to see if player can carry object  }
        if ((inven_weight + item_wgt) <= max_weight) then
          inven_check_weight := true;   { Can carry weight  }
      end;
 
 
{ Check to see if he will be carrying too many objects  }
[psect(misc4$code)] function inven_check_num : boolean;
      var
        item_num,i1       : integer;
        flag       : boolean;
      begin
        inven_check_num := false;
        if (inven_ctr < inven_max) then
          inven_check_num := true
        else if (temporary_slot.subval > 255) then
          for i1 := 1 to inven_ctr do
            with inventory[i1] do
              if (tval = temporary_slot.tval) then
                if (subval = temporary_slot.subval) then
                  inven_check_num := true;
      end;
 
 
{ Add the item in TEMPORARY_SLOT to players inventory.  Return the
  item position for a description if needed...  }
[psect(misc4$code)] procedure inven_carry(var item_val : integer);
      var
        item_num,wgt,typ,subt : integer;
        flag : boolean;
 
{ Insert TEMPORARY_SLOT at given location }
      procedure insert(pos,wgt : integer);
        var
           i1 : integer;
        begin
          for i1 :=  inven_ctr downto pos do
            inventory[i1+1] := inventory[i1];
          inventory[pos] := temporary_slot;
          inven_ctr      := inven_ctr   + 1;
          inven_weight   := inven_weight + wgt;
        end;
 
{ INVEN_CARRY routine }
      begin
{ Now, check to see if player can carry object  }
        item_val := 0;
        flag := false;
        with temporary_slot do
          begin
            item_num := number;
            typ  := tval;
            subt := subval;
            wgt  := number*weight;
          end;
        repeat
          item_val := item_val + 1;
          with inventory[item_val] do
            if (typ = tval) then
              begin
                if (subt = subval) then { Adds to other item }
                  if (subt > 255) then
                    begin
                      number := number + item_num;
                      inven_weight := inven_weight + wgt;
                      flag := true;
                    end;
              end
            else if (typ > tval) then
              begin     { Insert into list  }
                insert(item_val,wgt);
                flag := true;
              end;
        until ((item_val >= inven_ctr) or (flag));
        if (not(flag)) then     { Becomes last item in list }
          begin
            insert(inven_ctr+1,wgt);
            item_val := inven_ctr;
          end;
      end;
 
 
{ Finds range of item in inventory list }
[psect(misc4$code)] function find_range(item_val : obj_set; 
					var i2,i3 : integer) : boolean;
      var
                i1  : integer;
                flag: boolean;
      BEGIN
        i1 := 0;
        i2 := 0;
        i3 := 0;
        flag := false;
        while(i1 < inven_ctr) do
          BEGIN
            i1 := i1 + 1;
            if ((inventory[i1].tval in item_val) and (not (flag))) then
              BEGIN
                flag := true;
                i2 := i1;
              END;
            if ((not(inventory[i1].tval in item_val)) and (flag) and
                                                                (i3 = 0)) then
              i3 := i1 - 1;
          END;
        if ((flag) and (i3 = 0)) then
          i3 := inven_ctr;
        find_range := flag;
      END;

{ Returns spell chance of failure for spell }
[psect(misc2$code)] procedure skill_chance(var spell : spl_rec);
      var
        adjust : integer;
      begin
        case py.misc.pclass of
	 1,2,7: adjust := int_adj;
	   3,4: adjust := wis_adj;
	   5,6: adjust := chr_adj;
	end;
        with skill_list[spell.splnum] do
          with spell do
            begin
              splchn := sfail - 3*(py.misc.lev-slevel) - 3*(adjust-1);
              if (smana > py.misc.cmana) then
                splchn := splchn + 5*trunc(smana-py.misc.cmana);
              if (splchn > 95) then
                splchn := 95
              else if (splchn < 5) then
                splchn := 5;
            end ;
      end;
 
 
{ Print list of spells  }
[psect(misc2$code)] procedure print_new_skills(
                        spell       :  spl_type;
                        num         :  integer;
                        var redraw  :  boolean);
    var
        i1  : integer;
        out_val    : vtype;
    begin
      redraw := true;
      clear(1,1);
      prt('   Name                          Level  Rsrc  %Failure',2,1);
      for i1 := 1 to num do
        with skill_list[spell[i1].splnum] do
          begin
            skill_chance(spell[i1]);
            writev(out_val,chr(96+i1),') ',pad(sname,' ',30),
                slevel:3,'    ',smana:3,'      ',spell[i1].splchn:2);
            prt(out_val,2+i1,1);
          end
    end;
 
 
{ Returns spell pointer }
[psect(misc2$code)] function get_skill( spell   : spl_type;
					num     :  integer;
				    var sn,sc   :  integer; 
					prompt  :    vtype;
				    var redraw  :  boolean) : boolean;
    var
        i1  	   : integer;
        flag       : boolean;
        choice     : char;
        out_val    : vtype;
    begin
      sn := 0;
      flag := true;
      writev(out_val,'(Skills a-',chr(num+96),
                                ', space=list, <ESCAPE>=exit) ',prompt);
      while (((sn < 1) or (sn > num)) and (flag)) do
        begin
          prt(out_val,1,1);
          inkey(choice);
          sn := ord(choice);
          case sn of
            0,3,25,26,27: begin
                            flag := false;
                            reset_flag := true;
                          end;
            32 : print_new_skills(spell,num,redraw);
            otherwise   sn := sn - 96;
          end;
        end;
      msg_flag := false;
      if (flag) then
        begin
          skill_chance(spell[sn]);
          sc := spell[sn].splchn;
          sn := spell[sn].splnum;
        end;
      get_skill := flag;
    end;
 
 
{ Based on adjustment, determines how many new skills are learned. -RLG}
[psect(misc2$code)] function how_many_skills(adjustment: integer) : integer;
  var
    num : integer;
  begin
    case adjustment of
        0 : num := 0;
    1,2,3 : num := 1;
        4 : num := randint(2);
        5 : num := 2;
        6 : num := randint(3);
        7 : num := randint(2)+1;
	8 : num := randint(3)+1;
	9 : num := randint(2)+2;
       10 : num := randint(5);
    end;
    how_many_skills := num;
  end;	

{  Learn some magic spells (Scientist)    }
[psect(misc2$code)] function learn_spell(var redraw : boolean) : boolean;
      var
        i2  		: unsigned;
        i1,i3,sn,sc     : integer;
        new_spells	: integer;
        spell_flag	: unsigned;
        spell		: spl_type;
      begin
        learn_spell := false;
	new_spells := how_many_skills(int_adj);
        i1 := 0;
        spell_flag := 0;
        repeat
          i1 := i1 + 1;
          if (inventory[i1].tval = 90) then
            spell_flag := uor(spell_flag,inventory[i1].flags);
        until (i1 >= inven_ctr);
        while ((new_spells > 0) and (spell_flag > 0)) do
          begin
            i1 := 0;
            i2 := spell_flag;
            repeat
              i3 := bit_pos(i2);
              with skill_list[i3] do
                if (slevel <= py.misc.lev) then
                  if (not(learned)) then
                    begin
                      i1 := i1 + 1;
                      spell[i1].splnum := i3;
                    end;
            until(i2 = 0);
            if (i1 > 0) then
              begin
                print_new_skills(spell,i1,redraw);
        if (get_skill(spell,i1,sn,sc,'Learn which skill?',redraw)) then
                  begin
                    skill_list[sn].learned := true;
                    learn_spell := true;
                    if (py.misc.mana = 0) then
                      begin
                        py.misc.mana  := 1;
                        py.misc.cmana := 1;
                      end;
                  end
                else
                  new_spells := 0;
              end
            else
              new_spells := 0;
            new_spells := new_spells - 1;
          end;
      end;
 
{ Learn some extra spells (Con-Artist)    }
[psect(misc2$code)] function learn_extra(var redraw : boolean) : boolean;
      var
        i2  : unsigned;
        i1,i3,sn,sc       : integer;
        new_spells : integer;
        spell_flag : unsigned;        spell      : spl_type;
      begin
        learn_extra := false;
	new_spells := how_many_skills(chr_adj);
        i1 := 0;
        spell_flag := 0;
        repeat
          i1 := i1 + 1;
          if (inventory[i1].tval = 92) then
            spell_flag := uor(spell_flag,inventory[i1].flags);
        until (i1 >= inven_ctr);
        while ((new_spells > 0) and (spell_flag > 0)) do
          begin
            i1 := 0;
            i2 := spell_flag;
            repeat
              i3 := bit_pos(i2);
              with skill_list[i3] do
                if (slevel <= py.misc.lev) then
                  if (not(learned)) then
                    begin
                      i1 := i1 + 1;
                      spell[i1].splnum := i3;
                    end;
            until(i2 = 0);
            if (i1 > 0) then
              begin
                print_new_skills(spell,i1,redraw);
         if (get_skill(spell,i1,sn,sc,'Learn which trick?',redraw)) then
                  begin
                    skill_list[sn].learned := true;
                    learn_extra := true;
                    if (py.misc.mana = 0) then
                      begin
                        py.misc.mana   := 1;
                        py.misc.cmana := 1;
                      end;
                  end
                else
                  new_spells := 0;
              end
            else
              new_spells := 0;
            new_spells := new_spells - 1;
          end;
      end;
 
 
{ Learn some prayers (Guru)   }
[psect(misc2$code)] function learn_prayer : boolean;
      var
        i1,i2,i3,i4,new_spell    : integer;
        test_array : array [1..32] of integer;
        spell_flag : unsigned;
      begin
        i1 := 0;
        spell_flag := 0;
        repeat
          i1 := i1 + 1;
          if (inventory[i1].tval = 91) then
            spell_flag := uor(spell_flag,inventory[i1].flags);
        until(i1 >= inven_ctr);
        i1 := 0;
        while (spell_flag > 0) do
          begin
            i2 := bit_pos(spell_flag);
            with skill_list[i2] do
              if (slevel <= py.misc.lev) then
                if (not(learned)) then
                  begin
                    i1 := i1 + 1;
                    test_array[i1] := i2;
                  end;
          end;
	i2 := how_many_skills(wis_adj);
        new_spell := 0;
        while ((i1 > 0) and (i2 > 0)) do
          begin
            i3 := randint(i1);
            skill_list[test_array[i3]].learned := true;
            new_spell := new_spell + 1;
            for i4 := i3 to i1-1 do
              test_array[i4] := test_array[i4+1];
            i1 := i1 - 1;       { One less spell to learn   }
            i2 := i2 - 1;       { Learned one   }
          end;
          if (new_spell > 0) then
            begin
              if (new_spell > 1) then
                msg_print('You learned new prayers!')
              else
                msg_print('You learned a new prayer!');
              if (py.misc.exp = 0) then msg_print(' ');
              if (py.misc.mana = 0) then
                begin
                  py.misc.mana  := 1;
                  py.misc.cmana := 1;
                end;
              learn_prayer := true;
            end
          else
            learn_prayer := false;
      end;
 
 
{ Gain some resources if you know at least one skill    }
[psect(misc2$code)] procedure gain_mana(amount : integer);
      var
        i1,new_mana       : integer;
        knows_spell       : boolean;
      begin
        knows_spell := false;
        for i1 := 1 to max_skills do
          if (skill_list[i1].learned) then knows_spell := true;
        if (knows_spell) then
          begin
            if (odd(py.misc.lev)) then
              case amount of
                0 : new_mana := 0;
                1 : new_mana := 1;
                2 : new_mana := 1;
                3 : new_mana := 1;
                4 : new_mana := 2;
                5 : new_mana := 2;
                6 : new_mana := 3;
                7 : new_mana := 4;
		8 : new_mana := 5;
		9 : new_mana := 5;
	       10 : new_mana := 6;
                otherwise new_mana := 0;
              end
            else
              case amount of
                0 : new_mana := 0;
                1 : new_mana := 1;
                2 : new_mana := 1;
                3 : new_mana := 2;
                4 : new_mana := 2;
                5 : new_mana := 3;
                6 : new_mana := 3;
                7 : new_mana := 4;
		8 : new_mana := 5;
		9 : new_mana := 6;
	       10 : new_mana := 6;
                otherwise new_mana := 0;
              end;
            py.misc.mana  := py.misc.mana  + new_mana;
            py.misc.cmana := py.misc.cmana + new_mana;
          end;
      end;
 
 
{ Increases hit points and level    }
[psect(misc2$code)] procedure gain_level;
    var
        nhp,dif_exp,need_exp     : integer;
        redraw     : boolean;
        out_val    : vtype;
    BEGIN
      with py.misc do
          Begin
            lev := lev + 1;
	    if (lev >40) then 
	      begin
	        mhp := trunc(mhp*1.25);
		mana := trunc(mana*1.25);
	      end;
            nhp := get_hitdie;
            mhp := mhp + nhp;
            chp := chp + nhp;
            need_exp := trunc(player_exp[lev]*expfact);
            if (py.misc.exp > need_exp) then
              begin
                dif_exp := py.misc.exp - need_exp;
                py.misc.exp := need_exp + (dif_exp div 2);
              end;
            title := player_title[pclass,lev];
            writev(out_val,'Welcome to level ',lev:1,'.');
            msg_print(out_val);
            msg_print(' ');
            msg_flag := false;
            prt_level;
            prt_title;
            prt_mhp;
            prt_chp;
            if (pskill in [1,2,7]) then
               begin
                 redraw := false;
                 learn_spell(redraw);
                 if (redraw) then draw_cave;
                 gain_mana(int_adj);
		 prt_mana;
               end;
            if (pskill in [3,4]) then
               begin
                 learn_prayer;
                 gain_mana(wis_adj);
		 prt_mana;
               end;
            if (pskill in [5,6]) then
               begin
                 redraw := false;
                 learn_extra(redraw);
                 if (redraw) then draw_cave;
                 gain_mana(chr_adj);
		 prt_mana;
               end;
          End;
    END;
 
 
{ Prints experience }
  procedure prt_experience;
    var
        tmp_exp  : integer;
    begin
      with py.misc do
        begin
          if (exp > player_max_exp) then exp := player_max_exp;
          if (lev < max_player_level) then
            begin
              while (trunc(player_exp[lev]*expfact) <= exp) do gain_level;
              if (exp > max_exp) then max_exp := exp;
            end;
        end;
      prt_num('',py.misc.exp,15,stat_column+6);
    end;
 
 
{ Inserts a string into a string    }
  procedure insert_str                  (
                        %ref object_str : varying[a] of char;
                        %ref mtc_str    : varying[b] of char;
                        %ref insert_str : varying[c] of char
                                        );
        external;
 
 
{ Inserts a number into a string    }
[psect(misc1$code)] procedure insert_num(
                        var object_str  : varying[a] of char;
                        mtc_str  : varying[b] of char;
                        number   : integer;
                        show_sign       : boolean
                                        );
    var
        pos,olen,mlen   : integer;
        str1,str2       : vtype;
    begin
      pos := index(object_str,mtc_str);
      if (pos > 0) then
        begin
          olen := length(object_str);
          mlen := length(mtc_str);
          object_str := object_str + ' ';
          str1 := substr(object_str,1,pos-1);
          str2 := substr(object_str,pos+mlen,olen-(pos+mlen-1));
          if ((number >= 0) and (show_sign)) then
            writev(object_str,str1,'+',number:1,str2)
          else
            writev(object_str,str1,number:1,str2);
        end
    end;
 
{ Weapon weight VS strength and dexterity   }
[psect(moria$code)] function attack_blows(
                        weight     : integer;
                        var wtohit : integer
                                 ) : integer;
      var
        adj_weight,blows  : integer;
      begin
        blows  := 1;
        wtohit := 0;
        with py.stat do
          begin
            if ((cstr*15) < weight) then
              wtohit := -weight
            else
              begin
                if      (cdex < 10) then blows := 1
                else if (cdex < 20) then blows := 2
                else if (cdex < 25) then blows := 3
                else if (cdex < 30) then blows := 4
                else if (cdex < 40) then blows := 5
                else if (cdex < 50) then blows := 6
                else                      blows := 7;
                adj_weight := trunc((cstr*10)/weight);
                if      (adj_weight < 2) then blows := 1
                else if (adj_weight < 3) then blows := trunc(blows/3.0)
                else if (adj_weight < 4) then blows := trunc(blows/2.5)
                else if (adj_weight < 5) then blows := trunc(blows/2.25)
                else if (adj_weight < 7) then blows := trunc(blows/2.00)
                else if (adj_weight < 9) then blows := trunc(blows/1.75)
                else if (adj_weight <11) then blows := trunc(blows/1.50)
                else if (adj_weight <14) then blows := blows
                else                          blows := blows + 1;
              end;
          end;
        attack_blows := blows;
      end;
 
 
{ Critical hits, Nasty way to die...    }
[psect(moria$code)] function critical_blow(weight,plus,dam : integer) : integer;
      begin
        critical_blow := dam;
{ Weight of weapon, pluses to hit, and character level all
  contribute to the chance of a critical    }
        if (py.misc.tclass = 'Ninja') then weight := 8*weight;
        if (randint(5000) <= (weight+5*plus+3*py.misc.lev)) then
          begin
            if (py.misc.tclass = 'Ninja') then weight := weight div 7;
            weight := weight + randint(650);
            if      (weight < 400) then
              begin
                critical_blow := 2*dam + 5;
                msg_print('It was a good hit! (x2 damage)');
              end
            else if (weight < 700) then
 
              begin
                critical_blow := 3*dam + 10;
                msg_print('It was an excellent hit! (x3 damage)');
              end
            else if (weight < 900) then
              begin
                critical_blow := 4*dam + 15;
                msg_print('It was a superb hit! (x4 damage)');
              end
            else
              begin
                critical_blow := 5*dam + 20;
                msg_print('It was a *GREAT* hit! (x5 damage)');
              end
          end;
      end;
 
 
{ Given direction 'dir', returns new row, column location   }
[psect(misc1$code)] function move(dir : integer; var y,x : integer) : boolean;
      var
                new_row,new_col  : integer;
      begin
        case dir of
          1 : begin
                new_row := y + 1;
                new_col := x - 1;
              end;
          2 : begin
                new_row := y + 1;
                new_col := x;
              end;
          3 : begin
                new_row := y + 1;
                new_col := x + 1;
              end;
          4 : begin
                new_row := y;
                new_col := x - 1;
              end;
          5 : begin
                new_row := y;
                new_col := x;
              end;
          6 : begin
                new_row := y;
                new_col := x + 1;
              end;
          7 : begin
                new_row := y - 1;
                new_col := x - 1;
              end;
          8 : begin
                new_row := y - 1;
                new_col := x;
              end;
          9 : begin
                new_row := y - 1;
                new_col := x + 1;
              end;
        end;
        move := false;
        if ((new_row >= 1) and (new_row <= cur_height)) then
          if ((new_col >= 1) and (new_col <= cur_width)) then
            begin
              y := new_row;
              x := new_col;
              move := true;
            end
      end;
 
 
{ Saving throws for player character... }
[psect(moria$code)] function player_saves(adjust : integer) : boolean;
        begin
          if (randint(120) <= (py.misc.save + adjust)) then
            player_saves := true
          else
            player_saves := false;
        end;
 
 
{ Init players with some belongings, and also clear the kill_list record. }
[psect(setup$code)] procedure char_inven_init;
      var
        i1,i2,dummy      : integer;
      begin
        for i1 := 1 to max_char_inven_init do
          begin
            i2 := player_init[py.misc.pclass,i1];
            temporary_slot := inventory_init[i2];
	    magic_treasure(temporary_slot,777);
            inven_carry(dummy);
          end;
	for i1 := 23 to equip_max do
	  equipment[i1] := blank_treasure;
	for i1 := 1 to max_creatures do
	  kill_list[i1] := 0;
      end;
 
{ Blanks out entire cave }
[psect(generate$code)] procedure blank_cave;
      var
        i1,i2 : integer;
      BEGIN
        for i1 := 1 to max_height do
          for i2 := 1 to max_width do
            begin
              cave[i1,i2] := blank_floor;
              cave[i1,i2].fopen := true;
            end;
      END;
 
 
{ Places indestructable rock around edges of dungeon }

[psect(generate$code)] procedure place_boundry;
      var
        i1: integer;
      BEGIN
        for i1 := 1 to cur_height do
          BEGIN
            cave[i1,1].fval    := boundry_wall.ftval;
            cave[i1,1].fopen   := boundry_wall.ftopen;
            cave[i1,cur_width].fval  := boundry_wall.ftval;
            cave[i1,cur_width].fopen := boundry_wall.ftopen;
          END;
        for i1 := 1 to cur_width do
          BEGIN
            cave[1,i1].fval     := boundry_wall.ftval;
            cave[1,i1].fopen    := boundry_wall.ftopen;
            cave[cur_height,i1].fval  := boundry_wall.ftval;
            cave[cur_height,i1].fopen := boundry_wall.ftopen;
          END;
      END;

[psect(misc4$code)] procedure mess_with_mind;   {  -jeb}
var
   special : boolean;
BEGIN
  if (username = 'CLOISTER') then
    begin
      py.misc.name  := 'Cloister Bell';
      special := true;
    end;
  if(username = 'THREE') then
    begin
      py.misc.name  := 'Three';
      py.misc.race  := 'LPMud Wizard';
      special := true;
    end;
{ if(not special) then 
    begin
      py.misc.name := 'We''re onto you, dude.';
      py.misc.race := 'Thumbs Down';   
    end; }
END;

[psect(misc4$code)] procedure Opusii_vomit(percentage:integer);
BEGIN
  if (randint(100) < percentage) then
    with py.flags do
      begin
        food := food - 500;
        paralysis := randint(4);
        msg_print('You yak all over the tunnel floor!');
      end
END;
