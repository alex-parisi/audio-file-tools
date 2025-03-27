/// WavConfiguration.h

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

#ifndef WAV_CONFIGURATION_H
#define WAV_CONFIGURATION_H

#include <iostream>
#include <string>
#include <type_traits>

/** Concept to check if a type is any of a list of types */
template<typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

/** Concept to check if a type is an allowed audio data type */
template<typename T>
concept AllowedAudioDataType =
        IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>,
                float, uint8_t, int16_t, int32_t>;

/** Supported data formats for WAV files */
enum class WavFormat {
    PCM = 1,
    FLOAT = 3,
};

/** Supported sample rates for WAV files */
enum class WavSampleRate {
    SAMPLE_RATE_8000 = 8000,
    SAMPLE_RATE_11025 = 11025,
    SAMPLE_RATE_16000 = 16000,
    SAMPLE_RATE_22050 = 22050,
    SAMPLE_RATE_32000 = 32000,
    SAMPLE_RATE_44100 = 44100,
    SAMPLE_RATE_48000 = 48000,
    SAMPLE_RATE_96000 = 96000,
    SAMPLE_RATE_176400 = 176400,
    SAMPLE_RATE_192000 = 192000,
    SAMPLE_RATE_352800 = 352800,
    SAMPLE_RATE_384000 = 384000,
};

/** Supported bit depths for WAV files */
enum class WavBitDepth {
    BIT_DEPTH_8 = 8,
    BIT_DEPTH_16 = 16,
    BIT_DEPTH_24 = 24,
    BIT_DEPTH_32 = 32,
};

/** Configuration for the WAV writer */
struct WavFileConfiguration {
    std::string filename;
    WavSampleRate sampleRate = WavSampleRate::SAMPLE_RATE_16000;
    uint8_t numChannels = 1;
    WavBitDepth bitDepth = WavBitDepth::BIT_DEPTH_32;
    WavFormat format = WavFormat::FLOAT;
    uint16_t blockAlign = 0;
    uint32_t dataChunkSize = 0;

    /** Get the number of samples in the WAV file */
    [[nodiscard]] uint32_t num_samples() const {
        if (blockAlign == 0) return 0;
        return dataChunkSize / blockAlign;
    }

    /** Print the configuration */
    auto print() const -> void {
        std::cout << "Configuration for: " << filename << std::endl;
        std::cout << "\tChannels: " << static_cast<int>(numChannels) << std::endl;
        std::cout << "\tSample Rate: " << static_cast<int>(sampleRate)
                  << std::endl;
        std::cout << "\tBit Depth: " << static_cast<int>(bitDepth) << std::endl;
        std::cout << "\tFormat: " << static_cast<int>(format) << std::endl;
        std::cout << "\tSample count: " << num_samples() << std::endl;
    }
};

#endif // WAV_CONFIGURATION_H
