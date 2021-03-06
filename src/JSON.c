#include "JSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void DestroyObject(JSON *obj){
    if (obj == NULL) return;
    switch (obj->type){
        case JSON_STRING:{
            free(obj->valuestring);
            break;
        }
        case JSON_ARRAY:{
            JSON *item = obj->son, *nxt_item;
            for ( ; item != NULL; item = nxt_item){
                nxt_item = item->next;
                DestroyObject(item);
            }
            break;
        }
        case JSON_OBJECT:{
            JSON *item = obj->son, *nxt_item;
            for ( ; item != NULL; item = nxt_item){
                nxt_item = item->next;
                DestroyObject(item);
            }
            break;
        }
        case JSON_ENTRY:{
            free(obj->key);
            DestroyObject(obj->value);
            break;
        }
    }
    free(obj);
}

JSON *CreateNULL(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_NULL;
    return obj;
}

JSON *CreateFalse(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_FALSE;
    obj->valueint = 0;
    return obj;
}

JSON *CreateTrue(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_TRUE;
    obj->valueint = 1;
    return obj;
}

JSON *CreateBool(int b){
    return b?CreateTrue():CreateFalse();
}

JSON *CreateNumber(double ival){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_NUMBER;
    obj->valuedouble = ival;
    return obj;
}

JSON *CreateString(const char *s){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_STRING;
    int length = strlen(s);
    obj->valuestring = (char*)calloc(length+1, sizeof(char));
    memcpy(obj->valuestring, s, length*sizeof(char));
    return obj;
}

JSON *CreateArray(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_ARRAY;
    return obj;
}

JSON *CreateObject(void){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_OBJECT;
    return obj;
}

JSON *CreateEntry(const char *str, JSON *value){
    JSON *obj = (JSON*)calloc(1, sizeof(JSON));
    obj->type = JSON_ENTRY;
    int length = strlen(str);
    obj->key = (char*)calloc(length+1, sizeof(char));
    memcpy(obj->key, str, length*sizeof(char));
    obj->value = value;
    return obj;
}

void AddItemToArray(JSON *array, JSON *item){
    JSON *it = array->son;
    if (it != NULL){
        for ( ; it->next != NULL; it = it->next);
        it->next = item;
        item->last = it;
        item->next = NULL;
    }
    else{
        array->son = item;
        item->next = NULL;
        item->last = NULL;
    }
}

//notice : because of name "Add",
//it indicate us never to care above
//repetitive entry
//what a stupid name
void AddEntryToObject(JSON *object, JSON *item){
    JSON *it = object->son;
    if (it != NULL){
        item->next = it;
        it->last = item;
        object->son = item;
    }
    else{
        object->son = item;
    }
}

void AddItemToObject(JSON *object, const char *key, JSON *value){
    JSON *it = object->son;
    JSON *entry = CreateEntry(key, value);
    AddEntryToObject(object, entry);
}


void ReplaceItemInArray(JSON *array, int which, JSON *new_item){
    JSON *it = array->son;
    if (it==NULL)
        return; // fail
    int count = 0;
    for ( ; it != NULL&& count < which; it = it->next, ++count);
    assert(count == which);
    new_item->next = it->next;
    if (it->next != NULL)
        it->next->last = new_item;
    new_item->last = it->last;
    if (it->last != NULL)
        it->last->next = new_item;
    DestroyObject(it);
}

void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value){
    JSON *it = object->son;
    if (it==NULL)
        return; //fail
    for ( ; it != NULL&& strcmp(it->key, key) != 0; it = it->next);
    if (it==NULL)
        return; //fail
    DestroyObject(it->value);
    it->value = new_value;
}

JSON *DetachItemFromArray(JSON *array, int which){
    JSON *it = array->son;
    int count = 0;
    for ( ; it != NULL&& count < which; it = it->next, ++count);
    assert(count == which);
    if (it->last != NULL)
        it->last->next = it->next;
    if (it->next != NULL)
        it->next->last = it->last;
    if (it == array->son)
        array->son = it->next;
    it->next = NULL;
    it->last = NULL;
    return it;
}

