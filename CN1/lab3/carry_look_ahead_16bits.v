`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/10/2020 07:35:19 PM
// Design Name: 
// Module Name: carry_look_ahead_16bits
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


module carry_look_ahead_16bits (
input [15:0] a, b,
input cin,
output [15:0] sum,
output cout
);
wire c1,c2,c3;
 
carry_look_ahead_4bits module1 (a[3:0], b[3:0], cin, sum[3:0], c1);
carry_look_ahead_4bits module2 (a[7:4], b[7:4], c1, sum[7:4], c2);
carry_look_ahead_4bits module3 (a[11:8], b[11:8], c2, sum[11:8], c3);
carry_look_ahead_4bits module4 (a[15:12], b[15:12], c3, sum[15:12], cout);
 
endmodule