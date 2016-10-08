{ Object descriptor routines 
  Randomize colors, woods, and metals }
[psect(setup$code)] procedure randes;
    var
        i1,i2 : integer;
        tmp : vtype;
    BEGIN
      for i1 := 1 to max_colors do
        BEGIN
          i2 := randint(max_colors);
          tmp := colors[i1];
          colors[i1] := colors[i2];
          colors[i2] := tmp;
        END;
      for i1 := 1 to max_woods do
        BEGIN
          i2 := randint(max_woods);
          tmp := woods[i1];
          woods[i1] := woods[i2];
          woods[i2] := tmp;
        END;
      for i1 := 1 to max_metals do
        BEGIN
          i2 := randint(max_metals);
          tmp := metals[i1];
          metals[i1] := metals[i2];
          metals[i2] := tmp;
        END;
      for i1 := 1 to max_rocks do
        BEGIN
          i2 := randint(max_rocks);
          tmp := rocks[i1];
          rocks[i1] := rocks[i2];
          rocks[i2] := tmp;
        END;
      for i1 := 1 to max_amulets do
        BEGIN
          i2 := randint(max_amulets);
          tmp := amulets[i1];
          amulets[i1] := amulets[i2];
          amulets[i2] := tmp;
        END;
      for i1 := 1 to max_mush do
        BEGIN
          i2 := randint(max_mush);
          tmp := mushrooms[i1];
          mushrooms[i1] := mushrooms[i2];
          mushrooms[i2] := tmp;
        END;
    END;
 
 
{ Return random title }
[psect(setup$code)] procedure rantitle  ( var title : varying[a] of char );

    var
     	i3   :  integer;

    BEGIN 
      i3  := (max_syllables div 4);
      title := 'Labeled "';
      title := title + syllables[ (i3*0) + randint(i3)];
      title := title + ' to ';
      title := title + syllables[ (i3*1) + randint(i3)];
      title := title + ' ';
      title := title + syllables[ (i3*2) + randint(i3)];
      title := title + ' ';
      title := title + syllables[ (i3*3) + randint(i3)];
      title := title + '"';
    END; 


{ Initialize all Potions, wands, staves, scrolls, ect..}
[psect(setup$code)] procedure magic_init(random_seed : unsigned);
    var
        i1,tmpv         : integer;
        tmps            : vtype;
    BEGIN
      seed := random_seed;
      randes;
      for i1 := 1 to max_objects do
        BEGIN
          tmpv := int(uand(%X'FF',object_list[i1].subval));
          case object_list[i1].tval of
           3,75,76 : if (tmpv <= max_colors) then
                       insert_str(object_list[i1].name,'%C',colors[tmpv]);
             70,71 : BEGIN
                       rantitle(tmps);
                       insert_str(object_list[i1].name,'%T',tmps);
                     END;
                45 : if (tmpv <= max_colors) then
                       insert_str(object_list[i1].name,'%R',rocks[tmpv]);
                40 : if (tmpv <= max_rocks) then
                       insert_str(object_list[i1].name,'%A',amulets[tmpv]);
                65 : if (tmpv <= max_amulets) then
                       insert_str(object_list[i1].name,'%M',metals[tmpv]);
                55 : if (tmpv <= max_woods) then
                       insert_str(object_list[i1].name,'%W',woods[tmpv]);
                80 : if (tmpv <= max_mush) then
                       insert_str(object_list[i1].name,'%M',mushrooms[tmpv]);
                60 : {if (tmpv <= max_rods) then
                       insert_str(object_list[i1].name,'%D',rods[tmpv])};
                otherwise ;
          END
        END
    END;
 
 
{ Remove 'Secret' symbol for identity of object }
[psect(misc1$code)] procedure known1 ( var object_str  : varying[a] of char );
    var
        pos,olen : integer;
         str1,str2 : vtype;
    BEGIN
      pos := index(object_str,'|');
      if (pos > 0) then
        BEGIN
          olen := length(object_str);
          str1 := substr(object_str,1,pos-1);
          str2 := substr(object_str,pos+1,olen-pos);
          writev(object_str,str1,str2);
        END;
    END;
 
 
{ Remove 'Secret' symbol for identity of pluses }
[psect(misc1$code)] procedure known2 (var object_str : varying[a] of char );
    var
        pos,olen : integer;
        str1,str2 : vtype;
    BEGIN
       pos := index(object_str,'^');
      if (pos > 0) then
        BEGIN
          olen := length(object_str);
          str1 := substr(object_str,1,pos-1);
          str2 := substr(object_str,pos+1,olen-pos);
          writev(object_str,str1,str2);
        END;
    END;
 
 
{ Return string without quoted portion }
[psect(misc1$code)] procedure unquote (var object_str : varying[a] of char );
    var
        pos0,pos1,pos2,olen : integer;
        str1,str2 : vtype;
    BEGIN
      pos0 := index(object_str,'"');
      if (pos0 > 0) then
        BEGIN
          pos1 := index(object_str,'~');
          pos2 := index(object_str,'|');
          olen := length(object_str);
          str1 := substr(object_str,1,pos1);
          str2 := substr(object_str,pos2+1,olen-pos2);
          writev(object_str,str1,str2);
        END
    END;
 
 
 
