/*
 *
 * Memory map (STM32F103C8 Datasheet.pdf page 34):
 * 0x0000 0000 - 0x07FF FFFF : Aliased to Flash or system memory depending on BOOT pins
 * 0x0800 0000 - 0x0800 FFFF : Flash (64KB)
 * 0x1FFF F000 - 0x1FFF F7FF : System memory / Boot firmware
 * 0x1FFF F800 - 0x1FFF F80E : Option Bytes
 * 0x2000 0000 - 0x4000 0000 : SRAM (20KB)
 */

MEMORY {
	flash(rx)	: ORIGIN = 0x08000000, LENGTH = 64K
	ram(rwx)	: ORIGIN = 0x20000000, LENGTH = 20K
}

ENTRY(_start)	/* Mark program entry*/

SECTIONS {
	.text : {
		/*. = ALIGN(4);*/
		*(.text)
		*(.text*)
		*(.rodata)
		*(.rodata.*)
		/*. = ALIGN(4);*/
	} >flash

	. = ALIGN(4);
	.bss : {
		/* remember start address of section bss */
		_sbss = .;
    	__bss_start = _sbss;

		*(.bss)
		
		. = ALIGN(4);

		/* remember end address of section bss */
		_ebss = .;
    	__bss_end = _ebss;
	} >ram

	.data : {
		. = ALIGN(4);
		_sdata = .;

		*(.data)
		*(.data.*)

		. = ALIGN(4);
		_edata = .;
	} >ram AT >flash
}
