
{ Generates a random dungeon level }
[psect(generate$code)] procedure generate_cave;
    type
        coords = record
                y : integer;
                x : integer;
        END;
    var
        doorstk : array [1..100] of coords;
        doorptr : integer;
	i1	: integer; 
 
{ Always picks a correct direction }
      procedure correct_dir(var rdir,cdir : integer; y1,x1,y2,x2 : integer);
        var
                test_dir  : integer;
        BEGIN
          if (y1 < y2) then
            rdir :=  1
          else if (y1 = y2) then
            rdir :=  0
          else
            rdir := -1;
          if (x1 < x2) then
            cdir :=  1
          else if (x1 = x2) then
            cdir :=  0
          else
            cdir := -1;
          if ((rdir <> 0) and (cdir <> 0)) then
            CASE randint(2) of
                1 :     rdir := 0;
                2 :     cdir := 0
            END
        END;
 
 
{ Chance of wandering direction }
      procedure rand_dir(var rdir,cdir : integer;
                             y1,x1,y2,x2,chance : integer);
        BEGIN
          CASE randint(chance) of
                1 :     BEGIN
                          rdir := -1;
                          cdir :=  0
                        END;
                2 :     BEGIN
                          rdir :=  1;
                          cdir :=  0
                        END;
                3 :     BEGIN
                          rdir :=  0;
                          cdir := -1
                        END;
                4 :     BEGIN
                          rdir :=  0;
                          cdir :=  1
                        END;
                otherwise correct_dir(rdir,cdir,y1,x1,y2,x2)
          END
        END;
 
 { Fills in empty spots with desired rock
   Note: 9 is a temporary value. }
    procedure fill_cave(fill : floor_type);
      var
        i1,i2   : integer;
      BEGIN
        for i1 := 2 to cur_height-1 do
          for i2 := 2 to cur_width-1 do
            with cave[i1,i2] do
              if (fval in [0,8,9]) then
                BEGIN
                  fval := fill.ftval;
                  fopen := fill.ftopen;
                END;
      END;
 
 
{ Places "streamers" of rock through dungeon }
    procedure place_streamer(rock : floor_type; treas_chance : integer);
      var
        i1,y,x,dir,ty,tx,t1,t2 : integer;
        flag : boolean;
      BEGIN
 
{ Choose starting point and direction }
        y := trunc(cur_height/2.0) + 11 - randint(23);
        x := trunc(cur_width/2.0)  + 16 - randint(33);
 
        dir := randint(8);      { Number 1-4, 6-9 }
        if (dir > 4) then dir := dir + 1;
 
