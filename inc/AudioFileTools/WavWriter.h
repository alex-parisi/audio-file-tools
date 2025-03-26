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

#ifndef WAV_WRITER_H
#define WAV_WRITER_H

#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "WavConfiguration.h"
#include "WavUtils.h"

/**
 * @brief WAV file writer class.
 * @details The WAV file writer class writes audio data to a WAV file.
 */
class WavWriter {
public:
    /**
     * @brief Public constructor that verifies the configuration and creates a
     * WAV file writer object.
     * @param configuration WAV writer configuration
     * @return A WAV writer object if the configuration is valid, std::nullopt
     * otherwise
     */
    static auto create(WavFileConfiguration configuration)
            -> std::optional<WavWriter>;

    /**
     * @brief Public destructor
     */
    ~WavWriter();

    /**
     * @brief Writes float audio data to the WAV file.
     * @param samples Pointer to the first channel of audio data.
     * @param rest Other audio channels
     * @param count Number of samples to write.
     */
    template<typename T, typename... Args>
    auto write(const size_t count, const T *samples, Args... rest) -> void {
        /// Assemble input arrays into a single array
        constexpr size_t num_arrays = sizeof...(rest) + 1;
        assert(num_arrays == m_config.numChannels);
        std::array<const T *, num_arrays> sampleArrays = {samples, rest...};
        write_buffer(sampleArrays.data(), count);
    }

    /**
     * @brief Close the WAV file.
     */
    auto close_file() -> void;

    /**
     * @brief Overloaded move constructor
     * @param other The other WAV writer object
     */
    WavWriter(WavWriter &&other) noexcept :
        m_config(std::move(other.m_config)),
        m_fileStream(std::move(other.m_fileStream)) {}

    /**
     * @brief Overloaded move assignment operator
     * @param other The other WAV writer object
     * @return The WAV writer object
     */
    WavWriter &operator=(WavWriter &&other) noexcept {
        if (this != &other) {
            m_config = std::move(other.m_config);
            m_fileStream = std::move(other.m_fileStream);
        }
        return *this;
    }

    /** Delete copy constructor and copy assignment operator */
    WavWriter(const WavWriter &) = delete;
    WavWriter &operator=(const WavWriter &) = delete;

private:
    /**
     * @brief Private constructor
     * @param configuration The configuration for the WAV writer
     */
    explicit WavWriter(WavFileConfiguration configuration) :
        m_config(std::move(configuration)) {}

