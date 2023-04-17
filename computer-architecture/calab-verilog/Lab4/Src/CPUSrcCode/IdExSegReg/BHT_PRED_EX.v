module BHT_PRED_EX(
    input wire clk, bubbleE, flushE,
    input wire              bht_hit,
    input wire   [1:0]      bht_pred_br,
    output reg              bht_hit_EX,
    output reg   [1:0]      bht_pred_br_EX
    );
    initial begin
        bht_hit_EX =0;
        bht_pred_br_EX = 0;
    end    
    always@(posedge clk)
        if (!bubbleE) 
        begin
            if (flushE)begin
                bht_hit_EX <= 0;
                bht_pred_br_EX <= 0;
            end
                

            else begin
                bht_hit_EX <= bht_hit ;
                bht_pred_br_EX <=bht_pred_br ;
            end
                
        end
    
endmodule
