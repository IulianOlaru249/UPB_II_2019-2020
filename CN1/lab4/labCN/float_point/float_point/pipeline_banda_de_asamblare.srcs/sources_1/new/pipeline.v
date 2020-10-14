`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 07:58:17 PM
// Design Name: 
// Module Name: pipeline
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

module pipeline(
    input clear,
    input load,
    input clk,
    input [31:0] A,
    input [31:0] B,
    output [31:0] reset
    );
    
    //Segment 0 Banda de asamblare 
    wire [15:0] exponent_in0, exponent_out0;
    wire [47:0] mantisa_in0, mantisa_out0;
    
    parse p(A, B, exponent_in0, mantisa_in0);

    registre_exponent16 regE0(clear, load, clk, exponent_in0, exponent_out0);
    registre_mantise48 regM0(clear, load, clk, mantisa_in0, mantisa_out0);
    
    //  Segment 1 Banda de asamblare 
    wire [15:0] exponent_in1, exponent_out1;
    wire [56:0] mantisa_in1, mantisa_out1;
    wire [8:0] val1;
    
    compareExponent compare(exponent_out0, val1, exponent_in1);
    concat con(mantisa_out0, val1, mantisa_in1);

    registre_exponent16 regE1(clear, load, clk, exponent_in1, exponent_out1);
    registre_mantise57 regM1(clear, load, clk, mantisa_in1, mantisa_out1);
    
    //  Segment 2 Banda de asamblare 
    wire [7:0] exponent_in2, exponent_out2;
    wire [47:0] mantisa_in2, mantisa_out2;
    wire sign, op;
    
    correctExponent correct(exponent_out1, exponent_in2);
    alignMantise align(mantisa_out1, mantisa_in2, sign, op);

    registre_exponent8 regE2(clear, load, clk, exponent_in2, exponent_out2);
    registre_mantise48 regM2(clear, load, clk, mantisa_in2, mantisa_out2);
    
    //  Segment 3 Banda de asamblare 
    wire [7:0] exponent_out3;
    wire [25:0] mantisa_in3, mantisa_out3;
    
    addMantise add(mantisa_out2, op, sign, mantisa_in3);

    registre_exponent8 regE3(clear, load, clk, exponent_out2, exponent_out3);
    registre_mantise26 regM3(clear, load, clk, mantisa_in3, mantisa_out3);
    
    //  Segment 4 Banda de asamblare 
    wire [7:0] exponent_in4, exponent_out4;
    wire [23:0] mantisa_in4, mantisa_out4;
    wire [8:0] val2;
    
    normalize norm(mantisa_out3, val2, mantisa_in4);
    changeExponent change(exponent_out3, val1, val2, exponent_in4);

    registre_exponent8 regE4(clear, load, clk, exponent_in4, exponent_out4);
    registre_mantise24 regM4(clear, load, clk, mantisa_in4, mantisa_out4);
    
    assign reset = {mantisa_out4[23], exponent_out4, mantisa_out4[22:0]};
 
endmodule