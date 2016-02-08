
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name UDP_1GbE_if -dir "/home/skippy/Git/NeXtRAD-TCU/planAhead_run_2" -part xc6slx150tfgg676-2
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "/home/skippy/Git/NeXtRAD-TCU/gpmc_test_top.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {/home/skippy/Git/NeXtRAD-TCU} {pcores/rtl/spartan6} }
add_files [list {pcores/rtl/spartan6/icon.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {pcores/rtl/spartan6/ila1.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {pcores/rtl/spartan6/ila2.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {pcores/rtl/spartan6/ila.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {pcores/rtl/spartan6/vio.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {pcores/rtl/spartan6/ila0.ncf}] -fileset [get_property constrset [current_run]]
set_property target_constrs_file "/home/skippy/Git/NeXtRAD-TCU/pcores/rtl/ip/UDP_1GbE_if.ucf" [current_fileset -constrset]
add_files [list {/home/skippy/Git/NeXtRAD-TCU/pcores/rtl/ip/UDP_1GbE_if.ucf}] -fileset [get_property constrset [current_run]]
link_design
