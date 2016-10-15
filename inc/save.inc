{ Data Corruption means character is dead, or save file was screwed with.
  Keep them guessing as to what is actually wrong.}
procedure data_exception;
      begin
        clear(1,1);
        prt('%BOSS-F-ATTREUSE,Attempted Reuse of a Character',1,1);
        prt('',2,1);
        halt(0) 
      end;

procedure data_exception1;
      begin
        clear(1,1);
        prt('%BOSS-F-BADCHECK,Data Exceptyion Error,Corruption',1,1);
        prt('',2,1);
        halt(0) 
      end;

{ Actual save procedure }
function save_char : boolean;
      var
        tot_monsters,tot_treasure     : integer;
        i1,i2{,tries}                 : integer;
        xfloor                        : unsigned;
        fnam,saveline                 : vtype;
        out_rec                       : ntype;
        f1                            : text;
        c                             : char;

  { Boolean saving is not so easy in Free Pascal. -MB }
  procedure save_booleans(list : array of boolean);
  var
    i : integer;
  begin
  for i := 0 to high(list) do
    write(f1, ord(list[i]), ' ');
  writeln(f1)
  end;
 
begin
  save_char := true;
  fnam := py.misc.name + '.sav';
  assign(f1,fnam);
          { Check if filename already exists. Prompt to overwrite. }
  reset(f1);
  if IOResult = 0 then
    begin
    prt('File with same name character name exists. Overwrite? [y/n]',1,1);
    repeat
      inkey(c);
      c := upcase(c)
    until (c = 'Y') or (c = 'N');
    if c = 'N' then
      begin
      erase_line(1,1);
      prt('Please change character name by pressing ''C'', ''c'' and try again.',1,1);
      close(f1);
      exit(false)
      end
    end;
  rewrite(f1);
  if IOResult <> 0 then
    begin
    msg_print('Error creating> ' + fnam);
    exit(false)
    end;
{ Master file shall be unused in ported version of BOSS. -MB }
{ Encryption is also dropped. It is easier to encrypt file afterwards but
nonethless I find it pointless. -MB }
          { Message to player on what is happening}
      clear(1,1);
      saveline := 'Saving character as ' + fnam + '...'; 
      prt(saveline,1,1);

          { Write out the player record.  }
      with py.misc do
        begin
        writeln(f1,name);
        writeln(f1,race);
        writeln(f1,sex);
        writeln(f1,tclass);
        writeln(f1,title);

        for i1 := 1 to 5 do
          writeln(f1,history[i1]);

        writeln(f1,char_row,' ',char_col,' ',
          pclass,' ',prace,' ', pskill, ' ',
          age,' ',ht,' ',wt,' ',sc,' ',max_exp,' ',
          exp,' ',lev,' ',max_lev,' ',expfact:2:1);

        writeln(f1,srh,' ',perc,' ',stl,' ',bth,' ',
          bthb,' ',mana,' ',cmana,' ',mhp,' ',
          chp:1:1,' ',au,' ',ptohit,' ',ptodam,' ',
          pac,' ',ptoac,' ',dis_th,' ',dis_td,' ',
          dis_ac,' ',dis_tac,' ',disarm,' ',
          wierd,' ',save,' ',hitdie)
        end;

      writeln(f1,inven_ctr,' ',
        inven_weight,' ',equip_ctr,' ',dun_level,' ',
        missle_ctr,' ',mon_tot_mult,' ',turn,' ',day,
                    ' ',town_num,' ',ord(defeated_badguy));

          {Write out the kill list}
      for i1 := 1 to 400 do
        writeln(f1,kill_list[i1]);

          {Write out bank records} 
      writeln(f1,act_bal,' ',principal,' ',payment_count,
            ' ',iod_val:2:1,' ',iol_val:2:1,' ',danny_cut:2:1);

          { Write out the inventory records.      }
      for i1 := 1 to inven_ctr do
        begin
          writeln(f1, inventory[i1].tchar + inventory[i1].name);

          writeln(f1, inventory[i1].damage);

          with inventory[i1] do
            writeln(f1,tval,' ',subval,' ',weight,' ',
              number,' ',tohit,' ',todam,' ',ac,' ',
              toac,' ',p1,' ',flags,' ',level,' ',cost)
        end;

          { Write out the equipment records.      }
      for i1 := 23 to equip_max do
        begin
          writeln(f1, equipment[i1].tchar + equipment[i1].name);

          writeln(f1, equipment[i1].damage);

          with equipment[i1] do
            writeln(f1,tval,' ',subval,' ',weight,' ',
              number,' ',tohit,' ',todam,' ',ac,' ',
              toac,' ',p1,' ',flags,' ',level,' ',cost)
        end;

      with py.stat do
        writeln(f1,str,' ',cstr,' ',dex,' ',cdex,' ',
          con,' ',ccon,' ',int,' ',cint,' ',wis,' ',
          cwis,' ',chr,' ',cchr);

      with py.flags do
        begin
          writeln(f1,status,' ',blind,' ',confused,' ',
                  food,' ',food_digested,' ',protection,' ',
                  speed,' ',afraid,' ',poisoned);

          writeln(f1,fast,' ',slow,' ',protevil);
          save_booleans([see_inv, teleport, free_act, slow_digest]);

          save_booleans([aggravate, sustain_str, sustain_int,
            sustain_wis, sustain_con, sustain_dex, sustain_chr]);

          save_booleans([fire_resist, cold_resist, acid_resist, 
            regenerate, lght_resist, ffall, confuse_monster]);

          writeln(f1,image,' ',invuln,' ',hero,' ',
                  shero,' ',blessed,' ',resist_heat,' ',
                  resist_cold,' ',ring_blast,' ',special,' ',
                  petrified,' ',diseased,' ',detect_inv,' ',
                  word_recall,' ',see_infra,' ',tim_infra)
        end;

      for i1 := 1 to max_skills do
        with skill_list[i1] do
          begin
            writeln(f1,sname);

            writeln(f1,slevel,' ',smana,' ',
                   sexp,' ',sfail,' ',ord(learned))
          end;

     { Write the important dungeon info and floor    -RAK-   }
      begin
        writeln(f1,cur_height,' ',cur_width,' ',
          max_panel_rows,' ',max_panel_cols);

          { Save the floor        }
        tot_treasure := 0;
        for i1 := 1 to cur_height do
          begin
            out_rec := addchar(' ','',cur_width);
            for i2 := 1 to cur_width do
              begin
                with cave[i1,i2] do
                  begin
                    xfloor := fval;
                    if (fopen) then
                      xfloor := xfloor or $10;
                    if (pl) then
                      xfloor := xfloor or $20;
                    if (fm) then
                      xfloor := xfloor or $40;
                    { TODO: instead of saving raw bytes output their actual
                      decimal values to keep saves in pure text format }
                    out_rec[i2] := chr(xfloor);
                    if (tptr > 0) then
                      tot_treasure := tot_treasure + 1;
                  end;
              end;
            writeln(f1,out_rec)
          end;

          { Save the Treasure List                }
        writeln(f1,tot_treasure);
        for i1 := 1 to cur_height do
          for i2 := 1 to cur_width do
            if (cave[i1,i2].tptr > 0) then
              with t_list[cave[i1,i2].tptr] do
                begin
                  writeln(f1,i1,' ',i2);

                  writeln(f1,tchar + name);

                  writeln(f1,damage);

                  writeln(f1,tval,' ',subval,' ',weight,' ',
                    number,' ',tohit,' ',todam,' ',ac,' ',
                    toac,' ',p1,' ',flags,' ',level,' ',cost)
                end;

          { Save identified list                  }
        out_rec := '';
        for i1 := 1 to max_objects do
          begin
            if (object_ident[i1]) then
              out_rec := out_rec + 'T'
            else
              out_rec := out_rec + 'F';
          end;
        writeln(f1,out_rec);

          { Save the Monster List                 }
        i1 := muptr;
        tot_monsters := 0;
        if (i1 > 0) then
          repeat
            tot_monsters := tot_monsters + 1;
            with m_list[i1] do
              i1 := nptr;
          until (i1 = 0);
        writeln(f1,tot_monsters);
        i1 := muptr;
        if (i1 > 0) then
          repeat
            with m_list[i1] do
              begin
                writeln(f1,fy,' ',fx,' ',mptr,' ',hp,
                  ' ',cspeed,' ',csleep,' ',cdis,' ',ord(ml),
                  ' ',ord(confused),' ',cur_reprod);
                i1 := nptr;
              end;
          until (i1 = 0);

          { Save the town level stores            }
        for i1 := 1 to max_stores do
          with store[i1] do
            begin
          { Save items...                 }
              writeln(f1,store_ctr);
              for i2 := 1 to store_ctr do
                with store_inven[i2].sitem do
                  begin
                    writeln(f1,store_inven[i2].scost);
                    writeln(f1,tchar + name);
                    writeln(f1,damage);
                    writeln(f1,tval,' ',subval,' ',weight,
                          ' ',number,' ',tohit,' ',todam,' ',
                          ac,' ',toac,' ',p1,' ',flags,' ',
                          level,' ',cost)
                  end;

              writeln(f1,owner,' ',insult_cur,' ',store_open)
            end;
      end;
      close(f1);
      { save file version is not reported -MB }
      prt('Character saved.',1,1);
      writeln;
      halt(0)
