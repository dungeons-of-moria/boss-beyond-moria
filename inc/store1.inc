{ Returns the value for any given object }
[psect(store$code)] function item_value(item : treasure_type) : integer;
 
      function search_list(x1,x2 : integer) : integer;
        var
          i1,i2         : integer;
        BEGIN
          i1 := 0;
          i2 := 0;
          repeat
            i1 := i1 + 1;
            with object_list[i1] do
              if ((tval = x1) and (subval = x2)) then
                i2 := cost;
          until ((i1 = max_objects) or (i2 > 0));
          search_list := i2
        END;
 
      BEGIN
        with item do
          BEGIN
            item_value := cost;
            if (tval in [19,20,21,23,30,31,32,33,34,35,36]) then
              BEGIN     { Weapons and armor }
                if (index(name,'^') > 0) then
                  item_value := search_list(tval,subval)*number
                else if (tval in [20,21,22,23]) then
                  BEGIN
                    if (tval in [21,22,23]) then
                      item_value := (cost+(tohit+todam+toac)*100)*number
		    else 
		     {armors}
                      item_value := (cost+(tohit+toac)*100)*number
                  END
                else
                  item_value := (cost+toac*100)*number
              END
            else if (tval in [9,10,11,12,13,82,84]) then
              BEGIN     { Ammo, Spikes, Thrown and Grenades }
                if (index(name,'^') > 0) then
                  item_value := search_list(tval,1)*number
                else
                  item_value := (cost+(tohit+todam+toac)*10)*number
              END
            else if (tval in [70,71,75,76,80]) then
              BEGIN     { Potions, Scrolls, and Food }
                if (index(name,'|') > 0) then
                  case tval of
                    70,71 : item_value :=  20;
                    75,76 : item_value :=  20;
                    80    : item_value :=   1;
                    otherwise
                  END
              END
            else if (tval in [40,45]) then
              BEGIN     { Rings and amulets }
                if (index(name,'|') > 0) then
                  case tval of
                    40 : item_value := 45;
                    45 : item_value := 45;
                    otherwise
                  END
                else if (index(name,'^') > 0) then
                  item_value := abs(cost);
              END
            else if (tval in [55,60,65]) then
              BEGIN     { Wands rods, and staves}
                if (index(name,'|') > 0) then
                  case tval of
                    55 : item_value := 70;
                    60 : item_value := 60;
                    65 : item_value := 50;
                    otherwise ;
                  END
                else if (index(name,'^') = 0) then
                  BEGIN
                    item_value := cost + trunc(cost/20.0)*p1
                  END
              END
          END
      END;
 
 
{ Asking price for an item } 
[psect(store$code)] function sell_price (snum : integer;
                                         item : treasure_type ) : integer;
      var
        i1 : integer;
BEGIN
        with store[snum] do                                 
          BEGIN
            i1 := item_value(item);
            if (item.cost > 0) then
	      with owners[snum,town_num] do
                begin    
                  i1 := i1 + trunc(i1*rgold_adj[owner_class,py.misc.pclass]);
                  i1 := i1 + trunc(i1*(buy_inflate));
                  if (i1 < 1) then i1 := 1;
                    sell_price := i1
                end            
            else
                sell_price := 0
          END;
