#ifndef _DENTRY_H_
#define _DENTRY_H_

#include "datahandle.h"

// any time/date related fields are set to zero
// doesn't support long filename now
class DEntry_t : public DataHandle
{
public:
    enum AttrFlag : uint8_t
    {
        ReadOnly = 0x01,
        Hidden = 0x02,
        System = 0x04,
        VolumeID = 0x08,
        Directory = 0x10,
        Archive = 0x20,
    };
    // general
    using DataHandle::DataHandle;

    //data
    uint8_t *name() { return (m_data + 0); }       // len: 8
    uint8_t *extension() { return (m_data + 8); }  // len: 3
    uint8_t *attribute() { return (m_data + 11); } // len: 1
    uint32_t getClusterId();                       // len: 2 + 2
    void setClusterId(uint32_t id);
    uint32_t *size() { return (uint32_t *)(m_data + 28); } // len: 4

    // operations
    bool init(uint8_t attr = Archive);
    void info() override;

private:
    uint64_t m_SPC; // sector per cluster
};

#endif