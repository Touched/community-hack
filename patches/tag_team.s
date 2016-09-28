	// Quick hack to enable the tag team party menu
        // FIXME: Do it properly
        .org 0x0811FA2E
        mov r0, #1

        // Load
        .org 0x08127AE8
        ldr r0, =tag_team_partner_name_in_switch_hook|1
        bx r0
        .pool
