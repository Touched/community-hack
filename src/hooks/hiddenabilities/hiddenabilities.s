    .thumb
    .align 2
    .global b_copy_species_ability_new_hook
    
    b_copy_species_ability_new_hook:
        push {lr}
        bl get_ability
        ldr r1, =(0x2023D6A)
        strb r0, [r1]
        pop {pc}
            

@@@ --------------------------------------------------------------------------

    .thumb
    .align 2
    .global atkE5_pickupitemcalculation_hook
    
    atkE5_pickupitemcalculation_hook:
        mov r1, r0
        mov r0, r5
        bl get_ability

    has_pickup:
        ldr r1, =(0x0802CEA2|1)
        bx r1
        
@@@ --------------------------------------------------------------------------

    .thumb
    .align 2
    .global ai_switch_blocking_ability_hook
    
    ai_switch_blocking_ability_hook:
        mov r1, r0
        mov r0, r4
        bl get_ability
        ldr r1, =(0x08039572|1)
        bx r1
        
        
@@@ --------------------------------------------------------------------------

    .thumb
    .align 2
    .global get_attr_ability_hook
    
    get_attr_ability_hook:
        cmp r1, #46
        bne hook_restore
        
    ability_bit_get:
        add r0, r0, #30
        ldrb r0, [r0]
        lsr r0, r0, #6
        ldr r3, =(0x0804036A|1)
        bx r3
        
    hook_restore:
        mov r9, r4
        mov r10, r4
        mov r7, #0
        mov r5, #0
        cmp r1, #0xA
        ble hop_decoder
        ldr r2, =(0x0803FD64|1)
        bx r2
        
    hop_decoder:
        ldr r1, =(0x0803FDC2|1)
        bx r1
