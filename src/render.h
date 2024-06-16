#ifndef _RENDER_H_
#define _RENDER_H_

#include "game_variables.h"

/* Image Size */
#define COVER_SCALE_RATIO 8
#define SCENE_SCALE_RATIO 4.5
#define COVER_ASPECT_RATIO 5/3
#define SCENE_ASPECT_RATIO 5/3

/* Main Menu MACRO */
#define MAIN_MENU_OPT_NUM 4
#define MAIN_MENU_WIDTH 40
#define MAIN_MENU_PADDING 1

/* Playing Screen MACRO */
#define PLAYING_SCREEN_OPT_NUM 4
#define DIALOGUE_HEIGHT 8
#define CHAR_NAME_WIDTH 30

/* Error Message */
// #define ERROR_MESSAGE_OPTION_NUM 1

// enum IMAGE_TYPE
// {
//     COVER,
//     SCENE
// };

// void display_image(char *filename, enum IMAGE_TYPE type);

// initialize curses settings
gameStatus init_curses();

// end curses settings
gameStatus end_curses();

// render main menu
gameStatus render_main_menu(char *img);

gameStatus end_main_menu();

gameStatus render_playing_screen(char *img, dialogue dlg);

gameStatus end_playing_screen();

// gameStatus render_game_screen(char *game_dir, char *scene, char *character, char *text);

// gameStatus render_game_screen_part(char *game_dir, Dialogue *dialogue);

// int render_error_message(char *errorMessage);

// int end_error_message();

// int end_start_screen();

// int end_game_screen();

#endif