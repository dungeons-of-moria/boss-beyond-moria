[psect(create$code)] procedure create_character;

{ Changes stats by given amount }
  function change_stat(cur_stat,amount : integer) : integer;
    var
        i : integer;
    begin
      if (amount < 0) then
        for i := -1 downto amount do
          cur_stat := de_statp(cur_stat)
      else
        for i := 1 to amount do
          cur_stat := in_statp(cur_stat);
      change_stat := cur_stat;
    end;

{Get stats - mwk}
function race_base(i2:integer):boolean;
begin
  race_base:=true;
  py.misc.age := race[i2].b_age + randint(race[i2].m_age);
  with py do
    with race[i2] do
      begin
       with stat do
       BEGIN
        str    := 12;
        int    := 12;
        wis    := 12;
        dex    := 12;
        con    := 12;
        chr    := 12;
        str    := change_stat(stat.str,str_adj);
        int    := change_stat(stat.int,int_adj);
        wis    := change_stat(stat.wis,wis_adj);
        dex    := change_stat(stat.dex,dex_adj);
        con    := change_stat(stat.con,con_adj);
        chr    := change_stat(stat.chr,chr_adj);
       END;
       with misc do
       BEGIN
        bth    := bbth;
        bthb   := bbthb;
        perc   := bperc;
        stl    := bstl;
	wierd  := bwierd;
        save   := bsav;
        srh    := bsrh;
        hitdie := bhitdie;
        lev    := 1;
        expfact:= b_exp;
	au     := 400;
       END;
        flags.see_infra := binfra
      end
end;{race_base}
                                 
{ Allows player to select a race }
  function choose_race : boolean;
    var
        i2,i3,i4,i5 : integer;
        s : char;
        exit_flag : boolean;
    begin
      i2 := 1;
      i3 := 1;
      i4 := 3;
      i5 := 22;
      clear(21,1);
      prt('Choose a quality (? for Help):',21,3);
      repeat
        put_buffer (chr(i3+96)+') '+race[i2].trace,i5,i4);
        i3 := i3 + 1;
        i4 := i4 + 15;
        if (i4 > 70) then
          begin
            i4 := 3;
            i5 := i5 + 1
          end;
        i2 := i2 + 1
      until (i2 > max_races);
      py.misc.race := '';
      put_buffer('',21,33);
      exit_flag := false;
      repeat
        inkey_flush(s);
        i2 := index('abcdefghijklmnopqrstuvwxyz',s);
        if ((i2 <= max_races) and (i2 >= 1)) then
            with py do
              with race[i2] do
                begin
                  misc.prace  := i2;
                  misc.race   := trace;
                  race_base(i2);
                  exit_flag := true;
                  choose_race := true;
                  put_buffer(py.misc.race,4,11);
                end
        else if (s = '?') then
          begin
            moria_help('Character Qualities');
            exit_flag := true;
            choose_race := false;
          end;
      until (exit_flag);
    end;
 
 
{Will print the history of a character }
procedure print_history;
var
   i1 : integer;
begin
  put_buffer('Character Background',16,28);
  for i1 := 1 to 5 do
     put_buffer(py.misc.history[i1],i1+16,5)
