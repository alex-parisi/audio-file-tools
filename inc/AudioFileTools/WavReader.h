/// WavWriter.h

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

#ifndef WAV_READER_H
#define WAV_READER_H

#include <fstream>
#include <optional>
#include <string>

#include "WavConfiguration.h"
#include "WavUtils.h"

/**
 * @brief WAV file reader class.
 * @details The WAV file reader class reads audio data from a WAV file.
 */
class WavReader {
public:
    /**
     * @brief Public constructor that verifies the configuration and creates a
     * WAV file reader object.
     * @param filename The filename of the WAV file
     * @return A WAV reader object if the configuration is valid, std::nullopt
     * otherwise
     */
    static auto create(const std::string &filename) -> std::optional<WavReader>;

    /**
     * @brief Public destructor
     */
    ~WavReader();

    /**
     * @brief Close the WAV file.
     */
    auto close_file() -> void;

    /**
     * @brief Gets the reader configuration.
     * @return The reader configuration
     */
    auto get_configuration() -> WavFileConfiguration;

    /**
     * @brief Overloaded move constructor
     * @param other The other WAV reader object
     */
    WavReader(WavReader &&other) noexcept :
        m_config(std::move(other.m_config)),
        m_fileStream(std::move(other.m_fileStream)) {}

    /**
     * @brief Overloaded move assignment operator
     * @param other The other WAV reader object
     * @return The WAV reader object
     */
    WavReader &operator=(WavReader &&other) noexcept {
        if (this != &other) {
            m_config = std::move(other.m_config);
            m_fileStream = std::move(other.m_fileStream);
        }
        return *this;
    }

    /** Delete copy constructor and copy assignment operator */
    WavReader(const WavReader &) = delete;
    WavReader &operator=(const WavReader &) = delete;

    template<AllowedAudioDataType T>
    auto read(const size_t count) -> std::vector<std::vector<T>> {
        if (m_config.format == WavFormat::FLOAT) {
            auto samples = read_samples<float>(count);
            if constexpr (std::same_as<T, float>)
                return samples;
            if constexpr (std::same_as<T, uint8_t>)
                return convert_samples<float, uint8_t>(samples,
                                                       convert_float_to_uint8);
            if constexpr (std::same_as<T, int16_t>)
                return convert_samples<float, int16_t>(samples,
                                                       convert_float_to_int16);
            if constexpr (std::same_as<T, int32_t>)
                return convert_samples<float, int32_t>(samples,
                                                       convert_float_to_int32);
        } else if (m_config.format == WavFormat::PCM) {
            switch (m_config.bitDepth) {
                case WavBitDepth::BIT_DEPTH_8: {
                    auto samples = read_samples<uint8_t>(count);
                    if constexpr (std::same_as<T, float>)
                        return convert_samples<uint8_t, float>(
                                samples, convert_uint8_to_float);
                    if constexpr (std::same_as<T, uint8_t>)
                        return samples;
                    if constexpr (std::same_as<T, int16_t>)
                        return convert_samples<uint8_t, int16_t>(
                                samples, convert_uint8_to_int16);
                    if constexpr (std::same_as<T, int32_t>)
                        return convert_samples<uint8_t, int32_t>(
                                samples, convert_uint8_to_int32);
                    break;
                }
                case WavBitDepth::BIT_DEPTH_16: {
                    auto samples = read_samples<int16_t>(count);
                    if constexpr (std::same_as<T, float>)
                        return convert_samples<int16_t, float>(
                                samples, convert_int16_to_float);
                    if constexpr (std::same_as<T, uint8_t>)
                        return convert_samples<int16_t, uint8_t>(
                                samples, convert_int16_to_uint8);
                    if constexpr (std::same_as<T, int16_t>)
                        return samples;
                    if constexpr (std::same_as<T, int32_t>)
                        return convert_samples<int16_t, int32_t>(
                                samples, convert_int16_to_int32);
                    break;
                }
                case WavBitDepth::BIT_DEPTH_24: {
                    /// TODO - clean this up
                    const auto interleavedSamples =
                            read_raw_samples(count * m_config.numChannels * 3);
                    std::vector<std::vector<int32_t>> samples(
                            m_config.numChannels, std::vector<int32_t>(count));
                    for (size_t i = 0; i < count; ++i) {
                        for (size_t j = 0; j < m_config.numChannels; ++j) {
                            const int32_t sample =
                                    (interleavedSamples
                                             [(i * m_config.numChannels + j) *
                                              3] &
                                     0xFF) |
                                    ((interleavedSamples
                                              [(i * m_config.numChannels + j) *
                                                       3 +
                                               1] &
                                      0xFF)
                                     << 8) |
                                    ((interleavedSamples
                                              [(i * m_config.numChannels + j) *
                                                       3 +
                                               2] &
                                      0xFF)
                                     << 16);
                            samples[j][i] =
                                    (sample & 0x800000)
                                            ? static_cast<int32_t>(sample |
                                                                   0xFF000000)
                                            : sample;
                        }
                    }
                    return convert_samples<int32_t, T>(samples,
                                                       convert_int32_to_float);
                }
                case WavBitDepth::BIT_DEPTH_32: {
                    auto samples = read_samples<int32_t>(count);
                    if constexpr (std::same_as<T, float>)
                        return convert_samples<int32_t, float>(
                                samples, convert_int32_to_float);
                    if constexpr (std::same_as<T, uint8_t>)
                        return convert_samples<int32_t, uint8_t>(
                                samples, convert_int32_to_uint8);
                    if constexpr (std::same_as<T, int16_t>)
                        return convert_samples<int32_t, int16_t>(
                                samples, convert_int32_to_int16);
                    if constexpr (std::same_as<T, int32_t>)
                        return samples;
                    break;
                }
            }
        }
        return {};
    }

private:
    /**
     * @brief Private constructor
     * @param filename The filename of the WAV file
     */
    explicit WavReader(std::string filename) {
        m_config.filename = std::move(filename);
    }

