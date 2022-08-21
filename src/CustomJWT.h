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
    size_t maxPayloadLen;
    size_t maxSigLen;

    char *header = nullptr;
    char *payload = nullptr;
    char *signature = nullptr;
    size_t headerLength;
    size_t payloadLength;
    size_t signatureLength;

    char *out = nullptr;

    CustomJWT(char *secret, size_t maxPayloadLen, size_t maxSigLen = 32, char *alg = "HS256", char *typ = "JWT")
    {
        this->secret = (uint8_t *)secret;
        this->alg = alg;
        this->typ = typ;
        this->maxPayloadLen = maxPayloadLen;
        this->maxSigLen = maxSigLen;
    }

    void allocateJWTMemory()
    {
        size_t headerLen = strlen(alg) + strlen(typ) + 30;
        size_t b64HeaderLen = (size_t)(4.0 * (headerLen / 3.0)) + 5;
        size_t b64PayloadLen = (size_t)(4.0 * (maxPayloadLen / 3.0)) + 5;
        size_t b64SigLen = (size_t)(4.0 * (maxSigLen / 3.0)) + 5;

        header = (char *)calloc(b64HeaderLen, sizeof(char));
        payload = (char *)calloc(b64PayloadLen, sizeof(char));
        signature = (char *)calloc(b64SigLen, sizeof(char));
        out = (char *)calloc(b64HeaderLen + b64PayloadLen + b64SigLen + 4, sizeof(char));
    }

    void encodeJWT(char *string)
    {
        memset(header, 0, sizeof(header));
        memset(payload, 0, sizeof(payload));
        memset(signature, 0, sizeof(signature));

        char headerJSON[strlen(alg) + strlen(typ) + 30];
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
        return;
    }

    bool decodeJWT(char *string)
    {
        return true;
    }

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
    }

private:
    Base64URL b64Handler;
};

#endif //_CUSTOM_JWT_H_
