#include "reader.hh"
#include "threads.hh"

static Semaphore OrderBufferEmpty(200);
static Semaphore OrderBufferFull(0);
static TDynamicWriteBuffer HelpBuffer(KB(64));
