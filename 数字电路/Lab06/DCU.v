module DCU(input clk,
            output  [11:0] prgb,
            output hs,vs
            );
wire pclk;
wire hen,ven;
wire [5:0] raddr;
wire [1:0] rdata;

//获取50MHz的时钟
clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk)); 
blk_mem_gen_0 RAM(.clkb(pclk),.enb(1),.addrb(raddr),.doutb(rdata)); //仅使用读端口即B端口
VDT vdt(.pclk(pclk),.hs(hs),.vs(vs),.hen(hen),.ven(ven));
VDS vds(.pclk(pclk),.hen(hen),.ven(ven),.rdata(rdata),.raddr(raddr),.prgb(prgb));
endmodule

module VDS( input pclk,
            input hen,ven,
            input [1:0] rdata,
            output reg [5:0] raddr,
            output reg [11:0] prgb);
    parameter HSize = 100,VSize = 100;
    reg [6:0] hscnt,vscnt;
    reg hen_reg;
    wire nextline;

    assign nextline = hen_reg&&(!hen);
    always @(posedge pclk) begin
      hen_reg <= hen;
    end 

   always @(posedge pclk) begin
     if(hen)  hscnt <= (hscnt==(HSize-1))?0:(hscnt+1);
     else  hscnt <=0;  
    end

    always @(posedge pclk) begin
      if(ven)begin  
        if(nextline) vscnt <= (vscnt==(VSize-1))?0:(vscnt+1);
      end 
      else if(!ven) vscnt <= 0;
    end

    always @(posedge pclk) begin
        if(hen&&ven)begin
          if(hscnt==(HSize-1)) raddr <= raddr+1;
        end
        else  begin
          if(nextline)begin
            if(vscnt<(VSize-1)) raddr <= raddr -8;
          end 
          if((!hen)&&(!ven)) raddr <= 6'b000000;  
        end
        end
        
        always @(posedge pclk) begin
          if(hen&&ven)begin
            case(rdata)
            2'b00: prgb <= 12'hF00;
            2'b01: prgb <= 12'h0F0;
            2'b10: prgb <= 12'h00F;
            default: prgb <= 12'hFFF;
          endcase
          end
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

