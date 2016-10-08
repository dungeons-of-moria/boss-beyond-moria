{ Chance of treasure having magic abilities
  Chance increases with each dungeon power  }

{Pre-declared in MISC.INC}
procedure magic_treasure;

  { parameters: 
     var item : treasure_type;
     power    : integer; 
  }

  var
     chance,special,unique,cursed,i1 : integer;
 
{ Boolean : is object enchanted }
    function magik(chance : integer) : boolean;
      BEGIN
        if (randint(100) <= chance) then
          magik := true
        else
          magik := false;
      end;
 
    procedure inc_damage(var dice : dtype; amount : integer);
      var
        num,sides,i1  : integer;
      begin
        for i1 := 1 to length(dice) do
	if (dice[i1] = 'd') then
          dice[i1] := ' ';
	num := 0;
	sides := 0;
	readv(dice,num,sides,error:=continue);
	i1 := num*amount;
	if ((i1 > 9) and (sides > 99)) then
          i1 := 9
        else if ((i1 > 99) and (sides > 9)) then
          i1 := 99;
        dice := '';
        writev(dice,i1:1,'d',sides:1);
      end;

{ Enchant a bonus based on degree desired   }
    function m_bonus(base,max_std,power : integer) : integer;
      var
        x,stand_dev      : integer;
      begin
        stand_dev := trunc(obj_std_adj*power) + obj_std_min;
        if (stand_dev > max_std) then stand_dev := max_std;
        x := trunc(abs(randnor(0,stand_dev))/10.0) + base;
        if (x < base) then
          m_bonus := base
        else
          m_bonus := x;
      end;
 
    BEGIN
      if (power = 999) then  {black market stuff}
        begin
	  chance  := 100;
	  special := 70;
	  unique  := 0;
	  cursed  := 0;
	  power   := 40;
	end 
      else if (power = 777) then  {beginning character inventory}
        begin
	  chance  := 50;
	  special := 10;
	  unique  := 0;
	  cursed  := 0;
	  power   := 5;
	end 
      else
        begin
	  chance := obj_base_magic + power;
	  if (chance > obj_base_max) then chance := obj_base_max;
	  special := trunc(chance/obj_div_special);
	  unique  := trunc(chance/obj_div_unique);
	  cursed  := trunc(chance/obj_div_cursed);
	end;
      with item do
{ Depending on treasure type, it can have certain magical properties}
        case tval of
        34,35,36: begin { Armor and shields}
                    if (magik(chance)) then
                      begin
                        toac := toac + m_bonus(1,30,power);
                        if (magik(special)) then
                          case randint(12) of
                        1     : begin
                                  flags := uor(flags,%X'023C0000');
                                  name := name + ' (Resist)';
                                  toac := toac + 5;
                                  cost := cost + 3500;
                                end;
                        2     : begin 
                                  flags := uor(flags,%X'00100000');
                                  name := name + ' (RA)';
                                  cost := cost + 1000;
                                end;
                        3,4   : begin 
                                  flags := uor(flags,%X'00080000');
                                  name := name + ' (RF)';
                                  cost := cost + 600;
                                end;
                        5,6   : begin
                                  flags := uor(flags,%X'00200000');
                                  name := name + ' (RC)';
                                  cost := cost + 600;
                                end;
                          7   : begin 
                                  flags := uor(flags,%X'02000000');
                                  name := name + ' (RL)';
                                  cost := cost + 500;
                                end;
                          8   : begin 
                                  flags := uor(flags,%X'00040000');
                                  name := name + ' (RG)';
                                  cost := cost + 1000;
                                end;
                          9   : begin 
                                  toac := toac + 15;
                                  name := name + ' (P)';
				  weight := trunc(weight*1.50);
                                end;
                         10   : begin 
				  tohit := tohit + m_bonus(1,40,power);
                                  name := name + ' (%P2) (CR)';
				  weight := trunc(weight*0.75);
                                end;
                      11,12   : begin 
                                  name := name + ' (W)';
				  cost := cost + weight*20;
				  weight := 1;
                                end;
                          end
			else if (magik(unique)) then
			  case randint(4) of
			  1   : begin
			          name := name + ' (Qwethleg)';
				  ac := ac + 10;
				  toac := toac + 10;
				  tohit := tohit + m_bonus(5,40,power);
				  cost := cost + 1000 + tohit*100;
				end;
			  2   : begin
			          name := name + ' (Fi-Yoh)';
				  toac := toac + m_bonus(2,40,power);
				  p1 := p1 + m_bonus(1,30,power);
				  {Wisdom and Regeneration}
				  flags := uor(flags,%X'00000810');
				  cost := cost + 2000 + p1*200;
				end;
			  3   : begin
			          name := name + ' (Jarth)';
				  toac := toac + m_bonus(2,40,power);
				  p1 := p1 + m_bonus(1,30,power);
				  {Dexterity and Resistance}
				  flags := uor(flags,%X'02380002');
			  	  cost := cost + 2500 + p1*200;
				end;
				
			  4   : begin
			          name := name + ' (?????)';
				  cost := cost*10;
				end;
			  end;
                      end
                    else if (magik(cursed)) then
                      begin
                        toac := -m_bonus(1,40,power);
                        cost := 0;
                        flags := uor(%X'80000000',flags);
                      end
                  end;
           21,23: begin { Weapons   }
                    if (magik(chance)) then
                      begin
                        tohit := tohit + m_bonus(0,40,power);
                        todam := todam + m_bonus(0,40,power);
                        if (magik(special)) then
                          case randint(18) of
			    1 : begin  
			       { Str,KS,KM,See-Invis}
                                  flags := uor(flags,%X'01018001');
                                  tohit := tohit + 5;
                                  todam := todam + 5;
                                  toac  := toac + randint(4);
                                  p1    := 1 + randint(4);
                                  name  := name + ' (%P1) (HA)';
                                  cost  := cost + p1*500;
                                  cost  := cost + 10000;
                                end;
			    2 : begin  
			        {Stl,Regen,See-Invis,FeatherFall}
                                  flags := uor(flags,%X'05000900');
                                  tohit := tohit + 3;
                                  todam := todam + 3;
                                  toac  := toac + 5 + randint(5);
                                  name  := name + ' [%P4] (DF)';
                                  p1    := randint(3);
				  weight := trunc(weight*1.25);
                                  cost  := cost + p1*500;
                                  cost  := cost + 7000;
                                end;
                          3,4 : begin  
                                  flags := uor(flags,%X'00004000');
                                  tohit := tohit + 3;
                                  todam := todam + 3;
                                  name := name + ' (KC)';
                                  cost := cost + 5000;
                                end;
                          5,6 : begin  
                                  flags := uor(flags,%X'00002000');
                                  tohit := tohit + 3;
                                  todam := todam + 3;
                                  name := name + ' (KA)';
                                  cost := cost + 4000;
                                end;
                          7,8 : begin  
                                  flags := uor(flags,%X'00008000');
                                  tohit := tohit + 3;
                                  todam := todam + 3;
                                  name := name + ' (KS)';
                                  cost := cost + 4000;
                                end;
                         9,10 : begin  
                                  flags := uor(flags,%X'00010000');
                                  tohit := tohit + 2;
                                  todam := todam + 2;
                                  name := name + ' (KM)';
                                  cost := cost + 3000;
                                end;
                        11,12 : begin  
                                  flags := uor(flags,%X'00080000');
                                  tohit := tohit + 1;
                                  todam := todam + 3;
                                  name := name + ' (KR)';
                                  cost := cost + 3500;
                                end;
                        13,14 : begin
                                  flags := uor(flags,%X'00040000');
                                  tohit := tohit + 1;
                                  todam := todam + 3;
                                  name := name + ' (FT)';
                                  cost := cost + 2000;
                                end;
                        15,16 : begin  
                                  flags := uor(flags,%X'00020000');
                                  tohit := tohit + 1;
                                  todam := todam + 3;
                                  name := name + ' (FB)';
                                  cost := cost + 2000;
                                end;
                           17 : begin
                                  todam := todam + 20;
                                  name := name + ' (DST)';
                                  cost := cost + 5000;
				  weight := trunc(weight*1.5);
                                end;
			   18 : begin
			          {See-Invis}
				  flags := uor(flags,%X'01000000');
			          name := name + '  :)';
				  cost := cost*2;
				end;
                            end
			else if (magik(unique)) then
			  case randint(3) of
			1,2   : begin
				  inc_damage(damage,2);
				  name := name + ' (Slicer)';
				  cost := cost*2;
			        end;
			  3   : begin
				  inc_damage(damage,4);
				  name := name + ' (Razor)';
				  cost := cost*4;
				end;
			  end;
                      end
                    else if (magik(cursed)) then
                      begin
                        tohit := -m_bonus(1,55,power);
                        todam := -m_bonus(1,55,power);
                        flags := uor(%X'80000000',flags);
                        cost := 0;
                      end
                  end;
           19,20 : begin { Guns, Automatic Guns  }
                    if (magik(chance)) then
                      tohit := tohit + m_bonus(1,30,power)
                    else if (magik(cursed)) then
                      begin
                        tohit := -m_bonus(1,50,power);
                        flags := uor(%X'80000000',flags);
                        cost := 0;
                      end;
                  end;
             82 : begin { Thrown  }
		    number := randint(4)+1;
                    if (magik(chance)) then
		      begin
                      	tohit := tohit + m_bonus(1,30,power);
                      	todam := todam + m_bonus(1,30,power);
		      end
                    else if (magik(cursed)) then
                      begin
                        tohit := -m_bonus(1,50,power);
                        flags := uor(%X'80000000',flags);
                        cost := 0;
                      end;
                  end;
             84 : begin { Grenades  }
                    if (magik(chance)) then
                      todam := todam + m_bonus(1,30,power);
                    number := 0;
                    for i1 := 1 to 3 do number := number + randint(2);
                  end;
             25 : begin { Digging tools }
                    if (magik(chance)) then
                      case randint(3) of
                        1,2:begin
                              p1 := m_bonus(2,25,power);
                              cost := cost + p1*100;
                            end;
                        3 : begin
                              p1 := -m_bonus(1,30,power);
                              cost := 0;
                              flags := uor(%X'80000000',flags);
                            end;
                      end;
                  end;
             31 : begin { Gloves and Gauntlets  }
                    if (magik(chance)) then
                      begin
                        toac := toac + m_bonus(1,20,power);
                        if (magik(special)) then
                          case randint(4) of
                          1,2  :begin
                                  tohit := tohit + 1 + randint(3);
                                  todam := todam + 1 + randint(3);
                                  name := name + ' of Slaying';
                                end;
                            3  :begin
                                  flags := uor(%X'00800000',flags);
                                  name := name + ' of Free Action';
                                  cost := cost + 1000;
                                end;
                            4  :begin
			        {Dex,Stealth}
                                  flags := uor(%X'00000102',flags);
			          p1 := m_bonus(1,30,power);
                                  name := name + ' of Thievery';
                                  cost := cost + p1*500;
                                end;
                          end;
                      end
                    else if (magik(cursed)) then
                      begin
                        if (magik(special)) then
                          case randint(2) of
                            1 : begin
                                  flags := uor(%X'80000002',flags);
                                  name := name + ' of Clumsiness';
                                  p1 := 1;
                                end;
                            2 : begin
                                  flags := uor(%X'80000001',flags);
                                  name := name + ' of Weakness';
                                  p1 := 1;
                                end;
                          end;
                        toac := -m_bonus(1,40,power);
                        p1   := -m_bonus(1,10,power);
                        flags := uor(%X'80000000',flags);
                        cost := 0;
                      end
                  end;
          30    : begin { Boots }
                    if (magik(chance)) then
                      begin
                        toac := toac + m_bonus(1,20,power);
                        if (magik(special)) then
                          case randint(12) of
                            1 : begin
                                  flags := uor(%X'00001000',flags);
                                  name := name + ' of Speed';
                                  p1 := 1;
                                  cost := cost + 5000;
                                end;
                      2,3,4,5 : begin
                                  flags := uor(%X'00000100',flags);
                                  name := name + ' of Stealth';
                                  cost := cost + 500;
                                end;
                      otherwise begin
                                  flags := uor(%X'04000000',flags);
                                  name := name + ' of Slow decent';
                                  cost := cost + 250;
                                end
                            end
                      end
                    else if (magik(cursed)) then
                      begin
                        case randint(3) of
                          1 : begin
                                flags := uor(%X'80001000',flags);
                                name := name + ' of Slowness';
                                p1 := -1;
                              end;
                          2 : begin
                                flags := uor(%X'80000200',flags);
                                name := name + ' of Noise';
                              end;
                          3 : begin
                                flags := uor(%X'80000000',flags);
                                name := name + ' of Great Mass';
                                weight := weight*5;
                              end;
                        end;
                        cost := 0;
                        ac := -m_bonus(2,45,power);
                      end;
                  end;
          33    : begin { Helms }
                    if (magik(chance)) then
                      begin
                        toac := toac + m_bonus(1,20,power);
                        if (magik(special)) then
                          case subval of 
                1,2,3,4,5  :  case randint(3) of
                                1 : begin
                                      p1 := randint(2);
                                      flags := uor(%X'00000008',flags);
                                      name := name + ' (%P1) of Intelligence';
                                      cost := cost + p1*500;
                                    end;
                                2 : begin
                                      p1 := randint(2);
                                      flags := uor(%X'00000010',flags);
                                      name := name + ' (%P1) of Wisdom';
                                      cost := cost + p1*500;
                                    end;
                                3 : begin
                                      p1 := 1 + randint(4);
                                      flags := uor(%X'40000000',flags);
                                      name := name + ' (%P1) of Infra-Vision';
                                      cost := cost + p1*250;
                                    end;
                              end;
                6,7,8      :  case randint(6) of  { Crowns }
                                1 : begin
                                      p1 := randint(3);
				      {STR,CON,DEX,Free Action}
                                      flags := uor(%X'00800007',flags);
                                      name := name + ' (%P1) of Might';
                                      cost := cost + 1000 + p1*500;
                                    end;
                                2 : begin
                                      p1 := randint(3);
                                      flags := uor(%X'00000030',flags);
                                      name := name + ' (%P1) of Lordliness';
				      toac := toac + randint(10);
                                      cost := cost + 1000 + p1*500;
                                    end;
                                3 : begin
                                      p1 := randint(3);
                                      flags := uor(%X'01380008',flags);
                                      name := name + ' (%P1) of Jarwangia';
                                      cost := cost + 3000 + p1*500;
                                    end;
                                4 : begin
                                      p1 := randint(3);
                                      flags := uor(%X'00000020',flags);
                                      name := name + ' (%P1) of Beauty';
                                      cost := cost + 750;
                                    end;
                                5 : begin
                                      p1 := 1 + randint(4);
                                      flags := uor(%X'01000040',flags);
                                      name := name + ' (%P1) of Seeing';
                                      cost := cost + 1000 + p1*100;
                                    end;
                                6 : begin
                                      flags := uor(%X'00000800',flags);
                                      name := name + ' of Regeneration';
                                      cost := cost + 1500;
                                    end;
                              end;
                          end;
                      end
                    else if (magik(cursed)) then
                      begin
                        toac := -m_bonus(1,45,power);
                        flags := uor(%X'80000000',flags);
                        cost := 0;
                        if (magik(special)) then
                          case randint(7) of
                            1 : begin
                                  p1 := -1;
                                  flags := uor(%X'00000008',flags);
                                  name := name + ' of Stupidity';
                                end;
                            2 : begin
                                  p1 := -1;
                                  flags := uor(%X'00000010',flags);
                                  name := name + ' of Dullness';
                                end;
                            3 : begin
                                  flags := uor(%X'08000000',flags);
                                  name := name + ' of Blindness';
                                end;
                            4 : begin
                                  flags := uor(%X'10000000',flags);
                                  name := name + ' of Timidness';
                                end;
                            5 : begin
                                  p1 := -1;
                                  flags := uor(%X'00000001',flags);
                                  name := name + ' of Weakness';
                                end;
                            6 : begin
                                  flags := uor(%X'00000400',flags);
                                  name := name + ' of Teleportation';
                                end;
                            7 : begin
                                  p1 := -1;
                                  flags := uor(%X'00000020',flags);
                                  name := name + ' of Ugliness';
                                end;
                          end;
                        p1 := p1*randint(5);
                      end;
                  end;
          45    : begin { Rings }
                    case subval of
                1,2,3,4,5,6 :   if (magik(cursed)) then
                                  begin
                                    p1 := -m_bonus(1,20,power);
                                    flags := uor(%X'80000000',flags);
                                    cost := -cost;
                                  end
                                else
                                  begin
                                    p1 := m_bonus(1,10,power);
                                    cost := cost + p1*100;
                                  end;
                          7 :   if (magik(cursed)) then  {Speed}
                                  begin
                                    p1 := -randint(3);
                                    flags := uor(%X'80000000',flags);
				    cost := -cost;
                                  end
                                else
				  begin
                                    p1 := m_bonus(1,30,power);
				    cost := cost + p1*p1*10000;
				  end;
                          8  :  begin
                                  p1 := m_bonus(1,20,power);
                                  cost := cost + p1*100;
                                end;
                          22 :  begin   { Increase damage   }
                                  todam := todam + m_bonus(1,20,power);
                                  if (magik(cursed)) then
                                    begin
                                      todam := -todam;
                                      flags := uor(%X'80000000',flags);
                                      cost := -cost;
                                    end
                                end;
                          23 :  begin   { Increase To-Hit   }
                                  tohit := tohit + m_bonus(1,20,power);
                                  cost := cost + tohit*100;
                                  if (magik(cursed)) then
                                    begin
                                      tohit := -tohit;
                                      flags := uor(%X'80000000',flags);
                                      cost := -cost;
                                    end
                                end;
                          24 :  begin   { Protection    }
                                  toac := toac + m_bonus(1,20,power);
                                  if (magik(cursed)) then
                                    begin
                                      toac := -toac;
                                      flags := uor(%X'80000000',flags);
                                      cost := -cost;
                                    end
                                end;
                          33 :  begin   { Slaying   }
                                  tohit := tohit + m_bonus(1,25,power);
                                  todam := todam + m_bonus(1,25,power);
                                  if (magik(cursed)) then
                                    begin
                                      tohit := -tohit;
                                      todam := -todam;
                                      flags := uor(%X'80000000',flags);
                                      cost := -cost;
                                    end
                                end;
                            otherwise ;
                          end;
                  end;
          40    : begin { Amulets   }
		    if (magik(cursed)) then
                      begin
                        p1 := -m_bonus(1,20,power);
                        flags := uor(%X'80000000',flags);
                        cost := -cost;
                      end
                    else
                      begin
                        p1 := m_bonus(1,10,power);
                        cost := cost + p1*100;
                      end;
                  end;

     { Light sources -- Subval should be even for store, odd for dungeon
         Dungeon found ones will be partially charged    }
          15    : begin { Lamps and torches}
                    if ((subval mod 2) = 1) then
                      p1 := randint(p1);
                  end;
          65    : begin { Ray Guns }
                    case subval of
                      1,2   :   p1 := randint(9)  + 6;
                        3   :   p1 := randint(5)  + 6;
                        4   :   p1 := randint(8)  + 6;
                        5   :   p1 := randint(4)  + 3;
                        6   :   p1 := randint(8)  + 6;
                       7,8  :   p1 := randint(20) + 12;
                      9,10  :   p1 := randint(11) + 6;
                        11  :   p1 := randint(10) + 12;
                        12  :   p1 := randint(3)  + 3;
                     13,14  :   p1 := randint(9)  + 6;
                     15,16  :   p1 := randint(5)  + 3;
                     17,18  :   p1 := randint(5)  + 5;
                  19,20,21  :   p1 := randint(8)  + 4;
                        22  :   p1 := randint(24) + 12;
               23,24,25,26  :   p1 := randint(9)  + 5;
                     otherwise  p1 := randint(10) + 2;
                    end
                  end;
          55    : begin { Devices    }
                    case subval of
                        1   :   p1 := randint(20) + 12;
                        2   :   p1 := randint(8)  + 6;
                        3   :   p1 := randint(5)  + 6;
                        4   :   p1 := randint(20) + 12;
                        5   :   p1 := randint(15) + 6;
                      6,7   :   p1 := randint(4)  + 4;
	            8,9,10  :   p1 := randint(3)  + 1;
                        11  :   p1 := randint(5)  + 6;
                        12  :   p1 := randint(10) + 12;
                  13,14,15  :   p1 := randint(5)  + 6;
                        16  :   p1 := randint(10) + 12;
                        17  :   p1 := randint(3)  + 4;
                     18,19  :   p1 := randint(5)  + 6;
                        20  :   p1 := randint(3)  + 4;
                        21  :   p1 := randint(10) + 12;
                     22,23  :   p1 := randint(3)  + 4;
                        24  :   p1 := randint(3)  + 1;
                        25  :   p1 := randint(10) + 6;
                     otherwise  p1 := randint(10) + 2; 
                    end
                  end;
          32    : begin { Cloaks    }
                    if (magik(chance)) then
                      begin
                        if (magik(special)) then
                          case randint(2) of
                1      : begin
                                  name := name + ' of Protection';
                                  toac := toac + m_bonus(2,40,power);
                                  cost := cost + 250;
                                end;
                2      : begin
                                  toac := toac + m_bonus(1,20,power);
                                  p1 := randint(3);
                                  flags := uor(%X'00000100',flags);
                                  name := name + ' of Stealth (%P1)';
                                  cost := cost + p1*500;
                                end;
                          end
                        else
                          toac := toac + m_bonus(1,20,power);
                      end
                    else if (magik(cursed)) then
                      case randint(3) of
                        1 : begin
                              flags := uor(%X'80000200',flags);
                              name := name + ' of Irritation';
                              ac   :=  0;
                              toac  := -m_bonus(1,10,power);
                              tohit := -m_bonus(1,10,power);
                              todam := -m_bonus(1,10,power);
                              cost :=  0;
                            end;
                        2 : begin
                              flags := uor(%X'80000000',flags);
                              name := name + ' of Vulnerability';
                              ac   := 0;
                              toac := -m_bonus(10,100,power+50);
                              cost := 0;
                            end;
                        3 : begin
                              flags := uor(%X'80000000',flags);
                              name := name + ' of Enveloping';
                              toac  := -m_bonus(1,10,power);
                              tohit := -m_bonus(2,40,power+10);
                              todam := -m_bonus(2,40,power+10);
                              cost := 0;
                            end;
                      end;
                  end;
          2     : Begin { Chests    }
                    case (randint(power)+4) of
                      1  : begin
                                    name := name + '^ (Empty)';
                                  end;
                      2  : begin
                                    flags := uor(%X'00000001',flags);
                                    name := name + '^ (Locked)';
                                  end;
                      3,4       : begin
                                    flags := uor(%X'00000011',flags);
                                    name := name + '^ (Poison Needle)';
                                  end;
                      5,6       : begin
                                    flags := uor(%X'00000021',flags);
                                    name := name + '^ (Poison Needle)';
                                  end;
                      7,8,9     : begin
                                    flags := uor(%X'00000041',flags);
                                    name := name + '^ (Gas Trap)';
                                  end;
                      10,11     : begin
                                    flags := uor(%X'00000081',flags);
                                    name := name + '^ (Explosion Device)';
                                  end;
                      12,13,14  : begin
                                    flags := uor(%X'00000101',flags);
                                    name := name + '^ (Summoning Runes)';
                                  end;
                      15,16,17  : begin
                                    flags := uor(%X'00000071',flags);
                                    name := name + '^ (Multiple Traps)';
                                  end;
                      otherwise   begin
                                    flags := uor(%X'00000181',flags);
                                    name := name + '^ (Multiple Traps)';
                                  end;
                    end;
                  end;
  9,10,11,12,13 : Begin      { Gun ammo and spikes}
                    number := 0;
                    for i1 := 1 to 7 do 
		      number := number + randint(6);
		    if (tval in [10,11,12]) then                    
		      if (magik(chance)) then
                        begin
                          tohit := tohit + m_bonus(1,35,power);
                          todam := todam + m_bonus(1,35,power);
                          if ((magik(special)) and (tval in [11,12])) then
			    case randint(12) of
			   1,2,3 :begin
                                    name := name + '(high density)';
                                    tohit := tohit + 5;
                                    todam := todam + 5;
                                    cost := cost + 20;
                                  end;
                           4,5   :begin
			            {Flame}
                                    flags := uor(flags,%X'00040000');
                                    tohit := tohit + 2;
                                    todam := todam + 4;
                                    name := name + '(explosive)';
                                    cost := cost + 25;
                                  end;
                           6,7   :begin
                                    flags := uor(flags,%X'00008000');
                                    tohit := tohit + 3;
                                    todam := todam + 3;
                                    name := name + ' of Scum Killing';
                                    cost := cost + 25;
                                  end;
                           8,9   :begin
                                    flags := uor(flags,%X'00004000');
                                    tohit := tohit + 2;
                                    todam := todam + 2;
                                    name := name + ' of Creature Killing';
                                    cost := cost + 30;
                                  end;
                           10,11 :begin
                                    flags := uor(flags,%X'00080000');
                                    tohit := tohit + 5;
                                    todam := todam + 10;
                                    name := name + ' of Robot Slaying';
                                    cost := cost + 30;
                                  end;
                           12    :begin
                                    flags := uor(flags,%X'00002000');
                                    tohit := tohit + 7;
                                    todam := todam + 7;
                                    name := name + ' of Alien Slaying';
                                    cost := cost + 35;
                                  end;
                            end; {case}
			end
                      else if ((tval in [10,11,12]) and (magik(cursed))) then
                        begin
                          tohit := -m_bonus(5,55,power);
                          todam := -m_bonus(5,55,power);
                          flags := uor(%X'80000000',flags);
                          cost := 0;
                        end;
                  End;
	{Coupons}      
	     3: Begin
		  case randint(6) of
		 1,2,3: p1 := 20;
		   4,5: p1 := 30;
		     6: p1 := 40;
		  end;		    
		  if (magik(special)) then 
		    p1 := p1 + 30;
		End;
	    otherwise ;
          end;  {case}
	{Multiple Number Types.  Need to adjust subval.}
	with item do
	  If (tval in [9,10,11,12,13,82,84]) then
	    begin
              missle_ctr := missle_ctr + 1;
              if (missle_ctr > 65534) then
                missle_ctr := 1;
              subval := missle_ctr + 512;
 	    end;
      END;

