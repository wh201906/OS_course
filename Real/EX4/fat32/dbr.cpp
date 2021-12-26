#include "dbr.h"

bool DBR_t::isValid()
{
    return DataHandle::isValid() && *(uint16_t *)end() == 0xAA55;
}

void DBR_t::info()
{
    uint8_t *jmpData;
    uint8_t *OEMData;
    BPB_t bpb;

    DataHandle::info();
    printf("DBR_t\n");
    jmpData = jmp();
    OEMData = OEM();
    bpb = BPB();
    printf("jmp:0x%02x 0x%02x 0x%02x <-> 0xEB 0x58 0x90\n", jmpData[0], jmpData[1], jmpData[2]);
    printf("OEM:");
    for (uint8_t i = 0; i < 8; i++)
        putchar(OEMData[i]);
    putchar('\n');
    bpb.info();
    // Check backup?
}
