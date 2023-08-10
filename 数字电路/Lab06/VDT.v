module VDT (input pclk,
            output  hs,vs,
            output reg hen,ven
            );
    parameter henMax=1039 ,venMax=665;
    
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