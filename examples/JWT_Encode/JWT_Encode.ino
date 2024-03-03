#include <CustomJWT.h>

char key[] = "tester";
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
CustomJWT jwt(key, 256);

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
    jwt.allocateJWTMemory();
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
    jwt.clear();
}

void loop()
{
}
