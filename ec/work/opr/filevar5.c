#include "omtfp.h"
//DOOD : do ta hast.
//BYTE scrn_save[256][80];
char command_buf[120];
BYTE buf_ptr;
BYTE scrn_save[256][80];
BYTE scrn_save_line;
BYTE scrn_save_column;
char command_buf_save[120];
BYTE buf_ptr_save;
BYTE scroll_pointer;
BYTE *dos_key[32];
BYTE dos_key_ptr;
BYTE dos_key_current_ptr;
