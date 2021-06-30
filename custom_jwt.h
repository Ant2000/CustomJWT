#ifndef _CUSTOM_JWT_H_
#define _CUSTOM_JWT_H_

#include <Arduino.h>

class CustomJWT{
private:
    char* secretKey;

public:
    String header;
    String payload;
    String signature;
    String debug;
    String out;

    CustomJWT(char* secret);

    char* getSecret();

    void changeSecret(char* secret);

    void encodeJWT(char* string);

    bool decodeJWT(char* string);

};

#endif //_CUSTOM_JWT_H_
