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
    
    
    wire    [31:0]      PC;
    wire   [31:0]       PC_4;

    wire    [31:0]      inst;
    wire                alu_src1_is_pc;
    wire                alu_src2_is_4;
    wire                alu_src2_is_imm;
    wire    [2:0]       imm_type;
    wire    [1:0]       alu_op;
    wire                br_jal;
    wire                br_beq;
    wire                br_blt;
    wire                br_bltu;
    wire                mem_to_reg;
    wire                reg_w_en;
    wire                dm_w_en;
    wire                dm_r_en;

    wire    [31:0]      Imm;

    wire    [4:0]       rs;
    wire    [4:0]       rt;
    wire    [4:0]       rd;
   
    wire    [31:0]      rs_value;
    wire    [31:0]      rt_value;
    wire    [31:0]      rd_value;

    wire        [31:0]  alu_src1_value;
    wire        [31:0]  alu_src2_value;
    wire        [31:0]  alu_out;

    wire                br_taken;
    wire        [31:0]  br_target;

    wire        [7:0]   dm_addr;
    wire        [31:0]  dm_r_data;
    wire        [31:0]  dm_w_data;

    wire                dm_w_dev_en;
    wire                dm_r_dev_en;
    wire        [31:0]  dm_r_data_raw;
    wire        [31:0]  dm_r_data_dev;


    

    wire         [4:0]  debug_rf_addr;
    wire         [7:0]  debug_dm_addr;


    assign              PC_4 = PC + 4;
    assign              rs   = inst[19:15];
    assign              rt   = inst[24:20];
    assign              rd   = inst[11:7];

    PCGenerator PCGenerator(
        .clk(clk),
        .rstn(rstn),
        .br_taken(br_taken),
        .br_target(br_target),
        .PC_4(PC_4),
        .NPC(PC)
    );

    InstructionMemory InstructionMemory(.a(PC[9:2]),.spo(inst));

    ControlDecoder ControlDecoder(
        .inst(inst),
        .alu_src1_is_pc(alu_src1_is_pc),
        .alu_src2_is_imm(alu_src2_is_imm),
        .alu_src2_is_4(alu_src2_is_4),
        .imm_type(imm_type),
        .alu_op(alu_op),
        .br_jal(br_jal),
        .br_beq(br_beq),
        .br_blt(br_blt),
        .br_bltu(br_bltu),
        .mem_to_reg(mem_to_reg),
        .reg_w_en(reg_w_en),
        .dm_w_en(dm_w_en),
        .dm_r_en(dm_r_en)
    );

    ImmGen ImmGen(
        .imm_type(imm_type),
        .inst(inst),
        .imm(Imm)
    );

    RegFile RegFile(
        .clk(clk),
        .raddr1(rs),
        .raddr2(rt),
        .rdata1(rs_value),
        .rdata2(rt_value),
        .waddr(rd),
        .wdata(rd_value),
        .w_en(reg_w_en),
        .debug_addr(debug_rf_addr),
        .debug_data(debug_rf_data)
    );


    assign alu_src1_value = alu_src1_is_pc ? PC :
                                            rs_value;
    assign alu_src2_value = alu_src2_is_4 ? 32'h4 :
                            alu_src2_is_imm ? Imm :
                            rt_value; 

    ALU ALU(
        .alu_src1(alu_src1_value),
        .alu_src2(alu_src2_value),
        .alu_op(alu_op),
        .alu_out(alu_out)
    );

    BranchDecision BranchDecision(
        .src1(alu_src1_value),
        .src2(alu_src2_value),
        .beq(br_beq),
        .jal(br_jal),
        .blt(br_blt),
        .bltu(br_bltu),
        .br_taken(br_taken)
    );

    assign br_target = PC + (Imm <<1);
    

    DataMemory  DataMemory(
        .a(alu_out[7:0]),
        .d(dm_w_data),
        .dpra(debug_dm_addr),
        .dpo(debug_dm_data),
        .clk(clk),
        .we(dm_w_en&(!dm_w_dev_en)),
        .spo(dm_r_data_raw)
        );

    assign dm_w_data     = rt_value;
    assign dm_r_data_dev = io_din;
    assign io_addr       = alu_out[7:0];
    assign io_rd         = dm_r_dev_en;
    assign io_we         = dm_w_dev_en;
    assign io_dout       = dm_w_data;

    MMIO   MMIO(
        .dm_addr(alu_out[31:0]),
        .dm_r_en(dm_r_en),
        .dm_r_data_raw(dm_r_data_raw),
        .dm_r_data_dev(dm_r_data_dev),
        .dm_w_dev_en(dm_w_dev_en),
        .dm_r_data(dm_r_data),
        .dm_w_en(dm_w_en),
        .dm_r_dev_en(dm_r_dev_en)
    );

    assign    rd_value = mem_to_reg ? dm_r_data : alu_out ;

    assign    debug_pc = PC;
    assign    debug_rf_addr  = debug_dm_rf_addr[4:0];
    assign    debug_dm_addr  = debug_dm_rf_addr[7:0];

endmodule