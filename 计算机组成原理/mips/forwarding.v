`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/06 18:33:11
// Design Name: 
// Module Name: forwarding
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module forwarding(
    //from es
    input [4:0] es_rs1,
    input [4:0] es_rs2,
    input [4:0] es_rd,
    input es_reg_write,
    //from ms
    input [4:0] ms_rd,
    input ms_reg_write,
    //from ws
    input [4:0] ws_rd,
    input ws_reg_write,
    input [3:0] ws_rf_wen,
    //to es
    output [1:0] alu_src1_fwd,
    output [1:0] alu_src2_fwd,
    output [3:0] alu1_fwd_wen,
    output [3:0] alu2_fwd_wen,
    //from ds
    input [4:0] ds_rs1,
    input [4:0] ds_rs2,
    //to ds
    output [1:0] fwd1,
    output [1:0] fwd2,
    output [3:0] fwd1_wen,
    output [3:0] fwd2_wen,

    input ds_valid,
    input es_valid,
    input ms_valid,
    input ws_valid
    );
//fwd淇″彿涓?0鏃讹紝涓嶅墠閫掞紱涓?1鏃讹紝鍓嶉?抦s_alu_out锛涗负2鏃讹紝鍓嶉?抴addr
// assign alu_src1_fwd = {2{(ds_rs1 == ms_rd) && (ms_rd != 5'd0) && ms_reg_write}} & 2'd1
//                     | {2{(ds_rs1 == ws_rd) && (ws_rd != 5'd0) && ws_reg_write}} & 2'd2 ;

// assign alu_src2_fwd = {2{(ds_rs2 == ms_rd) && (ms_rd != 5'd0) && ms_reg_write}} & 2'd1
//                     | {2{(ds_rs2 == ws_rd) && (ws_rd != 5'd0) && ws_reg_write}} & 2'd2 ;

assign fwd1_wen = (fwd1 == 2'd2) ? ws_rf_wen : 4'b1111;
assign fwd2_wen = (fwd2 == 2'd2) ? ws_rf_wen : 4'b1111;
assign alu1_fwd_wen = (alu_src1_fwd == 2'd2) ? ws_rf_wen : 4'b1111;
assign alu2_fwd_wen = (alu_src2_fwd == 2'd2) ? ws_rf_wen : 4'b1111;

assign alu_src1_fwd = !(es_valid) ? 0
                     : ((es_rs1 == ms_rd) && (ms_rd != 5'd0) && ms_reg_write && ms_valid) ? 2'd1 
                     : ((es_rs1 == ws_rd) && (ws_rd != 5'd0) && ws_reg_write && ws_valid) ? 2'd2 
                     : 0;

assign alu_src2_fwd = !(es_valid) ? 0
                     : ((es_rs2 == ms_rd) && (ms_rd != 5'd0) && ms_reg_write && ms_valid) ? 2'd1 
                     : ((es_rs2 == ws_rd) && (ws_rd != 5'd0) && ws_reg_write && ws_valid) ? 2'd2 
                     : 0;

//fwd涓?0鏃讹紝涓嶅墠閫掞紱涓?1鏃讹紝鍓嶉?抦s_alu_out锛涗负2鏃讹紝鍓嶉?抴s_rdata
assign fwd1 = !(ds_valid) ? 0
            : ((ds_rs1 == ms_rd) && (ms_rd != 5'd0) && ms_reg_write && ms_valid) ? 2'd1
            : ((ds_rs1 == ws_rd) && (ws_rd != 5'd0) && ws_reg_write && ws_valid) ? 2'd2
            : 0;

assign fwd2 = !(ds_valid) ? 0
            : ((ds_rs2 == ms_rd) && (ms_rd != 5'd0) && ms_reg_write && ms_valid) ? 2'd1
            : ((ds_rs2 == ws_rd) && (ws_rd != 5'd0) && ws_reg_write && ws_valid) ? 2'd2
            : 0;

endmodule
