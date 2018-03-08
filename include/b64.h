#pragma once

#include <cstddef> // size_t

namespace b64 {
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
}
