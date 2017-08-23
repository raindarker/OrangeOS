%include "sconst.inc"

  INT_VECTOR_SYS_CALL equ 0x90
  _NR_printx          equ 0
  _NR_sendrecv        equ 1

global  printx
global  sendrecv

bits 32
[section .text]

printx:
  mov	eax, _NR_printx
  mov	edx, [esp + 4]
  int	INT_VECTOR_SYS_CALL
  ret

sendrecv:
  mov	eax, _NR_sendrecv
  mov	ebx, [esp + 4]	; function
  mov	ecx, [esp + 8]	; src_dest
  mov	edx, [esp + 12]	; p_msg
  int	INT_VECTOR_SYS_CALL
  ret
