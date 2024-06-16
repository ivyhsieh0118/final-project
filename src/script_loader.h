#ifndef _SCRIPT_LOADER_H_
#define _SCRIPT_LOADER_H_

#include "game_variables.h"

toml_table_t *table;
toml_table_t *tInfo, *tPlayer, *tChar, *tScenario;
toml_table_t *nowtBrc, *nowtDlg;
toml_array_t *aBrc, *aDlg, *aChar, *aAvts, *aAvt, *aNxtBrcs, *aNxtBrc;

gameStatus init_script_loader();

gameStatus load_player();

gameStatus load_characters();

gameStatus load_scenario();

gameStatus load_branch(int index);

gameStatus load_dialogue(int index, dialogue *dlg);

gameStatus end_script_loader();

#endif