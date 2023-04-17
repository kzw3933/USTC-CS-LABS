`include "../parameter.v"
module  ALU(
    input  [31:0] alu_src1,
    input  [31:0] alu_src2,
    input         alu_beq,
    input         alu_jal,
    input         alu_op,
    output [31:0] alu_out,
    output        alu_zero
    );

assign  alu_out = (alu_op == `ALU_ADD) ? (alu_src1 + alu_src2) :
                                        (alu_op == `ALU_SUB) ? (alu_src1 - alu_src2) :
                                                                32'h0;

assign  alu_zero = alu_jal ? 1 :
                            (alu_beq & (alu_out == 0)) ? 1 :
                                                         0;


endmodule