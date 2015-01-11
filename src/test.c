#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JSON.h"

int TestSomeFuncs() {
	
	int score = 10;
	
	// 调用同学们实现的接口
	JSON *root = CreateObject();
	// 评测1 是否能正确创建Object
	if (root->type != JSON_OBJECT) { // 类型不对
		score -= 2;
	}
	
	AddItemToObject(root, "name", CreateString("Hello World"));
	// 评测2 是否能正确AddItemToObject并且读取
	JSON *value = GetItemInObject(root, "name");
	// 类型不对或者值不对
	if (value->type != JSON_STRING || strcmp(value->valuestring, "Hello World")) {
		score -= 2;
	}
	
	JSON *array = CreateArray();
	AddItemToArray(array, CreateBool(0));
	AddItemToArray(array, CreateNumber(2.3));
	// 评测3 是否能正确AddItemToArray并且读取
	JSON *item = GetItemInArray(array, 1);
	if (item->type != JSON_NUMBER || item->valuedouble != 2.3) {
		score -= 2;
	}
	
	AddItemToObject(root, "array", array);
	// 现在root的状态
	/*
	{
		"name": "Hello Wrold",
		"array": [
			false,
			2.3
		]
	}
	*/
	
	// 评测4 是否能正确地根据路径读取值
	item = GetItemInJSON(root, "/array/0");
	if (item->type != JSON_FALSE) {
		score -= 2;
	}
	
	PrintJSONToFile(root, "test.json");
	// 评测5 是否与标准答案文件相同
	return score;
}

int main() {
	printf("Score: %d\n", TestSomeFuncs());
	return 0;
}
