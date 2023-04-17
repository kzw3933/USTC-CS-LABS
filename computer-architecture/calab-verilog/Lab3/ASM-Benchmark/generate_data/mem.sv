
module mem #(                   // 
    parameter  ADDR_LEN  = 11   // 
) (
    input  clk, rst,
    input  [ADDR_LEN-1:0] addr, // memory address
    output reg [31:0] rd_data,  // data read out
    input  wr_req,
    input  [31:0] wr_data       // data write in
);
localparam MEM_SIZE = 1<<ADDR_LEN;
reg [31:0] ram_cell [MEM_SIZE];

always @ (posedge clk or posedge rst)
    if(rst)
        rd_data <= 0;
    else
        rd_data <= ram_cell[addr];

always @ (posedge clk)
    if(wr_req) 
        ram_cell[addr] <= wr_data;

initial begin
    // dst matrix C
    ram_cell[       0] = 32'h0;  // 32'h54ca1110;
    ram_cell[       1] = 32'h0;  // 32'hd5d223aa;
    ram_cell[       2] = 32'h0;  // 32'h5510a9d1;
    ram_cell[       3] = 32'h0;  // 32'h7c3302aa;
    ram_cell[       4] = 32'h0;  // 32'h749f55ee;
    ram_cell[       5] = 32'h0;  // 32'hf723bb61;
    ram_cell[       6] = 32'h0;  // 32'h170376c7;
    ram_cell[       7] = 32'h0;  // 32'h487a5038;
    ram_cell[       8] = 32'h0;  // 32'h1e8c5c5a;
    ram_cell[       9] = 32'h0;  // 32'hd324c355;
    ram_cell[      10] = 32'h0;  // 32'h10e1e827;
    ram_cell[      11] = 32'h0;  // 32'h1f5db6e3;
    ram_cell[      12] = 32'h0;  // 32'h04fe53a6;
    ram_cell[      13] = 32'h0;  // 32'h264801e0;
    ram_cell[      14] = 32'h0;  // 32'h9d64f665;
    ram_cell[      15] = 32'h0;  // 32'h35c0ec15;
    ram_cell[      16] = 32'h0;  // 32'h207b2e3b;
    ram_cell[      17] = 32'h0;  // 32'h8bfa9c09;
    ram_cell[      18] = 32'h0;  // 32'hbfa1fff7;
    ram_cell[      19] = 32'h0;  // 32'h7d579bda;
    ram_cell[      20] = 32'h0;  // 32'h1b561164;
    ram_cell[      21] = 32'h0;  // 32'hba37cc12;
    ram_cell[      22] = 32'h0;  // 32'hd16454e4;
    ram_cell[      23] = 32'h0;  // 32'hbab6fd38;
    ram_cell[      24] = 32'h0;  // 32'hba660711;
    ram_cell[      25] = 32'h0;  // 32'hdc134d4b;
    ram_cell[      26] = 32'h0;  // 32'hca77d86b;
    ram_cell[      27] = 32'h0;  // 32'h0ff9704a;
    ram_cell[      28] = 32'h0;  // 32'hd156c1d8;
    ram_cell[      29] = 32'h0;  // 32'h89f57a96;
    ram_cell[      30] = 32'h0;  // 32'h641ffe25;
    ram_cell[      31] = 32'h0;  // 32'hc178e959;
    ram_cell[      32] = 32'h0;  // 32'h8556dbbe;
    ram_cell[      33] = 32'h0;  // 32'h303dfe17;
    ram_cell[      34] = 32'h0;  // 32'h748fe31a;
    ram_cell[      35] = 32'h0;  // 32'h5bde615e;
    ram_cell[      36] = 32'h0;  // 32'h43d217ad;
    ram_cell[      37] = 32'h0;  // 32'h592880fe;
    ram_cell[      38] = 32'h0;  // 32'h4d52e33d;
    ram_cell[      39] = 32'h0;  // 32'ha6cff8cc;
    ram_cell[      40] = 32'h0;  // 32'hfb18bff7;
    ram_cell[      41] = 32'h0;  // 32'h9523edc6;
    ram_cell[      42] = 32'h0;  // 32'h4ad55ef8;
    ram_cell[      43] = 32'h0;  // 32'h8a512cc0;
    ram_cell[      44] = 32'h0;  // 32'h899b1143;
    ram_cell[      45] = 32'h0;  // 32'h6cb27757;
    ram_cell[      46] = 32'h0;  // 32'hf546b6df;
    ram_cell[      47] = 32'h0;  // 32'h82da0f39;
    ram_cell[      48] = 32'h0;  // 32'h98c68349;
    ram_cell[      49] = 32'h0;  // 32'h8f05d285;
    ram_cell[      50] = 32'h0;  // 32'h70cf7b14;
    ram_cell[      51] = 32'h0;  // 32'h9ce3d421;
    ram_cell[      52] = 32'h0;  // 32'hd4a5361b;
    ram_cell[      53] = 32'h0;  // 32'hb338ab8a;
    ram_cell[      54] = 32'h0;  // 32'hb5d5ae5f;
    ram_cell[      55] = 32'h0;  // 32'h6200e4fa;
    ram_cell[      56] = 32'h0;  // 32'h580d0ca7;
    ram_cell[      57] = 32'h0;  // 32'hbf5105f8;
    ram_cell[      58] = 32'h0;  // 32'h7b1eec56;
    ram_cell[      59] = 32'h0;  // 32'h182fb5d1;
    ram_cell[      60] = 32'h0;  // 32'hdac37b72;
    ram_cell[      61] = 32'h0;  // 32'hbbc7c1a8;
    ram_cell[      62] = 32'h0;  // 32'h041cb1a7;
    ram_cell[      63] = 32'h0;  // 32'h19ae73b9;
    // src matrix A
    ram_cell[      64] = 32'h03dfe3e1;
    ram_cell[      65] = 32'hce23fb08;
    ram_cell[      66] = 32'h546fc4f6;
    ram_cell[      67] = 32'h3a7b8e31;
    ram_cell[      68] = 32'h2dc0ee2d;
    ram_cell[      69] = 32'hab97f8c4;
    ram_cell[      70] = 32'h434047e4;
    ram_cell[      71] = 32'hbce6d53b;
    ram_cell[      72] = 32'ha41690f5;
    ram_cell[      73] = 32'h82c8108e;
    ram_cell[      74] = 32'h7bb9aa32;
    ram_cell[      75] = 32'h5880e3b7;
    ram_cell[      76] = 32'h38bb7d64;
    ram_cell[      77] = 32'h1a621b21;
    ram_cell[      78] = 32'h960ee8eb;
    ram_cell[      79] = 32'h47ab56f0;
    ram_cell[      80] = 32'h8195709c;
    ram_cell[      81] = 32'ha1b769c2;
    ram_cell[      82] = 32'h97ce4498;
    ram_cell[      83] = 32'h582b1252;
    ram_cell[      84] = 32'hd97071cc;
    ram_cell[      85] = 32'h8f827e3d;
    ram_cell[      86] = 32'h4760939c;
    ram_cell[      87] = 32'h9b9c867d;
    ram_cell[      88] = 32'h50f5ef8c;
    ram_cell[      89] = 32'h2a96371d;
    ram_cell[      90] = 32'ha4401684;
    ram_cell[      91] = 32'h1f6f6ebe;
    ram_cell[      92] = 32'h121b5647;
    ram_cell[      93] = 32'he88402e2;
    ram_cell[      94] = 32'h010ab6b7;
    ram_cell[      95] = 32'h9ecfeb51;
    ram_cell[      96] = 32'ha5326c22;
    ram_cell[      97] = 32'h0b602d69;
    ram_cell[      98] = 32'h2769b97c;
    ram_cell[      99] = 32'h92b45166;
    ram_cell[     100] = 32'he94e430e;
    ram_cell[     101] = 32'h64e31666;
    ram_cell[     102] = 32'h9fe76dc5;
    ram_cell[     103] = 32'he4ad6445;
    ram_cell[     104] = 32'h79fde9f5;
    ram_cell[     105] = 32'h22f19eb4;
    ram_cell[     106] = 32'he7c43bb8;
    ram_cell[     107] = 32'h0f9e0119;
    ram_cell[     108] = 32'h3047fc3c;
    ram_cell[     109] = 32'h28cf1612;
    ram_cell[     110] = 32'h37eb009e;
    ram_cell[     111] = 32'h17f0b8c1;
    ram_cell[     112] = 32'h68d94ded;
    ram_cell[     113] = 32'ha928ab50;
    ram_cell[     114] = 32'he51c2f1a;
    ram_cell[     115] = 32'h98372f15;
    ram_cell[     116] = 32'h5bee509d;
    ram_cell[     117] = 32'hb26e338f;
    ram_cell[     118] = 32'h9892ad71;
    ram_cell[     119] = 32'hc668b770;
    ram_cell[     120] = 32'h7852c991;
    ram_cell[     121] = 32'h1e3fcd92;
    ram_cell[     122] = 32'h628d8348;
    ram_cell[     123] = 32'h684cb4e1;
    ram_cell[     124] = 32'hb525e462;
    ram_cell[     125] = 32'h50566615;
    ram_cell[     126] = 32'hf4a93328;
    ram_cell[     127] = 32'h8d56b579;
    // src matrix B
    ram_cell[     128] = 32'hadcb917b;
    ram_cell[     129] = 32'h12157e5c;
    ram_cell[     130] = 32'h399d1189;
    ram_cell[     131] = 32'h5bd4d1c0;
    ram_cell[     132] = 32'h21d57a5e;
    ram_cell[     133] = 32'h0b9b5dac;
    ram_cell[     134] = 32'h4ce3cebb;
    ram_cell[     135] = 32'h2338afd0;
    ram_cell[     136] = 32'ha8e47065;
    ram_cell[     137] = 32'he77b2482;
    ram_cell[     138] = 32'h02b24f41;
    ram_cell[     139] = 32'h92320d78;
    ram_cell[     140] = 32'h4f5d8655;
    ram_cell[     141] = 32'hc0695dec;
    ram_cell[     142] = 32'hf64b2ed1;
    ram_cell[     143] = 32'he3cc1c0e;
    ram_cell[     144] = 32'h4ef15726;
    ram_cell[     145] = 32'h0e893bfd;
    ram_cell[     146] = 32'h826f4644;
    ram_cell[     147] = 32'hd7caa0e0;
    ram_cell[     148] = 32'hfe138fdb;
    ram_cell[     149] = 32'h2759e85f;
    ram_cell[     150] = 32'h7dc91be6;
    ram_cell[     151] = 32'hdfd1ea4c;
    ram_cell[     152] = 32'h6a04b207;
    ram_cell[     153] = 32'hef97e8f3;
    ram_cell[     154] = 32'h475a56e6;
    ram_cell[     155] = 32'hab7e5ac9;
    ram_cell[     156] = 32'h4d31b0b2;
    ram_cell[     157] = 32'h9a0cb2c0;
    ram_cell[     158] = 32'h2ff9ab80;
    ram_cell[     159] = 32'h6e275a83;
    ram_cell[     160] = 32'h9a94e225;
    ram_cell[     161] = 32'h2754ff8c;
    ram_cell[     162] = 32'hffa028d1;
    ram_cell[     163] = 32'h69d3710a;
    ram_cell[     164] = 32'h976013a9;
    ram_cell[     165] = 32'h0b1255ae;
    ram_cell[     166] = 32'h46c688da;
    ram_cell[     167] = 32'h5a8b6c8f;
    ram_cell[     168] = 32'he9f57254;
    ram_cell[     169] = 32'h03179964;
    ram_cell[     170] = 32'h1907ad36;
    ram_cell[     171] = 32'h24183e87;
    ram_cell[     172] = 32'he87166eb;
    ram_cell[     173] = 32'h57d80c87;
    ram_cell[     174] = 32'hc7f3627c;
    ram_cell[     175] = 32'hf0b78b64;
    ram_cell[     176] = 32'h2685b1ec;
    ram_cell[     177] = 32'h9e4bc0dc;
    ram_cell[     178] = 32'h3f7322d4;
    ram_cell[     179] = 32'h1dc26c5d;
    ram_cell[     180] = 32'h2fa714a9;
    ram_cell[     181] = 32'h303129a9;
    ram_cell[     182] = 32'hd9a2a9d6;
    ram_cell[     183] = 32'hefb0b1d0;
    ram_cell[     184] = 32'he8672dbf;
    ram_cell[     185] = 32'hf5f579b5;
    ram_cell[     186] = 32'h55c060e7;
    ram_cell[     187] = 32'h2d6ef831;
    ram_cell[     188] = 32'h1a4a05a7;
    ram_cell[     189] = 32'h48859057;
    ram_cell[     190] = 32'h0a2e3d74;
    ram_cell[     191] = 32'h58fcf6de;
end

endmodule

