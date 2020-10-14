`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/12/2020 08:22:12 AM
// Design Name: 
// Module Name: Numarator_sim
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Numarator_sim();

reg clk, reset, pauza;
wire[5:0] valoare_bin;
wire carry_out;
Numarator nr(clk, reset, pauza, valoare_bin, carry_out);

initial
begin
    clk = 0;
    reset = 1;
    pauza = 0;
    #100 reset = 0;
end

    always
    #50 clk = !clk; 

endmodule
