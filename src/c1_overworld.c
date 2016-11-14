#include <pokeagb/pokeagb.h>

static u32** ScriptAddr = (u32**)0x203C0D0;

/* 
    This is hooked into from C1_Overworld for the basis to add extra features without hooking for each
    TODO: Berries

*/
void c1_overworld_plus() {

    if (super.callback2 != (SuperCallback)c2_overworld) {
        return;
    }

    // script queue
    if ((!script_env_2_is_enabled()) && (var_load(0x407B))) {
        script_env_init_script((void*)(*ScriptAddr));
        var_set(0x407B, 0x0);
    }
    
    c1_overworld_normal(super.buttons.new_remapped, super.buttons.held_remapped);
    return;   
}


/*      ADDED FUNCTIONALITIES:

    Script queue. Executes a script whenever possible if var 0x407B is set and 0203c0d0 holds a ptr to the script

*/
