   value

{ Residual objects.  Main object list is now in an external file! -RLG }

{ traplista are hidden,  traplistb are detected}
{ Traps are just Nasty treasures... }
        trap_lista := (
('an open pit',101,' ',%X'00000000',0,0,1,0,0,0,0,0,0,'2d6',-50),
('an arrow trap',101,'.',%X'00000000',0,0,2,0,0,0,0,0,0,'1d8',0),
('a covered pit',101,'.',%X'00000000',0,0,3,0,0,0,0,0,0,'2d6',0),
('a trap door',101,'.',%X'00000000',0,0,4,0,0,0,0,0,0,'2d8',0),
('a gas trap' ,101,'.',%X'00000000',0,0,5,0,0,0,0,0,0,'1d4',0),
('a loose rock' ,101,'.',%X'00000000',0,0,6,0,0,0,0,0,0,'0d0',0),
('a dart trap',101,'.',%X'00000000',0,0,7,0,0,0,0,0,0,'1d4',0),
('a strange rune',101,'.',%X'00000000',0,0,8,0,0,0,0,0,0,'0d0',0),
('some loose rock' ,101,'.',%X'00000000',0,0,9,0,0,0,0,0,0,'2d6',0),
('a gas trap' ,101,'.',%X'00000000',0,0,10,0,0,0,0,0,0,'1d4',0),
('a strange rune',101,'.',%X'00000000',0,0,11,0,0,0,0,0,0,'0d0',0),
('a blackened spot',101,'.',%X'00000000',0,0,12,0,0,0,0,0,0,'4d6',0),
('some corroded rock',101,'.',%X'00000000',0,0,13,0,0,0,0,0,0,'4d6',0),
('a gas trap',101,'.',%X'00000000',0,0,14,0,0,0,0,0,0,'2d6',0),
('a gas trap',101,'.',%X'00000000',5,0,15,0,0,0,0,0,0,'1d4',10),
('a bomb',101,'.',%X'00000000',5,0,16,0,0,0,0,0,0,'5d6',5),
('a dart trap',101,'.',%X'00000000',5,0,17,0,0,0,0,0,0,'1d8',10),
('a dart trap',101,'.',%X'00000000',5,0,18,0,0,0,0,0,0,'1d8',10),
('a teleport trap',101,'.',%X'00000000',5,0,20,0,0,0,0,0,0,'0d0',10));
 
{ Traps: Level represents the difficulty of disarming;
         and P1 represents the experienced gained when disarmed}
 
        trap_listb := (
('an open pit',102,' ',%X'00000000',1,0,1,0,0,0,0,0,0,'2d6',-50),
('an arrow trap',102,'^',%X'00000000',3,0,2,0,0,0,0,0,0,'1d8',-10),
('a covered pit',102,'^',%X'00000000',2,0,3,0,0,0,0,0,0,'2d6',-40),
('a trap door',102,'^',%X'00000000',5,0,4,0,0,0,0,0,0,'2d8',-25),
('a gas trap' ,102,'^',%X'00000000',3,0,5,0,0,0,0,0,0,'1d4',5),
('a loose rock' ,102,';',%X'00000000',0,0,6,0,0,0,0,0,0,'0d0',-90),
('a dart trap',102,'^',%X'00000000',5,0,7,0,0,0,0,0,0,'1d4',10),
('a strange rune',102,'^',%X'00000000',5,0,8,0,0,0,0,0,0,'0d0',-10),
('some loose rock' ,102,'^',%X'00000000',5,0,9,0,0,0,0,0,0,'2d6',-10),
('a gas trap',102,'^',%X'00000000',10,0,10,0,0,0,0,0,0,'1d4',5),
('a strange rune' ,102,'^',%X'00000000',5,0,11,0,0,0,0,0,0,'0d0',-10),
('a blackened spot',102,'^',%X'00000000',10,0,12,0,0,0,0,0,0,'4d6',10),
('some shattered rock',102,'^',%X'00000000',10,0,13,0,0,0,0,0,0,'4d6',10),
('a gas trap',102,'^',%X'00000000',5,0,14,0,0,0,0,0,0,'2d6',5),
('a gas trap',102,'^',%X'00000000',5,0,15,0,0,0,0,0,0,'1d4',10),
('a bomb',102,'^',%X'00000000',25,0,16,0,0,0,0,0,0,'5d6',25),
('a dart trap',102,'^',%X'00000000',5,0,17,0,0,0,0,0,0,'1d8',10),
('a dart trap',102,'^',%X'00000000',5,0,18,0,0,0,0,0,0,'1d8',10),
 
{ SPECIAL CASE,see DOOR_LIST below (SUBVALS MUST AGREE) }
('a closed door',105,'+',%X'00000000',0,0,19,0,0,0,0,0,0,'1d1',0),
('a teleport trap',102,'^',%X'00000000',0,0,20,0,0,0,0,0,0,'0d0',50));
 
