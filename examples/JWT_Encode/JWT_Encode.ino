#include <CustomJWT.h>

char key[] = "tester";
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
CustomJWT jwt(key);

void setup()
{
    Serial.begin(115200);
    jwt.encodeJWT(string);
    Serial.println(jwt.out); 
}

void loop()
{
}
