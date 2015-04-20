#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xls.h>
#include "ldef.h"

int lread(lua_State *L)
{
    int ret = 0;
    const char *fname = lua_tostring(L, 1);
    const char *charset = lua_tostring(L, 2);
    lua_settop(L, 0);
    xlsWorkBook *pwb = xls_open(fname, charset);
    if (pwb) {
        lua_createtable(L, 0, pwb->sheets.count);
        ret = 1;

        int i,j,k;
        for (i=0; i < pwb->sheets.count; i++) {
            xlsWorkSheet *pws = xls_getWorkSheet(pwb, i);
            xls_parseWorkSheet(pws);

            int row_count = pws->rows.lastrow + 1;
            int col_count = pws->rows.lastcol + 1;
            lua_createtable(L, row_count, 0);

            const char *sheet_name = (const char *)pwb->sheets.sheet[i].name;
            for (j=0; j<row_count; j++) {
                lua_createtable(L, row_count, 0);

                for (k=0; k<col_count; k++) {
                    struct st_cell_data	*cell = xls_cell(pws, j, k);
                    if (cell) {
                        if(cell->id == 0x0201) {
                            //printf("BLANK_CELL! [%s](%d,%d)\n", sheet_name, j, k);
                            continue;
                        }
                        const char *str = (const char *)cell->str;
                        char tmp[32];
                        sprintf(tmp, "%lf", cell->d);
                        if (!strcmp(tmp, str)) { // number
                            if (cell->d - (long)cell->d > 0) {
                                lua_pushnumber(L, cell->d);
                            } else {
                                lua_pushinteger(L, cell->d);
                            }
                        } else { // string
                            lua_pushstring(L, str);
                        }
                        lua_rawseti(L, -2, k+1);
                    }
                }
                lua_rawseti(L, -2, j+1);
            }

            lua_setfield(L, -2, sheet_name);
            xls_close_WS(pws);
        }
        xls_close_WB(pwb);
    } else {
        printf("Can't read file : %s\n", fname);
    }
    return ret;
}

int luaopen_xlsread_core(lua_State *L)
{
    luaL_checkversion(L);
    luaL_Reg l[] = {
        { "read", lread },
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    return 1;
}

/*
int readxls(const char *fname, const char *charset)
{
    xlsWorkBook *pwb = xls_open(fname, charset);
    int ret = 0;
    if (pwb) {
        int i,j,k;
        for (i=0; i < pwb->sheets.count; i++) {
            const char *sheet_name = (const char *)pwb->sheets.sheet[i].name;
            xlsWorkSheet *pws = xls_getWorkSheet(pwb, i);
            xls_parseWorkSheet(pws);
            for (j=0; j <= pws->rows.lastrow; j++) {
                for (k=0; k <= pws->rows.lastcol; k++) {
                    struct st_cell_data	*cell = xls_cell(pws, j, k);
                    if (cell) {
                        if(cell->id == 0x0201) {
                            printf("BLANK_CELL! [%s](%d,%d)\n", sheet_name, j, k);
                            continue;
                        }
                        char tmp[32];
                        sprintf(tmp, "%lf", cell->d);
                        printf("[%d](%d,%d):str=%s,d=%g,l=%d\n", i, j, k, cell->str, cell->d, cell->l);
                        if (!strcmp(tmp, (const char *)cell->str)) { // number
                        } else { // string
                        }
                    }
                }
            }
            xls_close_WS(pws);
        }
        xls_close_WB(pwb);
    } else {
        printf("Can't read file : %s\n", fname);
    }
    return ret;
}

int main(int argc, char *argv[])
{
    xlsWorkBook* pWB;
    xlsWorkSheet* pWS;
    unsigned int i;

	if(argc != 2) {
		printf("Need file arg\n");
		exit(0);
	}
	
    struct st_row_data* row;
    unsigned long t,tt;

    readxls(argv[1],"UTF-8");

    return 0;
}
*/
