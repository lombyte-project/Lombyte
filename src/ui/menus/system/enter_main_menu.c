#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/globals.h"

void enter_main_menu(void) {
    menu_system.close_request = 0;
    menu_system.unk10 = 0;
    game_mode = 3;
    menu_system.state = 0x2D;
    menu_system.update_count = 0;
}
