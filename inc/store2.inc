{ Comment one : Item is sold.}
[psect(store$code)] procedure prt_comment1;
      begin
        msg_flag := false;
        case randint(12) of
          1  : msg_print('Done!');
          2  : msg_print('Accepted!');
          3  : msg_print('Fine...');
          4  : msg_print('Agreed!');
          5  : msg_print('Ok...');
          6  : msg_print('Sold!');
          7  : msg_print('You''ll force me to bankrupt...');
          8  : msg_print('Sigh...');
          9  : msg_print('My poor sick children may starve...');
         10  : msg_print('Robbed again...');
         11  : msg_print('A pleasure to do business with you!');
         12  : msg_print('My spouse shall skin me...');
        end;
      end; 

{ Comment two : Item is sold with a coupon - RLG.}
[psect(store$code)] procedure prt_comment2(cash : integer);
      VAR
          str	: vtype;      		    

      BEGIN
        msg_flag := false;
        case randint(5) of
          1  : writev(str,'Done!  For the low, low price of $',cash:1);
          2  : writev(str,'Sold!  For the extra cheap price of $',cash:1);
          3  : writev(str,'Congrats. El Cheapo price, $',cash:1);
          4  : writev(str,'Agreed!  LOW PRICE: $',cash:1);
          5  : writev(str,'Ok...  You got away with it for only $',cash:1);
        end;
	msg_print(str);
      END;

{ Displays the set of commands }
[psect(store$code)] procedure display_commands(store_num:integer);
      begin
