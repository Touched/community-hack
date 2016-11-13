//Skips checking for HM:s in Pokemon menu and jumps directly to the field move check.
.org 0x08122A2A
ldrh r0, [r0, #0xE]

.org 0x08122A34
.byte 0x7

//Makes all moves deletable.
.org 0x080441D6
.byte 0x0

.org 0x08125AA8
.byte 0x0
