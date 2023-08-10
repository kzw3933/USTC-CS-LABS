`include "../../parameter.v"

module ID (
    input                           clk,
    input                           rstn,
    input                           flushD,
    input                           bubbleD,
    input   [`if_to_id_bus_w-1:0]   if_to_id_bus,
    input   [`wb_bus_w-1:0]         wb_bus,
    output  [`id_to_exe_bus_w-1:0]  id_to_exe_bus,
    output  [4:0]                   rs_id,
    output  [4:0]                   rt_id,
    input   [4:0]                   debug_rf_addr,
    output  [31:0]                  debug_rf_data,
    output                          alu_src1_is_pc_id,
    output                          alu_src2_is_imm_id,
    output                          alu_src2_is_4_id
    );

    reg     [`if_to_id_bus_w-1:0]   if_to_id_bus_r;
    wire    [`if_to_id_bus_w-1:0]   if_to_id_bus_now;
    reg     [`if_to_id_bus_w-1:0]   if_to_id_bus_old;
    reg                             bubbleD_r;
    reg                             flushD_r;

    wire    [31:0]                  pc_id;
    wire    [31:0]                  inst;
    wire    [31:0]                  wb_data;
    wire    [4:0]                   wb_addr;
    wire                            wb_en;

    wire                            alu_src1_is_pc;
    wire                            alu_src2_is_4;
    wire                            alu_src2_is_imm;
    wire    [2:0]                   imm_type;
    wire    [1:0]                   alu_op;
    wire                            br_beq;
    wire                            br_blt;
    wire                            br_bltu;
    wire                            br_jal;
    wire                            mem_to_reg;
    wire                            reg_w_en;
    wire                            dm_r_en;
    wire                            dm_w_en;

    wire    [31:0]                  Imm;
    wire    [4:0]                   rs;
    wire    [4:0]                   rt;
    wire    [4:0]                   rd;
    wire    [31:0]                  rs_value;
    wire    [31:0]                  rt_value;

    assign rs = inst[19:15];
    assign rt = inst[24:20];
    assign rd = inst[11:7];

    assign rs_id = rs;
    assign rt_id = rt;

    assign alu_src1_is_pc_id = alu_src1_is_pc;
    assign alu_src2_is_4_id = alu_src2_is_4;
    assign alu_src2_is_imm_id = alu_src2_is_imm;

    assign  {pc_id,
            inst
    } = if_to_id_bus_now;

    assign  if_to_id_bus_now = bubbleD_r ? if_to_id_bus_old :
                               flushD_r  ? 0:if_to_id_bus_r;


    assign  {
            wb_data,
            wb_en,
            wb_addr
            } = wb_bus;

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            if_to_id_bus_r <= 0;
            if_to_id_bus_old <= 0;
            bubbleD_r <= 0;
            flushD_r <= 0;
        end      
        else begin
            if_to_id_bus_r <= if_to_id_bus;
            if_to_id_bus_old <= if_to_id_bus_now;
            bubbleD_r <= bubbleD;
            flushD_r <= flushD;
        end
            
    end

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
        .waddr(wb_addr),
        .wdata(wb_data),
        .w_en(wb_en),
        .debug_addr(debug_rf_addr),
        .debug_data(debug_rf_data)
    );



    assign   id_to_exe_bus = {
                            pc_id,
                            rs,
                            rt,
                            rs_value,
                            rt_value,
                            rd,
                            alu_src1_is_pc,
                            alu_src2_is_4,
                            alu_src2_is_imm,
                            alu_op,
                            Imm,
                            br_beq,
                            br_blt,
                            br_bltu,
                            br_jal,
                            mem_to_reg,
                            reg_w_en,
                            dm_w_en,
                            dm_r_en
                    };
  
endmodule