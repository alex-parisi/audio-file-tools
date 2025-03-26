/// WavWriterTest.cpp

#include <gtest/gtest.h>
#include "WavReader.h"
#include "WavWriter.h"

#include <fstream>
#include <vector>

TEST(WavWriterTest, CreateValidWavWriter) {
    const WavFileConfiguration config = {
            .filename = "test.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::FLOAT,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    const std::ifstream file(config.filename);
    EXPECT_TRUE(file.good());
    writer->close_file();
    /// Try to read it back
    auto reader = WavReader::create(config.filename);
    ASSERT_TRUE(reader.has_value());
    auto [filename, sampleRate, numChannels, bitDepth, format] = reader->get_configuration();
    EXPECT_EQ(config.filename, filename);
    EXPECT_EQ(config.sampleRate, sampleRate);
    EXPECT_EQ(config.numChannels, numChannels);
    EXPECT_EQ(config.format, format);
    EXPECT_EQ(config.bitDepth, bitDepth);
    reader->close_file();
    std::remove(config.filename.c_str());
}

TEST(WavWriterTest, WriteFloatBufferToFloat32) {
    const WavFileConfiguration config = {
            .filename = "float32-in_float32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::FLOAT,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<float> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<float>(
                0.5 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
    /// Now read it back
    auto reader = WavReader::create(config.filename);
    ASSERT_TRUE(reader.has_value());
    auto [filename, sampleRate, numChannels, bitDepth, format] = reader->get_configuration();
    EXPECT_EQ(config.filename, filename);
    EXPECT_EQ(config.sampleRate, sampleRate);
    EXPECT_EQ(config.numChannels, numChannels);
    EXPECT_EQ(config.format, format);
    EXPECT_EQ(config.bitDepth, bitDepth);
    const auto readSamples = reader->read<float>(44100);
    for (size_t i = 0; i < 44100; ++i) {
        EXPECT_FLOAT_EQ(samples[i], readSamples[0][i]);
    }
    reader->close_file();
    std::remove(config.filename.c_str());
}

TEST(WavWriterTest, WriteFloatBufferToPCM8) {
    const WavFileConfiguration config = {
            .filename = "float32-in_pcm8-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_8,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<float> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<float>(
                0.5 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
    /// Now read it back
    auto reader = WavReader::create(config.filename);
    ASSERT_TRUE(reader.has_value());
    WavFileConfiguration readConfig = reader->get_configuration();
    EXPECT_EQ(config.filename, readConfig.filename);
    EXPECT_EQ(config.sampleRate, readConfig.sampleRate);
    EXPECT_EQ(config.numChannels, readConfig.numChannels);
    EXPECT_EQ(config.format, readConfig.format);
    EXPECT_EQ(config.bitDepth, readConfig.bitDepth);
    auto readSamples = reader->read<float>(44100);
    for (size_t i = 0; i < 44100; ++i) {
        EXPECT_NEAR(samples[i], readSamples[0][i], 0.01);
    }
    reader->close_file();
    std::remove(config.filename.c_str());
}

TEST(WavWriterTest, WriteFloatBufferToPCM16) {
    const WavFileConfiguration config = {
            .filename = "float32-in_pcm16-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_16,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<float> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<float>(
                0.5 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
    /// Now read it back
    auto reader = WavReader::create(config.filename);
    ASSERT_TRUE(reader.has_value());
    WavFileConfiguration readConfig = reader->get_configuration();
    EXPECT_EQ(config.filename, readConfig.filename);
    EXPECT_EQ(config.sampleRate, readConfig.sampleRate);
    EXPECT_EQ(config.numChannels, readConfig.numChannels);
    EXPECT_EQ(config.format, readConfig.format);
    EXPECT_EQ(config.bitDepth, readConfig.bitDepth);
    auto readSamples = reader->read<float>(44100);
    for (size_t i = 0; i < 44100; ++i) {
        EXPECT_NEAR(samples[i], readSamples[0][i], 0.01);
    }
    reader->close_file();
    std::remove(config.filename.c_str());
}

TEST(WavWriterTest, WriteFloatBufferToPCM24) {
    const WavFileConfiguration config = {
            .filename = "float32-in_pcm24-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_24,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<float> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<float>(
                0.5 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
    /// Now read it back
    // auto reader = WavReader::create(config.filename);
    // ASSERT_TRUE(reader.has_value());
    // WavFileConfiguration readConfig = reader->get_configuration();
    // EXPECT_EQ(config.filename, readConfig.filename);
    // EXPECT_EQ(config.sampleRate, readConfig.sampleRate);
    // EXPECT_EQ(config.numChannels, readConfig.numChannels);
    // EXPECT_EQ(config.format, readConfig.format);
    // EXPECT_EQ(config.bitDepth, readConfig.bitDepth);
    // auto readSamples = reader->read<float>(44100);
    // for (size_t i = 0; i < 44100; ++i) {
    //     EXPECT_NEAR(samples[i], readSamples[0][i], 0.01);
    // }
    // reader->close_file();
    // std::remove(config.filename.c_str());
}

TEST(WavWriterTest, WriteFloatBufferToPCM32) {
    const WavFileConfiguration config = {
            .filename = "float32-in_pcm32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<float> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<float>(
                0.5 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
    /// Now read it back
    auto reader = WavReader::create(config.filename);
    ASSERT_TRUE(reader.has_value());
    WavFileConfiguration readConfig = reader->get_configuration();
    EXPECT_EQ(config.filename, readConfig.filename);
    EXPECT_EQ(config.sampleRate, readConfig.sampleRate);
    EXPECT_EQ(config.numChannels, readConfig.numChannels);
    EXPECT_EQ(config.format, readConfig.format);
    EXPECT_EQ(config.bitDepth, readConfig.bitDepth);
    auto readSamples = reader->read<float>(44100);
    for (size_t i = 0; i < 44100; ++i) {
        EXPECT_NEAR(samples[i], readSamples[0][i], 0.01);
    }
    reader->close_file();
    std::remove(config.filename.c_str());
}

TEST(WavWriterTest, WritePCM8BufferToFloat32) {
    const WavFileConfiguration config = {
            .filename = "pcm8-in_float32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::FLOAT,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<uint8_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<uint8_t>(
                127.5 / 2.0 *
                        std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) /
                                 44100.0) +
                127.5);
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
    /// Now read it back
    auto reader = WavReader::create(config.filename);
    ASSERT_TRUE(reader.has_value());
    WavFileConfiguration readConfig = reader->get_configuration();
    EXPECT_EQ(config.filename, readConfig.filename);
    EXPECT_EQ(config.sampleRate, readConfig.sampleRate);
    EXPECT_EQ(config.numChannels, readConfig.numChannels);
    EXPECT_EQ(config.format, readConfig.format);
    EXPECT_EQ(config.bitDepth, readConfig.bitDepth);
    auto readSamples = reader->read<uint8_t>(44100);
    for (size_t i = 0; i < 44100; ++i) {
        EXPECT_NEAR(samples[i], readSamples[0][i], 1);
    }
    reader->close_file();
    std::remove(config.filename.c_str());
}

TEST(WavWriterTest, WritePCM8BufferToPCM8) {
    const WavFileConfiguration config = {
            .filename = "pcm8-in_pcm8-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_8,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<uint8_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<uint8_t>(
                127.5 / 2.0 *
                        std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) /
                                 44100.0) +
                127.5);
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();

}

TEST(WavWriterTest, WritePCM8BufferToPCM16) {
    const WavFileConfiguration config = {
            .filename = "pcm8-in_pcm18-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_16,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<uint8_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<uint8_t>(
                127.5 / 2.0 *
                        std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) /
                                 44100.0) +
                127.5);
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM8BufferToPCM24) {
    const WavFileConfiguration config = {
            .filename = "pcm8-in_pcm24-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_24,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<uint8_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<uint8_t>(
                127.5 / 2.0 *
                        std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) /
                                 44100.0) +
                127.5);
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM8BufferToPCM32) {
    const WavFileConfiguration config = {
            .filename = "pcm8-in_pcm32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<uint8_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<uint8_t>(
                127.5 / 2.0 *
                        std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) /
                                 44100.0) +
                127.5);
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM16BufferToFloat32) {
    const WavFileConfiguration config = {
            .filename = "pcm16-in_float32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::FLOAT,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int16_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int16_t>(
                32767.5 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM16BufferToPCM8) {
    const WavFileConfiguration config = {
            .filename = "pcm16-in_pcm8-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_8,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int16_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int16_t>(
                32767.5 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM16BufferToPCM16) {
    const WavFileConfiguration config = {
            .filename = "pcm16-in_pcm16-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_16,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int16_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int16_t>(
                32767.5 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM16BufferToPCM24) {
    const WavFileConfiguration config = {
            .filename = "pcm16-in_pcm24-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_24,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int16_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int16_t>(
                32767.5 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM16BufferToPCM32) {
    const WavFileConfiguration config = {
            .filename = "pcm16-in_pcm32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int16_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int16_t>(
                32767.5 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM32BufferToFloat32) {
    const WavFileConfiguration config = {
            .filename = "pcm32-in_float32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::FLOAT,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int32_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int32_t>(
                2147483647.0 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM32BufferToPCM8) {
    const WavFileConfiguration config = {
            .filename = "pcm32-in_pcm8-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_8,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int32_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int32_t>(
                2147483647.0 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM32BufferToPCM16) {
    const WavFileConfiguration config = {
            .filename = "pcm32-in_pcm16-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_16,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int32_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int32_t>(
                2147483647.0 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM32BufferToPCM24) {
    const WavFileConfiguration config = {
            .filename = "pcm32-in_pcm24-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_24,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int32_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int32_t>(
                2147483647.0 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}

TEST(WavWriterTest, WritePCM32BufferToPCM32) {
    const WavFileConfiguration config = {
            .filename = "pcm32-in_pcm32-out.wav",
            .sampleRate = WavSampleRate::SAMPLE_RATE_44100,
            .numChannels = 1,
            .format = WavFormat::PCM,
            .bitDepth = WavBitDepth::BIT_DEPTH_32,
    };
    auto writer = WavWriter::create(config);
    ASSERT_TRUE(writer.has_value());
    /// Write one second of a 1 kHz sine wave
    std::vector<int32_t> samples(44100);
    for (size_t i = 0; i < 44100; ++i) {
        samples[i] = static_cast<int32_t>(
                2147483647.0 / 2.0 *
                std::sin(2.0 * M_PI * 5.0 * static_cast<double>(i) / 44100.0));
    }
    writer->write(samples.size(), samples.data());
    writer->close_file();
}
