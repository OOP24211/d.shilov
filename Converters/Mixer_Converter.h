#ifndef LAB_3_MIXER_CONVERTER_H
#define LAB_3_MIXER_CONVERTER_H


#include "../src/Converter.h"
#include "../src/WAV_Reader.h"
#include "../src/Factory.h"
#include <sstream>
#include <string>

class Mixer_Converter : public Converter {
private:
    const uint32_t SAMPLE_RATE = 44100;
    std::unique_ptr<WAV_Reader> additional_stream;
    std::string additional_file_name;
    int start_time = 0;

public:
    Mixer_Converter() = default;
    void convert(WAV_Reader &in_stream, WAV_Writer &out_stream, std::string arg) override;
};


#endif //LAB_3_MIXER_CONVERTER_H