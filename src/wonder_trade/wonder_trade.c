#include <pokeagb/pokeagb.h>

#define BAN_LIST_COUNT 23
static const u16 species_ban_list [BAN_LIST_COUNT] = {
    SPECIES_MISSINGNO,
    SPECIES_ARTICUNO,
    SPECIES_ZAPDOS,
    SPECIES_MOLTRES,
    SPECIES_MEWTWO,
    SPECIES_MEW,
    SPECIES_RAIKOU,
    SPECIES_ENTEI,
    SPECIES_SUICUNE,
    SPECIES_LUGIA,
    SPECIES_HOOH,
    SPECIES_CELEBI,
    SPECIES_REGIROCK,
    SPECIES_REGICE,
    SPECIES_REGISTEEL,
    SPECIES_KYOGRE,
    SPECIES_GROUDON,
    SPECIES_RAYQUAZA,
    SPECIES_LATIAS,
    SPECIES_LATIOS,
    SPECIES_JIRACHI,
    SPECIES_DEOXYS,
    SPECIES_MAX
};

static const pchar trainer_names[50][10] = {
    _"James",
    _"Christoph",
    _"Ronald",
    _"Mary",
    _"Lisa",
    _"Michelle",
    _"John",
    _"Darry",
    _"Anthony",
    _"Patricia",
    _"Nancy",
    _"Laura",
    _"Robert",
    _"Paul",
    _"Kevin",
    _"Linda",
    _"Karen",
    _"Sarah",
    _"Michael",
    _"Mark",
    _"Jason",
    _"Barbara",
    _"Betty",
    _"Kimberly",
    _"William",
    _"Donald",
    _"Jeff",
    _"Elizabeth",
    _"Helen",
    _"Deborah",
    _"David",
    _"George",
    _"Jennifer",
    _"Sandra",
    _"Richard",
    _"Kenneth",
    _"Maria",
    _"Donna",
    _"Charles",
    _"Steven",
    _"Susan",
    _"Carol",
    _"Joseph",
    _"Edward",
    _"Margaret",
    _"Ruth",
    _"Thomas",
    _"Brian",
    _"Dorothy",
    _"Sharon",
};

/* TODO: Update after insertion of these species */
#define SPECIES_HONCHKROW 0
#define SPECIES_SHINX 0
#define SPECIES_LUXIO 0
#define SPECIES_LUXRAY 0
#define SPECIES_TOGEKISS 0
#define SPECIES_LUCARIO 0
#define SPECIES_RIOLU 0
#define SPECIES_GLACEON 0
#define SPECIES_GLISCOR 0
#define SPECIES_PUMPKABOO 0
#define SPECIES_GOURGEIST 0
#define SPECIES_WEAVILE 0
#define SPECIES_ELECRIVIRE 0
#define SPECIES_MUNCHLAX 0
#define SPECIES_BUDEW 0
#define SPECIES_ROSELIA 0
#define SPECIES_ROSERADE 0
#define SPECIES_MISMAGIUS 0
#define SPECIES_PETILIL 0
#define SPECIES_LILLIGANT 0


struct hack_members {
    pchar name[7];
    u16 species_claimed;
    pchar nick[10];
    u8 pokeball_type;
};

#define CLAIMS_COUNT 72

