module homepage #(parameter UP_BOUND = 29,DOWN_BOUND = 628,LEFT_BOUND = 184,RIGHT_BOUND = 983)(              //??????????????????????????800*600
    input up,input down,
    input clk,
    input [10:0] x, 
    input arrowEn,               
    input [9:0] y,
    input [11:0] rgb,
    output reg [11:0] prgb,
    output [14:0] addr
);
wire zone;
wire zone_array;

reg [10:0] arrow_x = 300;
reg [9:0]  arrow_y = 150;

reg [7:0] quot_x;

reg [7:0] quot_y;

parameter X = 600,Y1 = 150,Y2 = 340,Y3 = 470;

always @(*) begin
    if(zone)begin
        quot_x = (x-LEFT_BOUND)/4;
        quot_y =  (y - UP_BOUND)/4;
    end
    else begin
        quot_x = 0;
        quot_y = 0;
    end 
end

always @(posedge clk) begin
    if(up)begin
        if(arrow_y == Y1) arrow_y <= Y3;
        else if(arrow_y == Y2) arrow_y <= Y1;
        else if(arrow_y == Y3) arrow_y <= Y2;
    end

    else if(down)begin
        if(arrow_y == Y1) arrow_y <= Y2;
        else if(arrow_y == Y2) arrow_y <= Y3;
        else if(arrow_y == Y3) arrow_y <= Y1;
    end    
end
 assign addr = (quot_y-1) * 200+quot_x; 

assign zone = ((x >= LEFT_BOUND)&&(x<=RIGHT_BOUND))&&((y >= UP_BOUND)&&(y <= DOWN_BOUND));

assign zone_array = ((x>=arrow_x)&&(x<=(arrow_x+16))&&(y==arrow_y)) || ((y==(arrow_y-1))&&(x==(arrow_x+1))) ||((y==(arrow_y-2))&&(x==(arrow_x+2))) ||((y==(arrow_y-3))&&(x==(arrow_x+3))) || ((y==(arrow_y-4))&&(x==(arrow_x+4))) ||((y==(arrow_y+1))&&(x==(arrow_x+1))) ||((y==(arrow_y+2))&&(x==(arrow_x+2))) ||((y==(arrow_y+3))&&(x==(arrow_x+3))) ||((y==(arrow_y+4))&&(x==(arrow_x+4)));

always @(*) begin
    if(arrowEn)begin
        if(zone)begin
            if(zone_array)    prgb = 12'h000;
            else prgb = rgb;
        end
        else prgb = 12'h000;
    end
    else begin
        if(zone)begin
            prgb = rgb;
        end
        else prgb = 12'h000;
    end

end

  
endmodule


module vdt_hp (input pclk,
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
    input up,
    input down,
    output hs,vs,
    output [11:0] prgb
);

wire [10:0] x;
wire [9:0] y;
wire [14:0] addr;
wire pclk;
wire [11:0] rgb;

clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk));   
vdt_hp VDT(.pclk(pclk),.hs(hs),.vs(vs),.x(x),.y(y));
blk_mem_gen_0 ROM(.clka(clk),.addra(addr),.douta(rgb),.ena(1));
homepage hp(.x(x),.clk(clk),.up(up),.down(down),.rgb(rgb),.y(y),.prgb(prgb),.addr(addr));
    
endmodule









module CPU (
    input clk,//rstn
    input enter,
    input up,
    input down,
    input back,
    // input txd,                   //???????????
    // input ps2_clk,ps2_data,          //PS2????????????
    // output ja,jc,                  //???   
    output reg [11:0] prgb,            //?????????
    output reg hs,vs                  //???????????????????

);

//?????
wire pclk;
clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk));  


//??????????
// wire hs2,vs2;
reg [11:0] prgb2 = 12'hFFF;
// video VDT(.clk(pclk),.rstn(rstn),.txd(txd),.hs(hs2),.vs(vs2),.prgb(prgb2)) ;

//??????????
reg en1;
// auto_Buzzer BUZZER(.clk(pclk),.rstn(rstn),.en(en1),.sw(sw),.ja(ja),.jc(jc));


//??????????
reg en2;
wire [1:0] sw;
// paino  PIANO(.clk(clk),.rstn(rstn),.en(en2),.ps2_clk(ps2_clk),.ps2_data(ps2_data),.ja(ja));



//???????????
wire hs1,vs1;
wire [10:0] x;
wire [9:0] y;

wire Up,Down,Enter,Back;
wire [14:0] addr;
wire [11:0] rgb1,rgb2,rgb3;
reg [11:0] rgb;
reg arrowEn ;


assign Up = up;

assign Down = down;

assign Enter = enter;

assign Back = back;


vdt_hp VDTHP(.pclk(pclk),.hs(hs1),.vs(vs1),.x(x),.y(y));
blk_mem_gen_1 ROM1(.clka(clk),.addra(addr),.douta(rgb1),.ena(1));
blk_mem_gen_2 ROM2(.clka(clk),.addra(addr),.douta(rgb2),.ena(1));
blk_mem_gen_3 ROM3(.clka(clk),.addra(addr),.douta(rgb3),.ena(1));
homepage HOMEPAGE(.up(UP),.down(DOWN),.clk(clk),.x(x),.y(y),.rgb(rgb),.prgb(prgb1),.addr(addr),.arrowEn(arrowEn));        


parameter X = 600,Y1 = 150,Y2 = 340,Y3 = 470;
reg [1:0] choice1 = 0;
reg [1:0] choice2 = 0;
assign sw = choice2;


reg hs2 = 0;
reg vs2 = 0;



always @(posedge clk) begin
    if(Back) choice1 <= 2'b00;
    else begin
        case(choice1) 
        2'b00:begin
            if((x==X)&&Enter)begin
                if(y == Y1)
                    choice1 <= 2'b01;
                else if(y == Y2)
                    choice1 <= 2'b10;
                else if(y == Y3)
                    choice1 <= 2'b11;

            end
        end 
        2'b01:begin
              if((x==X)&&Enter)begin
                if(y == Y1)
                    choice2 <= 2'b01;
                else if(y == Y2)
                    choice2 <= 2'b10;
                else if(y == Y3)
                    choice2 <= 2'b11;

            end

        end
        endcase
    end
end

always @(*) begin
    case(choice1)
        2'b00: begin
            rgb = rgb1;
            prgb = prgb1;
            arrowEn = 1;
            hs = hs1;
            vs = vs1;
        end
         2'b01: begin
            rgb = rgb2;
            prgb = prgb1;
            hs = hs1;
            vs = vs1;
            arrowEn = 1;
        end
         2'b10: begin
            rgb = rgb3;
            prgb = prgb1;
            hs = hs1;
            vs = vs1;
            arrowEn = 0;
        end
         2'b11: begin
            rgb = 12'h000;
            prgb = prgb2;
            hs = hs2;
            vs = vs2;
            arrowEn = 0;
        end
    endcase
end


always @(*) begin
    case(choice1)
        2'b00:begin
            en1 = 0;
            en2 = 0;
        end
        2'b01:begin
            en1 = 1;
            en2 = 0;
        end
        2'b10:begin
            
            en1 = 0;
            en2 = 1;
        end
        2'b11:begin
            en1 = 0;
            en2 = 0;
        end
    endcase
end
endmodule


module cpu_tb();


reg clk;

wire hs,vs;


wire [11:0] prgb;


CPU cpu(.clk(clk),.enter(0),.up(0),.down(0),.back(0),.prgb(prgb),.hs(hs),.vs(vs));

initial begin
    clk = 1;
    forever #1 clk = ~clk;
end

endmodule