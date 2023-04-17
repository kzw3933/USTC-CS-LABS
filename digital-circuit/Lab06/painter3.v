`timescale 1ns / 1ps

module  disnum (
    input clk, 
    input [3:0] num,
    output reg [11:0] prgb,
    output hs,		//行同步
    output vs		//场同步
);
wire [3:0] hen,ven;

clk_wiz_0 clkmaker(.clk_in1(clk),.clk_out1(pclk));
vdt nvdt(.pclk(pclk),.hs(hs),.vs(vs),.hen(hen),.ven(ven));
always @(posedge pclk) begin
    case(num)
        4'b0000:begin
             if((hen==1&&ven==2)||((hen==1)&&(ven==3))||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==2)&&(ven==1))||((hen==2)&&(ven==5))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==3))||((hen==4)&&(ven==7))||(hen==5&&ven==2)||((hen==5)&&(ven==3))||((hen==5)&&(ven==4))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6))) 
                prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
            
        end
        4'b0001:begin
           if((hen==2&&ven==2)||((hen==3)&&(ven==1))||((hen==3)&&(ven==2))||((hen==3)&&(ven==3))||((hen==3)&&(ven==4))||((hen==3)&&(ven==5))||((hen==3)&&(ven==6))||((hen==3)&&(ven==7))||((hen==2)&&(ven==7))||((hen==4)&&(ven==7)))
                  prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b0010:begin
            if((hen==1&&ven==2)||((hen==1)&&(ven==7))||((hen==2)&&(ven==1))||((hen==2)&&(ven==6))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==5))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||((hen==4)&&(ven==7))||((hen==5)&&(ven==2))||((hen==5)&&(ven==3))||((hen==5)&&(ven==7)))
                prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000; 
        end
        4'b0011:begin
            if((hen==1&&ven==2)||((hen==1)&&(ven==6))||((hen==2)&&(ven==1))||((hen==2)&&(ven==7))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||((hen==4)&&(ven==7))||((hen==5)&&(ven==2))||((hen==5)&&(ven==3))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6)))
               prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b0100:begin
            if((hen==1&&ven==4)||((hen==1)&&(ven==5))||((hen==2)&&(ven==3))||((hen==2)&&(ven==5))||((hen==3)&&(ven==2))||((hen==3)&&(ven==5))||((hen==4)&&(ven==1))||((hen==4)&&(ven==2))||((hen==4)&&(ven==3))||((hen==4)&&(ven==4))||((hen==4)&&(ven==5))||((hen==4)&&(ven==6))||((hen==4)&&(ven==7))||((hen==5)&&(ven==5)))
                 prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;   
        end
        4'b0101:begin
           if((hen==1&&ven==1)||((hen==1)&&(ven==2))||((hen==1)&&(ven==3))||((hen==1)&&(ven==6))||((hen==2)&&(ven==1))||((hen==2)&&(ven==3))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==3))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==3))||((hen==4)&&(ven==7))||((hen==5)&&(ven==1))||((hen==5)&&(ven==4))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6))) 
                 prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000; 
        end
        4'b0110:begin
            if((hen==1&&ven==2)||((hen==1)&&(ven==3))||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==2)&&(ven==1))||((hen==2)&&(ven==4))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||((hen==4)&&(ven==7))||((hen==5)&&(ven==2))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6)))
                 prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b0111:begin
           if((hen==1&&ven==6)||((hen==1)&&(ven==7))||((hen==2)&&(ven==1))||((hen==2)&&(ven==5))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==4)&&(ven==1))||((hen==4)&&(ven==3))||((hen==5)&&(ven==1))||((hen==5)&&(ven==2)))  
               prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1000:begin
            if((hen==1&&ven==2)||((hen==1)&&(ven==3))||(hen==1&&ven==5)||((hen==1)&&(ven==6))||((hen==2)&&(ven==1))||((hen==2)&&(ven==4))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||((hen==4)&&(ven==7))||((hen==5)&&(ven==2))||((hen==5)&&(ven==3))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6)))  
                 prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1001:begin
            if((hen==1&&ven==2)||((hen==1)&&(ven==3))||((hen==1)&&(ven==6))||((hen==2)&&(ven==1))||((hen==2)&&(ven==4))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||((hen==4)&&(ven==7))||((hen==5)&&(ven==2))||((hen==5)&&(ven==3))||((hen==5)&&(ven==4))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6)))  
                   prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1010:begin
             if((hen==1&&ven==3)||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==1)&&(ven==7))||((hen==2)&&(ven==2))||((hen==2)&&(ven==5))||((hen==3)&&(ven==1))||((hen==3)&&(ven==5))||((hen==4)&&(ven==2))||((hen==4)&&(ven==5))||(hen==5&&ven==3)||((hen==5)&&(ven==4))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6))||((hen==5)&&(ven==7))) 
                  prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1011:begin
             if((hen==1&&ven==1)||((hen==1)&&(ven==2))||(hen==1&&ven==3)||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==1)&&(ven==7))||((hen==2)&&(ven==1))||((hen==2)&&(ven==4))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||((hen==4)&&(ven==7))||(hen==5&&ven==2)||((hen==5)&&(ven==3))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6))) 
                prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1100:begin
            if(((hen==1)&&(ven==2))||(hen==1&&ven==3)||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==2)&&(ven==1))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==7))||(hen==5&&ven==2)||((hen==5)&&(ven==6))) 
                 prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1101:begin
            if((hen==1&&ven==1)||((hen==1)&&(ven==2))||(hen==1&&ven==3)||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==1)&&(ven==7))||((hen==2)&&(ven==1))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==7))||(hen==5&&ven==2)||((hen==5)&&(ven==3))||((hen==5)&&(ven==4))||((hen==5)&&(ven==5))||((hen==5)&&(ven==6))) 
                 prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1110:begin
             if((hen==1&&ven==1)||((hen==1)&&(ven==2))||(hen==1&&ven==3)||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==1)&&(ven==7))||((hen==2)&&(ven==1))||((hen==2)&&(ven==4))||((hen==2)&&(ven==7))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==3)&&(ven==7))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||((hen==4)&&(ven==7))||(hen==5&&ven==1)||((hen==5)&&(ven==7))) 
                prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;
        end
        4'b1111:begin
           if((hen==1&&ven==1)||((hen==1)&&(ven==2))||(hen==1&&ven==3)||((hen==1)&&(ven==4))||((hen==1)&&(ven==5))||((hen==1)&&(ven==6))||((hen==1)&&(ven==7))||((hen==2)&&(ven==1))||((hen==2)&&(ven==4))||((hen==3)&&(ven==1))||((hen==3)&&(ven==4))||((hen==4)&&(ven==1))||((hen==4)&&(ven==4))||(hen==5&&ven==1)) 
              prgb <= 12'h000;
            else if((hen>0)&&(ven>0)) prgb <= 12'hFFF;
            else prgb <= 12'h000;  
        end
    endcase
end

endmodule







module vdt (input pclk,
            output wire hs,vs,
            output reg [3:0]  hen,ven
);
  parameter hmax=1040 ,vmax=666;  
  parameter UP_BOUND = 29;
	parameter DOWN_BOUND = 628;
	parameter LEFT_BOUND = 184;
	parameter RIGHT_BOUND = 983;
    parameter UP_POS = 325;
	parameter DOWN_POS = 331;
	parameter LEFT_POS = 582;
	parameter RIGHT_POS = 586;
  parameter HSW = 120,VSW=6;


  reg [11:0] hcnt=0;
  reg [11:0] vcnt=0;


  always@(posedge pclk)
  begin
      if(hcnt==hmax-1)
          begin
                hcnt=0;
                if(vcnt==vmax-1)vcnt<=0;
                else vcnt=vcnt+1;
          end
      else  hcnt=hcnt+1;

  end

  assign hs=(hcnt<HSW);
  assign vs=(vcnt<VSW);


  always@(posedge pclk)
  begin
        if((hcnt>=LEFT_BOUND)&&(hcnt<=RIGHT_BOUND))begin
            if((hcnt>=LEFT_POS)&&(hcnt<=RIGHT_POS))
                hen <= (hcnt -LEFT_POS+1);
            else hen <= 15;
        end
        else hen<= 0;
        if((vcnt>=UP_BOUND)&&(vcnt<=DOWN_BOUND))begin
            if((vcnt>=UP_POS)&&(vcnt<=DOWN_POS))
                ven <= (vcnt -UP_POS+1);
            else ven <= 15;
        end
        else ven<= 0;
  end

endmodule
















