{This Include file contains all the inventory display routines.
    Many of which have been extremely modified by  -RLG}


{ Returns a '*' for cursed items, a ')' for normal ones
  NOTE: '*' returned only if item has been identified.}
    function cur_char1(item_val : integer) : dtype;
      BEGIN
        with inventory[item_val] do
          if (uand(%X'80000000',flags) = 0) then
            cur_char1 := ')'     { Not cursed.}
          else if (index(name,'|') > 0) then
            cur_char1 := ')'     { Cursed, but not identified }
          else if (index(name,'^') > 0) then
            cur_char1 := ')'     { Cursed, but not identified }
          else
            cur_char1 := '*'    { Cursed and identified.}
      END;
 
 
{ Returns a '*' for cursed items, a ')' for normal ones }
    function cur_char2(item_val : integer) : char;
      BEGIN
        with equipment[item_val] do
          if (uand(%X'80000000',flags) = 0) then
            cur_char2 := ')' { Not cursed.}
          else
            cur_char2 := '*';   { Cursed.}
      END;

 {Returns the character label (a-Z) of an integer.}
    function raoul_label(position : integer) : dtype;
      BEGIN
        raoul_label  := substr(alpha_set,position,1);
      END;
 
 
{ Comprehensive function block to handle all inventory
  and equipment routines.  Five kinds of calls can take place.
  Note that '?' is a special call for other routines to display
  only a portion of the inventory, and take no other action. }

    function inven_command(command : char; r1,r2 : integer) : boolean;
      var
        com_val,scr_state   : integer;
	cur_top		    : integer;
        exit_flag,test_flag : boolean;
 
 { Displays inventory items from r1 to r2 }
 {r1 can be cur_top or start of range,  r2 can be inven_ctr or end of range}

      procedure show_inven(r1,r2 : integer);  {Much Improved by RLG}
        var
          i1 		   : integer;
          tmp_val,out_val  : vtype;

        BEGIN
          if (r1 > 0) then    { R1 = 0 dummy call }
            BEGIN
	      clear(2,1);
              scr_state := 1; { Set state to 1 }
	      if (r2-r1 > inven_per_page-1) then
	        r2 := r1 + inven_per_page-1;
              for i1 := 0 to (r2-r1) do
		    begin
		      objdes(tmp_val,r1+i1,true,'i');
                      writev(out_val,raoul_label(r1+i1),
		      		     cur_char1(r1+i1),' ',tmp_val);
                      prt(out_val,i1+2,1);
		    end;
            END;
        END;

{ Displays equipment items from r1 to end }
      procedure show_equip(r1 : integer);
        var
          i1,i2 : integer;
          prt1,prt2,out_val : vtype;
        BEGIN
          if (r1 > equip_ctr) then   { Last item gone }
            prt('',equip_ctr+3,1)
          else if (r1 > 0) then   { R1 = 0 dummy call }
            BEGIN
              i2 := 0;
              for i1 := 23 to equip_max do{ Range of equipment }
                BEGIN
                  with equipment[i1] do
                    if (tval > 0) then
                      BEGIN
                        i2 := i2 + 1;
                        if (i2 >= r1) then{ Display only given range }
                          BEGIN
                            CASE i1 of { Get position }
                              23 :      prt1 := ' You are wielding   : ';
                              24 :      prt1 := ' Worn on head       : ';
                              25 :      prt1 := ' Worn around neck   : ';
                              26 :      prt1 := ' Worn on body       : ';
                              27 :      prt1 := ' Worn on arm        : ';
                              28 :      prt1 := ' Worn on hands      : ';
                              29 :      prt1 := ' Worn on right hand : ';
                              30 :      prt1 := ' Worn on left hand  : ';
                              31 :      prt1 := ' Worn on feet       : ';
                              32 :      prt1 := ' Worn about body    : ';
                              33 :      prt1 := ' Light source       : ';
                              34 :      prt1 := ' Secondary weapon   : ';
                              35 :      prt1 := ' Computer           : ';
                              otherwise prt1 := ' Unknown value: ';
                            END;
                            objdes(prt2,i1,true,'e');
                            writev(out_val,chr(i2+96),cur_char2(i1),
                                                                prt1,prt2);
                            prt(out_val,i2+2,1);
                         END;
                      END;
                END;
              prt('',i2+3,1);   { Clear last line }
              scr_state := 2;   { Set state of screen }
            END;
        END;

