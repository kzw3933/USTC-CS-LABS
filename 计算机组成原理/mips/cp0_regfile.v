`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/30 21:07:11
// Design Name: 
// Module Name: cp0_regfile
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
//`include "mycpu.h"
    `define CR_STATUS 8'b01100_000
    `define CR_CAUSE 8'b01101_000
    `define CR_COUNT 8'b01001_000
    `define CR_COMPARE 8'b01011_000
    `define CR_EPC 8'b01110_000
    `define CR_BADVADDR 8'b01000_000
    
    `define EXC_INT 8'h00
    `define EXC_ADEL 8'h04
    `define EXC_ADES 8'h05
    `define EXC_SYS 8'h08
    `define EXC_BP 8'h09
    `define EXC_RI 8'h0a
    `define EXC_OV 8'h0c


module cp0_regfile(
    input           clk,
    input           reset,
    input [31: 0]   cp0_wdata,
    input [ 7: 0]   cp0_addr,
    input [ 5: 0]   ext_int_in,
    input           mtc0_we,
    input           ex_ex,
    input           ex_bd,
    input [31:0]    ex_pc,
    input [ 4:0]    ex_excode,
    input           eret_flush,
    output [31: 0]  cp0_rdata,
    output [31: 0]  epc,
    input  [31: 0]  ex_badvaddr,
    output          has_int
    );


//========count==========
reg tick;
reg [31:0] cp0_count;
always@(posedge clk)
begin
    if(reset)
        tick <= 1'b0;
    else 
        tick = ~tick;

    if(mtc0_we && cp0_addr == `CR_COUNT)
        cp0_count <= cp0_wdata;
    else if(tick)
        cp0_count = cp0_count + 1;
end

//========compare==========
reg [31:0] cp0_compare;
wire count_eq_compare;
always@(posedge clk)
begin
    if(reset)
        cp0_compare <= 0;
    else if(mtc0_we && cp0_addr == `CR_COMPARE)
        cp0_compare <= cp0_wdata;
end
assign count_eq_compare = (cp0_count == cp0_compare);

//========status==========
wire [31:0] cp0_status;
wire cp0_status_bev;
reg [7:0] cp0_status_im;
reg cp0_status_exl;
reg cp0_status_ie;
assign cp0_status = {
    9'b0,
    cp0_status_bev,
    6'b0,
    cp0_status_im,
    6'b0,
    cp0_status_exl,
    cp0_status_ie
};
assign cp0_status_bev = 1'b1;
always@(posedge clk)
begin
    if(mtc0_we && cp0_addr == `CR_STATUS)
        cp0_status_im <= cp0_wdata[15:8];
end

always@(posedge clk)
begin
    if(reset)
        cp0_status_exl <= 1'b0;
    else if(ex_ex)
        cp0_status_exl <= 1'b1;
    else if(eret_flush)
        cp0_status_exl <= 1'b0;
    else if(mtc0_we && cp0_addr == `CR_STATUS)
        cp0_status_exl <= cp0_wdata[1];
end

always@(posedge clk)
begin
    if(reset)
        cp0_status_ie <= 1'b0;
    else if(mtc0_we && cp0_addr == `CR_STATUS)
        cp0_status_ie <= cp0_wdata[0];
end
    
    
//========cause==========
wire [31:0] cp0_cause;
reg cp0_cause_bd;
reg cp0_cause_ti;
reg [7:0] cp0_cause_ip;
reg [4:0] cp0_cause_excode;
assign cp0_cause = {
    cp0_cause_bd,
    cp0_cause_ti,
    14'b0,
    cp0_cause_ip,
    1'b0,
    cp0_cause_excode,
    2'b0
};
always@(posedge clk)
begin
    if(reset)
        cp0_cause_bd <= 1'b0;
    else if(ex_ex && !cp0_status_exl)
        cp0_cause_bd <= ex_bd;
end
always@(posedge clk)
begin
    if(reset)
        cp0_cause_ti <= 1'b0;
    else if(mtc0_we && cp0_addr == `CR_COMPARE)
        cp0_cause_ti <= 1'b0;
    else if(count_eq_compare)
        cp0_cause_ti <= 1'b1;
end
always@(posedge clk)
begin
    if(reset)
        cp0_cause_ip[7:2] <= 6'b0;
    else begin
        cp0_cause_ip[7]   <= ext_int_in[5] | cp0_cause_ti;
        cp0_cause_ip[6:2] <= ext_int_in[4:0];
    end
end
always@(posedge clk)
begin
    if(reset)
        cp0_cause_ip[1:0] <= 2'b0;
    else if(mtc0_we && cp0_addr == `CR_CAUSE)
        cp0_cause_ip[1:0] <=cp0_wdata[9:8];
end
always@(posedge clk)
begin
    if(reset)
        cp0_cause_excode <= 5'b0;
    else if(ex_ex)
        cp0_cause_excode <= ex_excode;
end
//========epc==========
reg [31:0] cp0_epc;
always@(posedge clk)
begin
    if(ex_ex && !cp0_status_exl)
        cp0_epc <= ex_bd ? ex_pc - 3'h4 : ex_pc;
    else if(mtc0_we && cp0_addr == `CR_EPC)
        cp0_epc <= cp0_wdata;
end

//=======BadVAddr=======
reg [31:0] cp0_badvaddr;
always@(posedge clk)
begin
    if(ex_ex && (ex_excode == `EXC_ADEL || ex_excode == `EXC_ADES))
        cp0_badvaddr <= ex_badvaddr;
end

assign epc = cp0_epc;
assign cp0_rdata = {32{cp0_addr == `CR_COUNT}} & cp0_count
                 | {32{cp0_addr == `CR_COMPARE}} & cp0_compare
                 | {32{cp0_addr == `CR_STATUS}} & cp0_status
                 | {32{cp0_addr == `CR_CAUSE}} & cp0_cause
                 | {32{cp0_addr == `CR_EPC}} & cp0_epc
                 | {32{cp0_addr == `CR_BADVADDR}} & cp0_badvaddr;

//===========Int===========
assign has_int = ((cp0_cause_ip & cp0_status_im) != 8'h00) && cp0_status_ie && cp0_status_exl == 0;

endmodule
