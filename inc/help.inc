procedure ident_char;
    var
        command         : char;
    BEGIN
      if (get_com('Enter character to be identified :',command)) then
        case command of
          ' ' : prt('  - An open pit.',1,1);
          '!' : prt('! - A potion.',1,1);
          '"' : prt('" - An amulet, periapt, or necklace.',1,1);
          '#' : prt('# - A stone wall, or box.',1,1);
          '$' : prt('$ - Treasure.',1,1);
          '%' : prt('% - Computer.',1,1);
          '&' : prt('& - Treasure chest.',1,1);
          '''': prt(''' - An open door.',1,1);
          '(' : prt('( - Soft armor.',1,1);
          ')' : prt(') - A shield.',1,1);
          '*' : prt('* - Grenade,or Treasure.',1,1); 
          '+' : prt('+ - A closed door.',1,1);
          ',' : prt(', - Food.',1,1);
          '-' : prt('- - A ray gun',1,1);
          '.' : prt('. - Floor.',1,1);
          '/' : prt('/ - Sports Equipment.',1,1);
          ':' : prt(': - Rubble.',1,1);
          ';' : prt('; - A loose rock.',1,1);
          '<' : prt('< - An up staircase.',1,1);
          '=' : prt('= - A ring.',1,1);
          '>' : prt('> - A down staircase.',1,1);
          '?' : prt('? - A floppy disk.',1,1);
          '0' : prt('0 - ???.',1,1);
          '@' : prt(py.misc.name,1,1);
          'A' : prt('A - Alien Daysians.',1,1);
          'B' : prt('B - The Boss.',1,1);
          'C' : prt('C - Acidic Cubes.',1,1);
          'D' : prt('D - Droids.',1,1);
          'E' : prt('E - Evil Things.',1,1);
          'F' : prt('F - Flayers.',1,1);
          'G' : prt('G - Guards.',1,1);
          'H' : prt('H - Hounds or Horses.',1,1);
          'I' : prt('I - Insane Lads.',1,1);
          'J' : prt('J - Jarwangian Molds.',1,1);
         {'K' : prt('K - ???.',1,1);}
          'L' : prt('L - ???.',1,1);
          'M' : prt('M - Mutants.',1,1);
         {'N' : prt('N - Negative Beings.',1,1);}
          'O' : prt('O - Oozes.',1,1);
          'P' : prt('P - Giant/Powerful humanoids.',1,1);
         {'Q' : prt('Q - ???.',1,1);}
          'R' : prt('R - Robots.',1,1);
          'S' : prt('S - ???.',1,1);
          'T' : prt('T - ???.',1,1);
          'U' : prt('U - ???.',1,1);
          'V' : prt('V - Vortexes.',1,1);
          'W' : prt('W - Warbots.',1,1);
          'X' : prt('X - ???.',1,1);
          'Y' : prt('Y - ???.',1,1);
          'Z' : prt('Z - Zarg!!.',1,1);
          '[' : prt('[ - Hard armor.',1,1);
          ']' : prt('] - Misc. armor.',1,1);
          '^' : prt('^ - A trap.',1,1);
          '_' : prt('_ - An alien device.',1,1);
         {'`' : prt('` - Not used.',1,1);}
          'a' : prt('a - Animals.',1,1);
          'b' : prt('b - Bats.',1,1);
          'c' : prt('c - cloister.',1,1);
          'd' : prt('d - Droids.',1,1);
          'e' : prt('e - Ectoplasms.',1,1);
          'f' : prt('f - Walking Fungi.',1,1);
          'g' : prt('g - Gang Members.',1,1);
          'h' : prt('h - Hackers.',1,1);
          'i' : prt('i - Insect (swarms).',1,1);
          'j' : prt('j - ???.',1,1);
          'k' : prt('k - Kitchen Utensils.',1,1);
          'l' : prt('l - Lasers.',1,1);
          'm' : prt('m - Machinery.',1,1);
          'n' : prt('n - ???.',1,1);
          'o' : prt('o - Orbs.',1,1);
          'p' : prt('p - Person (Humanoid).',1,1);
          'q' : prt('q - ???.',1,1);
          'r' : prt('r - Rodents.',1,1);
          's' : prt('s - Snakes.',1,1);
          't' : prt('t - Ticks.',1,1);
          'u' : prt('u - ???.',1,1);
          'v' : prt('v - Vogons.',1,1);
          'w' : prt('w - Worms.',1,1);
          'x' : prt('x - Spiders.',1,1);
          'y' : prt('y - ???.',1,1);
          'z' : prt('z - Zippy.',1,1);
          '{' : prt('{ - Ammo.',1,1);
          '|' : prt('| - A sword or dagger.',1,1);
          '}' : prt('} - Gun.',1,1);
          '~' : prt('~ - Miscellaneous item.',1,1);
          otherwise prt('Not Used.',1,1);
        end
    END;
 
 
        { Help for available commands}
