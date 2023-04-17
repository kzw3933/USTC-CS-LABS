`include "mycpu.h"

module exe_stage(
    input                          clk           ,
    input                          reset         ,
    //allowin
    input                          ms_allowin    ,
    output                         es_allowin    ,
    //from ds
    input                          ds_to_es_valid,
    input  [`DS_TO_ES_BUS_WD -1:0] ds_to_es_bus  ,
    //to ms
    output                         es_to_ms_valid,
    output [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus  ,
    //to forwarding
    output [4                  :0] es_rs1        ,
    output [4                  :0] es_rs2        ,
    //from forwarding
    input  [1                  :0] alu_src1_fwd  ,
    input  [1                  :0] alu_src2_fwd  ,
    input  [3                  :0] alu1_fwd_wen  ,
    input  [3                  :0] alu2_fwd_wen  ,
    input  [31                 :0] ms_alu_result ,
    input  [31                 :0] ws_final_result,
    output [4                  :0] es_rd,
    //from hazard
    input                           ms_flush,
    input                           es_stall,
    //to hazard           
    output                         es_res_from_mem,
    output reg                     es_valid,
    output                         div_block,
    // data sram interface
    output                         data_sram_req    ,
    output                         data_sram_wr     ,
    output   [1:0]                 data_sram_size   ,
    output   [3:0]                 data_sram_wstrb  ,
    output   [31:0]                data_sram_addr   ,
    output   [31:0]                data_sram_wdata  ,
    input                          data_sram_addr_ok,
    //cp0
    output [31:0]   es_cp0_wdata,
    output [7:0]    es_cp0_addr,
    input  [31:0]   es_cp0_rdata,
    output [31:0]   es_pc,
    output [ 4:0]   es_excode,
    output          es_mtc0_we,
    output          es_ex,
    output          es_bd,
    output          es_eret_flush,
    output [31:0]   es_badvaddr
);


wire        es_ready_go   ;

