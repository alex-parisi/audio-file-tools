/// WavReader.cpp

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

#include <AudioFileTools/WavReader.h>

/**
 * @brief Public constructor that verifies the configuration and creates a
 * WAV file reader object.
 * @param filename The filename of the WAV file
 * @return A WAV reader object if the configuration is valid, std::nullopt
 * otherwise
 */
auto WavReader::create(const std::string &filename)
        -> std::optional<WavReader> {
    auto obj = WavReader(filename);
    if (!obj.open_file()) {
        return std::nullopt;
    }
    return obj;
}

/**
 * @brief Public destructor
 */
WavReader::~WavReader() {
    if (m_fileStream.is_open()) {
        close_file();
    }
}

/**
 * @brief Close the WAV file.
 */
auto WavReader::close_file() -> void {
    if (!m_fileStream.is_open()) {
        return;
    }
    m_fileStream.close();
}

/**
 * @brief Gets the reader configuration.
 * @return The reader configuration
 */
auto WavReader::get_configuration() -> WavFileConfiguration { return m_config; }

/**
 * @brief Reads raw samples from the WAV file.
 * @param byteCount The number of bytes to read
 * @return The raw samples
 */
auto WavReader::read_raw_samples(const size_t byteCount)
        -> std::vector<uint8_t> {
    std::vector<uint8_t> buffer(byteCount);
    m_fileStream.read(reinterpret_cast<char *>(buffer.data()),
                      static_cast<std::streamsize>(byteCount));
    return buffer;
}

/**
 * @brief Opens the WAV file for reading.
 * @return True if the file was opened successfully, false otherwise
 */
auto WavReader::open_file() -> bool {
    m_fileStream.open(m_config.filename, std::ios::binary | std::ios::in);
    if (!m_fileStream) {
        return false;
    }
    return read_header();
}

/**
 * @brief Reads the WAV file header and verifies that the configuration is
 * valid.
 */
auto WavReader::read_header() -> bool {
    // Read RIFF header
    std::array<char, 4> chunkId{};
    m_fileStream.read(chunkId.data(), chunkId.size());
    if (std::strncmp(chunkId.data(), "RIFF", 4) != 0) return false;

    uint32_t chunkSize;
    m_fileStream.read(reinterpret_cast<char *>(&chunkSize), sizeof(chunkSize));

    std::array<char, 4> format{};
    m_fileStream.read(format.data(), format.size());
    if (std::strncmp(format.data(), "WAVE", 4) != 0) return false;

    bool foundFmt = false;
    bool foundData = false;

    while (m_fileStream && (!foundFmt || !foundData)) {
        std::array<char, 4> subchunkId{};
        m_fileStream.read(subchunkId.data(), 4);
        if (m_fileStream.gcount() != 4) break;

        uint32_t subchunkSize = 0;
        m_fileStream.read(reinterpret_cast<char *>(&subchunkSize), sizeof(subchunkSize));

        if (std::strncmp(subchunkId.data(), "fmt ", 4) == 0) {
            foundFmt = true;

            uint16_t audioFormat, numChannels;
            uint32_t sampleRate, byteRate;
            uint16_t blockAlign, bitDepth;

            m_fileStream.read(reinterpret_cast<char *>(&audioFormat), sizeof(audioFormat));
            m_fileStream.read(reinterpret_cast<char *>(&numChannels), sizeof(numChannels));
            m_fileStream.read(reinterpret_cast<char *>(&sampleRate), sizeof(sampleRate));
            m_fileStream.read(reinterpret_cast<char *>(&byteRate), sizeof(byteRate));
            m_fileStream.read(reinterpret_cast<char *>(&blockAlign), sizeof(blockAlign));
            m_fileStream.read(reinterpret_cast<char *>(&bitDepth), sizeof(bitDepth));

            // Skip any extra fmt bytes
            std::streamoff fmtExtraBytes = static_cast<std::streamoff>(subchunkSize) - 16;
            if (fmtExtraBytes > 0) {
                m_fileStream.seekg(fmtExtraBytes, std::ios::cur);
            }

            // Assign to config
            if (audioFormat == 1 || audioFormat == 3) {
                m_config.format = static_cast<WavFormat>(audioFormat);
            } else return false;

            if (numChannels > 0) {
                m_config.numChannels = numChannels;
            } else return false;

            if (bitDepth == 8 || bitDepth == 16 || bitDepth == 24 || bitDepth == 32) {
                m_config.bitDepth = static_cast<WavBitDepth>(bitDepth);
            } else return false;

            if (sampleRate == 8000 || sampleRate == 11025 || sampleRate == 16000 ||
                sampleRate == 22050 || sampleRate == 32000 || sampleRate == 44100 ||
                sampleRate == 48000 || sampleRate == 96000 || sampleRate == 176400 ||
                sampleRate == 192000 || sampleRate == 352800 || sampleRate == 384000) {
                m_config.sampleRate = static_cast<WavSampleRate>(sampleRate);
            } else return false;

            m_config.blockAlign = blockAlign;

            // Validate config
            if (m_config.format == WavFormat::FLOAT && m_config.bitDepth != WavBitDepth::BIT_DEPTH_32) {
                return false;
            }

        } else if (std::strncmp(subchunkId.data(), "data", 4) == 0) {
            foundData = true;
            m_config.dataChunkSize = subchunkSize;
        } else {
            // Skip unknown or unneeded chunk (and pad if odd size)
            m_fileStream.seekg((subchunkSize + 1) & ~1, std::ios::cur);
        }

        if (m_fileStream.fail()) return false;
    }

    return foundFmt && foundData;
}

auto WavReader::num_samples() const -> uint32_t {
    if (m_config.blockAlign == 0) return 0;
    return m_config.dataChunkSize / m_config.blockAlign;
}