{ Handles the gravestone and the top fifty routines, and
  also the 100-day time limit routine. }

procedure upon_death;

var
	points : longint;
   
 
{ Prints the gravestone of the character }
    procedure print_tomb;
      var
        str1,str2,str3,str4,str5,str6,str7,str8 : vtype;
        dstr : array [0..19] of vtype;
        fnam : vtype;
        f1 : text;
        i1 : integer;
        deathdate : string[10];
      BEGIN
	deathdate := formatdatetime('dd-mm-yyyy',now);
        str1 := fill_str(py.misc.name,31);
        str2 := fill_str(py.misc.title,31);
        str3 := fill_str(py.misc.tclass,31);
        str4 := 'Level : ' + inttostr(py.misc.lev);
        str4 := fill_str(str4,31);
        str5 := inttostr(py.misc.exp) + ' Exp';
        str5 := fill_str(str5,31);
        str6 := inttostr(py.misc.au) + ' Dollars';
        str6 := fill_str(str6,31);
        str7 := 'Died near ' + townlist[town_num].town_name;
        str7 := fill_str(str7,31);
        str8 := fill_str(died_from,31);
dstr[00] := ' ';
dstr[01] := '               _______________________';
dstr[02] := '              /                       \         ___';
dstr[03] := '             /                         \ ___   /   \      ___';
dstr[04] := '            /            RIP            \   \  |   |     /   \';
dstr[05] := '           /                             \  | _|___|_    |   |';
dstr[06] := '          /'+str1+                       '\_|_          _|___|_';
dstr[07] := '         |               the               |   ___';
dstr[08] := '         | '+str2+                       ' |  /   \';
dstr[09] := '         |                                 |  |   |';
dstr[10] := '         | '+str3+                       ' | _|___|_   ____';
dstr[11] := '         | '+str4+                       ' |          /    \';
dstr[12] := '         | '+str5+                       ' |          |    |';
dstr[13] := '         | '+str6+                       ' |          |    |';
dstr[14] := '         | '+str7+                       ' |         _|____|_';
dstr[15] := '         |            killed by            |';
dstr[16] := '         | '+str8+                       ' |';
dstr[17] := '         |           '+deathdate+'            |';
dstr[18] := '        *|   *     *     *    *   *     *  | *';
dstr[19] := '________)/\\_)_/___(\/___(//_\)/_\//__\\(/_|_)_______';
        clear(1,1);
        for i1 := 0 to 19 do
          prt(dstr[i1],i1+1,1);
        fnam := py.misc.name + '.txt';
        assign(f1,fnam);
        if total_winner_num > 0 then
          { Procedure kingly already created file. -MB }
          append(f1)
        else
          rewrite(f1);
        if IOResult = 0 then
          begin
            for i1 := 0 to 19 do
              writeln(f1,dstr[i1]);
            character_print
          end
        else
          begin
            writeln('Error in opening ',fnam);
            writeln('Character dump not saved.');
            readln
          end;
        close(f1)
      END;
 
 
 
{ Enters a players name on the top fifty list }
    procedure top_fifty(pts : longint);
    const top_scores = 50;
      var
        list        : array [1..top_scores] of hiscore_type;
        place       : byte;
        flag        : boolean; { set to true if player makes high score }
        f1          : text;
        player      : string;
        i2,n1       : integer;
        spc1        : string[2];
        spc2        : string[4];
        line        : string;
      BEGIN
        clrscr;
        clear(1,1);
        assign(f1,BOSS_TOP);
{ File opening loop removed. If first try fails next four are 
  likely to fail as well. -MB }
        reset(f1);
        if (IOResult <> 0) then
          BEGIN
            writeln('Error in opening ',BOSS_TOP);
            writeln('Please contact local BOSS Wizard.');
            readln;
            halt(0)
          END;
        n1 := 1;
        while ((not eof(f1)) and (n1 <= top_scores)) do
          BEGIN
            with list[n1] do
              readln(f1, playername, score, spc1, charactername, level,
                spc2, race, tclass);
            n1 := n1 + 1
          END;
        n1 := n1 - 1;                
        place := 1;
        flag := false;
        while ((place <= n1) and (not flag)) do
          BEGIN
            if (list[place].score < pts) then
              flag := true
            else
              inc(place)
          END;
        if flag then
          BEGIN
            close(f1);
            prt('Enter your name:', 1, 1);
            get_string(player, 1, 18, 12);
            rewrite(f1);
            for i2 := 1 to place - 1 do { unchanged places }
              with list[i2] do
                writeln(f1, padright(playername, 12), score:8, '  ',
                  padright(charactername, 25), level:3, '    ', 
                  padright(race, 13), tclass);

            writeln(f1, padright(player, 12), pts:8, '  ',
              padright(py.misc.name, 25), py.misc.lev:3, '    ',
              padright(py.misc.race, 13), py.misc.tclass);

            if n1 < top_scores then { still adding to full table }
              inc(n1);
            for i2 := place + 1 to n1 do
              with list[i2 - 1] do
                writeln(f1, padright(playername, 12), score:8, '  ',
                  padright(charactername, 25), level:3, '    ',
                  padright(race, 13), tclass);
          END;
        close(f1);    
        reset(f1);    
        clear(1,1);
	writeln;
	writeln;
        writeln('Username      Points  Character               Level   Quality       Class');
        for i2 := 1 to n1 do
          BEGIN
            readln(f1, line);
            writeln(line);
            if ((i2 mod 20) = 0) then
              begin
                msg_flag := true;
                msg_print('');
                clear(4,1)
              end
          END;
        close(f1);    
        writeln
      END;
 
 
{ Change the player into a King! }
    procedure kingly;
    var fnam : string;
        f1 : text;
        i1 : byte;