procedure help;
    begin
      clear(1,1);
prt('B <Dir> Bash (object/creature)|  q        Quaff a potion.',1,1);
prt('C       Display character.    |  r        Run a disk program.',2,1);
prt('D <Dir> Disarm a trap/chest.  |  s        Search for hidden doors.',3,1);
prt('E       Eat some food.        |  t        Take off an item.',4,1);
prt('F       Put D-cell in lantern.|  U        Use an alien device.',5,1);
prt('L       Current location.     |  V        Version and credits.',6,1);
prt('P       Print map.            |  w        Wear/Wield an item.',7,1);
prt('R       Rest for a period.    |  x        Exchange weapon.',8,1);
prt('S       Search Mode.          |  /        Identify a character.',9,1);
prt('T <Dir> Tunnel.               |  ?        Display this panel.',10,1);
prt('a       Aim & fire a ray gun. |  K        Display the Kill list.)',11,1);
prt('v       Browse a book.        |  ^M       Repeat the last message.',12,1);
prt('c <Dir> Close a door.         |  ^R       Redraw the screen.',13,1);
prt('d       Drop an item.         |  ^Y       Quit the game.',14,1);
prt('e       Equipment list.       |  ^Z       Save character and quit.',15,1);
prt('f       Fire a gun.           |',16,1);
prt('H       Hurl an item.         |',17,1);
prt('i       Inventory list.       |  < Go up an up-staircase.',18,1);
prt('J <Dir> Jam a door with spike.|  > Go down a down-staircase.',19,1);
prt('g <Dir> Look given direction. |  . <Dir>  Move in direction.',20,1);
prt('m       Do skill from manual. |  Movement: 7  8  9',21,1);
prt('o <Dir> Open a door/chest.    |            4     6    5 = Rest',22,1);
prt('p       Read a prayer.        |            1  2  3',23,1);
      pause(24);
      draw_cave;
    end;
 

{Displays the list of all the monsters -NBT}
procedure do_all_mons;

Begin
   Clear(1,1);
prt(' Note: A "*" in front of the symbol denotes that more than one',3,1);
prt('       type of creature uses that symbol.',4,1);
prt('*$ Coins                   *a Animals                  *b Bats',6,1);
prt(' c Cloister                *d Droids                   *e Ectoplasms ',7,1);
prt('*f Walking Fungi           *g Gangs                    *h Hackers',8,1);
prt('*i Insects (swarm)          j Jackal                   *k Kitchen Utensils',9,1);
prt('*l Lasers                  *m Machinery                 n ???',10,1);
prt('*o Orbs                    *p Humanoid                  q ???',11,1);
prt('*r Rodent                  *s Snakes                   *t Tick',12,1);
prt('*v Vortexes                *w Worm(s)                   y ???',13,1);
prt('*z Zippy                   *| Evil Sword               *A Aliens',14,1);
prt(' B The Boss                 C Acidic Cube              *D Droids',15,1);
prt('*E Evil Things              F Mind Flayer              *G Guards',16,1);
prt('*H Hound, Horse             I A Lad Insane             *J Jelly',17,1);
prt(' L ???                     *M Mutants',18,1);
prt('*O Ooze                    *P Giant Humanoid           *R Robots',19,1);
prt(' S ???                      T ???                       U ???',20,1);
prt('*V Vortexes                *W Warbots                   X ???',21,1);
prt(' Y ???			 Z Zarg!!',22,1);
   End;

