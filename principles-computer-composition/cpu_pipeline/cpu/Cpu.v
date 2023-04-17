`include "../parameter.v"
`timescale 1ns / 1ps

//设计实现add，addi，sw，lw，beq，jal指令的单周期cpu
module Cpu(
    input               clk,
    input               rstn,
    //IO_BUS
    output  [7:0]       io_addr,     //led和seg的数据
    output  [31:0]      io_dout,     //输出led和seg的数据
    output              io_we,       //输出led和seg数据的使能信号
    output              io_rd,       //输入数据时的使能信号
    input   [31:0]      io_din,      //来自sw的输入数据
    //Debug_BUS
    output  [31:0]      debug_pc,          //PC的内容
    input   [7:0]       debug_dm_rf_addr,  //数据寄存器或寄存器堆的调试读口地址
    output  [31:0]      debug_rf_data,     //从RF读取的数据
    output  [31:0]      debug_dm_data      //从DM读取的数据
    );

wire               flushF;
wire               bubbleF;
wire               flushD;
wire               bubbleD;
wire               flushE;
wire               bubbleE;
wire               flushM;
wire               bubbleM;
wire               flushW;
wire               bubbleW;
wire       [`br_bus_w-1:0]   br_bus;
wire       [`if_to_id_bus_w-1:0] if_to_id_bus;
wire       [`wb_bus_w-1:0]  wb_bus;
wire       [`id_to_exe_bus_w-1:0] id_to_exe_bus;
wire       [`exe_to_mem_bus_w-1:0] exe_to_mem_bus;
wire       [`mem_to_wb_bus_w-1:0] mem_to_wb_bus;

wire                reg_w_en_mem;
wire                reg_w_en_wb;
wire       [4:0]    reg_dest_mem;
wire       [4:0]    reg_dest_wb;
wire       [4:0]    reg_src1_exe;
wire       [4:0]    reg_src2_exe;
wire       [1:0]    rs_value_from;
wire       [1:0]    rt_value_from; 
wire                alu_src1_is_pc_id;
wire                alu_src2_is_4_id;
wire                alu_src2_is_imm_id;

wire       [31:0]   mem_result;
wire       [31:0]   wb_result;

wire       [4:0]    rs_id;
wire       [4:0]    rt_id;
wire       [4:0]    rd_exe;
wire                br_taken_exe;
wire                dm_r_en_exe;



IF IF(
    .clk(clk),
    .rstn(rstn),
    .flushF(flushF),
    .bubbleF(bubbleF),
    .br_bus(br_bus),
    .if_to_id_bus(if_to_id_bus)
);

ID ID(
    .clk(clk),
    .rstn(rstn),
    .flushD(flushD),
    .bubbleD(bubbleD),
    .if_to_id_bus(if_to_id_bus),
    .wb_bus(wb_bus),
    .id_to_exe_bus(id_to_exe_bus),
    .rs_id(rs_id),
    .rt_id(rt_id),
    .debug_rf_addr(debug_dm_rf_addr[4:0]),
    .debug_rf_data(debug_rf_data),
    .alu_src1_is_pc_id(alu_src1_is_pc_id),
    .alu_src2_is_4_id(alu_src2_is_4_id),
    .alu_src2_is_imm_id(alu_src2_is_imm_id)
);

EXE EXE(
    .clk(clk),
    .rstn(rstn),
    .flushE(flushE),
    .bubbleE(bubbleE),
    .id_to_exe_bus(id_to_exe_bus),
    .exe_to_mem_bus(exe_to_mem_bus),
    .br_bus(br_bus),
    .reg_src1_exe(reg_src1_exe),
    .reg_src2_exe(reg_src2_exe),
    .mem_result(mem_result),
    .wb_result(wb_result),
    .rd_exe(rd_exe),
    .dm_r_en_exe(dm_r_en_exe),
    .br_taken_exe(br_taken_exe),
    .debug_pc(debug_pc),
    .rs_value_from(rs_value_from),
    .rt_value_from(rt_value_from)
);

MEM MEM(
    .clk(clk),
    .rstn(rstn),
    .flushM(flushM),
    .bubbleM(bubbleM),
    .exe_to_mem_bus(exe_to_mem_bus),
    .mem_to_wb_bus(mem_to_wb_bus),
    .reg_dest_mem(reg_dest_mem),
    .reg_w_en_mem(reg_w_en_mem),
    .mem_result(mem_result),
    .io_addr(io_addr),
    .io_rd(io_rd),
    .io_we(io_we),
    .io_dout(io_dout),
    .io_din(io_din),
    .debug_dm_addr(debug_dm_rf_addr[7:0]),
    .debug_dm_data(debug_dm_data)
);

WB WB(
    .clk(clk),
    .rstn(rstn),
    .flushW(flushW),
    .bubbleW(bubbleW),
    .mem_to_wb_bus(mem_to_wb_bus),
    .wb_bus(wb_bus),
    .reg_dest_wb(reg_dest_wb),
    .reg_w_en_wb(reg_w_en_wb),
    .wb_result(wb_result)
);


ForwardingUnit ForwardingUnit(
    .reg_w_en_mem(reg_w_en_mem),
    .reg_w_en_wb(reg_w_en_wb),
    .reg_dest_mem(reg_dest_mem),
    .reg_dest_wb(reg_dest_wb),
    .reg_src1_exe(reg_src1_exe),
    .reg_src2_exe(reg_src2_exe),
    .rs_value_from(rs_value_from),
    .rt_value_from(rt_value_from)
);

HazardUnit HazardUnit(
    .br_taken_exe(br_taken_exe),
    .dm_r_en_exe(dm_r_en_exe),
    .alu_src1_is_pc_id(alu_src1_is_pc_id),
    .alu_src2_is_4_id(alu_src2_is_4_id),
    .alu_src2_is_imm_id(alu_src2_is_imm_id),
    .rs_id(rs_id),
    .rt_id(rt_id),
    .rd_exe(rd_exe),
    .flushF(flushF),
    .bubbleF(bubbleF),
    .flushD(flushD),
    .bubbleD(bubbleD),
    .flushE(flushE),
    .bubbleE(bubbleE),
    .flushM(flushM),
    .bubbleM(bubbleM),
    .flushW(flushW),
    .bubbleW(bubbleW)
);
   
endmodule