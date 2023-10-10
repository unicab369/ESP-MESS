// MIT License

// Copyright (c) 2018 

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Source: https://github.com/kwasmich/RC4D

#include <limits.h>

#define SWAP(a, b) do { uint8_t tmp = (a); (a) = (b); (b) = tmp; } while (0);
#define IV 42

class RC4D {
    uint8_t s_s[256];

    void rc4cfbTranscryptInplace(uint8_t* const block, const uint8_t in_BLOCK_SIZE, const bool in_DECRYPT) {
        uint8_t s[256];
        uint8_t i = 0;
        uint8_t j = 0;
        uint8_t prev = 0;
        uint8_t prevPrev = 0;
        uint8_t cnt = 0;

        memcpy(s, s_s, 256);

        do {
            i = (i + 1) & 255;
            j = (j + s[i]) & 255;
            SWAP(s[i], s[j]);
            const uint8_t k = s[(s[i] + s[j]) & 255];

            if (in_DECRYPT) {
                prev = prevPrev;
                prevPrev = block[cnt];
            }

            if (cnt) {
                if (in_DECRYPT) {
                    block[cnt] = block[cnt] ^ k ^ s[prev];
                } else {
                    block[cnt] = block[cnt] ^ k ^ s[block[cnt - 1]];
                }
            } else {
                block[cnt] = block[cnt] ^ k ^ s[IV];
            }

            cnt++;
        } while (cnt < in_BLOCK_SIZE);
    }

    void transposeInplace(uint8_t* const d, const int in_LEN) {
        uint8_t i = 0;
        uint8_t j = in_LEN - 1;

        do {
            SWAP(d[i], d[j]);
            i++;
            j--;
        } while (i < j);
    }

    public:
        void rc4dInit(const uint8_t in_KEY[16]) {
            uint8_t i = UCHAR_MAX;
            uint8_t j = 0;

            do {
                s_s[i] = i;
            } while (i--);

            i = 0;

            do {
                j = (j + s_s[i] + in_KEY[i % 16]);
                SWAP(s_s[i], s_s[j]);
            } while (++i & UCHAR_MAX);
        }

        void rc4dEncrypt(uint8_t* const block, const uint8_t in_BLOCK_SIZE) {
            rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, false);
            transposeInplace(block, in_BLOCK_SIZE);
            rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, false);
        }

        void rc4dDecrypt(uint8_t* const block, const uint8_t in_BLOCK_SIZE) {
            rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, true);
            transposeInplace(block, in_BLOCK_SIZE);
            rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, true);
        }


        static void printHex(const uint8_t * const buf, const uint8_t len) {
            for (uint8_t i = 0; i < len; i++) {
                printf("%02x ", buf[i]);
            }
            puts("");   // stop null character
        }

        static void runTest() {
            RC4D rc4d;

            uint8_t in[] = "Hello World!";
            // make the key volatile to be not optimized away in the NOP version
            const uint8_t key[16] = "Key____________";
            const uint8_t inLen = strlen((char *)in);
            
            rc4d.rc4dInit(key);
            Serial.print("Before Encrypt: ");
            printHex(in, inLen);

            rc4d.rc4dEncrypt(in, inLen);
            Serial.print("After Encrypt: ");
            printHex(in, inLen);

            rc4d.rc4dDecrypt(in, inLen);
            Serial.print("After Decrypt: ");
            printHex(in, inLen);

            Serial.println((char*)in);

            uint8_t in2[32] = { 0 };
            const uint8_t in2Len = 32;
            // make the key volatile to be not optimized away in the NOP version
            const uint8_t key2[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 };

            // currently there is no 128 bit test vector to verify against
            rc4d.rc4dInit(key2);
            rc4d.rc4dEncrypt(in2, in2Len);
            printHex(in2, in2Len);

            unsigned long start = millis();

            for (uint16_t i = 0; i < 1024; i++) {
                rc4d.rc4dEncrypt(in2, in2Len);
                rc4d.rc4dDecrypt(in2, in2Len);
            }

            unsigned long end = millis();
            printf("%d ms\n", end - start);
        }
};