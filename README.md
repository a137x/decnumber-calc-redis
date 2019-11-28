# DECNUMBERCALC FOR REDIS

Module that will allow you to make decimal calculations with redis.
DecNumber library used - http://download.icu-project.org/files/decNumber/decNumber-icu-368.zip

### 1. Decimal number calculation module

A minimal module (module.o) implementing a few commands.

**For now it includes 6 commands:**

* `DECIMAL.ADD` - adds two numbers together.
* `DECIMAL.SUBSTRACT` - substract second number from the first one.
* `DECIMAL.MULTIPLY` - multiply two numbers.  
* `DECIMAL.QUANTIZE` - quantizing number with given exponent and rounding mode.
* `DECIMAL.DIVIDE` - divide first number by second.
* `DECIMAL.POWER` - raise a first number to a power of second number.

Possible rounding modes:

    |ROUNDING MODE|how it works|
    |-------|-------|
    |ROUND_CEILING|round towards +infinity|
    |ROUND_UP| round away from 0|
    |ROUND_HALF_UP| 0.5 rounds up|
    |ROUND_HALF_EVEN| 0.5 rounds to nearest even|
    |ROUND_HALF_DOWN| 0.5 rounds down|
    |ROUND_DOWN| round towards 0 (truncate)|
    |ROUND_FLOOR| round towards -infinity|
    |ROUND_05UP| round for reround|

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
127.0.0.1:9979> DECIMAL.QUANTIZE 0.1337 0.01 ROUND_UP
"0.14"
127.0.0.1:6379> DECIMAL.QUANTIZE 0.1337 0.01 ROUND_DOWN
"0.13"
127.0.0.1:6379> DECIMAL.POWER 10 -8
"1E-8"
127.0.0.1:6379> DECIMAL.POWER 10 -3
"0.001"
127.0.0.1:6379> DECIMAL.POWER 10 -3.3
"0.0005011872336272722850015541868849458"
127.0.0.1:6379> DECIMAL.POWER 1.337 8.08
"10.45061312363680871642582659256680"
127.0.0.1:6379> DECIMAL.DIVIDE 10 23
"0.4347826086956521739130434782608696"
```

or use inside lua script:
```
redis.call("DECIMAL.ADD", "0.1337", "0.1337")
```



Build using https://github.com/RedisLabs/RedisModulesSDK

Enjoy!