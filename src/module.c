#include "../redismodule.h"
#include "../rmutil/util.h"
#include "../rmutil/strings.h"
#include "../rmutil/test_util.h"

#define  DECNUMDIGITS 34

#include "decNumber/decNumber.h"


int decimalAddition(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    if (argc != 3) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    /*
    RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ | REDISMODULE_WRITE);
    if (RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_HASH &&
        RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_EMPTY) {
        return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
    }

    RedisModuleCallReply *currentValueReply = RedisModule_Call(ctx, "HGET", "ss", argv[1], argv[2]);
    RMUTIL_ASSERT_NOERROR(ctx, currentValueReply);
    RedisModuleString *currentValueRedisString = RedisModule_CreateStringFromCallReply(currentValueReply);
    if (!currentValueRedisString) {
        return 0;
    }
    const char *currentValueString = RedisModule_StringPtrLen(currentValueRedisString, NULL);
    const char *decrementValueString = RedisModule_StringPtrLen(argv[3], NULL);
    */

    decNumber a, b, result;                  // working numbers
    decContext set;                  // working context
    set.traps = 0;
    set.digits = DECNUMDIGITS;       // 34
    decContextDefault(&set, DEC_INIT_DECIMAL128);

    char string[DECNUMDIGITS+14];    // conversion buffer
    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL);
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL);

    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);

    decNumberAdd(&result, &a, &b, &set);            // result=b+a
    decNumberToString(&result, string);

    /* decNumber resultNum;
    decNumberSubtract(&resultNum, &currentNum, &decrementNum, &set);

    if (!decNumberIsNegative(&resultNum)) {
        decNumberToString(&resultNum, resultStr);
        RedisModuleCallReply *srep =    (ctx, "HSET", "ssc", argv[1], argv[2], resultStr);
        RMUTIL_ASSERT_NOERROR(ctx, srep);

        RedisModule_ReplyWithStringBuffer(ctx, resultStr, strlen(resultStr));
        return REDISMODULE_OK;
    }
    if (RedisModule_CallReplyType(currentValueReply) == REDISMODULE_REPLY_NULL) {
        RedisModule_ReplyWithNull(ctx);
        return REDISMODULE_OK;
    }
    */

    //RedisModule_ReplyWithSimpleString(ctx, string);
    RedisModule_ReplyWithStringBuffer(ctx, string, strlen(string));
    return REDISMODULE_OK;
}

int decimalQuantize(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);

    decNumber a, exponent, result;          // working numbers
    decContext set;                  // working context
    set.traps = 0;
    set.digits = DECNUMDIGITS;       // 34
    //TODO: make round (DEC_ROUND_UP..) set from argv[]
    set.round = DEC_ROUND_CEILING;
    decContextDefault(&set, DEC_INIT_DECIMAL128);
    char result_string[DECNUMDIGITS+14];    //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL); // what we try to quantize
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL); // exponent
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&exponent, arg2, &set);
    decNumberQuantize(&result, &a, &exponent, &set);            // result = quantize(a) with certain exponent 
    
    decNumberToString(&result, result_string);
    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;

}

int decimalMultipy(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;          // working numbers
    decContext set;                  // working context
    set.traps = 0;
    set.digits = DECNUMDIGITS;       // 34
    decContextDefault(&set, DEC_INIT_DECIMAL128);
    char result_string[DECNUMDIGITS+20];    //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberMultiply(&result, &a, &b, &set);
    
    decNumberToString(&result, result_string);
    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalSubstract(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;          // working numbers
    decContext set;                  // working context
    set.traps = 0;
    set.digits = DECNUMDIGITS;       // 34
    decContextDefault(&set, DEC_INIT_DECIMAL128);
    char result_string[DECNUMDIGITS+20];    //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberSubtract(&result, &a, &b, &set);

    decNumberToString(&result, result_string);
    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx) {
    if (RedisModule_Init(ctx, "decimal", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
        return REDISMODULE_ERR;
    }
    RMUtil_RegisterWriteCmd(ctx, "decimal.add", decimalAddition);
    RMUtil_RegisterWriteCmd(ctx, "decimal.quantize", decimalQuantize);
    RMUtil_RegisterWriteCmd(ctx, "decimal.multiply", decimalMultipy);
    RMUtil_RegisterWriteCmd(ctx, "decimal.substract", decimalSubstract);
    return REDISMODULE_OK;
}