static const struct hack_members claims[CLAIMS_COUNT] = {
    {_"Harry", SPECIES_SHINX, _"", ITEM_LUXURYBALL},
    {_"Harry", SPECIES_LUXIO, _"", ITEM_LUXURYBALL},
    {_"Harry", SPECIES_LUXRAY, _"", ITEM_LUXURYBALL},   
    {_"Kostas", SPECIES_RIOLU, _"", ITEM_LUXURYBALL},
    {_"Kostas", SPECIES_LUCARIO, _"", ITEM_LUXURYBALL},  
    {_"Christos", SPECIES_SPHEAL, _"", ITEM_NETBALL},
    {_"FBI", SPECIES_SLOWPOKE, _"", ITEM_POKEBALL},
    {_"FBI", SPECIES_SLOWBRO, _"", ITEM_POKEBALL},
    {_"FBI", SPECIES_SLOWKING, _"", ITEM_POKEBALL},
    {_"Michael", SPECIES_GROWLITHE, _"", ITEM_GREATBALL},
    {_"Michael", SPECIES_ARCANINE, _"", ITEM_GREATBALL},
    {_"Daniel", SPECIES_LILEEP, _"Derp", ITEM_LUXURYBALL},
    {_"Daniel", SPECIES_CRADILY, _"Derp", ITEM_LUXURYBALL},
    {_"Sai", SPECIES_MURKROW, _"Moriarty", ITEM_LUXURYBALL},
    {_"Sai", SPECIES_HONCHKROW, _"Moriarty", ITEM_LUXURYBALL},
    {_"Haroun", SPECIES_TOGETIC, _"", ITEM_POKEBALL},
    {_"Uncomon", SPECIES_CYNDAQUIL, _"", ITEM_POKEBALL},
    {_"Uncomon", SPECIES_QUILAVA, _"", ITEM_POKEBALL},
    {_"Uncommo", SPECIES_TYPHLOSION, _"", ITEM_POKEBALL},  
    {_"Touched", SPECIES_JIGGLYPUFF, _"Jiggles", ITEM_LUXURYBALL},
    {_"Ben", SPECIES_GLISCOR, _"Achrobat", ITEM_LUXURYBALL},
    {_"Ddrox", SPECIES_CHINCHOU, _"Spark", ITEM_DIVEBALL},   
    {_"Ddrox", SPECIES_LANTURN, _"Spark", ITEM_DIVEBALL},
    {_"Mechakn", SPECIES_KANGASKHAN, _"Outrageous", ITEM_SAFARIBALL},     
    {_"Angel", SPECIES_GLACEON, _"Snow", ITEM_LUXURYBALL},
    {_"Denver", SPECIES_GLOOM, _"Lord Goomy", ITEM_DIVEBALL},    
    {_"itari", SPECIES_STARYU, _"", ITEM_DIVEBALL}, 
    {_"itari", SPECIES_STARMIE, _"", ITEM_DIVEBALL},   
    {_"Nina", SPECIES_RALTS, _"", ITEM_LUXURYBALL},   
    {_"Nina", SPECIES_KIRLIA, _"", ITEM_LUXURYBALL}, 
    {_"Nina", SPECIES_GARDEVOIR, _"", ITEM_LUXURYBALL},    
    {_"LAO TZU", SPECIES_ALAKAZAM, _"CONFUCIUS", ITEM_GREATBALL},   
    {_"LAO TZU", SPECIES_KADABRA, _"CONFUCIUS", ITEM_GREATBALL}, 
    {_"LAO TZU", SPECIES_ABRA, _"CONFUCIUS", ITEM_GREATBALL}, 
    {_"Summer", SPECIES_PICHU, _"", ITEM_POKEBALL},   
    {_"Kabocha", SPECIES_PUMPKABOO, _"", ITEM_LUXURYBALL},
    {_"Kabocha", SPECIES_GOURGEIST, _"", ITEM_LUXURYBALL},  
    {_"Banhamr", SPECIES_SQUIRTLE, _"Bowser", ITEM_POKEBALL},   
    {_"ElHeroe", SPECIES_MAGIKARP, _"BASEDGOD", ITEM_POKEBALL},
    {_"Josh", SPECIES_JUMPLUFF, _"Cotton", ITEM_PREMIERBALL},
    {_"Adriana", SPECIES_SHELLDER, _"Tight", ITEM_DIVEBALL},
    {_"Banhamr", SPECIES_CLOYSTER, _"Bowser", ITEM_POKEBALL},
    {_"Joe", SPECIES_SNEASEL, _"", ITEM_LUXURYBALL},
    {_"Joe", SPECIES_WEAVILE, _"", ITEM_LUXURYBALL},
    {_"Delta", SPECIES_ELEKID, _"Paul", ITEM_POKEBALL},
    {_"Delta", SPECIES_ELECTABUZZ, _"Paul", ITEM_POKEBALL},
    {_"Delta", SPECIES_ELECRIVIRE, _"Paul", ITEM_POKEBALL},
    {_"Lycan", SPECIES_ZANGOOSE, _"Rezzi", ITEM_POKEBALL}, // DUSKBALL
    {_"Rabinov", SPECIES_VOLTORB, _"Triggered", ITEM_POKEBALL}, // DUSKBALL
    {_"Rabinov", SPECIES_ELECTRODE, _"Triggered", ITEM_POKEBALL}, // DUSKBALL
    {_"tkyodft", SPECIES_FEEBAS, _"Mizu", ITEM_POKEBALL}, // LOVE BALL
    {_"tkyodft", SPECIES_MILOTIC, _"Mizu", ITEM_POKEBALL}, // LOVE BALL
    {_"Wobb", SPECIES_WYNAUT, _"Wub", ITEM_POKEBALL}, // FRIEND BALL
    {_"Wobb", SPECIES_WOBBUFFET, _"Wub", ITEM_POKEBALL}, // FRIEND BALL   
    {_"Janna", SPECIES_MAREEP, _"Janna", ITEM_POKEBALL}, // LOVE BALL    
    {_"Crizzle", SPECIES_SANDSHREW, _"Chris", ITEM_GREATBALL},
    {_"bnb", SPECIES_SANDSLASH, _"Locomoco", ITEM_NETBALL},
    {_"Tommy", SPECIES_UMBREON, _"Moony", ITEM_POKEBALL}, // MOON BALL
    {_"Sojourn", SPECIES_NIDORANM, _"Minos", ITEM_POKEBALL}, // DUSKBALL
    {_"Sojourn", SPECIES_NIDORINO, _"Minos", ITEM_POKEBALL},
    {_"Sojourn", SPECIES_NIDOKING, _"Minos", ITEM_POKEBALL},
    {_"Rika", SPECIES_PIKACHU, _"Rikachu", ITEM_PREMIERBALL},
    {_"Rika", SPECIES_RAICHU, _"Rikachu", ITEM_PREMIERBALL},
    {_"Fish", SPECIES_BUDEW, _"", ITEM_POKEBALL}, // FRIEND BALL
    {_"Fish", SPECIES_ROSELIA, _"", ITEM_POKEBALL}, // FRIEND BALL
    {_"Fish", SPECIES_ROSERADE, _"", ITEM_POKEBALL}, // FRIEND BALL
    {_"Deo", SPECIES_MISMAGIUS, _"Magicackle", ITEM_LUXURYBALL}, // MOON BALL
    {_"Deo", SPECIES_MISDREAVUS, _"Magicackle", ITEM_LUXURYBALL}, // MOON BALL
    {_"Rudy", SPECIES_NUMEL, _"Vesuvius", ITEM_POKEBALL},
    {_"Rudy", SPECIES_CAMERUPT, _"Vesuvius", ITEM_POKEBALL},
    {_"Doesnt", SPECIES_LILLIGANT, _"Anna", ITEM_POKEBALL},
    {_"Doesnt", SPECIES_PETILIL, _"Anna", ITEM_POKEBALL},
    {_"Mitch", SPECIES_GALLADE, _"HunterX", ITEM_POKEBALL},
};
 
