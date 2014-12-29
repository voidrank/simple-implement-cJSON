#include "JSON.h"
#include <stdio.h>
#include <stdlib.h>


JSON *CreateNULL(void){
    JSON *obj = (JSON*)calloc(1, JSON);
    obj->type = 2;
    return obj;
}

JSON *CreateFalse(void){
    JSON *obj = (JSON*)calloc(1, JSON);
    obj->type = 0;
    obj->valueint = 0;
    return obj;
}

JSON *CreateTrue(void){
    JSON *obj = (JSON*)calloc(1, JSON);
    obj->type = 1;
    obj->valueint = 1;
    return (JSON*)calloc(1,JSON);
}

JSON *CreateNumber(double ival){
    JSON *obj = (JSON*)calloc(1, JSON);
    obj->type = 3;
    obj->valuedouble = ival;
    return obj;
}

JSON *CreateString(char *s){
    JSON *obj = (JSON*)calloc(1, JSON);
    int length = strlen(s);
    obj->valuestring = (char*)calloc(length+1, char);
    memcpy(obj->valuestring, s, length*sizeof(char));
    return obj;
}

JSON *CreateArray(void){
    JSON *obj = (JSON*)calloc(1, JSON);
    obj->type = 5;
    obj->next = NULL;
}

JSON *CreateObject(void){
    JSON *obj = (JSON*)calloc(1, JSON);
    obj->type = 6;
}

JSON *CreateEntry(char *str, JSON *value){
    JSON *obj = (JSON*)calloc(1, JSON);
    obj->type = 7;
    int length = strlen(str);
    obj->valuestring = (char*)calloc(length+1, sizeof(char));
    memcpy(obj->valuestring, str, length*sizeof(char));
    obj->value = value;
    obj->next = NULL;
    return obj;
}

void AddItemToArray(JSON *array, JSON *item){
    JSON *it = array;
    for ( ; it->next != NULL; it = it->next);
    it->next = item;
}

//notice : because of name "Add",
//it indicate us never to care above
//repetitive entry
//what a stupid name
void AddItemToObject(JSON *object, const char *key, JSON *value){
    JSON *it = object;
    for ( ; it->next != NULL; it = it->next);
    JSON *entry = CreateEntry(key, value);
    it->next = entry;
}

