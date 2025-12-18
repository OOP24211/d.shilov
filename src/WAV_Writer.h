#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include "WAV_Structs.h"

#ifndef LAB_3_WAV_WRITER_H
#define LAB_3_WAV_WRITER_H


const uint32_t FIXED_SAMPLE_RATE = 44100;

class WAV_Writer {
private:
    std::ofstream file_stream;
    long data_size_bytes = 0;
    const uint16_t bytes_per_sample = 2;
    WavHeader header;
    Wav_Data data_chunk;

    void updateHeaderFields();

public:
    WAV_Writer(const std::string &filename);

    void writeSample(int16_t sampleValue);

    void close();
};

#endif //LAB_3_WAV_WRITER_H
