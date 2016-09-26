	.include "pokeagb/macros/battle_script.s"

	.global battle_script_victory
battle_script_victory:
        BS_CMP_WORD BS_OP_FLAGS_SET, battle_type_flags, 0x800000, 0f

        BS_MESSAGE 0x141
        BS_GOTO 1f

0:
        BS_MESSAGE 0x141        @ TODO: 0x16B (Trainer A and Trainer B were defeated)

1:
        BS_TRAINER_SLIDE_IN 1
        BS_WAITSTATE
        BS_MESSAGE 0xC

        @@ Skip sliding in the second trainer if a non-multi battle
        BS_CMP_WORD BS_OP_FLAGS_CLEAR, battle_type_flags, 0x800000, 2f

        @@ SLIDE OUT (0xf8, 0x1)
        BS_WAITSTATE

        BS_TRAINER_SLIDE_IN 3
        BS_WAITSTATE
        BS_MESSAGE 0xC          @ TODO: Second trainer loss. 0x16C in Emerald

2:
        BS_CALCULATE_MONEY 3f

3:
        BS_MESSAGE 0x1E         @ Money Message
        BS_GIVEMONEY

        BS_PICKUP
        BS_FINISH
