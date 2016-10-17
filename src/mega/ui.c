/*
 * Mega Evolution User Interface.
 *
 * This improved interface takes over the fight menu. When you can
 * mega evolve, you will see the following
 *
 * | > FIGHT       BAG   |
 * |   POKEMON     RUN   |
 *
 * Upon selecting fight, you'll see the attacks menu with one change:
 *
 * | > MOVE 1       MOVE 3   |
 * |   MOVE 2       MOVE 4   |
 * |            \/           |
 *
 * The big red arrow (like you see on extended multi-choice), allows
 * you to select a second page of the attack menu. From this mega you
 * can select Mega evolution.
 *
 * |        /\        |
 * | > MEGA EVOLUTION |
 *
 * See
 * http://www.pokecommunity.com/showpost.php?p=8903237&postcount=692
 * for an example of scrolling multi choice boxes.
 */

#include <pokeagb/pokeagb.h>
#include "../battle.h"
#include "mega.h"

/* Use PP rbox */
#define HIJACKED_RBOX_ID 7

void mega_ui_menu_draw_activation_text(void)
{
    const pchar deactivate[] = _"Press A to\ndeactivate";
    const pchar activate[] = _"Press A to\nactivate";
    pchar* str = extension_state.mega_evolution->ui.activated[b_active_side] ? deactivate : activate;

    /* Draw to the same rbox that the "Switch which?" text is drawn to */
    battle_draw_rbox(str, 11);
}

void mega_ui_menu_draw(void)
{
    const pchar mega_evolve[] = _"Mega Evolve";

    /* Erase the move names */
    for (u8 i = 0; i < 4; i++) {
        rboxid_clear_pixels(3 + i, 0xee);
        rboxid_update(3 + i, 3);
    }

    rboxid_update(HIJACKED_RBOX_ID, 3);

    battle_menu_erase_cursor(move_index_chosen_per_side[b_active_side]);

    /* Draw the text underneath the icon */
    /* rboxes[HIJACKED_RBOX_ID].width = 10; */
    /* rboxes[HIJACKED_RBOX_ID].x = 6; */
    /* rboxes[HIJACKED_RBOX_ID].y = 57; */
    /* battle_draw_rbox(mega_evolve, HIJACKED_RBOX_ID); */
}

void mega_ui_menu_reset(void)
{
    /* Move the PP rbox back to it's original location */
    /* rboxes[HIJACKED_RBOX_ID].width = 3; */
    /* rboxes[HIJACKED_RBOX_ID].x = 21; */
    /* rboxes[HIJACKED_RBOX_ID].y = 55; */
}

const struct ScrollArrows arrows = {
    /* Arrow configuration */
    { { 2, 0x50, 0x75 },
      { 3, 0x50, 0x9B } },

    /* Arrow IDs */
    { 0, 1 },

    /* Tile and Palette tags */
    0,
    1,
};

#define KEY_PRESSED(x) (super.buttons.new_remapped & x)

void mega_ui_menu(void);

void mega_ui_menu_go_back(void)
{
    extension_state.mega_evolution->ui.page = 0;
    mega_ui_menu_reset();
    battle_menu_fight_draw();
    b_x[b_active_side] = mega_ui_menu;
}

void mega_ui_menu_second_page(void)
{
    struct MegaEvolutionUI* ui = &extension_state.mega_evolution->ui;

    if (KEY_PRESSED(KEY_UP)) {
        /* Page up */
        audio_play(SOUND_GENERIC_CLINK);
        mega_ui_menu_go_back();
    } else if (KEY_PRESSED(KEY_B)) {
        /* Go back to the main battle menu and cancel trigger if B is pressed */
        ui->page = 0;
        ui->activated[b_active_side] = 0;
        tasks[ui->arrow_task].function = textbox_task_delete_scroll_arrows;
        ui->loaded = false;
        mega_ui_menu_reset();

        /* This handles the B button */
        battle_fight_menu();
    } else if (KEY_PRESSED(KEY_A)) {
        /* Toggle mega evolution, then allow the player to select a move */
        audio_play(SOUND_GENERIC_CLINK);

        /* Play an additional sound effect for enabling Mega Evolution */
        if (!ui->activated[b_active_side]) {
            /* TODO: Get better sound effect(s) */
            audio_play(SOUND_DAMAGE_ICE_FLOOR);
        }

        ui->activated[b_active_side] ^= 1;
        mega_ui_menu_go_back();
    }
}

bool mega_ui_should_display(void)
{
    union MegaEvolutionEntry* entry = mega_find_for_pokemon(&battle_data[b_active_side]);
    enum Item item = mega_bank_find_keystone(b_active_side);

    /*
     * The UI should only display if the bank meets the following criteria:
     * - The Pokemon is capable of Mega Evolution (see mega_find_for_pokemon)
     * - The Pokemon is holding the required Mega Stone (see mega_find_for_pokemon)
     * - The Trainer has a keystone
     */
    return entry && entry->mega.variant != MEGA_TYPE_PRIMAL && item != ITEM_NONE;
}

void mega_ui_menu(void)
{
    struct MegaEvolutionUI* ui = &extension_state.mega_evolution->ui;

    /* Load the scroll indicator arrows */
    if (mega_ui_should_display() && !ui->loaded) {
        ui->arrow_task = textbox_spawn_scroll_arrows(&arrows, &ui->page);
        ui->loaded = true;
    }

    /* Only allow paging down if the scroll indicators were loaded */
    if (ui->loaded && KEY_PRESSED(KEY_DOWN)) {
        u8 index = move_index_chosen_per_side[b_active_side];

        /* We can only page down if we're on the bottom row of moves */
        if (index == 2 || index == 3) {
            ui->page = 1;
            audio_play(SOUND_GENERIC_CLINK);
            mega_ui_menu_draw_activation_text();
            mega_ui_menu_draw();
            b_x[b_active_side] = mega_ui_menu_second_page;
            return;
        }
    }

    /* Use the original fight menu */
    battle_fight_menu();

    /* Delete the scroll indicators if we're exiting the menu */
    if (ui->loaded && b_x[b_active_side] != mega_ui_menu) {
        tasks[ui->arrow_task].function = textbox_task_delete_scroll_arrows;
        ui->loaded = false;
    }
}
