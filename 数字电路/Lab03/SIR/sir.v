module SIR(input x,                              //两段式，状态用独热编码 
            input clk,rstn,
            output reg y,
            output reg [7:0] rx,ry);   
parameter S0 =11'b000_0000_0001,S1 =11'b0000_0000_0010,S2 = 11'b000_0000_0100,S3 = 11'b000_0000_1000,S4=11'b000_0001_0000,S5 = 11'b000_0010_0000,T1 =11'b000_0100_0000,T2 = 11'b000_1000_0000,T3 = 11'b001_0000_0000,T4 = 11'b010_0000_0000,T5 = 11'b100_0000_0000; 

reg [10:0] CS,NS;

always @(posedge clk,negedge rstn) begin   //异步重置
    if(~rstn) begin
      CS <= S0;          
    end 
    else begin
      CS <= NS;           
    end 
   
end

always @(posedge clk,negedge rstn ) begin
  if(~rstn) begin
    rx <= 0;
    ry <= 0;
  end
  else begin
    rx <=rx<<1;
    ry <=ry<<1;
    rx[0]<=x;
    ry[0]<=y;
  end  
end
/*
S0 : rstn后的初始状态
S1 : 连续检测到1个0
S2 : 连续检测到2个0
S3 : 连续检测到3个0
S4 : 连续检测到4个0
S5 : 连续检测到5个0
T1 : 连续检测到1个1
T2 : 连续检测到2个1
T3 : 连续检测到3个1
T4 : 连续检测到4个1
T5 : 连续检测到5个1
*/

always @(CS,x) begin
    case(CS)
        S0: begin
          if(x == 0) NS = S1;
          else  NS = T1;
          y = ry[0];
        end
        S1: begin
          if(x == 0) NS = S2;
          else  NS = T1;
          y = ry[0];
          end
        S2: begin
          if(x == 0) NS = S3;
          else  NS = T1;
          y = ry[0];
        end
        S3: begin
          if(x == 0) NS = S4;
          else  NS = T1;
          y = ry[0];
        end
        S4: begin
          if(x == 0) NS= S5;
          else  NS = T1;
          y = ry[0];
        end
        S5: begin
          if(x == 0) begin
            NS = S5;
            y = 0;
          end 
          else NS = T1;
         end
        T1: begin
          if(x == 0) NS = S1;
          else NS = T2;
          y = ry[0];
        end
        T2: begin
          if(x == 0) NS = S1;
          else  NS = T3;
          y = ry[0];
        end
        T3: begin
          if(x == 0) NS = S1;
          else  NS = T4;
          y = ry[0];
        end
        T4: begin
          if(x == 0) NS = S1;
          else  NS = T5;
          y = ry[0];
        end

        T5: begin
          if(x == 0)
            NS = S1;
          else begin
            NS = T5;
            y = 1;
          end 
      end
    endcase
end 
endmodule

module SIRx(input x,                              //两段式，状态用独热编码 
            input clk,rstn,
            output reg y,
            output reg [7:0] rx,ry);   
parameter S0 =4'b0000,S1 =4'b0001,S2 = 4'b0010,S3 = 4'b0011,S4=4'b0100,S5 = 4'b0101,T1 =4'b0110,T2 = 4'b0111,T3 = 4'b1000,T4 = 14'b1001,T5 = 4'b1010; 

reg [3:0] CS,NS;

always @(posedge clk,negedge rstn) begin   //异步重置
    if(~rstn) begin
      CS <= S0;          
    end 
    else begin
      CS <= NS;           
    end 
   
end

always @(posedge clk,negedge rstn ) begin
  if(~rstn) begin
    rx <= 0;
    ry <= 0;
  end
  else begin
    rx <=rx<<1;
    ry <=ry<<1;
    rx[0]<=x;
    ry[0]<=y;
  end  
end
/*
S0 : rstn后的初始状态
S1 : 连续检测到1个0
S2 : 连续检测到2个0
S3 : 连续检测到3个0
S4 : 连续检测到4个0
S5 : 连续检测到5个0
T1 : 连续检测到1个1
T2 : 连续检测到2个1
T3 : 连续检测到3个1
T4 : 连续检测到4个1
T5 : 连续检测到5个1
*/

always @(CS,x) begin
    case(CS)
        S0: begin
          if(x == 0) NS = S1;
          else  NS = T1;
          y = ry[0];
        end
        S1: begin
          if(x == 0) NS = S2;
          else  NS = T1;
          y = ry[0];
          end
        S2: begin
          if(x == 0) NS = S3;
          else  NS = T1;
          y = ry[0];
        end
        S3: begin
          if(x == 0) NS = S4;
          else  NS = T1;
          y = ry[0];
        end
        S4: begin
          if(x == 0) NS= S5;
          else  NS = T1;
          y = ry[0];
        end
        S5: begin
          if(x == 0) begin
            NS = S5;
            y = 0;
          end 
          else NS = T1;
         end
        T1: begin
          if(x == 0) NS = S1;
          else NS = T2;
          y = ry[0];
        end
        T2: begin
          if(x == 0) NS = S1;
          else  NS = T3;
          y = ry[0];
        end
        T3: begin
          if(x == 0) NS = S1;
          else  NS = T4;
          y = ry[0];
        end
        T4: begin
          if(x == 0) NS = S1;
          else  NS = T5;
          y = ry[0];
        end

        T5: begin
          if(x == 0)
            NS = S1;
          else begin
            NS = T5;
            y = 1;
          end 
      end
    endcase
end 
endmodule