{ Somethings been identified }
[psect(misc1$code)] procedure identify(item : treasure_type);
    var
        i1,x1,x2 : integer;
    BEGIN
      x1 := item.tval;
      x2 := item.subval;
      if (index(item.name,'|') > 0) then
        BEGIN
          for i1 := 1 to max_talloc do
            with t_list[i1] do
              if ((tval = x1) and (subval = x2)) then
                BEGIN
                  unquote(name);
                  known1(name);
                END;
          for i1 := 1 to inven_max do
            with inventory[i1] do
              if ((tval = x1) and (subval = x2)) then
                BEGIN
                  unquote(name);
                  known1(name);
                END;
          i1 := 0;
          repeat              
            i1 := i1 + 1;
            with object_list[i1] do
              if ((tval = x1) and (subval = x2)) then
                if (index(name,'%T') > 0) then
                  BEGIN
                    insert_str(name,' %T|','');
                    object_ident[i1] := true;
                  END
                else
                  BEGIN
                    unquote(name);
                    known1(name);
                    object_ident[i1] := true;
                  END;
          until (i1 = max_objects);
        END;
    END;
 
 
{ Returns a description of item for inventory }
[psect(misc1$code)] procedure objdes(var out_val : varying[a] of char;
				         ptr	 : integer;
					 pref	 : boolean;
					 which_list : char);
    var
        pos 		: integer;
        tmp_val		: vtype;
	obj_describe    : treasure_type;
	
    BEGIN
      if (which_list = 'e') then 
        obj_describe := equipment[ptr]      
      else if (which_list = 't') then
        obj_describe := temporary_slot
      else
        obj_describe := inventory[ptr];

      with obj_describe do
        BEGIN
          tmp_val := name;
          pos := index(tmp_val,'|');
          if (pos > 0) then
            tmp_val := substr(tmp_val,1,pos-1);
          pos := index(tmp_val,'^');
          if (pos > 0) then
            tmp_val := substr(tmp_val,1,pos-1);
          if (not(pref)) then
            BEGIN
              pos := index(tmp_val,' (');
              if (pos > 0) then
                tmp_val := substr(tmp_val,1,pos-1);
            END;
          insert_num(tmp_val,'%P1',p1,true);
          insert_num(tmp_val,'%P2',tohit,true);
          insert_num(tmp_val,'%P3',todam,true);
          insert_num(tmp_val,'%P4',toac,true);
          insert_num(tmp_val,'%P5',p1,false);
          insert_num(tmp_val,'%P6',ac,false);
          if (number <> 1) then
            BEGIN
              insert_str(tmp_val,'ch~','ches');
	      insert_str(tmp_val,'fe~','ves');
              insert_str(tmp_val,'~','s');
            END
          else
            insert_str(tmp_val,'~','');
          if (pref) then
            BEGIN
              if (index(tmp_val,'&') > 0) then
                BEGIN
                  insert_str(tmp_val,'&','');
                  if (number > 1) then
                    writev(out_val,number:1,tmp_val)
                  else if (number < 1) then
                    writev(out_val,'no more',tmp_val)
                  else if (tmp_val[2] in vowel_set) then
                    writev(out_val,'an',tmp_val)
                  else
                    writev(out_val,'a',tmp_val);
                END
              else            
                out_val := tmp_val;
              out_val := out_val + '.';
            END
          else
            BEGIN
              insert_str(tmp_val,'& ','');
              out_val := tmp_val
            END;
        END
    END;
