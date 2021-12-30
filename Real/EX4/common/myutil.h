#ifndef _MYUTIL_H_
#define _MYUTIL_H_

#include "mycommon.h"
#include <regex>
#include <string>
#include <vector>

CHS_t LBA2CHS(uint64_t LBA);
uint64_t CHS2LBA(CHS_t CHS);
std::vector<std::string> split(const char *str, const char *splitter);
void printHex(uint8_t *data, uint64_t len);

#endif