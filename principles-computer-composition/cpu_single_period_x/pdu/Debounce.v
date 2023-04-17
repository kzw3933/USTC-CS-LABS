module Debounce #(parameter W = 1,CNT_VLU= 1000)(
                input                   clk,
                input                   rstn,
                input   [W-1 : 0]       in,
                output  [W-1 : 0]       out
                );
parameter  acc_stage = 1'b0 ,ass_stage = 1'b1;

wire   [19:0]   cnt;
wire            reset;
reg             CS;
reg             NS;

reg  [W-1:0]       in_r;
reg  [W-1:0]       out_r;

always@(posedge clk)begin
    if(!rstn)begin
        in_r  <= 0;
        out_r <= 0;
    end
    else begin
        in_r <= in;
        if(NS == ass_stage)begin
            out_r <= in;
        end
    end
    
end

assign  reset  =  (!rstn)|(in_r != in) | (cnt == CNT_VLU-1);

Counter Counter(.clk(clk),.rst(reset),.cnt(cnt));



always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        CS <= acc_stage;
    end
    else begin
        CS <= NS;
    end
end

always@(*)begin
    case(CS)
        acc_stage:begin
            NS = (cnt == CNT_VLU-1) ? ass_stage : acc_stage;
        end 
        ass_stage:begin
            NS = acc_stage;
        end
    endcase
end
 
assign  out  =  out_r;
                                     
endmodule