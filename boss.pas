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
BOSS version 1.0  by Robert Gulledge
		  and jason black
}
 { [inherit('sys$share:starlet'), environment('BOSS.env')] }

  program BOSS(input,output); 

        { Globals }
        %INCLUDE 'BOSS_INCLUDE:CONSTANTS.INC'
        %INCLUDE 'BOSS_INCLUDE:TYPES.INC'
        %INCLUDE 'BOSS_INCLUDE:VARIABLES.INC'

        {Global Values}
        %INCLUDE 'BOSS_INCLUDE:VALUES.INC'
        %INCLUDE 'BOSS_INCLUDE:OBJECTS.INC'
 
        { Libraries of routines }
        %INCLUDE 'BOSS_INCLUDE:IO.INC'
	%INCLUDE 'BOSS_INCLUDE:MISC.INC'
	%INCLUDE 'BOSS_INCLUDE:TREASURE.INC'
        %INCLUDE 'BOSS_INCLUDE:HELP.INC'
        %INCLUDE 'BOSS_INCLUDE:DESC.INC'
        %INCLUDE 'BOSS_INCLUDE:FILES.INC'
        %INCLUDE 'BOSS_INCLUDE:DEATH.INC'
        %INCLUDE 'BOSS_INCLUDE:STORE1.INC'
        %INCLUDE 'BOSS_INCLUDE:DATAFILES.INC'
        %INCLUDE 'BOSS_INCLUDE:SAVE.INC'
        %INCLUDE 'BOSS_INCLUDE:CREATE.INC'
        %INCLUDE 'BOSS_INCLUDE:GENERATE.INC'
        %INCLUDE 'BOSS_INCLUDE:MAIN.INC'
        %INCLUDE 'BOSS_INCLUDE:TERMDEF.INC'
 
     { Initialize, restore, and get the ball rolling. }
 
 BEGIN
       { SYSPRV stays off except when needed...}
    priv_switch(0);
 
       { Check the terminal type and see if it is supported}
    termdef;
 
       { Get the directory location of the image}
    get_paths;
 
       { Setup pause time for IO setup_io_pause; }

       {Check to see if user is a wiz, scum, or just annoying} 
    look_at_userid;

       { Some neccesary initializations }

    msg_line       := 1;
    quart_height   := trunc(screen_height/4);
    quart_width    := trunc(screen_width /4);
    dun_level      := 0;
    turn           := 5760; {8:00 a.m.}
    if (putzuser) then
      wierd_chance   := 2160  {make it a really wierd game...} 
    else
      wierd_chance   := 8640;

{ Init an IO channel for QIO }
    init_channel;
 
{ Grab a random seed from the clock }
    seed := get_seed;
 
{Read in the monster and object data files.} 
    read_data;

{ Sort the objects by level }
    sort_objects;
 
        { Init monster and treasure levels for allocate }
    init_m_level;
    init_t_level;
 
        { Init the store inventories }
    store_init;
    if (cost_adj <> 1.00) then price_adjust;
 
        { Check operating hours
          If not wizard then No_Control_Y }
    get_foreign(finam);
 
        { Check or create hours.dat, print message }
    intro(finam);
 
        { Generate a character, or retrieve old one...  }
    if (length(finam) > 0) then
      BEGIN     { Retrieve character }
        generate := get_char(finam);
        change_name;
        magic_init(randes_seed)
      END
    else
      BEGIN     { Create character }
        create_character;
        char_inven_init;
        if (py.misc.pskill in [1,2,7]) then
          BEGIN 
            learn_spell(msg_flag);
            gain_mana(int_adj);
          END;
        if (py.misc.pskill in [3,4]) then
          BEGIN         
            learn_prayer;
            gain_mana(wis_adj);
          END;
        if (py.misc.pskill in [5,6]) then
          BEGIN
            learn_extra(msg_flag);
            gain_mana(chr_adj)
          END;
        py.misc.cmana := py.misc.mana;
        randes_seed := seed;            { Description seed }
        town_seed   := seed;            { Town generation seed  }
        magic_init(randes_seed);
        generate := true
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
      generate := true;
    until (death);
    upon_death; { Character gets buried }
  END.
 
 

