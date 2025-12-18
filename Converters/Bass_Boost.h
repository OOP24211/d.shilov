#ifndef LAB_3_BASS_BOOST_H
#define LAB_3_BASS_BOOST_H

#include "../src/Converter.h"
#include "../src/WAV_Reader.h"
#include "../src/WAV_Writer.h"
#include <sstream>
#include <string>
#include <algorithm>

class Bass_Boost : public Converter {
private:
    int gain_factor = 2;
    const int16_t MAX_SAMPLE_VALUE = 32767;
    const int16_t MIN_SAMPLE_VALUE = -32768;

public:
    Bass_Boost() = default;
    void convert(WAV_Reader &in_stream, WAV_Writer &out_stream, std::string arg) override;
};


#endif //LAB_3_BASS_BOST_H