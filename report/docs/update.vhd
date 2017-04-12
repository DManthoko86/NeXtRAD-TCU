--this file contains the code snippets of the line of code that were added or changed
--in gpmc_test.vhd in NeXtRad-TCU-Controller

-- 16 bit versions  of the P and Pcounter
signal P					:	integer range 0 to 65535 := 0;
signal Pcounter		:	integer range 0 to 65535 := 0;

-- 32 bit versions of P and Pcounter
signal P			:	std_logic_vector(31 downto 0) := x"00000000";
signal Pcounter		:	std_logic_vector(31 downto 0) := x"00000000";
	
	
-- populate dataout from regbank based on Program Counter (PC)
dataout <= reg_bank(PC) & reg_bank(PC+1) & reg_bank(PC+2) & reg_bank(PC+3) & reg_bank(PC+4) & reg_bank(PC+5);
MB <= conv_integer(reg_bank(PC));
D <= conv_integer(reg_bank(PC+1));
P <= reg_bank(PC+2) & reg_bank(PC+5);
...
