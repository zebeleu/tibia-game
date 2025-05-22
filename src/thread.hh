#ifndef TIBIA_THREAD_HH_
#define TIBIA_THREAD_HH_ 1

#include "main.hh"
#include <pthread.h>

typedef pthread_t ThreadHandle;
typedef int (ThreadFunction)(void *);

// TODO(fusion): Probably have another way to tell whether the thread was
// successfully spawned or not?
constexpr ThreadHandle INVALID_THREAD_HANDLE = 0;

ThreadHandle StartThread(ThreadFunction *Function, void *Argument, bool Detach);
int JoinThread(ThreadHandle Handle);
void DelayThread(int Seconds, int MicroSeconds);

struct Semaphore {
	// REGULAR FUNCTIONS
	// =========================================================================
	Semaphore(void);
	~Semaphore(void);
	void up(void);
	void down(void);

	// DATA
	// =========================================================================
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t condition;
};

#endif //TIBIA_THREAD_HH_