end;
 
 
        { Restore a saved game                          -RAK- & -JWT-   }
function get_char(fnam : vtype) : boolean;
      var
        tot_treasures,tot_monsters : integer;
        i1,i2,i3,i4                : integer;
        xfloor                     : unsigned;
        in_rec                     : ntype;
        in_chr                     : char;
        f1                         : text;
        dun_flag                   : boolean;
        bool_tmp1, bool_tmp2       : byte;
 

  { Boolean saving is not so easy in Free Pascal. -MB }
  procedure load_booleans(list : array of pboolean);
  var
    i : integer;
    tmp : byte;
  begin
  for i := 0 to high(list)-1 do
    begin
    read(f1, tmp);
    list[i]^ := boolean(tmp)
    end;
  readln(f1, tmp);
  list[high(list)]^ := boolean(tmp)
  end; 

      BEGIN
        dun_flag := false;
        clear(1,1);
        assign(f1,fnam);
        reset(f1);
        if IOResult <> 0 then
          begin
            prt('Error Opening> '+fnam,1,1);
            prt('',2,1);
            halt(0)
          end;
 
 { Since by porting I aldready broke save file compatibility update
   procedures got deleted. -MB }

        with py.misc do
          begin
          readln(f1,name);
          readln(f1,race);
          readln(f1,sex);
          readln(f1,tclass);
          readln(f1,title)
          end;
 
        for i1 := 1 to 5 do
          readln(f1,py.misc.history[i1]);
 
        with py.misc do
          readln(f1,char_row,char_col,pclass,prace,pskill,age,ht,wt,sc,
            max_exp,exp,lev,max_lev,expfact);

        with py.misc do
          readln(f1,srh,perc,stl,bth,bthb,mana,cmana,mhp,chp,au,
            ptohit,ptodam,pac,ptoac,dis_th,dis_td,dis_ac,dis_tac,
            disarm,wierd,save,hitdie); 

        readln(f1,inven_ctr,inven_weight,equip_ctr,dun_level,missle_ctr,
          mon_tot_mult,turn,day,town_num,bool_tmp1);
        defeated_badguy := boolean(bool_tmp1);

        for i1 := 1 to 400 do
          readln(f1,kill_list[i1]);

	  {Read in Bank Variables}
        readln(f1,act_bal,principal,payment_count,iod_val,iol_val,
          danny_cut);

        { Read in the inventory records.  }
        for i1 := 1 to inven_ctr do
          begin
            readln(f1,inventory[i1].tchar,inventory[i1].name);
            readln(f1,inventory[i1].damage);
            with inventory[i1] do
              readln(f1,tval,subval,weight,number,tohit,todam,ac,
                toac,p1,flags,level,cost)
          end;
 
        { Read in the equipment records. }
        for i1 := 23 to equip_max do
          begin
            readln(f1,equipment[i1].tchar,equipment[i1].name);
            readln(f1,equipment[i1].damage);
            with equipment[i1] do
              readln(f1,tval,subval,weight,number,tohit,todam,ac,
                toac,p1,flags,level,cost)
          end;
 
        with py.stat do
          readln(f1,str,cstr,dex,cdex,con,ccon,int,cint,wis,cwis,chr,cchr);
 
        with py.flags do
          begin
            readln(f1,status,blind,confused,food,food_digested,protection,
              speed,afraid,poisoned);
            readln(f1,fast,slow,protevil);
            load_booleans([@see_inv,@teleport,@free_act,@slow_digest]);
            load_booleans([@aggravate,@sustain_str,@sustain_int,@sustain_wis,
              @sustain_con,@sustain_dex,@sustain_chr]);
            load_booleans([@fire_resist,@cold_resist,@acid_resist,@regenerate,
              @lght_resist,@ffall,@confuse_monster]);
            readln(f1,image,invuln,hero,shero,blessed,resist_heat,
              resist_cold,ring_blast,special,petrified,diseased,
              detect_inv,word_recall,see_infra,tim_infra)
          end;
 
	  for i1 := 1 to max_skills do
	    with skill_list[i1] do
	      begin
	        readln(f1,sname);
		readln(f1,slevel,smana,sexp,sfail,bool_tmp1);
                learned := boolean(bool_tmp1)
	      end;

                { Read the important dungeon info and floor     }
	    writeln('dung info');
            readln(f1,cur_height,cur_width,max_panel_rows,max_panel_cols);
 
                { Restore the floor     }
	    writeln('floor part');
            for i1 := 1 to cur_height do
              begin
                in_rec := '';
                for i2 := 1 to cur_width do
                  begin
                    read(f1,in_chr);
                    in_rec := in_rec + in_chr;
                  end;
                readln(f1);
                for i2 := 1 to cur_width do
                  begin
                    xfloor := ord(in_rec[i2]);
                    with cave[i1,i2] do
                      begin
                        fval := $0F and xfloor;
                        if (($10 and xfloor) <> 0) then
                          fopen := true;
                        if (($20 and xfloor) <> 0) then
                          pl := true;
                        if (($40 and xfloor) <> 0) then
                          fm := true;
                        tl := false;
                        tptr := 0;
                        cptr := 0;
                      end;
                  end;
              end;
 
                { Restore the Treasure List             }
            tlink;
            readln(f1,tot_treasures);
            for i1 := 1 to tot_treasures do
              begin
                popt(i2);
                with t_list[i2] do
                  begin
                    readln(f1,i3,i4);
                    cave[i3,i4].tptr := i2;
                    readln(f1,tchar,name);
                    readln(f1,damage);
                    readln(f1,tval,subval,weight,number,tohit,todam,ac,toac,
                      p1,flags,level,cost)
                  end;
              end;
 
                { Re-identify objects                   }
            readln(f1,in_rec);
            for i1 := 1 to max_objects do
              if (in_rec[i1] = 'T') then
                identify(object_list[i1])
              else
                object_ident[i1] := false;
 
                { Restore the Monster List              }
            mlink;
            readln(f1,tot_monsters);
            i3 := 0;
            for i1 := 1 to tot_monsters do
              begin
                popm(i2);
                with m_list[i2] do
                  begin
                    readln(f1,fy,fx,mptr,hp,cspeed,
                      csleep,cdis,bool_tmp1,bool_tmp2,cur_reprod);
                    ml := boolean(bool_tmp1);
                    confused := boolean(bool_tmp2);
                    cave[fy,fx].cptr := i2;
                    if (muptr = 0) then
                      muptr := i2
                    else
                      m_list[i3].nptr := i2;
                    nptr := 0;
                    i3 := i2;
                  end;
              end;
 
                { Restore the town level stores         }
            for i1 := 1 to max_stores do
              with store[i1] do
                begin
                  readln(f1,i2);
                  store_ctr := i2;
                  for i3 := 1 to i2 do
                    with store_inven[i3].sitem do
                      begin
                        readln(f1,store_inven[i3].scost);
                        readln(f1,tchar,name);
                        readln(f1,damage);
                        readln(f1,tval,subval,weight,number,tohit,todam,
                          ac,toac,p1,flags,level,cost)
                      end;
                  readln(f1,owner,insult_cur,store_open)
                end;
        close(f1);

    {Should the save file be deleted when the character is restored?}
        erase(f1);

        get_char:=dun_flag;  {If get_char is true, then moria_flag is set true}
	if (dun_flag) then
	  begin
	    store_init;
	    store_maint;
	  end;
	if (day >= day_limit) then 
	  out_of_time := true;
      END;

