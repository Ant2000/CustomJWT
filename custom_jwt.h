#ifndef _CUSTOM_JWT_H_
#define _CUSTOM_JWT_H_

#include <Arduino.h>

class CustomJWT{
private:
    char* secretKey;

public:
    char* header;
    char* payload;
    char* signature;
    String out;

    CustomJWT(char* secret);

    char* getSecret();

    void changeSecret(char* secret);

    void encodeJWT(char* string);

    void decodeJWT(char* string);

};

#endif //_CUSTOM_JWT_H_
