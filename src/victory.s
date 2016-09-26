	.include "pokeagb/macros/battle_script.s"

	.global battle_script_victory
battle_script_victory:
        BS_CMP_WORD BS_OP_FLAGS_SET, battle_type_flags, 0x800000, 0f

        @@ "Player defeated ..."
        BS_MESSAGE 0x141
        BS_GOTO 1f

0:
	@@ "Trainer A and Trainer B were defeated"
        BS_MESSAGE 0x14d

1:
        BS_TRAINER_SLIDE_IN 1
        BS_WAITSTATE

        @@ Print first enemy trainer loss message
        BS_MESSAGE 0xC

        @@ Skip sliding in the second trainer if a non-multi battle
        BS_CMP_WORD BS_OP_FLAGS_CLEAR, battle_type_flags, 0x800000, 2f

        @@ Callasm to trainer slide out
        .byte 0xf8
        .word multi_opponent_slide_out

        BS_WAITSTATE

        BS_TRAINER_SLIDE_IN 3
        BS_WAITSTATE

        @@ Print second enemy trainer loss message.
        @@ We just reconfigure the battle so we can use the same message.
        BS_MESSAGE 0xC

2:
        BS_CALCULATE_MONEY 3f

3:
	@@ Display winnings message
        BS_MESSAGE 0x1E
        BS_GIVEMONEY

        BS_PICKUP
        BS_FINISH
