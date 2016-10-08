[psect(moria$code)] procedure termdef;
  type
        term_type       =       packed array [1..3] of char;
        dvi_type        =       record
                item_len,
                item_code       : wordint;
                buff_add,
                len_add         : ^integer;
                end_item        : integer
        end;
  var
        dvi_buff        : dvi_type;
        i1              : integer;
        tmp_str         : varying[10] of char;
        tmp,
        escape          : char;
 
  [external(SYS$GETDVI)] function get_dvi       (
                 efn,
                 chan           : integer := %immed 0;
        %stdescr terminal       : term_type;
        %ref     itmlst         : dvi_type;
                 isob,
                 astadr,
                 astprm,
                 undefined      : integer := %immed 0
                                                ) : integer;
        external;
 
  begin
    escape := chr(27);
    with dvi_buff do
      begin
        item_len  := 4;
        item_code := 6;
        new(buff_add);
        new(len_add);
        end_item  := 0
      end;
    get_dvi(terminal:='TT:',itmlst:=dvi_buff);
        { Add new terminals in this case statement.  The case number is }
        { returned by SYS$GETVI.  Terminals are either row then col, or }
        { col then row.                                                 }
        {   ROW_FIRST should be true if the row is given first.         }
        {   CURSOR_ERL is the sequence for erase-to-end-of-line.        }
        {   CURSOR_ERP is the sequence for erase-to-end-of-page.        }
        {   CURLEN_R is the length of the ROW portion of cursor address }
        {   CURLEN_C is the length of the COL portion of cursor address }
        {   CURLEN_L is CURLEN_R + CURLEN_C                             }
        {   CURSOR_R is the ROW cursor portion characters               }
        {   CURSOR_C is the COL cursor portion characters               }
    case dvi_buff.buff_add^ of
        17 :    { ADM-3A (/FT2)                 }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := chr(24);
                  cursor_erp := chr(23);
                  curlen_r   := 3;
                  curlen_c   := 1;
                  cursor_l   := 4;
                  for i1 := 1 to 24 do
                    begin
                      tmp := chr(i1+31);                        { Row char}
                      cursor_r[i1] := escape + '=' + tmp        { Row part}
                    end;
                  for i1 := 1 to 80 do
                    begin
                      tmp := chr(i1+31);                        { Col char}
                      cursor_c[i1] := tmp                       { Col part}
                    end
                end;
        18 :    { ADDS100 (/FT3)                        }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + 'K';
                  cursor_erp := escape + 'k';
                  curlen_r   := 3;
                  curlen_c   := 1;
                  cursor_l   := 4;
                  for i1 := 1 to 24 do
                    begin
                      tmp := chr(i1+31);                        { Row char}
                      cursor_r[i1] := escape + 'Y' + tmp        { Row part}
                    end;
                  for i1 := 1 to 80 do
                    begin
                      tmp := chr(i1+31);                        { Col char}
                      cursor_c[i1] := tmp                       { Col part}
                    end
                end;
        19 :    { IBM3101 (/FT4)                        }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + 'I';
                  cursor_erp := escape + 'J';
                  curlen_r   := 3;
                  curlen_c   := 1;
                  cursor_l   := 4;
                  for i1 := 1 to 24 do
                    begin
                      tmp := chr(i1+39);                        { Row char}
                      cursor_r[i1] := escape + 'Y' + tmp        { Row part}
                    end;
                  for i1 := 1 to 80 do
                    begin
                      tmp := chr(i1+39);                        { Col char}
                      cursor_c[i1] := tmp                       { Col part}
                    end
                end;
        16 :    { Teleray 10 (/FT1)                     }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + 'K';
                  cursor_erp := escape + 'J';
                  curlen_r   := 3;
                  curlen_c   := 1;
                  cursor_l   := 4;
                  for i1 := 1 to 24 do
                    begin
                      tmp := chr(i1+31);                        { Row char}
                      cursor_r[i1] := escape + 'Y' + tmp        { Row part}
                    end;
                  for i1 := 1 to 80 do
                    begin
                      tmp := chr(i1+31);                        { Col char}
                      cursor_c[i1] := tmp                       { Col part}
                    end
                end;
        64 :    { VT52 (/VT52)                          }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + 'K';
                  cursor_erp := escape + 'J';
                  curlen_r   := 3;
                  curlen_c   := 1;
                  cursor_l   := 4;
                  for i1 := 1 to 24 do
                    begin
                      tmp := chr(i1+31);                        { Row char}
                      cursor_r[i1] := escape + 'Y' + tmp        { Row part}
                    end;
                  for i1 := 1 to 80 do
                    begin
                      tmp := chr(i1+31);                        { Col char}
                      cursor_c[i1] := tmp                       { Col part}
                    end
                end;
      96..102 : { VT100,101,102,105,125,131,132 and ANSI X3.64 standard }
                { Note that the row and column strings must always be   }
                { of the same length                                    }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + '[K';
                  cursor_erp := escape + '[J';
                  curlen_r   := 4;
                  curlen_c   := 4;
                  cursor_l   := 8;
                  for i1 := 1 to 24 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Row chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_r[i1] := escape + '[' + tmp_str    { Row part }
                    end;
                  for i1 := 1 to 80 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Col chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_c[i1] := ';' + tmp_str + 'H'       { Col part }
                    end
                end;
        110 :   { VT200 and ANSI X3.64 standard (/DEV=VT200)}
                { Note that the row and column strings must always be   }
                { of the same length                                    }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + '[K';
                  cursor_erp := escape + '[J';
                  curlen_r   := 4;
                  curlen_c   := 4;
                  cursor_l   := 8;
                  for i1 := 1 to 24 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Row chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_r[i1] := escape + '[' + tmp_str    { Row part }
                    end;
                  for i1 := 1 to 80 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Col chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_c[i1] := ';' + tmp_str + 'H'       { Col part }
                    end
                end;
        112 :   { VT300 and ANSI X3.64 standard (/DEV=VT300)}
                { Note that the row and column strings must always be   }
                { of the same length                                    }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + '[K';
                  cursor_erp := escape + '[J';
                  curlen_r   := 4;
                  curlen_c   := 4;
                  cursor_l   := 8;
                  for i1 := 1 to 24 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Row chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_r[i1] := escape + '[' + tmp_str    { Row part }
                    end;
                  for i1 := 1 to 80 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Col chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_c[i1] := ';' + tmp_str + 'H'       { Col part }
                    end
                end;
           113 :   { VT400 and ANSI X3.64 standard (/DEV=VT400)}
                   { Note that the row and column strings must always be   }
                   { of the same length.                                   }
                begin
                  row_first := true;    { Sequence is row,col   }
                  cursor_erl := escape + '[K';
                  cursor_erp := escape + '[J';
                  curlen_r   := 4;
                  curlen_c   := 4;
                  cursor_l   := 8;
                  for i1 := 1 to 24 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Row chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_r[i1] := escape + '[' + tmp_str    { Row part }
                    end;
                  for i1 := 1 to 80 do
                    begin
                      writev(tmp_str,'00',i1:1);                { Col chars}
                      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
                      cursor_c[i1] := ';' + tmp_str + 'H'       { Col part }
                    end
                end;
        otherwise
                begin
                  writeln('*** ERROR : Terminal not supported ***');
                  writeln('See TERMDEF.PAS for definning new terminals.');
                  writeln('*** Terminals supported:');
                  writeln('    VT52         Set Terminal/VT52');
                  writeln('    VT100        Set Terminal/VT100');
                  writeln('    VT101        Set Terminal/VT101');
                  writeln('    VT102        Set Terminal/VT102');
                  writeln('    VT105        Set Terminal/VT105');
                  writeln('    VT125        Set Terminal/VT125');
                  writeln('    VT131        Set Terminal/VT131');
                  writeln('    VT132        Set Terminal/VT132');
                  writeln('    VT200        Set Terminal/VT200');
                  writeln('    VT300        Set Terminal/VT300');
                  writeln('    VT400        Set Terminal/VT400');
                  writeln('    Teleray 10   Set Terminal/FT1');
                  writeln('    ADM-3A       Set Terminal/FT2');
                  writeln('    ADDS100      Set Terminal/FT3');
                  writeln('    IBM3101      Set Terminal/FT4');
                  writeln;
                  exit
                end
    end
end;