{ This aims to dispense with command line game loading. -MB }
function load_game(var filename : string) : boolean;
const games_on_page = 20;
var games          : array[1..games_on_page + 1] of string[80];
    search         : TSearchRec;
    count          : cardinal;
    page           : cardinal;
    reset_find     : boolean;
    choice_made    : boolean;
    save_slot      : cardinal;
    choice         : char;
    valid          : boolean;
begin
count := 0;
page := 0;
reset_find := true;
choice_made := false;
while not choice_made do
  begin
  clear(1,1);
  { Populate games array. See if there is at least one. }
  if reset_find = true then
    begin
    if findfirst('*.sav', faanyfile, search) = 0 then
      begin
      reset_find := false;
      inc(count);
      games[1] := copy(search.name, 1, length(search.name) - 4)
      end
    else
    { No saved games. Go straight to character generation. }
      if count = 0 then
        begin
        load_game := false;
        break
        end
    end;
  while (count <= games_on_page) and (findnext(search) = 0) do
    begin
    inc(count);
    games[count] := copy(search.name, 1, length(search.name) - 4)
    end;
  if count > games_on_page then
    inc(page);
  { Display options. }
  for save_slot := 1 to count do
    if save_slot <= games_on_page then
      prt(chr(ord('A') + save_slot - 1) + ': ' + games[save_slot], save_slot, 1);
  if page > 0 then
    if count > games_on_page then
      prt('[A - ' + chr(ord('A') + games_on_page - 1) + '] load game    [control-Z] cancel    [space] next page', games_on_page + 1, 1)
    else
      prt('[A - ' + chr(ord('A') + count - 1) + '] load game    [control-Z] cancel    [space] next page', games_on_page + 1, 1)
  else
    prt('[A - ' + chr(ord('A') + count - 1) + '] load game    [control-Z] cancel', games_on_page + 1, 1);
  { Process choices. }
  valid := false;
  repeat
    inkey(choice);
    choice := upcase(choice);
    if choice = #26 then
      valid := true;
    if (page > 0) then
      begin
      if choice = ' ' then
        valid := true;
      if (choice >= 'A') and (choice <= chr(ord('A') + games_on_page - 1)) then
        valid := true
      end
    else
      if (choice >= 'A') and (choice <= chr(ord('A') + count - 1)) then
        valid := true
  until valid = true;
  { Player decides to quit. }
  if choice = #26 then
    begin
    load_game := false;
    break
    end;
  { Possibly view next page of saved games. }
  if choice = ' ' then
    if count > games_on_page then
      begin
      count := count - games_on_page;
      games[count] := games[count + games_on_page]
      end
    { Seen all of possible saved games. Go back to first page. }
    else
      begin
      page := 0;
      count := 0;
      reset_find := true;
      findclose(search)
      end;
  { Game selected. }
  if (choice >= 'A') and (choice <= chr(ord('A') + count - 1)) then
    begin
    choice_made := true;
    load_game := true;
    filename := games[ord(choice) + 1 - ord('A')] + '.sav'
    end
  end;
findclose(search)
end;
