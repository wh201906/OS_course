#ifndef _DATACLUSTER_H_
#define _DATACLUSTER_H_

#include "datahandle.h"

class DataCluster_t : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    //data
    uint8_t *cluster(uint64_t id) { return (id > 1 ? (m_data + (id - 2) * m_SPC * BPS) : nullptr); }
    void setSPC(uint64_t spc) { m_SPC = spc; }
    uint64_t getSPC() { return m_SPC; }

    // operations
    void info() override;

private:
    uint64_t m_SPC; // sector per cluster
};

#endif