{ Place streamer into dungeon }
        flag := false;  { Set to true when y,x are out-of-bounds}
        t1 := 2*dun_str_rng + 1;        { Constants     }
        t2 :=   dun_str_rng + 1;
        repeat
          for i1 := 1 to dun_str_den do
            BEGIN
              ty := y + randint(t1) - t2;
              tx := x + randint(t1) - t2;
              if (in_bounds(ty,tx)) then
                with cave[ty,tx] do
                  if (fval = rock_wall1.ftval) then
                    BEGIN
                      fval := rock.ftval;
                      fopen := rock.ftopen;
                      if (randint(treas_chance) = 1) then
                        place_gold(ty,tx);
                    END;
            END;
          if (not(move(dir,y,x))) then flag := true;
        until(flag);
      END;
 
 
{ Place a trap with a given displacement of point }
    procedure vault_trap(y,x,yd,xd,num : integer);
      var
        count,y1,x1,i1    : integer;
        flag  : boolean;
      BEGIN
        for i1 := 1 to num do
          BEGIN
            flag := false;
            count := 0;
            repeat
              y1 := y - yd - 1 + randint(2*yd+1);
              x1 := x - xd - 1 + randint(2*xd+1);
              with cave[y1,x1] do
                if (fval in floor_set) then
                  if (tptr = 0) then
                    BEGIN
                      place_trap(y1,x1,1,randint(max_trapa));
                      flag := true;
                    END;
              count := count + 1;
            until((flag) or (count > 5));
          END;
      END;
 
 
{ Place a monster next to the given point }
    procedure vault_monster(y,x,num : integer);
      var
                i1,y1,x1    : integer;
      BEGIN
        for i1 := 1 to num do
          BEGIN
            y1 := y;
            x1 := x;
            summon_monster(y1,x1,true);
          END;
      END;
 
 
{ Builds a room at a row,column coordinate }
    procedure build_room(yval,xval : integer);
      var
           y_height,y_depth,x_left,x_right,i1,i2 : integer;
           cur_floor   : floor_type;
      BEGIN
        if (dun_level <= randint(10)) then
          cur_floor := lopen_floor      { Floor with light }
        else
          cur_floor := dopen_floor;     { Dark floor }
        y_height := yval - randint(4);
        y_depth  := yval + randint(3);
        x_left   := xval - randint(11);
        x_right  := xval + randint(11);
        for i1 := y_height to y_depth do
          for i2 := x_left to x_right do
            BEGIN
              cave[i1,i2].fval  := cur_floor.ftval;
              cave[i1,i2].fopen := cur_floor.ftopen;
            END;
        for i1 := (y_height - 1) to (y_depth + 1) do
          BEGIN
            cave[i1,x_left-1].fval   := rock_wall1.ftval;
            cave[i1,x_left-1].fopen  := rock_wall1.ftopen;
            cave[i1,x_right+1].fval  := rock_wall1.ftval;
            cave[i1,x_right+1].fopen := rock_wall1.ftopen;
          END;
        for i1 := x_left to x_right do
          BEGIN
            cave[y_height-1,i1].fval  := rock_wall1.ftval;
            cave[y_height-1,i1].fopen := rock_wall1.ftopen;
            cave[y_depth+1,i1].fval   := rock_wall1.ftval;
            cave[y_depth+1,i1].fopen  := rock_wall1.ftopen;
          END
      END;
 
 
{ Builds a room at a row,column coordinate
  Type 1 unusual rooms are several overlapping rectangular ones }
    procedure build_type1(yval,xval : integer);
      var
         y_height,y_depth,x_left,x_right,i0,i1,i2 : integer;
         cur_floor   : floor_type;
      BEGIN
        if (dun_level <= randint(10)) then
          cur_floor := lopen_floor      { Floor with light }
        else
          cur_floor := dopen_floor;     { Dark floor }
        for i0 := 1 to (1 + randint(2)) do
          BEGIN
            y_height := yval - randint(4);
            y_depth  := yval + randint(3);
            x_left   := xval - randint(11);
            x_right  := xval + randint(11);
            for i1 := y_height to y_depth do
              for i2 := x_left to x_right do
                BEGIN
                  cave[i1,i2].fval  := cur_floor.ftval;
                  cave[i1,i2].fopen := cur_floor.ftopen;
                END;
            for i1 := (y_height - 1) to (y_depth + 1) do
              BEGIN
                with cave[i1,x_left-1] do
                  if (fval <> cur_floor.ftval) then
                    BEGIN
                      fval  := rock_wall1.ftval;
                      fopen := rock_wall1.ftopen;
                    END;
                with cave[i1,x_right+1] do
                  if (fval <> cur_floor.ftval) then
                    BEGIN
                      fval  := rock_wall1.ftval;
                      fopen := rock_wall1.ftopen;
                    END;
              END;
            for i1 := x_left to x_right do
              BEGIN
                with cave[y_height-1,i1] do
                  if (fval <> cur_floor.ftval) then
                    BEGIN
                      fval  := rock_wall1.ftval;
                      fopen := rock_wall1.ftopen;
                    END;
                with cave[y_depth+1,i1] do
                  if (fval <> cur_floor.ftval) then
                    BEGIN
                      fval  := rock_wall1.ftval;
                      fopen := rock_wall1.ftopen;
                    END;
              END;
          END;
      END;
 
 
{ Builds an unusual room at a row,column coordinate
  Type 2 unusual rooms all have an inner room:
   1 - Just an inner room with one door
   2 - An inner room within an inner room
   3 - An inner room with pillar(s)
   4 - Inner room has a maze
   5 - A set of four inner rooms }
    procedure build_type2(yval,xval : integer); 
      var
        y_height,y_depth,x_left,x_right,i1,i2 : integer;
        cur_floor			      : floor_type;
      BEGIN
        if (dun_level <= randint(10)) then
          cur_floor := lopen_floor      { Floor with light }
        else
          cur_floor := dopen_floor;     { Dark floor }
        y_height := yval - 4;
        y_depth  := yval + 4;
        x_left   := xval - 11;
        x_right  := xval + 11;
        for i1 := y_height to y_depth do
          for i2 := x_left to x_right do
            BEGIN
              cave[i1,i2].fval  := cur_floor.ftval;
              cave[i1,i2].fopen := cur_floor.ftopen;
            END;
        for i1 := (y_height - 1) to (y_depth + 1) do
          BEGIN
            cave[i1,x_left-1].fval   := rock_wall1.ftval;
            cave[i1,x_left-1].fopen  := rock_wall1.ftopen;
            cave[i1,x_right+1].fval  := rock_wall1.ftval;
            cave[i1,x_right+1].fopen := rock_wall1.ftopen;
          END;
        for i1 := x_left to x_right do
          BEGIN
            cave[y_height-1,i1].fval  := rock_wall1.ftval;
            cave[y_height-1,i1].fopen := rock_wall1.ftopen;
            cave[y_depth+1,i1].fval   := rock_wall1.ftval;
            cave[y_depth+1,i1].fopen  := rock_wall1.ftopen;
          END;
{ The inner room }
        y_height := y_height + 2;
        y_depth  := y_depth  - 2;
        x_left   := x_left   + 2;
        x_right  := x_right  - 2;
        for i1 := (y_height - 1) to (y_depth + 1) do
          BEGIN
            cave[i1,x_left-1].fval   := 8;
            cave[i1,x_right+1].fval  := 8;
          END;
        for i1 := x_left to x_right do
          BEGIN
            cave[y_height-1,i1].fval  := 8;
            cave[y_depth+1,i1].fval   := 8;
          END;
{ Inner room varitions }
        CASE randint(5) of
          1 :   BEGIN   { Just an inner room... }
                  CASE randint(4) of    { Place a door  }
                    1 : place_secret_door(y_height-1,xval);
                    2 : place_secret_door(y_depth+1,xval);
                    3 : place_secret_door(yval,x_left-1);
                    4 : place_secret_door(yval,x_right+1);
                  END;
                  vault_monster(yval,xval,1);
                END;
          2 :   BEGIN   { Treasure Vault }
                  CASE randint(4) of    { Place a door  }
                    1 : place_secret_door(y_height-1,xval);
                    2 : place_secret_door(y_depth+1,xval);
                    3 : place_secret_door(yval,x_left-1);
                    4 : place_secret_door(yval,x_right+1);
                  END;
                  for i1 := yval-1 to yval+1 do
                    BEGIN
                      cave[i1,xval-1].fval   := 8;
                      cave[i1,xval+1].fval   := 8;
                    END;
                  cave[yval-1,xval].fval  := 8;
                  cave[yval+1,xval].fval  := 8;
                  CASE randint(4) of    { Place a door  }
                    1 : place_locked_door(yval-1,xval);
                    2 : place_locked_door(yval+1,xval);
                    3 : place_locked_door(yval,xval-1);
                    4 : place_locked_door(yval,xval+1);
                  END;
{ Place an object in the treasure vault }
                  CASE randint(10) of
                    1 : place_up_stairs(yval,xval);
                    2 : place_down_stairs(yval,xval);
                    otherwise place_object(yval,xval);
                  END;
{ Guard the treasure well }
                  vault_monster(yval,xval,2+randint(3));
{ If the monsters don't get 'em.}
                  vault_trap(yval,xval,4,10,2+randint(3));
                END;
          3 :   BEGIN   { Inner pillar(s).}
                  CASE randint(4) of    { Place a door  }
                    1 : place_secret_door(y_height-1,xval);
                    2 : place_secret_door(y_depth+1,xval);
                    3 : place_secret_door(yval,x_left-1);
                    4 : place_secret_door(yval,x_right+1);
                  END;
                  for i1 := yval-1 to yval+1 do
                    for i2 := xval-1 to xval+1 do
                      cave[i1,i2].fval   := 8;
                  if (randint(2) = 1) then
                    BEGIN
                      CASE randint(2) of
                        1 : BEGIN
                              for i1 := yval-1 to yval+1 do
                                for i2 := xval-6 to xval-4 do
                                  cave[i1,i2].fval   := 8;
                              for i1 := yval-1 to yval+1 do
                                for i2 := xval+4 to xval+6 do
                                  cave[i1,i2].fval   := 8;
                            END;
                        2 : BEGIN
                              for i1 := yval-1 to yval+1 do
                                for i2 := xval-7 to xval-5 do
                                  cave[i1,i2].fval   := 8;
                              for i1 := yval-1 to yval+1 do
                                for i2 := xval+5 to xval+7 do
                                  cave[i1,i2].fval   := 8;
                            END;
                      END;
                      if (randint(3) = 1) then  { Inner rooms   }
                        BEGIN
                          for i1 := xval-5 to xval+5 do
                            BEGIN
                              cave[yval-1,i1].fval := 8;
                              cave[yval+1,i1].fval := 8;
                            END;
                          CASE randint(2) of
                            1 : place_secret_door(yval+1,xval-3);
                            2 : place_secret_door(yval-1,xval-3);
                          END;
                          CASE randint(2) of
                            1 : place_secret_door(yval+1,xval+3);
                            2 : place_secret_door(yval-1,xval+3);
                          END;
                          if (randint(3) = 1) then place_object(yval,xval-2);
                          if (randint(3) = 1) then place_object(yval,xval+2);
                          vault_monster(yval,xval-2,randint(2));
                          vault_monster(yval,xval+2,randint(2));
                        END;
                    END;
                END;
          4 :   BEGIN   { Maze inside..}
                  CASE randint(4) of    { Place a door  }
                    1 : place_secret_door(y_height-1,xval);
                    2 : place_secret_door(y_depth+1,xval);
                    3 : place_secret_door(yval,x_left-1);
                    4 : place_secret_door(yval,x_right+1);
                  END;
                  for i1 := y_height to y_depth do
                    for i2 := x_left to x_right do
                      if (odd(i2+i1)) then
                        cave[i1,i2].fval := 8;
{ Monsters just love mazes.}
                  vault_monster(yval,xval-5,randint(3));
                  vault_monster(yval,xval+5,randint(3));
{ Traps make them entertaining.}
                  vault_trap(yval,xval-3,2,8,randint(3));
                  vault_trap(yval,xval+3,2,8,randint(3));
{ Mazes should have some treasure too.. }
                  for i1 := 1 to 6 do
                    random_object(yval,xval,1);
                END;
                5 : BEGIN { four small rooms ...}
                        for i1 := y_height to y_depth do
                           cave[i1,xval].fval:=8;
                        for i1 := x_left to x_right do
                           cave[yval,i1].fval := 8;
                        CASE randint(3) of
                          1 : BEGIN
                                i1 := randint(10);
                                place_secret_door(y_height-1,xval-i1);
                                place_secret_door(y_height-1,xval+i1);
                                place_secret_door(y_depth+1,xval-i1);
                                place_secret_door(y_depth+1,xval+i1);
                             END;
                          2 : BEGIN
                                i1 := randint(3);
                                place_secret_door(yval+i1,x_left-1);
                                place_secret_door(yval-i1,x_left-1);
                                place_secret_door(yval+i1,x_right+1);
                                place_secret_door(yval-i1,x_right+1)
                              END;
                          3 : BEGIN { make them dig for it ... REB}
                                place_secret_door(yval,xval);
                                place_secret_door(y_height,xval)
                              END
                    END;
{ Treasure in each one.}
                  random_object(yval,xval,2+randint(2));
{ Gotta have some monsters.}
                  vault_monster(yval+2,xval-4,randint(2));
                  vault_monster(yval+2,xval+4,randint(2));
                  vault_monster(yval-2,xval-4,randint(2));
                  vault_monster(yval-2,xval+4,randint(2));
                END;
        END;
      END;
 
 
{ Builds a room at a row,column coordinate
  Type 3 unusual rooms are cross shaped }
    procedure build_type3(yval,xval : integer);
      var
        y_height,y_depth,x_left,x_right,i0,i1,i2 : integer;
        cur_floor   : floor_type;
      BEGIN
        if (dun_level <= randint(10)) then
          cur_floor := lopen_floor      { Floor with light }
        else
          cur_floor := dopen_floor;     { Dark floor }
        i0 := 2 + randint(2);
        y_height := yval - i0;
        y_depth  := yval + i0;
        x_left   := xval - 1;
        x_right  := xval + 1;
        for i1 := y_height to y_depth do
          for i2 := x_left to x_right do
            BEGIN
              cave[i1,i2].fval  := cur_floor.ftval;
              cave[i1,i2].fopen := cur_floor.ftopen;
            END;
        for i1 := (y_height - 1) to (y_depth + 1) do
          BEGIN
            with cave[i1,x_left-1] do
              BEGIN
                fval  := rock_wall1.ftval;
                fopen := rock_wall1.ftopen;
              END;
            with cave[i1,x_right+1] do
              BEGIN
                fval  := rock_wall1.ftval;
                fopen := rock_wall1.ftopen;
              END;
          END;
        for i1 := x_left to x_right do
          BEGIN
            with cave[y_height-1,i1] do
              BEGIN
                fval  := rock_wall1.ftval;
                fopen := rock_wall1.ftopen;
              END;
            with cave[y_depth+1,i1] do
              BEGIN
                fval  := rock_wall1.ftval;
                fopen := rock_wall1.ftopen;
              END;
          END;
        i0 := 2 + randint(9);
        y_height := yval - 1;
        y_depth  := yval + 1;
        x_left   := xval - i0;
        x_right  := xval + i0;
        for i1 := y_height to y_depth do
          for i2 := x_left to x_right do
            BEGIN
              cave[i1,i2].fval  := cur_floor.ftval;
              cave[i1,i2].fopen := cur_floor.ftopen;
            END;
        for i1 := (y_height - 1) to (y_depth + 1) do
          BEGIN
            with cave[i1,x_left-1] do
              if (fval <> cur_floor.ftval) then
                BEGIN
                  fval  := rock_wall1.ftval;
                  fopen := rock_wall1.ftopen;
                END;
            with cave[i1,x_right+1] do
              if (fval <> cur_floor.ftval) then
                BEGIN
                  fval  := rock_wall1.ftval;
                  fopen := rock_wall1.ftopen;
                END;
          END;
        for i1 := x_left to x_right do
          BEGIN
            with cave[y_height-1,i1] do
              if (fval <> cur_floor.ftval) then
                BEGIN
                  fval  := rock_wall1.ftval;
                  fopen := rock_wall1.ftopen;
                END;
            with cave[y_depth+1,i1] do
              if (fval <> cur_floor.ftval) then
                BEGIN
                  fval  := rock_wall1.ftval;
                  fopen := rock_wall1.ftopen;
                END;
          END;
{ Special features.}
        CASE randint(4) of
          1 :   BEGIN   { Large middle pillar  }
                  for i1 := yval-1 to yval+1 do
                    for i2 := xval-1 to xval+1 do
                      cave[i1,i2].fval := 8;
                END;
          2 :   BEGIN   { Inner treasure vault }
                  for i1 := yval-1 to yval+1 do
                    BEGIN
                      cave[i1,xval-1].fval   := 8;
                      cave[i1,xval+1].fval   := 8;
                    END;
                  cave[yval-1,xval].fval  := 8;
                  cave[yval+1,xval].fval  := 8;
                  CASE randint(4) of    { Place a door  }
                    1 : place_secret_door(yval-1,xval);
                    2 : place_secret_door(yval+1,xval);
                    3 : place_secret_door(yval,xval-1);
                    4 : place_secret_door(yval,xval+1);
                  END;
{ Place a treasure in the vault }
                  place_object(yval,xval);
{ Let's gaurd the treasure well.}
                  vault_monster(yval,xval,2+randint(2));
{ Traps naturally }
                  vault_trap(yval,xval,4,4,1+randint(3));
                END;
          3 :   BEGIN
                  if (randint(3) = 1) then
                    BEGIN
                      cave[yval-1,xval-2].fval := 8;
                      cave[yval+1,xval-2].fval := 8;
                      cave[yval-1,xval+2].fval := 8;
                      cave[yval-1,xval+2].fval := 8;
                      cave[yval-2,xval-1].fval := 8;
                      cave[yval-2,xval+1].fval := 8;
                      cave[yval+2,xval-1].fval := 8;
                      cave[yval+2,xval+1].fval := 8;
                      if (randint(3) = 1) then
                        BEGIN
                          place_secret_door(yval,xval-2);
                          place_secret_door(yval,xval+2);
                          place_secret_door(yval-2,xval);
                          place_secret_door(yval+2,xval);
                        END;
                    END
                  else if (randint(3) = 1) then
                    BEGIN
                      cave[yval  ,xval  ].fval := 8;
                      cave[yval-1,xval  ].fval := 8;
                      cave[yval+1,xval  ].fval := 8;
                      cave[yval  ,xval-1].fval := 8;
                      cave[yval  ,xval+1].fval := 8;
                    END
                  else if (randint(3) = 1) then
                    cave[yval,xval].fval := 8;
                END;
          4 :   ;
        END;
      END;

{Builds a room with a circular inner room. - RLG} 
    procedure build_type4(yval,xval : integer);
      var
        i5,i6 				: integer;
	y_height,y_depth,x_left,x_right : integer;
        cur_floor			: floor_type;
      BEGIN
        if (dun_level <= randint(10)) then
          cur_floor := lopen_floor      { Floor with light }
        else
          cur_floor := dopen_floor;     { Dark floor }
        y_height := yval - 4;
        y_depth  := yval + 4;
        x_left   := xval - 11;
        x_right  := xval + 11;
        for i5 := y_height to y_depth do
          for i6 := x_left to x_right do
            begin
              cave[i5,i6].fval  := cur_floor.ftval;
              cave[i5,i6].fopen := cur_floor.ftopen;
            end;
	for i5 := 1 to 9 do
  	    begin
	      i6 := i5-1;
	      cave[yval+circle[i5],xval+i6].fval  := 8;
	      cave[yval-circle[i5],xval+i6].fval  := 8;
	      cave[yval+circle[i5],xval-i6].fval  := 8;
	      cave[yval-circle[i5],xval-i6].fval  := 8;
	    end;
	vault_trap(yval,xval,2,4,2);
	random_object(yval,xval,2);
	vault_monster(yval,xval,3);
     End; 

{ Builds collapsed room  --jeb}
    procedure build_type5(yval,xval : integer);
       var 
        i1, i2				: integer;
	y_height,y_depth,x_left,x_right : integer;
        cur_floor			: floor_type;

      BEGIN  
        y_height := yval - 4;
        y_depth  := yval + 4;
        x_left   := xval - 11;
        x_right  := xval + 11;
        if (dun_level <= randint(10)) then
          cur_floor := lopen_floor      { Floor with light }
        else
          cur_floor := dopen_floor;     { Dark floor }
	for i1 := y_height to y_depth do
	  for i2 := x_left   to x_right do
	    if (randint(4) > 1) then
	      begin
	        cave[i1,i2].fval  := cur_floor.ftval;
	        cave[i1,i2].fopen := cur_floor.ftopen;
	      end
	    else
	      begin
                cave[i1,i2].fval  := 8;  {non-tunnelable rock}
	        cave[i1,i2].fopen := false;
	      end;
	vault_monster(yval,xval,5);
	vault_trap(yval,xval,3,10,5);
	random_object(yval,xval,5);
      END; 


{ Constructs a tunnel between two points }
    procedure tunnel(row1,col1,row2,col2 : integer);

      var
        tmp_row,tmp_col,row_dir,col_dir,i1,i2,tmp   : integer;
        tunstk: array [1..1000] of coords;
        wallstk : array [1..1000] of coords;
        tunptr,wallptr : integer;
        stop_flag,door_flag : boolean;
 
{ Main procedure for Tunnel
  Note: 9 is a temporary value }
      BEGIN
        stop_flag := false;
        door_flag := false;
        tunptr    := 0;
        wallptr   := 0;
        correct_dir(row_dir,col_dir,row1,col1,row2,col2);
        repeat
          if (randint(100) > dun_tun_chg) then
            rand_dir(row_dir,col_dir,row1,col1,row2,col2,dun_tun_rnd);
          tmp_row := row1 + row_dir;
          tmp_col := col1 + col_dir;
          while (not(in_bounds(tmp_row,tmp_col))) do
            BEGIN
              rand_dir(row_dir,col_dir,row1,col1,row2,col2,dun_tun_rnd);
              tmp_row := row1 + row_dir;
              tmp_col := col1 + col_dir;
            END;
          with cave[tmp_row,tmp_col] do
            if (fval = rock_wall1.ftval) then
              BEGIN
                row1 := tmp_row;
                col1 := tmp_col;
                if (wallptr < 1000) then
                  wallptr := wallptr + 1;
                wallstk[wallptr].y := row1;
                wallstk[wallptr].x := col1;
                for i1 := row1-1 to row1+1 do
                  for i2 := col1-1 to col1+1 do
                    if (in_bounds(i1,i2)) then
                      with cave[i1,i2] do
                        if (fval in wall_set) then
                          fval := 9;
              END
            else if (fval = corr_floor1.ftval) then
              BEGIN
                row1 := tmp_row;
                col1 := tmp_col;
                if (not(door_flag)) then
                  BEGIN
                    if (doorptr <= 100) then
                      BEGIN
                        doorptr := doorptr + 1;
                        doorstk[doorptr].y := row1;
                        doorstk[doorptr].x := col1;
                      END;
                    door_flag := true;
                  END;
                if (randint(100) > dun_tun_con) then
                  stop_flag := true;
              END
            else if (fval = 0) then
              BEGIN
                row1 := tmp_row;
                col1 := tmp_col;
                if (tunptr < 1000) then
                  tunptr := tunptr + 1;
                tunstk[tunptr].y := row1;
                tunstk[tunptr].x := col1;
                door_flag := false;
              END
            else if (fval <> 9) then
              BEGIN
                row1 := tmp_row;
                col1 := tmp_col;
              END;
        until (((row1 = row2) and (col1 = col2)) or (stop_flag));
        for i1 := 1 to tunptr do
          BEGIN
            cave[tunstk[i1].y,tunstk[i1].x].fval  := corr_floor1.ftval;
            cave[tunstk[i1].y,tunstk[i1].x].fopen := corr_floor1.ftopen;
          END;
        for i1 := 1 to wallptr do
          with cave[wallstk[i1].y,wallstk[i1].x] do
            if (fval = 9) then
              BEGIN
                if (randint(100) < dun_tun_pen) then
                  place_door(wallstk[i1].y,wallstk[i1].x)
                else
                  BEGIN
                    fval  := corr_floor2.ftval;
                    fopen := corr_floor2.ftopen;
                  END;
              END;
      END;
 
 
{ Places door at y,x position if at least 2 walls found }
    procedure try_door(y,x : integer);
 
      function next_to(y,x : integer) : boolean;
        BEGIN
          if (next_to8(y,x,[4,5,6]) > 2) then
            if ((cave[y-1,x].fval in wall_set) and
                (cave[y+1,x].fval in wall_set)) then
              next_to := true
            else if ((cave[y,x-1].fval in wall_set) and
                     (cave[y,x+1].fval in wall_set)) then
              next_to := true
            else
              next_to := false
          else
            next_to := false
        END;
 
      BEGIN
        if (randint(100) > dun_tun_jct) then
          if (cave[y,x].fval = corr_floor1.ftval) then
            if (next_to(y,x)) then
              place_door(y,x);
      END;
 
 
{ Cave logic flow for generation of new dungeon }
    procedure cave_gen;
      type
        spot_type = record
            ENDx : integer;
            ENDy : integer;
        END;
        room_type = array [1..20,1..20] of boolean;
      var
        room_map    : room_type;
        i1,i2,i3,i4,y1,x1,y2,x2,pick1,pick2,row_rooms,col_rooms,
        alloc_level,bx,by : integer;
        yloc,xloc  : array [1..400] of worlint;
        bm_found : boolean;
 
BEGIN
        seed := get_seed;
        row_rooms := 2*trunc(cur_height/screen_height);
        col_rooms := 2*trunc(cur_width /screen_width);
        for i1 := 1 to row_rooms do
          for i2 := 1 to col_rooms do
            room_map[i1,i2] := false;
        for i1 := 1 to randnor(dun_roo_mea,2) do
          room_map[randint(row_rooms),randint(col_rooms)] := true;
        i3 := 0;
        for i1 := 1 to row_rooms do
          for i2 := 1 to col_rooms do
            if (room_map[i1,i2] = true) then
              BEGIN
                i3 := i3 + 1;
                yloc[i3] := (i1-1)*(quart_height*2 + 1) + quart_height + 1;
                xloc[i3] := (i2-1)*(quart_width*2  + 1) + quart_width  + 1;
                if (dun_power > randint(dun_unusual)) then
                  CASE randint(5) of
                    1 : build_type1(yloc[i3],xloc[i3]);
                    2 : build_type2(yloc[i3],xloc[i3]);
                    3 : build_type3(yloc[i3],xloc[i3]);
                    4 : build_type4(yloc[i3],xloc[i3]);
		    5 : build_type5(yloc[i3],xloc[i3]);
                  END
                else
                  build_room(yloc[i3],xloc[i3]);
              END;
        for i4 := 1 to i3 do
          BEGIN
            pick1 := randint(i3);
            pick2 := randint(i3);
            y1 := yloc[pick1];
            x1 := xloc[pick1];
            yloc[pick1] := yloc[pick2];
            xloc[pick1] := xloc[pick2];
            yloc[pick2] := y1;
            xloc[pick2] := x1
          END;
        doorptr := 0;
        for i4 := 1 to i3-1 do
          BEGIN
            y1 := yloc[i4];
            x1 := xloc[i4];
            y2 := yloc[i4+1];
            x2 := xloc[i4+1];
            tunnel(y2,x2,y1,x1)
          END;
        fill_cave(rock_wall1);
        for i1 := 1 to dun_str_mag do
          place_streamer(rock_wall2,dun_str_mc);
        for i1 := 1 to dun_str_qua do
          place_streamer(rock_wall3,dun_str_qc);
       place_boundry;
{ Place intersection doors }
        for i1 := 1 to doorptr do
          BEGIN
            try_door(doorstk[i1].y,doorstk[i1].x-1);
            try_door(doorstk[i1].y,doorstk[i1].x+1);
            try_door(doorstk[i1].y-1,doorstk[i1].x);
            try_door(doorstk[i1].y+1,doorstk[i1].x);
          END;
	alloc_level := dun_power div 3;
        if (alloc_level < 2) then
          alloc_level := 2
        else if (alloc_level > 10) then
          alloc_level := 10;
         if (dun_level < townlist[town_num].max_depth) then
	   place_stairs(2,randint(3)+2,3);
         place_stairs(1,randint(3)+2,3);
         alloc_monster([1,2],(randint(8)+min_malloc_level+alloc_level),0,true);
         alloc_object([4],3,randint(alloc_level));
         alloc_object([1,2],5,randnor(treas_room_alloc,3));
         alloc_object([1,2,4],5,randnor(treas_any_alloc,3));
         alloc_object([1,2,4],4,randnor(treas_gold_alloc,3));
         alloc_object([1,2,4],1,randint(alloc_level));
         if ((py.misc.lev > 5) and (randint(5) = 1))
         then				{Requirement for BM to appear -mwk}
           BEGIN
             bm_found:=false;
             while (NOT(bm_found)) do
               BEGIN
                 by := randint(max_height);
                 bx := randint(max_width);
                 if (cave[by,bx].fopen) then
                   BEGIN
                     place_trap(by,bx,3,0);
                     do_black_market;
                     bm_found := true
                   END
               END
           END;
      END;

{ Logic flow for generation of new *special* dungeons - 200,250 meters}

    procedure special_cave_gen(number : integer);

      var 
        alloc_level,i1,i2,rx,ry	:	integer;
	row_dir,col_dir		:	integer;
BEGIN
        seed := get_seed; 
	case number of
	  1: Begin
	       for i1 := 1 to cur_height do
	         for i2 := 1 to cur_width do
		   begin
		     cave[i1,i2].pl := true;
		     if (randint(5) = 1) then 
		       begin
		         cave[i1,i2].fval  := rock_wall1.ftval;
			 cave[i1,i2].fopen := rock_wall1.ftopen;
		       end
		     else
		       cave[i1,i2].fval := lopen_floor.ftval;
		   end;
	     End;

	  2: Begin
	       for i1 := 1 to cur_height do
	         for i2 := 1 to cur_width do
		   begin
		     cave[i1,i2].fval  := dopen_floor.ftval;
		     cave[i1,i2].fopen := dopen_floor.ftopen;
		     if (randint(10) > 3) then  {make dungeon 70% lit}
		       cave[i1,i2].pl  := true;
		   end;	       
	       for i1 := 1 to 90 do
	         begin
		   ry := randint(cur_height-2)+1;	
		   rx := randint(cur_width-2)+1;
		   for i2 := 1 to 60 do
		     begin	       		   	 
		       rand_dir(row_dir,col_dir,0,0,0,0,4);
		       if in_bounds(ry+row_dir,rx+col_dir) then
		         begin
  		           ry := ry + row_dir;
 		           rx := rx + col_dir;
		           cave[ry,rx].fval  := rock_wall2.ftval;
		           cave[ry,rx].fopen := rock_wall2.ftopen;
		         end;
		     end;		       		       
		 end;		       
	     End;	  
	  3: Begin
	       fill_cave(rock_wall3);
	       for i1 := 1 to 300 do
	         begin
		   ry := randint(cur_height-2)+1;	
		   rx := randint(cur_width-2)+1;
		   for i2 := 1 to 60 do
		     begin	       		   	 
		       rand_dir(row_dir,col_dir,0,0,0,0,4);
		       if in_bounds(ry+row_dir,rx+col_dir) then
		         begin
  		           ry := ry + row_dir;
 		           rx := rx + col_dir;
		           cave[ry,rx].fval  := dopen_floor.ftval;
		           cave[ry,rx].fopen := dopen_floor.ftopen;
		         end;
		     end;		       		       
		 end;		       
	     End;	  
	end; {case}
        place_boundry;
        alloc_level := dun_power div 3;
        if (alloc_level < 2) then
          alloc_level := 2
        else if (alloc_level > 10) then
          alloc_level := 10;
         if (dun_level < townlist[town_num].max_depth) then
	   place_stairs(2,randint(5)+5,3);
         place_stairs(1,randint(5)+5,3);
        alloc_monster([1,2],(randint(40)+min_malloc_level+alloc_level),0,true);
         alloc_object([1,2],5,randnor(treas_room_alloc,3));
         alloc_object([1,2],5,randnor(treas_any_alloc,3));
         alloc_object([1,2],4,randnor(treas_gold_alloc,3));
         alloc_object([1,2],1,randint(alloc_level));
END;


{Logic Flow for special "Bottom of Cave" type.} 
    procedure bottom_cave_gen;

      type
        spot_type = record
            ENDx : integer;
            ENDy : integer;
        END;
        room_type = array [1..20,1..20] of boolean;
      var
        room_map    : room_type;
        i1,i2,i3,i4,y1,x1,y2,x2,pick1,pick2,row_rooms,col_rooms,
        alloc_level,bx,by : integer;
        yloc,xloc  : array [1..400] of worlint;
      BEGIN
        seed := get_seed;
        row_rooms := 2*trunc(cur_height/screen_height);
        col_rooms := 2*trunc(cur_width /screen_width);
        for i1 := 1 to row_rooms do
          for i2 := 1 to col_rooms do
            room_map[i1,i2] := false;
        for i1 := 1 to randnor(dun_roo_mea,2) do
          room_map[randint(row_rooms),randint(col_rooms)] := true;
        i3 := 0;
        for i1 := 1 to row_rooms do
          for i2 := 1 to col_rooms do
            if (room_map[i1,i2] = true) then
              BEGIN
                i3 := i3 + 1;
                yloc[i3] := (i1-1)*(quart_height*2 + 1) + quart_height + 1;
                xloc[i3] := (i2-1)*(quart_width*2  + 1) + quart_width  + 1;
                CASE randint(5) of
                  1 : build_type1(yloc[i3],xloc[i3]);
                  2 : build_type2(yloc[i3],xloc[i3]);
                  3 : build_type3(yloc[i3],xloc[i3]);
                  4 : build_type4(yloc[i3],xloc[i3]);
                  5 : build_type5(yloc[i3],xloc[i3]);
                END
              END;
        place_boundry;	      
        for i1 := 1 to cur_height do
	  for i2 := 1 to cur_width do
	    with cave[i1,i2] do
	      if ((fval = rock_wall1.ftval) and (randint(10) = 1)) then
	        place_secret_door(i1,i2)
	      else if (fval = 0) then
	        begin
	          fval := dopen_floor.ftval;  {tval=1}
		  fopen := dopen_floor.ftopen;  
		end
	      else if (fval in [8,9]) then
	        begin
	          fval := rock_wall1.ftval;
		  fopen := rock_wall1.ftopen;
		end;
        if (dun_level < townlist[town_num].max_depth) then
          place_stairs(2,randint(3)+5,3);
        place_stairs(1,randint(3)+5,3);
        alloc_monster([1],(randint(20)+min_malloc_level),0,true);
        if (dun_power = win_mon_appear) then
	  if (not(is_boss_dead or out_of_time)) then 
	    place_the_boss;
	if ((dun_level = 10) and (not(defeated_badguy))) then
	  place_badguy_monster(town_num);
      END;

{ Town  ::  Builds a store at a row,column coordinate }
    procedure build_store(store_num,y,x,num_in_row : integer);
     var
     yval,y_height,y_depth,xval,x_adjst,x_left,x_right,i1,i2,cur_pos : integer;
      BEGIN
        yval     := (y-1)*6 + 5;
        xval     := (x-1)*(40-6*num_in_row) + (21-3*num_in_row);
          { this formula yields xval=(x-1)*22+11 if 3 stores in the row
                            and xval=(x-1)*16+8 for 4 stores in the row. }
        x_adjst  := 6 - (num_in_row - 3)*3;
        y_height := yval - randint(2);
        y_depth  := yval + randint(2);
        x_left   := xval - 1 - randint(x_adjst);
        x_right  := xval + 1 + randint(x_adjst);
        for i1 := y_height to y_depth do
          for i2 := x_left to x_right do
            BEGIN
              cave[i1,i2].fval  := boundry_wall.ftval;
              cave[i1,i2].fopen := boundry_wall.ftopen;
            END;
        CASE randint(4) of { determines which wall and where to put the door }
          1 : BEGIN
                i1 := randint(y_depth-y_height) + y_height - 1;
                i2 := x_left;
              END;
          2 : BEGIN
                i1 := randint(y_depth-y_height) + y_height - 1;
                i2 := x_right;
              END;
          3 : BEGIN
                i1 := y_depth;
                i2 := randint(x_right-x_left) + x_left - 1;
              END;
          4 : BEGIN
                i1 := y_height;
                i2 := randint(x_right-x_left) + x_left - 1;
              END;
        END;
        with cave[i1,i2] do
          BEGIN
            fval  := corr_floor3.ftval;
            fopen := corr_floor3.ftopen;
            popt(cur_pos);
            tptr := cur_pos;
            t_list[cur_pos] := store_door[store_num];
          END;
      END;
 

{ Town logic flow for generation of new town }
    procedure town_gen;
      type
        room_type = array [1..3,1..3] of boolean;
      var
        y,x,i1,i2,i3,i4,i5    : integer;
        rooms : array [1..max_doors] of integer;
 
      BEGIN
        seed := town_seed;
        for i1 := 1 to max_doors do rooms[i1] := i1;
        i4 := max_doors;      {i4 is store counter}
        for i1 := 1 to 3 do    {y value}
          for i2 := 1 to 4 do  {x value}
            BEGIN
              i3 := randint(i4);
              build_store(rooms[i3],i1,i2,4); {4 is number in row}
              for i5 := i3 to i4-1 do rooms[i5] := rooms[i5+1];
              i4 := i4 - 1;
            END;
        fill_cave(dopen_floor);
        place_boundry;
        if ((turn < 4320) or (turn > 12960)) then
          BEGIN          {Night}
            for i1 := 1 to cur_height do
              for i2 := 1 to cur_width do
                if (cave[i1,i2].fval <> dopen_floor.ftval) then
                  cave[i1,i2].pl := true;
            place_stairs(2,3,0);
            seed := get_seed;
            alloc_monster([1,2],min_malloc_tn,3,true);
          END
        else
          BEGIN         { Day }
            for i1 := 1 to cur_height do
              for i2 := 1 to cur_width do
                cave[i1,i2].pl := true;
            place_stairs(2,3,0);
            seed := get_seed;
            alloc_monster([1,2],min_malloc_td,3,true);
         END;
      END;

{ Logic flow for generation of tall building }
    procedure building_gen; 

  var	      i1,i2,cur_pos 	: 	integer;


    procedure place_building_door(spoty,spotx,door_num : integer);

    Begin
      with cave[spoty,spotx] do
        begin
          fval  := corr_floor3.ftval;
          fopen := corr_floor3.ftopen;
          popt(cur_pos);
          tptr := cur_pos;
          t_list[cur_pos] := building_door[door_num];
	end
    End;

  BEGIN
        fill_cave(lopen_floor);
	place_boundry;	
	for i2 := 1 to 30 do
	  with cave[5,i2] do
	    begin
	      fval := boundry_wall.ftval;
	      fopen := boundry_wall.ftopen;
	    end;	    
	for i1 := 2 to 9 do
	  begin
	    for i2 := 2 to 7 do
	      with cave[i1,i2] do
	        begin
		  fval := boundry_wall.ftval;
		  fopen := boundry_wall.ftopen;
		end;
	    for i2 := 23 to 29 do
	      with cave[i1,i2] do
	        begin
		  fval := boundry_wall.ftval;
		  fopen := boundry_wall.ftopen;
		end;
          end;
        place_building_door(7,7,1);  {Hotel}
        place_building_door(7,23,2); {Bank}
        place_building_door(3,7,3);  {Clinic}
        place_building_door(3,23,4); {NewsInc}
	place_closed_door(5,15);
	place_down_stairs(9,15);
	char_row :=  8;
	char_col := 15;
  END;
 

{Main procedure} 
    BEGIN
      panel_row_min     := 0;
      panel_row_max     := 0;
      panel_col_min     := 0;
      panel_col_max     := 0;
      char_row    := -1;
      char_col    := -1;
      tlink;
      mlink;
      blank_cave;
 
      if (dun_level < 0) then 
        Begin
          cur_height := 10;
          cur_width  := 30;
          max_panel_rows := 0;
          max_panel_cols := 0;
          panel_row := max_panel_rows;
          panel_col := max_panel_cols;
	  dun_power := 0;
          building_gen;
        End
      else if (dun_level = 0) then
        BEGIN
          cur_height := screen_height;
          cur_width  := screen_width;
          max_panel_rows := trunc(cur_height/screen_height)*2 - 2;
          max_panel_cols := trunc(cur_width /screen_width )*2 - 2;
          panel_row := max_panel_rows;
          panel_col := max_panel_cols;
	  dun_power := 0;
          town_gen;
	  if (exited_office) then
	    begin
	      char_row := office_char_row;
	      char_col := office_char_col;
	      exited_office := false;
	    end;
        END
      else
        BEGIN
          cur_height := max_height;
          cur_width  := max_width;
          max_panel_rows := trunc(cur_height/screen_height)*2 - 2;
          max_panel_cols := trunc(cur_width /screen_width )*2 - 2;
          panel_row := max_panel_rows;
          panel_col := max_panel_cols;
	  dun_power := dun_level + townlist[town_num].lev_plus;
	  case (dun_level mod 10) of
	         0:    bottom_cave_gen;
	         5:    special_cave_gen(1);
	       3,8:    special_cave_gen(2);
	       2,7:    special_cave_gen(3);
	     otherwise         cave_gen;	 
	       end;
        END;
    END;





