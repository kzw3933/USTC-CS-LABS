module tb_debounce();
reg clk;
reg rstn;
reg x;
wire y;
initial begin
    clk = 0;forever begin
       #1 clk = ~clk; 
    end   
end
initial begin
    x = 0;forever begin 
      #5 x=~x;
      #3 x= ~x;
      #2 x= ~x;
      #1 x= ~x; 
            #5 x=~x;
      #3 x= ~x;
      #2 x= ~x;
      #1 x= ~x;
            #5 x=~x;
      #3 x= ~x;
      #2 x= ~x;
      #1 x= ~x;
            #5 x=~x;
      #3 x= ~x;
      #2 x= ~x;
      #1 x= ~x;
            #5 x=~x;
      #3 x= ~x;
      #2 x= ~x;
      #1 x= ~x;
      #5 x=~x;
      #3 x= ~x;
      #2 x= ~x;
      #1 x= ~x;
      #1000 x= ~x;
    end
    
end
initial begin
    rstn = 0;
    #8 rstn = ~rstn; 
    
    
end
debounce DB(clk,rstn,x,y);
endmodule