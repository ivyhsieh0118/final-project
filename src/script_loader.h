#ifndef _SCRIPT_LOADER_H_
#define _SCRIPT_LOADER_H_

#include "game_variables.h"

extern toml_table_t *table;
extern toml_table_t *tInfo, *tPlayer, *tChar, *tScenario;
extern toml_table_t *nowtBrc, *nowtDlg, *nowtItm;
extern toml_array_t *aBrc, *aDlg, *aChar, *aAvts, *aAvt, *aNxtBrcs, *aNxtBrc, *aInv;

gameStatus init_script_loader();

gameStatus load_information(char *img);

gameStatus load_item();

gameStatus load_player();

gameStatus load_characters();

gameStatus load_scenario();

gameStatus load_branch(int index);

gameStatus load_dialogue(int index, dialogue *dlg);

gameStatus end_script_loader();

#endif