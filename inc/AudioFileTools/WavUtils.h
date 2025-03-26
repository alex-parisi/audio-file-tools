/// WavUtils.h

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

#ifndef WAV_UTILS_H
#define WAV_UTILS_H

#include <cstdint>

/**
 * @brief Helper function to convert a uint8_t sample to a float sample.
 * @param sample The uint8_t sample
 * @return The float sample
 */
auto convert_uint8_to_float(uint8_t sample) -> float;

/**
 * @brief Helper function to convert an int16_t sample to a float sample.
 * @param sample The int16_t sample
 * @return The float sample
 */
auto convert_int16_to_float(int16_t sample) -> float;

/**
 * @brief Helper function to convert an int32_t sample to a float sample.
 * @param sample The int32_t sample
 * @return The float sample
 */
auto convert_int32_to_float(int32_t sample) -> float;

/**
 * @brief Helper function to convert a float sample to an uint8_t sample.
 * @param sample The float sample
 * @return The uint8_t sample
 */
auto convert_float_to_uint8(float sample) -> uint8_t;

/**
 * @brief Helper function to convert an int16_t sample to an uint8_t sample.
 * @param sample The int16_t sample
 * @return The uint8_t sample
 */
auto convert_int16_to_uint8(int16_t sample) -> uint8_t;

/**
 * @brief Helper function to convert an int32_t sample to an uint8_t sample.
 * @param sample The int32_t sample
 * @return The uint8_t sample
 */
auto convert_int32_to_uint8(int32_t sample) -> uint8_t;

/**
 * @brief Helper function to convert a float sample to an int16_t sample.
 * @param sample The float sample
 * @return The int16_t sample
 */
auto convert_float_to_int16(float sample) -> int16_t;

/**
 * @brief Helper function to convert an uint8_t sample to an int16_t sample.
 * @param sample The uint8_t sample
 * @return The int16_t sample
 */
auto convert_uint8_to_int16(uint8_t sample) -> int16_t;

/**
 * @brief Helper function to convert an int32_t sample to an int16_t sample.
 * @param sample The int32_t sample
 * @return The int16_t sample
 */
auto convert_int32_to_int16(int32_t sample) -> int16_t;

/**
 * @brief Helper function to convert a float sample to an int24 sample,
 * which is stored in an int32_t.
 * @param sample The float sample
 * @retur The int24 sample
 */
auto convert_float_to_int24(float sample) -> int32_t;

/**
 * @brief Helper function to convert an uint8_t sample to an int24 sample,
 * which is stored in an int32_t.
 * @param sample The uint8_t sample
 * @return The int24 sample
 */
auto convert_uint8_to_int24(uint8_t sample) -> int32_t;

/**
 * @brief Helper function to convert an int16_t sample to an int24 sample,
 * which is stored in an int32_t.
 * @param sample The int16_t sample
 * @return The int24 sample
 */
auto convert_int16_to_int24(int16_t sample) -> int32_t;

/**
 * @brief Helper function to convert an int32_t sample to an int24 sample,
 * which is stored in an int32_t.
 * @param sample The int32_t sample
 * @return The int24 sample
 */
auto convert_int32_to_int24(int32_t sample) -> int32_t;

/**
 * @brief Helper function to convert a float sample to an int32_t sample.
 * @param sample The float sample
 * @return The int32_t sample
 */
auto convert_float_to_int32(float sample) -> int32_t;

/**
 * @brief Helper function to convert an uint8_t sample to an int32_t sample.
 * @param sample The uint8_t sample
 * @return The int32_t sample
 */
auto convert_uint8_to_int32(uint8_t sample) -> int32_t;

/**
 * @brief Helper function to convert an int16_t sample to an int32_t sample.
 * @param sample The int16_t sample
 * @return The int32_t sample
 */
auto convert_int16_to_int32(int16_t sample) -> int32_t;

#endif // WAV_UTILS_H
