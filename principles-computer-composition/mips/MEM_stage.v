`include "mycpu.h"

module mem_stage(
    input                          clk           ,
    input                          reset         ,
    //allowin
    input                          ws_allowin    ,
    output                         ms_allowin    ,
    //from es
    input                          es_to_ms_valid,
    input  [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus  ,
    //to ws
    output                         ms_to_ws_valid,
    output [`MS_TO_WS_BUS_WD -1:0] ms_to_ws_bus  ,
    //from data-sram
    input                          data_sram_data_ok,
    input    [31:0]                data_sram_rdata  ,
    //to forwarding
    output [4                  :0] ms_rd         ,
    output [31                 :0] ms_alu_result ,
    output                         ms_reg_write  ,
    //from hazard
    input                               ws_flush,
    input                               ms_stall,
    //to hazard
    output                         ms_res_from_mem,
    output reg                     ms_valid
);

wire        ms_ready_go;

reg [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus_r;
wire        ms_gr_we;
wire [ 4:0] ms_dest;
wire [31:0] ms_pc;
wire        ms_l_is_lw      ;
wire        ms_l_is_lb      ;
wire        ms_l_is_lbu     ;
wire        ms_l_is_lh      ;
wire        ms_l_is_lhu     ;
wire        ms_l_is_lwl     ;
wire        ms_l_is_lwr     ;
wire [1:0]  ms_low_addr     ;
wire [31:0] lb_rdata        ;
wire [31:0] lbu_rdata       ;
wire [31:0] lh_rdata        ;
wire [31:0] lhu_rdata       ;
wire [3: 0] ms_rf_wen       ;

assign {ms_data_sram_req     ,  //84
        ms_l_is_lwl    ,  //83
        ms_l_is_lwr    ,  //82
        ms_rf_wen      ,  //81:78
        ms_low_addr    ,  //77:76
        ms_l_is_lw     ,  //75
        ms_l_is_lb     ,  //74
        ms_l_is_lbu    ,  //73
        ms_l_is_lh     ,  //72
        ms_l_is_lhu    ,  //71
        ms_res_from_mem,  //70:70
        ms_gr_we       ,  //69:69
        ms_dest        ,  //68:64
        ms_alu_result  ,  //63:32
        ms_pc             //31:0
       } = es_to_ms_bus_r;
assign ms_rd = ms_dest;
assign ms_reg_write = ms_gr_we;

wire [31:0] mem_result;
wire [31:0] final_mem_result;
wire [31:0] ms_final_result;

assign ms_to_ws_bus = {
                       ms_rf_wen      ,  //73:70
                       ms_gr_we       ,  //69:69
                       ms_dest        ,  //68:64
                       ms_final_result,  //63:32
                       ms_pc             //31:0
                      };

assign ms_ready_go    = !ms_valid || !ms_stall;
assign ms_allowin     = !ms_valid || ms_ready_go && ws_allowin;
assign ms_to_ws_valid = ms_valid && ms_ready_go && !ws_flush &&(!ms_data_sram_req || data_sram_data_ok);
always @(posedge clk) begin
    if (reset) begin
        ms_valid <= 1'b0;
    end
    else if (ms_allowin) begin
        ms_valid <= es_to_ms_valid;
    end

    if (es_to_ms_valid && ms_allowin) begin
        es_to_ms_bus_r  = es_to_ms_bus;
    end
end

assign lb_rdata = {32{ms_low_addr == 2'b00}} & {{24{data_sram_rdata[7]}},data_sram_rdata[7:0]}
                | {32{ms_low_addr == 2'b01}} & {{24{data_sram_rdata[15]}},data_sram_rdata[15:8]}
                | {32{ms_low_addr == 2'b10}} & {{24{data_sram_rdata[23]}},data_sram_rdata[23:16]}
                | {32{ms_low_addr == 2'b11}} & {{24{data_sram_rdata[31]}},data_sram_rdata[31:24]};               
assign lbu_rdata ={32{ms_low_addr == 2'b00}} & {{24{1'b0}},data_sram_rdata[7:0]}
                | {32{ms_low_addr == 2'b01}} & {{24{1'b0}},data_sram_rdata[15:8]}
                | {32{ms_low_addr == 2'b10}} & {{24{1'b0}},data_sram_rdata[23:16]}
                | {32{ms_low_addr == 2'b11}} & {{24{1'b0}},data_sram_rdata[31:24]};
assign lh_rdata = {32{ms_low_addr == 2'b00}} & {{16{data_sram_rdata[15]}},data_sram_rdata[15:0]}
                | {32{ms_low_addr == 2'b10}} & {{16{data_sram_rdata[31]}},data_sram_rdata[31:16]};
assign lhu_rdata ={32{ms_low_addr == 2'b00}} & {{16{1'b0}},data_sram_rdata[15:0]}
                | {32{ms_low_addr == 2'b10}} & {{16{1'b0}},data_sram_rdata[31:16]};


assign mem_result =  {32{ms_l_is_lb}}  & lb_rdata
                   | {32{ms_l_is_lbu}} & lbu_rdata
                   | {32{ms_l_is_lh}}  & lh_rdata
                   | {32{ms_l_is_lhu}} & lhu_rdata
                   | {32{ms_l_is_lw | ms_l_is_lwl | ms_l_is_lwr}}  & data_sram_rdata;

assign final_mem_result = ms_l_is_lwl ? (mem_result << (8*(2'd3 - ms_low_addr)))
                        : ms_l_is_lwr ? (mem_result >> (8*ms_low_addr))
                        : mem_result;

assign ms_final_result = ms_res_from_mem ? final_mem_result
                                         : ms_alu_result;

endmodule
