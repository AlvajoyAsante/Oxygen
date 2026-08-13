.assume adl=1

.equ vram16_base, 0xD40000

.section .text
.global _vram16_Clear
.global _vram16_SetPixel
.global _vram16_GetPixel
.global _vram16_HorizLine
.global _vram16_VertLine
.global _vram16_Rectangle
.global _vram16_FillRectangle
.global _vram16_FillScreen
.global _vram16_ReplaceColor

; -----------------------------------------------------------------------------
; Internal helper: HL = VRAM address for (x,y)
; Input:
;   DE = x (16-bit value in D:E, upper byte must be 0)
;   A  = y (0..239)
; Clobbers: BC
; -----------------------------------------------------------------------------
vram16_addr_from_xy:
    ld hl, vram16_base
    or a
    jr z, .x_offset
.row_loop:
    ld bc, 640
    add hl, bc
    dec a
    jr nz, .row_loop
.x_offset:
    push hl
    ld hl, 0
    ld l, e
    ld h, d
    add hl, hl
    ex de, hl
    pop hl
    add hl, de
    ret

; -----------------------------------------------------------------------------
; void vram16_Clear(uint16_t color)
; -----------------------------------------------------------------------------
_vram16_Clear:
    push ix
    ld ix, 0
    add ix, sp

    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)

    ld hl, vram16_base
    ld bc, 38400
.clear_loop:
    ld a, b
    or c
    jr z, .clear_done
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    dec bc
    jr .clear_loop
.clear_done:
    pop ix
    ret

; -----------------------------------------------------------------------------
; void vram16_FillScreen(uint16_t color)
; -----------------------------------------------------------------------------
_vram16_FillScreen:
    jp _vram16_Clear

; -----------------------------------------------------------------------------
; void vram16_ReplaceColor(uint16_t old_color, uint16_t new_color)
; -----------------------------------------------------------------------------
_vram16_ReplaceColor:
    push ix
    ld ix, 0
    add ix, sp

    ld hl, vram16_base
    ld a, 240
.replace_row_loop:
    push af
    ld bc, 320
.replace_col_loop:
    ld d, (ix+6)
    ld a, (hl)
    cp d
    jr nz, .replace_skip_low
    inc hl
    ld d, (ix+7)
    ld a, (hl)
    cp d
    jr nz, .replace_next_pixel
    ld d, (ix+9)
    ld e, (ix+10)
    dec hl
    ld (hl), d
    inc hl
    ld (hl), e
    jr .replace_done_pixel
.replace_skip_low:
    inc hl
.replace_next_pixel:
.replace_done_pixel:
    inc hl
    dec bc
    ld a, b
    or c
    jr nz, .replace_col_loop
    pop af
    dec a
    jr nz, .replace_row_loop

    pop ix
    ret

; -----------------------------------------------------------------------------
; void vram16_SetPixel(uint16_t x, uint8_t y, uint16_t color)
; -----------------------------------------------------------------------------
_vram16_SetPixel:
    push ix
    ld ix, 0
    add ix, sp

    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld de, 0
    ld e, (ix+12)
    ld d, (ix+13)
    ld (hl), e
    inc hl
    ld (hl), d

    pop ix
    ret

; -----------------------------------------------------------------------------
; uint16_t vram16_GetPixel(uint16_t x, uint8_t y)
; Returns: HL = color (low 16 bits)
; -----------------------------------------------------------------------------
_vram16_GetPixel:
    push ix
    ld ix, 0
    add ix, sp

    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld e, (hl)
    inc hl
    ld d, (hl)
    ld hl, 0
    ld l, e
    ld h, d

    pop ix
    ret

; -----------------------------------------------------------------------------
; void vram16_HorizLine(uint16_t x, uint8_t y, uint16_t length, uint16_t color)
; -----------------------------------------------------------------------------
_vram16_HorizLine:
    push ix
    ld ix, 0
    add ix, sp

    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld bc, 0
    ld c, (ix+12)
    ld b, (ix+13)

    ld de, 0
    ld e, (ix+15)
    ld d, (ix+16)

.hline_loop:
    ld a, b
    or c
    jr z, .hline_done
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    dec bc
    jr .hline_loop
.hline_done:
    pop ix
    ret

