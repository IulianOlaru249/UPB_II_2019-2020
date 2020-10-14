//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Tue May 12 08:18:20 2020
//Host        : ghostmachine running 64-bit Linux Mint 19.3 Tricia
//Command     : generate_target cronometru.bd
//Design      : cronometru
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "cronometru,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=cronometru,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=6,numReposBlks=6,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=6,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "cronometru.hwdef" *) 
module cronometru
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
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.RESET_0 RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.RESET_0, INSERT_VIP 0, POLARITY ACTIVE_LOW" *) input reset_0;

  wire [3:0]bin2bcd_0_bcd0;
  wire [3:0]bin2bcd_0_bcd1;
  wire [3:0]bin2bcd_1_bcd0;
  wire [3:0]bin2bcd_1_bcd1;
  wire clk_in_0_1;
  wire [7:0]display_7_seg_0_An;
  wire display_7_seg_0_DP;
  wire [6:0]display_7_seg_0_Seg;
  wire divizor_ceas_0_clk_out_led;
  wire divizor_ceas_0_clk_out_seg;
  wire numarator_0_59_0_carry_out;
  wire [5:0]numarator_0_59_0_up_counter;
  wire [5:0]numarator_0_59_1_up_counter;
  wire pause_0_1;
  wire reset_0_1;

  assign An_0[7:0] = display_7_seg_0_An;
  assign DP_0 = display_7_seg_0_DP;
  assign Seg_0[6:0] = display_7_seg_0_Seg;
  assign clk_in_0_1 = clk_in_0;
  assign clk_out_led_0 = divizor_ceas_0_clk_out_led;
  assign pause_0_1 = pause_0;
  assign reset_0_1 = reset_0;
  cronometru_bin2bcd_0_0 bin2bcd_0
       (.bcd0(bin2bcd_0_bcd0),
        .bcd1(bin2bcd_0_bcd1),
        .bin_val(numarator_0_59_1_up_counter));
  cronometru_bin2bcd_1_0 bin2bcd_1
       (.bcd0(bin2bcd_1_bcd0),
        .bcd1(bin2bcd_1_bcd1),
        .bin_val(numarator_0_59_0_up_counter));
  cronometru_display_7_seg_0_0 display_7_seg_0
       (.An(display_7_seg_0_An),
        .BCD0(bin2bcd_0_bcd0),
        .BCD1(bin2bcd_0_bcd1),
        .BCD2(bin2bcd_1_bcd0),
        .BCD3(bin2bcd_1_bcd1),
        .DP(display_7_seg_0_DP),
        .Seg(display_7_seg_0_Seg),
        .clk_out_seg(divizor_ceas_0_clk_out_seg),
        .reset(reset_0_1));
  cronometru_divizor_ceas_0_0 divizor_ceas_0
       (.clk_in(clk_in_0_1),
        .clk_out_led(divizor_ceas_0_clk_out_led),
        .clk_out_seg(divizor_ceas_0_clk_out_seg),
        .reset(reset_0_1));
  cronometru_numarator_0_59_0_1 numarator_0_59_0
       (.carry_out(numarator_0_59_0_carry_out),
        .clk(divizor_ceas_0_clk_out_led),
        .pause(pause_0_1),
        .reset(reset_0_1),
        .up_counter(numarator_0_59_0_up_counter));
  cronometru_numarator_0_59_1_0 numarator_0_59_1
       (.clk(numarator_0_59_0_carry_out),
        .pause(pause_0_1),
        .reset(reset_0_1),
        .up_counter(numarator_0_59_1_up_counter));
endmodule
