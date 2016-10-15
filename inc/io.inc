procedure inkey(var x : char);
begin
x := readkey;
msg_flag := false
end;

procedure msg_print(str_buff : string);
var
  old_len : shortint;
  in_char : char;

begin
if msg_flag and more_flag then
  begin
  old_len := length(old_msg) + 1;
  gotoxy(old_len, 1);
  write(' -more-');
  repeat
    inkey(in_char)
  until in_char in [#10, #13, #27, ' ']
  end;
gotoxy(1, 1);
clreol;
write(str_buff);
old_msg := str_buff;
msg_flag := true
end;

procedure full_clear;
begin
clrscr
end;

procedure clear(row, col : integer);
var
  i : byte;
begin
gotoxy(col, row);
clreol;
for i := row + 1 to 25 do
  begin
  used_line[i] := false;
  gotoxy(1, i);
  clreol
  end;
gotoxy(row,col)
end;

procedure prt(strbuff : string; row, col : integer);
begin
gotoxy(col, row);
clreol;
write(strbuff)
end;

procedure erase_line(row, col : integer);
begin
gotoxy(col, row);
clreol
end;

procedure pause(prt_line : integer);
var dummy : char;
begin
prt('[Press any key to continue]', prt_line, 24);
inkey(dummy);
erase_line(prt_line, 1)
end;

procedure print(strbuff : string; row, col : integer);
begin
gotoxy(col - panel_col_prt, row - panel_row_prt);
used_line[row - panel_row_prt] := true;
write(strbuff)
end;

procedure put_buffer(strbuff : string; row, col : integer);
begin
gotoxy(col, row);
write(strbuff)
end;

function get_string(var in_str : string; row, col, slen : integer) : boolean;
var
  start_col, end_col : integer;
  x : char;
  flag, cancel : boolean;
 
begin
cancel := false;
flag := false;
in_str:= addcharr(' ', '', slen);
gotoxy(col, row);
write(in_str);
in_str := '';
start_col := col;
end_col := col + slen - 1;
gotoxy(col, row);
repeat
  inkey(x);
  case x of
    #26, #27: cancel := true;
    #10, #13: flag  := true;
    #8: 
      if (col > start_col) then
        begin
        col -= 1;
        gotoxy(col, row);
        write(' ');
        gotoxy(col, row);
        dec(in_str[0])
        end;
    otherwise
      begin
      write(x);
      in_str += x;
      inc(col);
      if (col > end_col) then
        flag := true;
      end;
  end
until (flag or cancel);
if (cancel) then
  get_string := false
else
  begin 
  in_str := trimright(in_str);
  get_string := true
  end
end;

function get_com(prompt : string; var command : char) : boolean;
begin
if length(prompt) > 0 then
  prt(prompt, 1, 1);
inkey(command);
get_com := command <> #27;
erase_line(msg_line, msg_line);
msg_flag := false
end;

procedure pause_exit(prt_line : integer);
    var
        dummy : char;
begin
prt('[Press any key to continue, or <Control>-Z to exit]',prt_line,11);
inkey(dummy);
erase_line(prt_line,1);
if dummy = #26 then
  halt(0)
end;

function get_hex_value(row, col, slen : integer) : integer;
var
  tmp_str : string;
begin
if get_string(tmp_str, row, col, slen) then
  get_hex_value := hex2dec(tmp_str)
else
  get_hex_value := 0
end;

procedure get_paths;
const image_path = './dat/';
begin
BOSS_HOU := image_path + 'hours.dat';
BOSS_MOR := image_path + 'message.dat';
BOSS_MAS := image_path + 'bosschr.dat';
BOSS_TOP := image_path + 'bosstop.dat';
BOSS_MON := image_path + 'monsters.dat';
BOSS_OBJ := image_path + 'objects.dat';
BOSS_INV := image_path + 'invent.dat';
BOSS_LOS := image_path + 'loser.dat';
BOSS_ANNOY:=image_path + 'putzs.dat';
BOSS_WIZ := image_path + 'wizard.dat';
BUS_PIC  := image_path + 'bus.dat';
QUOTES   := image_path + 'quotes.dat';
SKILLS   := image_path + 'skills.dat'
end;
