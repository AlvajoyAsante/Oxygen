.assume adl=1

.equ ti_errOffset, 0x0D008E3
.equ ti_Mov9ToOP1, 0x0020320
.equ ti_NewContext, 0x002016C
.equ ti_EditProg, 0x0020E74
.equ ti_kPrgmEd, 0x46

.section .text
.global _EditPrgm

; extern __exitsp

_EditPrgm:
    ; C ABI (ADL): arg0 at (sp+3), arg1 at (sp+6) before pushes.
    ; Preserve IX/IY as required by the C calling convention.
    push ix
    push iy
    ld ix, 0
    add ix, sp

    ; arg0: char *progname
    ld hl, (ix + 9)

    ; arg1: cursor/offset hint (low 16 bits used by OS)
    ld de, 0
    ld e, (ix + 12)
    ld d, (ix + 13)
    ld hl, ti_errOffset
    ld (hl), de

    ; Build OP1 from the variable name and enter program editor context.
    call ti_Mov9ToOP1
    ld a, ti_kPrgmEd
    call ti_NewContext
    call ti_EditProg

    pop iy
    pop ix
    ret

.section ".note.GNU-stack","",@progbits
