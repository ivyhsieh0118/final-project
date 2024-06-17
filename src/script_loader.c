#include "script_loader.h"

extern char *dir;

toml_table_t *table;
toml_table_t *tInfo, *tPlayer, *tChar, *tScenario;
toml_table_t *nowtBrc, *nowtDlg, *nowtItm;
toml_array_t *aBrc, *aDlg, *aChar, *aAvts, *aAvt, *aNxtBrcs, *aNxtBrc, *aInv, *aItm;

gameStatus init_script_loader() {
    LOG_INFO("Initializing script loader");
    char path[1024];
    char errbuf[200];
    sprintf(path, "./%s/%s", dir, SCRIPT_FILENAME);
    FILE *scriptFile;
    if ( (scriptFile = fopen(path, "r")) == NULL) {
        LOG_ERROR("Could not open file %s", SCRIPT_FILENAME);
        return GAME_ERROR;
    }
    table = toml_parse_file(scriptFile, errbuf, sizeof(errbuf));
    fclose(scriptFile);
    if (!table) {
        LOG_ERROR("%s", errbuf);
        return GAME_ERROR;
    }
    LOG_INFO("Script loader initialized");
    return GAME_INITIALIZING;
}

gameStatus load_information(char *img) {
    LOG_INFO("Loading information");
    tInfo = toml_table_in(table, "information");
    if (!tInfo) {
        LOG_ERROR("Could not find information table");
        return GAME_ERROR;
    }
    toml_datum_t cover = toml_string_in(tInfo, "cover");
    if (!cover.ok) {
        LOG_ERROR("Could not find information cover");
        return GAME_ERROR;
    }
    strcpy(img, cover.u.s);
    LOG_INFO("Information loaded");
    return GAME_INITIALIZING;
}

gameStatus load_item() {
    extern player plr;
    aItm = toml_array_in(table, "item");
    if (!aItm) {
        LOG_ERROR("Could not find item table");
        return GAME_ERROR;
    }
    for (int i = 0; i < toml_array_nelem(aItm); i++) {
        nowtItm = toml_table_at(aItm, i);
        if (!nowtItm) {
            LOG_ERROR("Could not find item table %d", i);
            return GAME_ERROR;
        }
        toml_datum_t name = toml_string_in(nowtItm, "name");
        if (!name.ok) {
            LOG_ERROR("Could not find item name %d", i);
            return GAME_ERROR;
        }
        int itmIndex = -1;
        for (int j = 0; j < plr.nInv; j++) {
            if (strcmp(plr.inv[j].name, name.u.s) == 0) {
                itmIndex = j;
                break;
            }
        }
        toml_datum_t desc = toml_string_in(nowtItm, "description");
        if (!desc.ok) {
            LOG_ERROR("Could not find item description %d", i);
            return GAME_ERROR;
        }
        strcpy(plr.inv[itmIndex].desc, desc.u.s);
        toml_datum_t val = toml_int_in(nowtItm, "value");
        if (!val.ok) {
            LOG_ERROR("Could not find item value %d", i);
            return GAME_ERROR;
        }
        plr.inv[itmIndex].val = val.u.i;
        toml_datum_t img = toml_string_in(nowtItm, "image");
        if (!img.ok) {
            LOG_ERROR("Could not find item image %d", i);
            return GAME_ERROR;
        }
        strcpy(plr.inv[itmIndex].img, img.u.s);
    }
    return GAME_INITIALIZING;
}

gameStatus load_player() {
    extern player plr;
    LOG_INFO("Loading player");
    tPlayer = toml_table_in(table, "player");
    if (!tPlayer) {
        LOG_ERROR("Could not find player table");
        return GAME_ERROR;
    }
    toml_datum_t name = toml_string_in(tPlayer, "name");
    if (!name.ok) {
        LOG_ERROR("Could not find player name");
        return GAME_ERROR;
    }
    strcpy(plr.name, name.u.s);
    aInv = toml_array_in(tPlayer, "inventory");
    if (!aInv) {
        LOG_ERROR("Could not find player inventory array");
        return GAME_ERROR;
    }
    plr.nInv = toml_array_nelem(aInv);
    for (int i = 0; i < plr.nInv; i++) {
        toml_datum_t item = toml_string_at(aInv, i);
        if (!item.ok) {
            LOG_ERROR("Could not find player inventory item %d", i);
            return GAME_ERROR;
        }
        strcpy(plr.inv[i].name, item.u.s);
    }
    load_item();
    LOG_INFO("Player loaded");
    return GAME_INITIALIZING;
}

