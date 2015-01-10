#include "JSON.h"
#include <string.h>
#include <stdio.h>

void __PrintJSON(JSON *item){
    if (item->type == JSON_TRUE){
        printf("true");
    }
    else if (item->type == JSON_FALSE){
        printf("false");
    }
    else if (item->type == JSON_NULL){
        printf("null");
    }
    else if (item->type == JSON_NUMBER){
        printf("%.9f", item->valuedouble);
    }
    else if (item->type == JSON_STRING){
        printf("\"");
        printf("%s", item->valuestring);
        printf("\"");
    }
    else if (item->type == JSON_ARRAY){
        printf("[");
        JSON *obj = GetItemInArray(item,0);
        if (obj != NULL){
            __PrintJSON(obj);
            obj = obj->next;
            for ( ; obj != NULL; obj = obj->next){
                printf(",");
                __PrintJSON(obj);
            }
        }
        printf("]");
    }
    else if (item->type==JSON_OBJECT){
        printf("{");
        JSON *obj = item->son;
        if (obj != NULL){
            __PrintJSON(obj);
            obj = obj->next;
            for ( ; obj != NULL; obj = obj->next){
                printf(",");
                __PrintJSON(obj);
            }
        }
        printf("}");
    }
    else if (item->type==JSON_ENTRY){
        printf("\"%s\":", item->key);
        __PrintJSON(item->value);
    }
}

void PrintJSON(JSON *object){
    __PrintJSON(object);
    puts("");
}