END;
 
 
{ Check to see if he will be carrying too many objects }
[psect(store$code)] function store_check_num(store_num : integer) : boolean;
      var    
        item_num,i1 : integer;
        flag : boolean;
      BEGIN
        store_check_num := false;
        with store[store_num] do
          if (store_ctr < store_inven_max) then
            store_check_num := true
          else
            if ((temporary_slot.subval > 255) and
                (temporary_slot.subval < 512)) then
              for i1 := 1 to store_ctr do
                with store_inven[i1].sitem do
                  if (tval = temporary_slot.tval) then
                    if (subval = temporary_slot.subval) then
                      store_check_num := true
      END;
                             
                     
{ Add the item in TEMPORARY_SLOT to store's inventory. }
 [psect(store$code)] procedure store_carry( store_num : integer;
                                            var ipos  : integer);
      var
        item_num,item_val,typ,subt,icost,dummy : integer;
        flag : boolean;
                                        
{ Insert TEMPORARY_SLOT at given location }
      procedure insert(store_num,pos,icost : integer);
        var
                i1 : integer;
        BEGIN
          with store[store_num] do
            BEGIN
              for i1 :=  store_ctr downto pos do
                store_inven[i1+1] := store_inven[i1];
              store_inven[pos].sitem := temporary_slot;
              store_inven[pos].scost := -icost;
              store_ctr := store_ctr + 1
            END
        END;
                                                    
{ Store_carry routine }
      BEGIN
        ipos := 0;
        identify(temporary_slot);
	unquote(temporary_slot.name);
	known1(temporary_slot.name);
        known2(temporary_slot.name);
        icost := sell_price(store_num,temporary_slot);
        if (icost > 0) then
          BEGIN
            with temporary_slot do
              with store[store_num] do
                BEGIN
                  item_val := 0;
                  item_num := number;
                  flag := false;
                  typ  := tval;
                  subt := subval;
                  repeat
                    item_val := item_val + 1;
                    with store_inven[item_val].sitem do
                      if (typ = tval) then
                        BEGIN
                          if (subt = subval) then{ Adds to other item   }
                            if (subt > 255) then
                              BEGIN
                                if (number < 24) then
                                  number := number + item_num;
                                flag := true
                              END
                        END
                      else if (typ > tval) then
                        BEGIN           { Insert into list }
                          insert(store_num,item_val,icost);
                          flag := true;
                          ipos := item_val
                        END;
                  until ((item_val >= store_ctr) or (flag));
                  if (not(flag)) then   { Becomes last item in list }
                    BEGIN
                      insert(store_num,store_ctr+1,icost);
                      ipos := store_ctr
                    END
                END
          END
      END;                    
 
 
 
{ Destroy an item in the store's inventory.  Note that if 'one_of' is false,
  an entire slot is destroyed }
[psect(store$code)] procedure store_destroy(store_num,item_val : integer;
                        one_of : boolean);
      var
                i2 : integer;
      BEGIN  
        with store[store_num] do
          BEGIN
            temporary_slot := store_inven[item_val].sitem;
            with store_inven[item_val].sitem do
              BEGIN
                if ((number > 1) and (subval < 512) and (one_of))  then
                  BEGIN
                    number := number - 1;
                    temporary_slot.number := 1
                  END
                else
                  BEGIN
                    for i2 := item_val to store_ctr-1 do
                      store_inven[i2] := store_inven[i2+1];
                    store_inven[store_ctr].sitem := blank_treasure;
                    store_inven[store_ctr].scost := 0;
                    store_ctr := store_ctr - 1
                  END
              END
          END
      END;
 
 
                 
{ Creates an item and inserts it into store's inven }
[psect(store$code)] procedure store_create(store_num : integer);
      var
        i1,tries,dummy : integer;
      BEGIN                   
        tries := 0;
        with store[store_num] do
          repeat              
            i1 := store_choice[store_num,randint(store$choices)];
            temporary_slot := inventory_init[i1];
            magic_treasure(temporary_slot,obj_town_level);
            if (store_check_num(store_num)) then
              with temporary_slot do
                if (cost > 0) then      { Item must be good }
                    BEGIN     
                      store_carry(store_num,dummy);
                      tries := 10
                    END;                              
            tries := tries + 1;
          until(tries > 3);
      END;                    

 {Add Black Market items to the inventory}
[psect(store$code)] procedure do_black_market;
var
   indx,iptr,inven_num : integer;

BEGIN
   inven_num := 0;
   store[7].store_ctr := 0;
   while (inven_num < store_inven_max) do
     inven_num := 5 + randint(store_inven_max);
   if (inven_num > store_inven_max) then inven_num := store_inven_max;
   for indx := 1 to inven_num do
      with store[7].store_inven[indx] do
        BEGIN
	  repeat
            iptr := randint(inven_init_max);
	    temporary_slot := inventory_init[iptr];
	    magic_treasure(temporary_slot,999); {special black magic}
	  until(temporary_slot.cost > 500);
          store_carry(7,iptr);
        END;
   for indx := inven_num to store_inven_max do
      with store[7].store_inven[indx] do
        sitem := blank_treasure;
   store[7].store_ctr := store[7].store_ctr - 1;
END;
 
                    
{ Rotate stock, create sales, and up-keep the store's inventory. }
{ Remember Doofus, that this procedure only works when there's a }
{ dungeon floor in existance.}
[psect(store$code)] procedure store_maint;
      var
        i1,i2,i3,i4	: integer;

    BEGIN
      do_black_market;
      for i1 := 1 to max_stores do
        if (i1 <> 7) then	 
          with store[i1] do
            BEGIN
{sale}	      for i3 := 1 to store_ctr do
	        begin
  	          if (randint(10) = 1) then 
		    begin
		      with store_inven[i3] do
		        i4 := abs(trunc(store_inven[i3].scost*sale_val));
		        if (i4 < 1) then i4 := 1;
		        store_inven[i3].scost :=  i4; 
		        store_inven[i3].sitem.cost := i4;
		    end;
		end;
{Maint.}      insult_cur := 0;
              if (store_ctr > store$max_inven) then
                for i2 := 1 to (store_ctr-store$max_inven+2) do
                  store_destroy(i1,randint(store_ctr),false)
              else
                if (store_ctr < store$min_inven) then
                  BEGIN
                    for i2 := 1 to (store$min_inven-store_ctr+2) do
                      store_create(i1)
                  END
                else
                  BEGIN
                    for i2 := 1 to (1+randint(store$turn_around)) do
                      store_destroy(i1,randint(store_ctr),true);
                    for i2 := 1 to (1+randint(store$turn_around)) do
                      store_create(i1)
                  END
            END
    END;
                    
                              
{ Initializes the town stores with inventory }
[psect(store$code)] procedure store_init;
      var
        i2,i3 : integer;
      BEGIN
        for i2 := 1 to max_stores do
          with store[i2] do
            Begin  {owner is now a dummy value}
              owner := 0; insult_cur := 0; store_open := 0; store_ctr  := 0;
              for i3 := 1 to store_inven_max do
                begin
                  store_inven[i3].sitem := blank_treasure;
                  store_inven[i3].scost := 0
                end;
            End;
      END;            



