#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "JSON.h"

#define eps 1e-8

void testNull(){
    JSON *obj = CreateNULL();
    if (obj->type != JSON_NULL)
        puts("null wrong type");
    DestroyObject(obj);
}

void testFalse(){
    JSON *obj = CreateFalse();
    if (obj->type != JSON_FALSE)
        puts("false wrong type");
    if (obj->valueint != 0)
        puts("false wrong value");
    DestroyObject(obj);
}

void testTrue(){
    JSON *obj = CreateTrue();
    if (obj->type != JSON_TRUE)
        puts("true wrong type");
    if (obj->valueint != 1)
        puts("true wrong value");
    DestroyObject(obj);
}

void testNumber(){
    JSON *obj = CreateNumber(123.123);
    if (obj->type != JSON_NUMBER)
        puts("number wrong type");
    if (abs(obj->valuedouble - 123.123)>eps)
        puts("number wrong value");
    DestroyObject(obj);
}

void testString(){
    JSON *obj = CreateString("123");
    if (obj->type != JSON_STRING)
        puts("string wrong type");
    if (strcmp(obj->valuestring, "123") != 0)
        puts("string wrong value");
    DestroyObject(obj);
}

void testArray(){
    JSON *obj = CreateArray();
    if (obj->type != JSON_ARRAY)
        puts("array wrong type");
}

void testObject(){
    JSON *obj = CreateObject();
    if (obj->type != JSON_OBJECT)
        puts("object wrong type");
}

void testParse(){
    JSON *obj = ParseJSONFromFile("test_array.json");
    PrintJSON(obj);
    PrintJSON(ParseJSONFromFile("test_object.json"));
}

int main(){
    testNull();
    testFalse();
    testTrue();
    testNumber();
    testString();
    testArray();
    testObject();
    testParse();

    return 0;
}
