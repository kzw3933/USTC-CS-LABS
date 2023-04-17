        .ORIG x3000
        AND R7,R7,#0
        AND R6,R6,#0
        AND R5,R5,#0
        AND R4,R4,#0
        AND R3,R3,#0
        AND R2,R2,#0
        AND R1,R1,#0
        
        ADD R7,R7,#1
        ADD R1,R1,#1
        ADD R2,R2,#1
        ADD R3,R3,#2
        LD  R4,MOD
        
        ADD R0,R0,#-1   
        BRz OVER
        
DECIDE  ADD R7,R3,#0
        ADD R0,R0,#-1
        BRz OVER
        
CALC    ADD R3,R3,R1
        ADD R3,R3,R1
        ADD R1,R2,#0
        ADD R2,R7,#0
        AND R3,R3,R4
        BRnzp DECIDE
        

OVER    TRAP x25
MOD     .FILL #1023 
FA      .FILL #930
FB      .FILL #18
FC      .FILL #710
FD      .FILL #370
        .end