; -----------------------------------------------------------------------------
; void vram16_VertLine(uint16_t x, uint8_t y, uint16_t length, uint16_t color)
; -----------------------------------------------------------------------------
_vram16_VertLine:
    push ix
    ld ix, 0
    add ix, sp

    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld bc, 0
    ld c, (ix+12)
    ld b, (ix+13)

    ld de, 0
    ld e, (ix+15)
    ld d, (ix+16)

.vline_loop:
    ld a, b
    or c
    jr z, .vline_done
    ld (hl), e
    inc hl
    ld (hl), d
    dec hl
    push bc
    ld bc, 640
    add hl, bc
    pop bc
    dec bc
    jr .vline_loop
.vline_done:
    pop ix
    ret

; -----------------------------------------------------------------------------
; void vram16_Rectangle(uint16_t x, uint8_t y, uint16_t width, uint16_t height,
;                       uint16_t color)
; -----------------------------------------------------------------------------
_vram16_Rectangle:
    push ix
    ld ix, 0
    add ix, sp

    ; top line
    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld bc, 0
    ld c, (ix+12)
    ld b, (ix+13)

    ld de, 0
    ld e, (ix+18)
    ld d, (ix+19)

.rect_top_loop:
    ld a, b
    or c
    jr z, .rect_top_done
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    dec bc
    jr .rect_top_loop
.rect_top_done:

    ; bottom line
    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld a, (ix+15)
    dec a
    jr z, .rect_bottom_ready
.rect_bottom_step:
    push af
    ld bc, 640
    add hl, bc
    pop af
    dec a
    jr nz, .rect_bottom_step
.rect_bottom_ready:

    ld bc, 0
    ld c, (ix+12)
    ld b, (ix+13)

    ld de, 0
    ld e, (ix+18)
    ld d, (ix+19)

.rect_bottom_loop:
    ld a, b
    or c
    jr z, .rect_bottom_done
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    dec bc
    jr .rect_bottom_loop
.rect_bottom_done:

    ; left line
    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld bc, 0
    ld c, (ix+15)
    ld b, (ix+16)

    ld de, 0
    ld e, (ix+18)
    ld d, (ix+19)

.rect_left_loop:
    ld a, b
    or c
    jr z, .rect_left_done
    ld (hl), e
    inc hl
    ld (hl), d
    dec hl
    push bc
    ld bc, 640
    add hl, bc
    pop bc
    dec bc
    jr .rect_left_loop
.rect_left_done:

    ; right line
    ld hl, 0
    ld l, (ix+6)
    ld h, (ix+7)
    ld bc, 0
    ld c, (ix+12)
    ld b, (ix+13)
    add hl, bc
    dec hl

    ld de, 0
    ld e, l
    ld d, h

    ld a, (ix+9)
    call vram16_addr_from_xy

    ld bc, 0
    ld c, (ix+15)
    ld b, (ix+16)

    ld de, 0
    ld e, (ix+18)
    ld d, (ix+19)

.rect_right_loop:
    ld a, b
    or c
    jr z, .rect_right_done
    ld (hl), e
    inc hl
    ld (hl), d
    dec hl
    push bc
    ld bc, 640
    add hl, bc
    pop bc
    dec bc
    jr .rect_right_loop
.rect_right_done:

    pop ix
    ret

; -----------------------------------------------------------------------------
; void vram16_FillRectangle(uint16_t x, uint8_t y, uint16_t width,
;                           uint16_t height, uint16_t color)
; -----------------------------------------------------------------------------
_vram16_FillRectangle:
    push ix
    ld ix, 0
    add ix, sp

    ld de, 0
    ld e, (ix+6)
    ld d, (ix+7)
    ld a, (ix+9)
    call vram16_addr_from_xy

    ld a, (ix+15)
    ld de, 0
    ld e, (ix+18)
    ld d, (ix+19)

.fill_row_loop:
    or a
    jr z, .fill_done

    push af
    push hl

    ld bc, 0
    ld c, (ix+12)
    ld b, (ix+13)

.fill_col_loop:
    ld a, b
    or c
    jr z, .fill_row_done
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    dec bc
    jr .fill_col_loop

.fill_row_done:
    pop hl
    pop af

    push af
    ld bc, 640
    add hl, bc
    pop af
    dec a
    jr .fill_row_loop

.fill_done:
    pop ix
    ret

.section ".note.GNU-stack","",@progbits
