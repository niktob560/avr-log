#include "logger.hpp"


namespace Log
{

size_t	critCount,				//2 bytes
		impCount,				//2 bytes
		optCount;				//2 bytes

								//will be called by Log::dump() functions
void (*defaultDumpFunc)(char);	//2 bytes
uint8_t (*defaultTickFunc)(void);//2 bytes


char 	*critLogRing[critRingLen], //arrays of 0-terminated cstrings
		*impLogRing	[impRingLen],
		*optLogRing	[optRingLen];


constexpr char** getRing(const Importance importance)
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
			return NULL;
	}
}

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
			return NULL;
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
	char **ring = getRing(importance);
	size_t *counter = getCounter(importance);
	size_t len = 0;

	while (ring[*counter][len])	//count len of log record
		len++;

	if(ring[*counter] != 0x00)	//clean memory
		free(ring[*counter]);
		
	
	ring[*counter] = (char*)malloc(sizeof(char) * (len + 1));	//allocate memory
	ring[*counter][0] = (type << 5) | (defaultTickFunc() & 0x1F);//put header
	memcpy(ring[*counter] + 1, cstr, len);//copy log record
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


void dump(const Importance importance, void (*func)(char))
{
	char **ring = getRing(importance);
	size_t counter = *getCounter(importance);


	auto print = [ring, func](size_t i)
	{
		size_t len = 0;
		while (ring[i][len])
		{
			func(ring[i][len]);
			len++;
		}
		func(0);
	};

	for(size_t i = counter + 1; i < getMaxLen(importance); i++)
		print(i);
	for(size_t i = 0; i < counter; i++)
		print(i);
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