    auto write_to_float32(AllowedAudioDataType auto *const *sampleArrays,
                          const size_t count) -> void {
        using DataType = std::remove_cv_t<
                std::remove_pointer_t<std::remove_cv_t<std::remove_pointer_t<
                        std::decay_t<decltype(sampleArrays)>>>>>;
        assert(m_config.bitDepth == WavBitDepth::BIT_DEPTH_32);
        std::vector<float> interleavedSamples(count * m_config.numChannels);
        for (size_t i = 0; i < count; ++i) {
            for (size_t ch = 0; ch < m_config.numChannels; ++ch) {
                /// Convert the input data type to float
                if constexpr (std::is_same_v<DataType, float>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            sampleArrays[ch][i];
                } else if constexpr (std::is_same_v<DataType, uint8_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_uint8_to_float(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int16_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_int16_to_float(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int32_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_int32_to_float(sampleArrays[ch][i]);
                }
            }
        }
        // Write interleaved samples to the file
        write_samples(interleavedSamples);
    }

    auto write_to_pcm8(AllowedAudioDataType auto *const *sampleArrays,
                       const size_t count) -> void {
        using DataType = std::remove_cv_t<
                std::remove_pointer_t<std::remove_cv_t<std::remove_pointer_t<
                        std::decay_t<decltype(sampleArrays)>>>>>;
        // Allocate a buffer to hold interleaved samples
        std::vector<uint8_t> interleavedSamples(count * m_config.numChannels);
        for (size_t i = 0; i < count; ++i) {
            for (size_t ch = 0; ch < m_config.numChannels; ++ch) {
                /// Convert the input data type to uint8_t
                if constexpr (std::is_same_v<DataType, float>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_float_to_uint8(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, uint8_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            sampleArrays[ch][i];
                } else if constexpr (std::is_same_v<DataType, int16_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_int16_to_uint8(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int32_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_int32_to_uint8(sampleArrays[ch][i]);
                }
            }
        }
        // Write interleaved samples to the file
        write_samples(interleavedSamples);
    }

    auto write_to_pcm16(AllowedAudioDataType auto *const *sampleArrays,
                        const size_t count) -> void {
        using DataType = std::remove_cv_t<
                std::remove_pointer_t<std::remove_cv_t<std::remove_pointer_t<
                        std::decay_t<decltype(sampleArrays)>>>>>;
        // Allocate a buffer to hold interleaved samples
        std::vector<int16_t> interleavedSamples(count * m_config.numChannels);
        for (size_t i = 0; i < count; ++i) {
            for (size_t ch = 0; ch < m_config.numChannels; ++ch) {
                /// Convert the input data type to int16_t
                if constexpr (std::is_same_v<DataType, float>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_float_to_int16(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, uint8_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_uint8_to_int16(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int16_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            sampleArrays[ch][i];
                } else if constexpr (std::is_same_v<DataType, int32_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_int32_to_int16(sampleArrays[ch][i]);
                }
            }
        }
        // Write interleaved samples to the file
        write_samples(interleavedSamples);
    }

    auto write_to_pcm24(AllowedAudioDataType auto *const *sampleArrays,
                        const size_t count) -> void {
        using DataType = std::remove_cv_t<
                std::remove_pointer_t<std::remove_cv_t<std::remove_pointer_t<
                        std::decay_t<decltype(sampleArrays)>>>>>;
        // Allocate a buffer to hold interleaved samples
        std::vector<uint8_t> interleavedSamples(count * m_config.numChannels *
                                                3);
        for (size_t i = 0; i < count; ++i) {
            for (size_t ch = 0; ch < m_config.numChannels; ++ch) {
                /// Convert the input data type to int32_t
                int32_t sample = 0;
                if constexpr (std::is_same_v<DataType, float>) {
                    sample = convert_float_to_int24(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, uint8_t>) {
                    sample = convert_uint8_to_int24(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int16_t>) {
                    sample = convert_int16_to_int24(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int32_t>) {
                    sample = convert_int32_to_int24(sampleArrays[ch][i]);
                }
                interleavedSamples[((i * m_config.numChannels + ch) * 3 + 0) *
                                           m_config.numChannels +
                                   ch] = static_cast<uint8_t>(sample & 0xFF);
                interleavedSamples[((i * m_config.numChannels + ch) * 3 + 1) *
                                           m_config.numChannels +
                                   ch] =
                        static_cast<uint8_t>((sample >> 8) & 0xFF);
                interleavedSamples[((i * m_config.numChannels + ch) * 3 + 2) *
                                           m_config.numChannels +
                                   ch] =
                        static_cast<uint8_t>((sample >> 16) & 0xFF);
            }
        }
        // Write interleaved samples to the file
        write_samples(interleavedSamples, true);
    }

    auto write_to_pcm32(AllowedAudioDataType auto *const *sampleArrays,
                        const size_t count) -> void {
        using DataType = std::remove_cv_t<
                std::remove_pointer_t<std::remove_cv_t<std::remove_pointer_t<
                        std::decay_t<decltype(sampleArrays)>>>>>;
        // Allocate a buffer to hold interleaved samples
        std::vector<int32_t> interleavedSamples(count * m_config.numChannels);
        for (size_t i = 0; i < count; ++i) {
            for (size_t ch = 0; ch < m_config.numChannels; ++ch) {
                /// Convert the input data type to int32_t
                if constexpr (std::is_same_v<DataType, float>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_float_to_int32(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, uint8_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_uint8_to_int32(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int16_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            convert_int16_to_int32(sampleArrays[ch][i]);
                } else if constexpr (std::is_same_v<DataType, int32_t>) {
                    interleavedSamples[i * m_config.numChannels + ch] =
                            sampleArrays[ch][i];
                }
            }
        }
        // Write interleaved samples to the file
        write_samples(interleavedSamples);
    }

    /**
     * @brief Writes an array of samples to the WAV file.
     * @param sampleArrays The array of samples. The first dimension
     * represents the channel, and the second dimension represents the sample.
     * @param count The number of samples to write
     */
    auto write_buffer(AllowedAudioDataType auto *const *sampleArrays,
                      const size_t count) -> void {
        /// Float-32 output format
        if (m_config.format == WavFormat::FLOAT) {
            write_to_float32(sampleArrays, count);
        } else if (m_config.format == WavFormat::PCM) {
            switch (m_config.bitDepth) {
                /// PCM-8 output format
                case WavBitDepth::BIT_DEPTH_8: {
                    write_to_pcm8(sampleArrays, count);
                    break;
                }
                /// PCM-16 output format
                case WavBitDepth::BIT_DEPTH_16: {
                    write_to_pcm16(sampleArrays, count);
                    break;
                }
                /// PCM-24 output format
                case WavBitDepth::BIT_DEPTH_24: {
                    write_to_pcm24(sampleArrays, count);
                    break;
                }
                /// PCM-32 output format
                case WavBitDepth::BIT_DEPTH_32: {
                    write_to_pcm32(sampleArrays, count);
                    break;
                }
            }
        }
    }

    /**
     * @brief Writes an array of samples to the WAV file.
     * @tparam T The type of the samples
     * @param formattedSamples The array of samples already formatted in the
     * output format.
     * @param ignoreSize Whether to ignore the size of the samples, which is
     * used when writing PCM24 samples, since there is no native int24 datatype
     */
    template<AllowedAudioDataType T>
    auto write_samples(const std::vector<T> &formattedSamples,
                       const bool ignoreSize = false) -> void {
        m_fileStream.write(
                reinterpret_cast<const char *>(formattedSamples.data()),
                static_cast<std::streamsize>(formattedSamples.size() *
                                             sizeof(T)));
        if (ignoreSize) {
            m_totalFileSize += formattedSamples.size();
        } else {
            m_totalFileSize += formattedSamples.size() * sizeof(T);
        }
    }

    /**
     * @brief Opens the WAV file for writing.
     * @return True if the file was opened successfully, false otherwise
     */
    auto open_file() -> bool;

    /**
     * @brief Write the WAV file header.
     */
    auto write_header() -> void;

    /**
     * @brief Finalize the WAV file header.
     */
    auto finalize_header() -> void;

    /** The configuration for the WAV writer */
    WavFileConfiguration m_config;

    /** The file stream for the WAV file */
    std::ofstream m_fileStream;

    /** The total file size */
    uint32_t m_totalFileSize = 0;
};

#endif // WAV_WRITER_H
