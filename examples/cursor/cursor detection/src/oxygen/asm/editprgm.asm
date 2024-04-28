public _EditPrgm
include 'include/ez80.inc'
include 'include/ti84pceg.inc'

_EditPrgm:
	extern __exitsp
	ld	iy, ti.flags
	call	ti.DrawStatusBar
	pop	bc
	pop	hl
	pop	de
	ld.sis	(ti.errOffset and 0FFFFh), de

	ld	de, 0xD0065B
	ld	bc, 8
	push	hl
	ldir
	pop	hl

	call	ti.Mov9ToOP1
	call	ti.PushOP1
;make a reloader?

; Cleanup C things
	ld   sp,(__exitsp)
	pop   iy,af,hl
	ld   (hl),a
    call   $04F0
	
	ld	de, ti.plotSScreen
	ld	hl, StartProgramEditor
	ld	bc, StopProgramEditor - StartProgramEditor
	ldir
	jp	ti.plotSScreen
	
StartProgramEditor:
	ld	hl, ti.userMem
	ld	de, (ti.asm_prgm_size)
	call	ti.DelMem
	ld	a, ti.kPrgmEd
	call	ti.NewContext
	ld.sis	bc, (ti.errOffset and 0FFFFh)
	ld	hl, (ti.editTail)
	ld	de, (ti.editCursor)
	ld	a, b
	or	a, c
	jr	z, FindPreviousEnter
	ldir
	ld	(ti.editTail), hl
	ld	(ti.editCursor), de
	
FindPreviousEnter:
	call	ti.BufLeft
	jr	z, AtTopOfProgram
	ld	a, e
	cp	a, ti.tEnter
	jr	z, AtStartOfLine
	inc	bc
	jr	FindPreviousEnter

AtStartOfLine:
	call	ti.BufRight

AtTopOfProgram:
	push	bc
	call	ti.ClrWindow
	ld	hl, 0000001h
	ld	(ti.curRow), hl
	ld	a, ':'
	call	ti.PutC
	call	ti.DispEOW
	pop	bc

FindCursor:
	ld	a, b
	or	a, c
	jp	z, ti.Mon
	call	ti.CursorRight
	dec	bc
	jr	FindCursor

StopProgramEditor: