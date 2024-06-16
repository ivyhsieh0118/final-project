#ifndef GAME_VARIABLES_H
#define GAME_VARIABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <sixel.h>
#include <locale.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "toml.h"

//  MACRO
#define SCRIPT_FILENAME "script.toml"
#define LOG_FILENAME "game.log"
#define LOG_DEBUG(msg, ...) log_record("Debug in %s:%s:%d: " msg, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) log_record("Info in %s:%s:%d: " msg, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) log_record("Warning in %s:%s:%d: " msg, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_CRITICAL(msg, ...) log_record("Critical in %s:%s:%d: " msg, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) log_record("Error in %s:%s:%d: " msg, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

typedef enum _gameStatus {
    GAME_INITIALIZING,    // Game is initializing
    GAME_MAIN_MENU,       // Main menu screen
    GAME_STARTING,        // Starting new game
    GAME_LOADING,         // Loading game
    GAME_PLAYING,         // Game is in progress
    GAME_PAUSED,          // Game is paused
    GAME_ENDING,          // Ending the game
    GAME_EXITING,         // Exiting the game
    GAME_ERROR            // General error state
} gameStatus;

typedef enum _colorMode {
    COLOR_MODE_BLACK_AND_WHITE,
    COLOR_MODE_COLOR
} colorMode;

typedef enum _dlgType {
    TEXT,
    OPT_FLA,
    OPT_CHO
} dlgType;

typedef struct _dlgOptFla {
    char text[100];
    int val;
} dlgOptFla;

typedef struct _dlgOptCho {
    char text[100];
    int nxtBrc;
} dlgOptCho;

typedef struct _brcOpt {
    int lb;
    int ub;
    int nxtBrc;
} brcOpt;

typedef struct _dialogue {
    char img[300]; 
    char chr[100];
    dlgType type;
    char text[1000];
    int optNum;
    char obj[100];
    int nxtBrcNum;
    brcOpt brc[5];
    dlgOptFla optFla[5];
    dlgOptCho optCho[5];
} dialogue;

typedef struct _chrAvatar {
    char img[300];
    int lowerBound;
    int upperBound;
} chrAvatar;

typedef struct _character {
    char name[100];
    int favor;
    int avtNum;
    chrAvatar avatar[4];
} character;

gameStatus status;
colorMode mode;
extern int nowMainMenuOpt;
extern int cptNum, brcNum, dlgNum;

//
void init_log();

// game log
void log_record(const char *format, ...);

// variables of development

#endif