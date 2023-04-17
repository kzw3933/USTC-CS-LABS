module CommonEncoder(                       //8421码的10-4编码器
    input wire[9:0] In,
    output reg [3:0] Out
    );
    always @(*)
        begin
        case(In)
            10'b0000_0000_01 : Out = 4'b0000;
            10'b0000_0000_10 : Out = 4'b0001;
            10'b0000_0001_00 : Out = 4'b0010;
            10'b0000_0010_00 : Out = 4'b0011;
            10'b0000_0100_00 : Out = 4'b0100;
            10'b0000_1000_00 : Out = 4'b0101;
            10'b0001_0000_00 : Out = 4'b0110;
            10'b0010_0000_00 : Out = 4'b0111;
            10'b0100_0000_00 : Out = 4'b1000;
            10'b1000_0000_00 : Out = 4'b1001;
            default : Out = 4'b1111;        //  无效输入
        endcase
        end   
endmodule

module PriorityEncoder(               //余3循环码的优先编码器
input wire [9:0] In,
output  reg [3:0] Out 
  );
    always @(*) begin
        if(In[9])   Out =4'b1001;
        else if(In[8]) Out =4'b1000;
        else if(In[7]) Out =4'b0111;
        else if(In[6]) Out =4'b0110;
        else if(In[5]) Out =4'b0101;
        else if(In[4]) Out =4'b0100;
        else if(In[3]) Out =4'b0011;
        else if(In[2]) Out =4'b0010;
        else if(In[1]) Out =4'b0001;
        else if(In[0]) Out =4'b0000; 
        else Out = 4'b1111;              //  无效输入
    end
endmodule