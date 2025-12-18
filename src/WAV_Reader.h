#ifndef OOP_LABS_WAV_READER_H
#define OOP_LABS_WAV_READER_H

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include "WAV_Structs.h"

class WAV_Reader {
private:
    std::ifstream wav_file;
    size_t samples_read = 0;
    WavHeader header;
    Wav_Data data_chunk;
public:
    int16_t sample = 0;
    WAV_Reader(const std::string file);
    bool get_sample();
};


#endif