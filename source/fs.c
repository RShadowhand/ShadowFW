/*
*   fs.c
*       by Reisyukaku / Aurora Wright
*   Copyright (c) 2016 All Rights Reserved
*/

#include "fs.h"
#include "memory.h"
#include "fatfs/ff.h"

static FATFS sdFs,
             nandFs;

u32 mountFs(void)
{
    if(f_mount(&sdFs, "0:", 1) != FR_OK) return 0;
    f_mount(&nandFs, "1:", 0);

    return 1;
}

u32 fileRead(void *dest, const char *path, u32 size)
{
    FRESULT result;
    FIL file;

    result = f_open(&file, path, FA_READ);
    if(result == FR_OK)
    {
        unsigned int read;
        if(!size) size = f_size(&file);
        result = f_read(&file, dest, size, &read);
    }

    f_close(&file);

    return result ? 0 : 1;
}

void firmRead(void *dest, const char *firmFolder)
{
    char path[48] = "1:/title/00040138/00000000/content";
    memcpy(&path[18], firmFolder, 8);

    DIR dir;
    FILINFO info;

    f_opendir(&dir, path);

    u32 id = 0;

    //Parse the target directory
    while(f_readdir(&dir, &info) == FR_OK)
    {
        //We've parsed the whole folder
        if(!info.fname[0]) break;

        //Not a cxi
        if(info.altname[9] != 'A') continue;

        //Convert the .app name to an integer
        u32 tempId = 0;
        for(char *tmp = info.altname; *tmp != '.'; tmp++)
        {
            tempId <<= 4;
            tempId += *tmp > '9' ? *tmp - 'A' + 10 : *tmp - '0';
        }

        //Found an older cxi
        if(tempId > id) id = tempId;
    }

    f_closedir(&dir);

    //Complete the string with the .app name
    memcpy(&path[34], "/00000000.app", 14);

    u32 i = 42;

    //Convert back the .app name from integer to array
    while(id)
    {
        //Last digit of the .app
        static const char hexDigits[] = "0123456789ABCDEF";
        path[i--] = hexDigits[id & 0xF];
        id >>= 4;
    }

    fileRead(dest, path, 0);
}