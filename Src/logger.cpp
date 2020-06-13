#include "logger.hpp"


namespace Log
{

size_t	critCount,				//2 bytes
		impCount,				//2 bytes
		optCount;				//2 bytes

								//will be called by Log::dump() functions
void (*defaultDumpFunc)(const unsigned char*);	//2 bytes
uint8_t (*defaultTickFunc)(void) = []() -> uint8_t {return 0;};//2 bytes


unsigned char 	*critLogRing[critRingLen], //arrays of 0-terminated cstrings
		*impLogRing	[impRingLen],
		*optLogRing	[optRingLen];


constexpr unsigned char** getRing(const Importance importance)
__attribute__((returns_nonnull));

constexpr unsigned char** getRing(const Importance importance)
{
	switch (importance)
	{
		case Critical:
			return critLogRing;
		case Important:
			return impLogRing;
		case Optional:
			return optLogRing;
		default:
			return optLogRing;
	}
}
constexpr size_t* getCounter(const Importance importance)
__attribute__((returns_nonnull));


constexpr size_t* getCounter(const Importance importance)
{
	switch (importance)
	{
		case Critical:
			return &critCount;
		case Important:
			return &impCount;
		case Optional:
			return &optCount;
		default:
			return &optCount;
	}
}

constexpr size_t getMaxLen(const Importance importance)
{
	switch (importance)
	{
		case Critical:
			return critRingLen;
		case Important:
			return impRingLen;
		case Optional:
			return optRingLen;
		default:
			return 0;
	}
}



void write(const Type type, const Importance importance, const char *cstr)
{
	unsigned char **ring = getRing(importance);
	size_t *counter = getCounter(importance);
	size_t len = 0;

	len = strlen(cstr);
	
	if(ring[*counter] != 0x00)																	//clean memory
	{
		free(ring[*counter]);
		ring[*counter] = 0x00;
	}
	
	ring[*counter] = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * (len + 1)));	//allocate memory
	
	ring[*counter][0] = static_cast<unsigned char>((type << 5) | (defaultTickFunc() & 0x1F));	//put header

	// memcpy(ring[*counter] + 1, cstr, len);														//copy log record
	for(size_t i = 0; i < len; i++)
		ring[*counter][i + 1] = cstr[i];
	

	if(*counter + 1 < getMaxLen(importance))
		*counter = *counter + 1;
	else
		*counter = 0;
	
}

void info(const Importance importance, const char *cstr)
{
	write(Info, importance, cstr);
}



void debug(const Importance importance, const char *cstr)
{
	write(Debug, importance, cstr);
}



void warn(const Importance importance, const char *cstr)
{
	write(Warn, importance, cstr);
}



void err(const Importance importance, const char *cstr)
{
	write(Err, importance, cstr);
}



void wtf(const Importance importance, const char *cstr)
{
	write(WTF, importance, cstr);
}


void dump(const Importance importance, void (*func)(const unsigned char*))
{

	unsigned char **ring = getRing(importance);
	const size_t counter = *getCounter(importance);

	for(size_t i = counter + 1; i < getMaxLen(importance); i++)
		if(ring[i] != 0x00)
			func(ring[i]);
	for(size_t i = 0; i < counter; i++)
		if(ring[i] != 0x00)
			func(ring[i]);
	
}



void dump(const Importance importance)
{
	dump(importance, defaultDumpFunc);
}




void dump()
{
	dump(Critical);
	dump(Important);
	dump(Optional);
}

} // namespace Log
