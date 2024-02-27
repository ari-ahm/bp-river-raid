#include "utils/window.h"
#include "scenes/scenes.h"
#include "game/base_game.h"
#include "utils/recordHandler.h"
#include <stdlib.h>
#include <time.h>

int main()
{

    if (init_window())
    {
        return 0;
    }

    srand(time(NULL));

    int lvl = 0;
    int is_looping = 1;
    int ret;
    while (is_looping)
    {
        if (!is_music_playing())
            start_music("assets/intro.mp3");
        int mode = show_menu(get_renderer());
        switch (mode)
        {
        case 0:
            char name[32] = "NO_NAME";
            ret = getname(get_renderer(), name);
            if (ret == -1)
            {
                is_looping = 0;
                break;
            }
            free_music();
            time_t starting_time = time(NULL);
            int score = run_game(get_renderer(), lvl);
            time_t ending_time = time(NULL);
            if (score < 0)
            {
                score = -score;
                is_looping = 0;
            }

            writeRecord(name, starting_time, score, ending_time - starting_time, lvl);
            break;
        case 1:
            ret = show_level_selector(get_renderer());
            if (ret == -2)
            {
                is_looping = 0;
            }
            if (ret != -1)
                lvl = ret;
            break;
        case 2:
            int runRecord = show_records(get_renderer());
            if (runRecord == -1)
            {
                is_looping = 0;
            }
            break;
        case 3:
            is_looping = 0;
        }
    }

    free_music();
    free_window();
    return 0;
}