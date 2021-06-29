#include "custom_jwt.h"
#include "sha256.h"
#include <Arduino.h>

const char b64[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char head[] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9";

void decodeblock(unsigned char in[], char *clrstr) {
    unsigned char out[4];
    out[0] = in[0] << 2 | in[1] >> 4;
    out[1] = in[1] << 4 | in[2] >> 2;
    out[2] = in[2] << 6 | in[3] >> 0;
    out[3] = '\0';
    strncat(clrstr, reinterpret_cast<const char *>(out), sizeof(out));
}

void b64_decode(char *b64src, char *clrdst)
{
    int c, phase, i;
    unsigned char in[4];
    char *p;

    clrdst[0] = '\0';
    phase = 0; i=0;
    while(b64src[i]) {
        c = (int) b64src[i];
        if(c == '=') {
            decodeblock(in, clrdst);
            break;
        }
        p = strchr(b64, c);
        if(p) {
            in[phase] = p - b64;
            phase = (phase + 1) % 4;
            if(phase == 0) {
                decodeblock(in, clrdst);
                in[0]=in[1]=in[2]=in[3]=0;
            }
        }
        i++;
    }
}

void encodeblock( unsigned char in[], char b64str[], int len ) {
    unsigned char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
                                             ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, reinterpret_cast<const char *>(out), sizeof(out));
}

void b64_encode(char *clrstr, char *b64dst) {
    unsigned char in[3];
    int i, len = 0;
    int j = 0;

    b64dst[0] = '\0';
    while(clrstr[j]) {
        len = 0;
        for(i=0; i<3; i++) {
            in[i] = (unsigned char) clrstr[j];
            if(clrstr[j]) {
                len++; j++;
            }
            else in[i] = 0;
        }
        if( len ) {
            encodeblock( in, b64dst, len );
        }
    }
}

void b64_encode(unsigned char *clrstr, char *b64dst)
{
    unsigned char in[3];
    int i, len = 0;
    int j = 0;

    b64dst[0] = '\0';
    while(j < 32) {
        len = 0;
        for(i=0; i<3; i++) {
            in[i] = clrstr[j];

            if(clrstr[j] && j < 32) {
                len++; j++;
            }
            else in[i] = 0;
        }
        if( len ) {
            encodeblock( in, b64dst, len );
        }
    }
}

CustomJWT::CustomJWT(char *secret)
{
    CustomJWT::secretKey = secret;
}

char * CustomJWT::getSecret()
{
    return CustomJWT::secretKey;
}

void CustomJWT::changeSecret(char *secret)
{
    CustomJWT::secretKey = secret;
}

void CustomJWT::encodeJWT(char *string)
{
    //Create header and body
    int len = (strlen(string) + 2)/3*4;
    char str[len];
    b64_encode(string, str);
    while(str[strlen(str) - 1] == '=')
        str[strlen(str) - 1] = '\0';
    String temp = head;
    temp = temp + ".";
    temp = temp + str;
    //Convert passkey to unsigned int
    char* msg1 = CustomJWT::secretKey;
    size_t length = strlen(msg1 + 1);
    char* beg = msg1;
    char* end = msg1 + length + 1;
    uint8_t* key = new uint8_t[length + 1];
    size_t i = 0;
    for (; beg != end; ++beg, ++i)
    {
        key[i] = (uint8_t)(*beg);
    }
    //Initialise HMAC
    Sha256.initHmac(key, length + 1);
    //Convert string to char array
    char temp1[temp.length() + 10];
    temp.toCharArray(temp1, temp.length() + 10);
    //Obtain result
    Sha256.print(temp1);
    uint8_t* hash = Sha256.resultHmac();
    //Encode to b64
    len = ((32 + 2)/3)*4;
    char str1[len];
    b64_encode(hash, str1);
    while(str1[strlen(str1) - 1] == '=')
        str1[strlen(str1) - 1] = '\0';
    temp = temp + ".";
    temp = temp + str1;
    CustomJWT::out = temp;
}

