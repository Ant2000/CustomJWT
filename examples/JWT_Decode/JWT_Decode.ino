#include <CustomJWT.h>

char key[] = "tester";
char string[] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0ZW1wIjoyMi41LCJzcGVlZCI6MjUuMX0.t5CfO7HYM0I1sJknkPyCWctTk6tOVBIU07rjcSULDWI";
CustomJWT jwt(key);

void setup()
{
    Serial.begin(115200);
    Serial.println(jwt.decodeJWT(string));
    Serial.print(jwt.header);
    Serial.print(" ");
    Serial.print(jwt.payload);
    Serial.print(" ");
    Serial.println(jwt.signature);
}

void loop()
{
}
