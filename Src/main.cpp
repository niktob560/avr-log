#include "logger.hpp"


int main()
{
	
	Log::defaultDumpFunc = [](const unsigned char* c){c++;};
	Log::w(Log::Importance::Critical, "ABC");
	Log::e(Log::Importance::Critical, "ABC");
	Log::w(Log::Importance::Critical, "ABC");
	Log::wtf(Log::Importance::Important, "ABCsad");
	return 0;
}