prt('You may:',20,3);
prt(' p) Purchase an Item.        space) Browse store''s inventory.',21,3);
prt(' s) Sell an Item                 i) Inventory and Equipment Lists.',22,3);
prt(' u) Use a Coupon.               ^R) Redraw the screen.',23,3);
prt('^Z) Exit from Building.         ^T) Seek Special Training.',24,3);
      end;
 
 
{ Displays a store's inventory }
[psect(store$code)] procedure display_inventory(store_num,start : integer);
      var
        i1,i2,stop,dum1,dum2            : integer;
        out_val1,out_val2               : vtype;
      begin
        with store[store_num] do
            begin
              i1 := ((start-1) mod 12);
              stop := (((start-1) div 12) + 1)*12;
              if (stop > store_ctr) then stop := store_ctr;
              while (start <= stop) do
                begin
                  temporary_slot := store_inven[start].sitem;
                  with temporary_slot do
                    if ((subval > 255) and (subval < 512)) then
                      number := 1;
                  objdes(out_val1,1,true,'t');
                  writev(out_val2,chr(97+i1),') ',out_val1);
                   prt(out_val2,i1+6,1);
                   if (store_inven[start].scost < 0) then
                    begin
                      i2 := abs(store_inven[start].scost);
                      i2 := i2 + trunc(i2*chr_padj);
                      writev(out_val2,i2:7);
                    end
                  else
                    writev(out_val2,store_inven[start].scost:7,' [Sale]');
                  prt(out_val2,i1+6,60);
                  i1 := i1 + 1;
                  start := start + 1;
                end;
              if (i1 < 12) then
                for i2 := 1 to (12 - i1 + 1) do
                  prt('',i2+i1+5,1);
            end;
      end;
 
 
        { Re-displays only a single cost                        -RAK-   }
[psect(store$code)] procedure display_cost(store_num,pos : integer);
      var
        i1                              : integer;
        out_val                         : vtype;
      begin
        with store[store_num] do
          begin
            i1 := ((pos-1) mod 12);
            if (store_inven[pos].scost < 0) then
              begin
                i2 := abs(store_inven[pos].scost);
                i2 := i2 + trunc(i2*chr_padj);
                writev(out_val,i2:7);
              end
            else
              writev(out_val,store_inven[pos].scost:7,' [Sale]');
            prt(out_val,i1+6,60);
          end;
      end;
 
 
        { Displays players gold                                 -RAK-   }
[psect(store$code)] procedure store_prt_gold;
      var
        out_val                 : vtype;
      begin
        writev(out_val,'Dollars Remaining : ',py.misc.au:1);
        prt(out_val,19,20);
      end;
 
 
        { Displays store                                        -RAK-   }
[psect(store$code)] procedure display_store(store_num,cur_top : integer);
      begin
        with store[store_num] do
          begin
            clear(1,1);
            prt(owners[store_num,town_num].owner_name,3,10);
            prt('   Item',5,1);
            prt('Today''s Price',5,60);
            store_prt_gold;
            display_commands(store_num);
            display_inventory(store_num,cur_top);
          end;
      end;
 
 
        { Get the ID of a store item and return it's value      -RAK-   }
[psect(store$code)] function get_store_item(
                                var com_val     : integer;
                                pmt             : vtype;
                                i1,i2           : integer) : boolean;
      var
                command         : char;
                out_val         : vtype;
                flag            : boolean;
      begin
        com_val := 0;
        flag := true;
        writev(out_val,'(Items ',chr(i1+96),'-',chr(i2+96),
                                        ', ^Z to exit) ',pmt);
        while (((com_val < i1) or (com_val > i2)) and (flag)) do
          begin
            prt(out_val,1,1);
            inkey(command);
            com_val := ord(command);
            case com_val of
                3,25,26,27 :    flag := false;
                otherwise com_val := com_val - 96;
            end;
          end;
        msg_flag := false;
        erase_line(msg_line,msg_line);
        get_store_item := flag;
      end;
 
        { Returns the char inven position of a coupon} 
[psect(store$code)] function get_coupon(store_num    : integer;
					var coup_val : integer;
					cur_top	     : integer) : boolean;
   var
   	i2,i3	    :  integer;
	redraw	    :  boolean;
 
BEGIN
  redraw := false;
  get_coupon := false;
  if (not(find_range([3],i2,i3))) then
    msg_print('You don''t have any coupons.')
  else 
    if (get_item(coup_val,'Use which coupon?',redraw,i2,i3)) then
      begin
        if (redraw) then display_store(store_num,cur_top);
	with inventory[coup_val] do
	  if ((subval = store_num) or (subval = 20)) then 
	    get_coupon := true
	  else
	    msg_print('I cannot accept that!');
      end;
END;
 
 
{ Returns whether a sale has been made and the selling price}
{0- sale ,  2- no sale ,  3- player tryed to sell valueless item}
[psect(store$code)] function ask_price(
                                store_num : integer;
     			        var price : integer;
                                item : treasure_type) : integer;
 var
        cost 	   	: integer;
	accept		: char; 
 
 BEGIN
      ask_price := 0;
      price := 0;
      msg_flag := false;
      with store[store_num] do
        Begin
          cost := item_value(item);
          If (cost < 1) then
              ask_price := 3
          Else
            with owners[store_num,town_num] do
              begin
	        cost  := cost - trunc(cost*chr_padj) -
                         trunc(cost*rgold_adj[owner_class,py.misc.pclass]);
	        cost  := trunc(cost*(1-sell_deflate));
                if (cost < 1) then cost := 1;
		price := cost;
		prt_num('Hmmmm, that''s worth: ',price,2,2);
		msg_print('');
		if (get_com('Accept the Offer? (y/n)',accept)) then
                  if (accept = 'y') then  
                    ask_price := 0
		  else
                    ask_price := 2
	        else ask_price := 2;
                prt('',2,1);
              end;
        End;
 END;
 
 
        { Buy an item from a store                              -RAK-   }
[psect(store$code)] function store_purchase(
			     	       store_num     : integer;
				       coupon	     : boolean;
				       coup_val	     : integer;
			     	       var cur_top   : integer) : boolean;
      var
        i1,item_val,price	                : integer;
        item_new,choice                         : integer;
        save_number                             : integer;
	per_off					: real;
        out_val                                 : vtype;

BEGIN
  store_purchase := false;
  with store[store_num] do
    Begin
       { i1 = number of objects shown on screen }
    if (cur_top = 13) then
      i1 := store_ctr - 12
    else if (store_ctr > 12) then
      i1 := 12
    else
      i1 := store_ctr;
    If (store_ctr < 1) then
      msg_print('I am currently out of stock.')
      {get coupon value}
    Else if (get_store_item(item_val,		{Get the store item number}
                'Which item are you interested in? ',1,i1)) then

              begin
                item_val := item_val + cur_top - 1;     { true item_val }
                temporary_slot := store_inven[item_val].sitem;
		price := abs(store_inven[item_val].scost);
		price := price + trunc(price*chr_padj);
		if (coupon) then 
		  begin
		    per_off := (100 - inventory[coup_val].p1)/100;
		    price := trunc(price*per_off);
		    inven_destroy(coup_val);		    
		  end;
		if (price < 1) then price := 1;
                with temporary_slot do
                  if ((subval > 255) and (subval < 512)) then
                    begin
                      save_number := number;
                      number := 1;
                    end
                  else
                    save_number := 1;
                if (inven_check_weight) then
                  if (inven_check_num) then
                    begin
		      if (wizard) then
		        py.misc.au := py.misc.au + price;
                      if (py.misc.au < price) then
                        msg_print('You Scum!  Cough up more dough!')
                      else
                          Begin
			    if (coupon) then 
                              prt_comment2(price)
			    else
                              prt_comment1;
                            py.misc.au := py.misc.au - price;
                            store_destroy(store_num,item_val,true);
                            inven_carry(item_new);
                            objdes(out_val,item_new,true,'i');
           out_val := 'You have ' + out_val + ' (' + chr(item_new+96) + ')';
                            msg_print(out_val);
                            if (cur_top > store_ctr) then
                              begin
                                cur_top := 1;
                                display_inventory(store_num,cur_top);
                              end
                            else
                              with store_inven[item_val] do
			        begin
			         display_inventory(store_num,item_val);
                                 store_prt_gold;
                                end;
                         End;
                      prt('',2,1);
                    end
                  else
                    prt('You cannot carry that many different items.',1,1)
                else
                  prt('You can not carry that much weight.',1,1);
              end;
          end;
  END;
 
 
{ Sell an item to the store }
[psect(store$code)] function store_sell(store_num,cur_top : integer)
						: boolean;
      var
        item_val,i1,
        item_pos,price : integer;
        redraw         : boolean;
        out_val        : vtype;
      begin
        store_sell := false;
        with store[store_num] do
          begin
            redraw := false;
         if (get_item(item_val,'Sell which item?',redraw,1,inven_ctr)) then
              begin
                if (redraw) then display_store(store_num,cur_top);
                temporary_slot := inventory[item_val];
                with temporary_slot do
                  if ((subval > 255) and (subval < 512)) then
                    number := 1;
                objdes(out_val,1,true,'t');
                out_val := 'Selling ' +out_val+ ' (' + chr(item_val+96) + ')';
                msg_print(out_val);
                msg_print(' ');
                if (temporary_slot.tval in store_buy[store_num]) then
                  if (store_check_num(store_num)) then
                    case ask_price(store_num,price,temporary_slot) of
                      0 : begin
                            prt_comment1;
                            py.misc.au := py.misc.au + price;
                            inven_destroy(item_val);
                            store_carry(store_num,item_pos);
                            if (item_pos > 0) then
                              if (item_pos < 13) then
                                if (cur_top < 13) then
                                  display_inventory(store_num,item_pos)
                                else
                                  display_inventory(store_num,cur_top)
                              else if (cur_top > 12) then
                                display_inventory(store_num,item_pos);
                            store_prt_gold;
                          end;
                      2 : store_sell := true;
                      3 : begin
                            msg_print('How dare you!');
                            msg_print('I will not buy that!');
                          end;
                      otherwise ;
                    end
                  else
                    prt('I have not the room in my store to keep it...',1,1)
                else
                  prt('I do not buy such items.',1,1);
              end
            else if (redraw) then
              display_store(store_num,cur_top);
          end;
      end;
 
 
{ Entering a store }
[psect(store$code)] procedure enter_store(store_num : integer);
      var
        com_val,cur_top,coup_val,dummy  : integer;
        command                         : char;
        exit_flag,coupon		: boolean; 
 
    BEGIN
        dummy  := 1;
        coupon := false;
        with store[store_num] do
          if (store_num = 7) and (py.misc.au < 500) then
            msg_print('The bouncer says that you''re too poor to enter.')

          else if (((store_open > day) or (turn > 12960) or (turn < 4320)) 
			       and (store_num <> 7)) then
	    msg_print('The doors are locked.')

	  else
            Begin
              exit_flag := false;
              cur_top := 1;
              display_store(store_num,cur_top);
              repeat
                if (get_com('',command)) then
                  begin
                    msg_flag := false;
                    com_val := ord(command);
                    case com_val of
                      18      : display_store(store_num,cur_top);
		      20      : begin  {specific training}
				  training(store_num);
				  exit_flag := true;
				end;
                      32      : begin  {browse inventory}
                                  if (cur_top = 1) then
                                    if (store_ctr > 12) then
                                      begin
                                        cur_top := 13;
                                        display_inventory(store_num,cur_top);
                                      end
                                    else
                                      prt('Entire inventory is shown.',1,1)
                                  else
                                    begin
                                      cur_top := 1;
                                      display_inventory(store_num,cur_top);
                                    end
                                end;
                      101     : begin   { Equipment List        }
                                  if (inven_command('e',0,0)) then
                                    display_store(store_num,cur_top);
                                end;
                      105     : begin   { Inventory             }
                                  if (inven_command('i',0,0)) then
                                    display_store(store_num,cur_top);
                                end;
                      109     : begin   { Make an item     }
                                  if (store_num = 7) then
                                    prt('Command not implemented yet.',1,1)
                                  else
                                    prt('Invalid Command.',1,1);
                                end;
                      116     : begin   { Take off              }
                                  if (inven_command('t',0,0)) then
                                    display_store(store_num,cur_top);
                                end;
                      119     : begin   { Wear                  }
                                  if (inven_command('w',0,0)) then
                                    display_store(store_num,cur_top);
                                end;
                      120     : begin   { Switch weapon         }
                                  if (inven_command('x',0,0)) then
                                    display_store(store_num,cur_top);
                                end;
                      112     : begin
			         coupon := false;
  	   exit_flag := store_purchase(store_num,coupon,dummy,cur_top);
				end;
                      117     : Begin
				if get_coupon(store_num,coup_val,cur_top) then
				  begin
				    coupon := true;
	   exit_flag := store_purchase(store_num,coupon,coup_val,cur_top);
			          end;
				End;
                      115     : exit_flag := store_sell(store_num,cur_top);
                      otherwise prt('Invalid Command.',1,1);
                    end;
                  end
                else
                  exit_flag := true;
              until(exit_flag);
              draw_cave;
            End
    END;

{ Entering a hotel}
[psect(store$code)] procedure enter_inn;

    var 
        i5,com_val,oa,ob,oc,od,oe	: integer;
	option,command			: char;
        exit_flag                       : boolean;
	hungry				: vtype;
BEGIN
    exit_flag := false;
    hungry := 'You are too hungry to sleep right now.';
    clear(1,1);
    i5 := turn;
    oa := trunc(5 + 5*chr_padj);    ob := trunc(2 + 1*chr_padj);
    oc := trunc(20 + 20*chr_padj);    od := trunc(50 + 50*chr_padj);
    oe := trunc(500 + 500*chr_padj);
{Display Hotel Options}
     begin
      prt('Max Footroom      (Proprieter)    Paradise Hotel',3,10);
      prt('     Options:',5,1);          prt('  Set Price:',5,61);
      prt('  a) Light Meal',7,1);        prt_num('',oa,7,66);
      prt('  b) Short Nap',8,1);         prt_num('',ob,8,66);
      prt('  c) Overnight Stay',9,1);    prt_num('',oc,9,66);
      prt('  d) Executive Suite',10,1);  prt_num('',od,10,66);
      prt('  e) Mud Bath',11,1);	 prt_num('',oe,11,66);
      store_prt_gold;
      prt('You may:',21,1);
prt('  p) Pay for Option.                   c) Catch a bus.',22,1);
prt(' ^Z) Exit from Building.               j) Apply for a Job.',23,1);
    end;
    repeat
      if (get_com('',command)) then
        begin
	  msg_flag := false;
	  com_val := ord(command);
	  case com_val of
 	    106  : begin
		     if (py.flags.food < 1000) then
		       msg_print('You are too hungry to work')
		     else if ((turn > 4320) and (turn < 12960)) then
		       get_job  {procedure in train.inc}
		     else		    
		       msg_print('We are not hiring right now.');
		     msg_print('');
		     exit_flag := true;
		   end;
	     99	 : begin
		     if (town_num = max_towns) then
		       msg_print('The bus station has been destroyed.')
		     else if (not(defeated_badguy)) then
		       begin
    msg_print('You can''t seem to find a bus.');
    msg_print('Try again after you have defeated the mob boss of this city.');
		       end
		     else if (payment_count > 10) then
		       begin
		         msg_print('Danny''s goons throw you off the bus!');
			 msg_print('"Pay off your loan!!"');
		       end
		     else if (py.flags.food < 2200) then
		       begin
		         msg_print('Your stomach is growling.');
			 msg_print('You''d better eat before you leave.');
		       end
		     else if (py.misc.au < 50) then
		      msg_print('The bus fare is $50, which you don''t have.')
		     else
		       begin
		         turn := turn + 10000;
			 py.flags.food := py.flags.food - 2000;
			 py.misc.au := py.misc.au - 50;
			 town_num := town_num + 1;
			 draw_the_bus;          {code located in misc.inc}
			 town_seed := get_seed;
			 store_init;
			 defeated_badguy := false;
			 py.misc.max_lev := 0;
			 moria_flag := true;    {will draw new cave on exit}
			 office_char_row := -1;
			 office_char_col := -1;
			 exit_flag := true;
		       end;
		   end;
	    112  : begin
		     prt('What would you like?',1,1);
		     exit_flag := get_com('',option);
		   end;
	    otherwise prt('Invalid Command.',1,1);	
	  end;
	end
      else
        exit_flag := true;
    until(exit_flag);
    com_val := ord(option);
    case com_val of
     97:if (py.misc.au < oa) then 
	    msg_print('What! You don''t have enough money!')
	else
	 begin
       	   msg_print('You eat a light meal at the hotel.');
	   py.misc.au := py.misc.au - oa;
	   py.flags.food := py.flags.food + 3000;
	   turn := turn + 360;
 	   zero_pyflag;
	 end;
     98:if (py.misc.au < ob) then 
            msg_print('You can''t even afford to sleep on the lobby couch!')
	else if (py.flags.food < 300) then
	    msg_print(hungry)
	else
	 begin
      	   msg_print('You take a short nap at the hotel.');
	   py.misc.au := py.misc.au - ob;
	   py.flags.food := py.flags.food - 300;
	   py.misc.chp   := py.misc.chp + (py.misc.mhp - py.misc.chp) / 3;
	   turn := turn + 720;
	   zero_pyflag;
	 end;
     99:If (py.misc.au < oc) then
             msg_print('You can''t afford a room, sorry.')
	else if (py.flags.food < 700) then
	    msg_print(hungry)
	else
	 begin
	   msg_print('You spend the night at the hotel.');
	   py.misc.au    := py.misc.au - oc;
	   py.flags.food := 1100 + randint(20);  {small breakfast}
	   midnight;
	   turn := 5800;
	 end;
    100:If (py.misc.au < od) then 
	    msg_print('You cannot afford our luxury suite.  Begone, vermin!')
	else if (py.flags.food < 700) then
	    msg_print(hungry)
	else
	  begin
	   msg_print('You purchase a night in the finest suite of the hotel.');
	   py.misc.au := py.misc.au - od;
	   py.flags.food := 7500;   { large breakfast...  --jeb}
	   midnight;
	   turn := 5800 + randint(1000);
          end;
    101:If (py.misc.au < oe) then 
	    msg_print('Our mud baths are for paying customers only!')
	else if (py.flags.food < 400) then
	    msg_print('You are too hungry to think about mud.')
	else
	 Begin
	   msg_print('You relax in a soothing vat of mud.');
	   py.flags.food := py.flags.food - 360;
	   turn := turn  + 720;
	   zero_pyflag;
	   with py.stat do
	     begin
	       cstr := str;  cint := int;  cwis := wis;
	       cdex := dex;  ccon := con;  cchr := chr;
	     end;
	   with py.misc do
	     begin
	       au := au - oe;
	       chp := mhp;  
	       cmana := mana;
	     end;
	 End;	   	   	       
    otherwise 
      turn := turn + 12;
    End;
    	  {If option makes a day go by, then...}
    if ((com_val in [99,100]) and (turn <> i5)) then
      begin
	zero_pyflag;
        py.misc.chp := py.misc.mhp;
	py.misc.cmana := py.misc.mana;
      end;
    {Safe-check effects of all options:}
    if (py.misc.au < 0) then py.misc.au := 0;
    if (py.flags.food < 10) then py.flags.food := 10;
    if ((i5 < 4320) and (turn > 4320)) then moria_flag := true;
    if ((i5 < 12960) and (turn > 12960)) then moria_flag := true;
    if ((i5 < 8640) and (turn > 8640)) then store_maint;
    if (turn > 17280) then 
      midnight;
    msg_print('');
    draw_cave;
END;


{ Entering the bank}	
[psect(store$code)] procedure enter_bank;

    var 
        i1,com_val,loan_max                     : integer;
	option,command			        : char;
        exit_flag,good_val,exit_bank,payoff     : boolean;
	tmp_str				        : tentype;

BEGIN
    if (payment_count > 10) then
      begin
        iod_val := 0.060 - (0.002 * payment_count);
        iol_val := 0.100 + (0.005 * payment_count);
        danny_cut := 0.080 + (0.002 * payment_count);
	if (payment_count > 15) then
	  begin
	    principal := principal - act_bal;
	    act_bal   := 0;
	    if (principal < 0) then principal := 0;
	  end;	
      end
    else
      begin
        iod_val := 0.060;     {defaults}
        iol_val := 0.100;
        danny_cut := 0.080;
      end;
    with py.misc do
      loan_max := lev * 1000;

   {loan_max := trunc((player_exp[lev]*expfact)*(-99.0*lev/42.0+102.414634));}
    {note:  this is engineered to be around $1000 at first level, and
    decreases to player_exp[lev]*expfact at 42nd level.  -jeb}

    exit_bank := false;
    exit_flag := false;
    payoff    := false;

{Display Bank Options}
    repeat   {exit bank loop}
      clear(1,1);
      prt('Danny Dollar    (Bookie)    '+townlist[town_num].town_name+' Savings and Loan',3,10);
      prt_num('Account Balance     :  ',act_bal,7,20);
      prt_num('Interest on Deposit :  ',trunc(100.0*iod_val),8,20);
      prt_num('Loan Principal      :  ',principal,9,20);
      prt_num('Interest on Loans   :  ',trunc(100.0*iol_val),10,20);
      prt_num('Danny''s Cut         :  ',trunc(100.0*danny_cut),11,20);
      prt('%',8,46);
      prt('%',10,46);
      prt('%',11,46);
      store_prt_gold;
      prt('You may:',21,1);
prt('d) Deposit     l) Ask for a loan           ^Z) Exit Danny''s S&L',22,1);
prt('w) Withdraw    p) Make a loan payment                           ',23,1);

    if (payment_count > 10) then 
      prt('Better get to paying off that loan of yours...',1,10);

    repeat
      if (get_com('',command)) then
        begin
	  msg_flag := false;
	  com_val := ord(command);
	  case com_val of
 	    100  : begin   {deposit}
	             exit_flag := true;
		     repeat
		       good_val := true;
                       prt('How much would you like to deposit?',1,10);
		       get_string(tmp_str,1,47,10);
		       i1 := -1;
		       readv(tmp_str,i1,error:=continue);
		       if (i1 > py.misc.au) then
		         begin
		           good_val := false;
                           msg_print('What, you want I should take the difference out of your gold fillings?');
			   msg_print('');
		         end;
	               if (i1 < 0) then
		         begin
		           good_val := false;
		           msg_print('Look, friend, if you want I should give you money then ask for a loan!');
			   msg_print('');
		         end;
		     until (good_val);
		     if (py.misc.au = 0) then
		       begin
			 exit_bank := true;
   		         msg_print('Come back when you have some cash');
			 msg_print('');
		       end
		     else
		       if (i1 <> 0) then
		         begin
		           act_bal := act_bal + trunc((1.0-danny_cut)*i1);
		           py.misc.au := py.misc.au - i1;
		           msg_print('Your money is safe with me.');
			   msg_print('');
		         end;
		   end;
	    119	 :   if (act_bal = 0) then  {withdraw}
		       begin
		         msg_print('You have no money in your account.');
		         msg_print('');
		       end
		     else
		       begin
		         exit_flag := true;
		         repeat
		           prt('How much would you like to withdraw?',1,10);
		           good_val := true;
		           get_string(tmp_str,1,48,10);
		           i1 := -1;
		           readv(tmp_str,i1,error:=continue);
			   if ((i1 < 0) or (i1 > act_bal)) then
			     begin
			       msg_print('Surely you jest.');
			       msg_print('');
			       good_val := false;
		             end;
		         until(good_val);
		         if (i1 <> 0) then
		           begin
		             py.misc.au := py.misc.au + i1;
			     act_bal := act_bal - i1;
			    msg_print('Now don''t spend it all in one place.');
			     msg_print('');
		           end
		         else
		           msg_print('Come back if you change your mind.');
		       end;
		       
	    108  : begin   {loan}
		     repeat
		       good_val := true;
		       prt('How much do you need?',1,10);
		       get_string(tmp_str,1,32,10);
		       i1 := -1;
		       readv(tmp_str,i1,error:=continue);
		       if (i1 < 0) then good_val := false;
		     until(good_val);
		     if (i1 + principal > loan_max) then
		       msg_print('I don''t think you''re good for that much.')
		     else if (i1 = 0) then
		       msg_print('What, you just like to fill out paperwork?')
		     else
		       begin
		         py.misc.au := py.misc.au + i1;
		         principal := principal + i1;
		         exit_flag := true;
		       end;
		   end;

	    112: begin  {pay off loan}
		   repeat
		     good_val := true;
	             prt('How much would you like to pay?',1,10);
		     get_string(tmp_str,1,43,10);
		     readv(tmp_str,i1,error:=continue);
		     if (i1 < 0) then good_val := false;
		   until(good_val);
		   if (i1 > py.misc.au) then
		     begin
		       msg_print('You''re already in debt, don''t try to pay with money you don''t have.');
		       msg_print('');
		     end
		   else if (i1 = 0) then
		     begin
  		       msg_print('Ok, more interest for me then.');
		       msg_print('');
		     end
		   else
		     begin
		       if (i1 > principal) then 
		         msg_print('Hey!  Thanks for the tip!');
		       principal := principal - i1;
		       if (principal < 0) then principal := 0;
		       if (principal = 0) then payment_count := 0;
		       py.misc.au := py.misc.au - i1;
		       exit_flag := true;
		       payoff    := true;
		     end;
                 end;  {main begin}

	    otherwise prt('Invalid Command.',1,1);	
	  end;
	end
      else
        begin
          exit_flag := true;
	  exit_bank := true;
	end;
    until(exit_flag);
    until(exit_bank);
    if (py.misc.au < 0) then py.misc.au := 0;
    if (not payoff and (principal > 0)) then payment_count := payment_count+1;
    msg_print('');
    draw_cave;
END;


{ Entering the clinic   -jeb}
[psect(store$code)] procedure enter_clinic;
var
    oa,ob     : integer;
    command   : char;
    exit_flag : boolean;
BEGIN
  exit_flag := false;
  clear(1,1);
  oa := py.flags.diseased * 10 * py.misc.lev;
  ob := py.misc.au - randint(100);
  if (ob < 10000) then ob := 10000;
  prt('  Riot Shield medical clinic.       (Dr. Mac ''the knife'' Jones)',3,1);
  prt('Options:',5,6);         prt('Cost:',5,66);
  prt('a) Cure disease',6,3);  prt_num('',oa,6,66);
{  prt('b) New Body',7,3);      prt_num('',ob,7,66);}
  store_prt_gold;
  
  repeat
    get_com('',command);
    case ord(command) of
    97: begin
	  if (py.misc.au < oa) then
	    begin
	      py.flags.diseased := py.flags.diseased
	                        - ((py.misc.au div 10) div py.misc.lev);
	      py.misc.au := 0;
	      msg_print('You feel a bit better.');
	      pause(24);
  	    end
	  else
	    begin
	      py.flags.diseased := 0;
	      py.misc.au := py.misc.au - oa;
	      msg_print('You are Healed!  Be more careful next time.');
	      pause(24);
	    end;
	  if (py.flags.diseased < 0) then py.flags.diseased := 0;
	  if (py.misc.au < 0) then py.misc.au := 0;
	  exit_flag := true;
 	end;
   otherwise
     begin
       prt('We don''t do _that_ here',1,1);
       exit_flag := true;
     end;
  end; {case end}
  until (exit_flag);
  draw_cave;
END;


{ Newsstand/lotto store  --jeb}
[psect(store$code)] procedure newsinc;

  BEGIN
    moria_help('NEWS');
    draw_cave;
  END;




























































































































