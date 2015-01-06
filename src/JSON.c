#include "JSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void DestroyObject(JSON *obj){
}

JSON *CreateNULL(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = 2;
    return obj;
}

JSON *CreateFalse(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = 0;
    obj->valueint = 0;
    return obj;
}

JSON *CreateTrue(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = 1;
    obj->valueint = 1;
    return (JSON*)calloc(1,JSON);
}

JSON *CreateNumber(double ival){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = 3;
    obj->valuedouble = ival;
    return obj;
}

JSON *CreateString(const char *s){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = 4;
    int length = strlen(s);
    obj->valuestring = (char*)calloc(length+1, sizeof(char));
    memcpy(obj->valuestring, s, length*sizeof(char));
    return obj;
}

JSON *CreateArray(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = 5;
    obj->next = NULL;
}

JSON *CreateObject(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = 6;
    obj->next = NULL;
}

JSON *CreateEntry(char *str, JSON *value){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
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


void ReplaceItemInArray(JSON *array, int which, JSON *new_item){
    JSON *it = array->next, *last = array;
    int count = 0;
    for ( ; it != NULL&& count < which; it = it->next, last = last->next, ++count);
    assert(count == which);
    last->next = new_item;
    new_item->next = it->next;
    DestroyObject(it);
}

void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value){
    JSON *it = object->next, *last = object;
    int count = 0;
    for ( ; it != NULL&& strcmp(it->valuestring, key) != 0; it = it->next, last = last->next);
    assert(it != NULL);
    DestroyObject(it->value);
    it->value = new_value;
}

void DetachItemFromArray(JSON *array, int which){
    JSON *it = array->next, *last = array;
    int count = 0;
    for ( ; it != NULL&& count < which; it = it->next, last = last->next, ++count);
    assert(count == which);
    last->next = it->next;
    return it;
}

void DeleteItemFromArray(JSON *array, int which){
    JSON *obj = DetachItemFromArray(array, which);
    DestroyObject(obj);
}

JSON *DetachItemFromObject(JSON *object, const char *key){
    JSON *it = object->next, *last = object;
    int count = 0;
    for ( ; it != NULL&& strcmp(it->valuestring, key) == 0; it = it->next, last = last->next);
    assert(it != NULL);
    JSON *ret = it->value;
    it->value = NULL;
    last->next = it->next;
    DestroyObject(it);
    return ret;
}

void DeleteItemFromObject(JSON *object, const char *key){
    JSON *obj = DetachItemFromObject(object, key);
    DestroyObject(obj);
}
