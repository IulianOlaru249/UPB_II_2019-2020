
################################################################
# This is a generated script based on design: cronometru
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2019.2
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_msg_id "BD_TCL-109" "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source cronometru_script.tcl


# The design that will be created by this Tcl script contains the following 
# module references:
# bin2bcd, bin2bcd, display_7_seg, divizor_ceas, numarator_0_59, numarator_0_59

# Please add the sources of those modules before sourcing this Tcl script.

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xc7a100tcsg324-1
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name cronometru

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_msg_id "BD_TCL-001" "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_msg_id "BD_TCL-002" "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_msg_id "BD_TCL-003" "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_msg_id "BD_TCL-004" "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_msg_id "BD_TCL-005" "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_msg_id "BD_TCL-114" "ERROR" $errMsg}
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports

  # Create ports
  set An_0 [ create_bd_port -dir O -from 7 -to 0 An_0 ]
  set DP_0 [ create_bd_port -dir O DP_0 ]
  set Seg_0 [ create_bd_port -dir O -from 6 -to 0 Seg_0 ]
  set clk_in_0 [ create_bd_port -dir I clk_in_0 ]
  set clk_out_led_0 [ create_bd_port -dir O clk_out_led_0 ]
  set pause_0 [ create_bd_port -dir I pause_0 ]
  set reset_0 [ create_bd_port -dir I -type rst reset_0 ]

  # Create instance: bin2bcd_0, and set properties
  set block_name bin2bcd
  set block_cell_name bin2bcd_0
  if { [catch {set bin2bcd_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $bin2bcd_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: bin2bcd_1, and set properties
  set block_name bin2bcd
  set block_cell_name bin2bcd_1
  if { [catch {set bin2bcd_1 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $bin2bcd_1 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: display_7_seg_0, and set properties
  set block_name display_7_seg
  set block_cell_name display_7_seg_0
  if { [catch {set display_7_seg_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $display_7_seg_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: divizor_ceas_0, and set properties
  set block_name divizor_ceas
  set block_cell_name divizor_ceas_0
  if { [catch {set divizor_ceas_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $divizor_ceas_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: numarator_0_59_0, and set properties
  set block_name numarator_0_59
  set block_cell_name numarator_0_59_0
  if { [catch {set numarator_0_59_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $numarator_0_59_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: numarator_0_59_1, and set properties
  set block_name numarator_0_59
  set block_cell_name numarator_0_59_1
  if { [catch {set numarator_0_59_1 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $numarator_0_59_1 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create port connections
  connect_bd_net -net bin2bcd_0_bcd0 [get_bd_pins bin2bcd_0/bcd0] [get_bd_pins display_7_seg_0/BCD0]
  connect_bd_net -net bin2bcd_0_bcd1 [get_bd_pins bin2bcd_0/bcd1] [get_bd_pins display_7_seg_0/BCD1]
  connect_bd_net -net bin2bcd_1_bcd0 [get_bd_pins bin2bcd_1/bcd0] [get_bd_pins display_7_seg_0/BCD2]
  connect_bd_net -net bin2bcd_1_bcd1 [get_bd_pins bin2bcd_1/bcd1] [get_bd_pins display_7_seg_0/BCD3]
  connect_bd_net -net clk_in_0_1 [get_bd_ports clk_in_0] [get_bd_pins divizor_ceas_0/clk_in]
  connect_bd_net -net display_7_seg_0_An [get_bd_ports An_0] [get_bd_pins display_7_seg_0/An]
  connect_bd_net -net display_7_seg_0_DP [get_bd_ports DP_0] [get_bd_pins display_7_seg_0/DP]
  connect_bd_net -net display_7_seg_0_Seg [get_bd_ports Seg_0] [get_bd_pins display_7_seg_0/Seg]
  connect_bd_net -net divizor_ceas_0_clk_out_led [get_bd_ports clk_out_led_0] [get_bd_pins divizor_ceas_0/clk_out_led] [get_bd_pins numarator_0_59_0/clk]
  connect_bd_net -net divizor_ceas_0_clk_out_seg [get_bd_pins display_7_seg_0/clk_out_seg] [get_bd_pins divizor_ceas_0/clk_out_seg]
  connect_bd_net -net numarator_0_59_0_carry_out [get_bd_pins numarator_0_59_0/carry_out] [get_bd_pins numarator_0_59_1/clk]
  connect_bd_net -net numarator_0_59_0_up_counter [get_bd_pins bin2bcd_1/bin_val] [get_bd_pins numarator_0_59_0/up_counter]
  connect_bd_net -net numarator_0_59_1_up_counter [get_bd_pins bin2bcd_0/bin_val] [get_bd_pins numarator_0_59_1/up_counter]
  connect_bd_net -net pause_0_1 [get_bd_ports pause_0] [get_bd_pins numarator_0_59_0/pause] [get_bd_pins numarator_0_59_1/pause]
  connect_bd_net -net reset_0_1 [get_bd_ports reset_0] [get_bd_pins display_7_seg_0/reset] [get_bd_pins divizor_ceas_0/reset] [get_bd_pins numarator_0_59_0/reset] [get_bd_pins numarator_0_59_1/reset]

  # Create address segments


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


