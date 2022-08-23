# Arduino JWT library

A library created to encode and decode JWT for Arduino. Tested and working on an Arduino UNO, NodeMCU and ESP32 Wrover Module. Only supports HMAC-SHA256 for creating signature. This library does not use any strings and works only with char arrays.
<br>
Information about JSON Web Tokens(JWT) can be found here-
https://jwt.io/
<br>
<h1>Usage</h1>
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

<br>
Now, we must allocate space so that we can store the output of the encoding and decoding functions.<br>
For example- <br>

```
jwt.allocateJWTMemory();
```

To generate a JWT, we must call the EncodeJWT function. The resulting header, payload, signature and final output are stored in the class variables header, payload, signature and out respectively.<br>
The function returns true if encoding is sucessfull and false if anything goes wrong.<br>
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

For decoding, the decodeJWT function is used. Similar to the encoding function, the decoded header, payload, and signature are stored in the chass variables header, payload and signature respectively. This function hoowever, returns an integer instead of a boolean value<br>
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
Once this function is called, the allocate function will need to be called to cuntinue using the object.<br>
Note - This function is not meant to initialise class variables. It only deallocates memory. Continuously calling clear() and allocateJWTMemory() may cause heap fragmentation.
