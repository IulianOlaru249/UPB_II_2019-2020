`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:47:25 PM
// Design Name: 
// Module Name: registre_mantise24
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

module registre_mantise24(
    input clear,
    input load,
    input clk,
    input [23:0] mantisa_in,
    output reg [23:0] mantisa_out
    );
    
    always @(posedge clk)
    begin
        if (clear)
            mantisa_out <= 0;
        if (load)
            mantisa_out <= mantisa_in;
    end    
endmodule