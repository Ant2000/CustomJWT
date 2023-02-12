#include <CustomJWT.h>

char key[] = "tester";
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
char header[50];
char payload[400];
char signature[50];
char out[400];

CustomJWT jwt(key, header, sizeof(header), payload, sizeof(payload), signature, sizeof(signature), out, sizeof(out));

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
