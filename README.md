# Arduino JWT library

A library created to encode and decode JWT for Arduino. Tested and working on an Arduino UNO, NodeMCU and ESP32 Wrover Module. Information about JSON Web Tokens(JWT) can be found here-
https://jwt.io/
<br>
<h1>Usage</h1>
There are several ways this library can be used as listed below:<br>
1. <a href="#method-1">The "It Just Works" method:</a><br>
2. <a href="#method-2">Providing own memory</a><br>
3. <a href="#method-3">Providing own hashing function</a><br>
4. <a href="#method-4">Providing own hashing function and memory</a><br>
<h2 id="method-1">1. The "It Just Works" method:</h2>
This method is ideal for those who just want a working system to create JWTs. This method supports only HMAC-SHA256 for hashing the signature and uses malloc functions to allocate memory to store JWT output.<br><br>
To import the library, use:<br>

```
#include <CustomJWT.h>
```

First one needs to create a CustomJWT object with the encryption key and expected payload length. The key must be a char array. <br>
For example:<br>

```
char key[] = "tester";
CustomJWT jwt(key, 256);
```
There are several parameters that can be configured in this constructor. Below is the exact function signature:
```
CustomJWT(char *secret, 
          size_t maxPayloadLen, 
          size_t maxHeadLen = 40, 
          size_t maxSigLen = 32, 
          char *alg = "HS256", 
          char *typ = "JWT")
```
Now, we must allocate space so that we can store the output of the encoding and decoding functions.<br>
For example- <br>

```
jwt.allocateJWTMemory();
```

To generate a JWT, we must call the EncodeJWT function. The resulting header, payload, signature and final output are stored in the class variables header, payload, signature and out respectively.<br>
The function returns true if encoding is successful and false if anything goes wrong.<br>
For example: <br>

```
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
jwt.encodeJWT(string);
Serial.printf("Header: %s\nHeader Length: %d\n", jwt.header, jwt.headerLength);
Serial.printf("Payload: %s\nPayload Length: %d\n", jwt.payload, jwt.payloadLength);
Serial.printf("Signature: %s\nSignature Length: %d\n", jwt.signature, jwt.signatureLength);
Serial.printf("Final Output: %s\nFinalOutput Length: %d\n", jwt.out, jwt.outputLength);
```

<br>
The expected output is: 

```
Header: eyJhbGciOiAiSFMyNTYiLCJ0eXAiOiJKV1QifQ
Header Length: 38
Payload: eyJ0ZW1wIjoyMi41LCJzcGVlZCI6MjUuMX0
Payload Length: 35
Signature: CWvXOoyZCskhDHOMJzMmf333G6M_XUwZ9mg_fkIw4ZQ
Signature Length: 43
Final Output: eyJhbGciOiAiSFMyNTYiLCJ0eXAiOiJKV1QifQ.eyJ0ZW1wIjoyMi41LCJzcGVlZCI6MjUuMX0.CWvXOoyZCskhDHOMJzMmf333G6M_XUwZ9mg_fkIw4ZQ
FinalOutput Length: 118
```

<br>

For decoding, the decodeJWT function is used. Similar to the encoding function, the decoded header, payload, and signature are stored in the class variables header, payload and signature respectively. This function however, returns an integer instead of a boolean value<br>
Return 0: The function ran without any problems<br>
Return 1: Memory has not been allocated<br>
Return 2: Input is not valid JWT<br>
Return 3: Signature validation failed<br>
For example:<br>
```
char string[] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0ZW1wIjoyMi41LCJzcGVlZCI6MjUuMX0.t5CfO7HYM0I1sJknkPyCWctTk6tOVBIU07rjcSULDWI";
Serial.println(jwt.decodeJWT(string));
Serial.printf("Header: %s\nHeader Length: %d\n", jwt.header, jwt.headerLength);
Serial.printf("Payload: %s\nPayload Length: %d\n", jwt.payload, jwt.payloadLength);
Serial.printf("Signature: %s\nSignature Length: %d\n", jwt.signature, jwt.signatureLength);
``` 
<br>
The expected outputs are -<br>

```
Header: {"alg":"HS256","typ":"JWT"}
Header Length: 27
Payload: {"temp":22.5,"speed":25.1}
Payload Length: 26
Signature: t5CfO7HYM0I1sJknkPyCWctTk6tOVBIU07rjcSULDWI
Signature Length: 43
```

