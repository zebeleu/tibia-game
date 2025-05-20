#ifndef TIBIA_MAIN_HH_
#define TIBIA_MAIN_HH_ 1

#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uintptr_t uintptr;
typedef size_t usize;

#define STATIC_ASSERT(expr) static_assert((expr), "static assertion failed: " #expr)
#define NARRAY(arr) (sizeof(arr) / sizeof(arr[0]))
#define ISPOW2(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)
#define KB(x) ((usize)(x) << 10)
#define MB(x) ((usize)(x) << 20)
#define GB(x) ((usize)(x) << 30)

// TODO(fusion): We might not actually compile the output for the decompilation
// step but if we did, we'd need to make sure we're compiling in 32 bits mode.
STATIC_ASSERT(sizeof(bool) == 1);
STATIC_ASSERT(sizeof(int) == 4);
//STATIC_ASSERT(sizeof(void*) == 4);

#if !OS_WINDOWS && !OS_LINUX
#	if defined(_WIN32)
#		define OS_WINDOWS 1
#	elif defined(__linux__) || defined(__gnu_linux__)
#		define OS_LINUX 1
#	else
#		error "Operating system not supported."
#	endif
#endif

#if !COMPILER_MSVC && !COMPILER_GCC && !COMPILER_CLANG
#	if defined(_MSC_VER)
#		define COMPILER_MSVC 1
#	elif defined(__GNUC__)
#		define COMPILER_GCC 1
#	elif defined(__clang__)
#		define COMPILER_CLANG 1
#	endif
#endif

// NOTE(fusion): Compiler attributes.
#if COMPILER_GCC || COMPILER_CLANG
#	define ATTR_FALLTHROUGH __attribute__((fallthrough))
#	define ATTR_PRINTF(x, y) __attribute__((format(printf, x, y)))
#else
#	define ATTR_FALLTHROUGH
#	define ATTR_PRINTF(x, y)
#endif

#if COMPILER_MSVC
#	define TRAP() __debugbreak();
#elif COMPILER_GCC || COMPILER_CLANG
#	define TRAP() __builtin_trap()
#else
#	define TRAP() abort()
#endif

#define ASSERT_ALWAYS(expr) if(!(expr)) { TRAP(); }
#if BUILD_DEBUG
#	define ASSERT(expr) ASSERT_ALWAYS(expr)
#else
#	define ASSERT(expr) ((void)(expr))
#endif

// TODO(fusion): We should re-implement the multi-process structure used by the
// original code but only for reference. Making it compile on Windows shouldn't
// be too difficult either. Overall this design is outdated and should be reviewed.
//	Nevertheless, we should focus on getting it working as intended, on the target
// platform (which is Linux 32-bits) before attempting to refine it.
STATIC_ASSERT(OS_LINUX);
#include <sys/types.h>
//

// config.cc
extern char BINPATH[4096];
extern char DATAPATH[4096];
extern char LOGPATH[4096];
extern char MAPPATH[4096];
extern char MONSTERPATH[4096];
extern char NPCPATH[4096];
extern char ORIGMAPPATH[4096];
extern char SAVEPATH[4096];
extern char USERPATH[4096];
extern int SHMKey;
extern int AdminPort;
extern int GamePort;
extern int QueryManagerPort;
extern char AdminAddress[16];
extern char GameAddress[16];
extern char QueryManagerAddress[16];
extern char QueryManagerAdminPW[9];
extern char QueryManagerGamePW[9];
extern char QueryManagerWebPW[9];
extern int DebugLevel;
extern bool PrivateWorld;
extern TWorldType WorldType;
extern char WorldName[30];
extern int MaxPlayers;
extern int MaxNewbies;
extern int PremiumPlayerBuffer;
extern int PremiumNewbieBuffer;
extern int Beat;
extern int RebootTime;
extern TDatabaseSettings ADMIN_DATABASE;
extern TDatabaseSettings VOLATILE_DATABASE;
extern TDatabaseSettings WEB_DATABASE;
extern TDatabaseSettings FORUM_DATABASE;
extern TDatabaseSettings MANAGER_DATABASE;
extern int NumberOfQueryManagers;
extern TQueryManagerSettings QUERY_MANAGER[10];
void ReadConfig(void);

// shm.cc
void StartGame(void);
void CloseGame(void);
void EndGame(void);
bool LoginAllowed(void);
bool GameRunning(void);
bool GameStarting(void);
bool GameEnding(void);
pid_t GetGameThreadPID(void);
int GetPrintlogPosition(void);
char *GetPrintlogLine(int Line);
void IncrementObjectCounter(void);
void DecrementObjectCounter(void);
uint32 GetObjectCounter(void);
void IncrementPlayersOnline(void);
void DecrementPlayersOnline(void);
int GetPlayersOnline(void);
void IncrementNewbiesOnline(void);
void DecrementNewbiesOnline(void);
int GetNewbiesOnline(void);
void SetRoundNr(uint32 RoundNr);
uint32 GetRoundNr(void);
void SetCommand(int Command, char *Text);
int GetCommand(void);
char *GetCommandBuffer(void);
void InitSHM(bool Verbose);
void ExitSHM(void);
void InitSHMExtern(bool Verbose);
void ExitSHMExtern(void);

// time.cc
extern uint32 RoundNr;
extern uint32 ServerMilliseconds;
void GetRealTime(int *Hour, int *Minute);
void GetTime(int *Hour, int *Minute);
void GetDate(int *Year, int *Cycle, int *Day);
void GetAmbiente(int *Brightness, int *Color);
uint32 GetRoundAtTime(int Hour, int Minute);
uint32 GetRoundForNextMinute(void);

// util.cc
typedef void TErrorFunction(const char *Text);
typedef void TPrintFunction(int Level, const char *Text);
void SetErrorFunction(TErrorFunction *Function);
void SetPrintFunction(TPrintFunction *Function);
void error(char *Text, ...) ATTR_PRINTF(1, 2);
void print(int Level, char *Text, ...) ATTR_PRINTF(1, 2);

#endif //TIBIA_MAIN_HH_
