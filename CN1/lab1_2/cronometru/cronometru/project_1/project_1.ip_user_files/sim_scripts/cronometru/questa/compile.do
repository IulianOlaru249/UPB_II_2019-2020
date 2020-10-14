vlib questa_lib/work
vlib questa_lib/msim

vlib questa_lib/msim/xil_defaultlib

vmap xil_defaultlib questa_lib/msim/xil_defaultlib

vlog -work xil_defaultlib -64 \
"../../../../project_1.srcs/sources_1/bd/cronometru/ip/cronometru_numarator_0_59_1_0/sim/cronometru_numarator_0_59_1_0.v" \
"../../../../project_1.srcs/sources_1/bd/cronometru/ip/cronometru_bin2bcd_0_0/sim/cronometru_bin2bcd_0_0.v" \
"../../../../project_1.srcs/sources_1/bd/cronometru/ip/cronometru_bin2bcd_1_0/sim/cronometru_bin2bcd_1_0.v" \
"../../../../project_1.srcs/sources_1/bd/cronometru/ip/cronometru_divizor_ceas_0_0/sim/cronometru_divizor_ceas_0_0.v" \
"../../../../project_1.srcs/sources_1/bd/cronometru/ip/cronometru_display_7_seg_0_0/sim/cronometru_display_7_seg_0_0.v" \
"../../../../project_1.srcs/sources_1/bd/cronometru/ip/cronometru_numarator_0_59_0_1/sim/cronometru_numarator_0_59_0_1.v" \
"../../../../project_1.srcs/sources_1/bd/cronometru/sim/cronometru.v" \


vlog -work xil_defaultlib \
"glbl.v"