u8 check_claimed(u16 species) {
    u8 i;
    for (i = 0; i < CLAIMS_COUNT; i++) {
        if (claims[i].species_claimed == species) {
            return i;
        }
    }
    return 0xFF;
}

void set_claim_data(u8 member) {
    struct PokemonBase* pkmn = (struct PokemonBase*)&party_opponent[0];
    
    pokemon_setattr(pkmn, REQUEST_OT_NAME, &claims[member].name);
    pokemon_setattr(pkmn, REQUEST_POKEBALL, (void*)&claims[member].pokeball_type);
                    
    if (claims[member].nick[0] != 0xFF) {
        pokemon_setattr(pkmn, REQUEST_NICK, &claims[member].nick);
    }
}
 
void generate_trademon(u16 species, u8 lvl) {
    u32 PID = ((rand() << 16) | (rand()));
    u32 TID = ((rand() << 16) |  (rand()));
    u8 iv = 0xFF;
    // this is nothing, promise
    if ((species == SPECIES_SLOWPOKE)) {
        iv = 0x1F;
    }
    pokemon_make_full(  &party_opponent[0],
                        species,
                        lvl,
                        iv,
                        1,
                        (u32)&PID,
                        1,
                        (u32)&TID);
    u8 claiming_member = check_claimed(species);
    if (claiming_member != 0xFF) {
        set_claim_data(claiming_member);
    } else {
        pokemon_setattr((struct PokemonBase*)&party_opponent[0], REQUEST_OT_NAME, (void*)&trainer_names[rand() % 50]);
    }
    
    // absolutely nothing suspicous going on here. No favouritism at all 
    if ((iv == 0x1F) && (rand() % 3) && (lvl > 35)) {
        u16 i = 0xBB;
        pokemon_setattr((struct PokemonBase*)&party_opponent[0], REQUEST_HELD_ITEM, &i); 
    }
    recalculate_stats(&party_opponent[0]);
}

