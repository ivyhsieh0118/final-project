#include "render.h"

// Variables for main menu
WINDOW *wMainMenu = NULL;
WINDOW *wMainMenuOpts[MAIN_MENU_OPT_NUM] = {NULL};

// Variables for playing screen
WINDOW *wCharName = NULL;
WINDOW *wDialogue = NULL;
WINDOW *wPlayScrOpts[PLAYING_SCREEN_OPT_NUM] = {NULL};

WINDOW *wInvs = NULL;

// initialize curses settings
gameStatus init_curses()
{
    LOG_INFO("Initializing curses settings");

    // Clear the screen using system call
    system("clear");

    // Set locale to support wide characters
    setlocale(LC_ALL, "");

    // Initialize the curses library
    if (initscr() == NULL)
    {
        LOG_ERROR("Cannot initialize curses library");
        return GAME_ERROR;
    }

    // Check if the terminal supports colors
    if (has_colors() == FALSE)
    {
        LOG_WARNING("Terminal does not support colors");
        mode = COLOR_MODE_BLACK_AND_WHITE;
    }
    else
    {
        start_color();
        mode = COLOR_MODE_COLOR;
    }

    // Set raw mode to read input character by character
    raw();

    // Disable echoing of input characters
    noecho();

    // Enable function keys like F1, F2, arrow keys, etc.
    keypad(stdscr, TRUE);

    // Hide the cursor
    curs_set(0);

    // Refresh the screen to apply changes
    refresh();

    LOG_INFO("Curses setting initialized");

    return GAME_INITIALIZING;
}

// end curses settings
gameStatus end_curses()
{
    LOG_INFO("Ending curses settings");
    endwin();
    system("clear");
    LOG_INFO("Curses settings ended");
    return GAME_EXITING;
}

gameStatus render_main_menu(char *img)
{
    LOG_INFO("Starting rendering main menu");
    static int isFirst = 1;

    if (isFirst) {
       char command[300];
        extern char *dir;
        sprintf(command, "img2sixel %s/%s", dir, img);
        LOG_DEBUG("Running command: %s", command);
        system(command); 
    }
    
    extern int nowMainMenuOpt;

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);

    // Render main menu window
    int wMainMenu_height, wMainMenu_width, wMainMenu_start_x, wMainMenu_start_y;
    wMainMenu_height = MAIN_MENU_OPT_NUM * 3 + MAIN_MENU_PADDING * 2;
    wMainMenu_width = MAIN_MENU_WIDTH;
    wMainMenu_start_x = (COLS - wMainMenu_width) / 2;
    wMainMenu_start_y = LINES - wMainMenu_height;
    if (isFirst)
    {
        wMainMenu = newwin(wMainMenu_height, wMainMenu_width, wMainMenu_start_y, wMainMenu_start_x);
        box(wMainMenu, 0, 0);
        LOG_INFO("Main Menu window is created");
    }
    for (int i = 1; i < wMainMenu_height - 1; i++)
    {
        for (int j = 1; j < wMainMenu_width - 1; j++)
        {
            wattron(wMainMenu, COLOR_PAIR(3));
            mvwaddch(wMainMenu, i, j, 'a');
            wattroff(wMainMenu, COLOR_PAIR(3));
        }
    }
    wrefresh(wMainMenu);
    LOG_INFO("Main menu window rendered successfully");
    int wMainMenuOpt_height, wMainMenuOpt_width, wMainMenuOpt_start_x, wMainMenuOpt_start_y;
    wMainMenuOpt_height = 3;
    wMainMenuOpt_width = wMainMenu_width - MAIN_MENU_PADDING * 2;
    wMainMenuOpt_start_x = wMainMenu_start_x + MAIN_MENU_PADDING;
    wMainMenuOpt_start_y = wMainMenu_start_y + MAIN_MENU_PADDING;
    char *mainMenuOptText[MAIN_MENU_OPT_NUM] = {"開新遊戲", "載入遊戲", "功能測試", "離開遊戲"};
    for (int i = 0; i < MAIN_MENU_OPT_NUM; i++)
    {
        if (wMainMenuOpts[i] == NULL)
        {
            wMainMenuOpts[i] = newwin(wMainMenuOpt_height, wMainMenuOpt_width, wMainMenuOpt_start_y + i * 3, wMainMenuOpt_start_x);
            LOG_INFO("Main Menu option window %d is created", i);
        }
        box(wMainMenuOpts[i], 0, 0);
        if (i != nowMainMenuOpt)
        {
            wattron(wMainMenuOpts[i], COLOR_PAIR(1));
            for (int j = 1; j < wMainMenuOpt_width - 1; j++)
            {
                mvwaddch(wMainMenuOpts[i], 1, j, ' ');
            }
            mvwaddstr(wMainMenuOpts[i], 1, (wMainMenu_width - 6) / 2, mainMenuOptText[i]);
            wattroff(wMainMenuOpts[i], COLOR_PAIR(1));
        }
        else
        {
            wattron(wMainMenuOpts[i], COLOR_PAIR(2));
            for (int j = 1; j < wMainMenuOpt_width - 1; j++)
            {
                mvwaddch(wMainMenuOpts[i], 1, j, ' ');
            }
            mvwaddstr(wMainMenuOpts[i], 1, (wMainMenu_width - 6) / 2, mainMenuOptText[i]);
            wattroff(wMainMenuOpts[i], COLOR_PAIR(2));
        }
        wrefresh(wMainMenuOpts[i]);
    }
    isFirst = 0;
    LOG_INFO("Main menu option window rendered successfully");
    return GAME_MAIN_MENU;
}

