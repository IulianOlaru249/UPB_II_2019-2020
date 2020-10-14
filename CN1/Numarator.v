`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: com.company
// Engineer: Costache
// 
// Create Date: 03/10/2020 08:40:51 AM
// Design Name: 
// Module Name: Numarator
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


module Numarator(clk, reset, pauza, valoare_bin, carry_out);
    input clk; input reset; input pauza; output[5:0] valoare_bin; output carry_out;

    reg[5:0] valoare_bin;
    reg[0:0] carry_out;
    always @(posedge clk)
        if(reset)
        begin
            valoare_bin = 0;
            carry_out = 0;
        end
        else
        begin
            if(valoare_bin < 59)
            begin
                valoare_bin = valoare_bin + 1;
            end
            else
            begin
                valoare_bin = 0;
                carry_out = 1;
            end
        end   
endmodule
