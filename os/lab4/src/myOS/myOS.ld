OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(start)

SECTIONS {
	. = 1M;
	.text : {
		__multiboot_start = .;
	    *(.multiboot_header) 
		__multiboot_end = .;
		. = ALIGN(8);
		__text_start = .;
		*(.text)
		__text_end = .;
	}
	
	. = ALIGN(16);
	__data_start = .;
	.data		: { *(.data*) }
	__data_end = .;
	
	. = ALIGN(16);
	.bss		:
	{
		__bss_start = .;
		_bss_start = .;
		*(.bss)
		__bss_end = .;
	}
	. = ALIGN(16);
	_end = .;
}
