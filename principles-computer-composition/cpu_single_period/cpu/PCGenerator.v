module PCGenerator(
    input               clk,
    input               rstn,
    input               br_taken,
    input       [31:0]  br_target,
    input       [31:0]  PC_4,
    output reg  [31:0]  NPC
    );

    always@(posedge clk or negedge rstn) begin
        if(!rstn)begin
            NPC <= -32'h4;
        end
        else begin
            NPC <= br_taken ? br_target :
                            PC_4;
        end
    end


endmodule