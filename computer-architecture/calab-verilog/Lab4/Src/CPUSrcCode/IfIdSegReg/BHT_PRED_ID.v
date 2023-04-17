module BHT_PRED_ID(
    input wire clk, bubbleD, flushD,
    input wire        bht_hit,
    input wire  [1:0] bht_pred_br,
    output reg        bht_hit_ID,
    output reg  [1:0] bht_pred_br_ID
    );
    initial begin
        bht_hit_ID = 0;
        bht_pred_br_ID = 0;
    end    
    always@(posedge clk)
        if (!bubbleD) 
        begin
            if (flushD)begin
                bht_hit_ID <= 0;
                bht_pred_br_ID <= 0;
            end
                

            else begin
                bht_hit_ID <= bht_hit ;
                bht_pred_br_ID <=bht_pred_br ;
            end
                
        end
    
endmodule