module BTB_PRED_EX(
    input wire clk, bubbleE, flushE,
    input wire        btb_hit,
    input wire        btb_pred_br,
    output reg        btb_hit_EX,
    output reg        btb_pred_br_EX
    );
    initial begin
        btb_hit_EX =0;
        btb_pred_br_EX = 0;
    end    
    always@(posedge clk)
        if (!bubbleE) 
        begin
            if (flushE)begin
                btb_hit_EX <= 0;
                btb_pred_br_EX <= 0;
            end
                

            else begin
                btb_hit_EX <= btb_hit ;
                btb_pred_br_EX <=btb_pred_br ;
            end
                
        end
    
endmodule