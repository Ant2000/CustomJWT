#ifndef _CUSTOM_JWT_H_
#define _CUSTOM_JWT_H_


class CustomJWT{
private:
    char* secretKey;

public:
    char* header;
    char* payload;
    char* signature;
    char* out;

    CustomJWT(char* secret);

    char* getSecret();

    void changeSecret(char* secret);

    void encodeJWT(char* string);

    bool decodeJWT(char* string);

};

#endif //_CUSTOM_JWT_H_
