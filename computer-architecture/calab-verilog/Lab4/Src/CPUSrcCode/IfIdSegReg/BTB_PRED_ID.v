module BTB_PRED_ID(
    input wire clk, bubbleD, flushD,
    input wire        btb_hit,
    input wire        btb_pred_br,
    output reg        btb_hit_ID,
    output reg        btb_pred_br_ID
    );
    initial begin
        btb_hit_ID =0;
        btb_pred_br_ID = 0;
    end    
    always@(posedge clk)
        if (!bubbleD) 
        begin
            if (flushD)begin
                btb_hit_ID <= 0;
                btb_pred_br_ID <= 0;
            end
                

            else begin
                btb_hit_ID <= btb_hit ;
                btb_pred_br_ID <=btb_pred_br ;
            end
                
        end
    
endmodule