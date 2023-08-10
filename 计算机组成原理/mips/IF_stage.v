`include "mycpu.h"

module if_stage(
    input                          clk            ,
    input                          reset          ,
    //from ds
    input                          ds_allowin     ,
    input  [`BR_BUS_WD       -1:0] br_bus         ,
    //to ds
    output                         fs_to_ds_valid ,
    output [`FS_TO_DS_BUS_WD -1:0] fs_to_ds_bus   ,
    //from hazard
    input                          ds_flush       ,
    input                          fs_stall       ,
    //eret
    input                          ex_taken         ,
    input                          eret_taken       ,
    input    [31:0]                epc              ,
    //inst_cache interface
    output                         inst_sram_req    ,
    output                         inst_sram_wr     ,
    output   [1:0]                 inst_sram_size   ,
    output   [3:0]                 inst_sram_wstrb  ,
    output   [31:0]                inst_sram_addr   ,
    output   [31:0]                inst_sram_wdata  ,
    input                          inst_sram_addr_ok,
    input                          inst_sram_data_ok,
    input    [31:0]                inst_sram_rdata  
    
);

reg         fs_valid;
wire        fs_ready_go;
wire        fs_allowin;
wire        to_fs_valid;

wire        pfs_ready_go;

wire [31:0] seq_pc;
wire [31:0] nextpc;

wire         b_or_j;
wire         br_taken;
wire [ 31:0] br_target;

reg  [`BR_BUS_WD       -1:0] br_bus_old;
reg                          br_bus_stall;

assign      {b_or_j,
            br_taken,
            br_target} = br_bus_stall ?br_bus_old : br_bus;

wire         fs_ex;
wire [4:0]   fs_excode;

wire [31:0] fs_inst;
reg  [31:0] fs_pc;

reg  [31:0] fs_inst_old;
reg         fs_inst_old_stall;
reg         fs_inst_old_flush;



//=============异常=============
assign fs_ex = (fs_pc[1:0] != 2'b00);
assign fs_excode = fs_ex ? 5'h04 : 0;
//=============异常=============



assign fs_to_ds_bus = { fs_ex   ,
                        fs_excode,
                        b_or_j   ,
                        fs_inst ,
                        fs_pc   };

// pre-IF stage

assign pfs_ready_go =  inst_sram_addr_ok;

assign to_fs_valid  = ~reset && pfs_ready_go && !ds_flush;


assign seq_pc       = fs_pc + 3'h4;
assign nextpc       = ex_taken ? 32'hbfc00380:
                      eret_taken ? epc:
                      (b_or_j&&!fs_valid) ? seq_pc:
                      br_taken ? br_target :
                      seq_pc; 

   

// IF stage
assign fs_ready_go    = !fs_valid || (!fs_stall&&inst_sram_data_ok);
assign fs_allowin     = !fs_valid || fs_ready_go && ds_allowin;
assign fs_to_ds_valid =  fs_valid && fs_ready_go && !ds_flush && !fs_inst_old_flush;

always @(posedge clk) begin
    if (reset) begin
        fs_valid <= 1'b0;
    end
    else if (fs_allowin) begin
        fs_valid <= to_fs_valid;
    end

    if (reset) begin
        fs_pc <= 32'hbfbffffc;  
    end
    else if (to_fs_valid && fs_allowin) begin
        fs_pc <= nextpc;
    end
end


// inst值到达，但无法使用,保存使用
always@(posedge clk)begin
    if(reset)begin
        fs_inst_old <= 32'h0;
        fs_inst_old_stall <= 1'b0;
    end
    else if(fs_ready_go)begin
        fs_inst_old <= fs_inst;
        if(ds_allowin)begin
            fs_inst_old_stall <= 1'b0;
        end
        else begin
            fs_inst_old_stall <= 1'b1;
        end
    end
end

// inst值未到达，但要flush,取消使用
always@(posedge clk)begin
    if(reset)begin
        fs_inst_old_flush  <= 1'b0;
    end
    else begin
        if(((to_fs_valid||!fs_ready_go&&!fs_allowin)&&ds_flush))begin   //to check out 
            fs_inst_old_flush <= 1'b1;
        end
        else if(inst_sram_data_ok)begin
            fs_inst_old_flush <= 1'b0;
        end
    end
end

// 延迟槽指令未到达,但要执行跳转
always@(posedge clk)begin
    if(reset)begin
        br_bus_old <= 0;
        br_bus_stall <= 0;
    end
    if(pfs_ready_go && fs_allowin)begin
        br_bus_stall <= 1'b0;
    end
    else if(b_or_j && ds_allowin && fs_to_ds_valid && !pfs_ready_go)begin
        br_bus_old <= br_bus;
        br_bus_stall <= 1'b1;
    end
end


assign inst_sram_req   = fs_allowin && !fs_stall;
assign inst_sram_wr    = 1'b0;
assign inst_sram_wstrb = 4'h0;
assign inst_sram_addr  = nextpc;
assign inst_sram_wdata = 32'h0;
assign inst_sram_size  = 2'h2;

assign fs_inst         = fs_inst_old_stall ? fs_inst_old : fs_inst;

endmodule
