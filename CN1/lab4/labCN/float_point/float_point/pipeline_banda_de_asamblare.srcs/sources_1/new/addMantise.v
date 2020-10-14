`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:16:18 PM
// Design Name: 
// Module Name: addMantise
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

module addMantise(
    input [47:0] mantisa_in,
    input op,
    input sign,
    output reg [25:0] mantisa_out
    );
    
    always @(mantisa_in)
    begin
        if (op == 1)
            mantisa_out[24:0] = mantisa_in[47:24] - mantisa_in[23:0];
        else
            mantisa_out[24:0] = mantisa_in[47:24] + mantisa_in[23:0];
    
        mantisa_out[25] = sign;
    end
endmodule
