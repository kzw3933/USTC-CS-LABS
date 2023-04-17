`timescale 1ns / 1ps
//  功能说明
    //  判断是否branch
// 输入
    // reg1               寄存�??1
    // reg2               寄存�??2
    // br_type            branch类型
// 输出
    // br                 是否branch
// 实验要求
    // 补全模块

`include "Parameters.v"   
module BranchDecision(
    input wire [31:0] reg1, reg2,
    input wire [2:0]  br_type,
    input wire [1:0]  bht_pred_br,
    input wire        bht_hit,
    output wire       br,
    input  wire       br_op,
    output wire [1:0] bht_w_en,
    output wire       bht_w_br
    );

    // TODO: Complete this module

//@HIDE-BEGIN
    wire signed [31:0] reg1_s, reg2_s;
    assign reg1_s = reg1;
    assign reg2_s = reg2;
//@HIDE-END

    reg   br_raw;

    assign br = br_raw && (!bht_pred_br[1]) || !br_raw && bht_pred_br[1] ;

    wire   br_btb_wrong;
    assign br_btb_wrong = br_raw && (!bht_pred_br[0]) || !br_raw && bht_pred_br[0] ;

    assign bht_w_en[0] = br_op?(bht_hit ? br_btb_wrong : 1'b1):
                            1'b0;

    assign bht_w_en[1] = br_op ? 1'b1 : 1'b0;
    
    assign bht_w_br = br_raw;

    always @ (*)
    begin
        case(br_type)
            `NOBRANCH: br_raw = 0;
            `BEQ: br_raw = (reg1 == reg2) ? 1 : 0;
            `BLTU: br_raw = (reg1 < reg2) ? 1 : 0;
//@BEGIN 
            `BNE: br_raw = (reg1 != reg2) ? 1 : 0;
            `BGEU: br_raw = (reg1 >= reg2) ? 1 : 0;
            `BLT: br_raw = (reg1_s < reg2_s) ? 1 : 0;
            `BGE: br_raw = (reg1_s >= reg2_s) ? 1 : 0;
//@END
            default: br_raw = 0;
        endcase
    end

endmodule
