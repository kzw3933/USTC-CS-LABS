module BranchDecision(
    input           blt,
    input           bltu,
    input           jal,
    input           beq,
    input   [31:0]  src1,
    input   [31:0]  src2,
    output          br_taken
);

assign br_taken = blt ? ($signed(src1) < $signed(src2)) :
                        bltu ? (src1 < src2) :
                        beq ? (src1 == src2) :
                        jal ? 1:
                        0 ;

endmodule