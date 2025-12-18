#include "Bass_Boost.h"
#include "../src/Factory.h"
#include <cmath>

void Bass_Boost::convert(WAV_Reader &in_stream, WAV_Writer &out_stream, std::string arg) {
    if (!arg.empty()) {
        std::stringstream ss(arg);
        if (!(ss >> gain_factor)) {
            throw std::invalid_argument("Некорректный аргумент");
        }
    }
    while (in_stream.get_sample()) {
        int16_t sample_value = in_stream.sample;
        double boosted_sample = (double) sample_value * gain_factor;
        int16_t final_sample = (int16_t) std::round(std::min((double) MAX_SAMPLE_VALUE,
                                                             std::max((double) MIN_SAMPLE_VALUE, boosted_sample)));
        out_stream.writeSample(final_sample);
    }
    out_stream.close();
}

namespace {
    Converter *create() {
        return new Bass_Boost;
    }
    bool crt = Factory_Converter::getInstance()->register_converter("bass", create);
}
