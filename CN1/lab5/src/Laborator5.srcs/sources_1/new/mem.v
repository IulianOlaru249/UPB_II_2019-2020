`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 18.05.2020 01:26:18
// Design Name: 
// Module Name: mem
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


module mem #(parameter biti = 32) (
	input clk, we_a, we_b,
	input [2:0] addr_a, addr_b,
	input [biti - 1: 0] d_in_a, d_in_b,
	output [biti - 1: 0] d_out_a, d_out_b
);
	
	reg [biti -1 : 0] ram_vec_a[7:0];
	reg [2 : 0] read_addr_a;
	reg [biti -1 : 0] ram_vec_b[7:0];
	reg [2 : 0] read_addr_b;
	always @(posedge clk)
	begin
		if (we_a)
			ram_vec_a[addr_a] <= d_in_a;
		read_addr_a <= addr_a;
	end
	assign d_out_a = ram_vec_a[read_addr_a];
	always @(posedge clk)
	begin
		if (we_b)
			ram_vec_b[addr_b] <= d_in_b;
		read_addr_b <= addr_b;
	end
	
	assign d_out_b = ram_vec_b[read_addr_b];
endmodule