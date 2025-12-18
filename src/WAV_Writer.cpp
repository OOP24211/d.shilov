#include "WAV_Writer.h"
#include <cstring>
#include <iostream>

WAV_Writer::WAV_Writer(const std::string& filename) { 

    file_stream.open(filename, std::ios::binary | std::ios::out);

    if (!file_stream.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }
    file_stream.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));
    file_stream.write(reinterpret_cast<const char*>(&data_chunk), sizeof(data_chunk));
}

void WAV_Writer::updateHeaderFields() {
    data_chunk.data_size = static_cast<uint32_t>(data_size_bytes);
    header.chunkSize = sizeof(WavHeader) - 8 + sizeof(data_chunk) + data_size_bytes;
    header.byteRate = header.sampleRate * header.numChannels * bytes_per_sample;
    header.blockAlign = header.numChannels * bytes_per_sample;
}

void WAV_Writer::writeSample(int16_t sampleValue) {
    if (!file_stream.is_open()) {
        throw std::runtime_error("Файл не открыт");
    }
    file_stream.write(reinterpret_cast<const char*>(&sampleValue), bytes_per_sample);
    data_size_bytes += bytes_per_sample;
}

void WAV_Writer::close() {
    if (!file_stream.is_open()) {
        return; 
    }
    updateHeaderFields();
    file_stream.seekp(0, std::ios::beg);
    file_stream.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));
    file_stream.write(reinterpret_cast<const char*>(&data_chunk), sizeof(data_chunk));
    file_stream.close();
}