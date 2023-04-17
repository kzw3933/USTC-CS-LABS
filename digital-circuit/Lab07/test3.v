module homepage #(parameter UP_BOUND = 29,DOWN_BOUND = 628,LEFT_BOUND = 184,RIGHT_BOUND = 983)(              //产生首页的画面信息，画面�??800*600
    input [10:0] x,
    input [9:0] y,
    output reg [11:0] prgb
);
wire zone;
wire zone0;
wire zone1;
wire zone2;
wire zone3;
wire zone4;

reg [9:0] Lf1 = LEFT_BOUND+300 ,Up1 = UP_BOUND + 100,Dn1 = UP_BOUND + 150,Rg1 = LEFT_BOUND+600;
reg [9:0] Lf2 = LEFT_BOUND+300 ,Up2 = UP_BOUND + 200,Dn2 = UP_BOUND + 250,Rg2 = LEFT_BOUND+600;
reg [9:0] Lf3 = LEFT_BOUND+300 ,Up3 = UP_BOUND + 300,Dn3 = UP_BOUND + 350,Rg3 = LEFT_BOUND+600;
reg [9:0] Lf4 = LEFT_BOUND+300 ,Up4 = UP_BOUND + 400,Dn4 = UP_BOUND + 450,Rg4 = LEFT_BOUND+600;

assign zone = ((x >= LEFT_BOUND)&&(x<=RIGHT_BOUND))&&((y >= UP_BOUND)&&(y <= DOWN_BOUND));
assign zone0 = ((x == LEFT_BOUND)&&(y <= DOWN_BOUND)&&(y >=  UP_BOUND))  || ((x == RIGHT_BOUND)&&(y <= DOWN_BOUND)&&(y >=  UP_BOUND)) || ((y ==UP_BOUND)&&(x >= LEFT_BOUND)&&(x<=RIGHT_BOUND)) || ((y == DOWN_BOUND)&&(x >= LEFT_BOUND)&&(x<=RIGHT_BOUND));
assign zone1 = ((x == Lf1)&&(y <= Dn1)&&(y >= Up1))  || ((x == Rg1)&&(y <= Dn1)&&(y >= Up1)) || ((y == Up1)&&(x >= Lf1)&&(x<=Rg1)) || ((y == Dn1)&&(x >= Lf1)&&(x<=Rg1));
assign zone2 = ((x == Lf2)&&(y <= Dn2)&&(y >= Up2))  || ((x == Rg2)&&(y <= Dn2)&&(y >= Up2)) || ((y == Up2)&&(x >= Lf2)&&(x<=Rg2)) || ((y == Dn2)&&(x >= Lf2)&&(x<=Rg2));
assign zone3 =((x == Lf3)&&(y <= Dn3)&&(y >= Up3))  || ((x == Rg3)&&(y <= Dn3)&&(y >= Up3)) || ((y == Up3)&&(x >= Lf3)&&(x<=Rg3)) || ((y == Dn3)&&(x >= Lf3)&&(x<=Rg3));
assign zone4 = ((x == Lf4)&&(y <= Dn4)&&(y >= Up4))  || ((x == Rg4)&&(y <= Dn4)&&(y >= Up4)) || ((y == Up4)&&(x >= Lf4)&&(x<=Rg4)) || ((y == Dn4)&&(x >= Lf4)&&(x<=Rg4));


always @(*) begin
    if(zone)begin
        if(zone0 || zone1 || zone2 || zone3 || zone4) 
                    prgb = 12'h000;
        else prgb = 12'hFFF;
    end 
    else prgb = 12'h000;
end

  
endmodule


module vdt (input pclk,
            output  hs,vs,
            output [10:0] x,
            output [9:0] y
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

    assign x = hcnt;
    assign y = vcnt;

    always @(posedge pclk) begin
        if(hcnt==henMax)begin
            hcnt <= 0;
            vcnt <=(vcnt==venMax)?0:(vcnt+1);
        end
        else hcnt <= hcnt +1;   
    end


    assign hs = (hcnt>(HSW-1))?0:1;
    assign vs = (vcnt>(VSW-1))?0:1;
endmodule

module top (
    input clk,
    output hs,vs,
    output [11:0] prgb
);

wire [10:0] x;
wire [9:0] y;

clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk));   //获取50MHz的时钟
vdt VDT(.pclk(pclk),.hs(hs),.vs(vs),.x(x),.y(y));
homepage hp(.x(x),.y(y),.prgb(prgb));
    
endmodule