{ Moria Version 5.0     COPYRIGHT (c) Robert Alan Koeneke
                        Public Domain
                   Modified EXTENSIVELY by:
                    Matthew W. Koch   -MWK
                   with minor help from:
                    Kendall R. Sears  -Opusii
                    Russell E. Billings -REB
                    David G. Strubel    -DGS
 
        I lovingly dedicate this game to hackers and adventurers
        everywhere...
 
        Designer and Programmer : Robert Alan Koeneke
                                  University of Oklahoma
        Assitant Programmer     : Jimmey Wayne Todd
                                  University of Oklahoma
 
        Moria may be copied and modified freely as long as the above
        credits are retained.  No one who-so-ever may sell or market
        this software in any form without the expressed written consent
        of the author Robert Alan Koeneke.
 
_______________________________________________________________________
BOSS version 2.4  by Robert Gulledge
		  and Jason Black

BOSS version 2.4b by Michal Bielinski
}
 { [inherit('sys$share:starlet'), environment('BOSS.env')] }

  program BOSS;
        {$DEFINE SH_DEBUG}


        {$INCLUDEPATH inc}
        {$I-}

  uses crt, dateutils, math, strutils, sysutils;

        { Globals }
        {$INCLUDE constants.inc}
        {$INCLUDE types.inc}
        {$INCLUDE variables.inc}

        { Global Values }
        {.$INCLUDE objects.inc}
        // included in globals
 
        { Libraries of routines }
        {$INCLUDE io.inc}
	{$INCLUDE misc.inc}
	{$INCLUDE treasure.inc}
        {$INCLUDE help.inc}
        {$INCLUDE desc.inc}
        {$INCLUDE files.inc}
        {$INCLUDE death.inc}
        {$INCLUDE store1.inc}
        {$INCLUDE datafiles.inc}
        {$INCLUDE save.inc}
        {$INCLUDE create.inc}
        {$INCLUDE generate.inc}
        {$INCLUDE main.inc}

     { Initialize, restore, and get the ball rolling. }
 
 BEGIN
       { Get the directory location of the image}
    get_paths;
 
       { Some neccesary initializations }

    msg_line       := 1;
    quart_height   := screen_height div 4;
    quart_width    := screen_width div 4;
    dun_level      := 0;
    turn           := 5760; {8:00 a.m.}

    wierd_chance   := 8640;

{ Grab a random seed from the clock }
    //seed := get_seed;
    randomize; { this should be enough for now -MB }
 
{ Read in the monster and object data files. }
    read_data;

{ Sort the objects by level }
    sort_objects;
 
        { Init monster and treasure levels for allocate }
    init_m_level;
    init_t_level;
 
        { Init the store inventories }
    store_init;
    { If you wish call this to change economical game balance. 
      1.00 is default value and does not need to run price_adjust. -MB}
    //price_adjust(1.00);

        { Check or create hours.dat, print message }
    if paramcount = 1 then 
      finam := paramstr(1);
    intro('');

        { Generate a character, or retrieve old one...  }
    if load_game(finam) = true then
      BEGIN     { Retrieve character }
        generate := get_char(finam);
        change_name;
        magic_init(system.randseed)
      END
    else
      BEGIN     { Create character }
        create_character;
        char_inven_init;
        if (py.misc.pskill in [1,2,7]) then
          BEGIN 
            learn_spell(msg_flag);
            gain_mana(int_adj)
          END;
        if (py.misc.pskill in [3,4]) then
          BEGIN         
            learn_prayer;
            gain_mana(wis_adj)
          END;
        if (py.misc.pskill in [5,6]) then
          BEGIN
            learn_extra(msg_flag);
            gain_mana(chr_adj)
          END;
        py.misc.cmana := py.misc.mana;
        magic_init(system.randseed);
        generate := true;
      END;

{ begin the game }
      with py.misc do     { This determines the maximum player experience }
        player_max_exp := trunc(player_exp[max_player_level-1]*expfact);
      clear(1,1);
      prt_stat_block;

{ Loop till dead, or exit }
    repeat
      if (generate) then generate_cave; { New level }
      dungeon; { Dungeon logic-located in Main.Inc }
      generate := true
    until (death);
    upon_death { Character gets buried }
  END.
 
 

