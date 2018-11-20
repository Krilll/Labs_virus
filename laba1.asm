%include "io.inc"

; settings
%define MAX_STACK_SIZE 10
%define STOP_SYMBOL 0xA
%define STACK_PTR pointer
%define STACK_SIZE_PTR STACK_PTR
%define STACK_DATA_PTR STACK_PTR + 1
%define STACK_SIZE [STACK_SIZE_PTR]

section .bss
pointer resb 5

section .text
global CMAIN

CMAIN:
mov edi, STACK_DATA_PTR
jmp _input_loop

_input_loop:
GET_CHAR al
cmp al, STOP_SYMBOL
je _pop_loop
jmp _push

_push:
mov [edi], al
inc edi
cmp byte STACK_SIZE, MAX_STACK_SIZE
je _error
inc byte STACK_SIZE
jmp _input_loop

_pop_loop:
dec edi
PRINT_CHAR [edi]
dec byte STACK_SIZE
cmp edi, STACK_DATA_PTR
jne _pop_loop
Ret

_error:
PRINT_STRING "stackoverflow"
NEWLINE
Ret
