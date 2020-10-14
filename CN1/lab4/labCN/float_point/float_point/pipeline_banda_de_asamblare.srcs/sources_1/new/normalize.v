`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:28:53 PM
// Design Name: 
// Module Name: normalize
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

module normalize(
    input [25:0] mantisa_in,
    output reg [8:0] val2,
    output reg [23:0] mantisa_out
    );
    
    integer i;
    reg [24:0] mantisa_cu_1;
    reg semn;
    
    always @(mantisa_in)
    begin
    
        val2 = 0;
        mantisa_cu_1 = mantisa_in[24:0];
        semn = mantisa_in[25];
        
        //mantisa nu e 0
        if (mantisa_in[24:0] != 0)
        begin
            if (mantisa_cu_1[24] == 1)
            begin
                mantisa_cu_1 = mantisa_cu_1 >> 1;
                val2 = 1;
                val2[8] = 1;
            end
            else
            begin
                for (i = 0; i < 23; i = i + 1)
                begin
                    if (mantisa_cu_1[23] == 0)
                    begin
                        mantisa_cu_1 = mantisa_cu_1 << 1;
                        val2 = val2 + 1;
                    end
                end
            end
        end
        
        mantisa_out[23] = semn;
        mantisa_out[22:0] = mantisa_cu_1[22:0];
    end
    
endmodule

