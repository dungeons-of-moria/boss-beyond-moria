{
 Following code is taken from early version of my other unfinished
 roguelike game. It was written when I was still learning to program
 in in Free Pascal. The code piece rivals BOSS in ugliness.  -MB

 TODO: Export octant differences into a neat table.
 TODO: Clean up ShadowCast subroutine.
}

function fixy(y : Integer) : Integer; inline;
begin
if      y < panel_row_min then fixy := panel_row_min
else if y > panel_row_max then fixy := panel_row_max
else                           fixy := y
end;

function fixx(x : Integer) : Integer; inline;
begin
if      x < panel_col_min then fixx := panel_col_min
else if x > panel_col_max then fixx := panel_col_max
else                           fixx := x
end;

procedure fov(y, x, r : Byte);

   { Return true for tile obscuring sight. }
   function blocks(x, y : Byte) : Boolean;
   const door_set = [103,105,109]; { Rubble, closed and secret doors. }
   begin
   blocks := cave[y,x].fval in pwall_set;
   if not blocks and (cave[y,x].tptr > 0) then
      blocks := t_list[cave[y,x].tptr].tval in door_set
   end;

 procedure ShadowCast(Oct, StDep : Byte; StSlop, EdSlop : Single);
 {
  Implements shadow casting for each of eight octants:

  \111|222/
  8\11|22/3
  88\1|2/33
  888\|/333
  ----@----   Lines marked by "|", "-", "/" and "\" are shared edges.
  777/|\444
  77/6|5\44
  7/66|55\4
  /666|555\
 }
 var A, B : SmallInt;
     Shot : Boolean;
     Fr, Lt : SmallInt;
 const Dst = 1 / 65;
       No = 32;
       Vr1 = No*Dst;
       Vr2 = (No+1)*Dst;

 begin
 case oct of
 1: begin
     for A := Y-StDep downto FixY(Y-R) do
      begin
       Shot := false;
       Fr := FixX(X+Round((A-Y)*StSlop));
       Lt := FixX(X+Round((A-Y)*EdSlop));
       if Lt < Fr then Lt := Fr;
       for B := Fr to Lt do
        begin
         cave[A, B].tl := true;
         if blocks(B, A) AND (B = Fr) then Shot := true;
         if blocks(B, A) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, Y-A+1, StSlop, (X-B+Vr2) / (Y-A-Vr1))
          end;
         if blocks(B, A) AND (B = Lt) then exit;
         if (NOT blocks(B, A)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (X-B+Vr1) / (Y-A+Vr1);
          end
        end
      end
    end;
 2: begin
     for A := Y-StDep downto FixY(Y-R) do
      begin
       Shot := false;
       Fr := FixX(X-Round((A-Y)*StSlop));
       Lt := FixX(X-Round((A-Y)*EdSlop));
       if Fr < Lt then Fr := Lt;
       for B := Fr downto Lt do
        begin
         cave[A, B].tl := true;
         if blocks(B, A) AND (B = Fr) then Shot := true;
         if blocks(B, A) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, Y-A+1, StSlop, (B-X+Vr2) / (Y-A-Vr1))
          end;
         if blocks(B, A) AND (B = Lt) then exit;
         if (NOT blocks(B, A)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (B-X+Vr1) / (Y-A+Vr1)
          end
        end
      end
    end;
 3: begin
     for A := X+StDep to FixX(X+R) do
      begin
       Shot := false;
       Fr := FixY(SmallInt(Y-Round((A-X)*StSlop)));
       Lt := FixY(SmallInt(Y-Round((A-X)*EdSlop)));
       if Lt < Fr then Lt := Fr;
       for B := Fr to Lt do
        begin
         cave[B, A].tl := true;
         if blocks(A, B) AND (B = Fr) then Shot := true;
         if blocks(A, B) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, A-X+1, StSlop, (Y-B+Vr2) / (A-X-Vr1))
          end;
         if blocks(A, B) AND (B = Lt) then exit;
         if (NOT blocks(A, B)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (Y-B+Vr1) / (A-X+Vr1)
          end
        end
      end
    end;
 4: begin
     for A := X+StDep to FixX(X+R) do
      begin
       Shot := false;
       Fr := FixY(Y+Round((A-X)*StSlop));
       Lt := FixY(Y+Round((A-X)*EdSlop));
       if Fr < Lt then Fr := Lt;
       for B := Fr downto Lt do
        begin
         cave[B, A].tl := true;
         if blocks(A, B) AND (B = Fr) then Shot := true;
         if blocks(A, B) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, A-X+1, StSlop, (B-Y+Vr2) / (A-X-Vr1))
          end;
         if blocks(A, B) AND (B = Lt) then exit;
         if (NOT blocks(A, B)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (B-Y+Vr1) / (A-X+Vr1)
          end
        end
      end
    end;
 5: begin
     for A := Y+StDep to FixY(Y+R) do
      begin
       Shot := false;
       Fr := FixX(X+Round((A-Y)*StSlop));
       Lt := FixX(X+Round((A-Y)*EdSlop));
       if Fr < Lt then Fr := Lt;
       for B := Fr downto Lt do
        begin
         cave[A, B].tl := true;
         if blocks(B, A) AND (B = Fr) then Shot := true;
         if blocks(B, A) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, A-Y+1, StSlop, (B-X+Vr2) / (A-Y-Vr1))
          end;
         if blocks(B, A) AND (B = Lt) then exit;
         if (NOT blocks(B, A)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (B-X+Vr1) / (A-Y+Vr1)
          end
        end
      end
    end;
 6: begin
     for A := Y+StDep to FixY(SmallInt(Y+R)) do
      begin
       Shot := false;
       Fr := FixX(X-Round((A-Y)*StSlop));
       Lt := FixX(X-Round((A-Y)*EdSlop));
       if Lt < Fr then Lt := Fr;
       for B := Fr to Lt do
        begin
         cave[A, B].tl := true;
         if blocks(B, A) AND (B = Fr) then Shot := true;
         if blocks(B, A) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, A-Y+1, StSlop, (X-B+Vr2) / (A-Y-Vr1))
          end;
         if blocks(B, A) AND (B = Lt) then exit;
         if (NOT blocks(B, A)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (X-B+Vr1) / (A-Y+Vr1)
          end
        end
      end
    end;
 7: begin
     for A := X-StDep downto FixX(X-R) do
      begin
       Shot := false;
       Fr := FixY(Y-Round((A-X)*StSlop));
       Lt := FixY(Y-Round((A-X)*EdSlop));
       if Fr < Lt then Fr := Lt;
       for B := Fr downto Lt do
        begin
         cave[B, A].tl := true;
         if blocks(A, B) AND (B = Fr) then Shot := true;
         if blocks(A, B) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, X-A+1, StSlop, (B-Y+Vr2) / (X-A-Vr1))
          end;
         if blocks(A, B) AND (B = Lt) then exit;
         if (NOT blocks(A, B)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (B-Y+Vr1) / (X-A+Vr1)
          end
        end
      end
    end;
 8: begin
     for A := X-StDep downto FixX(X-R) do
      begin
       Shot := false;
       Fr := FixY(Y+Round((A-X)*StSlop));
       Lt := FixY(Y+Round((A-X)*EdSlop));
       if Lt < Fr then Lt := Fr;
       for B := Fr to Lt do
        begin
         cave[B, A].tl := true;
         if blocks(A, B) AND (B = Fr) then Shot := true;
         if blocks(A, B) AND (Shot = false) then
          begin
           Shot := true;
           ShadowCast(Oct, X-A+1, StSlop, (Y-B+Vr2) / (X-A-Vr1))
          end;
         if blocks(A, B) AND (B = Lt) then exit;
         if (NOT blocks(A, B)) AND (Shot = true) then
          begin
           Shot := false;
           StSlop := (Y-B+Vr1) / (X-A+Vr1)
          end
        end
      end
    end
 end
 end;

var n : Byte;
begin
for n := 1 to 8 do
   ShadowCast(n, 1, 1, 0);
cave[y, x].tl := true
end;

procedure clear_fov(y, x, r : Byte);
var a, b : Byte;
begin
for a := fixy(y-r) to fixy(y+r) do
   for b := fixx(x-r) to fixx(x+r) do
      cave[a, b].tl := false
end;
