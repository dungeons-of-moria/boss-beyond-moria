   const
{Note to the Wizard:
   Tweaking these constants can *GREATLY* change the game.
   Two years+ of constant tuning have generated these values.
   Minor adjustments are encouraged, but you must be very
   careful not to unbalance the game.  Moria/Boss was meant to be
   challenging, not a give away.  Many adjustments can cause
   cause the game to act strangely, or even cause errors. }
 
{ Current version number of Boss  }
        cur_version   = 2.40;

{ QIOW constants, see $IODEF in STARLET.MLB }

        IO$_WRITEVBLK = %B'0000000000110000';  {   48D, 0030H   }
        IO$_TTYREADALL= %B'0000000000111010';  {   58D, 003AH   }
        IO$M_NOECHO   = %B'0000000001000000';  {   64D, 0040H   }
        IO$M_NOWAIT   = %B'0000000010000000';  {  128D, 0080H   }
        IO$M_PURGE    = %B'0000100000000000';  { 2048D, 0800H	}

        IO$MOR_OUTPUT = IO$_WRITEVBLK;
        IO$MOR_INPUT  = IO$_TTYREADALL + IO$M_NOECHO;
        IO$MOR_DELAY  = IO$MOR_INPUT   + IO$M_NOWAIT;
        IO$MOR_IPURGE = IO$MOR_DELAY   + IO$M_PURGE;
 
{ IO bug exists at high baud rates (baud >= 9600).  This bug
  is believed to be in the device driver.  It can be avoided by
  hibernating the process a small amount of time before perform-
  ing a QIOW read operation. }
        IO$MOR_IOPAUSE= 5;  { x/100 secs of sleep before inkey
 
  ENCRYPTION CONSTANTS                                          
  NOTE: These numbers were pulled from the air, and can be      
        changed.  If changed, characters produced from other    
        versions will fail to restore}

        encrypt_seed1 = 342342;
        encrypt_seed2 = 342944081;
 
{ DUNGEON SIZE PARAMETERS }
        max_height    = 66;     { Multiple of 11; >= 22 }
        max_width     = 198;    { Multiple of 33; >= 66 }
        screen_height = 22;	{ screen is intended to }
        screen_width  = 66;	{ show four panels.     }
 
{ OUTPUT DUNGEON SECTION SIZES }
        outpage_height= 44;{ 44 lines of dungeon/section }
        outpage_width = 99;{ 100 columns of dungeon/section }
 
