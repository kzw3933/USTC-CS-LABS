 module EtoD(      //普通编码器、普通译码器、七段译码器
    input wire[9:0] In,
    output wire [9:0] Out1,
    output wire [6:0] Out2 
        );
        wire [3:0] temp;
       CommonEncoder cer (In,temp);        
        CommonDecoder cdr (temp,Out1);
        Decoder7Seg d7s (temp,Out2);
 endmodule