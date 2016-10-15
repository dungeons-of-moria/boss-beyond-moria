 type
        byteint         = byte; //[byte] 0..255;
        bytlint         = shortint; //[byte] -128..127;            
        wordint         = word; //[word] 0..65535;
//        word		= 0..65535;
        worlint         = smallint; //[word] -32768..32767;
        unsigned        = longword;
        integer         = smallint;
        real            = single;
        quad_type       = record
                            l0  : unsigned;
                            l1  : unsigned;
                          end;
        atype           = string[16];   //varying  [16] of char;
        btype           = string[14];   //varying  [14] of char;
        ctype           = string[26];   //varying  [26] of char;
        dtype           = string[5];    //varying   [5] of char;
        etype           = string[60];   //varying  [60] of char;
        mtype           = string[190];  //varying [190] of char;
        ntype           = ansistring;   //varying[1024] of char;
        ttype           = string[68];   //varying  [68] of char;
        utype           = string[100];  //varying [100] of char;
        vtype           = string[80];   //varying  [80] of char;
	tentype		= string[10];   //varying  [10] of char;
        stat_type       = packed array [1..6] of char;
        obj_set         = set of 0..255;
        char_set        = set of 'A'..'z';
	usernam_type	= packed array [1..12] of char;
        recj	= record
             unameinfo       : packed record
               unamelen        : word;
               jpiX_username   : word;
             END;
             ptr_uname       : ^usernam_type;
             ptr_unamelen    : ^integer;
             endlist         : integer
        END;
        key_type        = record                { For char saver        }
                file_id : {[key(0)]} packed array [1..70] of char;
                seed    : integer;
        end;
        creature_type = record
                name    : ctype;        { Descrip of creature   }
                cmove   : unsigned;     { Bit field             }
                spells  : unsigned;     { Creature spells       }
                cdefense: unsigned;     { Bit field             }
                sleep   : worlint;      { Inactive counter      }
                mexp    : wordint;      { Exp value for kill    }
                aaf     : byteint;      { Area affect radius    }
                ac      : byteint;      { AC                    }
                speed   : bytlint;      { Movement speed        }
		reprod  : integer;	{ Reproduction Amount   }
		size    : integer;	{ Relative Size         }
		tt      : integer;	{ Treasure Type -- Tval }
		intel   : integer;      { Creature's Int  0-50  }
		speaks  : integer;	{ Speaking Type?        }
                cchar   : char;         { Character rep.        }
                hd      : dtype;        { Creatures hit die     }
                damage  : etype;        { Type attack and damage}
                level   : byteint;      { Level of creature     }
        end;
        monster_type = record
                hp      : worlint;      { Hit points            }
                csleep  : worlint;      { Inactive counter      }
                cdis    : worlint;      { Cur dis from player   }
                mptr    : wordint;      { Pointer into creature }
                nptr    : wordint;      { Pointer to next block }
                cspeed  : bytlint;      { Movement speed        }
                        { Note: FY and FX constrain dungeon size to 255 }
                fy      : byteint;      { Y Pointer into map    }
                fx      : byteint;      { X Pointer into map    }
 
                stuned  : -32..31;      { Rounds stunned}
                //[bit(6),pos(104)] 
                ml      : boolean;      { On if shown   }
                //[bit(1),pos(110)]
                confused: boolean;      { On if confused}
                //[bit(1),pos(111)] 
	    cur_reprod  : byteint;      { Current reproduction factor}
        end;
        treasure_type = record
                name    : utype;        { Object name           }
                tval    : byteint;      { Catagory number       }
                tchar   : char;         { Character representation}
                flags   : unsigned;     { Special flags         }
                p1      : integer;      { Misc. use variable    }
                cost    : integer;      { Cost of item          }
                subval  : integer;      { Sub-catagory number   }
                weight  : wordint;      { Weight                }
                number  : wordint;      { Number of items       }
                tohit   : worlint;      { Pluses to hit         }
                todam   : worlint;      { Pluses to damage      }
                ac      : worlint;      { Normal AC             }
                toac    : worlint;      { Pluses to AC          }
                damage  : dtype;        { Damage when hits      }
                level   : bytlint;      { Level item found      }
        end;
        player_type = record
                misc : record
                        name    : vtype;        { Name of character }
                        race    : vtype;        { Race of character }
                        sex     : vtype;        { Sex of character }
                        title   : vtype;        { Character's title }
                        tclass  : vtype;        { Character's class }
                        max_exp : LongInt;      { Max experience}
                        exp     : LongInt;      { Cur experienc }
                        au      : LongInt;      { Gold }
{ exp, max_exp and au type changed from integer to LongInt to avoid range errors
  in wizard.inc -MB }
                        age     : wordint;      { Characters age}
                        ht      : wordint;      { Height }
                        wt      : wordint;      { Weight }
                        lev     : wordint;      { Level }
                        max_lev : wordint;      { Max level explored}
                        srh     : worlint;      { Chance in search}
                        perc    : worlint;      { Perception }
                        bth     : worlint;      { Base to hit }
                        bthb    : worlint;      { BTH with bows }
                        mana    : worlint;      { Mana points }
                        mhp     : worlint;      { Max hit pts }
                        ptohit  : worlint;      { Pluses to hit }
                        ptodam  : worlint;      { Pluses to dam }
                        pac     : worlint;      { Total AC }
                        ptoac   : worlint;      { Magical AC }
                        dis_th  : worlint;      { Display +ToHit}
                        dis_td  : worlint;      { Display +ToDam}
                        dis_ac  : worlint;      { Display +ToAC }
                        dis_tac : worlint;      { Display +ToTAC}
                        disarm  : worlint;      { % to Disarm }
			wierd   : worlint;	{ Wierd Devices}
                        save    : worlint;      { Saving throw }
                        sc      : worlint;      { Social Class }
                        pclass  : byteint;      { # of class }
                        prace   : byteint;      { # of race }
			pskill  : byteint;	{ skill#  ie: 1=scientist}
						{ 2=soldier, 0=none      }
                        hitdie  : byteint;      { Char hit die }
                        stl     : bytlint;      { Stealth factor}
                        expfact : real;         { Experience factor}
                        cmana   : real;         { Cur mana pts }
                        chp     : real;         { Cur hit pts }
                        history : array [1..5] of vtype;{ History record}
                end;
                stat : record
                        str     : byteint;      { Max strength        }
                        cstr    : byteint;      { Current strength    }
                        dex     : byteint;      { Max dexterity       }
                        cdex    : byteint;      { Current dexterity   }
                        con     : byteint;      { Max constitution    }
                        ccon    : byteint;      { Current constitution}
                        int     : byteint;      { Max intelligence    }
                        cint    : byteint;      { Current intelligence}
                        wis     : byteint;      { Max wisdom          }
                        cwis    : byteint;      { Current wisdom      }
                        chr     : byteint;      { Max charisma        }
                        cchr    : byteint;      { Current charisma    }
                end;
                flags : record
                        status          : unsigned;     { Status of player    }
                        rest            : integer;      { Rest counter        }
                        blind           : integer;      { Blindness counter   }
                        paralysis       : integer;      { Paralysis counter   }
                        confused        : integer;      { Confusion counter   }
                        food            : integer;      { Food counter        }
                        food_digested   : integer;      { Food per round      }
                        protection      : integer;      { Protection fr. evil }
                        speed           : integer;      { Cur speed adjust    }
                        fast            : integer;      { Temp speed change   }
                        slow            : integer;      { Temp speed change   }
                        afraid          : integer;      { Fear                }
                        poisoned        : integer;      { Poisoned            }
                        image           : integer;      { Halucinate          }
                        protevil        : integer;      { Protect VS evil     }
                        invuln          : integer;      { Increases AC        }
                        hero            : integer;      { Heroism             }
                        shero           : integer;      { Super Heroism       }
                        blessed         : integer;      { Blessed             }
                        resist_heat     : integer;      { Timed heat resist   }
                        resist_cold     : integer;      { Timed cold resist   }
			ring_blast	: integer;	{ Fire/Frost Rings    }
			special		: integer;	{ Special Abilities   }
			petrified	: integer;	{ been stoned lately? }
			diseased	: integer;	{ various ailments    }
			detect_inv      : integer;      { Timed see invisible }
                        word_recall     : integer;      { Timed teleport level}
                        see_infra       : integer;      { See warm creatures  }
                        tim_infra       : integer;      { Timed infra vision  }
                        see_inv         : boolean;      { Can see invisible   }
                        teleport        : boolean;      { Random teleportation}
                        free_act        : boolean;      { Never paralyzed     }
                        slow_digest     : boolean;      { Lower food needs    }
                        aggravate       : boolean;      { Agravate monsters   }
                        fire_resist     : boolean;      { Resistance to fire  }
                        cold_resist     : boolean;      { Resistance to cold  }
                        acid_resist     : boolean;      { Resistance to acid  }
                        regenerate      : boolean;      { Regenerate hit pts  }
                        lght_resist     : boolean;      { Resistance to light }
                        ffall           : boolean;      { No damage falling   }
                        sustain_str     : boolean;      { Keep strength       }
                        sustain_int     : boolean;      { Keep intelligence   }
                        sustain_wis     : boolean;      { Keep wisdom         }
                        sustain_con     : boolean;      { Keep constitution   }
                        sustain_dex     : boolean;      { Keep dexterity      }
                        sustain_chr     : boolean;      { Keep charisma       }
                        confuse_monster : boolean;      { Glowing hands...    }
                end;
        end;                              
        skill_type = record		  {Values in skill_values.inc}
                sname   : ctype;
                slevel  : byteint;
                smana   : byteint;
                sexp    : wordint;
                sfail   : byteint;
                learned : boolean;
        end;
        spl_rec = record
                splnum  : integer;
                splchn  : integer;
        end;
        spl_type = array [1..22] of spl_rec;
         race_type = packed record
                trace   : vtype;        { Type of race }
                str_adj : bytlint;      { adjustments }
                int_adj : bytlint;
                wis_adj : bytlint;
                dex_adj : bytlint;
                con_adj : bytlint;
                chr_adj : bytlint;
                b_age   : wordint; { Base age of character }
                m_age   : wordint; { Maximum age of character }
                m_b_ht  : wordint; { base height for males }
                m_m_ht  : wordint; { mod height for males       }
                m_b_wt  : wordint; { base weight for males      }
                m_m_wt  : wordint; { mod weight for males       }
                f_b_ht  : wordint; { base height females        }
                f_m_ht  : wordint; { mod height for females     }
                f_b_wt  : wordint; { base weight for female     }
                f_m_wt  : wordint; { mod weight for females     }
                b_exp   : real;    { Base experience factor     }
                b_dis   : bytlint; { base chance to disarm      }
                bsrh    : bytlint; { base chance for search     }
                bstl    : bytlint; { Stealth of character       }
                bperc   : bytlint; { frequency of auto search   }
                bbth    : bytlint; { adj base chance to hit     }
                bbthb   : bytlint; { adj base to hit with bows  }
		bwierd  : bytlint; { Race base for wierd device }
                bsav    : bytlint; { Race base for saving throw }
                bhitdie : bytlint; { Base hit points for race   }
                binfra  : bytlint; { See infra-red              }
                tclass  : unsigned; { Bit field for class types }
        end;
        class_type = record
                title   : vtype;  { type of class }
                m_exp   : real;   { Class experience factor }
                adj_hd  : bytlint;{ Adjust hit points }
                mdis    : bytlint;{ mod disarming traps }
                msrh    : bytlint;{ modifier to searching }
                mstl    : bytlint;{ modifier to stealth }
                mperc   : bytlint;{ modifier to freq-of-search  }
                mbth    : bytlint;{ modifier to base to hit     }
                mbthb   : bytlint;{ modifier to base to hit - bows}
		mwierd  : bytlint;{ Class modifier for wierd devices}
                msav    : bytlint;{ Class modifier to save      }
                madj_str: bytlint;{ Class modifier for strength}
                madj_int: bytlint;{ Class modifier for intelligence}
                madj_wis: bytlint;{ Class modifier for wisdom   }
                madj_dex: bytlint;{ Class modifier for dexterity}
                madj_con: bytlint;{ Class modifier for constitution}
                madj_chr: bytlint;{ Class modifier for charisma}
		skill_no: byteint;{ Class Skill Number.  0=none}
				  { 1=sci, 2=sold, 3=guru, 4=wanderer, }
				  { 5=con-artist, 6=thief}
       end;
        background_type = record
                info    : vtype;  { History information        }
                roll    : byteint;{ Die roll needed for history}
                chart   : byteint;{ Table number             }
                next    : bytlint;{ Pointer to next table       }
                bonus   : bytlint;{ Bonus to the Social Class   }
        end;
        floor_type = record
                ftval   : 0..15;   //[bit(7),pos(0)] 0..15;
                ftopen  : boolean; //[bit(1),pos(7)] boolean;
        end;
        cave_type = record
                cptr    : byteint;
                tptr    : byteint;
                fval    : 0..15;   //[bit(4),pos(16)] 0..15;
                fopen   : boolean; //[bit(1),pos(20)] boolean;
                fm      : boolean; //[bit(1),pos(21)] boolean;
                pl      : boolean; //[bit(1),pos(22)] boolean;
                tl      : boolean; //[bit(1),pos(23)] boolean;
        end;
        row_floor  = array [1..max_width] of cave_type;
        owner_type = record
                owner_name      : vtype;
                beg_effect      : byteint;
                buy_inflate     : real;
                sell_deflate    : real;
                owner_class     : byteint;
        end;
        inven_record = record
                scost           : integer;
                sitem           : treasure_type;
        end;
        store_type = record
                store_open      : worlint;
                owner           : byteint;  {now just a dummy variable}
                insult_cur      : bytlint;
                store_ctr       : byteint;
                store_inven     : array [1..store_inven_max] of inven_record;
        end;
        town_type = record
		town_name	: atype;
		max_depth	: integer;
		lev_plus	: byteint;
        end;
        hiscore_type = record
                playername      : string[12];
                score           : LongInt;
                charactername   : string[25];
                level           : Byte;
                race            : string[13];
                tclass          : string[11]
        end;


