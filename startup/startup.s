.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

//global memory location

.global Reset_Handler

/* The reset handler. Called on reset */
.type Reset_Handler, %function
Reset_Handler:
    // Set the stack pointer to the end of the stack
    // The '_estack' value is defined in our linker srcipt
    LDR r0, = _estack
    MOV sp, r0

    // Copy data from flash to RAM data init section
    // R2 will store our progress along the sidata section
    MOVS r0, #0
    // Load the start/end addresses of the data sections,
    // and the start of the data init section
    LDR r1, =_sdata
    LDR r2, =_edata
    LDR r3, =_sidata
    B copy_sidata_loop

copy_sidata:
    // offset the data init section by our copy progess
    // load data from FLASH + offset to r4
    LDR r4, [r3, r0]
    // copy the current word into data, and increment
    // store data r4 to RAM data + offset
    STR r4, [r1, r0]
    // increment offset by 4 bytes
    ADDS r0, r0, #4

copy_sidata_loop:
    // r4 = r0 + r1
    ADDS r4, r0, r1
    // compare start data with end data
    CMP r4, r2
    // brach if carry is clear
    BCC copy_sidata

//One copying the data section into RAM done.
//Move on to filling the BSS section with 0s
MOVS r0, #0
LDR r1, =_sbss
LDR r2, =_ebss
B reset_bss_loop

reset_bss:
    //Store a 0 and increment by a word
    STR r0, [r1]
    ADDS r1, r1, #4

reset_bss_loop:
    //We'll use R1 to count progress here
    // compare start and end bss
    CMP r1, r2
    BCC reset_bss

//Branch to main method
    B main
.size Reset_Handler, .-Reset_Handler