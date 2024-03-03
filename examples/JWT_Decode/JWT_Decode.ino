#include <CustomJWT.h>

char key[] = "tester";
char string[] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0ZW1wIjoyMi41LCJzcGVlZCI6MjUuMX0.t5CfO7HYM0I1sJknkPyCWctTk6tOVBIU07rjcSULDWI";
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
    //Decode the JWT
    Serial.println("Decoding and verifying the JWT");
    Serial.print("JWT Decode ended with result: ");
    Serial.println(jwt.decodeJWT(string));
    Serial.println("Header Info");
    printData(jwt.header, jwt.headerLength);
    Serial.println("Payload Info");
    printData(jwt.payload, jwt.payloadLength);
    Serial.println("Signaure Info");
    printData(jwt.signature, jwt.signatureLength);
    jwt.clear();
}

void loop()
{
}