gameStatus end_main_menu()
{
    for (int i = 0; i < MAIN_MENU_OPT_NUM; i++)
    {
        wborder(wMainMenuOpts[i], ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wrefresh(wMainMenuOpts[i]);
        delwin(wMainMenuOpts[i]);
        LOG_INFO("Main menu option %d is deleted successfully", i);
    }
    wborder(wMainMenu, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(wMainMenu);
    delwin(wMainMenu);
    system("clear");
    LOG_INFO("Main menu is deleted successfully");
    return GAME_MAIN_MENU;
}

gameStatus render_playing_screen(char *img, dialogue dlg)
{
    LOG_INFO("Starting rendering playing screen");
    attron(COLOR_PAIR(3));
    mvaddch(0, 0, 'a');
    attroff(COLOR_PAIR(3));
    mvcur(0, 1, 0, 0);
    char command[300];
    sprintf(command, "img2sixel -h %d -w %d %s", SCENE_HEIGHT, SCENE_WIDTH, img);
    LOG_DEBUG("command: %s", command);
    system(command);
    static int isFirst = 1;

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);

    // Render dialogue window
    int wDialogue_height, wDialogue_width, wDialogue_start_x, wDialogue_start_y;
    wDialogue_height = DIALOGUE_HEIGHT;
    wDialogue_width = COLS;
    wDialogue_start_x = 0;
    wDialogue_start_y = LINES - wDialogue_height;
    if (isFirst)
    {
        wDialogue = newwin(wDialogue_height, wDialogue_width, wDialogue_start_y, wDialogue_start_x);
        box(wDialogue, 0, 0);
        LOG_INFO("Dialogue window is created");
    }

    for (int i = 1; i < wDialogue_height - 1; i++)
    {
        for (int j = 1; j < wDialogue_width - 1; j++)
        {
            wattron(wDialogue, COLOR_PAIR(3));
            mvwaddch(wDialogue, i, j, 'a');
            wattroff(wDialogue, COLOR_PAIR(3));
        }
    }
    if (dlg.type == TEXT)
    {
        mvwaddstr(wDialogue, 1, 1, dlg.text);
        wrefresh(wDialogue);
        LOG_INFO("Dialogue window rendered successfully");
    }
    else if (dlg.type == OPT_CHO || dlg.type == OPT_FLA)
    {
        extern int nowDlgOpt;
        int wPSOpt_height, wPSOpt_width, wPSOpt_start_x, wPSOpt_start_y;
        wPSOpt_height = 3;
        if (dlg.optNum <= 2)
        {
            wPSOpt_width = wDialogue_width - 2;
        }
        else if (dlg.optNum <= 4)
        {
            wPSOpt_width = (wDialogue_width - 2) / 2;
        }
        wPSOpt_start_x = wDialogue_start_x + 1;
        wPSOpt_start_y = wDialogue_start_y + 1;
        for (int i = 0; i < dlg.optNum; i++)
        {
            if (i == 1)
            {
                wPSOpt_start_y += 3;
            }
            else if (i == 2)
            {
                wPSOpt_start_y -= 3;
                wPSOpt_start_x += wPSOpt_width;
            }
            else if (i == 3)
            {
                wPSOpt_start_y += 3;
            }
            wPlayScrOpts[i] = newwin(wPSOpt_height, wPSOpt_width, wPSOpt_start_y, wPSOpt_start_x);
            box(wPlayScrOpts[i], 0, 0);
            if (dlg.type == OPT_CHO) {
                LOG_DEBUG("option %d text: %s", i, dlg.optCho[i].text);
            }
            else {
                LOG_DEBUG("option %d text: %s", i, dlg.optFla[i].text);
            }
            if (i != nowDlgOpt) {
                for (int j = 1; j < wPSOpt_width - 1; j++) {
                    wattron(wPlayScrOpts[i], COLOR_PAIR(1));
                    mvwaddch(wPlayScrOpts[i], 1, j, ' ');
                    if (dlg.type == OPT_CHO) {
                        mvwaddstr(wPlayScrOpts[i], 1, 1, dlg.optCho[i].text);
                    }
                    else {
                        mvwaddstr(wPlayScrOpts[i], 1, 1, dlg.optFla[i].text);
                    }
                    wattroff(wPlayScrOpts[i], COLOR_PAIR(1));
                }
            }
            else {
                for (int j = 1; j < wPSOpt_width - 1; j++) {
                    wattron(wPlayScrOpts[i], COLOR_PAIR(2));
                    mvwaddch(wPlayScrOpts[i], 1, j, ' ');
                    if (dlg.type == OPT_CHO) {
                        mvwaddstr(wPlayScrOpts[i], 1, 1, dlg.optCho[i].text);
                    }
                    else {
                        mvwaddstr(wPlayScrOpts[i], 1, 1, dlg.optFla[i].text);
                    }
                    wattroff(wPlayScrOpts[i], COLOR_PAIR(2));
                }
            }
            wrefresh(wPlayScrOpts[i]);
            LOG_INFO("Play Screen Option %d window is created", i);
        }
    }
    else
    {
        LOG_ERROR("Invalid dialogue type");
    }

    // Render character name window
    int wCharName_height, wCharName_width, wCharName_start_x, wCharName_start_y;
    wCharName_height = 3;
    wCharName_width = CHAR_NAME_WIDTH;
    wCharName_start_x = 0;
    wCharName_start_y = LINES - wDialogue_height - wCharName_height;
    if (isFirst)
    {
        wCharName = newwin(wCharName_height, wCharName_width, wCharName_start_y, wCharName_start_x);
        box(wCharName, 0, 0);
        LOG_INFO("Character name window is created");
    }

    for (int i = 1; i < wCharName_height - 1; i++)
    {
        for (int j = 1; j < wCharName_width - 1; j++)
        {
            wattron(wCharName, COLOR_PAIR(3));
            mvwaddch(wCharName, i, j, 'a');
            wattroff(wCharName, COLOR_PAIR(3));
        }
    }
    mvwaddstr(wCharName, 1, 1, dlg.chr);
    wrefresh(wCharName);
    LOG_INFO("Character name window rendered successfully");
    isFirst = 0;
    return GAME_PLAYING;
}

gameStatus end_playing_screen()
{
    wborder(wDialogue, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(wDialogue);
    delwin(wDialogue);
    LOG_INFO("Dialogue window is deleted successfully");

    wborder(wCharName, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(wCharName);
    delwin(wCharName);
    LOG_INFO("Character name window is deleted successfully");
    return GAME_PLAYING;
}

gameStatus render_inventory() {
    LOG_INFO("Starting rendering inventory");

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);

    // Render inventory window
    int wInvs_height, wInvs_width, wInvs_start_x, wInvs_start_y;
    wInvs_height = INV_HEIGHT;
    wInvs_width = INV_WIDTH;
    wInvs_start_x = (COLS - wInvs_width) / 2;
    wInvs_start_y = (LINES - wInvs_height) / 2;
    wInvs = newwin(wInvs_height, wInvs_width, wInvs_start_y, wInvs_start_x);
    box(wInvs, 0, 0);
    for (int i = 1; i < wInvs_height - 1; i++) {
        for (int j = 1; j < wInvs_width - 1; j++) {
            wattron(wInvs, COLOR_PAIR(3));
            mvwaddch(wInvs, i, j, 'a');
            wattroff(wInvs, COLOR_PAIR(3));
        }
    }
    wrefresh(wInvs);
    LOG_INFO("Inventory window is created");
    return GAME_PLAYING;
}

gameStatus end_inventory() {
    wborder(wInvs, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(wInvs);
    delwin(wInvs);
    LOG_INFO("Inventory window is deleted successfully");
    return GAME_PLAYING;
}