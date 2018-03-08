#pragma once

#include <cstddef> // size_t

namespace b64 {
    /**
     * @param inLength The size of the input buffer.
     * @return The length of the string needed to encode a buffer of this size (excluding terminating '\0').
     */
    inline constexpr unsigned int encodedLength(unsigned int inLength) {
        return 4 * ((inLength + 2) / 3); // 4 * ceil(inLength / 3)
    }

    /**
     * @param inLength The length of the input string.
     * @return The size of the buffer needed to decode a string of this length.
     */
    inline constexpr unsigned int decodedLength(unsigned int inLength) {
        return (3 * inLength) / 4;
    }

    /**
     * @param data The data to be encoded.
     * @param length The length of the data to be encoded.
     * @return A base64 encoded string, which must be freed using `free()`.
     */
    char * encode(const void * data, size_t length);

    /**
     * @param b64String The base64 encoded string to be decoded.
     * @param length The length of the string to be decoded.
     * @return An array of bytes, which must be freed using `free()`.
     */
    void * decode(const char * b64String, size_t length);

    /**
     * @param data The data to be encoded.
     * @param length The length of the data to be encoded.
     * @param out The string to copy the results into.
     * @return Returns `true` on success.
     */
    bool encode(const void * data, size_t length, char * out);

    /**
     * @param b64String The base64 encoded string to be decoded.
     * @param length The length of the string to be decoded.
     * @param out The buffer to copy the results into.
     * @return Returns `true` on success.
     */
    bool decode(const char * b64String, size_t length, void * out);
}
