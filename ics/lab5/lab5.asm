        .ORIG x3000
        JSR JUDGE
        HALT
JUDGE   AND R6,R6,#0
        ADD R6,R6,#2             
        AND R1,R1,#0
        ADD R1,R1,#1             
HEAD    AND R4,R4,#0
        ADD R5,R6,#0
LOOP    ADD R4,R4,R6
        ADD R5,R5,#-1
        BRp LOOP
        NOT R4,R4
        ADD R4,R4,#1            
        ADD R4,R0,R4
        BRn Tail
        NOT R4,R6
        ADD R4,R4,#1
        ADD R3,R0,#0
Calc    ADD R3,R3,R4
        BRp Calc
        BRz BREAK
        ADD R6,R6,#1
        BRnzp HEAD
BREAK   AND R1,R1,#0         
Tail    RET
        .END