Finally, if there is no longer a need to create or decode JWT or the memory is needed elsewhere, the memory assigned for JWT can be deallocated using the clear function.<br>
For example:
```
jwt.clear();
```
Once this function is called, the allocate function will need to be called to continue using the object.<br>
Note - This function is not meant to initialise class variables. It only deallocates memory. Continuously calling clear() and allocateJWTMemory() may cause heap fragmentation.<br><br>

<h2 id="method-2">2. Providing own memory</h2>
This method is for those who are not comfortable with using dynamic memory allocation in a microcontroller environment. This method is pretty much same as the "It Just Works" method except in this case, the variables for storage of JWT output are expected to be provided in the constructor. Additionally, since the memory allocation is static, the <b>allocateJWTMemory</b> and <b>clear</b> functions will not be used in this case.
In this method, the memory must be initialised before creating the CustomJWT object. For example:

```
#include <CustomJWT.h>
char header[50];
char payload[400];
char signature[50];
char out[400];
char key[] = "tester";
```
After this, the arrays must be passed to the constructor:

```
CustomJWT jwt(key, header, sizeof(header), payload, sizeof(payload), signature, sizeof(signature), out, sizeof(out));
```
After this, it will work exactly like the "It Just Works" method except there will be no need to call the <b>allocateJWTMemory</b> or <b>clear</b> functions.

<h2 id="method-3">3. Providing own hashing function</h2>
In case there is a requirement to use any hashing function other than HMAC-SHA256, this method can be followed. The hashing function must be defined with the given signature:

```
void functionName(char *output, 
                  size_t *outputLen, 
                  void *secret, size_t 
                  secretLen, 
                  void *data, 
                  size_t dataLen)
output    -> Pointer where b64 encoded JWT signature will be stored
outputLen -> Pointer where length of b64 encoded JWT signature will be stored
secret    -> Pointer to secret used to generate hash
secretLen -> Length of secret used to generate hash
data      -> Pointer to data that will be used to generate hash
dataLen   -> Size of data that will be used to generate hash
```

In this method, the hashing function must be declared before creating the JWT object. For example, here a HMAC-SHA512 hashing function has been created using the Crypto library.
```
#include <CustomJWT.h>
#include <Crypto.h>
#include <SHA512.h>

#define SHA512_HASH_LEN 64

void generateSignature(char *output, size_t *outputLen, void *secret, size_t secretLen, void *data, size_t dataLen) 
{
    uint8_t hashed[SHA512_HASH_LEN];
    memset(hashed, 0, SHA512_HASH_LEN);
    hmac<SHA512>(hashed, SHA512_HASH_LEN, secret, secretLen, data, dataLen);
    Base64URL::base64urlEncode(hashed, SHA512_HASH_LEN, output, outputLen);
}
```
After this, the CustomJWT object can be created. Since a custom hashing algorithm is being used, one is expected to provide the maximum expected header and signature length (before encoding to base64).
```
char key[] = "tester";
CustomJWT MyJWT(key, 256, 40, SHA512_HASH_LEN, "HS512", generateSignature);
```
After this, the usage is identical to the "It Just Works" method.

<h2 id="method-4">4. Providing own hashing function and memory</h2>
This is for the situation where one wants to both use static memory allocation and provide their own hashing function to generate the signature. This method is practically a combination of methods 2 and 3. First one must create the hashing function and memory arrays and then they can be passed into the constructer for the CustomJWT object. For example:

```
#include <CustomJWT.h>
#include <Crypto.h>
#include <SHA512.h>

#define SHA512_HASH_LEN 64

char header[50];
char payload[400];
char signature[50];
char out[400];
char key[] = "tester";

void generateSignature(char *output, size_t *outputLen, void *secret, size_t secretLen, void *data, size_t dataLen) 
{
    uint8_t hashed[SHA512_HASH_LEN];
    memset(hashed, 0, SHA512_HASH_LEN);
    hmac<SHA512>(hashed, SHA512_HASH_LEN, secret, secretLen, data, dataLen);
    Base64URL::base64urlEncode(hashed, SHA512_HASH_LEN, output, outputLen);
}
```
Now that the relevant variables and functions have been created, they can be passed on the constructor. 
```
CustomJWT jwt(key, header, sizeof(header), payload, sizeof(payload), signature, sizeof(signature), out, sizeof(out), "HS512", generateSignature);
``` 
After this, the usage is identical to Method 2, i.e. the "It Just Works" method minus the memory allocation related functions.
