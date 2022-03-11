
* = $1000
lda #$FF


start
sta $90
sta $8000
eor #$CC
jmp start