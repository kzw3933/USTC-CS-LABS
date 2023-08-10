`include "../../parameter.v"
module IF (
    input                               clk,
    input                               rstn,
    input                               flushF,
    input                               bubbleF,
    input   [`br_bus_w-1:0]             br_bus,
    output  [`if_to_id_bus_w-1:0]       if_to_id_bus
);

reg                                     bubbleF_r;
reg                                     flushF_r;

reg         [`br_bus_w-1:0]             br_bus_r;
wire                                    br_taken;
wire        [31:0]                      br_target;

wire        [31:0]                      pc_if;
reg         [31:0]                      pc_old;
reg         [31:0]                      pc_4;

wire        [31:0]                      inst;


assign    { br_taken,
            br_target } = br_bus_r;

always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        br_bus_r <= 0;
        bubbleF_r <= 0;
        flushF_r <= 0;
    end
    else begin
        br_bus_r <= br_bus;
        bubbleF_r <= bubbleF;
        flushF_r <= flushF;
    end       
end

always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        pc_old <= 32'h0;
        bubbleF_r <= 0;
        flushF_r <= 0;
        pc_4 <= 32'h0;
    end       
    else  begin
        pc_old <= pc_if;
        bubbleF_r <= bubbleF;
        flushF_r <= flushF;
        pc_4 <= pc_if + 4;
    end  
end


assign pc_if = bubbleF_r ? pc_old :
               flushF_r ? 0 : 
               br_taken ? br_target: pc_4;
   
InstructionMemory InstructionMemory(.a(pc_if[9:2]),.spo(inst));

assign if_to_id_bus = flushF ? 0:
                         { pc_if,inst};


endmodule