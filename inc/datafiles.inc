 { Flag string to Hex conversion }
[psect(datafiles$code)] function gethex(string : usernam_type) : unsigned;
const mult=7;
var  
     i,chnum,jindex,qcntr  : integer;
     ch                    : char;
     num		   : unsigned;
Begin
  gethex := 0;
  qcntr  := 0;
  jindex := 1;
  repeat
    if((string[jindex]>='A')and(string[jindex]<='F'))
      then
        chnum:=ord(string[jindex])-ord('A')+10
      else
        chnum:=ord(string[jindex])-ord('0');
    if (string[jindex]='''') then
      qcntr := qcntr + 1;
    if(qcntr>0)and(((jindex-4)>=0)and((jindex-4)<=7))then
      num:=num+(16**(mult-(jindex-4)))*chnum;
    jindex := jindex + 1;
  until(qcntr=2);
  gethex := num;
End;

{ Read in the creature data from an external file }
[psect(datafiles$code)] procedure read_creatures;
  var 
    count,indx  : integer;
    infile	: text;
    flagstr	: usernam_type;
  BEGIN
    {read creatures from external file}
      open (infile,BOSS_MON,readonly);
      reset(infile);
      readln(infile,max_creatures);
      for count := 1 to max_creatures do
        with c_list[count] do
          Begin
	    readln(infile,name);
            for indx := 1 to 3 do
              begin
	      	readln(infile,flagstr);
                case indx of
                 1 : cmove := gethex(flagstr);
                 2 : spells := gethex(flagstr);
                 3 : cdefense := gethex(flagstr);
                end;
              end;
            readln(infile,sleep,mexp,aaf,ac,speed);
	    readln(infile,reprod,size,tt,intel,speaks);
            readln(infile,cchar);
            readln(infile,hd);
            readln(infile,damage);
            readln(infile,level);
          End;
      close(infile);
  END;

{ Read in the master object list.  It's about time someone did this. -RLG}
[psect(datafiles$code)] procedure read_objects;
  var
	count   : integer;
	infile  : text;
	flagstr : usernam_type;
	dummy   : vtype;
  Begin
    { Read objects from external file }
      open (infile,BOSS_OBJ,readonly);
      reset(infile);
      readln(infile,max_objects);
      for count := 1 to max_objects do
        begin
          object_ident[count] := false; {set all objects to be not identified}
          with object_list[count] do
            begin
	      readln(infile,dummy); 	
              readln(infile,name);
	      readln(infile,flagstr);
	      flags := gethex(flagstr); {convert flag string to hex value}
	      readln(infile,tval,subval,cost,weight,number,level);
	      readln(infile,p1,tohit,todam,ac,toac);
	      readln(infile,damage);
	      readln(infile,tchar);
            end;
	end;
      close(infile);
  END;

[psect(datafiles$code)] procedure read_inventory_list; {The obvious sequel. -RLG}
  var
	count   : integer;
	infile  : text;
	flagstr : usernam_type;
	dummy   : vtype;
  Begin
    { Read store inventory objects from external file }
      open (infile,BOSS_INV,readonly);
      reset(infile);
      readln(infile,inven_init_max);
      for count := 1 to inven_init_max do
        begin
          with inventory_init[count] do
            begin
	      readln(infile,dummy); 	
              readln(infile,name);
	      readln(infile,flagstr);
	      flags := gethex(flagstr); {convert flag string to hex value}
	      readln(infile,tval,subval,cost,weight,number,level);
	      readln(infile,p1,tohit,todam,ac,toac);
	      readln(infile,damage);
	      readln(infile,tchar);
            end;
	end;
      close(infile);
  END;

{ Initialize, restore, and get the ball rolling. }
[psect(datafiles$code)] procedure read_data;
Begin
  read_creatures;
  read_objects;
  read_inventory_list;
End;



{Loads skill data from file into skill_list array. -RLG}
{Seems like a better way to handle the skill lists. }
{But geez did this take a lot of work to implement!!! }
{I was warned.. 

{ I should have listened to what my mother used to say. }
{ What did she say? }
{ I don't know.  I wasn't listening. }

[psect(datafiles$code)] procedure get_skill_data(number : byteint);
  VAR
    pos     : byteint;
    dummy   : vtype;
    infile  : text;
	
  BEGIN
      open (infile,SKILLS,readonly);
      reset(infile);

      repeat
        repeat
          readln(infile,dummy);
	until( substr(dummy,1,2) = '**');
	readln(infile,pos);
      until(pos = number);

      for pos := 1 to max_skills do
        begin
          with skill_list[pos] do
            begin
	      readln(infile,sname); 	
	      readln(infile,slevel,smana,sexp,sfail);
	      learned := false;
            end;
	end;
      close(infile);
  END;

