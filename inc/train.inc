{Training}

[psect(store$code)] function get_option : char;
   var 

       option		:	char;
       com_val		:	integer;
       exit_flag	:	boolean;
       
Begin
	prt_num('Dollars Remaining : ',py.misc.au,19,23);
	prt('You may:',21,6);
prt('  p) Pay for Option.               space) Browse options.',22,6);
prt(' ^Z) Exit.                            ^R) Redraw the screen.',23,6);
    repeat
      if (get_com('',command)) then
        begin
	  msg_flag := false;
	  com_val := ord(command);
	  case com_val of 
	     18  :;
	     32  :prt('All options are shown.',1,1);
	    112  :begin
		    prt('What would you like?',1,1);
		    exit_flag := get_com('',option);
		  end;
	    otherwise prt('Invalid Command.',1,1);
	  end;
	end
      else
        exit_flag := true;      
    until(exit_flag);
    get_option := option;
End;

{Specific Training within a store}

[psect(store$code)] procedure ltrain;
   var
       oa,ob,oc		:	integer;
       too_poor		:	ttype;       

BEGIN
     begin {set prices}
       oa := py.misc.stl*100;
       ob := (py.misc.perc)*10;
       oc := 20*py.stat.ccon**2;
     end;
     too_poor := 'You cannot afford such professional services.';
     prt('Welcome to the Survivalist Union',3,20);
     prt(' How can we assist you?',5,1);        prt('   Your Price:',5,61);
     prt(' a) Increase Stealth Skill',7,1);     prt_num('',oa,7,66);
     prt(' b) Increase Perception Skill',8,1);	prt_num('',ob,8,66);
     prt(' c) Increase Constitution',9,1);      prt_num('',oc,9,66);
    case ord(get_option) of {97-99}
      97: Begin
	    if (py.misc.au < oa) then
	       msg_print(too_poor)
	    else
	      begin
      	msg_print('You suffer hours of intensive games of hide and seek.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oa;
		py.flags.food := py.flags.food - 3000;		
		py.misc.stl := py.misc.stl + 1;
	      end;
	  End;
      98: Begin
	    if (py.misc.au < ob) then
	       msg_print(too_poor)
	    else
	      begin
      	msg_print('You spend hours doing intensive word search puzzles.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - ob;
		py.flags.food := py.flags.food - 3000;		
		py.misc.perc := py.misc.perc + 5 + randint(5);
	      end;
	  End;
      99: Begin
	    if (py.misc.au < oc) then
	       msg_print(too_poor)
	    else
	      begin
      	msg_print('You do aerobic exercises and practice swallowing poisons.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oc;
		py.flags.food := py.flags.food - 3000;		
		py.stat.ccon := in_statt(py.stat.ccon);
		if (py.stat.ccon > py.stat.con) then
		    py.stat.con := py.stat.ccon;
	      end;
	  End;
       otherwise turn := turn + 12;
    End;
END;

[psect(store$code)] procedure wtrain;

   var
       oa,ob,oc		:	integer;
       too_poor		:	ttype;       

BEGIN
     begin {set prices}
       oa := py.misc.bth*10;
       ob := py.misc.bthb*10;
       oc := 20*py.stat.cstr**2;
     end;
     too_poor := 'You cannot afford our physical training.';
     prt('Welcome to the Soldier of Fortune Outpost',3,15);
     prt(' How can we assist you?',5,1);    prt('   Your Price:',5,61);
     prt(' a) Increase Melee Skill',7,1);   prt_num('',oa,7,66);
     prt(' b) Increase Gun Skill',8,1);	    prt_num('',ob,8,66);
     prt(' c) Increase Strength',9,1);	    prt_num('',oc,9,66);
    case ord(get_option) of {97-99}
      97: Begin
	    if (py.misc.au < oa) then
	       msg_print(too_poor)
	    else
	      begin
	      	msg_print('You endure hours of hand to hand training.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oa;
		py.flags.food := py.flags.food - 3000;		
		py.misc.bth := py.misc.bth + 5 + randint(5);
	      end;
	  End;
      98: Begin
	    if (py.misc.au < ob) then
	       msg_print(too_poor)
	    else
	      begin
	      	msg_print('You endure hours of ranged weapon training.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - ob;
		py.flags.food := py.flags.food - 3000;		
		py.misc.bthb := py.misc.bthb + 5 + randint(5);
	      end;
	  End;
      99: Begin
	    if (py.misc.au < oc) then
	       msg_print(too_poor)
	    else
	      begin
	      	msg_print('You lift weights under skilled supervision.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oc;
		py.flags.food := py.flags.food - 3000;		
		py.stat.cstr := in_statt(py.stat.cstr);
		if (py.stat.cstr > py.stat.str) then
		    py.stat.str := py.stat.cstr;
	      end;
	  End;
	otherwise turn := turn + 12;
      End;	     		
END;

[psect(store$code)] procedure ttrain;

   var
       oa,ob,oc		:	integer;
       too_poor		:	ttype;       

BEGIN
     begin {set prices}
       oa := py.misc.save*10;
       if (py.misc.pclass in [3,6]) then
         ob := py.misc.lev*600
       else
         ob := py.misc.lev*1000;
       oc := 20*py.stat.cwis**2;
     end;
   too_poor := 'Sorry, but you must donate more money than you are carrying.';
     prt('Welcome to the Illustrious All-Knowing Club',3,15);
     prt(' How may we aid you?',5,1);    prt('   Your Price:',5,61);
     prt(' a) Increase Saving Throw',7,1);  prt_num('',oa,7,66);
     prt(' b) Increase Resources',8,1);	    prt_num('',ob,8,66);
     prt(' c) Increase Wisdom',9,1);	    prt_num('',oc,9,66);
    case ord(get_option) of {97-99}
      97: Begin
	    if (py.misc.au < oa) then
	       msg_print(too_poor)
	    else
	      begin
	      	msg_print('You study the mystical arts of avoiding pain.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oa;
		py.flags.food := py.flags.food - 3000;		
		py.misc.save := py.misc.save + 5 + randint(5);
	      end;
	  End;
      98: Begin
	    if (py.misc.au < ob) then
	       msg_print(too_poor)
	    else if(py.misc.pclass in [1,4]) then
	       msg_print('You don''t use resources!  How unresourceful.')
	    else
	      begin
	      	msg_print('You learn a few ways to conserve your resources.');
		turn := turn + 720;
		py.misc.au := py.misc.au - ob;
		py.flags.food := py.flags.food - 700;		
		py.misc.mana := py.misc.mana + 1 + wis_adj;
	      end;
	  End;
      99: Begin
	    if (py.misc.au < oc) then
	       msg_print(too_poor)
	    else
	      begin
     	   msg_print('You spend hours studying all sorts of theology books.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oc;
		py.flags.food := py.flags.food - 3000;		
		py.stat.cwis := in_statt(py.stat.cwis);
		if (py.stat.cwis > py.stat.wis) then
		    py.stat.wis := py.stat.cwis;
	      end;
	  End;
	otherwise turn := turn + 12;
      End;	     		
END;

[psect(store$code)] procedure dtrain;

   var
      i3,oa,ob,oc,od	:	integer;
      too_poor		:	ttype;
      

BEGIN
     begin {set price on options}
       oa := trunc(1000*(py.misc.mhp/py.misc.hitdie));
       ob := (py.flags.see_infra + 1)*100;
       oc := 20*py.stat.cchr**2;
       od := 500;
     end;
     too_poor := 'You cannot afford our chemical talents.';
     prt('Welcome to the Secret Society of Chemists',3,15);
     prt(' How can we help you?',5,1);   prt('   Your Price:',5,61);
     prt(' a) Increase Hit Points',7,1);    prt_num('',oa,7,66);
     prt(' b) Increase Infra-Vision',8,1);  prt_num('',ob,8,66);
     prt(' c) Increase Charisma',9,1);	    prt_num('',oc,9,66);
     prt(' d) Sex Change',10,1);	    prt_num('',od,10,66);
    case ord(get_option) of  {97-100}
      97:  if (py.misc.au < oa) then
	      msg_print(too_poor)
	   else
	     Begin
	       msg_print('You are bathed in a juicy mixture.');
	       msg_print('');
	       turn := turn + 360;
	       with py.misc do
	        begin
	         au := au - oa;
		 i3 := get_hitdie;
		 chp := chp + i3;  	mhp := mhp + i3;
		end;	       		 
	     End;
      98:  if (py.misc.au < ob) then
	      msg_print(too_poor)
	   else
	     Begin
	       msg_print('Strange chemicals are squirted into your eyes.');
	       msg_print('');
	       turn := turn + 120;
	       py.misc.au := py.misc.au - ob;
	       py.flags.see_infra := py.flags.see_infra + 1;	        
	     End;	       
      99:  if (py.misc.au < oc) then
	      msg_print(too_poor)
	   else
	     Begin
	       msg_print('You are given several skin treatments and');
	       msg_print('taught a few social graces.');
	       msg_print('');
	       turn := turn + 2160;
	       py.misc.au := py.misc.au - oc;
	       py.flags.food := py.flags.food - 3000;
	       py.stat.cchr := in_statt(py.stat.cchr);	       	     
	       if (py.stat.cchr > py.stat.chr) then 
		   py.stat.chr := py.stat.cchr;
	     End;
     100:  if (py.misc.au < od) then
	      msg_print(too_poor)
	   else
	     Begin
	      msg_print('The Chemists lock you in a dark closet for hours...');
	       msg_print('');
	       turn := turn + 2880;
  	       py.flags.food := py.flags.food - 2880;
	       with py.misc do
	         begin
		   if (randint(10) = 10) then
		     begin
		       sex := 'Neuter';
  msg_print('A horrible thing has happened!  The Chemists refund your money.');
		       msg_print('');
		     end
		   else
		     au := au - od;
		     if (sex = 'Male') then 
		         sex := 'Female'
		     else
		         sex := 'Male';
		 end;
	     End;
	   otherwise
	     turn := turn + 12;
	End;
END;

[psect(store$code)] procedure atrain;

   var
       oa,ob,oc		:	integer;
       too_poor		:	ttype;       
BEGIN
     oa := py.misc.wierd*10;
     if (py.misc.pclass in [2,5]) then
       ob := py.misc.lev*600
     else
       ob := py.misc.lev*1000;
     oc := 20*py.stat.cint**2;
     too_poor := 'You are too poor for us to help you.  Where is your money?';
     prt('Welcome to the Academy of Unknown Sciences',3,15);
     prt(' Shall we assist you?',5,1);      prt('   Your Price:',5,61);
     prt(' a) Increase Wierd Device',7,1);  prt_num('',oa,7,66);
     prt(' b) Increase Resources',8,1);	    prt_num('',ob,8,66);
     prt(' c) Increase Intellegence',9,1);  prt_num('',oc,9,66);
    case ord(get_option) of {97-99}
      97: Begin
	    if (py.misc.au < oa) then
	       msg_print(too_poor)
	    else
	      begin
	      	msg_print('You play around with a few strange thingies.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oa;
		py.flags.food := py.flags.food - 3000;		
		py.misc.wierd := py.misc.wierd + 5 + randint(5);
	      end;
	  End;
      98: Begin
	    if (py.misc.au < ob) then
	       msg_print(too_poor)
	    else if(py.misc.pclass in [1,4]) then
	       msg_print('You don''t use resources!  How unresourceful.')
	    else
	      begin
	      	msg_print('You learn a few ways to conserve your resources.');
		turn := turn + 720;
		py.misc.au := py.misc.au - ob;
		py.flags.food := py.flags.food - 700;		
		py.misc.mana := py.misc.mana + 1 + int_adj;
	      end;
	  End;
      99: Begin
	    if (py.misc.au < oc) then
	       msg_print(too_poor)
	    else
	      begin
     	   msg_print('You study your brains out and learn more about llamas.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oc;
		py.flags.food := py.flags.food - 3000;		
		py.stat.cint := in_statt(py.stat.cint);
		if (py.stat.cint > py.stat.int) then
		    py.stat.int := py.stat.cint;
	      end;
	  End;
	otherwise turn := turn + 12;
      End;	     		
END;


[psect(store$code)] procedure gtrain;

   var
       oa,ob,oc		:	integer;
       too_poor		:	ttype;       

BEGIN
     begin {set prices}
       oa := py.misc.disarm*10;
       if (py.misc.pclass in [7,8]) then
         ob := py.misc.lev*600
       else
         ob := py.misc.lev*1000;
       oc := 20*py.stat.cdex**2;
     end;
     too_poor := 'We can''t afford to help charity cases like you.';
     prt('Welcome to the Guild of Honest Citizens',3,15);
     prt(' What can we do for you?',5,1);    prt('   Your Price:',5,61);
     prt(' a) Increase Disarming Skill',7,1);  prt_num('',oa,7,66);
     prt(' b) Increase Resources',8,1);	    prt_num('',ob,8,66);
     prt(' c) Increase Dexterity',9,1);	    prt_num('',oc,9,66);
    case ord(get_option) of {97-99}
      97: Begin
	    if (py.misc.au < oa) then
	       msg_print(too_poor)
	    else
	      begin
      	msg_print('You carefully experiment with dangerous mousetraps.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oa;
		py.flags.food := py.flags.food - 3000;		
		py.misc.disarm := py.misc.disarm + 5 + randint(5);
	      end;
	  End;
      98: Begin
	    if (py.misc.au < ob) then
	       msg_print(too_poor)
            else if(py.misc.pclass in [1,4]) then
	       msg_print('You don''t use resources!  How unresourceful.')
	    else
	      begin
	      	msg_print('You learn a few ways to conserve your resources.');
		turn := turn + 720;
		py.misc.au := py.misc.au - ob;
		py.flags.food := py.flags.food - 700;		
		py.misc.mana := py.misc.mana + 1 + chr_adj;
	      end;
	  End;
      99: Begin
	    if (py.misc.au < oc) then
	       msg_print(too_poor)
	    else
	      begin
     	       msg_print('You spend a few hours juggling and throwing darts.');
		turn := turn + 2880;
		py.misc.au := py.misc.au - oc;
		py.flags.food := py.flags.food - 3000;		
		py.stat.cdex := in_statt(py.stat.cdex);
		if (py.stat.cdex > py.stat.dex) then
		    py.stat.dex := py.stat.cdex;
	      end;
	  End;
	otherwise turn := turn + 12;
      End;	     		
END;


{Special Training- From inside a store}
[psect(store$code)] procedure training(store_num : integer);

   var
   	i5 : integer;

BEGIN
	i5 := turn;
	clear(1,1);
	Case store_num of 
	  2:  ltrain;
	  3:  wtrain;
	  4:  ttrain;
	  5:  dtrain;
	  6:  atrain;
	 11:  gtrain;
    otherwise msg_print('You find no trainers here.  Try a different store.');
	End;
    if (py.flags.food < 650) then py.flags.food := 650;
    if ((i5 < 4320) and (turn > 4320)) then moria_flag := true;
    if ((i5 < 12960) and (turn > 12960)) then moria_flag := true;
    if (turn > 17280) then 
      midnight;
    msg_print('');
END;

	  
[psect(store$code)] function get_job_option(var option : char) : boolean;
   var 
       com_val		:	integer;
       exit_flag	:	boolean;
       
Begin
    exit_flag := false;
    get_job_option := false;
    prt_num('Dollars Remaining : ',py.misc.au,19,23);
    prt('You may:',21,1);
prt('  p) Pick a part time job.         space) Browse options.',22,6);
prt(' ^Z) Exit.                            ^R) Redraw the screen.',23,6);
    repeat
      if (get_com('',command)) then
        begin
 	  msg_flag := false;
	  com_val := ord(command);
	  case com_val of 
	     18  :;
	     32  :prt('All jobs are shown.',1,1);
	    112  :begin
		    prt('Which would you like?',1,1);
		    exit_flag := get_com('',option);
		    if ((ord(option) < 49) or (ord(option) > 57)) then
		      exit_flag := false
		    else 
		      get_job_option := true;
		  end;
	    otherwise prt('Invalid Command.',1,1);
	  end;
	end
      else
        exit_flag := true;      
    until(exit_flag);
 END;


[psect(store$code)] procedure get_job;

   var
       option		:	char;
       qualified	:	integer;
       i1,i3,i4		:	integer;
BEGIN
   qualified := (py.misc.lev div 4) + 2;
   clear(1,1);
   prt('Blue Fish Temporary Hiring Services, Inc.',3,16);
   prt(' List of Jobs Available:',5,1);    prt('   Hourly Wage:',5,61);
   for i1 := 1 to 9 do
   begin
     prt_num('  ',i1,i1+6,1);
     prt(')',i1+6,4);
     prt(joblist[i1],i1+6,5);
     prt_num('$',jobwage[i1],i1+6,66);
   end;
   if (get_job_option(option)) then 
     i3 := ord(option) - 48; {1-9}
     If (i3 > qualified) then 
        msg_print('You are not qualified for that job.')
     Else
       Begin
         msg_print('You endure several hours of work');
	 turn := turn + 5760;
	 i4 := randint(100);
	 Case i3 of
	   1: Begin
	        if (i4 > 95) then begin
		    msg_print('You lose a finger in the disposal machine');
		    py.stat.cdex := de_statt(py.stat.cdex);
		  end    
		else if (i4 > 80) then begin
		    msg_print('You find a missing earring and are rewarded!'); 
		    py.misc.au := py.misc.au + 20;
		  end;
	       End;
            2: Begin
	        if (i4 > 90) then begin
	          msg_print('You are steamed badly and your health declines.');
		    py.stat.ccon := de_statt(py.stat.ccon);
		  end		    
		else if (i4 > 80) then begin
		    msg_print('Your determination earns you a pay bonus!'); 
		    py.misc.au := py.misc.au + 30;
		  end;
	       End;
            3: Begin
	        if (i4 > 95) then begin
	            msg_print('You take the wrong elevator out!');
		    dun_level := 2;
		  end		    
		else if (i4 > 90) then begin
		    msg_print('A wealty couple gives you a *big* tip!'); 
		    py.misc.au := py.misc.au + 50;
		  end;
	       End;
	    Otherwise;
	 end;
	 py.misc.au := py.misc.au + (jobwage[i3]*8);
	 py.flags.food := py.flags.food - 5760;
	 zero_pyflag;
       End;	     		
END;

[psect(towns$code)] procedure take_bus;
BEGIN
END;


