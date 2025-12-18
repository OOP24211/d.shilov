#include "Mixer_Converter.h"
#include <iostream>

void Mixer_Converter::convert(WAV_Reader &in_stream, WAV_Writer &out_stream, std::string arg) {
    std::stringstream s_string(arg);
    int time_param;
    if (!(s_string >> additional_file_name)) {
        throw std::invalid_argument("Не задан дополнительный поток");
    }
    if (!(s_string >> time_param)) {
        start_time = 0;
    } else {
        start_time = time_param;
    }

    if (start_time < 0) {
        throw std::invalid_argument("Некорректное время вставки");
    }
    try {
        additional_stream = std::make_unique<WAV_Reader>(additional_file_name);
    } catch (const std::exception &e) {
        throw std::runtime_error("Не удалось открыть дополнительный поток");
    }

    long long start_sample = start_time * SAMPLE_RATE;
    long long current_sample = 0;
    while (in_stream.get_sample()) {
        int16_t sample = in_stream.sample;
        int16_t final_sample = sample;
        if (current_sample >= start_sample) {
            if (additional_stream->get_sample()) {
                int16_t secondary_sample = additional_stream->sample;
                final_sample = secondary_sample;
            }
        }
        out_stream.writeSample(final_sample);
        current_sample++;
    }
}

namespace {
    Converter *create_mixer() {
        return new Mixer_Converter;
    }

    bool crt = Factory_Converter::getInstance()->register_converter("mix", create_mixer);
}
