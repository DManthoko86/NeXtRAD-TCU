library ieee;
use ieee.std_logic_1164.all;

library unisim;
use unisim.vcomponents.all;

----------------------------------------------------------------------
-- Description: 
----------------------------------------------------------------------
----                                                              ----
---- Copyright (C) 2015 Lekhobola Tsoeunyane                       ----
----     lekhobola (at) gmail.com                             ----
----                                                              ----
---- This source file may be used and distributed without         ----
---- restriction provided that this copyright statement is not    ----
---- removed from the file and that any derivative work contains  ----
---- the original copyright notice and the associated disclaimer. ----
----                                                              ----
---- This source file is free software; you can redistribute it   ----
---- and/or modify it under the terms of the GNU Lesser General   ----
---- Public License as published by the Free Software Foundation; ----
---- either version 2.1 of the License, or (at your option) any   ----
---- later version.                                               ----
----                                                              ----
---- This source is distributed in the hope that it will be       ----
---- useful, but WITHOUT ANY WARRANTY; without even the implied   ----
---- warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ----
---- PURPOSE.  See the GNU Lesser General Public License for more ----
---- details.                                                     ----
----                                                              ----
---- You should have received a copy of the GNU Lesser General    ----
---- Public License along with this source; if not, download it   ----
---- from http://www.opencores.org/lgpl.shtml                     ----
----                                                              ----
----------------------------------------------------------------------

entity clk_manager is
	port(
		--External Control
		dcm_100mhz_in: in std_logic;
--		SYS_CLK_P_i  : in  std_logic;
--		SYS_CLK_N_i  : in  std_logic;
		SYS_RST_i    : in  std_logic;

		-- Clock out ports
		clk_125mhz    : out std_logic;
		clk_100mhz    : out std_logic;	
		clk_25mhz     : out std_logic;
		
		-- Status and control signals
		RESET         : out std_logic;
		sysclk_locked : out std_logic
	);
end entity clk_manager;

architecture RTL of clk_manager is
	
	component dcm
	port(-- Clock in ports
		  dcm_100mhz_in     : in     std_logic;
		  -- Clock out ports
		  dcm_125mhz        : out    std_logic;
		  dcm_100mhz        : out    std_logic;
		  dcm_25mhz         : out    std_logic;
		  -- Status and control signals
		  RESET             : in     std_logic;
		  LOCKED     		  : out    std_logic
		 );
	end component;
	
	--external buffering
	signal sys_clk_i_b : std_logic;

begin

	----	already declared
-- External buffering
--	SYS_CLK_IBUFGDS : IBUFGDS
--		generic map(
--			DIFF_TERM  => FALSE,
--			IOSTANDARD => "LVPECL_33"
--		)
--		port map(
--			O  => sys_clk_i_b,
--			I  => SYS_CLK_P_i,
--			IB => SYS_CLK_N_i
--		);
		
	dcm_inst : dcm
		port map(-- Clock in ports
		  dcm_100mhz_in  => dcm_100mhz_in,
		  -- Clock out ports
		  dcm_125mhz     => clk_125mhz,
		  dcm_100mhz     => clk_100mhz,
		  dcm_25mhz      => clk_25mhz,
		  -- Status and control signals
		  RESET		     => SYS_RST_i,
		  locked         => sysclk_locked
	);
end architecture RTL;
