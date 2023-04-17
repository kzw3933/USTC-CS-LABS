// alu_src2 为1时选择立即数作为alu的操作数2，为0时选择rt作为alu的操作数2


`include "../parameter.v"  


module ControlDecoder(
    input  [31:0]   inst,
    output          alu_src1_is_pc,
    output          alu_src2_is_imm,
    output          alu_src2_is_4,
    output [2:0]    imm_type,
    output [1:0]    alu_op,
    output          br_jal,
    output          br_beq,
    output          br_blt,
    output          br_bltu,
    output          mem_to_reg,
    output          reg_w_en,
    output          dm_w_en,
    output          dm_r_en
);

wire       [127:0]    opcode_dec;
wire       [127:0]    func7_dec;
wire       [7:0]      func3_dec;
wire       addi,add,beq,jal,lw,sw,auipc,lui,blt,bltu,sub;

decoder_7_128 decoder_7_128_opcode(.in(inst[6:0]),.out(opcode_dec));
decoder_7_128 decoder_7_128_func7(.in(inst[31:25]),.out(func7_dec));
decoder_3_8 decoder_3_8_func3(.in(inst[14:12]),.out(func3_dec));

assign     addi = opcode_dec[19] & func3_dec[0];
assign     add  = opcode_dec[51] & func3_dec[0] & func7_dec[0];
assign     beq  = opcode_dec[99] & func3_dec[0];
assign     jal  = opcode_dec[111];
assign     sw   = opcode_dec[35] & func3_dec[2];
assign     lw   = opcode_dec[3] & func3_dec[2];
assign     sub  = opcode_dec[51] & func3_dec[0] & func7_dec[32] ;
assign     auipc = opcode_dec[23];
assign     lui = opcode_dec[55];
assign     blt = opcode_dec[99] & func3_dec[4];
assign     bltu = opcode_dec[99] & func3_dec[6];

assign     alu_src1_is_pc  = jal | auipc;

assign     alu_src2_is_imm = addi | sw | lw | auipc | lui;
assign     alu_src2_is_4  = jal;
assign     imm_type = (addi | lw) ? `I_TYPE :
                       sw ? `S_TYPE :
                       jal ? `J_TYPE :
                       (beq | blt | bltu )? `B_TYPE :
                       (auipc | lui) ? `U_TYPE :
                        `R_TYPE;

assign     alu_op   = (addi | add | jal | sw | lw ) ? `ALU_ADD:
                      sub ? `ALU_SUB :
                      lui ? `ALU_OP2 :
                      2'h0;
                      
assign     br_jal  = jal;
assign     br_beq  = beq;
assign     br_blt  = blt;
assign     br_bltu = bltu;

assign      mem_to_reg  = lw ;
assign      reg_w_en    = add | addi | lw | jal | sub | auipc | lui;
assign      dm_w_en     = sw ;
assign      dm_r_en     = lw ;

endmodule