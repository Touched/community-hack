// Battle backgrounds insert

	// Normal grass BG
		.org 0x0824EE34
		.word grass_bgTiles
		.pool

		.org 0x0824EE38
		.word grass_bgMap
		.pool

		.org 0x0824EE3C
		.word grass_entryTiles
		.pool

		.org 0x0824EE40
		.word grass_entryMap
		.pool

		.org 0x08248400
		.incbin "build/src/bgpals/grass_pal.bin"

	// Long grass BG
		.org 0x0824EE48
		.word longgrass_bgTiles
		.pool

		.org 0x0824EE4C
		.word longgrass_bgMap
		.pool

		.org 0x0824EE50
		.word longgrass_entryTiles
		.pool

		.org 0x0824EE54
		.word longgrass_entryMap
		.pool

		.org 0x08249074
		.incbin "build/src/bgpals/longgrass_pal.bin"

	// Sand BG
		.org 0x0824EE5C
		.word sand_bgTiles
		.pool

		.org 0x0824EE60
		.word sand_bgMap
		.pool

		.org 0x0824EE64
		.word sand_entryTiles
		.pool

		.org 0x0824EE68
		.word sand_entryMap
		.pool

		.org 0x08249F98
		.incbin "build/src/bgpals/sand_pal.bin"

	// Dive BG
		//.org 0x0824EE70
		//.word dive_bgTiles
		//.pool

		//.org 0x0824EE74
		//.word dive_bgMap
		//.pool

		//.org 0x0824EE78
		//.word dive_entryTiles
		//.pool

		//.org 0x0824EE7C
		//.word dive_entryMap
		//.pool

		//.org 0x0824A940
		//.incbin "build/src/bgpals/dive_pal.bin"

	// Sea BG
		.org 0x0824EE84
		.word sea_bgTiles
		.pool

		.org 0x0824EE88
		.word sea_bgMap
		.pool

		.org 0x0824EE8C
		.word sea_entryTiles
		.pool

		.org 0x0824EE90
		.word sea_entryMap
		.pool

		.org 0x0824B19C
		.incbin "build/src/bgpals/sea_pal.bin"

	// Pond BG
		//.org 0x0824EE98
		//.word pond_bgTiles
		//.pool

		//.org 0x0824EE9C
		//.word pond_bgMap
		//.pool

		//.org 0x0824EEA0
		//.word pond_entryTiles
		//.pool

		//.org 0x0824EEA4
		//.word pond_entryMap
		//.pool

		//.org 0x0824BCE0
		//.incbin "build/src/bgpals/pond_pal.bin"

	// Mountain BG
		.org 0x0824EEAC
		.word mountain_bgTiles
		.pool

		.org 0x0824EEB0
		.word mountain_bgMap
		.pool

		.org 0x0824EEB4
		.word mountain_entryTiles
		.pool

		.org 0x0824EEB8
		.word mountain_entryMap
		.pool

		.org 0x0824C5D8
		.incbin "build/src/bgpals/mountain_pal.bin"

	// Cave BG
		.org 0x0824EEC0
		.word cave_bgTiles
		.pool

		.org 0x0824EEC4
		.word cave_bgMap
		.pool

		.org 0x0824EEC8
		.word cave_entryTiles
		.pool

		.org 0x0824EECC
		.word cave_entryMap
		.pool

		.org 0x0824CF98
		.incbin "build/src/bgpals/cave_pal.bin"

	// Unknown BG
		//.org 0x0824EED4
		//.word unknown_bgTiles
		//.pool

		//.org 0x0824EED8
		//.word unknown_bgMap
		//.pool

		//.org 0x0824EEDC
		//.word unknown_entryTiles
		//.pool

		//.org 0x0824EEE0
		//.word unknown_entryMap
		//.pool

		//.org 0x0824DDF0
		//.incbin "build/src/bgpals/unknown_pal.bin"

	// Field BG
		.org 0x0824EEE8
		.word field_bgTiles
		.pool

		.org 0x0824EEEC
		.word field_bgMap
		.pool

		.org 0x0824EEF0
		.word field_entryTiles
		.pool

		.org 0x0824EEF4
		.word field_entryMap
		.pool

		.org 0x0824E81C
		.incbin "build/src/bgpals/field_pal.bin"

	// Trainer Link BG
		//.org 0x0824EEFC
		//.word link_bgTiles
		//.pool

		//.org 0x0824EF00
		//.word link_bgMap
		//.pool

		//.org 0x0824EF04
		//.word link_entryTiles
		//.pool

		//.org 0x0824EF08
		//.word link_entryMap
		//.pool

		//.org 0x0824E528
		//.incbin "build/src/bgpals/link_pal.bin"

	// Trainer Gym BG
		.org 0x0824EF10
		.word gym_bgTiles
		.pool

		.org 0x0824EF14
		.word gym_bgMap
		.pool

		.org 0x0824EF18
		.word gym_entryTiles
		.pool

		.org 0x0824EF1C
		.word gym_entryMap
		.pool

		.org 0x0824E56C
		.incbin "build/src/bgpals/gym_pal.bin"

	// Trainer Class_x54 BG
		//.org 0x0824EF24
		//.word x54_bgTiles
		//.pool

		//.org 0x0824EF28
		//.word x54_bgMap
		//.pool

		//.org 0x0824EF2C
		//.word x54_entryTiles
		//.pool

		//.org 0x0824EF30
		//.word x54_entryMap
		//.pool

		//.org 0x0824E5B8
		//.incbin "build/src/bgpals/x54_pal.bin"

	// Trainer Battle type3 BG
		//.org 0x0824EF38
		//.word type3_bgTiles
		//.pool

		//.org 0x0824EF3C
		//.word type3_bgMap
		//.pool

		//.org 0x0824EF40
		//.word type3_entryTiles
		//.pool

		//.org 0x0824EF44
		//.word type3_entryMap
		//.pool

		//.org 0x0824E604
		//.incbin "build/src/bgpals/type3_pal.bin"

	// Trainer Team Rocket BG
		//.org 0x0824EF4C
		//.word teamRbgTiles
		//.pool

		//.org 0x0824EF50
		//.word teamRbgMap
		//.pool

		//.org 0x0824EF54
		//.word teamRentryTiles
		//.pool

		//.org 0x0824EF58
		//.word teamRentryMap
		//.pool

		//.org 0x0824E650
		//.incbin "build/src/bgpals/teamR_pal.bin"

	// E4 one BG
		//.org 0x0824EF60
		//.word E41_bgTiles
		//.pool

		//.org 0x0824EF64
		//.word E41_bgMap
		//.pool

		//.org 0x0824EF68
		//.word E41_entryTiles
		//.pool

		//.org 0x0824EF6C
		//.word E41_entryMap
		//.pool

		//.org 0x0824E6A4
		//.incbin "build/src/bgpals/E41_pal.bin"

	// E4 two BG
		//.org 0x0824EF74
		//.word E42_bgTiles
		//.pool

		//.org 0x0824EF78
		//.word E42_bgMap
		//.pool

		//.org 0x0824EF7C
		//.word E42_entryTiles
		//.pool

		//.org 0x0824EF80
		//.word E42_entryMap
		//.pool

		//.org 0x0824E6F0
		//.incbin "build/src/bgpals/E42_pal.bin"

	// E4 three BG
		//.org 0x0824EF88
		//.word E43_bgTiles
		//.pool

		//.org 0x0824EF8C
		//.word E43_bgMap
		//.pool

		//.org 0x0824EF90
		//.word E43_entryTiles
		//.pool

		//.org 0x0824EF94
		//.word E43_entryMap
		//.pool

		//.org 0x0824E740
		//.incbin "build/src/bgpals/E43_pal.bin"

	// E4 four BG
		//.org 0x0824EF9C
		//.word E44_bgTiles
		//.pool

		//.org 0x0824EFA0
		//.word E44_bgMap
		//.pool

		//.org 0x0824EFA4
		//.word E44_entryTiles
		//.pool

		//.org 0x0824EFA8
		//.word E44_entryMap
		//.pool

		//.org 0x0824E78C
		//.incbin "build/src/bgpals/E44_pal.bin"

	// Champion  BG
		//.org 0x0824EFB0
		//.word championbgTiles
		//.pool

		//.org 0x0824EFB4
		//.word championbgMap
		//.pool

		//.org 0x0824EFB8
		//.word championentryTiles
		//.pool

		//.org 0x0824EFBC
		//.word championentryMap
		//.pool

		//.org 0x0824E7DC
		//.incbin "build/src/bgpals/champion_pal.bin"


