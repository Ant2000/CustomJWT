#include <CustomJWT.h>

char key[] = "tester";
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
CustomJWT jwt(key, 256);

void setup()
{
    Serial.begin(115200);
    jwt.allocateJWTMemory();
    jwt.encodeJWT(string);
    Serial.printf("Header: %s\nHeader Length: %d\n", jwt.header, jwt.headerLength);
    Serial.printf("Payload: %s\nPayload Length: %d\n", jwt.payload, jwt.payloadLength);
    Serial.printf("Signature: %s\nSignature Length: %d\n", jwt.signature, jwt.signatureLength);
    Serial.printf("Final Output: %s\nFinalOutput Length: %d\n", jwt.out, jwt.outputLength);
    jwt.clear();
}

void loop()
{
}
