module BranchDecision(
    input   alu_zero,
    input   alu_jal,
    input   alu_beq,
    output  br_taken
);

assign br_taken = alu_zero&(alu_jal | alu_beq);

endmodule