/*
*   loader.c
*/

#include "loader.h"
#include "fs.h"
#include "memory.h"
#include "screeninit.h"
#include "../build/loader.h"

#define PAYLOAD_ADDRESS	0x24F00000

void loadPayload(void)
{
    if(defPayloadExists())
    {
        initScreens();
        memcpy((void *)PAYLOAD_ADDRESS, loader, loader_size);
        ((void (*)())PAYLOAD_ADDRESS)();
    }
}