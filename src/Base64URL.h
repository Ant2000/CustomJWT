#pragma once

#include <stdint.h>
#include <stddef.h>
class Base64URL
{

public:
    /**
     * @brief Base64url encoding algorithm
     * @param[in] input Input data to encode
     * @param[in] inputLen Length of the data to encode
     * @param[out] output NULL-terminated string encoded with Base64url algorithm
     * @param[out] outputLen Length of the encoded string (optional parameter)
     **/

    static void base64urlEncode(const void *input, size_t inputLen, char *output,
                         size_t *outputLen);

    /**
     * @brief Base64url decoding algorithm
     * @param[in] input Base64url-encoded string
     * @param[in] inputLen Length of the encoded string
     * @param[out] output Resulting decoded data
     * @param[out] outputLen Length of the decoded data
     * @return Decode success
     **/

    static bool base64urlDecode(const char *input, size_t inputLen, void *output,
                         size_t *outputLen);
};