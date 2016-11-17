        // Hook for hidden ability read position in pokemon_getattr
        .org 0x0803FD58
            ldr r5, =(get_attr_ability_hook|1);
            bx r5
        .pool

        // Hook the battle malloc
        .org 0x08040938
            ldrb r0, [r4]
            lsl r0, r0, #0x6
            mov r1, r7
            orr r0, r1
            strb r0, [r7, #0x1D]
            mov r8, r8
            mov r8, r8
            
        // b_copy_species_ability hook
        .org 0x08040D38
            ldr r2,=(b_copy_species_ability_new_hook|1)
            bx r2
        .pool
            
        // Pick up , atkE5_pickupitemcalculation
        .org 0x0802CE7E
            ldr r1, =(atkE5_pickupitemcalculation_hook|1)
            bx r1
        .pool
            
        // ai_switch_blocking_ability hook
        .org 0x08039548
            ldr r1, =(ai_switch_blocking_ability_hook|1)
            bx r1
        .pool

// patch abilities

		.org 0x82547D4
		.byte 0x22// ABILITY_CHLOROPHYLL

		.org 0x82547F0
		.byte 0x22// ABILITY_CHLOROPHYLL

		.org 0x825480C
		.byte 0x22// ABILITY_CHLOROPHYLL

		.org 0x825487C
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8254898
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x82548B4
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x82548D0
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x8254924
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x82549CC
		.byte 0x37// ABILITY_HUSTLE

		.org 0x82549E8
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254AE4
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254B00
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254B38
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254B54
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254BC4
		.byte 0x46// ABILITY_DROUGHT

		.org 0x8254BE0
		.byte 0x46// ABILITY_DROUGHT

		.org 0x8254C6C
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x8254C88
		.byte 0x1// ABILITY_STENCH

		.org 0x8254CA4
		.byte 0x1B// ABILITY_EFFECT_SPORE

		.org 0x8254CC0
		.byte 0x6// ABILITY_DAMP

		.org 0x8254CDC
		.byte 0x6// ABILITY_DAMP

		.org 0x8254CF8
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x8254DA0
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254DBC
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254E48
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254E64
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254E80
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254F98
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8254FB4
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8254FD0
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8254FEC
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8255008
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8255024
		.byte 0x31// ABILITY_FLAME_BODY

		.org 0x8255040
		.byte 0x31// ABILITY_FLAME_BODY

		.org 0x8255238
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x8255254
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x8255318
		.byte 0x4// ABILITY_BATTLE_ARMOR

		.org 0x8255334
		.byte 0x4// ABILITY_BATTLE_ARMOR

		.org 0x825536C
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x8255388
		.byte 0xD// ABILITY_CLOUD_NINE

		.org 0x825544C
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x8255468
		.byte 0x6// ABILITY_DAMP

		.org 0x8255484
		.byte 0x6// ABILITY_DAMP

		.org 0x8255564
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x8255580
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x8255698
		.byte 0x3E// ABILITY_GUTS

		.org 0x82557B0
		.byte 0x31// ABILITY_FLAME_BODY

		.org 0x82557CC
		.byte 0x3F// ABILITY_MARVEL_SCALE

		.org 0x82557E8
		.byte 0x3F// ABILITY_MARVEL_SCALE

		.org 0x82558AC
		.byte 0x12// ABILITY_FLASH_FIRE

		.org 0x82558C8
		.byte 0x12// ABILITY_FLASH_FIRE

		.org 0x82558E4
		.byte 0x12// ABILITY_FLASH_FIRE

		.org 0x8255A50
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8255A6C
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8255B4C
		.byte 0x39// ABILITY_PLUS

		.org 0x8255B68
		.byte 0x39// ABILITY_PLUS

		.org 0x8255B84
		.byte 0x39// ABILITY_PLUS

		.org 0x8255C10
		.byte 0x2// ABILITY_DRIZZLE

		.org 0x8255C9C
		.byte 0x30// ABILITY_EARLY_BIRD

		.org 0x8255CB8
		.byte 0x30// ABILITY_EARLY_BIRD

		.org 0x8255D44
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x8255E5C
		.byte 0x11// ABILITY_IMMUNITY

		.org 0x8255ECC
		.byte 0x16// ABILITY_INTIMIDATE

		.org 0x8255FC8
		.byte 0x2F// ABILITY_THICK_FAT

		.org 0x8255FE4
		.byte 0x2F// ABILITY_THICK_FAT

		.org 0x8256054
		.byte 0xF// ABILITY_INSOMNIA

		.org 0x8256070
		.byte 0x29// ABILITY_WATER_VEIL

		.org 0x82560E0
		.byte 0x6// ABILITY_DAMP

		.org 0x82560FC
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8256118
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8256188
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x82561DC
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x82561F8
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x82562A0
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x825639C
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x82563B8
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x82563D4
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x82563F0
		.byte 0x6// ABILITY_DAMP

		.org 0x825640C
		.byte 0x6// ABILITY_DAMP

		.org 0x8256428
		.byte 0x6// ABILITY_DAMP

		.org 0x82564B4
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x8256524
		.byte 0xE// ABILITY_COMPOUND_EYES

		.org 0x8256540
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x825655C
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x8256578
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x8256620
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x825663C
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x82566AC
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8256770
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x8256984
		.byte 0x3A// ABILITY_MINUS

		.org 0x82569A0
		.byte 0x3A// ABILITY_MINUS

		.org 0x82569D8
		.byte 0xA// ABILITY_VOLT_ABSORB

		.org 0x8256A80
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x8256A9C
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x8256AB8
		.byte 0x2E// ABILITY_PRESSURE

		.org 0x8256AD4
		.byte 0x2E// ABILITY_PRESSURE

		.org 0x8256AF0
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x8256B28
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x8256BEC
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8256C08
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8256C24
		.byte 0xD// ABILITY_CLOUD_NINE

		.org 0x8256C40
		.byte 0xD// ABILITY_CLOUD_NINE

		.org 0x8256DAC
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8256DC8
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8256E00
		.byte 0x38// ABILITY_CUTE_CHARM

		.org 0x8256F6C
		.byte 0xC// ABILITY_OBLIVIOUS

		.org 0x8256F88
		.byte 0xC// ABILITY_OBLIVIOUS

		.org 0x8256FA4
		.byte 0xC// ABILITY_OBLIVIOUS

		.org 0x8256FDC
		.byte 0x29// ABILITY_WATER_VEIL

		.org 0x8257014
		.byte 0x5// ABILITY_STURDY

		.org 0x82570F4
		.byte 0x5// ABILITY_STURDY

		.org 0x825720C
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x8257228
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x8257244
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x8257394
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x82573CC
		.byte 0x3E// ABILITY_GUTS

		.org 0x82573E8
		.byte 0x3E// ABILITY_GUTS

		.org 0x8257404
		.byte 0x3E// ABILITY_GUTS

		.org 0x8257490
		.byte 0x2B// ABILITY_SOUNDPROOF

		.org 0x82574AC
		.byte 0x2B// ABILITY_SOUNDPROOF

