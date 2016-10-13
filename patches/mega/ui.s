	// Wrap the fight menu callback to show a custom menu
        .org 0x0802E79C
        .word mega_ui_menu|1

        .org 0x0802F398
        .word mega_ui_menu|1

        .org 0x0802F3FC
        .word mega_ui_menu|1

        .org 0x08032C8C
        .word mega_ui_menu|1

        // TODO: Hook 080E7988 (make it call mega_ui_menu)

        // Load the indicator graphics when the healthbars are loaded
        .org 0x0803495C
        ldr r1, =mega_ui_load_indicators_hook|1
        bx r1
        .pool
