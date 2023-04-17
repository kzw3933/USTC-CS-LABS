// alu_src2 为1时选择立即数作为alu的操作数2，为0时选择rt作为alu的操作数2


`include "../parameter.v"  


module ControlDecoder(
    input  [31:0]   inst,
    output          alu_src1_is_pc,
    output          alu_src2_is_imm,
    output          alu_src2_is_4,
    output [2:0]    imm_type,
    output          alu_op,
    output          alu_jal,
    output          alu_beq,
    output          mem_to_reg,
    output          reg_w_en,
    output          dm_w_en,
    output          dm_r_en
);

wire       [127:0]    inst_dec;
wire       addi,add,beq,jal,lw,sw;
decoder_7_128 decoder(.in(inst[6:0]),.out(inst_dec));

assign     addi = inst_dec[19];
assign     add  = inst_dec[51];
assign     beq  = inst_dec[99];
assign     jal  = inst_dec[111];
assign     sw   = inst_dec[35];
assign     lw   = inst_dec[3];

assign     alu_src1_is_pc  = jal;

assign     alu_src2_is_imm = addi | sw | lw;
assign     alu_src2_is_4  = jal;
assign     imm_type = (addi | lw) ? `I_TYPE :
                                    sw ? `S_TYPE :
                                        jal ? `J_TYPE :
                                            beq ? `B_TYPE :
                                                `R_TYPE;

assign     alu_op   = (addi | add | jal | sw | lw ) ? `ALU_ADD:
                                                     beq ? `ALU_SUB :
                                                          1'h0;
assign     alu_jal  = jal;
assign     alu_beq  = beq;

assign      mem_to_reg = lw ;
assign      reg_w_en   = add | addi | lw | jal ;
assign      dm_w_en   = sw ;
assign      dm_r_en   = lw ;

endmodule