# avr-log
Logger library that contains three ring buffers and provides time-based logging. Inspired by Android ``logcat``  
This library is platform-independent and can be easilly ported into an another architectures

## Init:
### Provide dump function
It will be used for dumping log records into an external storage
```C++
Log::defaultDumpFunc = [](const unsigned char* c)
{
//Dump char here
};
```  

### Provide systick function (optional)
Systick is a in-system time measurement tool and used for marking log records
```C++
Log::defaultTickFunc = []() -> uint8_t {return systick;};
```
By default initialized with zero-returning lambda-function

## Usage:
### Importance:
There is three levels of importance: ``Critical``, ``Important`` and ``Optional`` (maybe can be renamed into serve in future) and three ring-buffers for an each of it
### Log type:
There is five log record types: ``Info``, ``Debug``, ``Warn``, ``Err`` and ``WTF`` (What-a-Terrible-Failtrue)
### Example:
```C++
Log::defaultDumpFunc = [](const unsigned char* c){c++;};
Log::w(Log::Importance::Critical, "Critical warn");
Log::e(Log::Importance::Critical, "Critical error");
Log::w(Log::Importance::Optional, "Optional warn");
Log::wtf(Log::Importance::Important, "Important terrible failtrue");
```

### Dumping:
Here is two variants of dump: all rings or via importance  
All importance rings dumping in a next seq: ``Critical``, ``Important`` and ``Optional``

## Log records structure:
Every log record contains:
- Non-zero header with type and timestampp
- Zero-terminated C-like string
  
More info about real numbers can be found in a [logger.hpp file](https://github.com/niktob560/avr-log/blob/master/Include/logger.hpp)  
Length of ring-buffers can be modifyed in [logger.hpp file](https://github.com/niktob560/avr-log/blob/master/Include/logger.hpp)  
