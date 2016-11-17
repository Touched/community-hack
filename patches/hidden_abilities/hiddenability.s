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

        // BC read battle data bit shift undo
        .org 0x08013152
            mov r8, r8


// patch abilities

		.org 0x82547BA
		.byte 0x22// ABILITY_CHLOROPHYLL

		.org 0x82547D6
		.byte 0x22// ABILITY_CHLOROPHYLL

		.org 0x82547F2
		.byte 0x22// ABILITY_CHLOROPHYLL

		.org 0x8254862
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x825487E
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x825489A
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x82548B6
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x825490A
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x82549B2
		.byte 0x37// ABILITY_HUSTLE

		.org 0x82549CE
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254ACA
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254AE6
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254B1E
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254B3A
		.byte 0x37// ABILITY_HUSTLE

		.org 0x8254BAA
		.byte 0x46// ABILITY_DROUGHT

		.org 0x8254BC6
		.byte 0x46// ABILITY_DROUGHT

		.org 0x8254C52
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x8254C6E
		.byte 0x1// ABILITY_STENCH

		.org 0x8254C8A
		.byte 0x1B// ABILITY_EFFECT_SPORE

		.org 0x8254CA6
		.byte 0x6// ABILITY_DAMP

		.org 0x8254CC2
		.byte 0x6// ABILITY_DAMP

		.org 0x8254CDE
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x8254D86
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254DA2
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254E2E
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254E4A
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254E66
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8254F7E
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8254F9A
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8254FB6
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8254FD2
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8254FEE
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x825500A
		.byte 0x31// ABILITY_FLAME_BODY

		.org 0x8255026
		.byte 0x31// ABILITY_FLAME_BODY

		.org 0x825521E
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x825523A
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x82552FE
		.byte 0x4// ABILITY_BATTLE_ARMOR

		.org 0x825531A
		.byte 0x4// ABILITY_BATTLE_ARMOR

		.org 0x8255352
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x825536E
		.byte 0xD// ABILITY_CLOUD_NINE

		.org 0x8255432
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x825544E
		.byte 0x6// ABILITY_DAMP

		.org 0x825546A
		.byte 0x6// ABILITY_DAMP

		.org 0x825554A
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x8255566
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x825567E
		.byte 0x3E// ABILITY_GUTS

		.org 0x8255796
		.byte 0x31// ABILITY_FLAME_BODY

		.org 0x82557B2
		.byte 0x3F// ABILITY_MARVEL_SCALE

		.org 0x82557CE
		.byte 0x3F// ABILITY_MARVEL_SCALE

		.org 0x8255892
		.byte 0x12// ABILITY_FLASH_FIRE

		.org 0x82558AE
		.byte 0x12// ABILITY_FLASH_FIRE

		.org 0x82558CA
		.byte 0x12// ABILITY_FLASH_FIRE

		.org 0x8255A36
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8255A52
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8255B32
		.byte 0x39// ABILITY_PLUS

		.org 0x8255B4E
		.byte 0x39// ABILITY_PLUS

		.org 0x8255B6A
		.byte 0x39// ABILITY_PLUS

		.org 0x8255BF6
		.byte 0x2// ABILITY_DRIZZLE

		.org 0x8255C82
		.byte 0x30// ABILITY_EARLY_BIRD

		.org 0x8255C9E
		.byte 0x30// ABILITY_EARLY_BIRD

		.org 0x8255D2A
		.byte 0x27// ABILITY_INNER_FOCUS

		.org 0x8255E42
		.byte 0x11// ABILITY_IMMUNITY

		.org 0x8255EB2
		.byte 0x16// ABILITY_INTIMIDATE

		.org 0x8255FAE
		.byte 0x2F// ABILITY_THICK_FAT

		.org 0x8255FCA
		.byte 0x2F// ABILITY_THICK_FAT

		.org 0x825603A
		.byte 0xF// ABILITY_INSOMNIA

		.org 0x8256056
		.byte 0x29// ABILITY_WATER_VEIL

		.org 0x82560C6
		.byte 0x6// ABILITY_DAMP

		.org 0x82560E2
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x82560FE
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x825616E
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x82561C2
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x82561DE
		.byte 0x48// ABILITY_VITAL_SPIRIT

		.org 0x8256286
		.byte 0x8// ABILITY_SAND_VEIL

		.org 0x8256382
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x825639E
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x82563BA
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x82563D6
		.byte 0x6// ABILITY_DAMP

		.org 0x82563F2
		.byte 0x6// ABILITY_DAMP

		.org 0x825640E
		.byte 0x6// ABILITY_DAMP

		.org 0x825649A
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x825650A
		.byte 0xE// ABILITY_COMPOUND_EYES

		.org 0x8256526
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x8256542
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x825655E
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x8256606
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8256622
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8256692
		.byte 0x2C// ABILITY_RAIN_DISH

		.org 0x8256756
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x825696A
		.byte 0x3A// ABILITY_MINUS

		.org 0x8256986
		.byte 0x3A// ABILITY_MINUS

		.org 0x82569BE
		.byte 0xA// ABILITY_VOLT_ABSORB

		.org 0x8256A66
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x8256A82
		.byte 0x3// ABILITY_SPEED_BOOST

		.org 0x8256A9E
		.byte 0x2E// ABILITY_PRESSURE

		.org 0x8256ABA
		.byte 0x2E// ABILITY_PRESSURE

		.org 0x8256AD6
		.byte 0x14// ABILITY_OWN_TEMPO

		.org 0x8256B0E
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x8256BD2
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8256BEE
		.byte 0xB// ABILITY_WATER_ABSORB

		.org 0x8256C0A
		.byte 0xD// ABILITY_CLOUD_NINE

		.org 0x8256C26
		.byte 0xD// ABILITY_CLOUD_NINE

		.org 0x8256D92
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8256DAE
		.byte 0x21// ABILITY_SWIFT_SWIM

		.org 0x8256DE6
		.byte 0x38// ABILITY_CUTE_CHARM

		.org 0x8256F52
		.byte 0xC// ABILITY_OBLIVIOUS

		.org 0x8256F6E
		.byte 0xC// ABILITY_OBLIVIOUS

		.org 0x8256F8A
		.byte 0xC// ABILITY_OBLIVIOUS

		.org 0x8256FC2
		.byte 0x29// ABILITY_WATER_VEIL

		.org 0x8256FFA
		.byte 0x5// ABILITY_STURDY

		.org 0x82570DA
		.byte 0x5// ABILITY_STURDY

		.org 0x82571F2
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x825720E
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x825722A
		.byte 0x4B// ABILITY_SHELL_ARMOR

		.org 0x825737A
		.byte 0x32// ABILITY_RUN_AWAY

		.org 0x82573B2
		.byte 0x3E// ABILITY_GUTS

		.org 0x82573CE
		.byte 0x3E// ABILITY_GUTS

		.org 0x82573EA
		.byte 0x3E// ABILITY_GUTS

		.org 0x8257476
		.byte 0x2B// ABILITY_SOUNDPROOF

		.org 0x8257492
		.byte 0x2B// ABILITY_SOUNDPROOF

