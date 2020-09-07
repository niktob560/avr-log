#include <avr/io.h> //include for base avr funcs
#include <util/atomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace Log
{

const size_t 	critRingLen = 255, 	//2 bytes
				impRingLen 	= 255,	//2 bytes
				optRingLen 	= 255;	//2 bytes

extern size_t	critCount,			//2 bytes
				impCount,			//2 bytes
				optCount;			//2 bytes

									//will be called by Log::dump() functions
extern 	void 	(*defaultDumpFunc)(const unsigned char*);//2 bytes
extern 	uint8_t (*defaultTickFunc)(void);//2 bytes

extern unsigned char 	*critLogRing[critRingLen], //arrays of 0-terminated cstrings
						*impLogRing	[impRingLen],
						*optLogRing	[optRingLen];

//Log record must be like
//<1 byte header>
//<info without zeroes>
//0x00

//1 byte header must be like
//<TYPE><TYPE><TYPE><TIME><TIME><TIME><TIME><TIME>

//type bits is enum type value
//time bits is 0-32 systick value that returns from function defaultTickFunc

enum Importance
{
	Critical,
	Important,
	Optional
};

enum Type 
{
	Info 	= 5, //101
	Debug 	= 4, //100
	Warn 	= 3, //011
	Err 	= 2, //010
	WTF 	= 1  //001
};

   /*
    *	Function:	write
    *	Desc:		Wtite <type> to <importance> log ring
    *	Input:			const Importance importance: 	importance of log record
	*					const Type type:				type of log
	*					const char *cstr: 				null-terminated string log record
    *	Output:		none
    */
void write(const Type type, const Importance importance, const char *cstr)
__attribute__((nonnull(3)));

   /*
    *	Function:	info
    *	Desc:		Wtite additional info to <importance> log ring
    *	Input:			const Importance importance: 	importance of log record
	*					const char *cstr: 				null-terminated string log record
    *	Output:		none
    */
void info(const Importance importance, const char *cstr)
__attribute__((nonnull(2)));

   /*
    *	Function:	debug
    *	Desc:		Wtite debug info to <importance> log ring
    *	Input:			const Importance importance: 	importance of log record
	*					const char *cstr: 				null-terminated string log record
    *	Output:		none
    */
void debug(const Importance importance, const char *cstr)
__attribute__((nonnull(2)));
inline void d(const Importance importance, const char *cstr) {debug(importance, cstr);}

   /*
    *	Function:	warn
    *	Desc:		Wtite warning to <importance> log ring
    *	Input:			const Importance importance: 	importance of log record
	*					const char *cstr: 				null-terminated string log record
    *	Output:		none
    */
void warn(const Importance importance, const char *cstr)
__attribute__((nonnull(2)));
inline void w(const Importance importance, const char *cstr) {warn(importance, cstr);}

   /*
    *	Function:	err
    *	Desc:		Wtite error to <importance> log ring
    *	Input:			const Importance importance: 	importance of log record
	*					const char *cstr: 				null-terminated string log record
    *	Output:		none
    */
void err(const Importance importance, const char *cstr)
__attribute__((nonnull(2)));
inline void e(const Importance importance, const char *cstr) {err(importance, cstr);}

   /*
    *	Function:	wtf
    *	Desc:		Wtite wat-a-terrible-failtrue to <importance> log ring
    *	Input:			const Importance importance: 	importance of log record
	*					const char *cstr: 				null-terminated string log record
    *	Output:		none
    */
void wtf(const Importance importance, const char *cstr)
__attribute__((nonnull(2)));

   /*
    *	Function:	dump
    *	Desc:		Dump <importance> log ring to specifyed function
    *	Input:			const Importance importance: 	importance of log ring
	*					void (*func)(unsigned char): 			function that gets logs unsigned char-by-unsigned char
    *	Output:		none
    */
void dump(const Importance importance, void (*func)(const unsigned char*))
__attribute__((nonnull(2)));

   /*
    *	Function:	dump
    *	Desc:		Dump <importance> log ring to default dump function
    *	Input:			const Importance importance: 	importance of log ring
    *	Output:		none
    */
void dump(const Importance importance);


   /*
    *	Function:	dump
    *	Desc:		Dump all log rings to specifyed function
    *	Input:		none
    *	Output:		none
    */
void dump();

} // namespace Log
