
macr macro_1
mov #1,r1
cmp r0, r1
add MAIN ,r2
endmacr

macr macro_3
 inc LIST
 dec r1
endmacr

macr macro_2
 clr r3
 not *r7
endmacr
; Calling macro_1
macro_1
MAIN: sub *r3     , r2
LOOP: lea t ,r6
; Calling macro_2
macro_2
; Calling macro_3
macro_3
jmp k
bne LOOP
red r1
prn #1
jsr *r1
rts
END: stop
STR: .string "abcd"
LIST: .data 6, -1
.data -100

.extern k
.extern t
.entry MAIN
.entry LOOP
.entry LIST