void DeleteItemFromArray(JSON *array, int which){
    JSON *obj = DetachItemFromArray(array, which);
    DestroyObject(obj);
}

JSON *DetachItemFromObject(JSON *object, const char *key){
    JSON *it = object->son;
    assert(it!=NULL);
    for ( ; it != NULL&& strcmp(it->key, key) != 0; it = it->next);
    assert(it != NULL);
    if (it->last!=NULL)
        it->last->next = it->next;
    if (it->next!=NULL)
        it->next->last = it->last;
    if (it == object->son)
        object->son = it->next;
    it->next = NULL;
    it->last = NULL;
    JSON *ret = it->value;
    it->value = NULL;
    DestroyObject(it);
    return ret;
}

void DeleteItemFromObject(JSON *object, const char *key){
    JSON *obj = DetachItemFromObject(object, key);
    DestroyObject(obj);
}


JSON *Duplicate(JSON *obj, int recurse){
    switch (obj->type){
        case JSON_NULL:
            return CreateNULL();
        case JSON_TRUE:
            return CreateTrue();
        case JSON_FALSE:
            return CreateFalse();
        case JSON_NUMBER:
            return CreateNumber(obj->valuedouble);
        case JSON_STRING:
            return CreateString(obj->valuestring);
        case JSON_ARRAY:{
            JSON *new_obj = CreateArray();
            if (recurse == 0)
                new_obj->son = obj->son;
            else{
                if (obj->son != NULL){
                    new_obj->son = Duplicate(obj->son, 1);
                    JSON *old_item = obj->son, *new_item = new_obj->son;
                    for ( ; old_item->next != NULL; old_item = old_item->next, new_item = new_item->next){
                        JSON *temp = Duplicate(old_item->next,1);
                        new_item->next = temp;
                        temp->last = new_item;
                    }
                }
            }
            return new_obj;
        }
        case JSON_OBJECT:{
            JSON *new_obj = CreateObject();
            if (recurse == 0)
                new_obj->son = obj->son;
            else{
                if (obj->son != NULL){
                    new_obj->son = Duplicate(obj->son, 1);
                    JSON *old_item = obj->son, *new_item = new_obj->son;
                    for ( ; old_item->next != NULL; old_item = old_item->next, new_item = new_item->next){
                        JSON *temp = Duplicate(old_item->next,1);
                        new_item->next = temp;
                        temp->last = new_item;
                    }
                }
            }
            return new_obj;
        }
        case JSON_ENTRY:{
            JSON *new_obj = CreateEntry(obj->key, Duplicate(obj->value,1));
            return new_obj;
        }
        default:
            return NULL;
    }
}

JSON *GetItemInArray(JSON *object, int which){
    JSON *it = object->son;
    int count = 0;
    for ( ; it!=NULL&& count<which; ++count, it = it->next);
    return it;
}

JSON *GetItemInObject(JSON *object, const char *key){
    JSON *it = object->son;
    for (; it!=NULL&& strcmp(it->key,key)!=0 ; it = it->next);
    assert(it!=NULL);
    return it->value;
}

JSON *__GetItemInJSON(JSON *object, char *key){
    if (object==NULL) return NULL; // fail
    if (key == NULL|| *key == 0) return object;
    else if (object->type == JSON_ARRAY){
        int which = 0;
        char *pos = key+1;
        while (*pos != 0&& *pos != '/'){
            which = which*10 + *pos-'0';
            ++pos;
        }
        return __GetItemInJSON(GetItemInArray(object, which), pos);
    }
    else if (object->type == JSON_OBJECT){
        char *op = key+1, *ed = op;
        for( ; *ed != 0&& *ed != '/'; ++ed);
        char *query_string = (char*)calloc(ed-op+1, sizeof(char));
        memcpy(query_string, op, sizeof(char)*(ed-op));
        return __GetItemInJSON(GetItemInObject(object, query_string), ed);
    }
    else
        return NULL;
}

JSON *GetItemInJSON(JSON *object, const char *key){
    int length = strlen(key);
    char *buffer = (char*)calloc(length+1, sizeof(char));
    memcpy(buffer, key, length*sizeof(char));
    return __GetItemInJSON(object, buffer);
}

