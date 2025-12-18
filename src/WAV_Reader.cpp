#include "WAV_Reader.h"
#include <cstring>

WAV_Reader::WAV_Reader(const std::string file) {
    wav_file.open(file, std::ios::binary);
    if (!wav_file.is_open()) {
        throw std::invalid_argument("Не удалось открыть файл");
    }
    wav_file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header.audioFormat != 1 || header.numChannels != 1 || header.sampleRate != 44100) {
        throw std::invalid_argument("Формат файла не поддерживается");
    }
    char current_chunk_id[4];
    uint32_t current_chunk_size = 0;
    while (true) {
        if (!wav_file.read(current_chunk_id, 4)) {
            throw std::invalid_argument("Файл WAV поврежден: фрагмент 'data' не найден или файл обрезан.");
        }
        if (!wav_file.read(reinterpret_cast<char *>(&current_chunk_size), 4)) {
            throw std::invalid_argument("Файл WAV поврежден: не удалось считать размер фрагмента.");
        }
        if (std::memcmp(current_chunk_id, "data", 4) == 0) {
            std::memcpy(data_chunk.data_id, current_chunk_id, 4);
            data_chunk.data_size = current_chunk_size;
            break;
        }

        wav_file.seekg(current_chunk_size, std::ios_base::cur);
        if (current_chunk_size % 2 != 0) {
            wav_file.seekg(1, std::ios_base::cur);
        }
    }
}

bool WAV_Reader::get_sample() {
    if (wav_file.read(reinterpret_cast<char *>(&sample), sizeof(sample))) {
        samples_read++;
        return true;
    }
    return false;
}
