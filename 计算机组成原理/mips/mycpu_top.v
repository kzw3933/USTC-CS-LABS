`include "mycpu.h"
module mycpu_top(
    input           clk,
    input           resetn,
    // inst 
    output          inst_sram_req    ,
    output          inst_sram_wr     ,
    output   [1:0]  inst_sram_size   ,
    output   [3:0]  inst_sram_wstrb  ,
    output   [31:0] inst_sram_addr   ,
    output   [31:0] inst_sram_wdata  ,
    input           inst_sram_addr_ok,
    input           inst_sram_data_ok,
    input    [31:0] inst_sram_rdata  ,
    // data
    output          data_sram_req    ,
    output          data_sram_wr     ,
    output   [1:0]  data_sram_size   ,
    output   [3:0]  data_sram_wstrb  ,
    output   [31:0] data_sram_addr   ,
    output   [31:0] data_sram_wdata  ,
    input           data_sram_addr_ok,
    input           data_sram_data_ok,
    input    [31:0] data_sram_rdata  ,

    //debug interface
    output   [31:0] debug_wb_pc      ,
    output   [3:0]  debug_wb_rf_wen  ,
    output   [4:0]  debug_wb_rf_wnum ,
    output   [31:0] debug_wb_rf_wdata
);
wire         reset;
assign reset = ~resetn;

wire         ds_allowin;
wire         es_allowin;
wire         ms_allowin;
wire         ws_allowin;
wire         fs_to_ds_valid;
wire         ds_to_es_valid;
wire         es_to_ms_valid;
wire         ms_to_ws_valid;
wire [`FS_TO_DS_BUS_WD -1:0] fs_to_ds_bus;
wire [`DS_TO_ES_BUS_WD -1:0] ds_to_es_bus;
wire [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus;
wire [`MS_TO_WS_BUS_WD -1:0] ms_to_ws_bus;
wire [`WS_TO_RF_BUS_WD -1:0] ws_to_rf_bus;
wire [`BR_BUS_WD       -1:0] br_bus;


//forwarding
wire [1:0] alu_src1_fwd;
wire [1:0] alu_src2_fwd;
wire [4:0] es_rs1;
wire [4:0] es_rs2;
wire [4:0] ms_rd;
wire [4:0] ws_rd;
wire ms_reg_write;
wire ws_reg_write;
wire [1:0] fwd1;
wire [1:0] fwd2;
wire [31:0] ms_alu_result;
wire [31:0] ws_final_result;
wire [3: 0] ws_rf_wen;
wire [3: 0] fwd1_wen;
wire [3: 0] fwd2_wen;
wire [3: 0] alu1_fwd_wen;
wire [3: 0] alu2_fwd_wen;

//hazard
wire [4:0] ds_rs1;
wire [4:0] ds_rs2;
wire [4:0] es_rd;
wire [5:0] ds_opcode;
wire es_mem_read;
wire ms_mem_read;
wire fs_stall;
wire ds_stall;
wire es_stall;
wire ms_stall;
wire ws_stall;
wire control_mux;
wire fs_flush;
wire ds_flush;
wire es_flush;
wire ms_flush;
wire ws_flush;
wire br;
wire div_block;

wire ds_valid;
wire es_valid;
wire ms_valid;
wire ws_valid;

//  CP0 regfile
wire [31:0] cp0_wdata  ;
wire [ 7:0] cp0_addr   ;
wire [31:0] cp0_rdata  ;
wire [ 5:0] ext_int_in ;
wire [31:0] es_pc      ;
wire [ 4:0] es_excode  ;
wire        es_mtc0_we ;
wire        es_ex      ;
wire        es_bd      ;
wire        es_eret_flush ;
wire [31:0] epc        ;
wire [31:0] es_badvaddr;
wire        has_int     ;

assign ext_int_in = 0;

