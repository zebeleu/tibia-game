#include "common.hh"

static void (*ErrorFunction)(const char *Text) = NULL;
static void (*PrintFunction)(int Level, const char *Text) = NULL;

void SetErrorFunction(TErrorFunction *Function){
	ErrorFunction = Function;
}

void SetPrintFunction(TPrintFunction *Function){
	PrintFunction = Function;
}

void error(char *Text, ...){
	char s[1024];

	va_list ap;
	va_start(ap, Text);
	vsnprintf(s, sizeof(s), Text, ap);
	va_end(ap);

	if(ErrorFunction){
		ErrorFunction(s);
	}else{
		printf("%s", s);
	}
}

void print(int Level, char *Text, ...){
	char s[1024];

	va_list ap;
	va_start(ap, Text);
	vsnprintf(s, sizeof(s), Text, ap);
	va_end(ap);

	if(PrintFunction){
		PrintFunction(Level, s);
	}else{
		printf("%s", s);
	}
}