blk_mkt_trap :=
('a Black Market Store',111,'B',%X'00000000',5,0,107,0,0,0,0,0,0,'1d1',5);
 
 
{ Stores are just special traps }
 
        store_door := (
('The entrance to a K-Mart',110,'K',%X'00000000',
0,0,101,0,0,0,0,0,0,'0d0',0),
('The entrance to Leather and Chains',110,'L',%X'00000000',
0,0,102,0,0,0,0,0,0,'0d0',0),
('The entrance to a Weapon Shop',110,'W',%X'00000000',
0,0,103,0,0,0,0,0,0,'0d0',0),
('The entrance to a Temple',110,'T',%X'00000000',
0,0,104,0,0,0,0,0,0,'0d0',0),
('The entrance to a Bar and Grill',110,'+',%X'00000000',
0,0,105,0,0,0,0,0,0,'0d0',0),
('The entrance to an Alien Artifact Shop',110,'A',%X'00000000',
0,0,106,0,0,0,0,0,0,'0d0',0),
('The entrance to a Black Market',110,'B',%X'00000000',
0,0,107,0,0,0,0,0,0,'0d0',1),
('The entrance to a Gun Shop',110,'G',%X'00000000',
0,0,108,0,0,0,0,0,0,'0d0',0),
('The entrance to a Pro Shop',110,'P',%X'00000000',
0,0,109,0,0,0,0,0,0,'0d0',0),
('The entrance to a Computer Store',110,'C',%X'00000000',
0,0,110,0,0,0,0,0,0,'0d0',0),
('a granite wall with the vague outline of a door.',110,'#',%X'00000000',
0,0,111,0,0,0,0,0,0,'0d0',0),
('The entrance to a tall building',110,'<',%X'00000000',
0,0,112,0,0,0,0,0,0,'0d0',0));
 
    building_door  := (
('the hotel front desk',120,']',%X'00000000',
0,0,201,0,0,0,0,0,0,'0d0',0),
('the bank teller window',120,'[',%X'00000000',
0,0,202,0,0,0,0,0,0,'0d0',0),
('the medical clinic entrance',120,']',%X'00000000',
0,0,203,0,0,0,0,0,0,'0d0',0),
('NewsInc, Ltd.',120,'[',%X'00000000',
0,0,204,0,0,0,0,0,0,'0d0',0)
);
	

  scare_monster :=
('a strange rune',102,'^',%X'00000000',0,0,99,0,0,0,0,0,0,'0d0',-90);
 
rubble := ('some rubble',103,':',%X'00000000',0,0,1,0,0,0,0,0,0,'0d0',0);
 
{ Secret door must have same subval as closed door in TRAP_LISTB.
  See CHANGE_TRAP}
 door_list := (
('an open door',104,'''',%X'00000000',0,0,1,0,0,0,0,0,0,'1d1',0),
('a closed door',105,'+',%X'00000000',0,0,19,0,0,0,0,0,0,'1d1',0),
('a secret door',109,'#',%X'00000000',0,0,19,0,0,0,0,0,0,'1d1',0));
 
 up_stair := (
('an staircase going up',107,'<',%X'00000000',
0,0,1,0,0,0,0,0,0,'1d1',0),
('a steep staircase leading upwards',107,'<',%X'00000000',
0,0,2,0,0,0,0,0,0,'1d1',0),
('a *very* steep staircase leading way, way up',107,'<',%X'00000000',
0,0,4,0,0,0,0,0,0,'1d1',0));
 
 down_stair := (
('a staircase going downwards',108,'>',%X'00000000',
0,0,1,0,0,0,0,0,0,'1d1',0),
('a steep staircase leading downwards',108,'>',%X'00000000',
0,0,2,0,0,0,0,0,0,'1d1',0),
('a *very* steep staircase leading way, way down',108,'>',%X'00000000',
0,0,4,0,0,0,0,0,0,'1d1',0));
 
 
{ Sets for easy testing }
  floor_set   := [1,2,4,5,6,7];  wall_set    := [10,11,12];
  pwall_set   := [10,11,12,15];  corr_set    := [4,5];
  trap_set    := [101,102,109,110,111,120];
  light_set   := [102,103,104,105,107,108,109,110,111];
