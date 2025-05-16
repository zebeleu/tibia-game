#include "main.hh"

#if 0
// TODO(fusion): All globals should probably be packed into a `GlobalState`
// structure to minimize collisions and bugs.
struct GlobalState{
	// world state
	// creatures
	// items
	// etc...
};

GlobalState G = {};
#endif

// TODO(fusion): Probably current server time. Used for delays, timers, etc.
int ServerMilliseconds = 0;

void (*ErrorFunction)(const char*) = NULL;

void error(char *Text, ...){
	char s[1024];

	va_list ap;
	va_start(ap, Text);
	vsnprintf(s, sizeof(s) - 1, Text, ap);
	va_end(ap);

	if(ErrorFunction){
		ErrorFunction(s);
	}else{
		printf("%s", s);
	}
}

int main(int argc, char **argv){
	return 0;
}
