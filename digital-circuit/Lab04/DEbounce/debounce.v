module debounce(input clk,rstn,                //其实可以例化一个计数器模块
                input x,
                output reg y);
parameter  Cnt0 = 3'b000, Cnt1 = 3'b001,Assign0 = 3'b010,Assign1=3'b011,Default = 3'b111;   //刚开始进入默认状态，两个计数状态，两个赋值状态    
reg [2:0] CS,NS;
reg [19:0] Count;
reg y_reg;
always @(posedge clk,negedge rstn) begin          //异步复位
    if(!rstn)begin
      CS <= Default;
      y_reg <= 0;
    end 
    else begin
      CS <= NS;
    y_reg <= y;
    end 
end

always @(posedge clk,negedge rstn) begin
  if(!rstn) Count <=0;
  else begin
    case(CS)
       Default: Count <= 0;
    Cnt0 :begin
      Count <= Count+1;
    end 
    Cnt1: Count <= Count+1;
    Assign0: Count <= 0;
    Assign1: Count <= 0;

  endcase
  end

  
end

always @(*) begin
    y = y_reg;
    NS = Default;
    case (CS) 
    Default: begin
      y = 0;
      if(x == 1) NS = Cnt1;
      else NS = Cnt0;
    end
    Cnt0:begin
       if(Count == 100000) begin
         NS = Assign0;
       end 
       else if(x == 0)begin
         NS = Cnt0;
       end    
       else begin
         NS = Default;
       end      
    end
    Cnt1:begin
       if(Count == 100000) begin
         NS = Assign1;
       end 
       else if(x == 1)begin
         NS = Cnt1;
       end    
       else begin
         NS = Default;
       end      
    end
    Assign0: begin
           y = 0;
      if(x == 0)
        NS = Assign0;                
      else 
         NS = Default;              //只有极个别才在此刻为1
    end
    Assign1:begin
      y = 1;
      if(x == 1) NS = Assign1;
      else NS = Default;                  //只有极个别才在此刻为1
    end
    endcase
end
endmodule

