#ifndef _DBR_H_
#define _DBR_H_

#include "datahandle.h"
#include "bpb.h"

class DBR_t : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    //data
    uint8_t *jmp() { return (m_data + 0); }        // len: 3
    uint8_t *OEM() { return (m_data + 3); }        // len: 8
    BPB_t BPB() { return BPB_t(m_data + 11, 90); } // len: 79
    uint8_t *bootCode() { return (m_data + 90); }  // len: 420
    uint8_t *end() { return (m_data + 510); }      // len: 2

    // operations
    bool isValid() override;
    void info() override;
};

#endif