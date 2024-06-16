#include "game_variables.h"
#include "script_loader.h"
#include "render.h"

int nowMainMenuOpt = 0;
int nowDlgOpt = 0;
char *dir;

int nowBrcNum = 0;
int nowDlgNum = 0;
int brcNum = -1;
int dlgNum = -1;

int chrNum = -1;

character chr[10];

int main(int argc, char **argv)
{
    init_log();

    // check arguments
    if (argc != 2)
    {
        LOG_ERROR("Usage: ./engine [directory]");
        return 1;
    }
    LOG_INFO("Confirmed command line arguments.");

    // check if directory exists
    dir = argv[1];
    DIR *dp;
    errno = 0;
    if ((dp = opendir(dir)) == NULL)
    {
        switch (errno)
        {
        case EACCES:
            LOG_ERROR("Permission denied");
            break;
        case ENOENT:
            LOG_ERROR("Directory does not exist");
            break;
        case ENOTDIR:
            LOG_ERROR("'%s' is not a directory", argv[1]);
            break;
        }
        exit(EXIT_FAILURE);
    }
    closedir(dp);
    LOG_INFO("Confirmed access right of directory: '%s'", argv[1]);

    status = GAME_INITIALIZING;
    int ch = -1;

    // Game Loop
    while (status != GAME_EXITING)
    {
        if (status == GAME_INITIALIZING)
        {
            // Loading ncurses and main menu resources
            status = init_curses();
            status = init_script_loader(dir);
            if (status == GAME_INITIALIZING)
            {
                status = GAME_MAIN_MENU;
            }
        }
        else if (status == GAME_MAIN_MENU)
        {
            if (ch == -1)
            {
                status = render_main_menu("/assets/cover.bmp");
                ch = getch();
            }
            else
            {
                if (ch == 'q')
                {
                    status = end_main_menu();
                    status = GAME_ENDING;
                }

                else if (ch == KEY_UP)
                {
                    nowMainMenuOpt = (nowMainMenuOpt + MAIN_MENU_OPT_NUM - 1) % MAIN_MENU_OPT_NUM;
                    status = render_main_menu("img");
                    ch = getch();
                }
                else if (ch == KEY_DOWN)
                {
                    nowMainMenuOpt = (nowMainMenuOpt + 1) % MAIN_MENU_OPT_NUM;
                    status = render_main_menu("img");
                    ch = getch();
                }

                else if (nowMainMenuOpt == 0 && ch == '\n')
                {
                    if (nowMainMenuOpt == 0)
                    {
                        status = end_main_menu();
                        // status = load_player();
                        status = load_characters();
                        status = load_scenario();
                        status = load_branch(nowBrcNum);
                        status = GAME_PLAYING;
                    }
                }

                else if (nowMainMenuOpt == 3 && ch == '\n') {
                    status = end_main_menu();
                    status = GAME_ENDING;
                }
                else {
                    ch = getch();
                }
            }
        }
        else if (status == GAME_PLAYING)
        {
            dialogue nowDlg;
            status = load_dialogue(nowDlgNum, &nowDlg);
            status = render_playing_screen("img", nowDlg);
            ch = getch();
            if (ch == 'q') {
                status = end_playing_screen();
                status = GAME_ENDING;
            }
            if (nowDlg.type == TEXT) {
                if (ch == ' ' || ch == '\n') {
                    nowDlgNum++;
                }
                if (nowDlgNum == dlgNum) {
                    status = end_playing_screen();
                    status = GAME_ENDING;
                }
            }
            else if (nowDlg.type == OPT_CHO || nowDlg.type == OPT_FLA) {
                if (ch == KEY_UP) {
                    nowDlgOpt = (nowDlgOpt + nowDlg.optNum - 1) % nowDlg.optNum;
                }
                else if (ch == KEY_DOWN) {
                    nowDlgOpt = (nowDlgOpt + 1) % nowDlg.optNum;
                }
                else if (ch == '\n') {
                    nowDlgNum = 0;
                    if (nowDlg.type == OPT_CHO) {
                        nowBrcNum = nowDlg.optCho[nowDlgOpt].nxtBrc;
                        load_branch(nowBrcNum);
                        nowDlgOpt = 0;
                    }
                    else if (nowDlg.type == OPT_FLA) {
                        int nowChar = -1;
                        for (int i = 0; i < chrNum; i++) {
                            if (!strcmp(nowDlg.obj, chr[i].name)) {
                                nowChar = i;
                                break;
                            }
                        }
                        if (nowChar == -1) {
                            LOG_ERROR(" Object character not found");
                            status = GAME_ERROR;
                        }
                        chr[nowChar].favor += nowDlg.optFla[nowDlgOpt].val;
                        for (int i = 0; i < nowDlg.nxtBrcNum; i++) {
                            if (chr[nowChar].favor >= nowDlg.brc[i].lb && chr[nowChar].favor <= nowDlg.brc[i].ub) {
                                nowBrcNum = nowDlg.brc[i].nxtBrc;
                                load_branch(nowBrcNum);
                                nowDlgOpt = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (status == GAME_ENDING)
        {
            end_curses();
            end_script_loader();
            status = GAME_EXITING;
        }
        else if (status == GAME_ERROR)
        {
            end_curses();
            status = GAME_EXITING;
        }
    }
    // Game exiting
    return 0;
}