    template<typename From, typename To, typename Converter>
    std::vector<std::vector<To>>
    convert_samples(const std::vector<std::vector<From>> &input,
                    Converter converter) {

        size_t numChannels = input.size();
        size_t count = input[0].size();
        std::vector<std::vector<To>> output(numChannels,
                                            std::vector<To>(count));

        for (size_t i = 0; i < count; ++i) {
            for (size_t j = 0; j < numChannels; ++j) {
                output[j][i] = converter(input[j][i]);
            }
        }
        return output;
    }

    /**
     * @brief Reads an array of samples from the WAV file.
     * @tparam T The type of the samples
     * @param count The number of samples to read
     */
    template<AllowedAudioDataType T>
    auto read_samples(const size_t count) -> std::vector<std::vector<T>> {
        std::vector<T> interleavedSamples(count * m_config.numChannels);
        m_fileStream.read(reinterpret_cast<char *>(interleavedSamples.data()),
                          static_cast<std::streamsize>(
                                  interleavedSamples.size() * sizeof(T)));
        const size_t samplesRead = m_fileStream.gcount() / sizeof(T);
        size_t framesRead = samplesRead / m_config.numChannels;
        std::vector<std::vector<T>> deinterleavedSamples(
                m_config.numChannels, std::vector<T>(framesRead));
        for (size_t i = 0; i < framesRead; ++i) {
            for (size_t j = 0; j < m_config.numChannels; ++j) {
                deinterleavedSamples[j][i] =
                        interleavedSamples[i * m_config.numChannels + j];
            }
        }

        return deinterleavedSamples;
    }

    /**
     * @brief Reads raw samples from the WAV file.
     * @param byteCount The number of bytes to read
     * @return The raw samples
     */
    auto read_raw_samples(size_t byteCount) -> std::vector<uint8_t>;

    /**
     * @brief Opens the WAV file for reading.
     * @return True if the file was opened successfully, false otherwise
     */
    auto open_file() -> bool;

    /**
     * @brief Read the WAV file header.
     */
    auto read_header() -> bool;

    /**
     * @brief Gets the number of samples in the WAV file.
     * @return The number of samples
     */
    auto num_samples() const -> uint32_t;

    /** The configuration for the WAV reader */
    WavFileConfiguration m_config = {};

    /** The file stream for the WAV file */
    std::ifstream m_fileStream;
};

#endif // WAV_READER_H