end;

 
{ Get the racial history, determines social class 
  Assumtions:   Each race has init history beginning at
                 (race-1)*3+1
                All history parts are in accending order }
  procedure get_history;
    var
        hist_ptr,cur_ptr,test_roll,
        start_pos,end_pos,cur_len,
        line_ctr,new_start,social_class : integer;
        history_block : varying [400] of char;
        flag : boolean;
    begin
{ Get a block of history text }
      hist_ptr := (py.misc.prace-1)*3 + 1;
      history_block := '';
      social_class := randint(4);
      cur_ptr := 0;
      repeat
        flag := false;
        repeat
          cur_ptr := cur_ptr + 1;
          if (background[cur_ptr].chart = hist_ptr) then
            begin
              test_roll := randint(100);
              while (test_roll > background[cur_ptr].roll) do
                cur_ptr := cur_ptr + 1;
              with background[cur_ptr] do
                begin
                  history_block := history_block + info;
                  social_class := social_class + bonus;
                  if (hist_ptr > next) then cur_ptr := 0;
                    hist_ptr := next;
                end;
              flag := true;
            end;
        until(flag);
      until(hist_ptr < 1);
{ Process block of history text for pretty output }
      start_pos := 1;
      end_pos   := length(history_block);
      line_ctr  := 1;
      flag := false;
      while (history_block[end_pos] = ' ') do
        end_pos := end_pos - 1;
      repeat
        while (history_block[start_pos] = ' ') do
          start_pos := start_pos + 1;
        cur_len := end_pos - start_pos + 1;
        if (cur_len > 70) then
          begin
            cur_len := 70;
            while (history_block[start_pos+cur_len-1] <> ' ') do
              cur_len := cur_len - 1;
            new_start := start_pos + cur_len;
            while (history_block[start_pos+cur_len-1] = ' ') do
              cur_len := cur_len - 1;
          end
        else
          flag := true;
        py.misc.history[line_ctr] := substr(history_block,start_pos,cur_len);
        line_ctr := line_ctr + 1;
        start_pos := new_start;
      until(flag);
{Compute social class for player}
      if (social_class > 100) then
        social_class := 100
      else
        if (social_class < 1) then
          social_class := 1;
      py.misc.sc := social_class;
    end;
                           
 
{Gets the character's sex }
  function get_sex : boolean;
    var
        i1 : integer;
        s : char;
        exit_flag : boolean;
    begin
      py.misc.sex := '';
      clear(21,1);
      prt('Choose a sex (? for Help):',21,3);
      prt('m) Male       f) Female',22,3);
      prt('',21,29);
      i1 := py.misc.prace;
      repeat
        inkey_flush(s);
        case s of
          'f' : begin
		  py.misc.sex := 'Female';
		  py.misc.ht := randnor(race[i1].f_b_ht,race[i1].f_m_ht);
		  py.misc.wt := randnor(race[i1].f_b_wt,race[i1].f_m_wt);
                  prt(py.misc.sex,5,11);
                  exit_flag := true;
                  get_sex := true;
                end;
          'm' : begin
                  py.misc.sex := 'Male';
		  py.misc.ht := randnor(race[i1].m_b_ht,race[i1].m_m_ht);
		  py.misc.wt := randnor(race[i1].m_b_wt,race[i1].m_m_wt);
                  prt(py.misc.sex,5,11);
                  exit_flag := true;
                  get_sex := true;
                end;
          '?' : begin
                  moria_help('Character Sex');
                  exit_flag := true;
                  get_sex := false
                end;
          otherwise
         end;
      until (exit_flag)
    end;
 

{Gets a character class }
  function get_class : boolean;
    var
        i1,i2,i3,i4,i5 : integer;
        cl : array [0..max_class] of integer;
        s : char;
        exit_flag : boolean;         
    begin
      for i2 := 1 to max_class do cl[i2] := 0;
      i1 := py.misc.prace;                           
      i2 := 1;
      i3 := 0;
      i4 := 3;
      i5 := 22;
      clear(21,1);
      prt('Choose a class (? for Help):',21,3);
      repeat
        {if (uand(race[i1].tclass,bit_array[i2]) <> 0) then}
          begin
            i3 := i3 + 1;
            put_buffer (chr(i3+96)+') '+class[i2].title,i5,i4);
            cl[i3] := i2;
            i4 := i4 + 15;
            if (i4 > 70) then
              begin
                i4 := 3;
                i5 := i5 + 1
              end;
          end;
        i2 := i2 + 1;
      until (i2 > max_class);
      py.misc.pclass := 0;
      put_buffer('',21,31);
      exit_flag := false;
      repeat
        inkey_flush(s);
        i2 := index('abcdefghijklmnopqrstuvwxyz',s);
        if ((i2 <= i3) and (i2 >= 1)) then
          begin
            py.misc.tclass := class[cl[i2]].title;
            py.misc.pclass := cl[i2];
            exit_flag := true;
            get_class := true;
            clear(21,1);
            put_buffer(py.misc.tclass,6,11);
            with py.misc do
              begin
	        pskill := class[pclass].skill_no;
  		get_skill_data(pskill);  { located in datafiles.inc -RLG }
                hitdie := hitdie + class[pclass].adj_hd;
                bth    := bth     + class[pclass].mbth;
                bthb   := bthb    + class[pclass].mbthb;
                srh    := srh     + class[pclass].msrh;
                disarm := disarm  + class[pclass].mdis;
                perc   := perc    + class[pclass].mperc;
                stl    := stl     + class[pclass].mstl;
		wierd  := wierd   + class[pclass].mwierd;
                save   := save    + class[pclass].msav;
                title  := player_title[pclass,1];
                expfact:= expfact + class[pclass].m_exp;
              end;
{ Adjust the stats for the class adjustment }
            with py do
              begin
                stat.str  := change_stat(stat.str,class[misc.pclass].madj_str);
                stat.int  := change_stat(stat.int,class[misc.pclass].madj_int);
                stat.wis  := change_stat(stat.wis,class[misc.pclass].madj_wis);
                stat.dex  := change_stat(stat.dex,class[misc.pclass].madj_dex);
                stat.con  := change_stat(stat.con,class[misc.pclass].madj_con);
                stat.chr  := change_stat(stat.chr,class[misc.pclass].madj_chr);
              end
          end
        else if (s = '?') then
          begin
            moria_help('Character Classes');
            exit_flag := true;
            get_class := false;
	    clear(1,1);
          end;
      until(exit_flag);
    end;
                       
{New method to adjust a new character's skills and stats!!!  -RLG}
 procedure   skill_adjust;
   var  
     y,x,crtpts   : integer;
     out_val	  : vtype;
     command	  : char;
     exit_flag	  : boolean;

   procedure  spot(a,b : integer);
   begin
     put_buffer('*',a,b);
     put_buffer('',a,b);
   end;

   procedure  unspot(a,b : integer);
   begin
     put_buffer(' ',a,b);
   end;


   function modif_st(y,x,dir,stat,max,min,amt,upp : integer) : integer;
     var
       out_val : vtype;

     begin
       modif_st := stat;
       case dir of
         1:   {Raise Stat/Lower Points}
            if ((stat < max-upp+1) and (crtpts >= amt)) then
              begin
	        stat := stat + upp;
	        crtpts := crtpts - amt
	      end;
	 2:   {Raise Stat/Raise Points}
	    if (stat < max-upp+1) then
	      begin
	       stat := stat + upp;
	       crtpts := crtpts + amt;
	      end;
	-1:   {Lower Stat/Raise Points}
	    if (stat > min+upp-1) then
              begin
	        stat := stat - upp;
    	        crtpts := crtpts + amt
	      end;
	-2:   {Lower Stat/Lower Points}
	    if ((stat > min+upp-1) and (crtpts >= amt)) then
	      begin
	        stat := stat - upp;
   	        crtpts := crtpts - amt;
	      end;
	otherwise;
       end;
       if (y+x <> 83) then
         prt_num('',stat,y,x)
       else
	 begin
	   writev(out_val,0.1*sngl(stat):2:1);
	   prt(out_val,10,73);
	 end;
       prt_num('',crtpts,14,45);
       modif_st := stat;
     end;	      

   procedure alter_stat(dir,num : integer);
   begin
     case num of
        9: py.stat.str := modif_st(7,22,dir,py.stat.str,50,3,10,1);
       10: py.stat.int := modif_st(8,22,dir,py.stat.int,50,3,10,1);
       11: py.stat.wis := modif_st(9,22,dir,py.stat.wis,50,3,10,1);
       12: py.stat.dex := modif_st(10,22,dir,py.stat.dex,50,3,10,1);
       13: py.stat.con := modif_st(11,22,dir,py.stat.con,50,3,10,1);
       14: py.stat.chr := modif_st(12,22,dir,py.stat.chr,50,3,10,1);
      34: py.misc.bth := modif_st(7,48,dir,py.misc.bth,200,1,1,4);
      35: py.misc.bthb := modif_st(8,48,dir,py.misc.bthb,200,1,1,4);
      36: py.misc.save := modif_st(9,48,dir,py.misc.save,100,1,2,5);
      37: py.misc.stl := modif_st(10,48,dir,py.misc.stl,10,1,20,1);
      38: py.misc.disarm := modif_st(11,48,dir,py.misc.disarm,100,1,2,5);
      39: py.misc.wierd := modif_st(12,48,dir,py.misc.wierd,100,1,2,5);
       59: py.misc.perc := modif_st(7,73,dir,py.misc.perc,100,1,2,5);
       60: py.misc.srh := modif_st(8,73,dir,py.misc.srh,100,1,2,5);
       61: py.flags.see_infra := 
	      modif_st(9,73,dir,py.flags.see_infra,20,0,20,1);
       62: py.misc.expfact :=
	      0.1 * modif_st(10,73,2*dir,trunc(py.misc.expfact*10),30,10,40,1);
       63: py.misc.hitdie := modif_st(11,73,dir,py.misc.hitdie,30,4,40,1);
       64: py.misc.au := modif_st(12,73,dir,py.misc.au,5000,0,1,20);
       otherwise;     
     end; 
   end;

   BEGIN
     y := 7;
     x := 2;
     crtpts := 100;
     clear(1,1);
  prt('Move the cursor keys to the attribute that you wish to change.',2,5);
  prt('Then use ''>'' to increase, or ''<'' to decrease that attribute.',3,5);
  prt('The numbers in paratheses are the amount of change and cost.',4,5);
  prt('When you are finished hit the escape key.',5,5);
     with py.stat do
       begin
         prt_stat('Strength     (1/10)',str,7,3);
         prt_stat('Intelligence (1/10)',int,8,3);
         prt_stat('Wisdom       (1/10)',wis,9,3);
         prt_stat('Dexterity    (1/10)',dex,10,3);
         prt_stat('Constitution (1/10)',con,11,3);
         prt_stat('Charisma     (1/10)',chr,12,3);
       end;
     with py.misc do
       begin
         prt_num('Melee Fighting (5/1)',bth,7,28);
	 prt_num('Gun & Grenades (5/1)',bthb,8,28);
	 prt_num('Saving Throw   (5/2)',save,9,28);
	 prt_num('Stealth       (1/20)',stl,10,28);
	 prt_num('Disarming      (5/2)',disarm,11,28);
	 prt_num('Wierd Device   (5/2)',wierd,12,28);
	 prt_num('Perception     (5/2)',perc,7,53);
	 prt_num('Searching      (5/2)',srh,8,53);
	 prt_num('Infravision   (1/20)',py.flags.see_infra,9,53);
  writev(out_val,'Expr. Factor (.1/40)',expfact:2:1);
	 prt(out_val,10,53);
	 prt_num('Hit Die       (1/40)',hitdie,11,53);
	 prt_num('Starting Cash (20/1)',au,12,53);
	 prt_num('Creation Points Left : ',crtpts,14,22);
	 print_history;
	 prt('Note: Experience Factor is a multiplier of the amount of experience',23,5);
	 prt('points your character will need to advance levels.',24,11);
       end;
     spot(y,x);
     repeat
       inkey(command);
       case ord(command) of
         27: begin
	       exit_flag := true;
	     end;
	 50: begin {down}
	       unspot(y,x);
	       y := y + 1;
	       if (y = 13) then y := 7;	     
	       spot(y,x);
	     end;
	 52: if (x <> 2) then  {left}
	       begin
		 unspot(y,x);
		 x := x - 25;
		 spot(y,x);
	       end;
	 54: if (x <> 52) then  {right}
	       begin
	         unspot(y,x);
		 x := x + 25;
		 spot(y,x);
	       end;
	 56: begin {up}
	       unspot(y,x);
	       y := y - 1;
	       if (y = 6) then y := 12;
	       spot(y,x);
	     end;
     60,44 : { < (decrease) } alter_stat(-1,x+y);
     62,46 : { > (increase) } alter_stat( 1,x+y);
	 otherwise;
       end;

     until(exit_flag);
     py.misc.au := py.misc.au + 20*crtpts;  {Leftover points turn into money}
     with py.stat do
       begin
         cstr := str;
         cint := int;
         cwis := wis;
         cdex := dex;
         ccon := con;
         cchr := chr;
       end;
     with py.misc do
       begin
         mhp    := con_adj + hitdie;
         chp    := mhp;
{ Real } ptodam := todam_adj;
         ptohit := tohit_adj;
         ptoac  := toac_adj;
         pac    := 0;
{Display}dis_th := ptohit;
         dis_td := ptodam;
         dis_tac:= ptoac;
         dis_ac := pac
       end;
    END;

{begin create procedure}
    begin
    	put_character;
        repeat until(choose_race);
        repeat until(get_sex);
        get_history;
        race_base(py.misc.prace);
        get_class;
	skill_adjust;
        put_character;
        put_stats;
        put_misc1;
        put_misc2;
        put_misc3;
        get_name;
        pause_exit(24);
	clear(1,1);
	draw_the_bus;
    end;
