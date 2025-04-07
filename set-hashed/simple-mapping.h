#ifndef SIMPLE_MAPPING_H
#define SIMPLE_MAPPING_H

#include <cstddef>
#include <string>

size_t mapIntRangeSeq(int x, int n, int m);
size_t mapIntRangeStep2(int x, int n, int m);
size_t mapCharAz(char c);
size_t mapStringAz2(const std::string &s);

#endif // SIMPLE_MAPPING_H