const crown_ascii_art : array[1..15] of string[50] =
(
        '                                  #              ',
        '                                #####            ',
        '                                  #              ',
        '                            ,,,  $$$  ,,,        ',
        '                        ,,=$   "$$$$$"   $=,,    ',
        '                      ,$$        $$$        $$,  ',
        '                      *>         <*>         <*  ',
        '                      $$         $$$         $$  ',
        '                      "$$        $$$        $$"  ', 
        '                       "$$       $$$       $$"   ',
        '                        *#########*#########*    ',
        '                        *#########*#########*    ',
        '                          Veni, Vidi, Vici!      ',
        '                      You have slain the BOSS!   ',
        '                        And saved the world!     '
);

      BEGIN
{ Change the character attributes.}
        dun_level := 0;
        died_from := 'Ripe Old Age';
        with py.misc do
          BEGIN 
            if lev <= max_player_level then
              lev := lev + max_player_level;
            max_exp := 1000000*total_winner_num + randint(1000);
            exp := max_exp
          END;
{ Let the player know that he did good.}
        clear(1,1);
        fnam := py.misc.name + '.txt';
        assign(f1,fnam);
        rewrite(f1);
        if IOResult = 0 then
          for i1 := 0 to 19 do
            writeln(f1,crown_ascii_art[i1]);
{ Deliberate silent fail. Procedure print_tomb will signal problems. -MB }
        close(f1);
        for i1 := 0 to 19 do
          writeln(crown_ascii_art[i1]);
        pause(24)
      END;
 
 
{ What happens upon dying...}
  BEGIN
    with py.misc do
      begin
        points := trunc(max_exp/expfact) +
           100*(max_lev + townlist[town_num].lev_plus) - principal;
        if total_winner_num > 0 then
          begin      
            kingly;
            lev := lev + max_player_level;
            points := points + 1000000*total_winner_num + 1000*(100-day);
            exp := max_exp
          end
      end;
    print_tomb;
    top_fifty(points);
    halt(0)
  END;

procedure time_out;
   
BEGIN
  clear(1,1);
  prt('Midnight, the bewitching hour.  It has been 100 days since you',1,3);
  prt('started your quest.  You have been busy, and so has the Boss.',2,3);
  prt('Sadly for you and the world you were supposed to save, the Boss',3,3);
  prt('has amassed enough power to take over the world.  He has seized',4,3);
  prt('control of all major world governments and the world economy.',5,3);
  prt('A simple pack of chewing gum now costs over 400 dollars, and',6,3);
  prt('there is only one brand on the market.  All over the world,',7,3);
  prt('people cry out in vain for freedom and brand name choices.',8,3);
  prt('You were their last hope, and you have let them down.  The Boss',9,3);
  prt('has vacated the Earth and now resides in an ultra-secure satel-',10,3);
  prt('lite orbiting in geosynchronous orbit over Sumatra.  You lose,',11,3);
  prt('although you may explore the Boss'' demense to your heart''s',12,3);
  prt('content.  But it will do absolutely no good at all.',13,3);
  pause(24);
  out_of_time := true;
  draw_cave
END;
