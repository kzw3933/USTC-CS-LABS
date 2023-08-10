`include "../../parameter.v"

module EXE (
    input                           clk,
    input                           rstn,
    input                           flushE,
    input                           bubbleE,
    input  [`id_to_exe_bus_w-1:0]   id_to_exe_bus,
    output [`exe_to_mem_bus_w-1:0]  exe_to_mem_bus,
    output [`br_bus_w-1:0]          br_bus,
    input  [1:0]                    rs_value_from,
    input  [1:0]                    rt_value_from,
    output [4:0]                    reg_src1_exe,
    output [4:0]                    reg_src2_exe,
    input  [31:0]                   mem_result,
    input  [31:0]                   wb_result,
    output [4:0]                    rd_exe,
    output                          dm_r_en_exe,
    output                          br_taken_exe,
    output [31:0]                   debug_pc
);

    reg     [`id_to_exe_bus_w-1:0]  id_to_exe_bus_r;
    wire    [`id_to_exe_bus_w-1:0]  id_to_exe_bus_now;
    reg     [`id_to_exe_bus_w-1:0]  id_to_exe_bus_old;

    reg                             flushE_r;
    reg                             bubbleE_r;

    wire    [31:0]                  pc_exe;
    wire    [4:0]                   rs;
    wire    [4:0]                   rt;
    wire    [4:0]                   rd;
    wire    [31:0]                  rs_value;
    wire    [31:0]                  rt_value; 
    wire    [31:0]                  rs_value_exe;
    wire    [31:0]                  rt_value_exe;               
    wire    [1:0]                   alu_op;
    wire    [31:0]                  Imm;
    wire                            br_beq;
    wire                            br_blt;
    wire                            br_bltu;
    wire                            br_jal;
    wire                            mem_to_reg;
    wire                            reg_w_en;
    wire                            dm_r_en;
    wire                            dm_w_en;
    wire                            alu_src1_is_pc;
    wire                            alu_src2_is_4;
    wire                            alu_src2_is_imm;

    wire    [31:0]                  alu_src1_value;
    wire    [31:0]                  alu_src2_value;
    wire    [31:0]                  alu_out;
    wire                            br_taken;
    wire    [31:0]                  br_target;

    assign  alu_src1_value = alu_src1_is_pc ? pc_exe : rs_value_exe;
    assign  alu_src2_value = alu_src2_is_4 ? 32'h4 :
                            alu_src2_is_imm ? Imm : rt_value_exe;


    assign  rs_value_exe = (rs_value_from == 2'b00) ? rs_value :
                            (rs_value_from == 2'b01) ? mem_result :
                            (rs_value_from == 2'b10) ? wb_result : 32'h0;

    assign  rt_value_exe = (rt_value_from == 2'b00) ? rt_value :
                            (rt_value_from == 2'b01) ? mem_result :
                            (rt_value_from == 2'b10) ? wb_result : 32'h0;


    assign  rd_exe = rd;
    assign  reg_src1_exe = rs;
    assign  reg_src2_exe = rt;
    assign  dm_r_en_exe = dm_r_en;
    assign  br_taken_exe = br_taken;

    assign  {
            pc_exe,
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
            dm_r_en } = id_to_exe_bus_now;



    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            id_to_exe_bus_r <= 0;
            id_to_exe_bus_old <= 0;
            flushE_r <= 0;
            bubbleE_r <= 0;
        end    
        else begin
            id_to_exe_bus_r <= id_to_exe_bus;
            id_to_exe_bus_old <= id_to_exe_bus_now;
            flushE_r <= flushE;
            bubbleE_r <= bubbleE;
        end           
    end

    assign id_to_exe_bus_now = bubbleE_r ? id_to_exe_bus_old :
                               flushE_r ? 0: id_to_exe_bus_r;

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

    assign br_target = pc_exe + (Imm << 1);

    assign exe_to_mem_bus = {
                            pc_exe,
                            rd,
                            rt_value_exe,
                            alu_out,
                            mem_to_reg,
                            reg_w_en,
                            dm_r_en,
                            dm_w_en
                            };

    assign br_bus = {
                    br_taken,
                    br_target
                    };

    assign debug_pc = pc_exe;
    
endmodule