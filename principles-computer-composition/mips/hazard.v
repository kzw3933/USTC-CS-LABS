`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/06 21:23:38
// Design Name: 
// Module Name: hazard
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


module hazard(
    //from ds
    input [4:0] ds_rs1,
    input [4:0] ds_rs2,
    input [5:0] ds_opcode,
    input br_taken,
    input br,
    //from es 
    input [4:0] es_rd,
    input es_mem_read,
    input div_block,
    input es_eret_flush,
    input es_ex,
    //from ms
    input ms_mem_read,
    input [4:0] ms_rd,
    //exception
    input expt,
    //to fs
    output fs_stall,
    output fs_flush,
    //to ds
    output ds_stall,
    output ds_flush,
    output control_mux,
    //to es
    output es_flush,
    output es_stall,
    //to ms
    output ms_flush,
    output ms_stall,
    //to ws
    output ws_flush,
    output ws_stall,

    input ds_valid,
    input es_valid,
    input ms_valid,
    input ws_valid

);
    wire [4:0] es_rd_v;
    wire es_mem_read_v;

    wire [4:0] ds_rs1_v;
    wire [4:0] ds_rs2_v;
    wire [5:0] ds_opcode_v;
    wire br_taken_v;
    wire br_v;

    wire ms_mem_read_v;
    wire [4:0] ms_rd_v;


    assign es_rd_v          = {5{es_valid}} & es_rd;
    assign es_mem_read_v    = es_valid & es_mem_read; 
    assign ds_rs1_v         = {5{ds_valid}} & ds_rs1;
    assign ds_rs2_v         = {5{ds_valid}} & ds_rs2;
    assign ds_opcode_v      = {6{ds_valid}} & ds_opcode;
    assign br_taken_v       = ds_valid & br_taken;
    assign br_v             = ds_valid & br;
    assign ms_mem_read_v    = ms_valid & ms_mem_read;
    assign ms_rd_v          = {5{ms_valid}} & ms_rd;

    //control_mux 锟�?1鏃讹紝鍋滈】娴佹按锟�?
    assign control_mux = 0;
    assign fs_stall = !es_eret_flush & !es_ex & (es_flush | div_block);
    assign ds_stall = fs_stall;
    // assign ds_flush = !ds_valid ? 0: br_taken;
    assign ds_flush = es_eret_flush || es_ex;
    assign es_flush = es_eret_flush || es_ex || ((es_mem_read_v || br_v == 1 ) && (es_rd_v == ds_rs1_v || es_rd_v == ds_rs2_v) && es_rd_v != 0) || (ms_mem_read_v && (br_v == 1) && (ms_rd_v == ds_rs1_v || ms_rd_v == ds_rs2_v) && ms_rd_v != 0);

    assign ms_flush = es_ex;
    assign ws_flush = 0;
    assign fs_flush = 0;

    assign es_stall = div_block;
    assign ms_stall = 0;
    assign ws_stall = 0;


endmodule
