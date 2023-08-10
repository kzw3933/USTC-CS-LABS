module VgaDis(input clk,
            output hs,vs,
            output reg [11:0] prgb
            );
        wire pclk;
        wire hen,ven;
        //获取50MHz的时钟
        clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk));
        VDT vdt(.pclk(pclk),.hs(hs),.vs(vs),.hen(hen),.ven(ven));
        always @(posedge pclk) begin
            if(hen&&ven)
                prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end

endmodule

module VDT (input pclk,
            output  hs,vs,
            output reg hen,ven
            );
    parameter henMax=1039 ,venMax=665;
    //HBP =64  ,HFP = 56,VBP = 23  ,VFP= 37 ;
    parameter UP_BOUND = 29;
	parameter DOWN_BOUND = 628;
	parameter LEFT_BOUND = 184;
	parameter RIGHT_BOUND = 983;
    parameter HSW = 120,VSW=6;
    reg [10:0] hcnt=0;
    reg [9:0] vcnt=0;

    always @(posedge pclk) begin
        if(hcnt==henMax)begin
            hcnt <= 0;
            vcnt <=(vcnt==venMax)?0:(vcnt+1);
        end
        else hcnt <= hcnt +1;   
    end

    always @(posedge pclk) begin
        hen <= (hcnt>=LEFT_BOUND)&&(hcnt <=RIGHT_BOUND);
        ven <= (vcnt>=UP_BOUND)&&(vcnt <=DOWN_BOUND);
    end
    assign hs = (hcnt>(HSW-1))?0:1;
    assign vs = (vcnt>(VSW-1))?0:1;
endmodule
