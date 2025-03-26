/// WavWriter.cpp

/**
Copyright © 2025 Alex Parisi

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "WavWriter.h"

/**
 * @brief Public constructor that verifies the configuration and creates a
 * WAV file writer object.
 * @param configuration WAV writer configuration
 * @return A WAV writer object if the configuration is valid, std::nullopt
 * otherwise
 */
auto WavWriter::create(WavFileConfiguration configuration)
        -> std::optional<WavWriter> {
    auto obj = WavWriter(std::move(configuration));
    if (!obj.open_file()) {
        return std::nullopt;
    }
    return obj;
}

/**
 * @brief Public destructor
 */
WavWriter::~WavWriter() {
    if (m_fileStream.is_open()) {
        close_file();
    }
}

/**
 * @brief Close the WAV file.
 */
auto WavWriter::close_file() -> void {
    if (!m_fileStream.is_open()) {
        return;
    }
    finalize_header();
    m_fileStream.close();
}

/**
 * @brief Opens the WAV file for writing.
 * @return True if the file was opened successfully, false otherwise
 */
auto WavWriter::open_file() -> bool {
    m_fileStream.open(m_config.filename, std::ios::binary | std::ios::out);
    if (!m_fileStream) {
        return false;
    }
    write_header();
    return true;
}

/**
 * @brief Write the WAV file header.
 */
auto WavWriter::write_header() -> void {
    const auto sampleRate = static_cast<uint32_t>(m_config.sampleRate);
    const auto numChannels = static_cast<uint16_t>(m_config.numChannels);
    const auto bitDepth = static_cast<uint16_t>(m_config.bitDepth);
    // Write the initial header with placeholder values
    m_fileStream.write("RIFF", 4);
    constexpr uint32_t chunkSize = 0;
    m_fileStream.write(reinterpret_cast<const char *>(&chunkSize), 4);
    m_fileStream.write("WAVE", 4);
    m_fileStream.write("fmt ", 4);
    constexpr uint32_t subchunk1Size = 16;
    m_fileStream.write(reinterpret_cast<const char *>(&subchunk1Size), 4);
    const auto audioFormat = static_cast<uint16_t>(m_config.format);
    m_fileStream.write(reinterpret_cast<const char *>(&audioFormat), 2);
    m_fileStream.write(reinterpret_cast<const char *>(&numChannels), 2);
    m_fileStream.write(reinterpret_cast<const char *>(&sampleRate), 4);
    const uint32_t byteRate = sampleRate * numChannels * (bitDepth / 8);
    m_fileStream.write(reinterpret_cast<const char *>(&byteRate), 4);
    const uint16_t blockAlign = numChannels * (bitDepth / 8);
    m_fileStream.write(reinterpret_cast<const char *>(&blockAlign), 2);
    m_fileStream.write(reinterpret_cast<const char *>(&bitDepth), 2);
    m_fileStream.write("data", 4);
    constexpr uint32_t subchunk2Size = 0;
    m_fileStream.write(reinterpret_cast<const char *>(&subchunk2Size), 4);
}

/**
 * @brief Finalize the WAV file header.
 */
auto WavWriter::finalize_header() -> void {
    /// Update the RIFF chunk size and data subchunk size
    const uint32_t chunkSize = 36 + m_totalFileSize;
    m_fileStream.seekp(4, std::ios::beg);
    m_fileStream.write(reinterpret_cast<const char *>(&chunkSize), 4);
    m_fileStream.seekp(40, std::ios::beg);
    m_fileStream.write(reinterpret_cast<const char *>(&m_totalFileSize), 4);
}