# Arduino JWT library

A library created to encode and decode JWT for Arduino. Tested and working on an Arduino UNO. Only supports HMAC-SHA256 for creating signature.
<br>
SHA code is a slightly modified version of algorithm found here-
https://github.com/Cathedrow/Cryptosuite
<br>
Information about Java Web Tokens(JWT) can be found here-
https://jwt.io/
<br>
<h1>Usage</h1>
To import the library, use:<br>

```
#include <custom_jwt.h>
```

First one needs to create a CustomJWT object with the parameter key. The key must be a char array.<br>
For example-<br>

```
char key[] = "tester";
CustomJWT jwt(key);
```

<br>
All inputs to the functions of this library must also be char arrays. Similarly, the outputs are also all char arrays.<br>
For encoding, the encodeJWT function is used. The output is will be in the object variable "out". The function returns 0.<br>
For example-<br>

```
char string[] = "{\"temp\":22.5,\"speed\":25.1}";
jwt.encodeJWT(string);
Serial.println(jwt.out); 
```

<br>
The expected output is: 

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0ZW1wIjoyMi41LCJzcGVlZCI6MjUuMX0.t5CfO7HYM0I1sJknkPyCWctTk6tOVBIU07rjcSULDWI<br>
```

<br>

For decoding, the decodeJWT function is used. The decoded header is stored in the "header" object variable, the decoded body is stored in the "payload" object variable and the signature is stored in the "signature" object variable.<br>
The function returns 1 (true) if signature verification succeeded and returns 0 (false) if signature verification fails.<br>
For example-<br>
```
char string[] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0ZW1wIjoyMi41LCJzcGVlZCI6MjUuMX0.t5CfO7HYM0I1sJknkPyCWctTk6tOVBIU07rjcSULDWI";
Serial.println(jwt.decodeJWT(string));
Serial.print(jwt.header);
Serial.print(" ");
Serial.print(jwt.payload);
Serial.print(" ");
Serial.println(jwt.signature);
``` 
<br>
The expected outputs are -<br>

```
1
{"alg":"HS256","typ":"JWT"} {"temp":22.5,"speed":25.1} t5CfO7HYM0I1sJknkPyCWctTk6tOVBIU07rjcSULDWI
```
