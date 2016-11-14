#include <pokeagb/pokeagb.h>

void task_delete_completed_textbox(u8 t_id) {
    if (!box_status_and_type) {
        textbox_close();
        task_del(t_id);
        script_env_disable();
    }
}

void msg_normal(pchar *str){
   textbox_fdecode_auto_and_task_add(str);
   box_status_and_type = 1;
   task_add((TaskCallback)task_delete_completed_textbox, 0x1);
   script_env_enable();
   return;

}