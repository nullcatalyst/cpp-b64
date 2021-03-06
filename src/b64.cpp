#include <cstdlib> // malloc
#include <cstdio> // malloc

#include "b64.h"

namespace b64 {
    namespace {
        constexpr const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        inline constexpr int toInt(unsigned int c) {
            if ((c >= 'A') && (c <= 'Z')) return (c - 'A') + 0;   // Upper Case  >>  0-25
            if ((c >= 'a') && (c <= 'z')) return (c - 'a') + 26;  // Lower Case  >>  26-51
            if ((c >= '0') && (c <= '9')) return (c - '0') + 52;  // Numbers     >>  52-61
            if (c == '+') return 62;                              // Plus (+)    >>  62
            if (c == '/') return 63;                              // Slash (/)   >>  63
            if (c == '=') return 64;                              // Equal (=)   >>  64~
            return -1;
        }
    }


    /**
     * @param data The data to be encoded.
     * @param length The length of the data to be encoded.
     * @return A base64 encoded string, which must be freed using `free()`.
     */
    char * encode(const void * data, size_t length) {
        const unsigned char * in = (const unsigned char *) data;
        const unsigned int outLength = encodedLength(length);
        char * out = (char *) malloc(outLength + 1); // +1 for terminating '\0' byte

        if (encode(data, length, out)) {
            out[outLength] = '\0';
            return out;
        } else {
            free(out);
            return nullptr;
        }
    }

    /**
     * @param b64String The base64 encoded string to be decoded.
     * @param length The length of the string to be decoded.
     * @return An array of bytes, which must be freed using `free()`.
     */
    void * decode(const char * b64String, size_t length) {
        if ((length & 3) != 0) {
            // A base64 string must always have a length that is divisible by 4
            return nullptr;
        }

        const unsigned int outLength = decodedLength(length);
        unsigned char * out = (unsigned char *) malloc(outLength + 1); // +1 for terminating '\0' byte

        if (decode(b64String, length, out)) {
            out[outLength] = '\0';
            return out;
        } else {
            free(out);
            return nullptr;
        }
    }


    /**
     * @param data The data to be encoded.
     * @param length The length of the data to be encoded.
     * @param out The string to copy the results into.
     * @return Returns `true` on success.
     */
    bool encode(const void * data, size_t length, char * out) {
        const unsigned char * in = (const unsigned char *) data;

        const unsigned int remaining = length % 3;
        length -= remaining;

        int i = 0;
        for (int j = 0; j < length; j += 3) {
            unsigned char a = in[j + 0];
            unsigned char b = in[j + 1];
            unsigned char c = in[j + 2];

            out[i++] = table[((a & 0xff) >> 2)];
            out[i++] = table[((a & 0x03) << 4) | (b >> 4)];
            out[i++] = table[((b & 0x0f) << 2) | (c >> 6)];
            out[i++] = table[c & 0x3f];
        }

        switch (remaining) {
            case 0:
                // out[i] = '\0';
                break;

            case 1: {
                unsigned char a = in[length + 0];
                out[i++] = table[((a & 0xff) >> 2)];
                out[i++] = table[((a & 0x03) << 4)];
                out[i++] = '=';
                out[i++] = '=';
                // out[i] = '\0';
                break;
            }

            case 2: {
                unsigned char a = in[length + 0];
                unsigned char b = in[length + 1];
                out[i++] = table[((a & 0xff) >> 2)];
                out[i++] = table[((a & 0x03) << 4) | (b >> 4)];
                out[i++] = table[((b & 0x0f) << 2)];
                out[i++] = '=';
                // out[i] = '\0';
                break;
            }
        }

        return true;
    }

    /**
     * @param b64String The base64 encoded string to be decoded.
     * @param length The length of the string to be decoded.
     * @param out The buffer to copy the results into.
     * @return Returns `true` on success.
     */
    bool decode(const char * b64String, size_t length, void * out) {
        if ((length & 3) != 0) {
            // A base64 string must always have a length that is divisible by 4
            return false;
        }

        unsigned char * buffer = (unsigned char *) out;

#define checkBounds(var)                                \
        do {                                            \
            if (var < 0) return false;                  \
            if (atEnd && var < 64) return false;        \
            if (var >= 64) { atEnd = true; var = 0; }   \
        } while (false)

        bool atEnd = false;
        int i = 0;
        for (int j = 0; j < length; j += 4) {
            unsigned char a = toInt(b64String[j + 0]); checkBounds(a);
            unsigned char b = toInt(b64String[j + 1]); checkBounds(b);
            unsigned char c = toInt(b64String[j + 2]); checkBounds(c);
            unsigned char d = toInt(b64String[j + 3]); checkBounds(d);

            buffer[i++] = (a << 2) | (b >> 4);
            buffer[i++] = (b << 4) | (c >> 2);
            buffer[i++] = (c << 6) | (d >> 0);
        }

#undef checkBounds

        // buffer[i] = '\0';
        return true;
    }
}