reg  [`DS_TO_ES_BUS_WD -1:0] ds_to_es_bus_r;
wire [15:0] es_alu_op       ;
wire        es_load_op      ;
wire        es_src1_is_sa   ;  
wire        es_src1_is_pc   ;
wire        es_src2_is_imm  ;
wire        es_src2_is_0imm ; 
wire        es_src2_is_8    ;
wire        es_gr_we        ;
wire        es_mem_we       ;
wire [ 4:0] es_dest         ;
wire [15:0] es_imm          ;
wire [31:0] es_rs_value     ;
wire [31:0] es_rt_value     ;
wire        es_res_is_hi    ;
wire        es_res_is_lo    ;
wire        es_hi_wen       ;
wire        es_lo_wen       ;        
wire        es_l_is_lw      ;
wire        es_l_is_lb      ;
wire        es_l_is_lbu     ;
wire        es_l_is_lh      ;
wire        es_l_is_lhu     ;
wire        es_l_is_lwl     ;
wire        es_l_is_lwr     ;
wire        es_s_is_sw      ;
wire        es_s_is_sb      ;
wire        es_s_is_sh      ;
wire        es_s_is_swl     ;
wire        es_s_is_swr     ;
wire [3:0]  sb_wen          ;
wire [3:0]  sh_wen          ;
wire [3:0]  swl_wen         ;
wire [3:0]  swr_wen         ;
wire [1:0]  low_addr        ;
wire [3:0]  es_rf_wen       ;
wire [3:0]  lwl_wen         ;
wire [3:0]  lwr_wen         ;
wire [31:0] swl_wdata       ;
wire [31:0] swr_wdata       ;


wire        ds_ex           ;
wire [4:0]  ds_excode       ;
wire        es_res_from_cp0 ;
wire        ds_eret_flush   ;
wire        es_overflow_en  ;
wire        es_overflow     ;

assign es_eret_flush = ds_eret_flush & es_valid;

assign {
        es_overflow_en      ,  //185
        es_bd               ,  //184
        ds_eret_flush       ,  //183
        es_mtc0_we          ,  //182
        es_cp0_addr         ,  //181:174
        es_res_from_cp0     ,  //173
        ds_ex               ,  //172
        ds_excode           ,  //171:167
        es_l_is_lwl         ,  //166
        es_l_is_lwr         ,  //165
        es_l_is_lw          ,  //164
        es_l_is_lb          ,  //163
        es_l_is_lbu         ,  //162
        es_l_is_lh          ,  //161
        es_l_is_lhu         ,  //160
        es_s_is_swl         ,  //159
        es_s_is_swr         ,  //158
        es_s_is_sw          ,  //157
        es_s_is_sb          ,  //156
        es_s_is_sh          ,  //155:
        es_hi_wen           ,  //154:154
        es_lo_wen           ,  //153:153
        es_res_is_hi        ,  //152:152
        es_res_is_lo        ,  //151:151
        es_src2_is_0imm     ,  //150:150
        es_rs2              ,  //149:145
        es_rs1              ,  //144:140
        es_alu_op           ,  //139:124
        es_load_op          ,  //123:123
        es_src1_is_sa       ,  //122:122
        es_src1_is_pc       ,  //121:121
        es_src2_is_imm      ,  //120:120
        es_src2_is_8        ,  //119:119
        es_gr_we            ,  //118:118
        es_mem_we           ,  //117:117
        es_dest             ,  //116:112
        es_imm              ,  //111:96
        es_rs_value         ,  //95 :64
        es_rt_value         ,  //63 :32
        es_pc                  //31 :0
       } = ds_to_es_bus_r;

wire [31:0] es_alu_src1   ;
wire [31:0] es_alu_src2   ;
wire [31:0] es_alu_final_src1   ;
wire [31:0] es_alu_final_src2   ;
wire [63:0] es_alu_result ;
wire [31:0] hi_res;
wire [31:0] lo_res;
wire [31:0] es_final_result;

assign es_final_result = es_res_is_hi ? hi_res
                       : es_res_is_lo ? lo_res
                       : es_res_from_cp0 ? es_cp0_rdata
                       : es_alu_result[31:0];


assign es_rd = es_dest;

assign es_res_from_mem = es_load_op;


assign es_ready_go    = !es_valid || !es_stall && (!data_sram_req || data_sram_addr_ok) ;
assign es_allowin     = !es_valid || es_ready_go && ms_allowin;
assign es_to_ms_valid =  es_valid && es_ready_go && !ms_flush;
always @(posedge clk) begin
    if (reset) begin
        es_valid <= 1'b0;
    end
    else if (es_allowin) begin
        es_valid <= ds_to_es_valid;
    end

    if (ds_to_es_valid && es_allowin) begin
        ds_to_es_bus_r <= ds_to_es_bus;
    end
end

assign es_alu_final_src1 = es_src1_is_sa  ? {27'b0, es_imm[10:6]} : 
                                         es_src1_is_pc  ? es_pc[31:0] :
                                         es_alu_src1;
assign es_alu_final_src2 = es_src2_is_imm ? {{16{es_imm[15]}}, es_imm[15:0]} : 
                                         es_src2_is_0imm ? {{16{1'b0}}, es_imm[15:0]} :
                                         es_src2_is_8   ? 32'd8 :
                                         es_alu_src2;

assign es_alu_src1[7:0] = alu1_fwd_wen[0] ? { {32{(alu_src1_fwd == 2'd0)}} & es_rs_value[7:0]
                                            | {32{(alu_src1_fwd == 2'd1)}} & ms_alu_result[7:0]
                                            | {32{(alu_src1_fwd == 2'd2)}} & ws_final_result[7:0]}
                                            : es_rs_value[7:0];
assign es_alu_src1[15:8] = alu1_fwd_wen[1] ? { {32{(alu_src1_fwd == 2'd0)}} & es_rs_value[15:8]
                                            |  {32{(alu_src1_fwd == 2'd1)}} & ms_alu_result[15:8]
                                            |  {32{(alu_src1_fwd == 2'd2)}} & ws_final_result[15:8]}
                                            : es_rs_value[15:8];
assign es_alu_src1[23:16] = alu1_fwd_wen[2] ? { {32{(alu_src1_fwd == 2'd0)}} & es_rs_value[23:16]
                                            |   {32{(alu_src1_fwd == 2'd1)}} & ms_alu_result[23:16]
                                            |   {32{(alu_src1_fwd == 2'd2)}} & ws_final_result[23:16]}
                                            : es_rs_value[23:16];
assign es_alu_src1[31:24] = alu1_fwd_wen[3] ? { {32{(alu_src1_fwd == 2'd0)}} & es_rs_value[31:24]
                                            |   {32{(alu_src1_fwd == 2'd1)}} & ms_alu_result[31:24]
                                            |   {32{(alu_src1_fwd == 2'd2)}} & ws_final_result[31:24]}
                                            : es_rs_value[31:24];
                                                                                                           
assign es_alu_src2[7:0] = alu2_fwd_wen[0] ? { {32{(alu_src2_fwd == 2'd0)}} & es_rt_value[7:0]
                                            | {32{(alu_src2_fwd == 2'd1)}} & ms_alu_result[7:0]
                                            | {32{(alu_src2_fwd == 2'd2)}} & ws_final_result[7:0]}
                                            : es_rt_value[7:0];
assign es_alu_src2[15:8] = alu2_fwd_wen[1] ? { {32{(alu_src2_fwd == 2'd0)}} & es_rt_value[15:8]
                                            |  {32{(alu_src2_fwd == 2'd1)}} & ms_alu_result[15:8]
                                            |  {32{(alu_src2_fwd == 2'd2)}} & ws_final_result[15:8]}
                                            : es_rt_value[15:8];
assign es_alu_src2[23:16] = alu2_fwd_wen[2] ? { {32{(alu_src2_fwd == 2'd0)}} & es_rt_value[23:16]
                                            |   {32{(alu_src2_fwd == 2'd1)}} & ms_alu_result[23:16]
                                            |   {32{(alu_src2_fwd == 2'd2)}} & ws_final_result[23:16]}
                                            : es_rt_value[23:16];
assign es_alu_src2[31:24] = alu2_fwd_wen[3] ? { {32{(alu_src2_fwd == 2'd0)}} & es_rt_value[31:24]
                                            |   {32{(alu_src2_fwd == 2'd1)}} & ms_alu_result[31:24]
                                            |   {32{(alu_src2_fwd == 2'd2)}} & ws_final_result[31:24]}
                                            : es_rt_value[31:24];    
                   
//assign es_alu_src2 = {32{(alu_src2_fwd == 2'd0)}} & es_rt_value
//                   | {32{(alu_src2_fwd == 2'd1)}} & ms_alu_result
//                   | {32{(alu_src2_fwd == 2'd2)}} & ws_final_result ;

wire [63:0] multi_result;
wire [63:0] divi_result;
wire m_axis_dout_tvalid;
reg [31:0] HI;
reg [31:0] LO;
wire op_mul;
wire op_mulu;
wire op_div;
wire op_divu;
assign op_mul  = es_alu_op[12];
assign op_mulu = es_alu_op[13];
assign op_div  = es_alu_op[14];
assign op_divu = es_alu_op[15];

assign hi_res = HI;
assign lo_res = LO;

always@(posedge clk)
begin
    if(m_axis_dout_tvalid)
        {LO,HI} = es_alu_result;
    else if(op_mul | op_mulu)
        {HI,LO} = es_alu_result;
    else if (es_hi_wen)
        HI = es_alu_src1;
    else if (es_lo_wen)
        LO = es_alu_src1;
end

assign sb_wen =  {4{es_alu_result[1:0] == 2'b00}} & 4'b0001
               | {4{es_alu_result[1:0] == 2'b01}} & 4'b0010
               | {4{es_alu_result[1:0] == 2'b10}} & 4'b0100
               | {4{es_alu_result[1:0] == 2'b11}} & 4'b1000;
assign sh_wen =  {4{es_alu_result[1:0] == 2'b00}} & 4'b0011 
               | {4{es_alu_result[1:0] == 2'b10}} & 4'b1100;               
assign swl_wen = {4{es_alu_result[1:0] == 2'b00}} & 4'b0001
               | {4{es_alu_result[1:0] == 2'b01}} & 4'b0011
               | {4{es_alu_result[1:0] == 2'b10}} & 4'b0111
               | {4{es_alu_result[1:0] == 2'b11}} & 4'b1111;               
assign swr_wen = {4{es_alu_result[1:0] == 2'b00}} & 4'b1111 
               | {4{es_alu_result[1:0] == 2'b01}} & 4'b1110 
               | {4{es_alu_result[1:0] == 2'b10}} & 4'b1100 
               | {4{es_alu_result[1:0] == 2'b11}} & 4'b1000;             
assign swl_wdata = {32{es_alu_result[1:0] == 2'b00}} & {24'b0,es_alu_src2[31:24]}
                 | {32{es_alu_result[1:0] == 2'b01}} & {16'b0,es_alu_src2[31:16]}
                 | {32{es_alu_result[1:0] == 2'b10}} & {8'b0,es_alu_src2[31:8]}
                 | {32{es_alu_result[1:0] == 2'b11}} & es_alu_src2;        
assign swr_wdata = {32{es_alu_result[1:0] == 2'b00}} & es_alu_src2
                 | {32{es_alu_result[1:0] == 2'b01}} & {es_alu_src2[23:0],8'b0}
                 | {32{es_alu_result[1:0] == 2'b10}} & {es_alu_src2[15:0],16'b0}
                 | {32{es_alu_result[1:0] == 2'b11}} & {es_alu_src2[7:0],24'b0};        

               
alu u_alu(
    .clk                (clk                ),
    .reset              (reset              ),
    .alu_op             (es_alu_op          ),
    .alu_src1           (es_alu_final_src1  ),
    .alu_src2           (es_alu_final_src2  ),
    .alu_result         (es_alu_result      ),
    .overflow_en        (es_overflow_en     ),
    .overflow           (es_overflow        ),
    .m_axis_dout_tvalid (m_axis_dout_tvalid ),
    .div_block          (div_block          )
    );

assign low_addr        = es_alu_result[1:0];

assign data_sram_req   = es_load_op | es_mem_we ;

assign data_sram_wstrb = {4{es_mem_we & es_valid & !es_ex}} & (    {4{es_s_is_sb}} & sb_wen
                                                                |  {4{es_s_is_sh}} & sh_wen
                                                                |  {4{es_s_is_sw}} & 4'b1111
                                                                |  {4{es_s_is_swl}}& swl_wen
                                                                |  {4{es_s_is_swr}}& swr_wen
                                                               );
assign data_sram_wr    = es_mem_we;
assign data_sram_addr  = es_alu_result[31:0];
assign data_sram_wdata =  {32{es_s_is_sb}} & {4{es_alu_src2[7:0]}} 
                        | {32{es_s_is_sh}} & {2{es_alu_src2[15:0]}}
                        | {32{es_s_is_sw}} & es_alu_src2 
                        | {32{es_s_is_swl}}& swl_wdata
                        | {32{es_s_is_swr}}& swr_wdata;

assign data_sram_size  = {2{es_alu_result[1:0] == 2'b00}} & {2{es_l_is_lw | es_s_is_sw}}& 2'b10
                        |{2{es_l_is_lh | es_s_is_sh | es_l_is_lhu}}& 2'b01
                        |{2{es_l_is_lb | es_s_is_sb | es_l_is_lbu}}& 2'b00
                        |{2{es_alu_result[1:0] == 2'b00}} & {2{es_l_is_lwl | es_s_is_swl}}& 2'b00
                        |{2{es_alu_result[1:0] == 2'b01}} & {2{es_l_is_lwl | es_s_is_swl}}& 2'b01
                        |{2{es_alu_result[1:0] == 2'b10}} & {2{es_l_is_lwl | es_s_is_swl}}& 2'b10
                        |{2{es_alu_result[1:0] == 2'b11}} & {2{es_l_is_lwl | es_s_is_swl}}& 2'b10
                        |{2{es_alu_result[1:0] == 2'b00}} & {2{es_l_is_lwr | es_s_is_swr}}& 2'b10
                        |{2{es_alu_result[1:0] == 2'b01}} & {2{es_l_is_lwr | es_s_is_swr}}& 2'b10
                        |{2{es_alu_result[1:0] == 2'b10}} & {2{es_l_is_lwr | es_s_is_swr}}& 2'b01
                        |{2{es_alu_result[1:0] == 2'b11}} & {2{es_l_is_lwr | es_s_is_swr}}& 2'b00;
                        
assign lwl_wen = {4{es_alu_result[1:0] == 2'b00}} & 4'b1000
               | {4{es_alu_result[1:0] == 2'b01}} & 4'b1100
               | {4{es_alu_result[1:0] == 2'b10}} & 4'b1110
               | {4{es_alu_result[1:0] == 2'b11}} & 4'b1111;
               
assign lwr_wen = {4{es_alu_result[1:0] == 2'b00}} & 4'b1111
               | {4{es_alu_result[1:0] == 2'b01}} & 4'b0111
               | {4{es_alu_result[1:0] == 2'b10}} & 4'b0011
               | {4{es_alu_result[1:0] == 2'b11}} & 4'b0001;

assign es_rf_wen = {4{es_gr_we && ! es_l_is_lwl && !es_l_is_lwr}} & 4'b1111
                 | {4{es_l_is_lwl}} & lwl_wen
                 | {4{es_l_is_lwr}} & lwr_wen;



assign es_to_ms_bus = {data_sram_req  ,
                       es_l_is_lwl    ,  //83
                       es_l_is_lwr    ,  //82
                       es_rf_wen      ,  //81:78
                       low_addr       ,  //77:76
                       es_l_is_lw     ,  //75
                       es_l_is_lb     ,  //74
                       es_l_is_lbu    ,  //73
                       es_l_is_lh     ,  //72
                       es_l_is_lhu    ,  //71
                       es_res_from_mem,  //70:70
                       es_gr_we       ,  //69:69
                       es_dest        ,  //68:64
                       es_final_result,  //63:32
                       es_pc             //31:0
                      };


//=============异常==============
wire lw_addr_wrong;
wire lh_addr_wrong;
wire sw_addr_wrong;
wire sh_addr_wrong;
assign lw_addr_wrong = (es_l_is_lw & (low_addr != 2'b00));
assign lh_addr_wrong = ((es_l_is_lh | es_l_is_lhu) & (low_addr[0] != 1'b00));
assign sw_addr_wrong = (es_s_is_sw & (low_addr != 2'b00));
assign sh_addr_wrong = (es_s_is_sh & (low_addr[0] != 1'b00));

assign es_ex = (ds_ex | lw_addr_wrong | lh_addr_wrong | sw_addr_wrong | sh_addr_wrong | es_overflow) && es_valid;
assign es_excode =  ds_ex ? ds_excode
                 :  (lw_addr_wrong | lh_addr_wrong) ? `EXC_ADEL
                 :  (sw_addr_wrong | sh_addr_wrong) ? `EXC_ADES
                 :  es_overflow ? `EXC_OV
                 :  5'h0;
assign es_cp0_wdata = es_alu_src2;
assign es_badvaddr = (ds_ex && es_excode == `EXC_ADEL) ? es_pc : es_alu_result;
//=============异常==============


endmodule
