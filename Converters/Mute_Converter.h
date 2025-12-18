#ifndef LAB_3_MUTE_CONVERTER_H
#define LAB_3_MUTE_CONVERTER_H

#include "../src/Converter.h"
#include "../src/WAV_Reader.h"
#include "../src/WAV_Writer.h"
#include <sstream>
#include <string>

class Mute_Converter : public Converter {
private:
    const uint32_t SAMPLE_RATE = 44100;
    int start_time = 0;
    int end_time = 0;

public:
    Mute_Converter() = default;
    void convert(WAV_Reader &in_stream, WAV_Writer &out_stream, std::string arg) override;
};

#endif //LAB_3_MUTE_CONVERTER_H
