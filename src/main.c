#include "utils/window.h"
#include "scenes/scenes.h"

int main() {
    
    if (init_window()) {
        return 0;
    }

    while (1)
    {
        int mode = show_menu(get_renderer());
        switch (mode)
        {
        // case 0:
        //     openOutputRecord();
        //     time_t tm = time(NULL);
        //     srand(tm);
        //     int score = run_game(keyboard_input, lvl);
        //     writeStats(lvl, tm, score, "bobby_marins");
        //     closeIORecord();
        //     break;
        // case 1:
        //     lvl = show_level_selector();
        //     break;
        // case 2:
        //     int runRecord = show_records();
        //     if (runRecord)
        //     {
        //         run_game(record_input, runRecord - 1);
        //         closeIORecord();
        //     }
        //     break;
        case 3:
            free_window();
            return 0;
        }
    }

    return 0;
}