gameStatus load_characters() {
    extern int chrNum;
    extern character chr[10];
    aChar = toml_array_in(table, "character");
    if (!aChar) {
        LOG_ERROR("Could not find character array");
        return GAME_ERROR;
    }
    chrNum = toml_array_nelem(aChar);
    for (int i = 0; i < chrNum; i++) {
        tChar = toml_table_at(aChar, i);
        if (!tChar) {
            LOG_ERROR("Could not find character %d", i);
            return GAME_ERROR;
        }
        toml_datum_t name = toml_string_in(tChar, "name");
        if (!name.ok) {
            LOG_ERROR("Could not find character %d name", i);
            return GAME_ERROR;
        }
        strcpy(chr[i].name, name.u.s);
        toml_datum_t favor = toml_int_in(tChar, "favor");
        if (!favor.ok) {
            LOG_ERROR("Could not find character %d favor", i);
            return GAME_ERROR;
        }
        chr[i].favor = favor.u.i;
        aAvts = toml_array_in(tChar, "avatar");
        if (!aAvts) {
            LOG_ERROR("Could not find character %d avatar array", i);
            return GAME_ERROR;
        }
        chr[i].avtNum = toml_array_nelem(aAvts);
        for (int j = 0; j < chr[i].avtNum; j++) {
            aAvt = toml_array_at(aAvts, j);
            if (!aAvt) {
                LOG_ERROR("Could not find character %d avatar %d", i, j);
                return GAME_ERROR;
            }
            toml_datum_t lowerBound = toml_int_at(aAvt, 0);
            if (!lowerBound.ok) {
                LOG_ERROR("Could not find character %d avatar %d lower bound", i, j);
                return GAME_ERROR;
            }
            chr[i].avatar[j].lowerBound = lowerBound.u.i;
            toml_datum_t upperBound = toml_int_at(aAvt, 1);
            if (!upperBound.ok) {
                LOG_ERROR("Could not find character %d avatar %d upper bound", i, j);
                return GAME_ERROR;
            }
            chr[i].avatar[j].upperBound = upperBound.u.i;
            toml_datum_t img = toml_string_at(aAvt, 2);
            if (!img.ok) {
                LOG_ERROR("Could not find character %d avatar %d image", i, j);
                return GAME_ERROR;
            }
            strcpy(chr[i].avatar[j].img, img.u.s);
        }
    }
    return GAME_MAIN_MENU;
}

gameStatus load_scenario() {
    extern int cptNum;
    tScenario = toml_table_in(table, "scenario");
    if (!tScenario) {
        LOG_ERROR("Could not find scenario table");
        return GAME_ERROR;
    }
    aBrc = toml_array_in(tScenario, "branch");

    if (!aBrc) {
        LOG_ERROR("Could not find branch array");
        return GAME_ERROR;
    }

    brcNum = toml_array_nelem(aBrc);
    LOG_DEBUG("Branch array length: %d", brcNum);

    LOG_INFO("Scenario loaded");
    return GAME_INITIALIZING;
}

gameStatus load_branch(int index) {
    extern int dlgNum;
    nowtBrc = toml_table_at(aBrc, index);
    if (!nowtBrc) {
        LOG_ERROR("Could not find branch %d", index);
        return GAME_ERROR;
    }
    aDlg = toml_array_in(nowtBrc, "dialogue");
    if (!aDlg) {
        LOG_ERROR("Could not find dialogue array");
        return GAME_ERROR;
    }
    dlgNum = toml_array_nelem(aDlg);
    LOG_DEBUG("dialogue array %d length: %d", index, dlgNum);
    LOG_INFO("Branch %d loaded", index);
    return GAME_INITIALIZING;
}

