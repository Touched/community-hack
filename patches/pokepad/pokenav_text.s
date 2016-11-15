	.org 0x083A7348
	.word prelaunch_pokenav_setup|1

	.org 0x0806F404
	.word prelaunch_pokenav_setup|1

        .org 0x083A7394
        .word pokepad_sm_description

        .org 0x083A7344
        .word pokepad_sm_name
        
        

// oei animation. Currently replaces grass. TODO: replace a null effect        
    .org 0x081D98A8
    .word cave_state
    .pool
    
    // extend animation frames for hidden water and cave
    .org 0x083A5B56
    .byte 0x8
    
    .org 0x083A5B5A
    .byte 0x8
    
    .org 0x083A5B5E
    .byte 0x8

    .org 0x083A5B62
    .byte 0x8
    
    .org 0x083A5B66
    .byte 0x8
    
    .org 0x083A5B6A
    .byte 0x8
    
