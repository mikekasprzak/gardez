        .export _main
        .segment "CODE"

_main:  lda #$00
        tax
        tay
        jsr song                ; Initialize music 

mainloop:
        lda $d012
        cmp #$80                ; Is rasterline equal to #$80?
        bne mainloop            ; If not, keep checking

        inc $d020               ; Inc border color
        jsr song+3              ; Jump to music play routine
        dec $d020               ; Dec border color
        jmp mainloop            ; Keep looping


        .segment "MUSIC"
;song:   .incbin "One_Man_and_His_Droid.sid", $7e
song:   .incbin "Small_Intro.sid", $7e