gameStatus load_dialogue(int index, dialogue *dlg) {
    nowtDlg = toml_table_at(aDlg, index);
    if (!nowtDlg) {
        LOG_ERROR("Could not find dialogue %d", index);
        return GAME_ERROR;
    }
    toml_datum_t img = toml_string_in(nowtDlg, "scene_image");
    if (!img.ok) {
        LOG_ERROR("Could not find scene image");
        return GAME_ERROR;
    }
    strcpy(dlg->img, img.u.s);
    toml_datum_t chr = toml_string_in(nowtDlg, "character");
    if (!chr.ok) {
        LOG_ERROR("Could not find character");
        return GAME_ERROR;
    }
    strcpy(dlg->chr, chr.u.s);
    toml_datum_t type = toml_string_in(nowtDlg, "type");
    if (!type.ok) {
        LOG_ERROR("Could not find type");
        return GAME_ERROR;
    }
    if (strcmp(type.u.s, "text") == 0) {
        dlg->type = TEXT;
        toml_datum_t txt = toml_string_in(nowtDlg, "text");
        if (!txt.ok) {
            LOG_ERROR("Could not find text");
            return GAME_ERROR;
        }
        strcpy(dlg->text, txt.u.s);
    }
    else if (strcmp(type.u.s, "option_by_favor") == 0) {
        dlg->type = OPT_FLA;
        toml_datum_t object = toml_string_in(nowtDlg, "object");
        if (!object.ok) {
            LOG_ERROR("Could not find option object");
            return GAME_ERROR;
        }
        strcpy(dlg->obj, object.u.s);
        aNxtBrcs = toml_array_in(nowtDlg, "next_branch");
        if (!aNxtBrcs) {
            LOG_ERROR("Could not find next branch array");
            return GAME_ERROR;
        }
        dlg->nxtBrcNum = toml_array_nelem(aNxtBrcs);
        for (int i = 0; i < dlg->nxtBrcNum; i++) {
            aNxtBrc = toml_array_at(aNxtBrcs, i);
            if (!aNxtBrc) {
                LOG_ERROR("Could not find next branch %d", i);
                return GAME_ERROR;
            }
            toml_datum_t lb = toml_int_at(aNxtBrc, 0);
            if (!lb.ok) {
                LOG_ERROR("Could not find next branch %d lower bound", i);
                return GAME_ERROR;
            }
            dlg->brc[i].lb = lb.u.i;
            toml_datum_t ub = toml_int_at(aNxtBrc, 1);
            if (!ub.ok) {
                LOG_ERROR("Could not find next branch %d upper bound", i);
                return GAME_ERROR;
            }
            dlg->brc[i].ub = ub.u.i;
            toml_datum_t brc = toml_int_at(aNxtBrc, 2);
            if (!brc.ok) {
                LOG_ERROR("Could not find next branch %d branch", i);
                return GAME_ERROR;
            }
            dlg->brc[i].nxtBrc = brc.u.i;
        }
        toml_array_t *opts = toml_array_in(nowtDlg, "option");
        if (!opts) {
            LOG_ERROR("Could not find option array");
            return GAME_ERROR;
        }
        dlg->optNum = toml_array_nelem(opts);
        for (int i = 0; i < toml_array_nelem(opts); i++) {
            toml_table_t *nowOpt = toml_table_at(opts, i);
            if (!nowOpt) {
                LOG_ERROR("Could not find option %d", i);
                return GAME_ERROR;
            }
            toml_datum_t txt = toml_string_in(nowOpt, "text");
            if (!txt.ok) {
                LOG_ERROR("Could not find option %d text", i);
                return GAME_ERROR;
            }
            strcpy(dlg->optFla[i].text, txt.u.s);
            toml_datum_t value = toml_int_in(nowOpt, "value");
            if (!value.ok) {
                LOG_ERROR("Could not find option %d value", i);
                return GAME_ERROR;
            }
            dlg->optFla[i].val = value.u.i;
        }
    }
    else if (strcmp(type.u.s, "option_by_choice") == 0) {
        dlg->type = OPT_CHO;
        toml_array_t *opts = toml_array_in(nowtDlg, "option");
        if (!opts) {
            LOG_ERROR("Could not find option array");
            return GAME_ERROR;
        }
        dlg->optNum = toml_array_nelem(opts);
        for (int i = 0; i < toml_array_nelem(opts); i++) {
            toml_table_t *nowOpt = toml_table_at(opts, i);
            if (!nowOpt) {
                LOG_ERROR("Could not find option %d", i);
                return GAME_ERROR;
            }
            toml_datum_t txt = toml_string_in(nowOpt, "text");
            if (!txt.ok) {
                LOG_ERROR("Could not find option %d text", i);
                return GAME_ERROR;
            }
            strcpy(dlg->optCho[i].text, txt.u.s);
            toml_datum_t nxtBrc = toml_int_in(nowOpt, "next_branch");
            if (!nxtBrc.ok) {
                LOG_ERROR("Could not find option %d next branch", i);
                return GAME_ERROR;
            }
            dlg->optCho[i].nxtBrc = nxtBrc.u.i;
        }
    }
    else {
        LOG_ERROR("Unknown type %s", type.u.s);
        return GAME_ERROR;
    }
    LOG_INFO("Dialogue %d loaded", index);
    return GAME_INITIALIZING;
}

// typedef struct _dialogue {
//     char img[300]; 
//     char chr[100];
//     dlgType type;
//     char text[1000];
//     dlgOptFla optFla;
//     dlgOptCho optCho;
// } dialogue;

gameStatus end_script_loader() {
    LOG_INFO("Ending script loader");
    toml_free(table);
    LOG_INFO("Script loader ended");
    return GAME_EXITING;
}