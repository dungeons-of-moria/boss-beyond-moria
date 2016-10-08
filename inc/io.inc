{ Convert an integer into a system bin time
  NOTE: Int_time is number of 1/100 seconds
        Max value = 5999}
[psect(io$code)] procedure convert_time(int_time : unsigned;
                                           var bin_time    : quad_type);
    type
        time_type = packed array [1..13] of char;
    var
        time_str  : time_type;
        secs,tics : unsigned;
        out_val   : varying[2] of char;
 
    [asynchronous,external(SYS$BINTIM)] function $bin_time(
                %stdescr give_str  : time_type;
                var slp_time  : quad_type ) : integer;
        external;
 
    BEGIN
      time_str := '0 00:00:00.00';
      bin_time.l0 := 0;
      bin_time.l1 := 0;
      tics := int_time mod 100;
      secs := int_time div 100;
      if (secs > 0) then
        BEGIN
          if (secs > 59) then secs := 59;
          writev(out_val,secs:2);
          time_str[10] := out_val[2];
          if (secs > 9) then time_str[9] := out_val[1];
        END;
      if (tics > 0) then
        BEGIN
          writev(out_val,tics:2);
          time_str[13] := out_val[2];
          if (tics > 9) then time_str[12] := out_val[1];
        END;
      $bin_time(time_str,bin_time);
    END;
 
 
{ Set timer for hibernation }
    [asynchronous,external(SYS$SETIMR)] function set_time(
        %immed efn: integer := %immed 5;
        var bintime     : quad_type;
        %ref astadr     : integer := %immed 0;
        %immed reqidt   : integer := %immed 0) : integer;
        external;
 
 
{ Hibernate }
    [asynchronous,external(SYS$WAITFR)] function hibernate(
        %immed efn: integer := %immed 5) : integer;
        external;
 
 
{ Sleep for given time
  NOTE: Int_time is in seconds }
[psect(io$code)] procedure sleep(int_time : unsigned);
    var
        bin_time  : quad_type;
    BEGIN
      convert_time(int_time*100,bin_time);
      set_time(bintime:=bin_time);
      hibernate;
    END;
 
[psect(io$code)] procedure tiny_sleep(int_time : unsigned);
    var
        bin_time  : quad_type;
    BEGIN
      convert_time(int_time,bin_time);
      set_time(bintime:=bin_time);
      hibernate;
    END;
 
{ Turns SYSPRV off if 0; on if 1;
  This is needed if image is installed with SYSPRV because user could write
  on system areas.  By turning the priv off system areas are secure }
[psect(io$code)] procedure priv_switch(switch_val : integer);
    type
        priv_field=     record  { Quad word needed for priv mask}
                          low   : unsigned;
                          high  : unsigned;
                        END;
    var
        priv_mask : priv_field;
 
{ Turn off SYSPRV }
    [external(SYS$SETPRV)] function $setprv(
        %immed enbflg   : integer := %immed 0;
        var privs : priv_field;
        %immed prmflg   : integer := %immed 0;
        %immed prvprv   : integer := %immed 0) : integer;
        external;
 
    BEGIN
      priv_mask.low  := %X'10000000';   { SYSPRV }
      priv_mask.high := %X'00000000';
      $setprv(enbflg:=switch_val,privs:=priv_mask);
    END;
    
    [external(LIB$DO_COMMAND)] function lib$do_command(
         %descr msg_str    : vtype) : integer; external;

{ Spawn a shell }
    [external(LIB$SPAWN)] function shell_out(
        command_str     : integer := %immed 0;
        input_file: integer := %immed 0;
        output_file     : integer := %immed 0;
        flags     : integer := %immed 0;
        process_name    : integer := %immed 0;
        process_id: integer := %immed 0;
        comp_status     : integer := %immed 0;
        comp_efn  : integer := %immed 0;
        comp_astadr     : integer := %immed 0;
        comp_astprm     : integer := %immed 0 ) : integer;
        external;
 
 
{ Turn off Control-Y }
[psect(io$code)] procedure no_controly;
    var
        bit_mask  : unsigned;
 
    [external(LIB$DISABLE_CTRL)] function y_off(
        var mask  : unsigned;
            old_mask    : integer := %immed 0) : integer;
        external;
 
    BEGIN
      bit_mask := %X'02000000'; { No Control-Y  }
      y_off(mask:=bit_mask);
    END;
 
 
{ Turn on Control-Y }
[psect(io$code)] procedure controly;
    var
        bit_mask  : unsigned;
 
    [external(LIB$ENABLE_CTRL)] function y_on(
        var mask  : unsigned;
            old_mask    : integer := %immed 0) : integer;
        external;
 
    BEGIN
      bit_mask := %X'02000000'; { Control-Y     }
      y_on(mask:=bit_mask);
    END;
 
 
{ Dump IO to buffer
  NOTE: Source is PUTQIO.MAR }
  procedure put_buffer          (
                %ref    out_str : varying [a] of char;
                %immed  row : integer;
                %immed  col : integer );
        external;
 
 
{ Dump the IO buffer to terminal
  NOTE: Source is PUTQIO.MAR }
  procedure put_qio;
        external;
 
 
