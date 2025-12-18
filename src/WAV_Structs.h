#ifndef LAB_3_WAV_STRUCTS_H
#define LAB_3_WAV_STRUCTS_H

#ifndef LAB_3_WAV_STRUCTURES_H
#define LAB_3_WAV_STRUCTURES_H

#include <cstdint>
#pragma pack(push, 1)

struct WavHeader {
    char     riff[4] = {'R', 'I', 'F', 'F'};
    uint32_t chunkSize = 0;
    char     wave[4] = {'W', 'A', 'V', 'E'};
    char     fmt[4] = {'f', 'm', 't', ' '};
    uint32_t subchunk1Size = 16;
    uint16_t audioFormat = 1;
    uint16_t numChannels = 1;
    uint32_t sampleRate = 44100;
    uint32_t byteRate = 0;
    uint16_t blockAlign = 0;
    uint16_t bitsPerSample = 16;
};

struct Wav_Data {
    char     data_id[4] = {'d', 'a', 't', 'a'};
    uint32_t data_size = 0;
};

#pragma pack(pop)

#endif // LAB_3_WAV_STRUCTURES_H

#endif //LAB_3_WAV_STRUCTS_H