#include "pokedex.h"
#include "generated/images/pokepad/icons/pokedex.h"

#define POKEPAD_POKEDEX_ICON_BASE_PAL_TAG 0xD000
#define POKEPAD_POKEDEX_ICON_BASE_GRAY_PAL_TAG 0xE000
#define POKEPAD_POKEDEX_ICON_BASE_TILE_TAG 0xD000
#define POKEPAD_POKEDEX_CURSOR_TAG 0x3000

static const struct OamData cursor_oam = {
    .size = 2,
    .priority = 0,
};

/* Only load the first two frames of the cursor */
static const struct SpriteTiles cursor_tiles = {
    storage_hand_tiles,
    sizeof(storage_hand_tiles) / 4 * 2,
    POKEPAD_POKEDEX_CURSOR_TAG,
};

static const struct SpritePalette cursor_palette = {
    storage_hand_palette,
    POKEPAD_POKEDEX_CURSOR_TAG,
};

static void oac_cursor(struct Object* obj)
{
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;
    obj->pos1.x = POKEDEX_ICON_X(state->cursor.x);
    obj->pos1.y = POKEDEX_ICON_Y(state->cursor.y) - 8;
}

static u8 load_cursor(void) {
    const struct Template cursor_template = {
        POKEPAD_POKEDEX_CURSOR_TAG,
        POKEPAD_POKEDEX_CURSOR_TAG,
        &cursor_oam,
        storage_hand_template.animation,
        NULL,
        SPRITE_NO_ROTSCALE,
        oac_cursor,
    };

    gpu_tile_obj_alloc_tag_and_upload(&cursor_tiles);
    gpu_pal_obj_alloc_tag_and_apply(&cursor_palette);
    return template_instanciate_forward_search(&cursor_template, 0, 0, 0);
}

static const struct OamData icon_oam = {
    .size = 2,
    .priority = 1,
};

static const struct Template pokedex_icon_template_base = {
    0,
    0,
    &icon_oam,
    SPRITE_NO_ANIMATION,
    NULL,
    SPRITE_NO_ROTSCALE,
    oac_nullsub,
};

static void load_icon_template(enum PokemonSpecies species, bool grayscale,
                               struct Template* template)
{
    /* Unown have a separate icon for each form */
    u32 pid = 0;
    if (species == SPECIES_UNOWN) {
        pid = saveblock2->unown_pid;
    }

    /* Load first frame of the Pokemon icon */
    struct SpriteTiles tiles = {
        .data = load_party_icon_tiles_with_form(species, pid, true),
        .size = POKEAGB_POKEMON_ICON_SIZE / 2,
        .tag = POKEPAD_POKEDEX_ICON_BASE_TILE_TAG + species,
    };

    u8 palette_index = load_party_icon_palette_index(species);

    /* Build palette */
    u16 palette_data[16];
    u16 palette_tag_base = grayscale ? POKEPAD_POKEDEX_ICON_BASE_GRAY_PAL_TAG
        : POKEPAD_POKEDEX_ICON_BASE_PAL_TAG;

    memcpy(palette_data, pokeicon_pals[palette_index], sizeof(palette_data));

    if (grayscale) {
        for (u8 i = 0; i < 16; i++) {
            struct ColorComponents* color = (struct ColorComponents*) &palette_data[i];

            u16 lum = (color->r * 0x4C + color->g * 0x97 + color->b * 0x1D) >> 8;

            union Color gray = {
                .components = {
                    .r = lum,
                    .g = lum,
                    .b = lum,
                }
            };

            palette_data[i] = gray.packed;
        }
    }

    struct SpritePalette palette = {
        .data = palette_data,
        .tag = palette_tag_base + palette_index,
    };

    /* Build the template */
    memcpy(template, &pokedex_icon_template_base, sizeof(struct Template));
    template->tiles_tag = tiles.tag;
    template->pal_tag = palette.tag;

    /* Upload */
    gpu_tile_obj_alloc_tag_and_upload(&tiles);
    gpu_pal_obj_alloc_tag_and_apply(&palette);
}