bool ban_list(u16 species) {
    u8 i;
    for (i = 0; i < BAN_LIST_COUNT; i++) {
        if (species == species_ban_list[i]) {
            return 1;
        }
    }
    return 0;
}


u16 get_bst(u16 species) {
    u16 bst =   pokemon_base_stats[species].hp + pokemon_base_stats[species].atk +
                pokemon_base_stats[species].def + pokemon_base_stats[species].spd +
                pokemon_base_stats[species].spatk + pokemon_base_stats[species].spdef;
    return bst;
}


u16 get_valid_species(u8 slot) {
    u16 species = pokemon_getattr((struct PokemonBase*)&party_player[slot], REQUEST_SPECIES, NULL);
    u16 player_bst = get_bst(species);
    u16 *valid_species = (u16*)malloc_and_clear(POKEAGB_POKEMON_SLOTS * 2);
    u16 i;
    u16 current_index = 0;
    
    // loop through all available pokemon and fill valid species array
    for (i = 1; i < POKEAGB_POKEMON_SLOTS; i++) {
        u16 bst = get_bst(i);    
        if (player_bst > bst) {
            bst = player_bst - bst;
        } else {
            bst = bst - player_bst;
        }
        
        // species is valid if stat difference is within range 30
        if (((bst < 20) && (species != i)) && (!ban_list(i))) {
            valid_species[current_index] = i;
            current_index += 1;
            u16* var = (u16*)0x203E000;
            *(var + current_index) = i;
        }
    }
    
    u16 target_species = valid_species[rand() % current_index];
    
    free(valid_species);
    return target_species;
}

void trade_name_buffering() {
    u16 slot = var_8004;
    memcpy((void*)&fcode_buffer4, (void*)&party_opponent[0].base.nick[0], 10);
    fcode_buffer4[10] = 0xFF;
    memcpy((void*)&fcode_buffer2, (void*)&party_opponent[0].base.otname[0], 7);
    fcode_buffer2[7] = 0xFF;
    memcpy((void*)&fcode_buffer3, (void*)&party_player[slot].base.nick[0], 10);
    fcode_buffer3[10] = 0xFF;
}

void to_trade(void) {
    // get good trade partner for offered 'mon
    u16 slot = var_8004;
    var_8005 = var_8004;
    u16 species = get_valid_species(slot);    
    // generate trademon
    generate_trademon(species, pokemon_getattr((struct PokemonBase*)&party_player[slot],
                     REQUEST_LEVEL, NULL));
    trade_pokemon();
    return;
}



