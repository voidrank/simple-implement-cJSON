#include "JSON.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


//TODO: special char(like control-character) assertion
//

char *s;

static char str_value(){
    return *s;
}

static char *str_addr(){
    return s;
}

static void set_char(char c){
    *s = c;
}

static int is_end(){
    if (str_value() == 0)
        return 1;
    else
        return 0;
}

static void next_char(){
    ++s;
}

static void next_non_space_char(){
    while (str_value() == ' '|| str_value()=='\n')
        next_char();
}


JSON *null_type_parse(){
    next_char();
    assert(*s == 'u');
    next_char();
    assert(*s == 'l');
    next_char();
    assert(*s == 'l');
    next_char();
    return CreateNULL();
}

JSON *true_type_parse(){
    next_char();
    assert(*s == 'r');
    next_char();
    assert(*s == 'u');
    next_char();
    assert(*s == 'e');
    next_char();
    return CreateTrue();
}

JSON *false_type_parse(){
    next_char();
    assert(*s == 'a');
    next_char();
    assert(*s == 'l');
    next_char();
    assert(*s == 's');
    next_char();
    assert(*s == 'e');
    next_char();
    return CreateFalse();
}

JSON *num_type_parse(){
    double val = 0;
    int dp_found = 0;
    double div = 0;
    while ((str_value()>='0'&& str_value()<='9')|| str_value()=='.'){
        // decimal point
        if (str_value() == '.'){
            assert(dp_found==0);
            dp_found = 1;
            div = 1;
        }
        // digital
        else{
            div *= 10;
            val = val * 10 + (str_value()-'0');
        }
        next_char();
    }
    if (dp_found)
        val /= div;
    return CreateNumber(val);
}

JSON *string_type_parse(){
    char *op = str_addr();
    op++;
    next_char();
    while (str_value() != '"'){
        assert(is_end()==0);
        next_char();
    }
    set_char(0);
    JSON *obj = CreateString(op);
    set_char('"');
    next_char();
    //puts(s);
    return obj;
}

JSON *array_type_parse(){
    JSON *arr_obj = CreateArray();
    next_char();
    next_non_space_char();
    while (str_value() != ']'){
        AddItemToArray(arr_obj, non_type_parse());
        next_non_space_char();
        if (str_value()==','){
            next_char();
        }
    }
    next_char();
    return arr_obj;
}


JSON *entry_type_parse(){
    next_non_space_char(); // get char '"'
    JSON *key_obj = string_type_parse();
    next_non_space_char();
    assert(str_value() == ':');
    next_char();
    JSON *value_obj = non_type_parse();
    return CreateEntry(key_obj->valuestring, value_obj);
}

JSON *object_type_parse(){
    JSON *obj = CreateObject();
    next_char();
    next_non_space_char();
    while (str_value() != '}'){
        JSON *ent_obj = entry_type_parse();
        AddEntryToObject(obj, ent_obj);
        next_non_space_char();
        if (str_value() == ',')
            next_char();
        next_non_space_char();
    }
    next_char();
    return obj;
}



JSON *non_type_parse(){
    if (str_value() == ' ')
        next_non_space_char();
    if (str_value() == '"'){
        //string type
        return string_type_parse();
    }
    else if ( str_value()>='0'&& str_value()<='9'){
        //num type
        return num_type_parse();
    }
    else if ( str_value()=='{'){
        //object type
        return object_type_parse();
    }
    else if ( str_value()=='['){
        //array type
        return array_type_parse();
    }
    else if ( str_value()=='n'){
        //null type
        return null_type_parse();
    }
    else if ( str_value()=='f'){
        //false type
        return false_type_parse();
    }
    else if ( str_value()=='t'){
        return true_type_parse();
    }
    else 
        return NULL;
}


JSON *ParseJSON(const char *value){
    s = calloc(strlen(value), sizeof(char));
    memcpy( s, value, strlen(value)*sizeof(char));
    return non_type_parse();
}

JSON *ParseJSONFromFile(const char *filename){
    FILE *pFile;
    long lSize;
    char * buffer;
    size_t result;

    pFile = fopen( filename, "rb" );
    if (pFile==NULL) { fputs("File error", stderr); exit(1);}

    // obtain file size:
    fseek( pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs("Memory error", stderr); exit(2);}

    //copy the file into the buffer
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {fputs("Reading error", stderr); exit(2);}

    /* the whole file is now loaded in the memory buffer */

    /* parse */
    JSON *obj = ParseJSON(buffer);

    fclose(pFile);
    free(buffer);
    return obj;
}

