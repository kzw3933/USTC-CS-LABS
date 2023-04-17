`define SaveDataFile "C:/Users/Administrator.Arknights-WNVMS/Desktop/CpuProjects/cpu_single_period_x/simulation/SaveDataFile.data"
`define END 1000000000


module cpu_tb();

    reg CPU_CLK;
    reg CPU_RSTN;
    wire           [31:0]  debug_pc;
    reg            [31:0]  debug_dm_rf_addr;
    wire           [31:0]  debug_rf_data;

    integer SaveDataFile;
    integer i;
always #1 CPU_CLK = ~CPU_CLK;
Cpu Cpu(
    .clk(CPU_CLK),
    .rstn(CPU_RSTN),
    .debug_pc(debug_pc),
    .debug_dm_rf_addr(debug_dm_rf_addr[7:0]),
    .debug_rf_data(debug_rf_data)
);

initial
begin
    $display("Initialing...");
    CPU_CLK = 1'b0;
    CPU_RSTN = 1'b1;
    debug_dm_rf_addr = 32'h6;
    #1
    $display("Open file to save data");
    SaveDataFile = $fopen(`SaveDataFile,"w");
    if(SaveDataFile == 0)
        $display("Fail to Open %s,Do not Save data to file",`SaveDataFile);
    else 
    begin
        $display("Start Intruction Execute...");
        CPU_RSTN = 0;
        #1 CPU_RSTN = 1;
        $fwrite(SaveDataFile,"i\tx6\tpc\n");
        for(i=0;i<`END;i=i+1)
            begin
                @(negedge CPU_CLK) begin
                    $fwrite(SaveDataFile,"%4d\t%4d\t%4d\n",i,debug_rf_data,debug_pc);
                end
            end
        $fclose(SaveDataFile);
    end
    $display("Simulation Ended!");
    $stop();
end


endmodule