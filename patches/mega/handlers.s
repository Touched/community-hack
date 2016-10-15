        // Insert the mega evolution handler such that
        // it activates before the moves are executed
        .org 0x08250038
        .word mega_evolution_handler|1
