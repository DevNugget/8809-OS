; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1            
MBFLAGS  equ  MBALIGN | MEMINFO 
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text

extern gdt_ptr

; Function to load the GDT (called from C)
global gdt_load
gdt_load:
    lgdt [gdt_ptr]  ; Load the GDT pointer
    jmp 0x08:.reload_cs  ; Far jump to reload CS (0x08 = code segment offset)
.reload_cs:
    ; Reload data segment registers
    mov ax, 0x10  ; 0x10 = data segment offset
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

global idt_load
extern idt_ptr
idt_load:
    lidt [idt_ptr]
    ret

; ISR Stubs (macro for error code/no error code)
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    cli
    push byte 0  ; Dummy error code
    push byte %1 ; Interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    push byte %1 ; Interrupt number (error code already pushed by CPU)
    jmp isr_common_stub
%endmacro

; Define CPU exception ISRs
ISR_NOERRCODE 0   ; Divide-by-zero
ISR_NOERRCODE 1   ; Debug
ISR_NOERRCODE 2   ; NMI
ISR_NOERRCODE 3   ; Breakpoint
ISR_NOERRCODE 4   ; Into Detected Overflow
ISR_NOERRCODE 5   ; Out of Bounds
ISR_NOERRCODE 6   ; Invalid Opcode
ISR_NOERRCODE 7   ; No Coprocessor
ISR_ERRCODE   8   ; Double Fault
ISR_NOERRCODE 9   ; Coprocessor Segment Overrun (legacy)
ISR_ERRCODE   10  ; Invalid TSS
ISR_ERRCODE   11  ; Segment Not Present
ISR_ERRCODE   12  ; Stack Segment Fault
ISR_ERRCODE   13  ; General Protection Fault
ISR_ERRCODE   14  ; Page Fault
ISR_NOERRCODE 15  ; Reserved
ISR_NOERRCODE 16  ; x87 Floating-Point Exception
ISR_NOERRCODE 17  ; Alignment Check
ISR_NOERRCODE 18  ; Machine Check
ISR_NOERRCODE 19  ; SIMD Floating-Point Exception
ISR_NOERRCODE 20  ; Virtualization Exception
ISR_NOERRCODE 21  ; Control Protection Exception
ISR_NOERRCODE 22  ; Reserved
ISR_NOERRCODE 23  ; Reserved
ISR_NOERRCODE 24  ; Reserved
ISR_NOERRCODE 25  ; Reserved
ISR_NOERRCODE 26  ; Reserved
ISR_NOERRCODE 27  ; Reserved
ISR_NOERRCODE 28  ; Hypervisor Injection Exception
ISR_NOERRCODE 29  ; VMM Communication Exception
ISR_NOERRCODE 30  ; Security Exception
ISR_NOERRCODE 31  ; Reserved

extern isr_handler

; Common ISR handler (calls C function)
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, isr_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

; IQR stubs
%macro IRQ 2
global irq%1
irq%1:
    cli
    push byte 0     ; Dummy error code
    push byte %2    ; Interrupt number (32-47)
    jmp irq_common_stub
%endmacro

; Define all 16 IRQs
IRQ  0, 32     ; PIT Timer
IRQ  1, 33     ; Keyboard
IRQ  2, 34     ; Cascade (unused)
IRQ  3, 35     ; COM2
IRQ  4, 36     ; COM1
IRQ  5, 37     ; LPT2
IRQ  6, 38     ; Floppy
IRQ  7, 39     ; LPT1
IRQ  8, 40     ; CMOS RTC
IRQ  9, 41     ; Free
IRQ 10, 42     ; Free
IRQ 11, 43     ; Free
IRQ 12, 44     ; PS2 Mouse
IRQ 13, 45     ; FPU
IRQ 14, 46     ; Primary ATA
IRQ 15, 47     ; Secondary ATA

extern irq_handler

irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

global _start:function (_start.end - _start)
_start:
	mov esp, stack_top

    push eax
    push ebx

	extern kernel_main
	call kernel_main

	cli
.hang:	hlt
	jmp .hang
.end:
