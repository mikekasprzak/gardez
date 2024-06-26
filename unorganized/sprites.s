


.export _Music
_Music: .INCBIN "Droid.sid", $7e


; http://www.cc65.org/mailarchive/2011-11/9665.html
; http://www.cc65.org/mailarchive/2006-11/5612.html

;.SEGMENT "AUDIO"

;_Music:

;.INCBIN "Knucklebusters.sid", $7e
;.INCBIN "Knucklebusters10.sid", $7e
;.INCBIN "One_Man_and_His_Droid.sid", $7e
;.INCBIN "Droid.sid", $7e

;.CODE

.export _SIDInit
.export _SIDStep

_SIDInit:
	lda #0
	tax
	tay
	jsr $4000
	rts

_SIDStep:
	jsr $4012
	rts
	
; ----------------------------------------------------------------------------------------------- ;

.export _SpriteHeader
.export _SpriteColor
.export _SpriteData

; ----------------------------------------------------------------------------------------------- ;

_SpriteHeader:
.BYTE 32, 0			; Count, PADDING
_SpriteColor:
.BYTE 0, 1			; Black, White
_SpriteData:		; Sprite Data (63+1 bytes each, raw plus attributes (MxxOCCCC))
.INCBIN "sprites.raw"

; ----------------------------------------------------------------------------------------------- ;

.export _CharHeader
.export _CharColor
.export _CharData
.export _TileData
.export _TileAttribData

.export _MapHeader
.export _MapData

; ----------------------------------------------------------------------------------------------- ;

_CharHeader:
.BYTE 24, 0			; Count, PADDING
_CharColor:
.BYTE 9, 11, 7, 0	; Brown, Gray, Yellow, PADDING
_CharData:			; Charactar Data (8 bytes per, raw data)
.INCBIN "chars.raw"

_TileData:			; Tile Data (9 bytes per, each an index in to the Char table)
.INCBIN "tiles.raw"
_TileAttribData:	; Tile Attribute Data (Color, etc). **BE SURE TO SELECT *PER CHARACTER* ON EXPORT!**
.INCBIN "attribs.raw"

_MapHeader:
.BYTE 13, 8			; Width, Height
_MapData:
.INCBIN "map.raw"

; ----------------------------------------------------------------------------------------------- ;