static u8 load_icon(u16 index, s16 x, s16 y)
{
    struct Template* template = malloc(sizeof(struct Template));
    u16 xpos = POKEDEX_ICON_X(x),
        ypos = POKEDEX_ICON_Y(y);

    u16 dex_index = pokedex_order[index];
    enum PokemonSpecies species = pokedex_index_to_species(dex_index);
    load_icon_template(species, false, template);

    /* if (dex_flag(index, DEX_FLAG_CHECK_CAUGHT, false)) { */
    /*     id = pokepad_pokedex_load_icon(species, false, xpos, ypos, 0); */
    /* } else if (dex_flag(index, DEX_FLAG_CHECK_SEEN, false)) { */
    /*     id = pokepad_pokedex_load_icon(species, true, xpos, ypos, 0); */
    /* } else { */
    /*     id = pokepad_pokedex_load_icon(SPECIES_MISSINGNO, false, xpos, ypos, 0); */
    /* } */

    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;

    u8 id = template_instanciate_forward_search(template, xpos, ypos, 0);

    struct PokedexIconState* icon = (struct PokedexIconState*) objects[id].private;
    icon->id = id;
    icon->index = dex_index;
    icon->x = x;
    icon->y = y;

    if (index >= state->last_index) {
        objects[id].bitfield2 |= 4;
    }

    return id;
}

static void load_icon_grid(void)
{
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;

    u8 i = 0;
    for (u8 y = 0; y < POKEDEX_GRID_HEIGHT; y++) {
        for (u8 x = 0; x < POKEDEX_GRID_WIDTH; x++) {
            state->icons[i] = load_icon(i + state->index, x, y);
            i++;
        }
    }
}

static void task_scroll_icons(u8 id)
{
    struct Task* task = &tasks[id];
    struct PokedexScrollState* scroll = (struct PokedexScrollState*) task->priv;

    /* Scroll exisiting icons up */
    if (scroll->row < scroll->rows) {

        for (u8 i = 0; i < scroll->icon_count; i++) {
            struct Object* obj = &objects[scroll->icons[i]];
            struct PokedexIconState* icon = (struct PokedexIconState*) obj->private;
            obj->pos1.y += scroll->shift * scroll->direction;
        }

        scroll->counter += scroll->shift;

        if (scroll->counter >= POKEDEX_ICON_HEIGHT) {
            scroll->row++;
            scroll->counter = 0;

            for (u8 i = 0; i < scroll->icon_count; i++) {
                if (scroll->icons[i] == 0xFF) {
                    continue;
                }

                struct Object* obj = &objects[scroll->icons[i]];
                struct PokedexIconState* icon = (struct PokedexIconState*) obj->private;

                icon->y += scroll->direction;

                /* Cull objects that go offscreen in the direction of scrolling */
                bool cull = (scroll->direction < 0 && icon->y < 0) ||
                    (scroll->direction > 0 && icon->y >= POKEDEX_GRID_HEIGHT);

                if (cull) {
                    scroll->icons[i] = 0xFF;
                    gpu_tile_obj_free_by_tag(obj->template->tiles_tag);
                    obj_delete(obj);
                }
            }
        }
    } else {
        /* Update the object ID array */
        struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;
        for (u8 a = 0, b = 0; b < scroll->icon_count; b++) {
            if (scroll->icons[b] != 0xFF) {
                state->icons[a++] = scroll->icons[b];
            }
        }

        /* Clean up */
        free(scroll->icons);
        task_del(id);
    }
}

static void update_info(void)
{
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;

    u16 selected = state->cursor.y * POKEDEX_GRID_WIDTH + state->cursor.x;

    if (selected != state->cursor.selected) {
        /* TODO: Update */
        state->cursor.selected = selected;
    }
}