{Displays a particular monster's "character sheet" -NBT}
procedure do_mon_out (moncode : integer);

Var 
    nbt_out_val, otw, attstr,attx  : vtype;
    cur_line, xpos                 : integer;
    atype, adesc                   : system.integer; {sscanf secure}
    damstr			   : ansistring;

BEGIN
   clear (1,1);
   with c_list[moncode] do
   BEGIN
      prt (name,1,1);
      prt ('Screen Code:',1,60);
      prt (cchar,1,72);
      prt_num ('Cur. Creature Number:',moncode,1,30);
      prt_num ('Speed =',speed,2,3);
      prt_num ('Level =',level,2,13);
      prt_num ('Exp =',mexp,2,24);
      prt_num ('AC =',ac,2,36);
      prt_num ('Eye-sight =',aaf,2,44);
      nbt_out_val := 'HD = ' + hd;
      prt (nbt_out_val,2,58);
      cur_line := 5;
      if (($80000000 and cmove) <> 0) then
        begin
         cur_line := 7;
         prt ('This creature is a ***Win Creature***',3,20);
        end;
      if (($00080000 and cmove) <> 0) then
        begin
         prt('Creature eats/kills other creatures.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0001 and cdefense) <> 0) then
        begin
         prt('Creature is an Alien.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0002 and cdefense) <> 0) then
        begin
         prt('Creature is a Creature.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0004 and cdefense) <> 0) then
        begin
         prt('Creature is Scum.',((cur_line + 1) div 2),(41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0008 and cdefense) <> 0) then
        begin
         prt('Creature is a Mutant.',((cur_line + 1) div 2),(41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0010 and cdefense) <> 0) then
        begin
         prt('Creature harmed by cold.',((cur_line + 1) div 2),
             (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0020 and cdefense) <> 0) then
        begin
         prt('Creature harmed by fire.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0040 and cdefense) <> 0) then
        begin
         prt('Creature harmed by poison.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0080 and cdefense) <> 0) then
        begin
         prt('Creature harmed by acid.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0100 and cdefense) <> 0) then
        begin
         prt('Creature harmed by blue light.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($0200 and cdefense) <> 0) then
        begin
         prt('Creature harmed by erosion',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($1000 and cdefense) <> 0) then
        begin
         prt('Creature can''t be charmed/slept.',((cur_line + 1) div 2),
             (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($2000 and cdefense) <> 0) then
        begin
         prt('Creature seen with Infra-Vision.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($4000 and cdefense) <> 0) then
        begin
         prt('hit points',2,69);
         prt('Creature has MAX hit points',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (cur_line mod 2) = 0 then cur_line := cur_line + 1;
      if (($00010000 and cmove) <> 0) then
        begin
         prt('Creature is invisible.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00100000 and cmove) <> 0) then
        begin
         prt('Creature picks up objects.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00200000 and cmove) <> 0) then
        begin
         prt('Creature multiplies.',((cur_line + 1) div 2),(41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($01000000 and cmove) <> 0) then
        begin
         prt('Creature carries objects.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($02000000 and cmove) <> 0) then
        begin
         prt('Creature carries gold, gems, etc.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($04000000 and cmove) <> 0) then
        begin
         prt('Has object/gold 60% of time.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($08000000 and cmove) <> 0) then
        begin
         prt('Has object/gold 90% of time.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($10000000 and cmove) <> 0) then
        begin
         prt('Has 1d2 object(s)/gold.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($20000000 and cmove) <> 0) then
        begin
         prt('Has 2d2 object(s)/gold.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
       end;
      if (($40000000 and cmove) <> 0) then
        begin
         prt('Has 4d2 object(s)/gold.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
       end;
      if (cur_line mod 2) = 0 then cur_line := cur_line + 1;
      prt('--- Creature Skills ---',((cur_line + 1) div 2),1);
      cur_line := cur_line + 2;
      nbt_out_val := '    Uses skills 1 out of every    turns.';
      prt(nbt_out_val,((cur_line + 1) div 2),1);
      prt_num('',($F and spells),((cur_line + 1) div 2),33);
      cur_line := cur_line + 2;
      if (($00000010 and spells) <> 0) then
        begin
         prt('Can teleport short.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000020 and spells) <> 0) then
        begin
         prt('Can teleport long.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000040 and spells) <> 0) then
        begin
         prt('Teleport player to itself.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000080 and spells) <> 0) then
        begin
         prt('Cause light wounds.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000100 and spells) <> 0) then
        begin
         prt('Cause serious wounds.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000200 and spells) <> 0) then
        begin
         prt('Hold person.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000400 and spells) <> 0) then
        begin
         prt('Cause blindness.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000800 and spells) <> 0) then
        begin
         prt('Cause confusion.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00001000 and spells) <> 0) then
        begin
         prt('Cause fear.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00002000 and spells) <> 0) then
        begin
         prt('Summon a creature.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00004000 and spells) <> 0) then
        begin
         prt('Summon a mutant.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00008000 and spells) <> 0) then
        begin
         prt('Slow person.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00010000 and spells) <> 0) then
        begin
         prt('Drains resources for healing.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00020000 and spells) <> 0) then
        begin
         prt('**Unknown spell value**',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00040000 and spells) <> 0) then
        begin
         prt('**Unknown spell value**',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00080000 and spells) <> 0) then
        begin
         prt('Breathes Lightning Breath.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00100000 and spells) <> 0) then
        begin
         prt('Breathes Gas Breath.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00200000 and spells) <> 0) then
        begin
         prt('Breathes Acid Breath.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00400000 and spells) <> 0) then
        begin
         prt('Breathes Frost Breath.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00800000 and spells) <> 0) then
        begin
         prt('Breathes Fire Breath.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (cur_line mod 2) = 0 then cur_line := cur_line + 1;
      prt('-- Movement:',((cur_line + 1) div 2),1);
      cur_line := cur_line + 2;
      if (($00000001 and cmove) <> 0) then
        begin
         prt('Moves only to attack.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000002 and cmove) <> 0) then
        begin
         prt('Moves and attacks normally.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000008 and cmove) <> 0) then
        begin
         prt('20% random movement.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000010 and cmove) <> 0) then
        begin
         prt('40% random movement.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00000020 and cmove) <> 0) then
        begin
         prt('75% random movement.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00020000 and cmove) <> 0) then
        begin
         prt('Can open doors.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (($00040000 and cmove) <> 0) then
        begin
         prt('Can phase through walls.',((cur_line + 1) div 2),
            (41 - 40*(cur_line mod 2)));
         cur_line := cur_line + 1;
        end;
      if (cur_line mod 2) = 0 then cur_line := cur_line + 1;
     prt('-- Creature attacks:',((cur_line + 1) div 2),1);
     cur_line := cur_line + 2;
     attstr := damage;
     while (length(attstr) > 0) do
       begin
        xpos := pos('|',attstr);
        if (xpos > 0) then
          begin
           attx := copy(attstr,1,xpos-1);
           attstr := copy(attstr,xpos+1,length(attstr)-xpos);
          end
        else
          begin
           attx := attstr;
           attstr := '';
          end;
	sscanf(attx, '%d%d%s', [@atype, @adesc, @damstr]);
    CASE atype of
{Normal attack  }     1  : otw := 'normal damage.';
{Poison Strength}     2  : otw := 'lowering strength.';
{Confusion attack}    3  : otw := 'confusion.';
{Fear attack    }     4  : otw := 'fear.';
{Fire attack    }     5  : otw := 'fire damage.';
{Acid attack    }     6  : otw := 'acid damage.';
{Cold attack    }     7  : otw := 'cold damage.';
{Lightning attack}    8  : otw := 'lightning damage.';
{Corrosion attack}    9  : otw := 'corrosion damage.';
{Blindness attack}    10 : otw := 'blindness.';
{Paralysis attack}    11 : otw := 'paralyzation.';
{Steal Money    }     12 : otw := 'stealing money.';
{Steal Object   }     13 : otw := 'stealing object.';
{Poison         }     14 : otw := 'poison damage.';
{Lose Dex       }     15 : otw := 'lose dexterity.';
{Lose Con       }     16 : otw := 'lose constitution.';
{Lose Int       }     17 : otw := 'lose intelligence.';
{Lose Wis       }     18 : otw := 'lose wisdom.';
{Lose Exp       }     19 : otw := 'lose experience.';
{Aggravation    }     20 : otw := 'aggravates monsters.';
{Disenchant     }     21 : otw := 'disenchants objects.';
{Eats food      }     22 : otw := 'eating food.';
{Eats light     }     23 : otw := 'eating light source.';
{Eats charges   }     24 : otw := 'absorbing charges.';
{Vampire Bite   }     25 : otw := 'losing strength and experience';
{Turn to Stone  }     26 : otw := 'turning to stone.';
{Mind Flayer    }     27 : otw := 'sucking your brain out.';
{Hell Horse	}     28 : otw := 'NightMare blindness.';
{Vargouille	}     29 : otw := 'draining max hit points.';
{Diseases       }     30 : otw := 'causes disease.';
{Lose Charisma  }     31 : otw := 'lose charisma.';
{Special        }     99 : otw := 'blank message.';
                      otherwise otw := '**Unknown value**';
   end;
   nbt_out_val:= 'It ' +attack_desc[adesc]+ 'for ' + otw + ' (' +damstr+ ')';
   prt(nbt_out_val,((cur_line + 1) div 2),1);
   cur_line := cur_line + 2;
  end;
end;
pause(24);
Draw_cave;
end;

{Displays info about monsters -NBT}
procedure mon_info;

Var choice : char;
    quitflag : boolean;
    howmany,i,j,moncode,aux_code : integer;
    namestr,dummy : vtype;

Begin
   choice := ' ';
   quitflag := true;
   Repeat
      if (get_com
('Enter creature code for information, ? for the list, or <Esc> to abort: '
         ,choice)) then
        begin
         quitflag := false;
         if choice = '?' then do_all_mons;
        end;
   Until ((choice <> '?') or quitflag);
   If not(quitflag) then
   Begin
      clear (1,1);
      howmany := 0;
      for i := 1 to max_creatures do
         if c_list[i].cchar = choice then howmany := howmany + 1;
      if howmany = 0 then
        Begin
         prt ('That character is not used for any creatures.',12,1);
         pause (24);
	 draw_cave;
        end
       else
        if howmany = 1 then
          begin
             for i := 1 to max_creatures do
                if c_list[i].cchar = choice then moncode := i;
             do_mon_out(moncode);
          end
        else
          begin { there's more than one creature in the list using that code }
	     howmany := 0;
	     j := 1;
	     for i := 1 to max_creatures do
	       if c_list[i].cchar = choice then
 		begin
                   namestr := '';
		   namestr := '    ' + c_list[i].name +
                           '                       ';
		   prt(namestr,(4+(howmany div 2)),(1+(40*(howmany mod 2))));
		   prt_num ( '',j,(4+(howmany div 2)),(1+40*(howmany mod 2)));
		   howmany := howmany + 1;
		   j := j + 1;
		end;
	     Repeat
		erase_line(msg_line,msg_line);
		msg_print('Enter the number for your choice:');
		get_string(dummy,1,36,10);
		aux_code := 999;
		aux_code := strtoint(dummy);
	     Until ((aux_code > 0) and (aux_code <= howmany));
	     howmany := 0;
	     i := 0;
	     while howmany <> aux_code do
               Begin
		i := i + 1;
		if c_list[i].cchar = choice then howmany := howmany + 1;
	       end;
	     moncode := i;
	     do_mon_out(moncode);
	  end;
   end;
end;


{Help for available wizard commands}
procedure wizard_help;
    begin
      clear(1,1);
          prt('^A - Remove Curse and Cure all maladies.',1,1);
          prt('^B - Print random objects sample.',2,1);
          prt('^C - Make nice character.',3,1);
          prt('^D - Down/Up n levels. Or n*100 to go to town #n',4,1);
          prt('^E - Change character.',5,1);
          prt('^F - Delete monsters.',6,1);
          prt('^G - Allocate treasures.',7,1);
          prt('^H - Wizard Help.',8,1);
          prt('^I - Identify.',9,1);
          prt('^J - Gain experience.',10,1);
          prt('^K - Summon monster.',11,1);
          prt('^L - Wizard light.',12,1);
          prt('^N - Print monster dictionary.',13,1);
          prt('^O - Make Wierdness Occur.',14,1);
          prt('^P - Wizard password on/off.',15,1);
          prt('^T - Teleport player.',16,1);       
	  prt('^U - Examine character values. (debugging).',17,1);
          prt('^V - Restore lost character.',18,1);
          prt('^W - Create any object *CAN CAUSE FATAL ERROR*',19,1);
          prt('^X - Examine an inventory item in detail.',20,1);
      pause(24);
      draw_cave;
    end;
 
 
 
{Disabled. -MB}
procedure moria_help(help_level : vtype);
 
    begin
      prt('Help files should be available in /doc directory.',1,1);
    end;


 
//    begin
//    end;