[psect(io$code)] procedure exit;
 
{ Immediate exit from program }
  [external(SYS$EXIT)] function $exit(
        %immed status : integer := %immed 1) : integer;
        external;
 
      BEGIN
        controly; { Turn control-Y back on }
        put_qio;  { Dump any remaining buffer }
        $exit;    { exit from game }
      END;
 
 
{ Initializes I/O channel for use with INKEY }
[psect(io$code)] procedure init_channel;
    type
      ttype = packed array [1..3] of char;
    var
      status: integer;
      terminal    : ttype;
 
  [external(SYS$ASSIGN)] function assign(
        %stdescr terminal : ttype;
        var channel : [volatile] integer;
        acmode: integer := %immed 0;
        mbxnam: integer := %immed 0) : integer;
        external;
 
    BEGIN
      terminal := 'TT:';
      status := assign(terminal,channel);
      if (not odd(status)) then
        BEGIN
          writeln('Channel could not be assigned <Status - ',status:4,'>');
          exit;
        END
    END;
 
 
{ QIOW definition }
  [asynchronous,external(SYS$QIOW)] function qiow_read(
        %immed efn  : integer := %immed 1;
        %immed chan : integer;
        %immed func : integer := %immed 0;
        %immed isob : integer := %immed 0;
        %immed astadr     : integer := %immed 0;
        %immed astprm     : integer := %immed 0;
        %ref get_char     : [unsafe] char := %immed 0;
        %immed buff_len   : integer := %immed 0;
        %immed delay_time : integer := %immed 0;
        %immed p4   : integer := %immed 0;
        %immed p5   : integer := %immed 0;
        %immed p6   : integer := %immed 0) : integer;
        external;
 
{ Gets single character from keyboard and returns }
[psect(io$code)] procedure inkey(var getchar : char);
    var
        status: integer;
    BEGIN
      put_qio;  { Dump IO buffer
Allow device driver to catch up
NOTE: Remove or comment out for VMS 4.0 or greater
      set_time(bintime:=IO$BIN_PAUSE);
      hibernate;
Now read }
      qiow_read(chan:=channel,
                func:=IO$MOR_INPUT,
                get_char:=getchar,
                buff_len:=1     );
      msg_flag := false;
    END;
 
{ Gets single character from keyboard and returns }
[psect(io$code)] procedure inkey_delay  (
                        var getchar : char;
                        delay : integer );
    var
        status: integer;
    BEGIN
      put_qio;  { Dump the IO buffer
Allow device driver to catch up
NOTE: Remove or comment out for VMS 4.0 or greater
      set_time(bintime:=IO$BIN_PAUSE);
      hibernate;
Now read }
      getchar := null;          { Blank out return character    }
      qiow_read(chan:=channel,
                func:=IO$MOR_DELAY,
                get_char:=getchar,
                buff_len:=1,
                delay_time:=delay );
    END;
 
 
{ Flush the buffer }
[psect(io$code)] procedure flush;
    BEGIN
{ Allow device driver to catch up
  NOTE: Remove or comment out for VMS 4.0 or greater
      set_time(bintime:=IO$BIN_PAUSE);
      hibernate;
Now flush }
      qiow_read(chan:=channel,func:=IO$MOR_IPURGE);
    END;
 
 
{ Flush buffer before input }
[psect(io$code)] procedure inkey_flush(var x : char);
    BEGIN
      put_qio;  { Dump the IO buffer     }
      if (not(wizard)) then flush;
      inkey(x);
    END;
 
 
{ Retrieves foreign string input with game command }
  [external(LIB$GET_FOREIGN)] procedure get_foreign(
        %descr msg_str    : vtype;
        %descr prompt     : vtype   := %immed 0;
        %ref len    : integer := %immed 0);
        external;
 
 
{ Clears given line of text }
[psect(io$code)] procedure erase_line ( row,col : integer );
    BEGIN
      put_buffer(cursor_erl,row,col);
    END;
 
 
{ Clears screen at given row, column }
[psect(io$code)] procedure clear(row,col : integer);
    var
        i1    : integer;
    BEGIN
      for i1 := 2 to 23 do used_line[i1] := false;
      put_buffer(cursor_erp,row,col);
      put_qio;  { Dump the Clear Sequence       }
    END;
 
 
{ Outputs a line to a given interpolated y,x position }
[psect(io$code)] procedure print(
                str_buff  : varying[a] of char;
                row : integer;
                col : integer );
    BEGIN
      row := row - panel_row_prt;{ Real co-ords convert to screen positions }
      col := col - panel_col_prt;
      used_line[row] := true;
      put_buffer(str_buff,row,col)
    END;
 
 
{ Outputs a line to a given y,x position }
[psect(io$code)] procedure prt(
                str_buff  : varying[a] of char;
                row : integer;
                col : integer );
    BEGIN
      put_buffer(cursor_erl+str_buff,row,col);
    END;

 
{ Outputs message to top line of screen }
[psect(io$code)] procedure msg_print(str_buff   : varying[a] of char);
    var
        old_len   : integer;
        in_char   : char;
	out_val	  : vtype;
    BEGIN
      if (msg_flag and more_flag) then
        BEGIN
          old_len := length(old_msg) + 1;
          put_buffer(' -more-',msg_line,old_len);
          repeat
            inkey(in_char);
          until (ord(in_char) in [3,13,25,26,27,32]);
        END;
      put_buffer(cursor_erl+str_buff,msg_line,msg_line);
      old_msg := str_buff;
      msg_flag := true;
    END;
 
 
{ Prompts (optional) and returns ord value of input char
  Function returns false if <ESCAPE>,CNTL/(Y,C,Z) is input }