static void change_page(u8 rows, s8 direction, u8 speed)
{
    if (task_is_running(task_scroll_icons)) {
        return;
    }

    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;
    s16 shift_amount = POKEDEX_GRID_WIDTH * rows * -direction;

    /* Determine maximum possible rows that can be shifted while
     * maintain a valid index */
    if (direction > 0) {
        while (state->index + shift_amount < 0 && rows > 0) {
            rows--;
            shift_amount = POKEDEX_GRID_WIDTH * rows * -direction;
        }
    } else if (direction < 0) {
        while (state->index + POKEDEX_ICONS - POKEDEX_GRID_WIDTH + shift_amount >= state->last_index && rows > 0) {
            rows--;
            shift_amount = POKEDEX_GRID_WIDTH * rows * -direction;
        }
    } else {
        /* Invalid direction */
        return;
    }

    if (rows == 0) {
        return;
    }

    state->index += shift_amount;

    u8 task_id = task_add(task_scroll_icons, 0xA);
    struct Task* task = &tasks[task_id];
    struct PokedexScrollState* scroll = (struct PokedexScrollState*) task->priv;

    /* Initialize scroll state */
    scroll->icon_count = POKEDEX_ICONS + rows * POKEDEX_GRID_WIDTH;
    scroll->icons = malloc(scroll->icon_count * sizeof(u8));
    scroll->direction = direction;
    scroll->shift = speed;
    scroll->rows = rows;
    scroll->row = 0;
    scroll->counter = 0;

    /* Copy existing icons into the buffer */
    memcpy(scroll->icons, state->icons, sizeof(state->icons));
    u8* buffer = scroll->icons + sizeof(state->icons);

    /* Load additional icons into the buffer */
    for (u8 i = 0, y = 0; y < rows; y++) {
        for (u8 x = 0; x < POKEDEX_GRID_WIDTH; x++, i++) {
            u16 index;
            u8 id;

            /* Determine which icons to load and where */
            if (direction < 0) {
                index = i + state->index + POKEDEX_ICONS - POKEDEX_GRID_WIDTH * rows;
                id = load_icon(index, x, y + POKEDEX_GRID_HEIGHT);
            } else if (direction > 0) {
                index = i + state->index;
                id = load_icon(index, x, -rows + y);
            }

            *buffer++ = id;
        }
    }

    update_info();
}

static void cursor_move(s8 x, s8 y) {
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;

    state->cursor.x += x;
    state->cursor.y += y;

    /* Horizontally wrap cursor */
    if (state->cursor.x < 0) {
        state->cursor.x = POKEDEX_GRID_WIDTH - 1;
    } else if (state->cursor.x >= POKEDEX_GRID_WIDTH) {
        state->cursor.x = 0;
    }

    /* Cursor should scroll by one row if it goes out of bounds vertically */
    if (state->cursor.y < 0) {
        state->cursor.y = 0;
        change_page(1, 1, POKEDEX_SCROLL_SPEED);
    } else if (state->cursor.y >= POKEDEX_GRID_HEIGHT) {
        state->cursor.y = POKEDEX_GRID_HEIGHT - 1;
        change_page(1, -1, POKEDEX_SCROLL_SPEED);
    }

    update_info();
}

static bool setup(u8* trigger)
{
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;

    switch (*trigger) {
    case 0:
        state = pokepad_state->app_state = malloc_and_clear(sizeof(struct PokepadPokedexState));
        state->last_index = 26; /* FIXME: Determine actual final index */
        *trigger = 1;
        break;

    case 1:
        load_cursor();
        *trigger = 2;
        break;

    case 2:
        load_icon_grid();
        *trigger = 2;

        return false;
    }

    return true;
}

static bool destroy(u8* trigger)
{
    return false;
}

static void callback(void)
{
    if (super.buttons.held & KEY_L) {
        /* Page Left */
        change_page(POKEDEX_GRID_HEIGHT, -1, POKEDEX_SCROLL_SPEED);
    } else if (super.buttons.held & KEY_R) {
        /* Page Right */
        change_page(POKEDEX_GRID_HEIGHT, 1, POKEDEX_SCROLL_SPEED);
    } else if (super.buttons.new & KEY_LEFT) {
        cursor_move(-1, 0);
    } else if (super.buttons.new & KEY_RIGHT) {
        cursor_move(1, 0);
    } else if (super.buttons.new & KEY_UP) {
        cursor_move(0, -1);
    } else if (super.buttons.new & KEY_DOWN) {
        cursor_move(0, 1);
    }
}

static bool unlocked(void)
{
    /* FIXME: Check Pokedex flag */
    return true;
}

static const pchar description[] = _"An application that records Pokémon\nsecrets "
    "upon meeting or catching them.";

const struct PokepadApplication pokepad_application_pokedex = {
    .name = _"Pokédex",
    .description = description,
    .role = POKEPAD_APP_EXTENSION,
    .icon = POKEPAD_BUILD_APP_ICON(pokedex),
    .setup = setup,
    .destroy = destroy,
    .callback = callback,
    .unlocked = unlocked,
};
