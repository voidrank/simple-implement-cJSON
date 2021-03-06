#ifndef JSON_H
#define JSON_H

/* cJSON Types: */
#define JSON_FALSE 0
#define JSON_TRUE 1
#define JSON_NULL 2
#define JSON_NUMBER 3
#define JSON_STRING 4
#define JSON_ARRAY 5
#define JSON_OBJECT 6
#define JSON_ENTRY 7

/* The cJSON structure: */
typedef struct JSON {
	int type;					/* The type of the item, as above. */

	char *valuestring;			/* The item's string, if type==JSON_STRING */
	int valueint;				/* The item's number, if type==JSON_TRUE||JSON_FALSE */
	double valuedouble;			/* The item's number, if type==JSON_NUMBER  */
    struct JSON *array;         /* The item's array */
    char *key;                  /* The item's entry */
    struct JSON *value;         /* The item's entry */
    struct JSON *next;          /* The item's next element of arrayitem||entryitem */
    struct JSON *last;          /* The item's last element of arrayitem||entryitem */
    struct JSON *son;           /* The item's son element of array||object */
} JSON;

/* Parse & Print */
extern JSON *non_type_parse(void);
extern JSON *ParseJSON(const char *value);
extern JSON *ParseJSONFromFile(const char *file_name);

extern void PrintJSON(JSON *item);
extern void PrintJSONToFile(JSON *item, const char *file_name);

/* Create */
extern JSON *CreateNULL(void);
extern JSON *CreateTrue(void);
extern JSON *CreateFalse(void);
extern JSON *CreateBool(int b);
extern JSON *CreateNumber(double num);
extern JSON *CreateString(const char *string);
extern JSON *CreateArray(void);
extern JSON *CreateObject(void);
extern JSON *CreateEntry(const char *key, JSON *value);

/* Append */
extern void AddItemToArray(JSON *array, JSON *item);
extern void AddItemToObject(JSON *object, const char *key, JSON *value);
extern void AddEntryToObject(JSON *object, JSON *entry);

/* Update */
extern void ReplaceItemInArray(JSON *array, int which, JSON *new_item);
extern void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value);

/* Remove/Delete */
extern JSON *DetachItemFromArray(JSON *array, int which);
extern void DeleteItemFromArray(JSON *array, int which);
extern JSON *DetachItemFromObject(JSON *object, const char *key);
extern void DeleteItemFromObject(JSON *object, const char *key);

extern void DeleteJSON(JSON *item);

/* Duplicate */
extern JSON *Duplicate(JSON *item, int recurse);

/* Read */
extern JSON *GetItemInArray(JSON *array, int which);
extern JSON *GetItemInObject(JSON *object, const char *key);
extern JSON *GetItemInJSON(JSON *json, const char *path);

/* free */
extern void DestroyObject(JSON *obj);


#endif