// IF stage
if_stage if_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ds_allowin     (ds_allowin     ),
    //brbus
    .br_bus         (br_bus         ),
    //outputs
    .fs_to_ds_valid (fs_to_ds_valid ),
    .fs_to_ds_bus   (fs_to_ds_bus   ),
    //from hazard
    .ds_flush       (ds_flush       ),
    .fs_stall       (fs_stall       ),
    // inst sram interface
    .inst_sram_req  (inst_sram_req   ),
    .inst_sram_wr   (inst_sram_wr  ),
    .inst_sram_size (inst_sram_size ),
    .inst_sram_wstrb(inst_sram_wstrb),
    .inst_sram_addr (inst_sram_addr),
    .inst_sram_wdata(inst_sram_wdata),
    .inst_sram_addr_ok(inst_sram_addr_ok),
    .inst_sram_data_ok(inst_sram_data_ok),
    .inst_sram_rdata(inst_sram_rdata),
    //eret
    .ex_taken       (es_ex          ),
    .epc            (epc            ),
    .eret_taken     (es_eret_flush  )
);
// ID stage
id_stage id_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .es_allowin     (es_allowin     ),
    .ds_allowin     (ds_allowin     ),
    //from fs
    .fs_to_ds_valid (fs_to_ds_valid ),
    .fs_to_ds_bus   (fs_to_ds_bus   ),
    //to es
    .ds_to_es_valid (ds_to_es_valid ),
    .ds_to_es_bus   (ds_to_es_bus   ),
    //to fs
    .br_bus         (br_bus         ),
    //to rf: for write back
    .ws_to_rf_bus   (ws_to_rf_bus   ),

    //from forwarding
    .fwd1           (fwd1           ),
    .fwd2           (fwd2           ),
    .fwd1_wen       (fwd1_wen       ),
    .fwd2_wen       (fwd2_wen       ),
    .ms_alu_result  (ms_alu_result  ),
    .ws_final_result(ws_final_result),
    //to forwarding
    .br             (br             ),
    .ds_valid       (ds_valid       ),
    .ds_rs1         (ds_rs1         ),
    .ds_rs2         (ds_rs2         ),
    //from hazard
    .es_flush       (es_flush       ),
    .ds_stall       (ds_stall       ),
    //to hazard
    .br_taken       (br_taken       ),
    //from cp0
    .has_int        (has_int        )
);
// EXE stage
exe_stage exe_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ms_allowin     (ms_allowin     ),
    .es_allowin     (es_allowin     ),
    //from ds
    .ds_to_es_valid (ds_to_es_valid ),
    .ds_to_es_bus   (ds_to_es_bus   ),
    //to ms
    .es_to_ms_valid (es_to_ms_valid ),
    .es_to_ms_bus   (es_to_ms_bus   ),
    //to forwarding
    .es_rs1         (es_rs1         ),
    .es_rs2         (es_rs2         ),
    .es_rd          (es_rd          ),
    //from forwarding
    .alu_src1_fwd   (alu_src1_fwd   ),
    .alu_src2_fwd   (alu_src2_fwd   ),
    .alu1_fwd_wen   (alu1_fwd_wen   ),
    .alu2_fwd_wen   (alu2_fwd_wen   ),
    .ms_alu_result  (ms_alu_result  ),
    .ws_final_result(ws_final_result),
    //from hazard
    .ms_flush       (ms_flush       ),
    .es_stall       (es_stall       ),
    //to hazard
    .es_res_from_mem(es_mem_read    ),
    .es_valid       (es_valid       ),
    .div_block      (div_block      ),
    // data sram interface
    .data_sram_req  (data_sram_req)     ,
    .data_sram_wr   (data_sram_wr)      ,
    .data_sram_size (data_sram_size)    ,
    .data_sram_wstrb(data_sram_wstrb)   ,
    .data_sram_addr (data_sram_addr)    ,
    .data_sram_wdata(data_sram_wdata)   ,
    .data_sram_addr_ok(data_sram_addr_ok),
    //CP0-regfile
    .es_cp0_wdata           (cp0_wdata  ),
    .es_cp0_rdata           (cp0_rdata  ),
    .es_cp0_addr            (cp0_addr   ),
    .es_mtc0_we             (es_mtc0_we ),
    .es_ex                  (es_ex      ),
    .es_bd                  (es_bd      ),
    .es_pc                  (es_pc      ),
    .es_excode              (es_excode  ),
    .es_eret_flush          (es_eret_flush ),
    .es_badvaddr            (es_badvaddr)
);
// MEM stage
mem_stage mem_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ws_allowin     (ws_allowin     ),
    .ms_allowin     (ms_allowin     ),
    //from es
    .es_to_ms_valid (es_to_ms_valid ),
    .es_to_ms_bus   (es_to_ms_bus   ),
    //to ws
    .ms_to_ws_valid (ms_to_ws_valid ),
    .ms_to_ws_bus   (ms_to_ws_bus   ),
    //to forwarding
    .ms_rd          (ms_rd          ),
    .ms_alu_result  (ms_alu_result  ),
    .ms_reg_write   (ms_reg_write   ),
    //from hazard
    .ws_flush       (ws_flush       ),
    .ms_stall       (ms_stall       ),
    //to hazard
    .ms_res_from_mem(ms_mem_read    ),
    .ms_valid       (ms_valid       ),
    //from data-sram
    .data_sram_data_ok(data_sram_data_ok),
    .data_sram_rdata(data_sram_rdata)   
);
// WB stage
wb_stage wb_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ws_allowin     (ws_allowin     ),
    //from ms
    .ms_to_ws_valid (ms_to_ws_valid ),
    .ms_to_ws_bus   (ms_to_ws_bus   ),
    //to rf: for write back
    .ws_to_rf_bus   (ws_to_rf_bus   ),
    //from hazard
    .ws_stall       (ws_stall       ),
    //to forwarding
    .ws_rd          (ws_rd          ),
    .ws_final_result(ws_final_result),
    .ws_reg_write   (ws_reg_write   ),
    .ws_valid       (ws_valid       ),
    .ws_rf_wen      (ws_rf_wen      ),
    //trace debug interface
    .debug_wb_pc      (debug_wb_pc      ),
    .debug_wb_rf_wen  (debug_wb_rf_wen  ),
    .debug_wb_rf_wnum (debug_wb_rf_wnum ),
    .debug_wb_rf_wdata(debug_wb_rf_wdata)
);