[psect(io$code)] function get_com (
                                prompt : varying[a] of char;
                                var command : char ) : boolean;
    var
        com_val   : integer;
    BEGIN
      if (length(prompt) > 1) then prt(prompt,1,1);
      inkey(command);
      com_val := ord(command);
      CASE com_val of
        3,25,26,27: get_com := false;
        otherwise         get_com := true;
      END;
      erase_line(msg_line,msg_line);
      msg_flag := false;
    END;
 
 
{ Gets a string terminated by <RETURN>
  Function returns false if <ESCAPE>,CNTL/(Y,C,Z) is input }
[psect(io$code)] function get_string    (
                        var in_str: varying[a] of char;
                        row,column,slen : integer ) : boolean;
    var
        start_col,END_col,i1    : integer;
        x     : char;
        tmp   : vtype;
        flag,abort  : boolean;
 
    BEGIN
      abort := false;
      flag  := false;
      in_str:= '';
      put_buffer(pad(in_str,' ',slen),row,column);
      put_buffer('',row,column);
      start_col := column;
      END_col := column + slen - 1;
      repeat
        inkey(x);
        CASE ord(x) of
         3,25,26,27 :   abort := true;
                13  :   flag  := true;
                127 :   BEGIN
                          if (column > start_col) then
                            BEGIN
                              column := column - 1;
                              put_buffer(' '+chr(8),row,column);
                              in_str := substr(in_str,1,length(in_str)-1);
                            END;
                        END;
         otherwise      BEGIN
                          tmp := x;
                          put_buffer(tmp,row,column);
                          in_str := in_str + tmp;
                          column := column + 1;
                          if (column > END_col) then
                            flag := true;
                        END;
        END;
      until (flag or abort);
      if (abort) then
        get_string := false
      else
        BEGIN                   { Remove trailing blanks }
          i1 := length(in_str);
          if (i1 > 1) then
            BEGIN
              while ((in_str[i1] = ' ') and (i1 > 1)) do
                i1 := i1 - 1;
                in_str := substr(in_str,1,i1);
            END;
          get_string := true;
        END;
    END;
 
 
{ Return integer value of hex string }
[psect(io$code)] function get_hex_value(row,col,slen : integer) : integer;
    type
        pack_type  = packed array [1..9] of char;
    var
        bin_val : integer;
        tmp_str : vtype;
        pack_str : pack_type;
 
    [asynchronous,external(OTS$CVT_TZ_L)] function convert_hex_to_bin(
                %stdescr hex_str  : pack_type;
                %ref     hex_val  : integer;
                %immed   val_size : integer := %immed 4;
                %immed   flags    : integer := %immed 1) : integer;
                external;
 
    BEGIN
      bin_val := 0;
      get_hex_value := 0;
      if (get_string(tmp_str,row,col,slen)) then
        if (length(tmp_str) <= 8) then
          BEGIN
            pack_str := pad(tmp_str,' ',9);
            if (odd(convert_hex_to_bin(pack_str,bin_val))) then
              get_hex_value := bin_val;
          END;
    END;
 
 
 
{ Pauses for user response before returning }
[psect(io$code)] procedure pause(prt_line : integer);
    var
        dummy : char;
    BEGIN
      prt('[Press any key to continue]',prt_line,24);
      inkey(dummy);
      erase_line(prt_line,1);
    END;
 
 
