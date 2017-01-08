#ifndef __JSMN_H_
#define __JSMN_H_

#include <stddef.h>
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * JSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
typedef enum {
	JSMN_PRIMITIVE = 0,
	JSMN_OBJECT = 1,
	JSMN_ARRAY = 2,
	JSMN_STRING = 3
} jsmntype_t;

typedef enum {
    JSMN_NO_ERROR = 0,  /*消除警告,返回一个正确值，有个问题，没有解析到字符时也返回的是0*/
	/* Not enough tokens were provided */
	/* 提供没有足够的标记 */
	JSMN_ERROR_NOMEM = -1,
	/* Invalid character inside JSON string */
	/* 非法字符 */
	JSMN_ERROR_INVAL = -2,
	/* The string is not a full JSON packet, more bytes expected */
	/* 字符串是不是一个完整的JSON数据包，预期多个字节 */
	JSMN_ERROR_PART = -3
} jsmnerr_t;

/**
 * JSON token description.
 * @param		type	type (object, array, string etc.)
 * @param		start	start position in JSON data string,开始
 * @param		end		end position in JSON data string，结束
 */
typedef struct {
	jsmntype_t type;
	int start;
	int end;
	int size;
#ifdef JSMN_PARENT_LINKS
	int parent;
#endif
} jsmntok_t;

/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 */
typedef struct {
	unsigned int pos; /* offset in the JSON string */
	unsigned int toknext; /* next token to allocate */
	int toksuper; /* superior token node, e.g parent object or array */
} jsmn_parser;

/**
 * Create JSON parser over an array of tokens
 */
void jsmn_init(jsmn_parser *parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each describing
 * a single JSON object.
 */
/*原函数返回值是jsmnerr_t，在keil编译中出现警告，因为函数返回值有不是这个类型的值*/
int8_t jsmn_parse(jsmn_parser *parser, const char *js, size_t len,
		jsmntok_t *tokens, unsigned int num_tokens);

#ifdef __cplusplus
}
#endif

#endif /* __JSMN_H_ */
