	.align 2
	.global repointed_battle_messages
repointed_battle_messages:
        .incbin "roms/BPRE0.gba", 0x3FDF3C, (0x176 * 4)
        .word message_mega_evolution_start
        .word message_mega_wish_start
        .word message_mega_evolution_end

        .align 2
        .global message_mega_evolution_start
message_mega_evolution_start:
        .string "{STRING 18}’s {STRING 22} is reacting\nto {STRING 2}’s {STRING 3}!\p$"

        .align 2
	.global message_mega_wish_start
message_mega_wish_start:
        .string "{STRING 2}’s fervant wish has reached {STRING 18}!\p$"

        .align 2
        .global message_mega_evolution_end
message_mega_evolution_end:
        .string "{STRING 18} Mega Evolved into\nMega {STRING 18}!\p$"
