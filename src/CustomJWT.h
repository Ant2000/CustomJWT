#ifndef _CUSTOM_JWT_H_
#define _CUSTOM_JWT_H_

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

    void initialiseJWT()
    {
        size_t headerLen = strlen(alg) + strlen(typ) + 30;
        size_t b64HeaderLen = (size_t)(4.0 * (headerLen / 3.0)) + 5;
        size_t b64PayloadLen = (size_t)(4.0 * (maxPayloadLen / 3.0)) + 5;
        size_t b64SigLen = (size_t)(4.0 * (maxSigLen / 3.0)) + 5;

        header = (char *)calloc(b64HeaderLen, sizeof(char));
        payload = (char *)calloc(b64PayloadLen, sizeof(char));
        signature = (char *)calloc(b64SigLen, sizeof(char));

        char headerJSON[headerLen];
        sprintf(headerJSON, "{\"alg\": \"%s\",\"typ\":\"%s\"}", alg, typ);
        b64Handler.base64urlEncode(headerJSON, strlen(headerJSON), header, &headerLength);
    }

    void encodeJWT(char *string)
    {
        b64Handler.base64urlEncode(string, strlen(string), payload, &payloadLength);
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
