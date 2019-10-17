# DECNUMBERCALC FOR REDIS

Module that will allow you to make decimal calculations with redis.
DecNumber library used - http://download.icu-project.org/files/decNumber/decNumber-icu-368.zip

### 1. Decimal number calculation module

A minimal module (module.o) implementing a few commands.

**For now it includes 4 commands:**

* `DECIMAL.ADD` - adds two numbers together.
* `DECIMAL.SUBSTRACT` - substract second number from the first one.
* `DECIMAL.MULTIPLY` - multiply two numbers.  
* `DECIMAL.QUANTIZE` - quantizing number with given exponent. Rounding - DEC_ROUND_CEILING.

All comand parameters given in strings.


# Quick Start Guide

Here's what you need to do to build your first module:

0. Build Redis in a build supporting modules.
1. Build librmutil and the module by running `make`. (you can also build them seperatly by running `make` in their respective dirs)
2. Run redis loading the module: `/path/to/redis-server --loadmodule ./module.so`

Now run `redis-cli` and try the commands:

```
127.0.0.1:9979> DECIMAL.ADD 0.1337 0.1337
"0.2674"
127.0.0.1:9979> DECIMAL.SUBSTRACT 0.1337 0.1337
"0"
127.0.0.1:9979> DECIMAL.MULTIPLY 2 0.1337
"0.2674"
127.0.0.1:9979> DECIMAL.QUANTIZE 0.1337 0.001
"0.133"
```

or use inside lua script:
```
redis.call("DECIMAL.ADD", "0.1337", "0.1337")
```



Build using https://github.com/RedisLabs/RedisModulesSDK

Enjoy!