`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:26:19 PM
// Design Name: 
// Module Name: alignMantise
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


module alignMantise(
    input [56:0] mantisa_in,
    output reg [47:0] mantisa_out,
    output reg sign,
    output reg op
    );
    
    reg sign1, sign2;
    reg [8:0] val1;
    reg [23:0] mantisa1, mantisa2;
    
    always @(*)
    begin
        val1 = mantisa_in[56:48];
        sign1 = mantisa_in[47];
        mantisa1 = {1'b1, mantisa_in[46:24]};
        sign2 = mantisa_in[23];
        mantisa2 = {1'b1, mantisa_in[22:0]};
        op = sign1 ^ sign2;
        
        if (val1[8] == 1)
        begin
            mantisa2 = mantisa2 >> val1[7:0];
            mantisa_out = {mantisa1, mantisa2};
            sign = sign1;
        end
        
        else
        begin
            mantisa1 = mantisa1 >> val1[7:0];
            mantisa_out = {mantisa2, mantisa1};
            sign = sign2;
        end
    end
endmodule
