#ifndef LAB_3_CONVERTER_H
#define LAB_3_CONVERTER_H
#include <string>
#include "WAV_Reader.h"
#include "WAV_Writer.h"

class Converter {
public:
    virtual ~Converter() = default;
    virtual void convert(WAV_Reader& in_stream, WAV_Writer& out_stream, std::string arg) = 0;
};

#endif //LAB_3_CONVERTER_H