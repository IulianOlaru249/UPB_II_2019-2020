`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:33:41 PM
// Design Name: 
// Module Name: compareExponent
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

module compareExponent(
    input [15:0] exponent_in,
    output reg [8:0] val1,
    output reg [15:0] exponent_out
    );
    
    reg [7:0] exponent1, exponent2;
    
    always @(exponent_in)
    begin
        exponent1 = exponent_in[15:8];
        exponent2 = exponent_in[7:0];
        if (exponent1 > exponent2)
        begin
            val1[7:0] = exponent1 - exponent2;
            val1[8] = 1;
            exponent_out = {exponent1, exponent2};
        end    
        else
        begin
            val1[7:0] = exponent2 - exponent1;
            val1[8] = 0;
            exponent_out = {exponent2, exponent1};
        end  
    end
endmodule