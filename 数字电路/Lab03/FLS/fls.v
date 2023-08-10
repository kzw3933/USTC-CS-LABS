module FLS(input clk,rstn,en,      
            input [7:0] d,
            output reg [7:0] f,
            output reg err);       

parameter Input1 = 2'b01,Input2 = 2'b10,Calcu = 2'b11; 
reg [1:0] CS,NS;
reg [7:0] f1,f2;

//FSM   两段式

always @(posedge clk,negedge rstn) begin
  if(~rstn) begin
    CS <= Input1;
  end
  else if(en) CS <= NS; 
end

always @(*) begin
  case(CS) 
  Input1 : NS = Input2;
  Input2 : NS = Calcu;
  Calcu  : NS = Calcu;
  endcase  
  end

//data path
always @(posedge clk,negedge rstn) begin
  if(~rstn) begin
    err <= 0;
    f <= 0;
    f1 <= 0;
    f2 <= 0;
  end 
  else if(en)
    case(CS) 
    Input1: f1 <= d;
    Input2: f2 <= d;
    Calcu : begin
      if(((f1+f2)<f1) || ((f1+f2)<f1))       //溢出
        begin
          f <= f2;
          err <= 1;
        end     
      else begin
        f <= f1 + f2;
        f1 <= f2;
        f2 <= f1 + f2;
      end
    end
    endcase
end

endmodule

module FLSx(input clk,rstn,en,      
            input [7:0] d,
            output reg [7:0] f,
            output reg err);       

parameter Input1 = 2'b01,Input2 = 2'b10,Calcu = 2'b11; 
parameter s0 = 2'b01,s1 = 2'b10,s2 = 2'b11;
reg [1:0] CS,NS;
reg [1:0] Signal;
reg [7:0] f1,f2;

//FSM   三段式 对FSM的状态寄存后输出

always @(posedge clk,negedge rstn) begin
  if(~rstn) begin
    CS <= s0;
  end
  else if(en) CS <= NS; 
end

always @(*) begin
  case(CS) 
  s0 : NS = s1;
  s1 : NS = s2;
  s2 : NS = s2;
  endcase  
  end

always @(posedge clk,negedge rstn) begin
  if(~rstn) begin
    Signal <= Input1; 
  end
  else if(en) begin
  case(NS) 
  s1: Signal <= Input2;
  s2: Signal <= Calcu;
  endcase
  end

end


//data path
always @(posedge clk,negedge rstn) begin
  if(~rstn) begin
    err <= 0;
    f <= 0;
    f1 <= 0;
    f2 <= 0;
  end 
  else if(en)
    case(Signal) 
    Input1: f1 <= d;
    Input2: f2 <= d;
    Calcu : begin
      if(((f1+f2)<f1) || ((f1+f2)<f1))       //溢出
        begin
          f <= f2;
          err <= 1;
        end     
      else begin
        f <= f1 + f2;
        f1 <= f2;
        f2 <= f1 + f2;
      end
    end
    endcase
end

endmodule