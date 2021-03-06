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
    
    decNumber a, b, result;                             // working numbers
    decContext set;                                     // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);       // settings set.traps = 0, set.digits = 34 while doing this

    char string[DECNUMDIGITS+14];    // conversion buffer
    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL);
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL);

    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberAdd(&result, &a, &b, &set);            // result=b+a
    decNumberToString(&result, string);

    RedisModule_ReplyWithStringBuffer(ctx, string, strlen(string));
    return REDISMODULE_OK;
}

int decimalQuantize(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 4) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);

    char result_string[DECNUMDIGITS+14];            // string for result
    decNumber a, exponent, result;                  // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    
    const char *rounding_mode = RedisModule_StringPtrLen(argv[3], NULL);
    //As we can't use switch for strings:
    if (strcmp(rounding_mode, "ROUND_CEILING") == 0) {
        set.round = DEC_ROUND_CEILING;                              /* round towards +infinity         */
    } else if (strcmp(rounding_mode, "ROUND_UP") == 0) {
        set.round = DEC_ROUND_UP;                                   /* round away from 0               */
    } else if (strcmp(rounding_mode, "ROUND_HALF_UP") == 0) {
        set.round = DEC_ROUND_HALF_UP;                              /* 0.5 rounds up                   */
    } else if (strcmp(rounding_mode, "ROUND_HALF_EVEN") == 0) {
        set.round = DEC_ROUND_HALF_EVEN;                            /* 0.5 rounds to nearest even      */
    } else if (strcmp(rounding_mode, "ROUND_HALF_DOWN") == 0) {
        set.round = DEC_ROUND_HALF_DOWN;                            /* 0.5 rounds down                 */
    } else if (strcmp(rounding_mode, "ROUND_DOWN") == 0) {
        set.round = DEC_ROUND_DOWN;                                 /* round towards 0 (truncate)      */
    } else if (strcmp(rounding_mode, "ROUND_FLOOR") == 0) {
        set.round = DEC_ROUND_FLOOR;                                /* round towards -infinity         */
    } else if (strcmp(rounding_mode, "ROUND_05UP") == 0) {
        set.round = DEC_ROUND_05UP;                                 /* round for reround               */
    } else {
        return RedisModule_ReplyWithSimpleString(ctx, "Wrong rounding type. Use: ROUND_CEILING, ROUND_UP, ROUND_HALF_UP, ROUND_HALF_EVEN, ROUND_HALF_DOWN, ROUND_DOWN, ROUND_FLOOR or ROUND_05UP.");
    }

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
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberMultiply(&result, &a, &b, &set);
    decNumberToString(&result, result_string);

    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalSubtract(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberSubtract(&result, &a, &b, &set);
    decNumberToString(&result, result_string);
    
    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalDivide(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberDivide(&result, &a, &b, &set);
    decNumberToString(&result, result_string);

    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalPower(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 3) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[1], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[2], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberPower(&result, &a, &b, &set);
    decNumberToString(&result, result_string);

    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

//**************************** CLUSTER COMMANDS ****************************
//ARGV[1] is KEY from database, it's important when you work with cluster. It defines on which server
// we will execute command; especially this is important wen call commands from LUA script
int decimalAdditionCluster(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {  
    if (argc != 4) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                             // working numbers
    decContext set;                                     // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);       // settings set.traps = 0, set.digits = 34 while doing this

    char string[DECNUMDIGITS+14];    // conversion buffer
    const char *arg1 = RedisModule_StringPtrLen(argv[2], NULL);
    const char *arg2 = RedisModule_StringPtrLen(argv[3], NULL);

    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberAdd(&result, &a, &b, &set);            // result=b+a
    decNumberToString(&result, string);

    RedisModule_ReplyWithStringBuffer(ctx, string, strlen(string));
    return REDISMODULE_OK;
}

int decimalQuantizeCluster(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 5) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);

    char result_string[DECNUMDIGITS+14];            // string for result
    decNumber a, exponent, result;                  // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    
    const char *rounding_mode = RedisModule_StringPtrLen(argv[4], NULL);
    //As we can't use switch for strings:
    if (strcmp(rounding_mode, "ROUND_CEILING") == 0) {
        set.round = DEC_ROUND_CEILING;                              /* round towards +infinity         */
    } else if (strcmp(rounding_mode, "ROUND_UP") == 0) {
        set.round = DEC_ROUND_UP;                                   /* round away from 0               */
    } else if (strcmp(rounding_mode, "ROUND_HALF_UP") == 0) {
        set.round = DEC_ROUND_HALF_UP;                              /* 0.5 rounds up                   */
    } else if (strcmp(rounding_mode, "ROUND_HALF_EVEN") == 0) {
        set.round = DEC_ROUND_HALF_EVEN;                            /* 0.5 rounds to nearest even      */
    } else if (strcmp(rounding_mode, "ROUND_HALF_DOWN") == 0) {
        set.round = DEC_ROUND_HALF_DOWN;                            /* 0.5 rounds down                 */
    } else if (strcmp(rounding_mode, "ROUND_DOWN") == 0) {
        set.round = DEC_ROUND_DOWN;                                 /* round towards 0 (truncate)      */
    } else if (strcmp(rounding_mode, "ROUND_FLOOR") == 0) {
        set.round = DEC_ROUND_FLOOR;                                /* round towards -infinity         */
    } else if (strcmp(rounding_mode, "ROUND_05UP") == 0) {
        set.round = DEC_ROUND_05UP;                                 /* round for reround               */
    } else {
        return RedisModule_ReplyWithSimpleString(ctx, "Wrong rounding type. Use: ROUND_CEILING, ROUND_UP, ROUND_HALF_UP, ROUND_HALF_EVEN, ROUND_HALF_DOWN, ROUND_DOWN, ROUND_FLOOR or ROUND_05UP.");
    }

    const char *arg1 = RedisModule_StringPtrLen(argv[2], NULL); // what we try to quantize
    const char *arg2 = RedisModule_StringPtrLen(argv[3], NULL); // exponent
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&exponent, arg2, &set);
    decNumberQuantize(&result, &a, &exponent, &set);            // result = quantize(a) with certain exponent 
    decNumberToString(&result, result_string);

    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalMultipyCluster(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 4) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[2], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[3], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberMultiply(&result, &a, &b, &set);
    decNumberToString(&result, result_string);

    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalSubtractCluster(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 4) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[2], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[3], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberSubtract(&result, &a, &b, &set);
    decNumberToString(&result, result_string);
    
    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalDivideCluster(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 4) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[2], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[3], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberDivide(&result, &a, &b, &set);
    decNumberToString(&result, result_string);

    RedisModule_ReplyWithStringBuffer(ctx, result_string, strlen(result_string));
    return REDISMODULE_OK;
}

