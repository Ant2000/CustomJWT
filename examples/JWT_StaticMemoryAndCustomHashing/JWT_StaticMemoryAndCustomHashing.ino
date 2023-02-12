#include <CustomJWT.h>
#include <Crypto.h>
#include <SHA512.h>

#define SHA512_HASH_LEN 64

char key[] = "tester";
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
char header[50];
char payload[450];
char signature[100];
char out[400];

void generateSignature(char *output, size_t *outputLen, void *secret, size_t secretLen, void *data, size_t dataLen) 
{
    uint8_t hashed[SHA512_HASH_LEN];
    memset(hashed, 0, SHA512_HASH_LEN);
    hmac<SHA512>(hashed, SHA512_HASH_LEN, secret, secretLen, data, dataLen);
    Base64URL::base64urlEncode(hashed, SHA512_HASH_LEN, output, outputLen);
}

CustomJWT jwt(key, header, sizeof(header), payload, sizeof(payload), signature, sizeof(signature), out, sizeof(out), "HS512", generateSignature);

void setup()
{
    Serial.begin(115200);
    // Generate a JWT
    Serial.println("Generating a JWT");
    jwt.encodeJWT(string);
    Serial.printf("Header: %s\nHeader Length: %d\n", jwt.header, jwt.headerLength);
    Serial.printf("Payload: %s\nPayload Length: %d\n", jwt.payload, jwt.payloadLength);
    Serial.printf("Signature: %s\nSignature Length: %d\n", jwt.signature, jwt.signatureLength);
    Serial.printf("Final Output: %s\nFinalOutput Length: %d\n", jwt.out, jwt.outputLength);

    //Decode the JWT
    Serial.println("Decoding and verifying the JWT that was just generated");
    Serial.println(jwt.decodeJWT(jwt.out));
    Serial.printf("Header: %s\nHeader Length: %d\n", jwt.header, jwt.headerLength);
    Serial.printf("Payload: %s\nPayload Length: %d\n", jwt.payload, jwt.payloadLength);
    Serial.printf("Signature: %s\nSignature Length: %d\n", jwt.signature, jwt.signatureLength);
}

void loop()
{
}
