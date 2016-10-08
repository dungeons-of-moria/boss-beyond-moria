var     
    tempfile : text;  {debugging file}
    scumuser :  boolean; { used when detecting users who shouldn't play }
    putzuser :  boolean;
    validuser : boolean;
    resist_gas : [psect(player$data)] boolean;  {Soon to be a py.flag}
    max_weight : [psect(weight$code)] integer;
    is_boss_dead : [psect(boss$code)] boolean;
    total_winner_num : [psect(setup$data)] integer;
    defeated_badguy : [psect(setup$data)] boolean;
    out_of_time : boolean; { 100-day time limit flag. -jeb}
    max_creatures : [psect(moria$data)] integer; {total # of monsters in mlist}
    max_objects : [psect(moria$data)] integer; {total # in objects.dat }
    inven_init_max : [psect(moria$data)] integer; { Size of store inventory }
    player_max_exp : [psect(player$data)] integer;  { Max exp possible    }
    seed : [psect(player$data),global] unsigned; { Contains seed # }
    randes_seed : [psect(setup$data)] unsigned; { For encoding colors }
    town_seed : [psect(generate$data)] unsigned; { Seed for town genera}
    channel : [psect(io$data),global] integer;  { I/O channel #   }
    io$bin_pause : [psect(io$data)] quad_type;    { I/O pause time  }
    cur_height,cur_width : [psect(player$data)] integer;  { Cur dungeon size}
    dun_level : [psect(player$data)] integer;  { Cur dungeon level   }
    dun_power : [psect(player$data)] integer;  { Cur power of dun level}
    town_num  : [psect(player$data)] integer;  { Cur town player is in -RLG}
    missle_ctr : [psect(player$data)] integer;  { Counter for missles }
    msg_line : [psect(player$data)] integer;  { Contains message txt}
    msg_flag : [psect(player$data)] boolean;  { Set with first msg  }
    more_flag: [psect(player$data)] boolean;  { Question -more- on/off -jeb}
    wall_flag: [psect(player$data)] boolean;  { Toggle Neat Wall Paterns. -RLG}
    old_msg : [psect(player$data)] vtype;    { Last message    }
    generate : [psect(setup$data)] boolean;  { Generate next level }
    death : [psect(player$data)] boolean;  { True if died    }
    died_from : [psect(player$data)] vtype;    { What killed him }
    find_flag : [psect(player$data)] boolean;  { Used in MORIA   }
    reset_flag : [psect(player$data)] boolean;  { Used in MORIA   }
    cave_flag : [psect(player$data)] boolean;  { Used in GET_PANEL   }
    light_flag : [psect(player$data)] boolean;  { Used in MOVE_LIGHT  }
    redraw : [psect(player$data)] boolean;  { For redraw screen   }
    stat_column : [psect(player$data)] integer;  { Column for stats    }
    print_stat : [psect(player$data)] unsigned; { Flag for stats  }
    turn : [psect(player$data)] integer;  { Cur turn of game    }
    day : [psect(player$data)] integer;  { day counter     -RLG   }
    wizard : [psect(player$data)] boolean;  { Wizard flag }
    used_line : [psect(player$data)] array [2..23] of boolean;
    password1 : [psect(setup$data)] usernam_type;
    password2 : [psect(setup$data)] usernam_type;
    days : [psect(setup$data)] array [1..7] of vtype;
    closing_flag : [psect(player$data)] integer;  { Used for closing   }

    {Bank Stuff} 
    act_bal       : [psect(store$data)] integer;
    principal     : [psect(store$data)] integer;
    payment_count : [psect(store$data)] integer;
    iod_val, iol_val, danny_cut  : [psect(store$data)] real;
    
    { Bit testing array }
    bit_array : [psect(player$data)] array [1..32] of unsigned;
 
    { External file names; are all located in directory with image  }
    BOSS_HOU,BOSS_MOR   : [psect(setup$data)] vtype;
    BOSS_MAS,BOSS_TOP   : [psect(setup$data)] vtype;    
    BOSS_HLP,BOSS_MON   : [psect(setup$data)] vtype;
    BOSS_OBJ,BOSS_LOS   : [psect(setup$data)] vtype;
    BOSS_WIZ,BOSS_ANNOY : [psect(setup$data)] vtype;
    BUS_PIC ,QUOTES     : [psect(setup$data)] vtype;
    BOSS_INV,SKILLS     : [psect(setup$data)] vtype;
   
    {  Following are calculated from max dungeon sizes  }
    max_panel_rows,max_panel_cols : [psect(player$data)] integer;
    quart_height,quart_width : [psect(player$data)] integer;
    panel_row,panel_col : [psect(player$data)] integer;
    panel_row_min,panel_row_max : [psect(player$data)] integer;
    panel_col_min,panel_col_max : [psect(player$data)] integer;
    panel_col_prt,panel_row_prt : [psect(player$data)] integer;
 
    {  Following are all floor definitions  }
    cave : [psect(moria$data)] array [1..max_height] of row_floor;
    blank_floor : [psect(generate$data)] cave_type;
    dopen_floor : [psect(generate$data)] floor_type;
    lopen_floor : [psect(generate$data)] floor_type;
    corr_floor1, corr_floor2 : [psect(generate$data)] floor_type;
    corr_floor3, corr_floor4 : [psect(generate$data)] floor_type;
    rock_wall1, rock_wall2   : [psect(generate$data)] floor_type;
    rock_wall3, boundry_wall : [psect(generate$data)] floor_type;
 
    {  Following are set definitions    }
    floor_set, wall_set : [psect(moria$data)] obj_set;
    pwall_set, corr_set : [psect(moria$data)] obj_set;
    trap_set, light_set : [psect(moria$data)] obj_set;
    alpha_set : [psect(moria$data)] etype;
    open_cave_set : [psect(moria$data)] obj_set;
    circle    : [psect(generate$data)] array [1..9] of integer; {Circle Room}
    { Following are player variables    }
    py : [psect(player$data)] player_type;
    player_title : [psect(player$data)]
	    array [1..max_class] of array [1..max_player_level] of btype;
    player_exp : [psect(player$data)] array [1..max_player_level] of integer;
    acc_exp : [psect(player$data)] real; { Accumulator for fractional exp}
    bare_hands : [psect(player$data)] dtype;
    char_row : [psect(player$data)] integer;
    char_col : [psect(player$data)] integer;
    com_val : [psect(player$data)] integer;
    pclass : [psect(player$data)] integer;
    sex_type : [psect(player$data)] vtype;
    race : [psect(create$data)] array [1..max_races] of race_type;
    background : [psect(create$data)] array [1..max_background] of background_type;
    rgold_adj : [psect(store$data)] array [1..max_class] of array [1..max_class] of real;
    class : [psect(create$data)] array [1..max_class] of class_type;

    skill_list : [psect(player$data)] array [1..max_skills] of skill_type;

    mush : [psect(setup$data)] treasure_type;
    player_init : [psect(create$data)] 
                     array [1..max_class] of array [1..8] of byteint;
    berz_y  : [psect(moria$data)] array [1..8] of integer;
    berz_x  : [psect(moria$data)] array [1..8] of integer;
    { Following are store definitions   }
    office_char_row : [psect(generate$data)] integer;
    office_char_col : [psect(generate$data)] integer;
    exited_office : [psect(generate$data)] boolean;
    owners : [psect(store$data)] array [1..max_stores] of
				 array [1..max_towns] of owner_type;
    store : [psect(store$data)] array [1..max_stores] of store_type;
    store_door : [psect(generate$data)] array [1..max_doors] of treasure_type;
    building_door : [psect(generate$data)] array 
					   [1..max_bdoors] of treasure_type;
    store_choice : [psect(store$data)] array [1..max_stores] of array 
[1..store$choices] of integer;
    store_buy : [psect(store$data)] array [1..max_stores] of obj_set;
    joblist   :	[psect(store$data)] array [1..9] of ctype;
    jobwage   :	[psect(store$data)] array [1..9] of byteint;
    townlist  : [psect(towns$data)] array [1..max_towns] of town_type;

    { Following are treasure arrays and variables   }
    object_list : [psect(moria$data)] array [1..450] of treasure_type;
    object_ident : [psect(moria$data)] array [1..450] of boolean;
    t_level : [psect(moria$data)] array [0..max_obj_level] of integer;
    gold_list : [psect(moria$data)] array [1..max_gold] of treasure_type;
    t_list : [psect(moria$data)] array [1..max_talloc] of treasure_type;
    inventory : [psect(player$data)] array [1..inven_max] of treasure_type;
    equipment : [psect(player$data)] array [23..equip_max] of treasure_type;
    temporary_slot : [psect(player$data)] treasure_type;
    inventory_init : [psect(setup$data)] array [1..300] of treasure_type;
    blank_treasure : [psect(moria$data)] treasure_type;
    inven_ctr : [psect(player$data)] integer;  { Total different obj's }
    inven_weight : [psect(player$data)] integer;  { Cur carried weight    }
    equip_ctr : [psect(player$data)] integer;  { Cur equipment ctr }
    tcptr : [psect(moria$data)] integer;  { Cur treasure heap ptr }
 
    { Following are feature objects defined for dungeon }
    trap_lista : [psect(generate$data)] array [1..max_trapa] of treasure_type;
    trap_listb : [psect(generate$data)] array [1..max_trapb] of treasure_type;
    blk_mkt_trap : [psect(generate$data)] treasure_type; {Black Market trap}
    scare_monster : [psect(generate$data)] treasure_type; {Special trap}
    rubble : [psect(generate$data)] treasure_type;
    door_list : [psect(generate$data)] array [1..3] of treasure_type;
    up_stair : [psect(generate$data)] array [1..max_stair] of treasure_type;
    down_stair : [psect(generate$data)] array [1..max_stair] of treasure_type; 
 
    { Following are creature arrays and variables   }
    c_list : [psect(moria$data)] array [1..400] of creature_type;
    m_list : [psect(moria$data)] array [1..max_malloc] of monster_type;
    m_level : [psect(generate$data)] array [0..max_mons_level] of integer;
    blank_monster : [psect(generate$data)] monster_type; {Blank monster values}
    muptr : [psect(generate$data)] integer;  { Cur used monster ptr  }
    mfptr : [psect(generate$data)] integer;  { Cur free monster ptr  }
    mon_tot_mult : [psect(moria$data)] integer;  { # of repro's of creature  }
    attack_desc : [psect(creature$data)] array [1..max_attack_desc] of etype;
    hit_desc : [psect(creature$data)] array [1..max_hit_desc] of btype;
    kill_list: [psect(creature$data)] array [1..400] of integer;

    { Following are arrays for descriptive pieces   }
    colors : [psect(setup$data)] array [1..max_colors] of atype;
    mushrooms : [psect(setup$data)] array [1..max_mush] of atype;
    woods : [psect(setup$data)] array [1..max_woods] of atype;
    metals : [psect(setup$data)] array [1..max_metals] of atype;
    rocks : [psect(setup$data)] array [1..max_rocks] of atype;
    amulets : [psect(setup$data)] array [1..max_amulets] of atype;
    syllables : [psect(setup$data)] array [1..max_syllables] of tentype;
    vowel_set : [psect(moria$data)] char_set;
 
    { Following are variables for the Save Character Routines   }
    finam : [psect(save$data)] vtype;
    data_corrupt: [psect(save$data)] boolean;
    key_rec : [psect(save$data)] key_type;
 
    { Cursor variables, used for cursor positioning }
    cursor_r : [psect(io$data),global] array [1..24] of varying[10] of char;
    curlen_r : [psect(io$data),global] integer;
    cursor_c : [psect(io$data),global] array [1..80] of varying[10] of char;
    curlen_c : [psect(io$data),global] integer;
    cursor_l : [psect(io$data),global] integer;
    row_first : [psect(io$data),global] boolean;
    cursor_erl : [psect(io$data),global] varying[10] of char;
    cursor_erp : [psect(io$data),global] varying[10] of char;
    raoul  : [psect(screen$data)] array [0..3] of char;
    wall_chr : [psect(screen$data)] tentype;
    username :  usernam_type;  {duh...}
    wierd_chance : integer;
