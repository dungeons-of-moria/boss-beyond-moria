[psect(wierd$code)]  function query_max_wierd : integer;
  Begin
    query_max_wierd := 18;
  End;


[psect(wierd$code)]  procedure random_wierdness(wierd_type : integer;
						force_wierd: boolean);
   const
       max_wierd = 18;
   var
       i1,i2,i3, y,x  : integer;
       centerx,centery: integer;
       r,theta	      : real;
       scramble       : array[1..6] of integer;
       was_wierd      : boolean;
       jstr	      : vtype;
BEGIN

if (putzuser) then mess_with_mind;
repeat
  was_wierd := true;
  if (wierd_type = 0) then wierd_type := randint(max_wierd);
  case wierd_type of

 1,2,3: read_text_data(0,QUOTES);

     4: if ((randint(1000) > 1) and (is_boss_dead = false)) then
	  was_wierd := false
	else
          begin
            msg_print('Extra! Extra! Read all about it!');
            msg_print('The BOSS has learned of your attempts to defeat him.');
            msg_print('Due to the stress this causes him, he has had');
            msg_print('a cardiac arrest and has died.  You are a...');
            msg_print('***TOTAL WINNER***');
            total_winner_num := total_winner_num + 1;
	    prt_winner;
	    msg_print('');
            if (randint(3) = 3) then        
	      is_boss_dead := true
	    else
              begin  {gee, that was fun.  --jeb}
  	        total_winner_num := total_winner_num - 1;
                msg_print('Just kidding!!!');
                msg_print('Psych!!!');
                msg_print('The BOSS lives!');
		msg_print('');
		draw_cave;
              end;
          end;
     5: begin
          msg_print(' ');
	  i1 := wierd_chance*max_wierd*3000;
	  writev(jstr,'You know, there''s a random one in ',i1:1);
          msg_print(jstr);
	  msg_print('chance that you will just spontaneously win the game.');
          msg_print(' ');
          msg_print('No, really.  It''s true.');
        end;
     6: begin
          msg_print('Just because you''re not paranoid,');
          msg_print('doesn''t mean that they''re not out to get you!');
          msg_print('In fact, we can assure you that they *are* out');
          msg_print('to get you.');
	  for i1 := 1 to 3 do
	    begin
	      y := char_row;
	      x := char_col;
              summon_monster(y,x,false);
	    end;
	  creatures(false);
	  prt_chp;
        end;
     7: begin
          msg_print('Suddenly, Dirty Harry pops out from out of nowhere');
	  msg_print('He says, "Tag! you''re it!".  You say "ouch."');
          if ((randint(2) = 1) and (dun_level <> 0)) then
            msg_print('He says, "I''ll see you back in town..."')
          else
            msg_print('Better watch your back...');
          msg_print('You feel somewhat damaged.');
          begin
            take_hit(damroll('1d10'),'Dirty Harry');
            prt_chp
          end;
        end;
     8: begin
          msg_print('You are hit on the noggin by a falling chunk of stone.');
	  if (randint(2) = 1) then
	    begin
              i1 := randint(max_skills);
              skill_list[i1].learned   := false;
              msg_print('Your head hurts.  You feel like you may have forgotten something.');
              msg_print('A voice says "Psst, hey you... Check your books."');
	    end
	  else
	    take_hit(damroll('1d15'),'a falling rock');
        end;
     9: begin
          msg_print('You feel your soul plummet to the depths of hell.');
          py.misc.max_lev := py.misc.max_lev + randint(py.misc.lev);
	  if (py.misc.max_lev > townlist[town_num].max_depth) then
	    py.misc.max_lev := townlist[town_num].max_depth;
        end;
    10: begin
          msg_print(' ');
          msg_print('Something strange just happened.');
          msg_print('But we''re not telling...');
          if (py.misc.sex = 'Female') then
	    Begin
	      py.misc.sex := 'Male';
              case randint(7) of
                1: py.misc.name := 'Herve Villichez';
		2: py.misc.name := 'The Third Amigo';
		3: py.misc.name := 'Rasputin';
		4: py.misc.name := 'Rassilon';
		5: py.misc.name := 'Dent Arthur Dent';
		6: py.misc.name := 'Ford Prefect';
		7: py.misc.name := 'Slartibartfast';
              end;
	    End
          else
	    Begin
	      py.misc.sex := 'Female';
              case randint(7) of
		1: py.misc.name := 'Elena Redlips';
		2: py.misc.name := 'Tricia McMillan';
		3: py.misc.name := 'Ginger Rogers';
		4: py.misc.name := 'Barbarella';
		5: py.misc.name := 'Dale Arden';
		6: py.misc.name := 'Elvira, Mistress of the dark';
		7: py.misc.name := 'Misery Chastain';
              end;
	    End;
        END;

    11: if (randint(100) > 20) then
          was_wierd := false
	else
	  BEGIN
	    msg_print('Suddenly and without warning, you are enveloped in an');
	    msg_print('Energy Vortex!!!!!');
	    with py.stat do
	      Begin
		scramble[1] := str;	{scramble the player's stats. }
	    	scramble[2] := dex;	{but reset the new currents to}
		scramble[3] := con;	{be restored.            --jeb}
		scramble[4] := int;
		scramble[5] := wis;  scramble[6] := chr;
		for i1 := 1 to 6 do
		  begin
		    i2 := randint(6);
		    i3 := scramble[i1];
		    scramble[i1] := scramble[i2];
		    scramble[i2] := i3;
		  end;	 
	        str := scramble[1];       int := scramble[2];
	        wis := scramble[3];       dex := scramble[4];
	        con := scramble[5];       chr := scramble[6];
		cstr := str;		  cdex := dex;
		ccon := con;		  cint := int;
		cwis := wis;		  cchr := chr;
              End;
	    prt_stat_block;
	  END;
    12: Begin
	  msg_print('You go momentarily insane.');
	  for i1 := 1 to 15 do
	    begin
	      move_char(8);	{make the player run in circles.}
	      move_char(8);	{hee hee hee...           --jeb }
	      move_char(9);  move_char(3);  move_char(2);
	      move_char(2);  move_char(1);  move_char(1);
	      move_char(randint(9));
	    end; 	
	End;		
    13: Begin 
	  msg_print('You completely lose your grip on reality.'); 
	  msg_print('');
	  i3 := py.misc.lev;
	  with py.flags do
	    begin
  	      image := i3+100;
	      fast := i3+50;
	      confused := i3+10;
	      afraid := i3+8;
	      blind := i3+6;
	      poisoned := i3+4;
	      paralysis := i3;
	    end;  
	End;
    14: Begin
          msg_print('You suddenly remember your old nickname!');
	  msg_print('');
	  with py.misc do
	    case randint(16) of
	      1:  race := 'Greedy';
	      2:  race := 'Boring';
	      3:  race := 'Wild';
	      4:  race := 'Friendly';
	      5:  race := 'Sleazy';
	      6:  race := 'Lucky';
	      7:  race := 'Muffin';
	      8:  race := 'Foolish';
	      9:  if (py.misc.sex = 'Female') then
		    race := 'Gorgeous'
		  else
		    race := 'Studly';
	     10:  race := 'Potato-Head';
	     11:  race := 'Dizzy';
	     12:  race := 'Unbearable';
	     13:  race := 'Pond Scum';
	     14:  race := 'Pizza-Face';
	     15:  race := 'Party-Animal';
	     16:  BEGIN
	            race := 'Weiner';
		    msg_print('Oh I wish I was an Oscar Meyer Weiner...');
		  END;
	    end;	
	End;
     15: Begin
	   msg_print('Nickels from heaven!');
	   for i1 := 1 to (100*py.misc.lev) do
	     begin
	       py.misc.au := py.misc.au + randint(3);
	       prt_gold;
	     end;
	 End;
     16: Begin      {danny's goons}
           if (payment_count < 10) then
	     was_wierd := false
	   else
	     begin
               clear(1,1);
    prt('Suddenly, from behind, a big pair of Danny Dollar''s thugs',1,1);
    prt('jump you!  "Danny says yer behind on yer payments," the first',2,1);
    prt('thug says.  "Danny don''t like late payments," his buddy says.',3,1);
    prt('Then, the first thug pins your arms from behind, and blows',4,1);
    prt('cigar smoke in your ear while the other slugs you in the gut',5,1);
    prt('*really* hard, a lot of times.  They take all your money, and',6,1);
    prt('leave you puking your guts out on the ground.',7,1);
               principal := principal - (py.misc.au div 2);
	       if (principal < 0) then principal := 0;
	       if (principal = 0) then payment_count := 0;
               py.misc.au := 0;
	       py.misc.chp := py.misc.chp/2;
	       de_statp(py.stat.con);
	       if (py.stat.ccon > py.stat.con) then py.stat.ccon :=
	         py.stat.con;
	       py.flags.paralysis := 15;
	       py.flags.food := 250;
	       pause(24);
	       draw_cave;
	     end;
	 End;  
     17: if (dun_level < 1) then
	   was_wierd := false
	 else
	 begin
           msg_print('Suddenly, the earth rumbles...');
	   msg_print('A giant hole opens underneath you!');
	   make_light(char_row,char_col,5,false); {big dark hole around player}
	   msg_print('It''s a SINKHOLE!!!');
	   msg_print('You are swirling around in the mud!');
	   centery := char_row - panel_row_prt;
	   centerx := char_col - panel_col_prt;
	   y := centery;
	   x := centerx;
	   for i1 := 1 to 360 do
	     begin
	       put_buffer(' ',y,x); 		    {erase @ from the screen  }
	       r := 5.0 - (1.0*i1) / 72.0;          {radius goes from 5 to 0  }
	       theta := (5.0*i1)/57.2957795;        {5 degree increments      }
	       y := centery + trunc(r*sin(theta));  {some basic trig calcs.   }
	       x := centerx + trunc(1.4*r*cos(theta));
	       sleep(0);
	       put_buffer('@',y,x);  		    {put @ at new place       }
	       sleep(0);
	     end;
           put_buffer(' ',y,x); 
	   dun_level := dun_level + randint(5);
	   if (dun_level > townlist[town_num].max_depth) then
	     dun_level := townlist[town_num].max_depth;
	   moria_flag := true;  		   	   {generate new level}
	   msg_print('You land with a really hard thud.');
	   take_hit(damroll('5d10'),'a sinkhole');
	 end;
     18: begin
          msg_print(' ');
          msg_print('You trip over a small rock, and discover');
          msg_print('some money lying underneath.');
	  msg_print(' ');
          if (randint(100) > 5) then
            i1 := py.misc.lev * 5
          else
            i1 := 1000+randint(1000);
          py.misc.au := py.misc.au + i1;
	  msg_print('');	  	  
	  prt_num('It''s worth $',i1,1,1);
          prt_gold;
        end;

{     : if (unleashed_doom) then (Unleash doom code is still buggy -RLG)
           was_wierd := false
 	else
	  begin
	    unleashed_doom := true;
	    msg_print('');
	    msg_print('You hear a loud clap of thunder');
	    msg_print('and a sickening feeling grows in your stomach.');
	  end;          }

      otherwise ;
  end;
  wierd_type := 0;
until((not force_wierd) or was_wierd);
  if (not was_wierd) then
    case randint(3) of 
      1: msg_print('You just avoided something strange.');
      2: msg_print('That was a close call!');
      3: msg_print('You are _sooo_ lucky!');
    end;	
END;
