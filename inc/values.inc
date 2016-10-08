  value
 
        wizard := false;
        used_line  := (22 of false);
        death      := false;
	more_flag  := true;
        stat_column := 1;
        day   := 1;	
	total_winner_num := 0;
	is_boss_dead := false;
	defeated_badguy := false;
	out_of_time := false;
	town_num := 1;
	wall_flag := true;
        pclass := 0;
        print_stat := 0;
	act_bal := 0;	principal := 0; payment_count := 0;
	iod_val := 0;	iol_val := 0;	danny_cut := 0;
{ For the testing of bits }
        bit_array := (
{ Bit #1 }     %X'00000001',{ Bit #2 }     %X'00000002',
{ Bit #3 }     %X'00000004',{ Bit #4 }     %X'00000008',
{ Bit #5 }     %X'00000010',{ Bit #6 }     %X'00000020',
{ Bit #7 }     %X'00000040',{ Bit #8 }     %X'00000080',
{ Bit #9 }     %X'00000100',{ Bit #10 }    %X'00000200',
{ Bit #11 }    %X'00000400',{ Bit #12 }    %X'00000800',
{ Bit #13 }    %X'00001000',{ Bit #14 }    %X'00002000',
{ Bit #15 }    %X'00004000',{ Bit #16 }    %X'00008000',
{ Bit #17 }    %X'00010000',{ Bit #18 }    %X'00020000',
{ Bit #19 }    %X'00040000',{ Bit #20 }    %X'00080000',
{ Bit #21 }    %X'00100000',{ Bit #22 }    %X'00200000',
{ Bit #23 }    %X'00400000',{ Bit #24 }    %X'00800000',
{ Bit #25 }    %X'01000000',{ Bit #26 }    %X'02000000',
{ Bit #27 }    %X'04000000',{ Bit #28 }    %X'08000000',
{ Bit #29 }    %X'10000000',{ Bit #30 }    %X'20000000',
{ Bit #31 }    %X'40000000',{ Bit #32 }    %X'80000000');
 
        { Operating hours for Moria
               X = Open; . = Closed  }
        days    :=(
                'SUN:........................|',
                'MON:........................|',
                'TUE:........................|',
                'WED:........................|',
                'THU:........................|',
                'FRI:........................|',
                'SAT:........................|');
 
        closing_flag := 0;
 
        { VALUES FOR FLOOR TYPES }
        blank_floor:= (0,0,0,false,false,false,false);
        dopen_floor  := (  1,true ); { Dark open floor }    {DO NOT CHANGE!}
        lopen_floor  := (  2,true ); { Light open floor }
        corr_floor1  := (  4,true ); { Corridor open floor }
        corr_floor2  := (  5,true ); { Room junction marker }
        corr_floor3  := (  6,true ); { Door type floor }
        corr_floor4  := (  7,false); { Secret door type floor
          FLOOR VALUES 8 AND 9 ARE USED IN GENERATE }
        rock_wall1   := ( 10,false); { Granite rock wall }
        rock_wall2   := ( 11,false); { Magma rock wall }
        rock_wall3   := ( 12,false); { Quartz rock wall }
        boundry_wall := ( 15,false); { Indestructable wall }
 
        { PLAYER RECORD IS SPECIAL }
        py              :=      (
        (' ',' ',' ',' ',' ',
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         ('','','','','')),(0,0,0,0,0,0,0,0,0,0,0,0),
        (0,0,0,0,0,5000,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         false,false,false,false,false,false,false,false,false,
         false,false,false,false,false,false,false,false,false));
 
        bare_hands := '1d2';
        acc_exp    := 0.0;


        { For Create Food spell use... }
mush := ('& Can~ of Beans and Weenies' ,80,',',%X'00000000',
2400, 0, 308, 1, 1,0, 0, 0, 0, '0d0' , 1);
 
        char_row := 0;
        char_col := 0;
 
        { Initialize player generation variables }
        sex_type  := 'FemaleMale  ';
 
        {Race   STR,INT,WIS,DEX,CON,CHR,
                Ages, heights, and weights,
                Racial Bases for: bdis,bsrh,bstl,bperc,bbth,bbthb,bdev,bsav,
		bhitdie,binfra, choice-classes }
{ Class choice bit string goes (from MSB to LSB): Thief, Con-Artist,
  Wanderer, Soldier, Ninja, Guru, Scientist, Wrestler }
 
        race :=  (
             ('Elite',1,1,1,1,1,1,20,30,72,6,180,25,66,4,120,20,1.40,
                         15,10, 3,10,15,15,10,5,6,0,%B'11111111'),
             ('Hefty',5,0,0,0,1,0,19,30,72,6,210,25,66,4,150,20,1.30,
                          5, 5, 2, 5,20,-5,10,10,5,0,%B'11111111'),
               ('Mad',0,5,0,1,0,0,25,45,60,6,150,25,54,4,100,20,1.00,
                          5, 5, 2, 5,-5, 5,20,5,2,0,%B'11111111'),
              ('Wise',0,1,5,0,0,0,35,50,70,6,150,25,66,4,100,20,1.20,
                          5,15, 2, 5,-5, 5,10,5,4,0,%B'11111111'),
             ('Lucky',0,0,1,5,0,0,18,30,72,6,150,25,66,4,100,20,1.40,
                         20,10, 2,15, 5,20,10,15,6,0,%B'11111111'),
           ('Healthy',1,0,0,0,5,0,20,30,72,6,150,25,66,4,100,20,1.60,
                          5, 5, 2,10,10, 5,10,5,8,0,%B'11111111'),
              ('Sexy',0,0,0,1,0,5,17,25,72,6,150,25,66,4,100,20,1.40,
                          5, 5, 2, 5, 5, 5,10,20,6,0,%B'11111111'),
             ('Alien',3,3,-3,3,3,-3,100,1000,72,6,150,25,66,4,100,20,1.40,
                          5, 5, 3,10,15,10,30,10,6,8,%B'10111011'));
 
{ Buying and selling adjustments for character race VS store owner race }
 
        rgold_adj :=  (      {Character Class:}

{StoreOwner:}  {  Wrs,  Sci,  Gru,  Nnj,  Sol,  Wnd,  Con,  Thf  Test}

{Wrestler   }  (-0.08, 0.06, 0.02,-0.04,-0.02, 0.02,-0.10, 0.00, 0.00),
{Scientist  }  ( 0.04,-0.08, 0.06, 0.02, 0.00, 0.04,-0.04,-0.04, 0.00),
{Guru       }  ( 0.00, 0.02,-0.04, 0.02, 0.04,-0.06, 0.00,-0.04, 0.00),
{Ninja      }  (-0.02, 0.00,-0.02,-0.06,-0.04, 0.00,-0.02, 0.00, 0.00),
{Soldier    }  (-0.02, 0.00, 0.04,-0.02,-0.08, 0.02,-0.04, 0.00, 0.00),
{Wanderer   }  ( 0.02, 0.04,-0.08,-0.04, 0.00,-0.08,-0.04,-0.04, 0.00),
{Con-Artist }  ( 0.10, 0.08, 0.00, 0.08, 0.08, 0.08, 0.00, 0.06, 0.08),
{Thief      }  ( 0.06, 0.06, 0.00, 0.06, 0.06, 0.06,-0.02, 0.00, 0.06),
{Test       }  ( 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00));
 

{ Classes...}
        class := (

{ Class	        Exp   HD Dis Src Stl Per Bth Bhb Wrd Sav  S  I  W  D  C  C sk#}
('Wrestler'    ,0.10, 15, 10, 10,  0,  5, 45, 10, -5, 20, 4,-1,-1, 1, 3,-1, 0),
('Scientist'   ,0.50,  2, 10, 10,  2, 10, 15, 20, 10, 20,-3, 3, 1, 1, 0, 0, 1),
('Guru'        ,0.30,  4,  8, 10,  2, 30, 30, 10,  5, 25,-1,-1, 3,-1, 2, 2, 3),
('Ninja'       ,0.10, 10, 15, 22,  4, 25, 40, 45,  0, 40, 2, 1, 2, 3, 0,-1, 0),
('Soldier'     ,0.30,  8, 10, 15,  3, 20, 35, 50,  5, 25, 2, 2, 1, 2, 1, 1, 2),
('Wanderer'    ,0.10,  8,  8, 10,  2, 15, 45, 35,  0, 20, 1,-1, 2, 1, 1, 1, 4),
('Con-Artist'  ,0.50,  6, 13, 10,  2, 15, 30, 30,  0, 25, 2, 0, 0, 2, 1, 3, 5),
('Thief'       ,0.30,  8, 15, 15,  3, 15, 30, 30,  0, 25, 3, 0, 0, 2, 2, 1, 6),
('Journalist'  ,0.20,  6, 10, 10,  3, 20, 20, 30,  0, 25, 0, 3, 0, 2, 0, 0, 7)
);
 
{ Class titles for different levels }
        player_title := (

{ Wrestler }
('Jock Strap','Jock Strap',
'Trainee','Trainee','Trainee','Trainee','Trainee','Trainee',
'Athlete','Athlete','Athlete','Athlete','Athlete','Athlete',
'Jock','Jock','Jock','Jock','Jock','Jock',
'Hunk','Hunk','Hunk','Hunk','Hunk','Hunk',
'Stud','Stud','Stud','Stud','Stud','Stud',
'Champ','Champ','Champ','Champ',
'Pro','Pro','Pro','Pro','Hulk','Hulk'),
	
{ Scientist }
('In the dark','In the dark','Sees the Light','Beginner',
'Beginner','Student','Student','Novice','Novice','Hobbiest',
'Hobbiest','Tinkerer','Tinkerer','Frosh','Frosh','Sophomore','Sophomore',
'Junior','Junior','Lab Tech','Lab Tech','Senior','Senior','Baccalauriate',
'Graduate','Grad. Student','Grad. Student','Doctor','Doctor','Professor',
'Professor','Ph.D.','Researcher','Researcher','Genius','Genius',
'Nobel Winner','Nobel Winner','Galileo','Newton','Fermi','Einstein'),

{ Guru }
('Believer','Acolyte','Acolyte','Acolyte','Adept','Adept','Adept','Priest',
'Priest','Priest','Priest','Priest','Priest','Priest','Priest',
'Priest','Curate','Curate','Curate','Curate','Curate','Curate','Curate',
'Curate','Curate','Canon','Canon','Canon','Canon','Canon',
'Low Lama','Lama','Lama','Lama','High Lama','Great Lama','Patriarch',
'High Priest','Great Priest','Noble Priest','Grand Poobah','Buddah'),

{ Ninja }
('Apprentice','Apprentice','Apprentice','Apprentice',
'Warrior','Warrior','Warrior','Warrior','Warrior','Warrior',
'Swordsman','Swordsman','Swordsman','Swordsman','Swordsman','Swordsman',
'Defender','Defender','Defender','Defender','Defender','Defender',
'Guardian','Guardian','Guardian','Guardian','Guardian','Guardian',
'Noble','Noble','Noble','Noble','Noble','Noble',
'Samuri','Samuri','Samuri','Samuri','Samuri','Samuri',
'Samuri Lord','Samuri Lord'),

{ Soldier }
('Street Punk','Street Punk','Recruit','Recruit','Trainee','Trainee',
'Cadet','Cadet','Private','Private','Prv. 1st Class','Prv. 1st Class',
'Corporal','Corporal','Sargeant','Sargeant','Sargeant','Sargeant',
'Master Sarg.','Master Srg.','2nd Lieut.','2nd Lieut.','1st Lieut.',
'1st Lieut.','Captain','Captain','Captian','Captain','Major','Major',
'Major','Major','Lt. Col.','Lt. Col.','Lt. Col','Lt. Col.','Colonel',
'Colonel','Colonel','Colonel','Hero','Hero'),

{ Wanderer }
('Gallant','Keeper','Keeper','Keeper','Keeper','Keeper','Keeper','Keeper',
'Keeper','Keeper','Protector','Protector','Protector','Protector','Protector',
'Protector','Protector','Protector','Defender','Defender','Defender',
'Defender','Defender','Defender','Defender','Defender','Warder','Warder',
'Warder','Warder','Warder','Warder','Warder','Warder','Warder','Gauardian',
'Chevalier','Justiciar','Paladin','High Lord','High Lord','High Lord'),

{ Con-Artist }
('Street Scum','Street Scum','Street Scum','Street Scum',
'Fingers','Fingers','Fingers','Fingers','Fingers','Fingers',
'Thief','Thief','Thief','Thief','Thief','Thief',
'Hustler','Hustler','Hustler','Hustler','Hustler','Hustler',
'Con','Con','Con','Con','Con','Con','Con','Con','Con','Con',
'Pro','Pro','Pro','Pro','Pro','Pro','Pro','Pro','Ice','Ice'),
 
{ Thief }
('Apprentice','Footpad','Cutpurse','Robber','Burglar','Filcher','Sharper',
'Magsman','Common Rogue','Rogue','Rogue','Rogue','Rogue','Rogue','Rogue',
'Rogue','Rogue','Rogue','Master Rogue','Expert Rogue','Senior Rogue',
'Chief Rogue','Prime Rogue','Low Thief','Thief','Thief','Thief',
'Thief','Thief','Thief','Thief','Thief','Thief','High Thief','Master Thief',
'Executioner','Low Assassin','Assassin','HighAssassin','Guildsmaster',
'Guildsmaster','Guildsmaster'),

{ Test }
('Loser','Loser','Bubble','Bubble','Frisbee','Frisbee','Spoon','Spoon',
'Silly','Silly','Warlord','Warlord','Plastic','Plastic','Plan9','Plan9',
'AliceB','AliceB','Maven','Maven','Byron','Byron','Milton','Milton',
'IBM','IBM','ttyp4','ttyp4','Ularn','Ularn','Dice','Dice','Raoul','Raoul',
'Ghost','Ghost','Blegga','Blegga','Free','Free','Association','Association'));
 
{Base experience levels, may be adjusted up for race and/or class}
        player_exp :=   (
{0,}10,20,40,80,140,220,290,370,500,
600,800,1100,1500,2000,2600,3300,4100,5000,6000,
7200,8800,11200,14400,19200,24400,30000,36000,42400,49000,
64000,81000,100000,121000,144000,169000,196000,225000,256000,300000,
400000,750000,9999999);


{ Background information }
        background := (
('You are the illegitimate and unacknowledged child ', 10, 1, 2,-25),
('You are the illegitimate but acknowledged child ', 20, 1, 2,-15),
('You are the only child ', 50, 1, 2, 10),
('You are the favorite child ', 100, 1, 2,10),
('of a Drunken Gardener.  ', 10, 2, 3, -20),
('of a Social Worker.  ', 30, 2, 3, 20),
('of a Policeman.  ', 50, 2, 3, 40),
('of a Businessman.  ', 70, 2, 3, 55),
('of a Doctor.  ', 80, 2, 3, 70),
('of a Programming Analyist.  ', 90, 2, 3, 40),
('of a Senator.  ', 100, 2, 3, 90),
('You have spent years in juvanile hall.  ', 10, 3,50,-30),
('You are a credit to the family.  ', 80, 3,50,5),
('You graduated first in your class.  ', 100, 3,50, 30),

('You are the illegitimate and unacknowledged child ', 10, 4, 5,-25),
('You are the illegitimate but acknowledged child ', 20, 4, 5,-15),
('You are the strongest child ', 80, 4, 5, 10),
('You are the favorite child ', 100, 4, 5,10),
('of a Vacuum Cleaner Salesman.  ', 10, 5, 6,-10),
('of a Pro-Wrestler.  ', 60, 5, 6,50),
('of a Lawyer.  ', 70, 5, 6,20),
('of a Soldier.  ', 80, 5, 6, 10),
('of a Philosopher.  ', 90, 5, 6, 15),
('of a Govenor.  ', 100, 5, 6, 70),
('You''ve owned three street gangs.  ', 10, 6,50,-30),
('You are a credit to the family.  ', 90, 6,50,5),
('You''ve never lost a wrestling match!  ', 100, 6,50, 50),

('You are the illegitimate and unacknowledged child ', 10, 7, 8,-25),
('You are the strangest child ', 80, 7, 8, 10),
('You are the favorite child ', 100, 7, 8,20),
('of a Bartender.  ', 10, 8, 9,0),
('of a Football Coach.  ', 30, 8, 9,5),
('of a Unix Programmer.  ', 50, 8, 9, 10),
('of a Scientist.  ', 80, 8, 9, 30),
('of a Doctor.  ', 100, 8, 9, 40),
('A fusion experiment of your''s has destroyed a small town.  ',5,9,54,-30),
('A biology experiment of your''s unleashed killer tomatoes.  ',10,9,54,-30),
('You are a credit to society.  ', 80, 9, 54,30),
('You have won a nobel prize.  ', 100, 9, 54,50),

('You are one of several children of a ', 50,10,11, -5),
('You are the only child of a ', 100,10,11,5),
('Gardener.  ', 10,11, 12,5),
('Tavern Owner.  ', 20,11, 12, -10),
('Political Analyist.  ', 30,11, 12, 20),
('Steel Worker.  ', 40,11, 12, 20),
('Real Estate Agent.  ',50,11, 12, 20),
('Wanderer.  ', 60,11, 12, 65),
('Theologist.  ', 70,11, 12, 75),
('Guru.  ', 100, 11, 12, 90),
('Your wisdom has led you only to sleazy taverns.  ',10,12,50,-30),
('You are a credit to your family.  ', 60, 12, 50,20),
('You have achieved oneness with yourself.  ', 100, 12, 50,30),

('You are one of several children of a  ', 85,13,14, -5),
('You are the only child of a  ', 100,13,14,5),
('Thief.  ',20,14, 15,-30),
('Chimney Sweep. ', 50,14, 15, 10),
('Con-Artist.  ', 75,14, 15, 0),
('VMS Programmer.  ', 95,14, 15, 20),
('Millionaire.  ', 100,14, 15, 75),
('Your luck may have have run out!  ',20,15,50,-10),
('You are the hero of your town.  ', 80, 15, 50,50),
('Your luck has made you a fortune!  ', 100, 15, 50,100),


('You are the proud child of a ', 50,16,17,0),
('You are the only child of a ', 100,16,17,0),
('Hypocondriac.  ', 20,17,18, -10),
('Prison Guard.  ', 30,17,18, 0),
('Politician.  ', 50,17,18, 40),
('Dentist.  ', 70,17,18, 60),
('Doctor.  ', 90,17,18, 80),
('Gold Medalist.  ', 100,17,18,80),
('You can out drink a rhino.  ', 15,18,50,-20),
('You are a credit to the family.  ', 85,18,50,0),
('Medical science is in awe of your health.  ', 100,18,50,50),

('You are the adopted child of a ' , 25,19,20,-10),
('You are the cutest child of a ' ,60,19,20,30),
('You are the only child of a ', 100,19,20,40),
('Farmer.  ', 10,20,21, -10),
('Hockey player.  ', 30,20,21, 30),
('Magazine Publisher.  ', 50,20,21, 60),
('Author.  ', 70,20,21, 60),
('Used Car Salesman.  ', 95,20,21, -10),
('Billionaire.  ', 100,20,21,100),
('Your mother sold all your nude baby pictures.  ', 20,21,58,-10),
('You are a credit to your family.  ', 85,21,58,10),
('You are society''s sex model.  ', 100,21,58,80),


('You are the rejected child of a ', 10,22,23,-20),
('You are the confused child of a ', 80,22,23,30),
('You are the proud child of a ', 100,22,23, 50),
('Jarwangian Thief.  ', 20,23,24, -40),
('Jarwangian Soldier.  ', 50,23,24,10),
('Jarwangian Scientist.  ', 90,23,24, 30),
('Jarwangian Diplomat.  ', 100,23,24, 50),
('You have made many enemies among the humans.  ',20,24,62,-30),
('You are a stable member in the human community.  ', 85,24,62,20),
('You are a favorite alien of many humans.  ', 100,24,62,50),

('You have dark brown eyes, ', 20,50,51,-4),
('You have brown eyes, ', 60,50,51,0),
('You have hazel eyes, ', 70,50,51,1),
('You have green eyes, ', 80,50,51,3),
('You have blue eyes, ', 90,50,51,5),
('You have blue-gray eyes, ', 100,50,51,-6),
('straight ', 70,51,52,0),
('wavey ', 90,51,52,1),
('curly ', 100,51,52,3),
('black hair, ', 30,52,53,1),
('brown hair, ', 70,52,53,3),
('auburn hair, ', 80,52,53,-3),
('red hair, ', 90,52,53,4),
('blonde hair, ', 100,52,53,-1),
('and a very dark complexion.', 10,53,-1,1),
('and a dark complexion.', 30,53,-1,3),
('and an average complexion.', 80,53,-1,-3),
('and a fair complexion.', 90,53,-1,4),
('and a very fair complexion.', 100,53,-1,5),

('You have small beady eyes, ', 30,54,55,0),
('You have brown eyes, ', 60,54,55,0),
('You have blue eyes, ', 90,54,55,0),
('You have glowing red eyes, ', 100,54,55,0),
('tossled ', 50,55,56,0),
('straight ', 75,55,56,0),
('wavey ', 100,55,56,0),
('grey hair, and a fair complexion.', 10,56,-1,0),
('brown hair, and a fair complexion.', 30,56,-1,0),
('white hair, and a fair complexion.',80,56,-1,0),
('silver hair, and a fair complexion.', 100,56,-1,0),

('You have alluring eyes, ', 20,58,59,0),
('You have mesmerising eyes, ', 40,58,59,0),
('You have blue eyes, ', 60,58,59,0),
('You have dark brown eyes, ', 80,58,59,0),
('You have nice eyes, ', 100,58,59,0),
('flowing ', 30,59,60,0),
('wavey ', 60,59,60,0),
('curly ', 100,59,60,0),
('black hair, ', 30,60,61,0),
('brown hair, ', 70,60,61,0),
('auburn hair, ', 80,60,61,0),
('red hair, ', 90,60,61,0),
('blonde hair, ', 100,60,61,0),
('and a dark complexion.', 33,61,-1,0),
('and a smooth complexion.', 66,61,-1,0),
('and a tanned complexion.', 100,61,-1,0),

('You have orange eyes, ', 33,62,63,0),
('You have glowing eyes, ', 66,62,63,0),
('You have very large eyes, ', 100,62,63,0),
('straight ', 33,63,64,0),
('wavey ', 66,63,64,0),
('mottled ', 100,63,64,0),
('purple hair, ', 33,64,65,0),
('orange hair, ', 66,64,65,0),
('blue hair, ', 100,64,65,0),
('and a dark complexion.', 33,65,-1,0),
('and a smooth complexion.', 66,65,-1,0),
('and a plastic complexion.', 100,65,-1,0)
);
 
{ Each store will buy only certain items,based on TVAL }
        store_buy := (
{K}     [2,3,30,32,77,15],
{L}     [30,31,32,33,34,35,36],
{W}     [21,23,82,84],
{T}     [45,40,91],
{+}     [75,76,80],
{A}     [40,45,55,65],
{B}	[9,10,11,12,19,20,21,23,29,30,31,32,33,34,35,40,45,
	   55,60,65,70,71,75,76,82,84,90,91,92],
{G}     [9,10,11,12,19,20],
{P}     [23,33],
{C}     [29,70,71,90],
{#}     [13,25,92]
);
 
{ Store owners have different characteristics for pricing }

        owners := (
(('Penny Preatty       (Wanderer)      K-Mart',		   1,0.04,0.40,6),
 ('Leisure Suit Larry  (Scientist)     K-Mart',		   1,0.06,0.40,2),
 ('Charlie Yatchet     (Con-Artist)    K-Mart',		   1,0.08,0.45,7),
 ('W.C. Meadows        (Guru)          K-Mart',		   1,0.10,0.45,3),
 ('John		       (Guitar)        K-Mart',		   1,0.12,0.50,9),
 ('Ringo	       (Drummer)       K-Mart',		   1,0.15,0.50,9),
 ('Paul		       (Bass)          K-Mart',		   1,0.20,0.55,9),
 ('George	       (Sitar)         K-Mart',		   1,0.30,0.55,9)),

(('The Hulkster        (Wrestler)      Leather & Chains',   1,0.06,0.40,1),
 ('Crazy Eddie         (Soldier)       Leather & Chains',   1,0.08,0.40,5),
 ('Steel-Fist Thelma   (Wrestler)      Leather & Chains',   1,0.10,0.45,1),
 ('James Bondage       (Wrestler)      Leather & Chains',   1,0.12,0.45,1),
 ('Major Disaster      (Soldier)       Leather & Chains',   1,0.15,0.50,5),
 ('Corporal Punishment (Soldier)       Leather & Chains',   1,0.20,0.50,5),
 ('Olga                (Wrestler)      Leather & Chains',   1,0.30,0.55,1),
 ('Cool Hand Fluke     (Soldier)       Leather & Chains',   1,0.40,0.55,5)),

(('Chainsaw Mike       (Soldier)       Weapon Shop',	   1,0.06,0.40,5),
 ('Samuri Sven         (Ninja)         Weapon Shop',	   1,0.08,0.40,4),
 ('Jon Wane	       (Soldier)       Weapon Shop',	   1,0.10,0.45,5),
 ('The Man From Aunt   (Soldier)       Weapon Shop',	   1,0.12,0.45,5),
 ('Killroy             (Ninja)         Weapon Shop',	   1,0.15,0.50,4),
 ('Sargent Granite     (Soldier)       Weapon Shop',	   1,0.20,0.50,5),
 ('Col. Mustardseed    (Soldier)       Weapon Shop',	   1,0.30,0.55,5),
 ('Fluffy	       (Teddy Bear)    Weapon Shop',	   1,0.40,0.55,9)),

(('Shari the Humble    (Guru)          Temple',		   1,0.04,0.40,3),
 ('Graceful Gretchen   (Wanderer)      Temple',		   1,0.06,0.40,6),
 ('Cosmo the Enlightened  (Guru)       Temple',		   1,0.08,0.45,3),
 ('Elvis	       (The King)      Temple',		   1,0.10,0.45,9),
 ('Elron Cupboard      (Guru)	       Temple',		   1,0.12,0.50,3),
 ('Tammy Fay Bacon     (Guru)          Temple',		   1,0.15,0.50,3),
 ('Hal 9000-B	       (Computer)      Temple',		   1,0.20,0.55,9),
 ('Okra Winfreid       (Guru)	       Temple',		   1,0.30,0.55,3)),

(('Pop Poolstone       (Scientist)     Bar and Grill',	   1,0.04,0.40,2),
 ('Benny the Wino      (Ninja)         Bar and Grill',	   1,0.06,0.40,6),
 ('Nuke-Brained Nick   (Con-Artist)    Bar and Grill',	   1,0.08,0.45,7),
 ('Yo-Gurt             (Guru)          Bar and Grill',	   1,0.10,0.45,3),
 ('Hash-Berry	       (Ninja)         Bar and Grill',	   1,0.12,0.50,4),
 ('Mike, Ike, and Jim  (Con-Artists)   Bar and Grill',	   1,0.15,0.50,7),
 ('Jabba the Pizza Hut (Scientist)     Bar and Grill',	   1,0.20,0.55,2),
 ('Donald McRonald     (Ninja)         Bar and Grill',	   1,0.30,0.55,4)),

(('Dr. StrangeLoaf     (Scientist)     Alien Artifacts',   1,0.06,0.40,2),
 ('Missing Linke       (Guru)          Alien Artifacts',   1,0.08,0.40,3),
 ('Professor X         (Scientist)     Alien Artifacts',   1,0.10,0.45,2),
 ('Chewy Bacon	       (Scientist)     Alien Artifacts',   1,0.12,0.45,2),
 ('Mr. Spot 	       (Guru)          Alien Artifacts',   1,0.15,0.50,3),
 ('E.Z.		       (Scientist)     Alien Artifacts',   1,0.20,0.50,2),
 ('Ham Salad	       (Scientist)     Alien Artifacts',   1,0.30,0.55,2),
 ('Mr. Dataset	       (Con-Artist)    Alien Artifacts',   1,0.40,0.55,7)),

(('Lt. Wharf           (Soldier)       Black Market',	   1,0.30,0.50,5),
 ('The Butcher         (Thief)         Black Market',	   1,0.35,0.50,8),
 ('Smith               (Con-Artist)    Black Market',	   1,0.40,0.55,7),
 ('Captain Quirk       (Soldier)       Black Market',	   1,0.50,0.55,5),
 ('Guido               (Thief)         Black Market',	   1,0.60,0.60,8),
 ('Grandma Wormtongue  (Con-Artist)    Black Market',	   1,0.70,0.60,7),
 ('Sun-Tsu             (Soldier)       Black Market',	   1,0.80,0.70,5),
 ('The Butcher         (Thief)         Black Market',	   1,0.90,0.70,8)),

(('Ginsing Gary        (Ninja)         Gun Shop',	   1,0.06,0.40,4),
 ('Wilber Samaroo      (Soldier)       Gun Shop',	   1,0.08,0.40,5),
 ('Queue	       (Scientist)     Gun Shop',	   1,0.10,0.45,2),
 ('Mr. Krueger         (Ninja)         Gun Shop',	   1,0.12,0.45,4),
 ('Wargha              (Scientist)     Gun Shop',	   1,0.15,0.50,2),
 ('Gramdpa Wormtongue  (Con-Artist)    Gun Shop',	   1,0.20,0.50,7),
 ('Helga               (Wanderer)      Gun Shop',	   1,0.30,0.55,6),
 ('Norma Bates	       (Psycho)	       Gun Shop',	   1,0.40,0.55,9)),

(('Big Jake            (Wrestler)      Pro Shop',	   1,0.06,0.40,1),
 ('Coach Crawldirt     (Soldier)       Pro Shop',	   1,0.08,0.40,5),
 ('Ski-Bunny Betty     (Wanderer)      Pro Shop',	   1,0.10,0.45,6),
 ('Mr. Buck Rogers     (Wrestler)      Pro Shop',	   1,0.12,0.45,1),
 ('Admiral Skeet       (Soldier)       Pro Shop',	   1,0.15,0.50,5),
 ('Bjorn Boring	       (Wanderer)      Pro Shop',	   1,0.20,0.50,6),
 ('Nadia Comb-an-inch  (Wrestler)      Pro Shop',	   1,0.30,0.55,1),
 ('Err Jordan	       (Soldier)       Pro Shop',	   1,0.40,0.55,5)),

(('Mr. Solar           (Scientist)     Computer Store',	   1,0.06,0.40,2),
 ('Dr. John Smith      (Scientist)     Computer Store',	   1,0.08,0.40,2),
 ('Zapper              (Mad Scientist) Computer Store',	   1,0.10,0.45,2),
 ('Bill Grates         (Scientist)     Computer Store',	   1,0.12,0.45,2),
 ('Steve Batch Jobs    (Scientist)     Computer Store',	   1,0.15,0.50,2),
 ('Donald Canoeth      (Scientist)     Computer Store',	   1,0.20,0.50,2),
 ('Jimmy Wayne Todd    (Programmer)    Computer Store',	   1,0.30,0.55,2),
 ('Robert Alan Koeneke (Programmer)    Computer Store',	   1,0.40,0.55,2)),

(('cloister            (Soldier)       Guild',		   1,0.06,0.40,5),
 ('Cutpurse Mallone    (Thief)         Guild',		   1,0.08,0.40,8),
 ('Lightfingered Lisa  (Con-Artist)    Guild',		   1,0.10,0.45,7),
 ('Peg Leg Pete        (Soldier)       Guild',		   1,0.12,0.45,5),
 ('Captain Blight      (Soldier)       Guild',		   1,0.15,0.50,5),
 ('Russell Fingers     (Con-Artist)    Guild',		   1,0.20,0.50,7),
 ('Milo Minderbender   (Soldier)       Guild',		   1,0.30,0.55,5),
 ('Six Fingered Louie  (Thief)         Guild',		   1,0.40,0.55,8)));

   joblist := ('Dish Washer','Boiler Room Worker','Bellhop',
	       'Doorman','Security Guard','Gourmet Chef',
	       'Body Guard','Accountant','Hotel Manager');

   jobwage := (3,5,7,9,11,15,25,40,90);

   townlist := (

('Seattle',10,0),
('Boise',10,6),
('Denver',10,12),
('Kansas City',10,18),
('Chicago',10,24),
('Detroit',10,30),
('Pittsburgh',10,36),
('New York City',50,42)  
);

  berz_y  := (0,-1,-1,-1,0,1,1,1);
  berz_x  := (-1,-1,0,1,1,1,0,-1);

  circle := (3,3,3,2,2,2,1,1,0);


 
{ Treasure related values }
        inven_ctr := 0;
        inven_weight := 0;
        missle_ctr := 0;
        equip_ctr := 0;
        blank_treasure := (' ',0,' ',0,0,0,0,0,0,0,0,0,0,' ',0);
 
{ Gold list (All types of gold and gems are defined here) }
        gold_list := (
('nickels', 100,      '$'   ,%X'00000000', 0,  1,2,0,0,0,0,0,0, ' ' , 1),
('arcade tokens',100, '$'   ,%X'00000000', 0,  1,2,0,0,0,0,0,0, ' ' , 1),
('dimes', 100,        '$'   ,%X'00000000', 0,  2,2,0,0,0,0,0,0, ' ' , 1),
('trinkets',100,      '*'   ,%X'00000000', 0,  2,3,0,0,0,0,0,0, ' ' , 1),
('blank disks',100,   '*'   ,%X'00000000', 0,  3,3,0,0,0,0,0,0, ' ' , 1),
('postage stamps',100,'$'   ,%X'00000000', 0,  3,1,0,0,0,0,0,0, ' ' , 1),
('loose change', 100, '$'   ,%X'00000000', 0,  5,2,0,0,0,0,0,0, ' ' , 1),
('pez dispensers' ,100, '*' ,%X'00000000', 0,  5,3,0,0,0,0,0,0, ' ' , 1),
('bubble gum', 100,   '*'   ,%X'00000000', 0,  8,3,0,0,0,0,0,0, ' ' , 1),
('gold fillings',100,   '*' ,%X'00000000', 0, 10,3,0,0,0,0,0,0, ' ' , 1),
('foreign bills', 100,'$'   ,%X'00000000', 0, 10,1,0,0,0,0,0,0, ' ' , 1),
('old comic books',100, '*' ,%X'00000000', 0, 15,3,0,0,0,0,0,0, ' ' , 1),
('$1 bills',100,      '$'   ,%X'00000000', 0, 15,1,0,0,0,0,0,0, ' ' , 1),
('food stamps',100,   '$'   ,%X'00000000', 0, 15,1,0,0,0,0,0,0, ' ' , 1),
('small bills',100,   '$'   ,%X'00000000', 0, 20,1,0,0,0,0,0,0, ' ' , 1),
('silicon chips' , 100, '*' ,%X'00000000', 0, 20,3,0,0,0,0,0,0, ' ' , 1),
('chic jewelry' ,100,   '*' ,%X'00000000', 0, 25,4,0,0,0,0,0,0, ' ' , 1),
('digital watches',100, '*' ,%X'00000000', 0, 30,3,0,0,0,0,0,0, ' ' , 1),
('car stereos' , 100,   '*' ,%X'00000000', 0, 50,4,0,0,0,0,0,0, ' ' , 1),
('large bills',100,   '$'   ,%X'00000000', 0, 60,1,0,0,0,0,0,0, ' ' , 1),
('illegal drugs', 100,  '*' ,%X'00000000', 0, 70,3,0,0,0,0,0,0, ' ' , 1),
('uncut diamonds',100,  '*' ,%X'00000000', 0, 75,3,0,0,0,0,0,0, ' ' , 1),
('silver coins',100,  '$'   ,%X'00000000', 0, 80,2,0,0,0,0,0,0, ' ' , 1),
('quarters',100,      '$'   ,%X'00000000', 0, 80,2,0,0,0,0,0,0, ' ' , 1),
('gold coins',100,    '$'   ,%X'00000000', 0,100,2,0,0,0,0,0,0, ' ' , 1),
('gold dust', 100,      '*' ,%X'00000000', 0,100,3,0,0,0,0,0,0, ' ' , 1),
('stock certificates',100,'$',%X'00000000',0,120,1,0,0,0,0,0,0, ' ' , 1),
('alien crystals',100,  '*' ,%X'00000000', 0,150,5,0,0,0,0,0,0, ' ' , 1),
('diamonds',100,        '*' ,%X'00000000', 0,200,3,0,0,0,0,0,0, ' ' , 1),
('alien crystals',100,  '*' ,%X'00000000', 0,300,5,0,0,0,0,0,0, ' ' , 1));
 
{ Descriptive phrases }
        colors  :=   ('Amber','Azure','Blue','Blue Speckled','Blue Spotted',
                      'Black','Black Speckled','Black Spotted','Brown',
                      'Brown Speckled','Brown Spotted','Bubbling',
                      'Chartreuse','Clear','Cloudy','Copper','Copper Spotted',
                      'Crimson','Cyan','Dark Blue','Dark Green','Dark Red',
                      'Ecru','Gold','Gold Spotted','Green','Green Speckled',
                      'Green Spotted','Grey','Grey Spotted','Hazy','Indigo',
                      'Light Blue','Light Green','Magenta','Metallic Blue',
                      'Metallic Red','Metallic Green','Metallic Purple',
                      'Misty','Orange','Orange Speckled','Orange Spotted',
                      'Pink','Pink Speckled','Plaid','Puce','Purple',
                      'Purple Speckled','Purple Spotted','Red','Red Speckled',
                      'Red Spotted','Silver','Silver Speckled',
                      'Silver Spotted','Smokey','Tan','Tangerine','Topaz',
                      'Turquoise','Violet','Vermillion','White',
                      'White Speckled','White Spotted','Yellow',
		      'Navy Blue','Striped','Maroon','Filmy','Holographic',
		      'Polka-Dot','Silly','Bloody');

        mushrooms := (
                      'Blue','Black','Brown','Copper','Crimson','Dark blue',
                      'Dark green','Dark red','Gold','Green','Grey',
                      'Light Blue','Light Green','Orange','Pink','Plaid',
                      'Purple','Red','Tan','Turquoise','Violet','White',
                      'Yellow','Wrinkled','Wooden','Slimey','Speckled',
                      'Spotted','Furry' );
        woods   :=   (
		      'Terra-cotta','Frenisium','Rectangular','Not there',
		      'Flammable','Icy','Gurgling','Broken',
		      'Bubbly','Crispy-crunchy','Deformed','Sluggish',
		      'Massy','Fossilized','Plaid flannel','Tacky',
		      'Objet-d''art','Very very tiny','Vibrating','Talkative',
		      'Half baked','Sauteed','Over easy','Bar-b-queued',
		      'MKS unit','Barrel-like','Polymerized','Phenelalenuric',
		      'Rabbit-fur','Teak','Edible','Bitchin''',
		      'Growing','Lethal','Kinky','Demon-spawn',
		      'Daisyian','Scaly','Warm','Soft');
        metals  :=   (
		     'Long','Skinny','Thick','Short','Curved','Notched',
		     'Ribbed','Hollow','Heavy','Really big','Maniacal',
		     'Cherry flavored','Caffeine free','Frenesium','Flammable',
		     'Broken','Deformed','Daisyian','Pulsating','Berzerker',
		     '''...---...''','Unguessable','Electronic','Diamond',
		     'Unsatisfactory','Massive','Wow! a','Not very good',
		     'My favorite','Reagan','RADIOACTIVE!'
		     );
        rocks   :=   (
                      'Beatles','Agate','Alexandrite','Amethyst','Antlerite',
                      'Aquamarine','Argentite','Azurite','Beryl','Bloodstone',
                      'Calcite','Carnelian','Coral','Corundum','Cryolite',
                      'Diamond','Diorite','Emerald','Flintstones','Fluorite',
                      'Gabbro','Garnet','Granite','Gypsum','Hematite','Jade',
                      'Jasper','Kryptonite','Lapus lazuli','Pink Floyd',
                      'Malachite','Manganite','Marble','Moonstone','Mudstone',
                      'Neptunite','Obsidian','Doors','Opulent','Pyrite',
                      'Quartz','Quartzite','Rhodonite','Rhyolite','Ruby',
                      'Saphire','Sphalerite','Staurolite','Tiger eye','Topaz',
                      'Turquoise','Rolling Stones','Bowie' );
        amulets :=   (
                      'Birch','Ceder','Dogwood','Driftwood','Elm wood',
                      'Hemlock','Hickory','Mahogany','Maple','Oak','Pine',
                      'Redwood','Rosewood','Walnut','Aluminium','Bone','Brass',
                      'Bronze','Copper','Iron','Lead','Nickel','Agate',
                      'Amethyst','Diamond','Emerald','Flint','Garnet','Jade',
                      'Obsidian','Onyx','Opal','Pearl','Quartz','Ruby',
                      'Saphire','Tiger eye','Topaz','Turquoise' );
        syllables := (
{tentype}	      'Methods','Methods','Methods',
		      'One way','Two ways','Ways','Many ways',
{Noun}		      'A program','Programs','Programs','Games',
		      'Steps','Two steps','Reasons','Questions',
		      'Problems','Puzzles','Truths','Lies','Sayings',
		      
		      'amuse','kill','scare','sway','buckle',
{Verb}		      'compile','destroy','maim','locate','find',
		      'negate','enhance','help','dismay','surprise',
		      'file','delete','link','separate','repair',
		      
		      'your','many','two','three','four','many',
{Adjective}	      'several','lots of','a lot of','ribbed',
		      'simple','strange','glowing','trapped',
		      'angry','lost','broken','many','your','evil',
		      
		      'friends','enemies','weapons','helms','neighbors',
{Plural Noun}	      'monsters','creatures','aliens','mutants',
		      'friends','traps','eyes','people','junk',
		      'doors','bones','disks','towels','walls','robots' );
 
{ Used in DESC.INC for replacing the & in descriptions }
        vowel_set := ['a','e','i','o','u','A','E','I','O','U'];
 


{ Monster Values }


        attack_desc := (	{max 60 chars long}
 
       'hits you.',
       'bites you.',
       'claws you.',
       'stings you.',
       'touches you.',
       'kicks you.',
       'gazes at you.',
       'breathes on you.',
       'spits on you.',
       'makes a horrible wail.',
       'embraces you.',
       'crawls on you.',
       'release a cloud of spores.',
       'begs you for money.',
       'covers you with slime.',
       'crushes you.',
       'tramples you.',
       'drools on you.',
       'random Eddie attacks.',
       'touches you with its tentacles.',
       'head-butts you.',
       'recites poetry at you.',
       'mentally stabs you.',
       'moons you.',
       'comments about your mother.',
       'makes the ground beneath your feet strike you.',
       'shoots you.',
       'chews on you.',
       'makes your brain bleed.',
       'does a ninja kick.',
       'blasts you with an alien device.',
       'surrounds you.',
       'shocks you.',
       'wrestles you to the floor.',
       'does the infamous daysian mind-trick on you.',
       'smacks you but good.',
       'stabs you.',
       'slices and dices you.',
       'sears you with a blast of heat.',
       'turns your own weapon against you.',
       'lip sync''s at you.',
       'asks you for directions.',
       'rings a bell.',
       'changes the save file.',
{45}   'is repelled.');
                    

     hit_desc  :=  (	{max 14 chars long}

   'smack around',
   'pound',
   'thwap',
   'hit',
   'wallop',
   'slice',
   'slash',
   'bash',
   'thump',
   'wail on');


{ Misc. Junk Values }

   raoul := ('S','B','O','S');
   wall_chr := '###&8#@8@&';  { (dun_level mod 10) +1  --> 1-10}
   alpha_set := 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
   open_cave_set := [0,2,3,5,7,8];



{ Store Values }

        store_choice    :=      (
{ K-Mart }
(6,7,8,9,10,11,28,29,30,31,57,57,57,59,59,59,59,60,60,60,60,64,64,70,81,86),

{ Leather & Chains }
(65,65,65,72,72,73,73,74,76,76,77,77,78,78,79,
80,81,81,82,82,83,83,84,84,85,85),

{ Weapon Shop }
(32,32,33,33,34,34,35,35,36,36,37,37,38,38,
42,42,43,43,44,44,45,45,46,46,154,154),

{ Temple }
(11,11,87,87,88,88,89,90,90,91,91,92,92,93,93,
110,110,111,111,111,112,112,112,113,113,113),

{ Bar and Grill }
(1,1,2,3,4,5,114,115,116,117,118,119,120,121,122,123,
124,135,138,139,148,149,150,151,152,153),

{ Alien Artifacts }
(125,126,127,128,129,130,131,132,133,134,140,155,
125,126,127,128,128,129,130,131,131,132,133,134,140,155),

{Black Market - non standard inventory}
(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),

{Gun Shop}
(47,47,48,48,49,49,50,50,51,51,52,52,
53,53,53,54,54,54,55,56,156,156,157,157,158,158),

{Pro Shop}
(12,13,14,15,16,17,17,19,20,21,22,23,24,25,26,27,39,40,41,
63,66,67,68,69,71,75),

{Computer Store}
(94,95,96,97,98,99,100,101,102,103,104,105,
106,107,107,108,108,109,109,136,137,137,
145,145,146,147),

{Guild}
(18,28,29,30,31,58,58,61,61,62,62,102,128,132,133,
141,141,142,142,142,143,143,143,144,144,144));
 
{ Each type of character starts out with a lots of provisions...
  NOTE: the the entries refer to array elements of INVENTORY_INIT array}
        player_init := (
{ Wrestler   }  (1,60,17,40,71,69,120,97),   
{ Scientist  }  (1,60,6,39,70,63,115,106),   
{ Guru       }  (1,60,11,39,70,63,116,110),
{ Ninja      }  (1,60,34,44,70,82,120,97),   
{ Soldier    }  (1,60,48,53,76,65,115,106),  
{ Wanderer   }  (1,60,11,39,70,63,116,110),  
{ Con-Artist }  (1,60,6,43,70,82,117,141),
{ Thief	     }  (1,60,6,43,70,82,117,141), 
{ Test	     }  (1,60,6,41,70,63,106,107));
