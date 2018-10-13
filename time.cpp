#include "time.h"

time_t CurrentTime()
{
	time_t curTime = 0;
	time(&curTime);
	static time_t startTime = time(&curTime);

	return curTime - startTime;
}
