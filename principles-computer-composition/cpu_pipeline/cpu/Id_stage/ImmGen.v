`include "../../parameter.v"

module ImmGen(
    input   [2:0]   imm_type,
    input   [31:0]  inst,
    output  [31:0]  imm
    );

assign  imm = (imm_type == `R_TYPE) ? 32'h0 :
              (imm_type == `I_TYPE ) ? {{21{inst[31]}}, inst[30:20]} :
              (imm_type == `S_TYPE ) ? {{21{inst[31]}}, inst[30:25],inst[11:8],inst[7]} :
              (imm_type == `B_TYPE ) ? {{21{inst[31]}}, inst[7],inst[30:25],inst[11:8]} :
              (imm_type == `J_TYPE ) ? {{13{inst[31]}}, inst[19:12],inst[20],inst[30:25],inst[24:21]} :
              (imm_type == `U_TYPE) ? {inst[31:12],{12{1'b0}}} :
              32'h0;        


endmodule