int decimalPowerCluster(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 4) {
        return RedisModule_WrongArity(ctx);
    }
    RedisModule_AutoMemory(ctx);
    
    decNumber a, b, result;                         // working numbers
    decContext set;                                 // working context
    decContextDefault(&set, DEC_INIT_DECIMAL128);   // settings set.traps = 0, set.digits = 34 while doing this
    char result_string[DECNUMDIGITS+20];            //string for result

    const char *arg1 = RedisModule_StringPtrLen(argv[2], NULL); // a
    const char *arg2 = RedisModule_StringPtrLen(argv[3], NULL); // b
    decNumberFromString(&a, arg1, &set);
    decNumberFromString(&b, arg2, &set);
    decNumberPower(&result, &a, &b, &set);
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
    RMUtil_RegisterWriteCmd(ctx, "decimal.subtract", decimalSubtract);
    RMUtil_RegisterWriteCmd(ctx, "decimal.divide", decimalDivide);
    RMUtil_RegisterWriteCmd(ctx, "decimal.power", decimalPower);
    //cluster commands
    RMUtil_RegisterWriteCmd(ctx, "decimal.add.c", decimalAdditionCluster);
    RMUtil_RegisterWriteCmd(ctx, "decimal.quantize.c", decimalQuantizeCluster);
    RMUtil_RegisterWriteCmd(ctx, "decimal.multiply.c", decimalMultipyCluster);
    RMUtil_RegisterWriteCmd(ctx, "decimal.subtract.c", decimalSubtractCluster);
    RMUtil_RegisterWriteCmd(ctx, "decimal.divide.c", decimalDivideCluster);
    RMUtil_RegisterWriteCmd(ctx, "decimal.power.c", decimalPowerCluster);

    return REDISMODULE_OK;
}