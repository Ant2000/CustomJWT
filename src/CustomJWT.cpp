#include "CustomJWT.h"
#include "sha256.h"

const char b64[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
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
    while(b64src[i])
    {
        c = (int) b64src[i];
        if(c == '=')
        {
            decodeblock(in, clrdst);
            break;
        }
        p = strchr(b64, c);
        if(p)
        {
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
    char temp[strlen(head) + strlen(str) + 10];
    temp[0] = '\0';
    strcat(temp, head);
    strcat(temp, ".");
    strcat(temp, str);
    CustomJWT::out = temp;

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
    Sha256.print(temp);
    //uint8_t* hash = Sha256.resultHmac();
    //Encode to b64
    len = ((32 + 2)/3)*4;
    char str1[len + 10];
    b64_encode(Sha256.resultHmac(), str1);
    while(str1[strlen(str1) - 1] == '=')
        str1[strlen(str1) - 1] = '\0';
    char temp1[strlen(temp) + strlen(str1) + 10];
    temp1[0] = '\0';
    strcat(temp1, temp);
    strcat(temp1, ".");
    strcat(temp1, str1);
    CustomJWT::out = temp1;
}

bool CustomJWT::decodeJWT(char *string)
{
    //strtok is behaving very weirdly with the Arduino UNO so I am going with this long method.
    uint8_t i, state = 0, headl = 0, bodyl = 0, signl = 0;
    for(i = 0; i < strlen(string); i++)
    {
        switch (state)
        {
            case 0:
                if(string[i] == '.')
                    state++;
                else
                    headl++;
                break;
            case 1:
                if(string[i] == '.')
                    state++;
                else
                    bodyl++;
                break;
            default:
                signl++;
                break;
        }
    }
    char head[headl + 50];//I do not know why this array needs special treatment but it works
    for(i = 0; i < headl; i++)
    {
        head[i] = string[i];
    }
    head[headl] = '\0';
    //CustomJWT::header = head;

    char body[bodyl + 10];
    for(i = headl + 1; i < (headl + 1 + bodyl); i++)
    {
        body[i - (headl + 1)] = string[i];
    }
    body[bodyl] = '\0';
    //CustomJWT::payload = body;

    char sign[signl + 10];
    for(i = headl + bodyl + 2; i < (headl + bodyl + signl + 2); i++)
    {
        sign[i - (headl + bodyl + 2)] = string[i];
    }
    sign[signl] = '\0';
    //CustomJWT::signature = sign;

    char data[headl + bodyl + 20];
    data[0] = '\0';
    strcat(data, head);
    strcat(data, ".");
    strcat(data, body);

    head[headl] = '=';
    head[headl + 1] = '\0';
    char output1[headl];
    b64_decode(head, output1);
    strcpy(head, output1);
    CustomJWT::header = head;

    body[bodyl] = '=';
    body[bodyl + 1] = '\0';
    char output[bodyl];
    b64_decode(body, output);
    strcpy(body, output);
    CustomJWT::payload = body;

    //Convert passkey to unsigned int
    char* msg1 = CustomJWT::secretKey;
    size_t length = strlen(msg1 + 1);
    char* beg = msg1;
    char* end = msg1 + length + 1;
    uint8_t* key = new uint8_t[length + 1];
    size_t j = 0;
    for (; beg != end; ++beg, ++j)
    {
        key[j] = (uint8_t)(*beg);
    }
    //Initialise HMAC
    Sha256.initHmac(key, length + 1);
    //Obtain result
    Sha256.print(data);
    //Encode to b64
    int len = ((32 + 2)/3)*4;
    char str1[len];
    b64_encode(Sha256.resultHmac(), str1);
    while(str1[strlen(str1) - 1] == '=')
        str1[strlen(str1) - 1] = '\0';

    CustomJWT::signature = str1;

    if(strcmp(CustomJWT::signature, sign) == 0)
        return true;
    else
        return false;
}
