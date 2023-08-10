`include "../parameter.v"
module  ALU(
    input  [31:0] alu_src1,
    input  [31:0] alu_src2,
    input  [1:0]  alu_op,
    output [31:0] alu_out
    );

assign  alu_out = (alu_op == `ALU_ADD) ? (alu_src1 + alu_src2) :
                  (alu_op == `ALU_SUB) ? (alu_src1 - alu_src2) :
                  (alu_op == `ALU_OP1) ? alu_src1 :
                  (alu_op == `ALU_OP2) ? alu_src2 :
                  32'h0;
                                                               
endmodule