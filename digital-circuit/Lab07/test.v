module asciiArt #(parameter UP_BOUND = 29-4,DOWN_BOUND = 628-4,LEFT_BOUND = 184-4,RIGHT_BOUND = 983-4)(                           //??????????????????????
    input [10:0] x,
    input [9:0] y,
    input pclk,
    input dot,
    output reg [11:0] prgb,
    output [14:0] raddr
);

wire zone;
wire [1:0] choice;

reg [7:0] quot_x,quot_y;
reg [1:0] rem_x,rem_y;
wire [11:0] rgb;
assign zone = ((x >= LEFT_BOUND)&&(x<=RIGHT_BOUND)&&(y >= UP_BOUND)&&(y <= DOWN_BOUND));

always @(*) begin
    if(zone)begin
        quot_x = (x-LEFT_BOUND)/4;
        rem_x = (x-LEFT_BOUND)%4;
        quot_y =  (y - UP_BOUND)/4;
        rem_y = (y - UP_BOUND)%4;
    end
    else begin
        quot_x = 0;
        quot_y = 0;
        rem_x = 0;
        rem_y = 0;
    end 
end



assign raddr = (quot_y-1) * 200+quot_x; 

random RAND(.x(quot_x),.y(quot_y),.choice(choice));
        
ascii ASCII(.x(rem_x),.y(rem_y),.choice(choice),.prgb(rgb));


always @(*) begin
    if(zone)begin
        if(!dot) prgb = rgb;
        else prgb = 12'hFFF;
    end
    else prgb = 12'h000;
end




    
endmodule


module ascii (
    input [1:0] x,
    input [1:0] y,
    input [1:0] choice ,
    output reg [11:0] prgb
);



always @(*) begin
case(choice)
        2'b00:begin
                case(x)
                2'b00:begin
                    prgb = 12'hFFF;
                end
                2'b01:begin
                     if((y==2'b01)||(y==2'b10))
                        prgb = 12'hFFF;
                    else prgb = 12'h000;
                end
                2'b10:begin
                    if((y==2'b01)||(y==2'b10))
                        prgb = 12'hFFF;
                    else prgb = 12'h000;
                end
                2'b11:begin
                    prgb = 12'hFFF;
                end
                endcase
        end



        2'b01:begin
                case(x)
                2'b00:begin
                    if(y==2'b01)
                        prgb = 12'h000;
                    else prgb = 12'hFFF;
                       
                end
                2'b01:begin
                     if(y==2'b01)
                        prgb = 12'h000;
                    else prgb = 12'hFFF;
                end
                2'b10:begin
                     prgb = 12'h000;
                end
                2'b11:begin
                     if(y==2'b01)
                        prgb = 12'h000;
                    else prgb = 12'hFFF;
                end
                endcase
            
        end



        2'b10:begin
            case(x)
                2'b00:begin
                    if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                       
                end
                2'b01:begin
                    if((y==2'b10) || (y==2'b01)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                2'b10:begin
                      if((y==2'b10) || (y==2'b01)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                2'b11:begin
                   if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                endcase
            
        end
        2'b11:begin
               case(x)
                2'b00:begin
                    if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                       
                end
                2'b01:begin
                    prgb = 12'hFFF;
                end
                2'b10:begin
                     prgb = 12'hFFF;
                end
                2'b11:begin
                   if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                endcase
  
        end
    endcase


end
  
endmodule










module vdt (
    input pclk,
    output  hs,vs,
    output  [10:0] x,          
    output  [9:0] y              //x,y 200*150???????????????
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



module random (
    input [7:0] x,
    input [7:0] y,
    output [1:0] choice
);
reg [1:0] out;
assign choice = out;
always @(*) begin
    out = x*x+y*y;
end
    
endmodule

module top (
    input clk,
    output hs,vs,
    output [11:0] prgb
);
wire pclk;
wire dot;

wire [14:0] raddr;
wire [10:0] x;
wire [9:0] y;


clk_wiz_1 PCLK(.clk_in1(clk),.clk_out1(pclk));
blk_mem_gen_0 ROM(.addra(raddr),.clka(clk),.douta(dot),.ena(1));
vdt VDT(.pclk(pclk),.hs(hs),.vs(vs),.x(x),.y(y));
asciiArt ASCIIART(.x(x),.y(y),.dot(dot),.pclk(pclk),.prgb(prgb),.raddr(raddr));
    
endmodule



module top_tb (
);


reg clk;
wire hs,vs;
wire [11:0] prgb;


top TOP(.clk(clk),.hs(hs),.vs(vs),.prgb(prgb));

initial begin
    clk = 0;
    forever #1 clk = ~clk;
end
    
endmodule