[psect(io$code)] procedure pause_exit(prt_line : integer);
    var
        dummy : char;
    BEGIN
      prt('[Press any key to continue, or <Control>-Z to exit]',prt_line,11);
      inkey(dummy);
      CASE ord(dummy) of
        3,25,26 :  Begin
                     erase_line(prt_line,1);
                     exit;
                   End;
        otherwise;
      END;
      erase_line(prt_line,1);
    END;
 
{ Prints a line to the screen, time efficient }
[psect(io$code)] procedure dprint(str : vtype; row : integer);
      var
        i1,i2,nblanks,xpos : integer;
        prt_str : vtype;
      BEGIN
        prt_str := '';
        nblanks := 0;
        xpos := 0;
        for i1 := 1 to length(str) do
          BEGIN
            if (str[i1] = ' ') then
              BEGIN
                if (xpos > 0) then
                  BEGIN
                    nblanks := nblanks + 1;
                    if (nblanks > 5) then
                      BEGIN
                        nblanks := 0;
                        put_buffer(prt_str,row,xpos);
                        prt_str := '';
                        xpos := 0;
                      END
                  END;
              END
            else
              BEGIN
                if (xpos = 0) then xpos := i1;
                if (nblanks > 0) then
                  BEGIN
                    for i2 := 1 to nblanks do
                      prt_str := prt_str + ' ';
                    nblanks := 0;
                  END;
                prt_str := prt_str + str[i1];
              END;
          END;
        if (xpos > 0) then
          put_buffer(prt_str,row,xpos);
      END;

 
{ Returns the image path for BOSS
 Path is returned in a VARYING[80] of char }
[psect(io$code)] procedure get_paths;
    type
        word    = 0..65535;
        rec_jpi = record
                        pathinfo : packed record
                                     pathlen: word;
                                     jpi$_imagname: word;
                                   END;
                        ptr_path  : ^path;
                        ptr_pathlen     : ^integer;
                        ENDlist   : integer
                  END;
        path            = packed array [1..128] of char;
    var
        i1  : integer;
        tmp_str   : path;
        image_path: vtype;
        flag: boolean;
 
{ Call JPI and return the image path as a packed 128 }
    function get_jpi_path : path;
      var
        status    : integer;
        user: path;
        jpirec    : rec_jpi;
 
{ GETJPI definition }
      [asynchronous,external(SYS$GETJPI)] function $getjpi(
                %immed  p1: integer := %immed 0;
                %immed  p2: integer := %immed 0;
                %immed  p3: integer := %immed 0;
                var     itmlst  : rec_jpi;
                %immed  p4: integer := %immed 0;
                %immed  p5: integer := %immed 0;
                %immed  p6: integer := %immed 0) : integer;
                external;
 
      BEGIN
        with jpirec do
          BEGIN
            pathinfo.pathlen:= 128;         { Image length  }
            pathinfo.jpi$_imagname:= %x207;       { Image path    }
            new (ptr_path);
            new (ptr_pathlen);
            ptr_pathlen^    := 0;
            ENDlist   := 0;
          END;
        status := $getjpi(itmlst:=jpirec);
        if (not(odd(status))) then
          BEGIN
            clear(1,1);
            put_buffer('Error in retrieving image path.',1,1);
            exit;
          END
        else
          get_jpi_path := jpirec.ptr_path^;
      END;
 
    BEGIN
      tmp_str := get_jpi_path;
      i1 := 0;
      flag := false;
      image_path := '';
      repeat
        i1 := i1 + 1;
        if (tmp_str[i1] = ']') then 
          if (tmp_str[i1+1] = '[') then
            i1 := i1 + 2
          else
   	    begin
   	      flag := true;
   	      image_path := image_path + '.DAT';	
   	    end;
        image_path := image_path + tmp_str[i1];
      until(flag);
      BOSS_HOU := image_path + 'HOURS.DAT';
      BOSS_MOR := image_path + 'MESSAGE.DAT';
      BOSS_MAS := image_path + 'BOSSCHR.DAT';
      BOSS_TOP := image_path + 'BOSSTOP.DAT';
      BOSS_HLP := image_path + 'BOSSHLP.HLB';
      BOSS_MON := image_path + 'MONSTERS.DAT';
      BOSS_OBJ := image_path + 'OBJECTS.DAT';
      BOSS_INV := image_path + 'INVENT.DAT';
      BOSS_LOS := image_path + 'LOSER.DAT';
      BOSS_ANNOY:=image_path + 'PUTZS.DAT';
      BOSS_WIZ := image_path + 'WIZARD.DAT';
      BUS_PIC  := image_path + 'BUS.DAT';
      QUOTES   := image_path + 'QUOTES.DAT';
      SKILLS   := image_path + 'SKILLS.DAT';
    END;
