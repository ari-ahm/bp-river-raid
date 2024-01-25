#include "utils/window.h"
#include "scenes/scenes.h"

int main()
{

    if (init_window())
    {
        return 0;
    }

    int lvl = 0;
    int is_looping = 1;
    while (is_looping)
    {
        if (!is_music_playing())
            start_music("assets/intro.mp3");
        int mode = show_menu(get_renderer());
        switch (mode)
        {
        case 0:
            free_music();
            // openOutputRecord();
            // time_t tm = time(NULL);
            // srand(tm);
            // int score = run_game(keyboard_input, lvl);
            // writeStats(lvl, tm, score, "bobby_marins");
            // closeIORecord();
            break;
        case 1:
            int ret = show_level_selector(get_renderer());
            if (ret == -2)
            {
                is_looping = 0;
            }
            if (ret != -1)
                lvl = ret;
            break;
        // case 2:
        //     int runRecord = show_records();
        //     if (runRecord)
        //     {
        //         free_music();
        //         run_game(record_input, runRecord - 1);
        //         closeIORecord();
        //     }
        //     break;
        case 3:
            is_looping = 0;
        }
    }

    free_music();
    free_window();
    return 0;
}