{Summary of player's inventory displayed to the screen} 
  procedure  count_inventory;
    var
      i1,i2,i3,row,num	: integer;
   BEGIN
     num := 0;
     prt('Inventory Summary',16,22);
     row := 17;
     if (find_range([20..23],i2,i3)) then
       begin
         num := i3-i2+1;
	 row := row+1;
	 prt_num('Weapons         : ',num,row,7);
       end;
     if (find_range([30..36],i2,i3)) then
       begin
         num := i3-i2+1;
	 row := row+1;
	 prt_num('Armor           : ',num,row,7);
       end;
     if (find_range([82,84],i2,i3)) then
       begin
         num := 0;
         for i1 := i2 to i3 do
	   num := num + inventory[i1].number;
	 row := row+1;
	 prt_num('Thrown/Grenades : ',num,row,7);
       end;
     if (find_range([10,11,12],i2,i3)) then
       begin
         num := 0;
         for i1 := i2 to i3 do
	   num := num + inventory[i1].number;
	 row := row+1;
	 prt_num('Rounds of Ammo  : ',num,row,7);
       end;
     if (find_range([80],i2,i3)) then
       begin
         num := 0;
         for i1 := i2 to i3 do
	   num := num + inventory[i1].number;
	 row := row+1;
	 prt_num('Food Items      : ',num,row,7);
       end;
     row := 17;
     if (find_range([75,76],i2,i3)) then
       begin
         num := 0;
         for i1 := i2 to i3 do
	   num := num + inventory[i1].number;
	 row := row+1;
	 prt_num('Potions         : ',num,row,35);
       end;
     if (find_range([70,71],i2,i3)) then
       begin
         num := 0;
         for i1 := i2 to i3 do
	   num := num + inventory[i1].number;
	 row := row+1;
	 prt_num('Floppy Disks    : ',num,row,35);
       end;
     if (find_range([65],i2,i3)) then
       begin
         num := i3-i2+1;
	 row := row+1;
	 prt_num('Ray Guns        : ',num,row,35);
       end;
     if (find_range([55],i2,i3)) then
       begin
         num := i3-i2+1;
	 row := row+1;
	 prt_num('Wierd Devices   : ',num,row,35);
       end;
     if (find_range([40,45],i2,i3)) then
       begin
         num := i3-i2+1;
	 row := row+1;
	 prt_num('Amulets/Rings   : ',num,row,35);
       end;
  END;

{ Displays inventory items and cleans item from list }
      procedure clean;
      var
                com_val	  : integer;
                out_val	  : vtype;
		command	  : char;
		exit_flag,test_flag : boolean;
      BEGIN
          exit_flag := false;
	  repeat
	    test_flag := false;
	    repeat
              writev(out_val,
	        '( <space> for next page, <esc> to exit ) ',
	        'Lose which item ?');
              prt(out_val,1,1);
	      inkey(command);
	      com_val := ord(command);
	      case com_val of
         0,3,13,25,26,27  :  begin
			      test_flag := true;
			      exit_flag := true;
			     end;		            
		       32 :  begin
			       if (cur_top+inven_per_page <= inven_ctr) then
 				 cur_top := cur_top + inven_per_page
			       else
			         cur_top := 1;
                               show_inven(cur_top,inven_ctr);
                             end;

                 otherwise   begin
			       com_val := index(alpha_set,command);
			       if (com_val in [1..inven_ctr]) then
                                 begin
			           objdes(out_val,com_val,true,'i');
			           out_val := 'Destroyed ' + out_val;
			           inven_destroy(com_val);
			           show_inven(cur_top,inven_ctr);
			           msg_print(out_val);
			           msg_print('');
                                   test_flag := true;
                                 end;
			     end;
	      end;
	    until (test_flag);
          until (exit_flag);
          erase_line(msg_line,msg_line);
    END;
 
{ Remove item from equipment list }
      function remove(item_val : integer) : integer;
        var
          i1,i2,typ  	    : integer;
          out_val,prt1,prt2 : vtype;
          flag		    : boolean;
	  item_removed	    : treasure_type;

        BEGIN
          i1 := 0;
          flag := false;
	  if (item_val = -1) then
	    item_removed := temporary_slot
	  else
            item_removed := equipment[item_val];
	  typ := item_removed.tval;
          repeat
            i1 := i1 + 1;
            with inventory[i1] do
              if (typ > tval) then
                BEGIN
                  for i2 :=  inven_ctr downto i1 do
                    inventory[i2+1] := inventory[i2];
                  inventory[i1]  := item_removed;
                  inven_ctr:= inven_ctr   + 1;
                  equip_ctr:= equip_ctr   - 1;
                  flag := true;
                END;
          until (flag);
          CASE typ of
            10,11,12,20,21,22,23,25 : prt1  := 'Was wielding ';
            15 : prt1  := 'Light source was ';
            otherwise                 prt1  := 'Was wearing ';
          END;
          objdes(prt2,i1,true,'i');
	  writev(out_val,prt1,prt2,' (',raoul_label(i1),')');
          msg_print(out_val);
	  if (item_val = -1) then
	    temporary_slot := blank_treasure
	  else 
	    equipment[item_val] := blank_treasure;	    
          if (item_val <> equip_max) then  { For secondary weapon}
            py_bonuses(inventory[i1],-1);
          remove := i1;
        END;
 
{ Unwear routine, remove a piece of equipment }
      procedure unwear;
        var
          i1,i2,com_val : integer;
          exit_flag,test_flag : boolean;
          command : char;
          out_val : vtype;
        BEGIN
          if (scr_state = 1) then
            BEGIN
              clear(1,1);
              show_equip(1);
            END;
          exit_flag := false;
          repeat
          writev(out_val,'(a-',chr(equip_ctr+96),
	  		 ', <space> for equipment list,',
			 ' <esc> to exit) ','Take off which one ?');
            test_flag := false;
            msg_print(out_val);
            repeat
              inkey(command);
              com_val := ord(command);
              CASE com_val of
                0,3,25,26,27 :  BEGIN
                                  test_flag := true;
                                  exit_flag := true;
                                END;
                32 :      BEGIN
                                  clear(2,1);
                                  show_equip(1);
                                END;
                otherwise       BEGIN
                                  com_val := com_val - 96;
                                  if ((com_val >= 1) and
                                      (com_val <= equip_ctr)) then
                                    test_flag := true;
                                END;
              END;
            until (test_flag);
            if (not(exit_flag)) then
              BEGIN
                reset_flag := false; { Player turn }
                i1 := 0;
                i2 := 22;
                repeat
                  i2 := i2 + 1;
                  if (equipment[i2].tval > 0) then
                    i1 := i1 + 1;
                until (i1 = com_val);
                if (uand(%X'80000000',equipment[i2].flags) <> 0) then
                  BEGIN
                    msg_print('Hmmm, it seems to be really stuck...');
                    com_val := 0;
                  END
                else
                  remove(i2);
              END;
            if (scr_state = 0) then
              exit_flag := true
            else if (equip_ctr = 0) then
              exit_flag := true
            else if (inven_ctr > 21) then
              exit_flag := true
            else if (not(exit_flag)) then
              show_equip(com_val);
          until(exit_flag);
          if (scr_state <> 0) then
            if (equip_ctr = 0) then
              clear(1,1)
            else
              prt('You are currently using -',1,1);
        END;
 
{ Wear routine, wear or wield an item }
      procedure wear;
        var
          com_val,i1,i2,i3,tmp : integer;
          out_val,prt1,prt2    : vtype;
          unwear_obj	       : treasure_type;
          exit_flag,test_flag  : boolean;
	  
      BEGIN
          exit_flag := false;
	  repeat
 	    test_flag := false;
            repeat
              writev(out_val,
	        '( <space> for next page, <esc> to exit ) ',
	        'Wear/Wield which item?');
              prt(out_val,1,1);
	      inkey(command);
	      com_val := ord(command);
	      case com_val of
         0,3,13,25,26,27  :  begin { Exit from module }
			       test_flag := true;
                               exit_flag := true;
		             end;

		       32 :  if (scr_state <> 1) then  {set scr_state to 1}
			       show_inven(cur_top,inven_ctr)
			     else
			       begin
			         if (cur_top+inven_per_page <= inven_ctr) then
 				   cur_top := cur_top + inven_per_page
			         else
			           cur_top := 1;
                                 show_inven(cur_top,inven_ctr);
			       end;

                 otherwise   begin
			       com_val := index(alpha_set,command);
			       if (com_val in [1..inven_ctr]) then
				   test_flag := true;
			     end;
	      end; {case}
            until (test_flag);
            if (not(exit_flag)) then { Main logic for wearing }
              BEGIN
                reset_flag := false; { Player turn }
                test_flag := true;
                CASE inventory[com_val].tval of { Slot for equipment }
                  15 : i1 := 33;
      19,20,21,23,25 : i1 := 23;
		  29 : i1 := 35;
                  30 : i1 := 31;
                  31 : i1 := 28;
                  32 : i1 := 32;
                  33 : i1 := 24;
                  34 : i1 := 27;
               35,36 : i1 := 26;
                  40 : i1 := 25;
                  45 : if (equipment[29].tval = 0) then   { Rings }
                            i1 := 29
                          else
                            i1 := 30;
                  otherwise
                          BEGIN
                            msg_print('Sorry, you can''t wear or wield that.');
			    msg_print('');
                            test_flag := false;
                            com_val := 0;
                          END;
                END;
                if (test_flag) then
                  if (equipment[i1].tval > 0) then
                    BEGIN
                      if (uand(%X'80000000',equipment[i1].flags) <> 0) then
                        BEGIN
                          objdes(out_val,i1,false,'e');
                          out_val := 'The ' + out_val + ' you are ';
                          CASE i1 of
                            23 :        out_val := out_val + 'wielding ';
                            otherwise   out_val := out_val + 'wearing ';
                          END;
                          msg_print(out_val + 'appears to be cursed.');
			  msg_print('');
                          test_flag := false;
                          com_val := 0;
                        END
                      else if (inven_ctr > inven_max-1) then
                        if (inventory[com_val].number > 1) then
                          if (inventory[com_val].subval < 512) then
                            BEGIN
                        msg_print('You will have to drop something first.');
                              test_flag := false;
                              com_val := 0;
                            END;
                        END;
                if (test_flag) then
                  BEGIN
                    unwear_obj := equipment[i1];
                    equipment[i1] := inventory[com_val];
                    with equipment[i1] do
		      Begin
{Fix for weight}        inven_weight := inven_weight + weight*number;
			if ((subval >= 255) and (subval < 512)) then
			  begin
			    number := 1;
{Fix for flashlights}  	    if (i1 = 33)  then subval := subval - 255;
                          end;
                      End;
                    inven_destroy(com_val);  { Subtracts weight }
		    if (scr_state <> 0) then
		      show_inven(cur_top,inven_ctr);
                    equip_ctr := equip_ctr + 1;
                    py_bonuses(equipment[i1],1);
                    if (unwear_obj.tval > 0) then
                      BEGIN
                        temporary_slot := unwear_obj;
                        tmp := remove(-1);
                        if (tmp < com_val) then
                          com_val := tmp;
		        if (scr_state <> 0) then
		          show_inven(cur_top,inven_ctr);
                      END;
                    CASE i1 of
                      23 : prt1 := 'You are wielding ';
                      33 : prt1 := 'Your light source is ';
                      otherwise prt1 := 'You are wearing ';
                    END;
                    objdes(prt2,i1,true,'e');
                    i2 := 0;
                    i3 := 22;
                    repeat   { Get the right letter of equipment }
                      i3 := i3 + 1;
                      if (equipment[i3].tval > 0) then
                        i2 := i2 + 1;
                    until (i3 = i1);
                    out_val := prt1 + prt2 + ' (' + chr(i2+96)+cur_char2(i1);
                    msg_print(out_val);
		    msg_print('');
                  END;
              END;
            if (scr_state = 0) then
              exit_flag := true
            else if (inven_ctr = 0) then
              exit_flag := true;
          until(exit_flag);
          if (scr_state <> 0) then prt('You are currently carrying -',1,1);
        END;
 
{ Switch primary and secondary weapons }
      procedure switch_weapon;
        var
	  prt1,prt2  : vtype;
          tmp_obj : treasure_type;
        BEGIN
          if (uand(%X'80000000',equipment[23].flags) <> 0) then
            BEGIN
              objdes(prt1,23,false,'e');
              msg_print('The ' + prt1 +
                        ' you are wielding is really stuck to you.');
	      msg_print('');
            END
          else
            BEGIN { Switch weapons }
              reset_flag := false;
              tmp_obj := equipment[34];
              equipment[34] := equipment[23];
              equipment[23] := tmp_obj;
              py_bonuses(equipment[34],-1);  { Subtract bonuses }
              py_bonuses(equipment[23],1);   { Add bonuses }
              if (equipment[23].tval > 0) then
                BEGIN
                  prt1 := 'Primary weapon: ';
                  objdes(prt2,23,true,'e');
                  msg_print(prt1 + prt2);
		  msg_print('');
                END;
              if (equipment[34].tval > 0) then
                BEGIN
                  prt1 := 'Secondary weapon : ';
                  objdes(prt2,34,true,'e');
                  msg_print(prt1 + prt2);
		  msg_print('');
                END;
            END;
          if (scr_state <> 0) then
            BEGIN
              msg_print('');
              clear(1,1);
              prt('You are currently using -',1,1);
              show_equip(1);
            END;
        END;
 
{ Main logic for INVEN_COMMAND }
      BEGIN
        inven_command := false;
        exit_flag := false;
        scr_state := 0;
	cur_top := 1;
        repeat
          CASE command of
            'i' : BEGIN   { Inventory }
                    if (inven_ctr = 0) then
                      msg_print('You are not carrying anything.')
                    else if (scr_state <> 1) then
                      BEGIN  { Sets scr_state to 1 }
		        erase_line(1,1);
                        prt('You are currently carrying -',1,1);
                        show_inven(cur_top,inven_ctr);
                      END;
                  END;
	    ' ' : BEGIN
		    if (cur_top+inven_per_page <= inven_ctr) then
		        cur_top := cur_top + inven_per_page
		    else
		        cur_top := 1;
		    show_inven(cur_top,inven_ctr);
		  END;
	    'c' : BEGIN { Clean }
		    if (inven_ctr = 0) then
		       msg_print('You are not carrying anything.')
		    else clean;
		  END;
            'e' : BEGIN   { Equipment }
                    if (equip_ctr = 0) then
                      msg_print('You are not using any equipment.')
                    else if (scr_state <> 2) then
                      BEGIN  { Sets scr_state to 2 }
                        clear(1,1);
                        prt('You are currently using -',1,1);
                        show_equip(1);
                      END;
                  END;
            't' : BEGIN   { Take off }
                    if (equip_ctr = 0) then
                      msg_print('You are not using any equipment.')
                    else if (inven_ctr > inven_max-1) then
                      msg_print('You will have to drop something first.')
                    else
                      unwear;   { May set scr_state to 2 }
                  END;
            'w' : BEGIN   { Wear/wield }
                    if (inven_ctr = 0) then
                      msg_print('You are not carrying anything.')
                    else
                      wear;  { May set scr_state to 1 }
                  END;
            'x' : BEGIN
                    if (equipment[23].tval <> 0) then
                      switch_weapon
                    else if (equipment[34].tval <> 0) then
                      switch_weapon
                    else
                      msg_print('But you are wielding no weapons.');
                  END;
{ Special function for other routines }
            '?' : BEGIN { Displays part inven, returns}
                    show_inven(r1,r2);
                    scr_state := 0;  { Clear screen state }
                  END;
{ Nonsense command }
            otherwise ;
          END;
          if (scr_state > 0) then
            BEGIN
	      prt('<e>quip, <i>inven, <t>ake-off, <w>ear/wield, ' +
		 'e<x>change, <c>lean, ^Z to exit.',24,2);
              test_flag := false;
	      if (scr_state = 1) then 
	        count_inventory;
              repeat
                inkey(command);
                com_val := ord(command);
                CASE com_val of
                  0,3,25,26,27  : BEGIN { Exit from module }
                                    exit_flag := true;
                                    test_flag := true;
                                  END;
	          otherwise
                   CASE command of  { Module commands }
		     ' ' : test_flag := true;
                     'c' : test_flag := true;
		     'e' : test_flag := true;
                     'i' : test_flag := true;
                     't' : test_flag := true;
                     'w' : test_flag := true;
                     'x' : test_flag := true;
                     '?' : ;  { Trap special feature}
                   otherwise ; { Nonsense command }
                   END;
                END;
              until (test_flag);
              prt('',24,1);
            END
          else
            exit_flag := true;
        until(exit_flag);
        if (scr_state > 0) then   { If true, must redraw screen }
          inven_command := true;
      END;
 
 { Get the ID of an item and return the CTR value of it }
    function get_item(  var com_val  : integer;
                        pmt : vtype;
                        var redraw: boolean;
                        r1,r2 : integer) : boolean;
      var
          command	      : char;
      	  cur_top	      : integer;
          out_val	      : vtype;
          exit_flag	      : boolean;
	  	  
      BEGIN
        cur_top := r1;
	get_item := false;
	redraw := false;
	exit_flag := false;
        if (inven_ctr > 0) then
          BEGIN
            writev(out_val,' (',raoul_label(r1),'-',raoul_label(r2),
	    	') <space> for list, <esc> to exit.  ',pmt);
            prt(out_val,1,1);
            repeat
	      inkey(command);
	      com_val := ord(command);
	      case com_val of
         0,3,13,25,26,27  :  begin { Exit from module }
                               exit_flag := true;
			       reset_flag := true;
                             end;

                       32 :  if (not redraw) then
			       begin
			         inven_command('?',cur_top,r2);
			         redraw := true;
			       end
			     else
			       begin
			         if (cur_top+inven_per_page <= r2) then
 				   cur_top := cur_top + inven_per_page
			         else
				   cur_top := r1;
                                 inven_command('?',cur_top,r2);
                               end;

                 otherwise   begin
			       com_val := index(alpha_set,command);
			       if ((com_val >= r1) and (com_val <= r2)) then
                                 begin
                                   exit_flag := true;
                                   get_item := true;
                                 end;
			     end;
	      end;
            until (exit_flag);
            erase_line(msg_line,msg_line);
          END
        else
          msg_print('You are not carrying anything.');
      END;