forwarding forwarding(
    //from ds
    .ds_rs1                 (ds_rs1         ),
    .ds_rs2                 (ds_rs2         ),
    //from es
    .es_rs1                 (es_rs1         ),
    .es_rs2                 (es_rs2         ),
    .es_rd                  (es_rd          ),
    .es_reg_write           (es_reg_write   ),
    //from ms
    .ms_rd                  (ms_rd          ),
    .ms_reg_write           (ms_reg_write   ),
    //from ws
    .ws_rd                  (ws_rd          ),
    .ws_reg_write           (ws_reg_write   ),
    .ws_rf_wen              (ws_rf_wen      ),
    //to ds
    .fwd1                   (fwd1           ),
    .fwd2                   (fwd2           ),
    .fwd1_wen               (fwd1_wen       ),
    .fwd2_wen               (fwd2_wen       ),
    //to es
    .alu_src1_fwd           (alu_src1_fwd   ),
    .alu_src2_fwd           (alu_src2_fwd   ),
    .alu1_fwd_wen           (alu1_fwd_wen   ),
    .alu2_fwd_wen           (alu2_fwd_wen   ),

    .ds_valid               (ds_valid       ),
    .es_valid               (es_valid       ),
    .ms_valid               (ms_valid       ),
    .ws_valid               (ws_valid       )
);

hazard hazard(
    //from ds
    .ds_rs1                 (ds_rs1),
    .ds_rs2                 (ds_rs2),
    .br                     (br),
    .ds_opcode              (ds_opcode),
    .br_taken               (br_taken),
    //from es 
    .es_rd                  (es_rd),
    .es_mem_read            (es_mem_read),
    .div_block              (div_block),
    .es_eret_flush          (es_eret_flush ),
    //from ms
    .ms_mem_read            (ms_mem_read),
    .ms_rd                  (ms_rd),
    //to fs
    .fs_flush               (fs_flush),
    .fs_stall               (fs_stall),
    //to ds
    .ds_stall               (ds_stall),
    .ds_flush               (ds_flush),
    .control_mux            (control_mux),
    //to es
    .es_flush               (es_flush),
    .es_stall               (es_stall),
    .es_ex                  (es_ex          ),
    //to ms
    .ms_flush               (ms_flush),
    .ms_stall               (ms_stall),
    //to ws
    .ws_flush               (ws_flush),
    .ws_stall               (ws_stall),

    .ds_valid       (ds_valid       ),
    .es_valid       (es_valid       ),
    .ms_valid       (ms_valid       ),
    .ws_valid       (ws_valid       )
);

cp0_regfile cp0_regfile(
    .clk                (clk        ),
    .reset              (reset      ),
    .cp0_wdata          (cp0_wdata  ),
    .cp0_rdata          (cp0_rdata  ),
    .cp0_addr           (cp0_addr   ),
    .ext_int_in         (ext_int_in ),
    .mtc0_we            (es_mtc0_we ),
    .ex_ex              (es_ex      ),
    .ex_bd              (es_bd      ),
    .ex_pc              (es_pc      ),
    .ex_excode          (es_excode  ),
    .eret_flush         (es_eret_flush ),
    .epc                (epc        ),
    .ex_badvaddr        (es_badvaddr),
    //to id
    .has_int            (has_int    )
    );

endmodule
