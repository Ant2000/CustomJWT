#include <CustomJWT.h>

char key[] = "tester";
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
char header[50];
char payload[400];
char signature[50];
char out[400];

CustomJWT jwt(key, header, sizeof(header), payload, sizeof(payload), signature, sizeof(signature), out, sizeof(out));

void printData(char *data, size_t dataLen) {
    Serial.print("Data: ");
    Serial.println(data);
    Serial.print("Data Length: ");
    Serial.println(dataLen);
    Serial.println();
}

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
