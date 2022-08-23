#ifndef _CUSTOM_JWT_H_
#define _CUSTOM_JWT_H_

#define SHA256_HASH 32

#include <Crypto.h>
#include <SHA256.h>
#include "Base64URL.h"

class CustomJWT
{
public:
    uint8_t *secret;
    char *alg;
    char *typ;
    size_t maxHeadLen;
    size_t maxPayloadLen;
    size_t maxSigLen;

    char *header = nullptr;
    char *payload = nullptr;
    char *signature = nullptr;
    size_t headerLength;
    size_t payloadLength;
    size_t signatureLength;
    size_t b64HeaderLen;
    size_t b64PayloadLen;
    size_t b64SigLen;

    char *out = nullptr;
    size_t maxOutputLen;
    size_t outputLength;

    Base64URL b64Handler;

/**
 * @brief Construct a new CustomJWT object
 * 
 * @param secret The Encryption Key
 * @param maxPayloadLen Maximum expected length of payload
 * @param maxHeadLen Maximum expected length of header. Defaults to 40.
 * @param maxSigLen Maximum expected length of signature. Defaults to 32.
 * @param alg Encryption algorithm used. Defaults to HS256.
 * @param typ Header type. Defaults to JWT.
 */
    CustomJWT(char *secret, size_t maxPayloadLen, size_t maxHeadLen = 40, size_t maxSigLen = 32, char *alg = "HS256", char *typ = "JWT")
    {
        this->secret = (uint8_t *)secret;
        this->alg = alg;
        this->typ = typ;
        this->maxHeadLen = maxHeadLen;
        this->maxPayloadLen = maxPayloadLen;
        this->maxSigLen = maxSigLen;
    }

    /**
     * @brief Allocate memory for internal char pointers
     * 
     */
    void allocateJWTMemory()
    {
        b64HeaderLen = (size_t)(4.0 * (maxHeadLen / 3.0)) + 5;
        b64PayloadLen = (size_t)(4.0 * (maxPayloadLen / 3.0)) + 5;
        b64SigLen = (size_t)(4.0 * (maxSigLen / 3.0)) + 5;
        maxOutputLen = (size_t)(b64HeaderLen + b64PayloadLen + b64SigLen + 4);

        header = (char *)malloc(b64HeaderLen * sizeof(char));
        payload = (char *)malloc(b64PayloadLen * sizeof(char));
        signature = (char *)malloc(b64SigLen * sizeof(char));
        out = (char *)malloc(maxOutputLen * sizeof(char));

        memoryAllocationDone = true;
    }

    /**
     * @brief Encode given string into JWT. Final output stored in out. 
     * 
     * @param string Data to be encoded
     * @return true: Encoding success
     * @return false: Memory not allocated
     */
    bool encodeJWT(char *string)
    {
        if(!memoryAllocationDone)
            return false;

        memset(header, 0, sizeof(char) * b64HeaderLen);
        memset(payload, 0, sizeof(char) * b64PayloadLen);
        memset(signature, 0, sizeof(char) * b64SigLen);
        memset(out, 0, sizeof(char) * maxOutputLen);

        char headerJSON[maxHeadLen];
        sprintf(headerJSON, "{\"alg\": \"%s\",\"typ\":\"%s\"}", alg, typ);
        b64Handler.base64urlEncode(headerJSON, strlen(headerJSON), header, &headerLength);

        b64Handler.base64urlEncode(string, strlen(string), payload, &payloadLength);
        
        char toHash[payloadLength + headerLength + 3];
        memset(toHash, 0, payloadLength + headerLength + 3);
        sprintf(toHash, "%s.%s", header, payload);
        
        uint8_t hashed[SHA256_HASH];
        memset(hashed, 0, SHA256_HASH);
        hmac<SHA256>(hashed, SHA256_HASH, secret, strlen((char *)secret), toHash, strlen(toHash));
        b64Handler.base64urlEncode(hashed, SHA256_HASH, signature, &signatureLength);
        
        sprintf(out, "%s.%s", toHash, signature);
        outputLength = strlen(out);
        return true;
    }

    /**
     * @brief Validate and decode JWT. Result stored int header, payload and signature.
     * 
     * @param string JWT to be decoded
     * @returns Code 0: Decode success \n Code 1: Memory not allocated \n Code 2: Invalid JWT \n Code 3: Signature Mismatch
     */
    int decodeJWT(char *string)
    {
        if(!memoryAllocationDone)
            return 1;
        
        const char* delimiter = ".";
        char* b64Head;
        char* b64Payload;
        char* b64Signature;

        
        b64Head = strtok(string, delimiter);
        if(b64Head == 0)
            return 2;
        b64Payload = strtok(0, delimiter);
        if(b64Payload == 0)
            return 2;
        b64Signature = strtok(0, delimiter);
        if(b64Signature == 0)
            return 2;
        if(strtok(0, delimiter) != 0)
            return 2;

        uint8_t hashed[SHA256_HASH];
        memset(hashed, 0, SHA256_HASH);
        
        char toCheckHash[strlen(b64Head) + strlen(b64Payload) + 3];
        memset(toCheckHash, 0, sizeof(toCheckHash));

        char testSignature[strlen(b64Signature)];
        memset(testSignature, 0, sizeof(testSignature));
        size_t testSignatureLength;

        sprintf(toCheckHash, "%s.%s", b64Head, b64Payload);
        hmac<SHA256>(hashed, SHA256_HASH, secret, strlen((char *)secret), toCheckHash, strlen(toCheckHash));
        b64Handler.base64urlEncode(hashed, SHA256_HASH, testSignature, &testSignatureLength);

        if(testSignatureLength != strlen(b64Signature)){
            return 3;
        }

        if(strncmp(b64Signature, testSignature, testSignatureLength) != 0) {
            return 3;
        }

        memset(header, 0, sizeof(char) * b64HeaderLen);
        memset(payload, 0, sizeof(char) * b64PayloadLen);
        memset(signature, 0, sizeof(char) * b64SigLen);
        
        b64Handler.base64urlDecode(b64Head, strlen(b64Head), header, &headerLength);
        b64Handler.base64urlDecode(b64Payload, strlen(b64Payload), payload, &payloadLength);
        sprintf(signature, "%s", b64Signature);
        signatureLength = strlen(signature);
        
        return 0;
    }

    /**
     * @brief Clear all allocated memory
     * 
     */
    void clear()
    {
        if(header != nullptr)
        {
            free(header);
            header = nullptr;
        }
        if(payload != nullptr)
        {
            free(payload);
            payload = nullptr;
        }
        if(signature != nullptr)
        {
            free(signature);
            signature = nullptr;
        }
        if(out != nullptr)
        {
            free(out);
            out = nullptr;
        }
        memoryAllocationDone = false;
    }

private:
    bool memoryAllocationDone = false;
};

#endif //_CUSTOM_JWT_H_
