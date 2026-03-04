/********************************************************************
 * SamVlirEdit.c
 *
 * This file contains the Edit Module code for the llvm-mos GEOS VLIR
 * sample application.
 *
 ********************************************************************/

#include <geos.h>
#include <stdio.h>

#include "SamVlir.h"

/* Through this pragma all functions and module data will be stored in VLIR 2
 * record. */

#pragma clang section text = ".vlir02.text" data = ".vlir02.data" bss =        \
    ".vlir02.bss"

#define MAX_LINES 20

/* Dialog box to indicate that an event handler funnction was called. */
/* Parameter 2 of DB_NAME_SECT sets the vlir record 2 */

DB_NAME_SECT(dlg_edit, vlir02)
DB_DEFPOS(1)
DB_ICON(OK, DBI_X_0, DBI_Y_2)
DB_VARSTR(DBI_X_0, DBI_Y_0, __r15)
DB_END(dlg_edit)

/* Define string constants as static variables. This way they will be stored
 * in the vlir record 2 and not in the main constant string pool.
 */

char cut_msg[] = "cut handler called.";
char copy_msg[] = "copy handler called.";
char paste_msg[] = "paste handler called.";
char icon1_msg[] = "icon handler called.";
static uint8_t buffer_backup[256];

// Init to zero. filenames are 16 chars plus null terminator
char dirname[17] = {0};

char outstring[41] = {0};

// To keep track of "cursor" for printing text
uint16_t text_x = 0 + 2;
// NOTE: Don't overlap the height of the font to top of screen - it crashes
// the OS!
uint8_t text_y = 0 + 20;

/********************************************************************
 * This is a dummy event handler function. Customize this for your
 * own application. The noinline decorator guarantees that the
 * function is stored in the VLIR 2 record and that the optimier
 * will not inline the code in the main (VLIR 0) record.
 ********************************************************************/

__attribute__((noinline)) void DoCut(void) {
  __r15 = (uint16_t)cut_msg;
  DoDlgBox(dlg_edit);
}

/********************************************************************
 * This is a dummy event handler function. Customize this for your
 * own application. The noinline decorator guarantees that the
 * function is stored in the VLIR 2 record and that the optimier
 * will not inline the code in the main (VLIR 0) record.
 ********************************************************************/

__attribute__((noinline)) void DoCopy(void) {
  dir_entry_t **de;

  // Stash diskBlkBuf so we don't interfere with anything
  MoveData(&diskBlkBuf, buffer_backup, 256);

  disk_err_t err = Get1stDirEntry(de);
  dir_entry_t *de_ptr = *de;

  if(err) {
    PutDecimal(err, text_x, text_y, SET_LEFTJUST | SET_NOSURPRESS);
    /*
    text_x = __r11;
    text_y = __r1H;
    */
  }
  else {
    CopyFString((uint8_t *)de_ptr->name, (uint8_t *)dirname, 16);

    PutString(text_x, text_y, dirname);
    text_x = 2;
    text_y += 10;

    // Get new values from Registers after PutString:
    /*
    text_x = __r11;
    text_y = __r1H;
    */

    bool end_dir = 0;
    uint8_t lines = 0;
    do {
      err = GetNxtDirEntry(de, &end_dir); 
      if (!err && !end_dir) {
        de_ptr = *de;
        CopyFString((uint8_t *)de_ptr->name, (uint8_t *)dirname, 16);
        PutString(text_x, text_y, dirname);
        text_x = 2;
        text_y += 10;
      }
    }
    while (!err && !end_dir && lines <= MAX_LINES);

    //PutString(text_x, text_y, SPACE);
  }
  
  sprintf(outstring, "diskBlkBuf address is %X", 1234);

  // Put diskBlkBuf back the way we found it
  MoveData(buffer_backup, &diskBlkBuf, 256);

  __r15 = (uint16_t)dirname;
  DoDlgBox(dlg_edit);
}

/********************************************************************
 * This is a dummy event handler function. Customize this for your
 * own application. The noinline decorator guarantees that the
 * function is stored in the VLIR 2 record and that the optimier
 * will not inline the code in the main (VLIR 0) record.
 ********************************************************************/

__attribute__((noinline)) void DoPaste(void) {
  __r15 = (uint16_t)paste_msg;
  DoDlgBox(dlg_edit);
}

/********************************************************************
 * This is a dummy event handler function. Customize this for your
 * own application. The noinline decorator guarantees that the
 * function is stored in the VLIR 2 record and that the optimier
 * will not inline the code in the main (VLIR 0) record.
 ********************************************************************/

__attribute__((noinline)) void DoIcon1(void) {
  __r15 = (uint16_t)icon1_msg;
  DoDlgBox(dlg_edit);
}
