var     
    resist_gas : boolean;  {Soon to be a py.flag}
    max_weight : integer;
    is_boss_dead : boolean = false;
    total_winner_num : integer = 0;
    defeated_badguy : boolean = false;
    out_of_time : boolean = false; { 100-day time limit flag. -jeb}
    max_creatures : integer; {total # of monsters in mlist}
    max_objects : integer; {total # in objects.dat }
    inven_init_max : integer; { Size of store inventory }
    player_max_exp : longword;  { Max exp possible    }
    cur_height,cur_width : integer;  { Cur dungeon size}
    dun_level : integer;  { Cur dungeon level   }
    dun_power : integer;  { Cur power of dun level}
    town_num  : integer = 1;  { Cur town player is in -RLG}
    missle_ctr : integer = 0;  { Counter for missles }
    msg_line : integer;  { Contains message txt}
    msg_flag : boolean;  { Set with first msg  }
    more_flag: boolean = true;  { Question -more- on/off -jeb}
    wall_flag: boolean = true;  { Toggle Neat Wall Paterns. -RLG}
    old_msg : vtype;    { Last message    }
    generate : boolean;  { Generate next level }
    death : boolean = false;  { True if died    }
    died_from : vtype;    { What killed him }
    find_flag : boolean;  { Used in MORIA   }
    cave_flag : boolean;  { Used in GET_PANEL   }
    light_flag : boolean;  { Used in MOVE_LIGHT  }
    stat_column : integer = 1;  { Column for stats    }
    print_stat : unsigned = 0; { Flag for stats  }
    turn : integer;  { Cur turn of game    }
    day : integer = 1;  { day counter     -RLG   }
    wizard : boolean = false;  { Wizard flag }
    used_line : array [2..23] of boolean =
    	(false, false, false, false, false, false, false, false, false, false, false,
    	false, false, false, false, false, false, false, false, false, false, false);

    { Operating hours for Moria       X = Open; . = Closed  }
    days : array [1..7] of vtype = (
                'SUN:........................|',
                'MON:........................|',
                'TUE:........................|',
                'WED:........................|',
                'THU:........................|',
                'FRI:........................|',
                'SAT:........................|');


    {Bank Stuff} 
    act_bal       : integer = 0;
    principal     : integer = 0;
    payment_count : integer = 0;
    iod_val       : real = 0;
    iol_val       : real = 0;
    danny_cut     : real = 0;
    
    { Bit testing array }
    bit_array : array [1..32] of unsigned = (
{ Bit #1 }     $00000001,{ Bit #2 }     $00000002,
{ Bit #3 }     $00000004,{ Bit #4 }     $00000008,
{ Bit #5 }     $00000010,{ Bit #6 }     $00000020,
{ Bit #7 }     $00000040,{ Bit #8 }     $00000080,
{ Bit #9 }     $00000100,{ Bit #10 }    $00000200,
{ Bit #11 }    $00000400,{ Bit #12 }    $00000800,
{ Bit #13 }    $00001000,{ Bit #14 }    $00002000,
{ Bit #15 }    $00004000,{ Bit #16 }    $00008000,
{ Bit #17 }    $00010000,{ Bit #18 }    $00020000,
{ Bit #19 }    $00040000,{ Bit #20 }    $00080000,
{ Bit #21 }    $00100000,{ Bit #22 }    $00200000,
{ Bit #23 }    $00400000,{ Bit #24 }    $00800000,
{ Bit #25 }    $01000000,{ Bit #26 }    $02000000,
{ Bit #27 }    $04000000,{ Bit #28 }    $08000000,
{ Bit #29 }    $10000000,{ Bit #30 }    $20000000,
{ Bit #31 }    $40000000,{ Bit #32 }    $80000000);
 
 
    { External file names; are all located in directory with image  }
    BOSS_HOU,BOSS_MOR   : vtype;
    BOSS_MAS,BOSS_TOP   : vtype;    
             BOSS_MON   : vtype;
    BOSS_OBJ,BOSS_LOS   : vtype;
    BOSS_WIZ,BOSS_ANNOY : vtype;
    BUS_PIC ,QUOTES     : vtype;
    BOSS_INV,SKILLS     : vtype;
   
    {  Following are calculated from max dungeon sizes  }
    max_panel_rows,max_panel_cols : integer;
    quart_height,quart_width : integer;
    panel_row,panel_col : integer;
    panel_row_min,panel_row_max : integer;
    panel_col_min,panel_col_max : integer;
    panel_col_prt,panel_row_prt : integer;
 
    {  Following are all floor definitions  }
    cave : array [1..max_height] of row_floor;
    blank_floor : cave_type =
        (cptr: 0; tptr : 0; fval : 0;
        fopen : false; fm : false; pl : false; tl : false);
    dopen_floor : floor_type =
        (ftval : 1; ftopen : true); { Dark open floor }    {DO NOT CHANGE!}
    lopen_floor : floor_type =
        (ftval : 2; ftopen : true); { Light open floor }
    corr_floor1 : floor_type =
        (ftval : 4; ftopen : true); { Corridor open floor }
    corr_floor2 : floor_type =
        (ftval : 5; ftopen : true); { Room junction marker }
    corr_floor3 : floor_type =
        (ftval : 6; ftopen : true); { Door type floor }
    corr_floor4 : floor_type =
        (ftval : 7; ftopen : false); { Secret door type floor
           FLOOR VALUES 8 AND 9 ARE USED IN GENERATE }
    rock_wall1   : floor_type =
        (ftval : 10; ftopen : false); { Granite rock wall }
    rock_wall2   : floor_type =
        (ftval : 11; ftopen : false); { Magma rock wall }
    rock_wall3   : floor_type =
        (ftval : 12; ftopen : false); { Quartz rock wall }
    boundry_wall : floor_type =
        (ftval : 15; ftopen : false); { Indestructable wall }

    {  Following are set definitions    }
    floor_set : obj_set = [1,2,4,5,6,7];
    wall_set  : obj_set = [10,11,12];
    pwall_set : obj_set = [10,11,12,15];
    corr_set  : obj_set = [4,5];
    trap_set  : obj_set = [101,102,109,110,111,120];
    light_set : obj_set = [102,103,104,105,107,108,109,110,111];
    alpha_set : etype =
        'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    open_cave_set : obj_set = [0,2,3,5,7,8];
    circle    : array [1..9] of integer = {Circle Room}
        (3,3,3,2,2,2,1,1,0);
    { Following are player variables    }
    py : player_type = (
        { PLAYER RECORD IS SPECIAL }
                misc : (
                        name    : ' ';
                        race    : ' ';
                        sex     : ' ';
                        title   : ' ';
                        tclass  : ' ';
                        max_exp : 0;
                        exp     : 0;
                        au      : 0;
                        age     : 0;
                        ht      : 0;
                        wt      : 0;
                        lev     : 0;
                        max_lev : 0;
                        srh     : 0;
                        perc    : 0;
                        bth     : 0;
                        bthb    : 0;
                        mana    : 0;
                        mhp     : 0;
                        ptohit  : 0;
                        ptodam  : 0;
                        pac     : 0;
                        ptoac   : 0;
                        dis_th  : 0;
                        dis_td  : 0;
                        dis_ac  : 0;
                        dis_tac : 0;
                        disarm  : 0;
			wierd   : 0;
                        save    : 0;
                        sc      : 0;
                        pclass  : 0;
                        prace   : 0;
			pskill  : 0;
                        hitdie  : 0;
                        stl     : 0;
                        expfact : 0;
                        cmana   : 0;
                        chp     : 0;
                        history : ('', '', '', '', ''));
                stat : (
                        str     : 0;
                        cstr    : 0;
                        dex     : 0;
                        cdex    : 0;
                        con     : 0;
                        ccon    : 0;
                        int     : 0;
                        cint    : 0;
                        wis     : 0;
                        cwis    : 0;
                        chr     : 0;
                        cchr    : 0);
                flags : (
                        status          : 0;
                        rest            : 0;
                        blind           : 0;
                        paralysis       : 0;
                        confused        : 0;
                        food            : 5000;
                        food_digested   : 2;
                        protection      : 0;
                        speed           : 0;
                        fast            : 0;
                        slow            : 0;
                        afraid          : 0;
                        poisoned        : 0;
                        image           : 0;
                        protevil        : 0;
                        invuln          : 0;
                        hero            : 0;
                        shero           : 0;
                        blessed         : 0;
                        resist_heat     : 0;
                        resist_cold     : 0;
			ring_blast	: 0;
			special		: 0;
			petrified	: 0;
			diseased	: 0;
			detect_inv      : 0;
                        word_recall     : 0;
                        see_infra       : 0;
                        tim_infra       : 0;
                        see_inv         : false;
                        teleport        : false;
                        free_act        : false;
                        slow_digest     : false;
                        aggravate       : false;
                        fire_resist     : false;
                        cold_resist     : false;
                        acid_resist     : false;
                        regenerate      : false;
                        lght_resist     : false;
                        ffall           : false;
                        sustain_str     : false;
                        sustain_int     : false;
                        sustain_wis     : false;
                        sustain_con     : false;
                        sustain_dex     : false;
                        sustain_chr     : false;
                        confuse_monster : false));

{ Class titles for different levels }
    player_title :
	    array [1..max_class] of array [1..max_player_level] of btype = (

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
    player_exp : array [1..max_player_level] of longword =
        ({0,}10,20,40,80,140,220,290,370,500,
        600,800,1100,1500,2000,2600,3300,4100,5000,6000,
        7200,8800,11200,14400,19200,24400,30000,36000,42400,49000,
        64000,81000,100000,121000,144000,169000,196000,225000,256000,300000,
        400000,750000,9999999);

    acc_exp : real = 0; { Accumulator for fractional exp}
    bare_hands : dtype = '1d2';
    char_row : integer = 0;
    char_col : integer = 0;
    com_val : integer;
    //sex_type : vtype = 'FemaleMale  ';

    race : array [1..max_races] of race_type = (
{ Class choice bit string goes (from MSB to LSB): Thief, Con-Artist,
  Wanderer, Soldier, Ninja, Guru, Scientist, Wrestler }
 
    (trace : 'Elite'; 
    str_adj : 1; int_adj : 1; wis_adj : 1;
    dex_adj : 1; con_adj : 1; chr_adj : 1;
    b_age : 20; m_age : 30; 
    m_b_ht : 72; m_m_ht : 6; m_b_wt : 180; m_m_wt : 25;
    f_b_ht : 66; f_m_ht : 4; f_b_wt : 120; f_m_wt : 20; 
    b_exp : 1.40;
    b_dis : 15; bsrh : 10; bstl : 3; bperc : 10; bbth : 15; bbthb : 15;
    bwierd : 10; bsav : 5; bhitdie : 6; binfra : 0; tclass : %11111111),

    (trace : 'Hefty'; 
    str_adj : 5; int_adj : 0; wis_adj : 0;
    dex_adj : 0; con_adj : 1; chr_adj : 0;
    b_age : 19; m_age : 30; 
    m_b_ht : 72; m_m_ht : 6; m_b_wt : 210; m_m_wt : 25;
    f_b_ht : 66; f_m_ht : 4; f_b_wt : 150; f_m_wt : 20; 
    b_exp : 1.30;
    b_dis : 5; bsrh : 5; bstl : 2; bperc : 5; bbth : 20; bbthb : -5;
    bwierd : 10; bsav : 10; bhitdie : 5; binfra : 0; tclass : %11111111),

    (trace : 'Mad'; 
    str_adj : 0; int_adj : 5; wis_adj : 0;
    dex_adj : 1; con_adj : 0; chr_adj : 0;
    b_age : 25; m_age : 45; 
    m_b_ht : 60; m_m_ht : 6; m_b_wt : 150; m_m_wt : 25;
    f_b_ht : 54; f_m_ht : 4; f_b_wt : 100; f_m_wt : 20; 
    b_exp : 1.00;
    b_dis : 5; bsrh : 5; bstl : 2; bperc : 5; bbth : -5; bbthb : 5;
    bwierd : 20; bsav : 5; bhitdie : 2; binfra : 0; tclass : %11111111),

    (trace : 'Wise'; 
    str_adj : 0; int_adj : 1; wis_adj : 5;
    dex_adj : 0; con_adj : 0; chr_adj : 0;
    b_age : 35; m_age : 50; 
    m_b_ht : 70; m_m_ht : 6; m_b_wt : 150; m_m_wt : 25;
    f_b_ht : 66; f_m_ht : 4; f_b_wt : 100; f_m_wt : 20; 
    b_exp : 1.20;
    b_dis : 5; bsrh : 15; bstl : 2; bperc : 5; bbth : -5; bbthb : 5;
    bwierd : 10; bsav : 5; bhitdie : 4; binfra : 0; tclass : %11111111),

    (trace : 'Lucky'; 
    str_adj : 0; int_adj : 0; wis_adj : 1;
    dex_adj : 5; con_adj : 0; chr_adj : 0;
    b_age : 18; m_age : 30; 
    m_b_ht : 72; m_m_ht : 6; m_b_wt : 150; m_m_wt : 25;
    f_b_ht : 66; f_m_ht : 4; f_b_wt : 100; f_m_wt : 20; 
    b_exp : 1.40;
    b_dis : 20; bsrh : 10; bstl : 2; bperc : 15; bbth : 5; bbthb : 20;
    bwierd : 10; bsav : 15; bhitdie : 6; binfra : 0; tclass : %11111111),

    (trace : 'Healthy'; 
    str_adj : 1; int_adj : 0; wis_adj : 0;
    dex_adj : 0; con_adj : 5; chr_adj : 0;
    b_age : 20; m_age : 30; 
    m_b_ht : 72; m_m_ht : 6; m_b_wt : 150; m_m_wt : 25;
    f_b_ht : 66; f_m_ht : 4; f_b_wt : 100; f_m_wt : 20; 
    b_exp : 1.60;
    b_dis : 5; bsrh : 5; bstl : 2; bperc : 10; bbth : 10; bbthb : 5;
    bwierd : 10; bsav : 5; bhitdie : 8; binfra : 0; tclass : %11111111),

    (trace : 'Sexy'; 
    str_adj : 0; int_adj : 0; wis_adj : 0;
    dex_adj : 1; con_adj : 0; chr_adj : 5;
    b_age : 17; m_age : 25; 
    m_b_ht : 72; m_m_ht : 6; m_b_wt : 150; m_m_wt : 25;
    f_b_ht : 66; f_m_ht : 4; f_b_wt : 100; f_m_wt : 20; 
    b_exp : 1.40;
    b_dis : 5; bsrh : 5; bstl : 2; bperc : 5; bbth : 5; bbthb : 5;
    bwierd : 10; bsav : 20; bhitdie : 6; binfra : 0; tclass : %11111111),

    (trace : 'Alien'; 
    str_adj : 3; int_adj : 3; wis_adj : -3;
    dex_adj : 3; con_adj : 3; chr_adj : -3;
    b_age : 100; m_age : 1000; 
    m_b_ht : 72; m_m_ht : 6; m_b_wt : 150; m_m_wt : 25;
    f_b_ht : 66; f_m_ht : 4; f_b_wt : 100; f_m_wt : 20; 
    b_exp : 1.40;
    b_dis : 5; bsrh : 5; bstl : 3; bperc : 10; bbth : 15; bbthb : 10;
    bwierd : 30; bsav : 10; bhitdie : 6; binfra : 8; tclass : %10111011));
    
    { Background information }
    background : array [1..max_background] of background_type =
(

(info : 'You are the illegitimate and unacknowledged child '; roll : 10; chart : 1; next : 2; bonus : -25),
(info : 'You are the illegitimate but acknowledged child '; roll : 20; chart : 1; next : 2; bonus : -15),
(info : 'You are the only child '; roll : 50; chart : 1; next : 2; bonus : 10),
(info : 'You are the favorite child '; roll : 100; chart : 1; next : 2; bonus : 10),
(info : 'of a Drunken Gardener.  '; roll : 10; chart : 2; next : 3; bonus : -20),
(info : 'of a Social Worker.  '; roll : 30; chart : 2; next : 3; bonus : 20),
(info : 'of a Policeman.  '; roll : 50; chart : 2; next : 3; bonus : 40),
(info : 'of a Businessman.  '; roll : 70; chart : 2; next : 3; bonus : 55),
(info : 'of a Doctor.  '; roll : 80; chart : 2; next : 3; bonus : 70),
(info : 'of a Programming Analyist.  '; roll : 90; chart : 2; next : 3; bonus : 40),
(info : 'of a Senator.  '; roll : 100; chart : 2; next : 3; bonus : 90),
(info : 'You have spent years in juvanile hall.  '; roll : 10; chart : 3; next : 50; bonus : -30),
(info : 'You are a credit to the family.  '; roll : 80; chart : 3; next : 50; bonus : 5),
(info : 'You graduated first in your class.  '; roll : 100; chart : 3; next : 50; bonus : 30),

(info : 'You are the illegitimate and unacknowledged child '; roll : 10; chart : 4; next : 5; bonus : -25),
(info : 'You are the illegitimate but acknowledged child '; roll : 20; chart : 4; next : 5; bonus : -15),
(info : 'You are the strongest child '; roll : 80; chart : 4; next : 5; bonus : 10),
(info : 'You are the favorite child '; roll : 100; chart : 4; next : 5; bonus : 10),
(info : 'of a Vacuum Cleaner Salesman.  '; roll : 10; chart : 5; next : 6; bonus : -10),
(info : 'of a Pro-Wrestler.  '; roll : 60; chart : 5; next : 6; bonus : 50),
(info : 'of a Lawyer.  '; roll : 70; chart : 5; next : 6; bonus : 20),
(info : 'of a Soldier.  '; roll : 80; chart : 5; next : 6; bonus : 10),
(info : 'of a Philosopher.  '; roll : 90; chart : 5; next : 6; bonus : 15),
(info : 'of a Govenor.  '; roll : 100; chart : 5; next : 6; bonus : 70),
(info : 'You''ve owned three street gangs.  '; roll : 10; chart : 6; next : 50; bonus : -30),
(info : 'You are a credit to the family.  '; roll : 90; chart : 6; next : 50; bonus : 5),
(info : 'You''ve never lost a wrestling match!  '; roll : 100; chart : 6; next : 50; bonus : 50),

(info : 'You are the illegitimate and unacknowledged child '; roll : 10; chart : 7; next : 8; bonus : -25),
(info : 'You are the strangest child '; roll : 80; chart : 7; next : 8; bonus : 10),
(info : 'You are the favorite child '; roll : 100; chart : 7; next : 8; bonus : 20),
(info : 'of a Bartender.  '; roll : 10; chart : 8; next : 9; bonus : 0),
(info : 'of a Football Coach.  '; roll : 30; chart : 8; next : 9; bonus : 5),
(info : 'of a Unix Programmer.  '; roll : 50; chart : 8; next : 9; bonus : 10),
(info : 'of a Scientist.  '; roll : 80; chart : 8; next : 9; bonus : 30),
(info : 'of a Doctor.  '; roll : 100; chart : 8; next : 9; bonus : 40),
(info : 'A fusion experiment of your''s has destroyed a small town.  '; roll : 5; chart : 9; next : 54; bonus : -30),
(info : 'A biology experiment of your''s unleashed killer tomatoes.  '; roll : 10; chart : 9; next : 54; bonus : -30),
(info : 'You are a credit to society.  '; roll : 80; chart : 9; next : 54; bonus : 30),
(info : 'You have won a nobel prize.  '; roll : 100; chart : 9; next : 54; bonus : 50),

(info : 'You are one of several children of a '; roll : 50; chart : 10; next : 11; bonus : -5),
(info : 'You are the only child of a '; roll : 100; chart : 10; next : 11; bonus : 5),
(info : 'Gardener.  '; roll : 10; chart : 11; next : 12; bonus : 5),
(info : 'Tavern Owner.  '; roll : 20; chart : 11; next : 12; bonus : -10),
(info : 'Political Analyist.  '; roll : 30; chart : 11; next : 12; bonus : 20),
(info : 'Steel Worker.  '; roll : 40; chart : 11; next : 12; bonus : 20),
(info : 'Real Estate Agent.  '; roll : 50; chart : 11; next : 12; bonus : 20),
(info : 'Wanderer.  '; roll : 60; chart : 11; next : 12; bonus : 65),
(info : 'Theologist.  '; roll : 70; chart : 11; next : 12; bonus : 75),
(info : 'Guru.  '; roll : 100; chart : 11; next : 12; bonus : 90),
(info : 'Your wisdom has led you only to sleazy taverns.  '; roll : 10; chart : 12; next : 50; bonus : -30),
(info : 'You are a credit to your family.  '; roll : 60; chart : 12; next : 50; bonus : 20),
(info : 'You have achieved oneness with yourself.  '; roll : 100; chart : 12; next : 50; bonus : 30),

(info : 'You are one of several children of a  '; roll : 85; chart : 13; next : 14; bonus : -5),
(info : 'You are the only child of a  '; roll : 100; chart : 13; next : 14; bonus : 5),
(info : 'Thief.  '; roll : 20; chart : 14; next : 15; bonus : -30),
(info : 'Chimney Sweep. '; roll : 50; chart : 14; next : 15; bonus : 10),
(info : 'Con-Artist.  '; roll : 75; chart : 14; next : 15; bonus : 0),
(info : 'VMS Programmer.  '; roll : 95; chart : 14; next : 15; bonus : 20),
(info : 'Millionaire.  '; roll : 100; chart : 14; next : 15; bonus : 75),
(info : 'Your luck may have have run out!  '; roll : 20; chart : 15; next : 50; bonus : -10),
(info : 'You are the hero of your town.  '; roll : 80; chart : 15; next : 50; bonus : 50),
(info : 'Your luck has made you a fortune!  '; roll : 100; chart : 15; next : 50; bonus : 100),


(info : 'You are the proud child of a '; roll : 50; chart : 16; next : 17; bonus : 0),
(info : 'You are the only child of a '; roll : 100; chart : 16; next : 17; bonus : 0),
(info : 'Hypocondriac.  '; roll : 20; chart : 17; next : 18; bonus : -10),
(info : 'Prison Guard.  '; roll : 30; chart : 17; next : 18; bonus : 0),
(info : 'Politician.  '; roll : 50; chart : 17; next : 18; bonus : 40),
(info : 'Dentist.  '; roll : 70; chart : 17; next : 18; bonus : 60),
(info : 'Doctor.  '; roll : 90; chart : 17; next : 18; bonus : 80),
(info : 'Gold Medalist.  '; roll : 100; chart : 17; next : 18; bonus : 80),
(info : 'You can out drink a rhino.  '; roll : 15; chart : 18; next : 50; bonus : -20),
(info : 'You are a credit to the family.  '; roll : 85; chart : 18; next : 50; bonus : 0),
(info : 'Medical science is in awe of your health.  '; roll : 100; chart : 18; next : 50; bonus : 50),

(info : 'You are the adopted child of a '; roll : 25; chart : 19; next : 20; bonus : -10),
(info : 'You are the cutest child of a '; roll : 60; chart : 19; next : 20; bonus : 30),
(info : 'You are the only child of a '; roll : 100; chart : 19; next : 20; bonus : 40),
(info : 'Farmer.  '; roll : 10; chart : 20; next : 21; bonus : -10),
(info : 'Hockey player.  '; roll : 30; chart : 20; next : 21; bonus : 30),
(info : 'Magazine Publisher.  '; roll : 50; chart : 20; next : 21; bonus : 60),
(info : 'Author.  '; roll : 70; chart : 20; next : 21; bonus : 60),
(info : 'Used Car Salesman.  '; roll : 95; chart : 20; next : 21; bonus : -10),
(info : 'Billionaire.  '; roll : 100; chart : 20; next : 21; bonus : 100),
(info : 'Your mother sold all your nude baby pictures.  '; roll : 20; chart : 21; next : 58; bonus : -10),
(info : 'You are a credit to your family.  '; roll : 85; chart : 21; next : 58; bonus : 10),
(info : 'You are society''s sex model.  '; roll : 100; chart : 21; next : 58; bonus : 80),


(info : 'You are the rejected child of a '; roll : 10; chart : 22; next : 23; bonus : -20),
(info : 'You are the confused child of a '; roll : 80; chart : 22; next : 23; bonus : 30),
(info : 'You are the proud child of a '; roll : 100; chart : 22; next : 23; bonus : 50),
(info : 'Jarwangian Thief.  '; roll : 20; chart : 23; next : 24; bonus : -40),
(info : 'Jarwangian Soldier.  '; roll : 50; chart : 23; next : 24; bonus : 10),
(info : 'Jarwangian Scientist.  '; roll : 90; chart : 23; next : 24; bonus : 30),
(info : 'Jarwangian Diplomat.  '; roll : 100; chart : 23; next : 24; bonus : 50),
(info : 'You have made many enemies among the humans.  '; roll : 20; chart : 24; next : 62; bonus : -30),
(info : 'You are a stable member in the human community.  '; roll : 85; chart : 24; next : 62; bonus : 20),
(info : 'You are a favorite alien of many humans.  '; roll : 100; chart : 24; next : 62; bonus : 50),

(info : 'You have dark brown eyes, '; roll : 20; chart : 50; next : 51; bonus : -4),
(info : 'You have brown eyes, '; roll : 60; chart : 50; next : 51; bonus : 0),
(info : 'You have hazel eyes, '; roll : 70; chart : 50; next : 51; bonus : 1),
(info : 'You have green eyes, '; roll : 80; chart : 50; next : 51; bonus : 3),
(info : 'You have blue eyes, '; roll : 90; chart : 50; next : 51; bonus : 5),
(info : 'You have blue-gray eyes, '; roll : 100; chart : 50; next : 51; bonus : -6),
(info : 'straight '; roll : 70; chart : 51; next : 52; bonus : 0),
(info : 'wavey '; roll : 90; chart : 51; next : 52; bonus : 1),
(info : 'curly '; roll : 100; chart : 51; next : 52; bonus : 3),
(info : 'black hair, '; roll : 30; chart : 52; next : 53; bonus : 1),
(info : 'brown hair, '; roll : 70; chart : 52; next : 53; bonus : 3),
(info : 'auburn hair, '; roll : 80; chart : 52; next : 53; bonus : -3),
(info : 'red hair, '; roll : 90; chart : 52; next : 53; bonus : 4),
(info : 'blonde hair, '; roll : 100; chart : 52; next : 53; bonus : -1),
(info : 'and a very dark complexion.'; roll : 10; chart : 53; next : -1; bonus : 1),
(info : 'and a dark complexion.'; roll : 30; chart : 53; next : -1; bonus : 3),
(info : 'and an average complexion.'; roll : 80; chart : 53; next : -1; bonus : -3),
(info : 'and a fair complexion.'; roll : 90; chart : 53; next : -1; bonus : 4),
(info : 'and a very fair complexion.'; roll : 100; chart : 53; next : -1; bonus : 5),

(info : 'You have small beady eyes, '; roll : 30; chart : 54; next : 55; bonus : 0),
(info : 'You have brown eyes, '; roll : 60; chart : 54; next : 55; bonus : 0),
(info : 'You have blue eyes, '; roll : 90; chart : 54; next : 55; bonus : 0),
(info : 'You have glowing red eyes, '; roll : 100; chart : 54; next : 55; bonus : 0),
(info : 'tossled '; roll : 50; chart : 55; next : 56; bonus : 0),
(info : 'straight '; roll : 75; chart : 55; next : 56; bonus : 0),
(info : 'wavey '; roll : 100; chart : 55; next : 56; bonus : 0),
(info : 'grey hair, and a fair complexion.'; roll : 10; chart : 56; next : -1; bonus : 0),
(info : 'brown hair, and a fair complexion.'; roll : 30; chart : 56; next : -1; bonus : 0),
(info : 'white hair, and a fair complexion.'; roll : 80; chart : 56; next : -1; bonus : 0),
(info : 'silver hair, and a fair complexion.'; roll : 100; chart : 56; next : -1; bonus : 0),

(info : 'You have alluring eyes, '; roll : 20; chart : 58; next : 59; bonus : 0),
(info : 'You have mesmerising eyes, '; roll : 40; chart : 58; next : 59; bonus : 0),
(info : 'You have blue eyes, '; roll : 60; chart : 58; next : 59; bonus : 0),
(info : 'You have dark brown eyes, '; roll : 80; chart : 58; next : 59; bonus : 0),
(info : 'You have nice eyes, '; roll : 100; chart : 58; next : 59; bonus : 0),
(info : 'flowing '; roll : 30; chart : 59; next : 60; bonus : 0),
(info : 'wavey '; roll : 60; chart : 59; next : 60; bonus : 0),
(info : 'curly '; roll : 100; chart : 59; next : 60; bonus : 0),
(info : 'black hair, '; roll : 30; chart : 60; next : 61; bonus : 0),
(info : 'brown hair, '; roll : 70; chart : 60; next : 61; bonus : 0),
(info : 'auburn hair, '; roll : 80; chart : 60; next : 61; bonus : 0),
(info : 'red hair, '; roll : 90; chart : 60; next : 61; bonus : 0),
(info : 'blonde hair, '; roll : 100; chart : 60; next : 61; bonus : 0),
(info : 'and a dark complexion.'; roll : 33; chart : 61; next : -1; bonus : 0),
(info : 'and a smooth complexion.'; roll : 66; chart : 61; next : -1; bonus : 0),
(info : 'and a tanned complexion.'; roll : 100; chart : 61; next : -1; bonus : 0),

(info : 'You have orange eyes, '; roll : 33; chart : 62; next : 63; bonus : 0),
(info : 'You have glowing eyes, '; roll : 66; chart : 62; next : 63; bonus : 0),
(info : 'You have very large eyes, '; roll : 100; chart : 62; next : 63; bonus : 0),
(info : 'straight '; roll : 33; chart : 63; next : 64; bonus : 0),
(info : 'wavey '; roll : 66; chart : 63; next : 64; bonus : 0),
(info : 'mottled '; roll : 100; chart : 63; next : 64; bonus : 0),
(info : 'purple hair, '; roll : 33; chart : 64; next : 65; bonus : 0),
(info : 'orange hair, '; roll : 66; chart : 64; next : 65; bonus : 0),
(info : 'blue hair, '; roll : 100; chart : 64; next : 65; bonus : 0),
(info : 'and a dark complexion.'; roll : 33; chart : 65; next : -1; bonus : 0),
(info : 'and a smooth complexion.'; roll : 66; chart : 65; next : -1; bonus : 0),
(info : 'and a plastic complexion.'; roll : 100; chart : 65; next : -1; bonus : 0)
);
    
    { Buying and selling adjustments for character race VS store owner race }
    rgold_adj : array [1..max_class] of array [1..max_class] of real =
 
                              {Character Class:}

{StoreOwner:}  {  Wrs,  Sci,  Gru,  Nnj,  Sol,  Wnd,  Con,  Thf  Test}

{Wrestler   } ((-0.08, 0.06, 0.02,-0.04,-0.02, 0.02,-0.10, 0.00, 0.00),
{Scientist  }  ( 0.04,-0.08, 0.06, 0.02, 0.00, 0.04,-0.04,-0.04, 0.00),
{Guru       }  ( 0.00, 0.02,-0.04, 0.02, 0.04,-0.06, 0.00,-0.04, 0.00),
{Ninja      }  (-0.02, 0.00,-0.02,-0.06,-0.04, 0.00,-0.02, 0.00, 0.00),
{Soldier    }  (-0.02, 0.00, 0.04,-0.02,-0.08, 0.02,-0.04, 0.00, 0.00),
{Wanderer   }  ( 0.02, 0.04,-0.08,-0.04, 0.00,-0.08,-0.04,-0.04, 0.00),
{Con-Artist }  ( 0.10, 0.08, 0.00, 0.08, 0.08, 0.08, 0.00, 0.06, 0.08),
{Thief      }  ( 0.06, 0.06, 0.00, 0.06, 0.06, 0.06,-0.02, 0.00, 0.06),
{Test       }  ( 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00));
 
    class : array [1..max_class] of class_type = (

(title : 'Wrestler'    ;   m_exp : 0.10;   adj_hd : 15;
          mdis : 10; msrh  : 10; mstl   :  0; mperc :  5;
          mbth : 45; mbthb : 10; mwierd : -5; msav  : 20;
          madj_str :  4; madj_int : -1; madj_wis : -1;
          madj_dex :  1; madj_con :  3; madj_chr : -1;   skill_no : 0),

(title : 'Scientist'   ;   m_exp : 0.50;   adj_hd :  2;
          mdis : 10; msrh  : 10; mstl   :  2; mperc : 10;
          mbth : 15; mbthb : 20; mwierd : 10; msav  : 20;
          madj_str : -3; madj_int :  3; madj_wis :  1;
          madj_dex :  1; madj_con :  0; madj_chr :  0;   skill_no : 1),

(title : 'Guru'        ;   m_exp : 0.30;   adj_hd :  4;
          mdis :  8; msrh  : 10; mstl   :  2; mperc : 30;
          mbth : 30; mbthb : 10; mwierd :  5; msav  : 25;
          madj_str : -1; madj_int : -1; madj_wis :  3; 
          madj_dex : -1; madj_con :  2; madj_chr :  2;   skill_no : 3),

(title : 'Ninja'       ;   m_exp : 0.10;   adj_hd : 10;
          mdis : 15; msrh  : 22; mstl   :  4; mperc : 25;
          mbth : 40; mbthb : 45; mwierd :  0; msav  : 40;
          madj_str :  2; madj_int :  1; madj_wis :  2;
          madj_dex :  3; madj_con :  0; madj_chr : -1;   skill_no : 0),

(title : 'Soldier'     ;   m_exp : 0.30;   adj_hd :  8;
          mdis : 10; msrh  : 15; mstl   :  3; mperc : 20;
          mbth : 35; mbthb : 50; mwierd :  5; msav  : 25;
          madj_str :  2; madj_int :  2; madj_wis :  1;
          madj_dex :  2; madj_con :  1; madj_chr :  1;   skill_no : 2),

(title : 'Wanderer'    ;   m_exp : 0.10;   adj_hd :  8;
          mdis :  8; msrh  : 10; mstl   :  2; mperc : 15;
          mbth : 45; mbthb : 35; mwierd :  0; msav  : 20;
          madj_str :  1; madj_int : -1; madj_wis :  2;
          madj_dex :  1; madj_con :  1; madj_chr :  1;   skill_no : 4),

(title : 'Con-Artist'  ;   m_exp : 0.50;   adj_hd :  6;
          mdis : 13; msrh  : 10; mstl   :  2; mperc : 15;
          mbth : 30; mbthb : 30; mwierd :  0; msav  : 25;
          madj_str :  2; madj_int :  0; madj_wis :  0;
          madj_dex :  2; madj_con :  1; madj_chr :  3;   skill_no : 5),

(title : 'Thief'       ;   m_exp : 0.30;   adj_hd :  8;
          mdis : 15; msrh  : 15; mstl   :  3; mperc : 15;
          mbth : 30; mbthb : 30; mwierd :  0; msav  : 25;
          madj_str :  3; madj_int :  0; madj_wis :  0;
          madj_dex :  2; madj_con :  2; madj_chr :  1;   skill_no : 6),

(title : 'Journalist'  ;   m_exp : 0.20;   adj_hd :  6;
          mdis : 10; msrh  : 10; mstl   :  3; mperc : 20;
          mbth : 20; mbthb : 30; mwierd :  0; msav  : 25;
          madj_str :  0; madj_int :  3; madj_wis :  0;
          madj_dex :  2; madj_con :  0; madj_chr :  0;   skill_no : 7));

    skill_list : array [1..max_skills] of skill_type;

    { For Create Food spell use... }
    mush : treasure_type =
        (name : '& Can~ of Beans and Weenies';
        tval : 80; tchar : ','; flags : $00000000; p1 : 2400; cost : 0;
        subval : 308; weight : 1; number : 1; tohit : 0; todam : 0; ac : 0;
        toac : 0; damage : '0d0'; level : 1);
 
    player_init : 
                     array [1..max_class] of array [1..8] of byteint = (
{ Each type of character starts out with a lots of provisions...
  NOTE: the the entries refer to array elements of INVENTORY_INIT array}
{ Wrestler   }  (1,60,17,40,71,69,120,97),   
{ Scientist  }  (1,60,6,39,70,63,115,106),   
{ Guru       }  (1,60,11,39,70,63,116,110),
{ Ninja      }  (1,60,34,44,70,82,120,97),   
{ Soldier    }  (1,60,48,53,76,65,115,106),  
{ Wanderer   }  (1,60,11,39,70,63,116,110),  
{ Con-Artist }  (1,60,6,43,70,82,117,141),
{ Thief	     }  (1,60,6,43,70,82,117,141), 
{ Test	     }  (1,60,6,41,70,63,106,107));
    berz_y  : array [1..8] of integer = (0,-1,-1,-1,0,1,1,1);
    berz_x  : array [1..8] of integer = (-1,-1,0,1,1,1,0,-1);

    { Following are store definitions   }
    office_char_row : integer;
    office_char_col : integer;
    exited_office : boolean;

{ Store owners have different characteristics for pricing }
    owners : array [1..max_stores] of
				 array [1..max_towns] of owner_type = (
((owner_name : 'Penny Preatty       (Wanderer)      K-Mart'; beg_effect : 1; buy_inflate : 0.04; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Leisure Suit Larry  (Scientist)     K-Mart'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Charlie Yatchet     (Con-Artist)    K-Mart'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'W.C. Meadows        (Guru)          K-Mart'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'John                (Guitar)        K-Mart'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Ringo               (Drummer)       K-Mart'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Paul                (Bass)          K-Mart'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'George              (Sitar)         K-Mart'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'The Hulkster        (Wrestler)      Leather & Chains'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Crazy Eddie         (Soldier)       Leather & Chains'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Steel-Fist Thelma   (Wrestler)      Leather & Chains'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'James Bondage       (Wrestler)      Leather & Chains'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Major Disaster      (Soldier)       Leather & Chains'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Corporal Punishment (Soldier)       Leather & Chains'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Olga                (Wrestler)      Leather & Chains'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Cool Hand Fluke     (Soldier)       Leather & Chains'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'Chainsaw Mike       (Soldier)       Weapon Shop'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Samuri Sven         (Ninja)         Weapon Shop'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Jon Wane            (Soldier)       Weapon Shop'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'The Man From Aunt   (Soldier)       Weapon Shop'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Killroy             (Ninja)         Weapon Shop'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Sargent Granite     (Soldier)       Weapon Shop'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Col. Mustardseed    (Soldier)       Weapon Shop'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Fluffy              (Teddy Bear)    Weapon Shop'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'Shari the Humble    (Guru)          Temple'; beg_effect : 1; buy_inflate : 0.04; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Graceful Gretchen   (Wanderer)      Temple'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Cosmo the Enlightened  (Guru)       Temple'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Elvis               (The King)      Temple'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Elron Cupboard      (Guru)          Temple'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Tammy Fay Bacon     (Guru)          Temple'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Hal 9000-B          (Computer)      Temple'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Okra Winfreid       (Guru)          Temple'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'Pop Poolstone       (Scientist)     Bar and Grill'; beg_effect : 1; buy_inflate : 0.04; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Benny the Wino      (Ninja)         Bar and Grill'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Nuke-Brained Nick   (Con-Artist)    Bar and Grill'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Yo-Gurt             (Guru)          Bar and Grill'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Hash-Berry          (Ninja)         Bar and Grill'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Mike, Ike, and Jim  (Con-Artists)   Bar and Grill'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Jabba the Pizza Hut (Scientist)     Bar and Grill'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Donald McRonald     (Ninja)         Bar and Grill'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'Dr. StrangeLoaf     (Scientist)     Alien Artifacts'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Missing Linke       (Guru)          Alien Artifacts'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Professor X         (Scientist)     Alien Artifacts'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Chewy Bacon         (Scientist)     Alien Artifacts'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Mr. Spot            (Guru)          Alien Artifacts'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'E.Z.                (Scientist)     Alien Artifacts'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Ham Salad           (Scientist)     Alien Artifacts'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Mr. Dataset         (Con-Artist)    Alien Artifacts'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'Lt. Wharf           (Soldier)       Black Market'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'The Butcher         (Thief)         Black Market'; beg_effect : 1; buy_inflate : 0.35; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Smith               (Con-Artist)    Black Market'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Captain Quirk       (Soldier)       Black Market'; beg_effect : 1; buy_inflate : 0.50; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Guido               (Thief)         Black Market'; beg_effect : 1; buy_inflate : 0.60; sell_deflate : 0.60; owner_class : 1),
 (owner_name : 'Grandma Wormtongue  (Con-Artist)    Black Market'; beg_effect : 1; buy_inflate : 0.70; sell_deflate : 0.60; owner_class : 1),
 (owner_name : 'Sun-Tsu             (Soldier)       Black Market'; beg_effect : 1; buy_inflate : 0.80; sell_deflate : 0.70; owner_class : 1),
 (owner_name : 'The Butcher         (Thief)         Black Market'; beg_effect : 1; buy_inflate : 0.90; sell_deflate : 0.70; owner_class : 1)),

((owner_name : 'Ginsing Gary        (Ninja)         Gun Shop'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Wilber Samaroo      (Soldier)       Gun Shop'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Queue               (Scientist)     Gun Shop'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Mr. Krueger         (Ninja)         Gun Shop'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Wargha              (Scientist)     Gun Shop'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Gramdpa Wormtongue  (Con-Artist)    Gun Shop'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Helga               (Wanderer)      Gun Shop'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Norma Bates         (Psycho)        Gun Shop'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'Big Jake            (Wrestler)      Pro Shop'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Coach Crawldirt     (Soldier)       Pro Shop'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Ski-Bunny Betty     (Wanderer)      Pro Shop'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Mr. Buck Rogers     (Wrestler)      Pro Shop'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Admiral Skeet       (Soldier)       Pro Shop'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Bjorn Boring        (Wanderer)      Pro Shop'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Nadia Comb-an-inch  (Wrestler)      Pro Shop'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Err Jordan          (Soldier)       Pro Shop'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'Mr. Solar           (Scientist)     Computer Store'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Dr. John Smith      (Scientist)     Computer Store'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Zapper              (Mad Scientist) Computer Store'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Bill Grates         (Scientist)     Computer Store'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Steve Batch Jobs    (Scientist)     Computer Store'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Donald Canoeth      (Scientist)     Computer Store'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Jimmy Wayne Todd    (Programmer)    Computer Store'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Robert Alan Koeneke (Programmer)    Computer Store'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1)),

((owner_name : 'cloister            (Soldier)       Guild'; beg_effect : 1; buy_inflate : 0.06; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Cutpurse Mallone    (Thief)         Guild'; beg_effect : 1; buy_inflate : 0.08; sell_deflate : 0.40; owner_class : 1),
 (owner_name : 'Lightfingered Lisa  (Con-Artist)    Guild'; beg_effect : 1; buy_inflate : 0.10; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Peg Leg Pete        (Soldier)       Guild'; beg_effect : 1; buy_inflate : 0.12; sell_deflate : 0.45; owner_class : 1),
 (owner_name : 'Captain Blight      (Soldier)       Guild'; beg_effect : 1; buy_inflate : 0.15; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Russell Fingers     (Con-Artist)    Guild'; beg_effect : 1; buy_inflate : 0.20; sell_deflate : 0.50; owner_class : 1),
 (owner_name : 'Milo Minderbender   (Soldier)       Guild'; beg_effect : 1; buy_inflate : 0.30; sell_deflate : 0.55; owner_class : 1),
 (owner_name : 'Six Fingered Louie  (Thief)         Guild'; beg_effect : 1; buy_inflate : 0.40; sell_deflate : 0.55; owner_class : 1)));

    store : array [1..max_stores] of store_type;
    store_door : array [1..max_doors] of treasure_type = (
(name : 'The entrance to a K-Mart'; tval : 110; tchar : 'K'; flags : $00000000; p1 : 0; cost : 0; subval : 101; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to Leather and Chains'; tval : 110; tchar : 'L'; flags : $00000000; p1 : 0; cost : 0; subval : 102; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to a Weapon Shop'; tval : 110; tchar : 'W'; flags : $00000000; p1 : 0; cost : 0; subval : 103; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to a Temple'; tval : 110; tchar : 'T'; flags : $00000000; p1 : 0; cost : 0; subval : 104; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to a Bar and Grill'; tval : 110; tchar : '+'; flags : $00000000; p1 : 0; cost : 0; subval : 105; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to an Alien Artifact Shop'; tval : 110; tchar : 'A'; flags : $00000000; p1 : 0; cost : 0; subval : 106; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to a Black Market'; tval : 110; tchar : 'B'; flags : $00000000; p1 : 0; cost : 0; subval : 107; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 1),
(name : 'The entrance to a Gun Shop'; tval : 110; tchar : 'G'; flags : $00000000; p1 : 0; cost : 0; subval : 108; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to a Pro Shop'; tval : 110; tchar : 'P'; flags : $00000000; p1 : 0; cost : 0; subval : 109; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to a Computer Store'; tval : 110; tchar : 'C'; flags : $00000000; p1 : 0; cost : 0; subval : 110; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'a granite wall with the vague outline of a door.'; tval : 110; tchar : '#'; flags : $00000000; p1 : 0; cost : 0; subval : 111; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'The entrance to a tall building'; tval : 110; tchar : '<'; flags : $00000000; p1 : 0; cost : 0; subval : 112; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0));

    building_door : array 
					   [1..max_bdoors] of treasure_type = (
(name : 'the hotel front desk'; tval : 120; tchar : ']'; flags : $00000000; p1 : 0; cost : 0; subval : 201; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'the bank teller window'; tval : 120; tchar : '['; flags : $00000000; p1 : 0; cost : 0; subval : 202; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'the medical clinic entrance'; tval : 120; tchar : ']'; flags : $00000000; p1 : 0; cost : 0; subval : 203; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'NewsInc, Ltd.'; tval : 120; tchar : '['; flags : $00000000; p1 : 0; cost : 0; subval : 204; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0));

{ Store Values }
    store_choice : array [1..max_stores] of array 
	[1..storeXchoices] of integer = (

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

{ Each store will buy only certain items,based on TVAL }
    store_buy : array [1..max_stores] of obj_set = (
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
 
    joblist   : array [1..9] of ctype =
              ('Dish Washer','Boiler Room Worker','Bellhop',
	       'Doorman','Security Guard','Gourmet Chef',
	       'Body Guard','Accountant','Hotel Manager');


    jobwage   : array [1..9] of byteint =
              (3,5,7,9,11,15,25,40,90);
    townlist  : array [1..max_towns] of town_type = (
(town_name : 'Seattle'; max_depth : 10; lev_plus : 0),
(town_name : 'Boise'; max_depth : 10; lev_plus : 6),
(town_name : 'Denver'; max_depth : 10; lev_plus : 12),
(town_name : 'Kansas City'; max_depth : 10; lev_plus : 18),
(town_name : 'Chicago'; max_depth : 10; lev_plus : 24),
(town_name : 'Detroit'; max_depth : 10; lev_plus : 30),
(town_name : 'Pittsburgh'; max_depth : 10; lev_plus : 36),
(town_name : 'New York City'; max_depth : 10; lev_plus : 43));

    { Following are treasure arrays and variables   }
    object_list : array [1..450] of treasure_type;
    object_ident : array [1..450] of boolean;
    t_level : array [0..max_obj_level] of integer;
{ Gold list (All types of gold and gems are defined here) }
    gold_list : array [1..max_gold] of treasure_type = (
(name : 'nickels'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 1; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'arcade tokens'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 1; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'dimes'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 2; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'trinkets'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 2; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'blank disks'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 3; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'postage stamps'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 3; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'loose change'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 5; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'pez dispensers'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 5; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'bubble gum'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 8; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'gold fillings'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 10; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'foreign bills'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 10; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'old comic books'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 15; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : '$1 bills'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 15; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'food stamps'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 15; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'small bills'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 20; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'silicon chips'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 20; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'chic jewelry'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 25; subval : 4; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'digital watches'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 30; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'car stereos'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 50; subval : 4; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'large bills'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 60; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'illegal drugs'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 70; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'uncut diamonds'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 75; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'silver coins'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 80; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'quarters'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 80; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'gold coins'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 100; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'gold dust'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 100; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'stock certificates'; tval : 100; tchar : '$'; flags : $00000000; p1 : 0; cost : 120; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'alien crystals'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 150; subval : 5; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'diamonds'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 200; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1),
(name : 'alien crystals'; tval : 100; tchar : '*'; flags : $00000000; p1 : 0; cost : 300; subval : 5; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : ' '; level : 1));

    t_list : array [1..max_talloc] of treasure_type;
    inventory : array [1..inven_max] of treasure_type;
    equipment : array [23..equip_max] of treasure_type;
    temporary_slot : treasure_type;
    inventory_init : array [1..300] of treasure_type;
    { Treasure related values }
    blank_treasure : treasure_type =
      (name : ' '; tval : 0; tchar : ' '; flags : 0; p1 : 0; cost : 0;
      subval : 0; weight : 0; number : 0; tohit : 0; todam : 0; ac : 0;
      toac : 0; damage : ' '; level : 0);
    inven_ctr : integer = 0;  { Total different obj's }
    inven_weight : integer = 0; { Cur carried weight }
    equip_ctr : integer = 0;  { Cur equipment ctr }
    tcptr : integer;  { Cur treasure heap ptr }
 
    { Following are feature objects defined for dungeon }
    trap_lista : array [1..max_trapa] of treasure_type = (
(name : 'an open pit'; tval : 101; tchar : ' '; flags : $00000000; p1 : 0; cost : 0; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : -50),
(name : 'an arrow trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d8'; level : 0),
(name : 'a covered pit'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : 0),
(name : 'a trap door'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 4; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d8'; level : 0),
(name : 'a gas trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 5; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 0),
(name : 'a loose rock'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 6; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'a dart trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 7; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 0),
(name : 'a strange rune'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 8; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'some loose rock'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 9; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : 0),
(name : 'a gas trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 10; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 0),
(name : 'a strange rune'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 11; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0),
(name : 'a blackened spot'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 12; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '4d6'; level : 0),
(name : 'some corroded rock'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 13; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '4d6'; level : 0),
(name : 'a gas trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 0; cost : 0; subval : 14; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : 0),
(name : 'a gas trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 5; cost : 0; subval : 15; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 10),
(name : 'a bomb'; tval : 101; tchar : '.'; flags : $00000000; p1 : 5; cost : 0; subval : 16; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '5d6'; level : 5),
(name : 'a dart trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 5; cost : 0; subval : 17; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d8'; level : 10),
(name : 'a dart trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 5; cost : 0; subval : 18; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d8'; level : 10),
(name : 'a teleport trap'; tval : 101; tchar : '.'; flags : $00000000; p1 : 5; cost : 0; subval : 20; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 10));

    trap_listb : array [1..max_trapb] of treasure_type = (
(name : 'an open pit'; tval : 102; tchar : ' '; flags : $00000000; p1 : 1; cost : 0; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : -50),
(name : 'an arrow trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 3; cost : 0; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d8'; level : -10),
(name : 'a covered pit'; tval : 102; tchar : '^'; flags : $00000000; p1 : 2; cost : 0; subval : 3; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : -40),
(name : 'a trap door'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 4; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d8'; level : -25),
(name : 'a gas trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 3; cost : 0; subval : 5; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 5),
(name : 'a loose rock'; tval : 102; tchar : ';'; flags : $00000000; p1 : 0; cost : 0; subval : 6; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : -90),
(name : 'a dart trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 7; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 10),
(name : 'a strange rune'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 8; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : -10),
(name : 'some loose rock'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 9; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : -10),
(name : 'a gas trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 10; cost : 0; subval : 10; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 5),
(name : 'a strange rune'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 11; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : -10),
(name : 'a blackened spot'; tval : 102; tchar : '^'; flags : $00000000; p1 : 10; cost : 0; subval : 12; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '4d6'; level : 10),
(name : 'some shattered rock'; tval : 102; tchar : '^'; flags : $00000000; p1 : 10; cost : 0; subval : 13; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '4d6'; level : 10),
(name : 'a gas trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 14; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '2d6'; level : 5),
(name : 'a gas trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 15; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d4'; level : 10),
(name : 'a bomb'; tval : 102; tchar : '^'; flags : $00000000; p1 : 25; cost : 0; subval : 16; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '5d6'; level : 25),
(name : 'a dart trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 17; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d8'; level : 10),
(name : 'a dart trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 5; cost : 0; subval : 18; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d8'; level : 10),

{ SPECIAL CASE,see DOOR_LIST below (SUBVALS MUST AGREE) }
(name : 'a closed door'; tval : 105; tchar : '+'; flags : $00000000; p1 : 0; cost : 0; subval : 19; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0),
(name : 'a teleport trap'; tval : 102; tchar : '^'; flags : $00000000; p1 : 0; cost : 0; subval : 20; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 50));

    blk_mkt_trap : treasure_type = {Black Market trap}
(name : 'a Black Market Store'; tval : 111; tchar : 'B'; flags : $00000000; p1 : 5; cost : 0; subval : 107; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 5);
    scare_monster : treasure_type = {Special trap}
(name : 'a strange rune'; tval : 102; tchar : '^'; flags : $00000000; p1 : 0; cost : 0; subval : 99; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : -90);
    rubble : treasure_type =
(name : 'some rubble'; tval : 103; tchar : ':'; flags : $00000000; p1 : 0; cost : 0; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '0d0'; level : 0);
    door_list : array [1..3] of treasure_type = (
(name : 'an open door'; tval : 104; tchar : ''''; flags : $00000000; p1 : 0; cost : 0; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0),
(name : 'a closed door'; tval : 105; tchar : '+'; flags : $00000000; p1 : 0; cost : 0; subval : 19; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0),
(name : 'a secret door'; tval : 109; tchar : '#'; flags : $00000000; p1 : 0; cost : 0; subval : 19; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0));
    up_stair : array [1..max_stair] of treasure_type = (
(name : 'an staircase going up'; tval : 107; tchar : '<'; flags : $00000000; p1 : 0; cost : 0; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0),
(name : 'a steep staircase leading upwards'; tval : 107; tchar : '<'; flags : $00000000; p1 : 0; cost : 0; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0),
(name : 'a *very* steep staircase leading way, way up'; tval : 107; tchar : '<'; flags : $00000000; p1 : 0; cost : 0; subval : 4; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0));

    down_stair : array [1..max_stair] of treasure_type = (
(name : 'a staircase going downwards'; tval : 108; tchar : '>'; flags : $00000000; p1 : 0; cost : 0; subval : 1; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0),
(name : 'a steep staircase leading downwards'; tval : 108; tchar : '>'; flags : $00000000; p1 : 0; cost : 0; subval : 2; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0),
(name : 'a *very* steep staircase leading way, way down'; tval : 108; tchar : '>'; flags : $00000000; p1 : 0; cost : 0; subval : 4; weight : 0; number : 0; tohit : 0; todam : 0; ac: 0; toac : 0; damage : '1d1'; level : 0));
 
    { Following are creature arrays and variables   }
    c_list : array [1..400] of creature_type;
    m_list : array [1..max_malloc] of monster_type;
    m_level : array [0..max_mons_level] of integer;
    {Blank monster values}
    blank_monster : monster_type = 
    (hp : 0; csleep : 0; cdis : 0; mptr : 0; nptr : 0; cspeed : 0;
     fy : 0; fx : 0; stuned : 0; ml : false; confused : false; cur_reprod : 0
    );
    muptr : integer;  { Cur used monster ptr  }
    mfptr : integer;  { Cur free monster ptr  }
    mon_tot_mult : integer;  { # of repro's of creature  }
    attack_desc : array [1..max_attack_desc] of etype =
      ('hits you.',
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
    hit_desc : array [1..max_hit_desc] of btype =
  ('smack around',
   'pound',
   'thwap',
   'hit',
   'wallop',
   'slice',
   'slash',
   'bash',
   'thump',
   'wail on');
    kill_list: array [1..400] of integer;

    { Following are arrays for descriptive pieces   }
    colors : array [1..max_colors] of atype =
                     ('Amber','Azure','Blue','Blue Speckled','Blue Spotted',
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
    mushrooms : array [1..max_mush] of atype =
                     ('Blue','Black','Brown','Copper','Crimson','Dark blue',
                      'Dark green','Dark red','Gold','Green','Grey',
                      'Light Blue','Light Green','Orange','Pink','Plaid',
                      'Purple','Red','Tan','Turquoise','Violet','White',
                      'Yellow','Wrinkled','Wooden','Slimey','Speckled',
                      'Spotted','Furry' );
    woods : array [1..max_woods] of atype =
		     ('Terra-cotta','Frenisium','Rectangular','Not there',
		      'Flammable','Icy','Gurgling','Broken',
		      'Bubbly','Crispy-crunchy','Deformed','Sluggish',
		      'Massy','Fossilized','Plaid flannel','Tacky',
		      'Objet-d''art','Very very tiny','Vibrating','Talkative',
		      'Half baked','Sauteed','Over easy','Bar-b-queued',
		      'MKS unit','Barrel-like','Polymerized','Phenelalenuric',
		      'Rabbit-fur','Teak','Edible','Bitchin''',
		      'Growing','Lethal','Kinky','Demon-spawn',
		      'Daisyian','Scaly','Warm','Soft');
    metals : array [1..max_metals] of atype =
		    ('Long','Skinny','Thick','Short','Curved','Notched',
		     'Ribbed','Hollow','Heavy','Really big','Maniacal',
		     'Cherry flavored','Caffeine free','Frenesium','Flammable',
		     'Broken','Deformed','Daisyian','Pulsating','Berzerker',
		     '''...---...''','Unguessable','Electronic','Diamond',
		     'Unsatisfactory','Massive','Wow! a','Not very good',
		     'My favorite','Reagan','RADIOACTIVE!'
		     );
    rocks : array [1..max_rocks] of atype =
                     ('Beatles','Agate','Alexandrite','Amethyst','Antlerite',
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
    amulets : array [1..max_amulets] of atype =
                     ('Birch','Ceder','Dogwood','Driftwood','Elm wood',
                      'Hemlock','Hickory','Mahogany','Maple','Oak','Pine',
                      'Redwood','Rosewood','Walnut','Aluminium','Bone','Brass',
                      'Bronze','Copper','Iron','Lead','Nickel','Agate',
                      'Amethyst','Diamond','Emerald','Flint','Garnet','Jade',
                      'Obsidian','Onyx','Opal','Pearl','Quartz','Ruby',
                      'Saphire','Tiger eye','Topaz','Turquoise' );
    syllables : array [1..max_syllables] of tentype =
		     ('Methods','Methods','Methods',
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
 
    vowel_set : char_set =
        ['a','e','i','o','u','A','E','I','O','U'];
 
    { Following are variables for the Save Character Routines   }
    finam : vtype;
 
    wall_chr : tentype = '###&8#@8@&';  { (abs(dun_level) mod 10) +1  --> 1-10}
    wierd_chance : integer;
