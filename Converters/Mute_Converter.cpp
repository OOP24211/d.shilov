#include "Mute_Converter.h"
#include "../src/Factory.h"


void Mute_Converter::convert(WAV_Reader &in_stream, WAV_Writer &out_stream, std::string arg) {
    std::stringstream ss(arg);
    if (!(ss >> start_time >> end_time)) {
        throw std::invalid_argument("Некорректный аргумент для Mute_Converter");
    }
    if (start_time < 0 || end_time < start_time) {
        throw std::invalid_argument("Некорректный интервал времени");
    }

    long long start_sample = start_time * SAMPLE_RATE;
    long long end_sample = end_time * SAMPLE_RATE;
    long long current_sample = 0;
    while (in_stream.get_sample()) {
        int16_t sample_value = in_stream.sample;

        if (current_sample >= start_sample && current_sample < end_sample) {
            out_stream.writeSample(0);
        } else {
            out_stream.writeSample(sample_value);
        }
        current_sample++;
    }
    out_stream.close();
}

namespace {
    Converter *create() {
        return new Mute_Converter;
    }

    bool crt = Factory_Converter::getInstance()->register_converter("mute", create);
}
