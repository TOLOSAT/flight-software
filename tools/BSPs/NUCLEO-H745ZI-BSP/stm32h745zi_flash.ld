ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM_D1) + LENGTH(RAM_D1); /* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x200 ; /* required amount of heap */
_Min_Stack_Size = 0x400 ; /* required amount of stack */

MEMORY
{
	RAM_D1(xrw)   : ORIGIN = 0x24000000, LENGTH = 512K
	FLASH(rx)     : ORIGIN = 0x08000000, LENGTH = 1024K    /* Memory is divided. Actual start is 0x08000000 and actual length is 2048K */
	DTCMRAM(xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
	RAM_D2(xrw)   : ORIGIN = 0x30000000, LENGTH = 288K
	RAM_D3(xrw)   : ORIGIN = 0x38000000, LENGTH = 64K
	ITCMRAM(xrw)  : ORIGIN = 0x00000000, LENGTH = 64K
}

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector)) /* Startup code */
		. = ALIGN(4);
	} >FLASH

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text*)
		. = ALIGN(4);
		_etext = .;
	} >FLASH

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)         /* .rodata sections (constants, strings, etc.) */
		*(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
		. = ALIGN(4);
	} >FLASH
	
	/* Utiliser dans le startup pour initialiser les datas*/
	_sidata = LOADADDR(.data);

	.data :
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		_edata = .;
	} > RAM_D1 AT> FLASH

	.bss :
	{
		. = ALIGN(4);
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
	} >RAM_D1

	._user_heap_stack :
	{
		. = ALIGN(8);
		PROVIDE ( end = . );
		PROVIDE ( _end = . );
		. = . + _Min_Heap_Size;
		. = . + _Min_Stack_Size;
		. = ALIGN(8);
	} >RAM_D1

	/* Remove information from the compiler libraries */
	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}
}