//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Tue May 12 08:18:21 2020
//Host        : ghostmachine running 64-bit Linux Mint 19.3 Tricia
//Command     : generate_target cronometru_wrapper.bd
//Design      : cronometru_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module cronometru_wrapper
   (An_0,
    DP_0,
    Seg_0,
    clk_in_0,
    clk_out_led_0,
    pause_0,
    reset_0);
  output [7:0]An_0;
  output DP_0;
  output [6:0]Seg_0;
  input clk_in_0;
  output clk_out_led_0;
  input pause_0;
  input reset_0;

  wire [7:0]An_0;
  wire DP_0;
  wire [6:0]Seg_0;
  wire clk_in_0;
  wire clk_out_led_0;
  wire pause_0;
  wire reset_0;

  cronometru cronometru_i
       (.An_0(An_0),
        .DP_0(DP_0),
        .Seg_0(Seg_0),
        .clk_in_0(clk_in_0),
        .clk_out_led_0(clk_out_led_0),
        .pause_0(pause_0),
        .reset_0(reset_0));
endmodule