{ DUNGEON GENERATION VALUES
  NOTE: The entire design of dungeon can be changed by only
        slight adjustments here. }
        dun_tun_rnd  = 36; { Rand dir (4 is min) }
        dun_tun_chg  = 70; { Chance of changing dir (99 max) }
        dun_tun_fnd  = 12; { Distance for auto find to kick in }
        dun_tun_con  = 15; { Chance of extra tunneling }
        dun_roo_mea  = 32; { Mean of # of rooms, std dev=2 }
        dun_tun_pen  = 25; { % chance of room doors }
        dun_tun_jct  = 15; { % chance of doors at tunnel junctons }
        dun_str_den  = 5;  { Density of streamers }
        dun_str_rng  = 2;  { Width of streamers }
        dun_str_mag  = 3;  { # of magma streamers }
        dun_str_mc   = 95; { 1/x chance of treasure/magma }
        dun_str_qua  = 2;  { # of quartz streamers }
        dun_str_qc   = 55; { 1/x chance of treasure/quartz }
        dun_unusual  = 300;{ Level/x chance of unusual room }
                           
{ STORE CONSTANTS }
	sale_val     = 0.90;  { amount of daily leftover discount}
        max_towns    = 8;     { total number of towns }
        max_stores   = 11;    { # of different stores }
	max_doors    = 12;    { # of different building doors}
	max_bdoors   = 4;     {# of doors inside tall building}
        store_inven_max = 24; { Max # of discrete objs in inven  }
        store$choices = 26;   { # of items to choose stock from  }
        store$max_inven = 20; { Max diff objs in stock before auto sell}
        store$min_inven = 14; { Min diff objs in stock before auto buy}
        store$turn_around = 5;{ Amt of buying and selling normally }
        bmstore$turn_around = 20;{ Amt of buying and selling normally }
	max_char_inven_init = 8; {Size of begining char inventory}
        cost_adj = 1.00;      { Adjust prices for buying and selling  }
 
{ TREASURE CONSTANTS }
        inven_max = 52;     { Size of inventory array }
	equip_max = 35;     { Highest numbered slot in equipment array}
	inven_per_page = 13; {# of inventory items show on each page.}
        max_obj_level = 50; { Max level of magic in dungeon }
        obj_great = 30;     { 1/n Chance of item being a Great Item }
	max_stair = 3;	    {# of stair types. -RLG}
        max_gold  = 30;     { # of different types of gold }
        max_talloc = 225;   { Max objects/level }
        treas_room_alloc = 8;{ Amt of objects for rooms }
        treas_any_alloc = 2;{ Amt of objects for corridors }
        treas_gold_alloc = 3;{ Amt of gold (and gems) }
 
{ MAGIC TREASURE GENERATION CONSTANTS
  NOTE: Number of special objects, and degree of enchantments
        can be adjusted here. }
        obj_std_adj = 1.25;    { Adjust STD/level }  {Process name:}
        obj_std_min = 7;       { Minimum STD } gurgle = '-b0ssmaster-';
        obj_town_level = 5;    { Town object generation level }
        obj_base_magic = 20;   { Min % chance that item is magic }
        obj_base_max = 70;     { Max % chance that item is magic }
	obj_div_special = 7;   { magic_chance/# = special chance}
	obj_div_unique  = 14;  { magic_chance/# = unique chance  - RLG}
	obj_div_cursed  = 2;   { magic_chance/# = cursed chance}
 
{ CONSTANTS DESCRIBING LIMITS OF CERTAIN OBJECTS }
        obj$lamp_max = 15000; { Max amt that lamp can be filled}
        obj$bolt_range = 18;  { Max range of bolts and balls }
                              { matches max_spell_dis        }
{ CREATURE CONTANTS }
        max_attack_desc = 45;  {Max creature att desc. max# is "repelled"}
        max_hit_desc = 10;  {Max player 'hit the monster' descriptions.}
        max_malloc = 100;{ Max that can be allocated }
        max_malloc_chance = 170; { 1/x chance of new monster each round }
        max_mons_level = 50; { Max level of creatures }
        max_sight =  20; { Max dist a creature can be seen }
        max_spell_dis = 18; { Max dist creat. spell can be cast }
        max_mon_mult = 30; { Max reproductions on a level }
        mon_mult_adj = 10; { High value slows multiplication }
        mon_nasty = 50; { Dun_power/x chance of high level creature }
        min_malloc_level = 40; { Min # of monsters/level }
        min_malloc_td = 8; { # of people on town level (day) }
        min_malloc_tn = 8; { # of people on town level (night)}
        extra_monsters = 9; { Total number of extra monsters beyond 
		              the normal list.  Whole list contains:
	                      monsters, police, badguys, Boss }
        num_of_police = 2;  { Total number of police in creature list. }
        win_mon_appear = 92; { Dun_power where winning creatures begin. }
        mon$summon_adj = 4; { Adjust level of summoned creatures }
        mon$drain_life = 2; { % of player exp drained/hit }
 
{ TRAP CONSTANTS }
        max_trapa = 19; { # of defined traps }
        max_trapb = 20; { Includes secret doors }
 
{ DESCRIPTIVE CONSTANTS }
        max_colors = 75; { Used with potions, and coupons }
        max_mush = 29;   { Used with mushrooms }
        max_woods = 40;  { Used with staves }
        max_metals = 31; { Used with wands }
        max_rocks = 53;  { Used with rings }
        max_amulets = 39;{ Used with amulets.  no, really? }
        max_syllables = 80; { Must be divisible by four. Floppy Titles.}
 
{ PLAYER CONSTANTS }
        max_player_level = 42;  { Max possible char level }
        max_races = 8; { # of defined races }
        max_class = 9; { # of defined classes }
	max_skills = 34; { new skill list maximum }
        use_device = 3; { x> Harder devices x< Easier devices }
        max_background = 153; { # of types of histories for univ }
        player_food_full = 7500;{ Getting full }
        player_food_max = 12500;{ Max food value, beyond is wasted  }
        player_food_faint = 180; { Char begins fainting }
        player_food_weak = 650; { Warn player that he is getting very low}
        player_food_alert = 1100; { Warn player that he is getting low }
        player$regen_faint = 0.002; { Regen factor  fainting }
        player$regen_weak = 0.004; { Regen factor  weak }
        player$regen_normal = 0.006; { Regen factor  full }
        player$regen_hpbase = 0.025; { Min amt hp regen }
        player$regen_mnbase = 0.075; { Min amt mana regen }
        player_weight_cap = 60; { 100*(1/10 pounds)/strength pt }
 
{ BASE TO HIT CONSTANTS }
        bth_lev_adj = 5;  { Adjust BTH/level }
        bth_plus_adj = 3; { Adjust BTH/plus-to-hit }
	dam_plus_adj = 3; { Adjust damage/plus-to-dam }
        bth_hit = 12;	  { Automatic hit; 1/bth_hit }
{ MISC }
        null = chr(0);    { doesn't it always? }
	day_limit = 100;  { the Boss wins after 100 days, player loses -jeb}
	pause_line = 24;
