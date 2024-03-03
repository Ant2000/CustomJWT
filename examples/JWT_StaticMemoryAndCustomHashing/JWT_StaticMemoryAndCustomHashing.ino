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
    Serial.begin(9600);
    // Generate a JWT
    Serial.println("Generating a JWT");
    jwt.encodeJWT(string);
    Serial.println("Header Info");
    printData(jwt.header, jwt.headerLength);
    Serial.println("Payload Info");
    printData(jwt.payload, jwt.payloadLength);
    Serial.println("Signaure Info");
    printData(jwt.signature, jwt.signatureLength);
    Serial.println("Final Output Info");
    printData(jwt.out, jwt.outputLength);

    //Decode the JWT
    Serial.println("Decoding and verifying the JWT that was just generated");
    Serial.print("JWT Decode ended with result: ");
    Serial.println(jwt.decodeJWT(jwt.out));
    Serial.println("Header Info");
    printData(jwt.header, jwt.headerLength);
    Serial.println("Payload Info");
    printData(jwt.payload, jwt.payloadLength);
    Serial.println("Signaure Info");
    printData(jwt.signature, jwt.signatureLength);
}

void loop()
{
}
