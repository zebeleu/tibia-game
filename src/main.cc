#include "common.hh"
#include "config.hh"
#include "map.hh"
#include "objects.hh"

#include "stubs.hh"

#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fstream>

static bool BeADaemon = false;
static bool Reboot = false;
static bool SaveMapOn = false;

static int SigAlarmCounter = 0;
static int SigUsr1Counter = 0;

static sighandler_t handler(int signr, sighandler_t sighandler){
	struct sigaction act;
	struct sigaction oldact;

	act.sa_handler = sighandler;

	// TODO(fusion): I feel we should probably use `sigfillset` specially for
	// signals that share the same handler.
	sigemptyset(&act.sa_mask);

	// TODO(fusion): We had this weird logic in the decompiled version of this
	// function but it looks like some GLIBC internal thing, since these values
	// would mask signals 1020 and 1021 which don't really make sense.
	//
	//if(signr == SIGALRM){
	//	act.sa_mask.__val[0x1f] = 0x20000000;
	//}else{
	//	act.sa_mask.__val[0x1f] = 0x10000000;
	//}
	//

	if(sigaction(signr, &act, &oldact) == 0){
		return oldact.sa_handler;
	}else{
		return SIG_ERR;
	}
}

static void SigHupHandler(int signr){
	// no-op (?)
}

static void SigAbortHandler(int signr){
	print(1, "SigAbortHandler: schalte Writer-Thread ab.\n");
	AbortWriter();
}

static void DefaultHandler(int signr){
	print(1, "DefaultHandler: Beende Game-Server (SigNr. %d: %s).\n",
			signr, sigdescr_np(signr));

	handler(SIGINT, SIG_IGN);
	handler(SIGQUIT, SIG_IGN);
	handler(SIGTERM, SIG_IGN);
	handler(SIGXCPU, SIG_IGN);
	handler(SIGXFSZ, SIG_IGN);
	handler(SIGPWR, SIG_IGN);

	SaveMapOn = (signr == SIGQUIT) || (signr == SIGTERM) || (signr == SIGPWR);
	if(signr == SIGTERM){
		int Hour, Minute;
		GetRealTime(&Hour, &Minute);
		RebootTime = (Hour * 60 + Minute + 6) % 1440;
		CloseGame();
	}else{
		EndGame();
	}

	Reboot = false;
}

// TODO(fusion): This function was exported in the binary but not referenced anywhere.
static void ErrorHandler(int signr){
	error("ErrorHandler: SigNr. %d: %s\n", signr, sigdescr_np(signr));
	EndGame();
	LogoutAllPlayers();
	exit(1);
}

static void InitSignalHandler(void){
	int count = 0;
	count += (handler(SIGHUP, SigHupHandler) != SIG_ERR);
	count += (handler(SIGINT, DefaultHandler) != SIG_ERR);
	count += (handler(SIGQUIT, DefaultHandler) != SIG_ERR);
	count += (handler(SIGABRT, SigAbortHandler) != SIG_ERR);
	count += (handler(SIGUSR1, SIG_IGN) != SIG_ERR);
	count += (handler(SIGUSR2, SIG_IGN) != SIG_ERR);
	count += (handler(SIGPIPE, SIG_IGN) != SIG_ERR);
	count += (handler(SIGALRM, SIG_IGN) != SIG_ERR);
	count += (handler(SIGTERM, DefaultHandler) != SIG_ERR);
	count += (handler(SIGSTKFLT, SIG_IGN) != SIG_ERR);
	count += (handler(SIGCHLD, SIG_IGN) != SIG_ERR);
	count += (handler(SIGTSTP, SIG_IGN) != SIG_ERR);
	count += (handler(SIGTTIN, SIG_IGN) != SIG_ERR);
	count += (handler(SIGTTOU, SIG_IGN) != SIG_ERR);
	count += (handler(SIGURG, SIG_IGN) != SIG_ERR);
	count += (handler(SIGXCPU, DefaultHandler) != SIG_ERR);
	count += (handler(SIGXFSZ, DefaultHandler) != SIG_ERR);
	count += (handler(SIGVTALRM, SIG_IGN) != SIG_ERR);
	count += (handler(SIGWINCH, SIG_IGN) != SIG_ERR);
	count += (handler(SIGPOLL, SIG_IGN) != SIG_ERR);
	count += (handler(SIGPWR, DefaultHandler) != SIG_ERR);
	print(1, "InitSignalHandler: %d Signalhandler eingerichtet (Soll=%d)\n", count, 0x1c);
}

static void ExitSignalHandler(void){
	// no-op
}

static void SigAlarmHandler(int signr){
	SigAlarmCounter += 1;

	struct itimerval new_timer = {};
	struct itimerval old_timer = {};
	new_timer.it_value.tv_usec = Beat * 1000;
	setitimer(ITIMER_REAL, &new_timer, &old_timer);
}

static void InitTime(void){
	SigAlarmCounter = 0;
	handler(SIGALRM, SigAlarmHandler);
	SigAlarmHandler(SIGALRM);
}

static void ExitTime(void){
	struct itimerval new_timer = {};
	struct itimerval old_timer = {};
	setitimer(ITIMER_REAL, &new_timer, &old_timer);
	handler(SIGALRM, SIG_IGN);
}

static void UnlockGame(void){
	// TODO(fusion): Probably use snprintf to format file name?
	char FileName[4096];
	strcpy(FileName, SAVEPATH);
	strcat(FileName, "/game.pid");

	std::ifstream InputFile(FileName, std::ios_base::in);
	if(!InputFile.fail()){
		int Pid;
		InputFile >> Pid;

		if(Pid == getpid()){
			unlink(FileName);
		}
	}
}

static void LockGame(void){
	// TODO(fusion): Probably use snprintf to format file name?
	char FileName[4096];
	strcpy(FileName, SAVEPATH);
	strcat(FileName, "/game.pid");

	{
		std::ifstream InputFile(FileName, std::ios_base::in);
		if(!InputFile.fail()){
			int Pid;
			InputFile >> Pid;
			if(Pid != 0){
				throw "Game-Server is already running, PID file exists.";
			}
		}
	}

	{
		std::ofstream OutputFile(FileName, std::ios_base::out | std::ios_base::trunc);
		OutputFile << (int)getpid();
	}

	atexit(UnlockGame);
}

void LoadWorldConfig(void){
#if 0
	// TODO(fusion): Whenever we implement query/database stuff.
	TQueryManagerConnection Connection(0x4000);
	if(Connection.WriteBuffer.Position < 0){
		error("LoadWorldConfig: Kann nicht zum Query-Manager verbinden.\n");
		throw "cannot connect to querymanager";
	}

	int HelpGameAddress[4];
	int Ret = Connection.loadWorldConfig(&WorldType, &RebootTime, HelpGameAddress,
		&Port, &MaxPlayers, &PremiumPlayerBuffer, &MaxNewbies, &PremiumNewbieBuffer);
	if(Ret != 0){ // TODO(fusion): Maybe `Ret != QUERY_OK` or something?
		error("LoadWorldConfig: Kann Konfigurationsdaten nicht holen.\n");
		throw "cannot load world config";
	}

	// NOTE(fusion): Ugh...
	snprintf(GameAddress, sizeof(GameAddress), "%d.%d.%d.%d",
				HelpGameAddress[0], HelpGameAddress[1],
				HelpGameAddress[2], HelpGameAddress[3]);
#endif

	WorldType = NORMAL;
	RebootTime = 6 * 60; // minutes
	strcpy(GameAddress, "127.0.0.1"); // I KNOW
	GamePort = 7171;
	MaxPlayers = 1000;
	PremiumPlayerBuffer = 100;
	MaxNewbies = 200;
	PremiumNewbieBuffer = 50;
}

static void InitAll(void){
	try{
		ReadConfig();
		//SetQueryManagerLoginData(1, WorldName);
		LoadWorldConfig();
		InitSHM(!BeADaemon);
		LockGame();
		//InitLog("game");
		srand(time(NULL));
		InitSignalHandler();
		//InitConnections();
		//InitCommunication();
		InitStrings();
		//InitWriter();
		//InitReader();
		InitObjects();
		InitMap();
		//InitInfo();
		//InitMoveUse();
		//InitMagic();
		//InitCr();
		//InitHouses();
		InitTime();
		//ApplyPatches();
	}catch(const char *str){
		error("Initialisierungsfehler: %s\n", str);
		exit(EXIT_FAILURE);
	}
}

static void ExitAll(void){
	// TODO(fusion): Probably missing some inlined empty `Exit*` functions here.

	EndGame();
	ExitTime();
	//ExitCr();
	//ExitMagic();
	//ExitMoveUse();
	//ExitInfo();
	//ExitHouses();
	ExitMap(SaveMapOn);
	ExitObjects();
	//ExitReader();
	//ExitWriter();
	ExitStrings();
	//ExitCommunication();
	//ExitConnections();
	ExitSignalHandler();
	ExitSHM();
}

static void ProcessCommand(void){
	int Command = GetCommand();
	if(Command != 0){
		char *Buffer = GetCommandBuffer();
		if(Command == 1){
			if(Buffer != NULL){
				BroadcastMessage(TALK_ADMIN_MESSAGE, "%s", Buffer);
			}else{
				error("ProcessCommand: Text für Broadcast ist NULL.\n");
			}
		}else{
			error("ProcessCommand: Unbekanntes Kommando %d.\n", Command);
		}

		SetCommand(0, NULL);
	}
}

static void AdvanceGame(int Delay){
	static int CreatureTimeCounter = 0;
	static int CronTimeCounter = 0;
	static int SkillTimeCounter = 0;
	static int OtherTimeCounter = 0;
	static int OldAmbiente = -1;
	static uint32 NextMinute = 30;
	static bool Lag = false;

	CreatureTimeCounter += Delay;
	CronTimeCounter += Delay;
	SkillTimeCounter += Delay;
	OtherTimeCounter += Delay;

	if(CreatureTimeCounter >= 1750){
		CreatureTimeCounter -= 1000;
		ProcessCreatures();
	}

	if(CronTimeCounter >= 1500){
		CronTimeCounter -= 1000;
		ProcessCronSystem();
	}

	if(SkillTimeCounter >= 1250){
		SkillTimeCounter -= 1000;
		ProcessSkills();
	}

	if(OtherTimeCounter >= 1000){
		OtherTimeCounter -= 1000;

		RoundNr += 1;
		SetRoundNr(RoundNr);

		ProcessConnections();
		ProcessMonsterhomes();
		ProcessMonsterRaids();
		ProcessCommunicationControl();
		ProcessReaderThreadReplies(RefreshSector, SendMails);
		ProcessWriterThreadReplies();
		ProcessCommand();

		// TODO(fusion): Shouldn't we be checking both brightness and color?
		int Brightness, Color;
		GetAmbiente(&Brightness, &Color);
		if(OldAmbiente != Brightness){
			OldAmbiente = Brightness;
			TConnection *Connection = GetFirstConnection();
			while(Connection != NULL){
				// TODO(fusion): This is probably an inlined function that checks
				// whether the connection is still going. The exact decompiled condition
				// was `Connection->State - CONDITION_LOGIN < 4` but I think that's
				// just a compiler optimization that wouldn't work properly on the
				// decompiled version. That comparison in the disassembly is unsigned
				// (`JBE`) but the enum is signed which would probably generate an
				// invalid signed comparison (`JLE`).
				//
				//	MOV EAX, dword ptr [Connection + Connection->State]
				//	SUB EAX, 0x3
				//	CMP EAX, 0x3
				//	JBE ... -> SendAmbiente(Connection)
				//
				if(Connection->State == CONNECTION_LOGIN
				|| Connection->State == CONNECTION_GAME
				|| Connection->State == CONNECTION_DEAD
				|| Connection->State == CONNECTION_LOGOUT){
					SendAmbiente(Connection);
				}
				Connection = GetNextConnection();
			}
		}

		if(RoundNr % 10 == 0){
			NetLoadCheck();
		}

		if(RoundNr >= NextMinute){
			int Hour, Minute;
			GetRealTime(&Hour, &Minute);

			RefreshCylinders();
			if(Minute % 5 == 0){
				CreatePlayerList(true);
			}
			if(Minute % 15 == 0){
				SavePlayerDataOrder();
			}
			if(Minute == 0){
				NetLoadSummary();
			}
			if(Minute == 55){
				WriteKillStatistics();
			}

			int RealTime = Minute + Hour * 60;
			if((RealTime + 5) % 1440 == RebootTime){
				if(Reboot){
					BroadcastMessage(TALK_ADMIN_MESSAGE,
						"Server is saving game in 5 minutes.\nPlease come back in 10 minutes.");
				}else{
					BroadcastMessage(TALK_ADMIN_MESSAGE,
						"Server is going down in 5 minutes.\nPlease log out.");
				}
				CloseGame();
			}else if((RealTime + 3) % 1440 == RebootTime){
				if(Reboot){
					BroadcastMessage(TALK_ADMIN_MESSAGE,
						"Server is saving game in 3 minutes.\nPlease come back in 10 minutes.");
				}else{
					BroadcastMessage(TALK_ADMIN_MESSAGE,
						"Server is going down in 3 minutes.\nPlease log out.");
				}
			}else if((RealTime + 1) % 1440 == RebootTime){
				if(Reboot){
					BroadcastMessage(TALK_ADMIN_MESSAGE,
						"Server is saving game in one minute.\nPlease log out.");
				}else{
					BroadcastMessage(TALK_ADMIN_MESSAGE,
						"Server is going down in one minute.\nPlease log out.");
				}
			}else if(RealTime == RebootTime){
				CloseGame();
				LogoutAllPlayers();
				SendAll();
				if(Reboot){
					RefreshMap();
				}
				SaveMap();
				SaveMapOn = false;
				EndGame();
			}

			NextMinute = GetRoundForNextMinute();
		}
		CleanupDynamicStrings();
	}

	if(Delay > Beat){
		Log("lag", "Verzögerung %d msec.\n", Delay);
	}

	// TODO(fusion): Why would we delay creature movement yet another beat?
	if(Delay < 1000){
		MoveCreatures(Delay);
		Lag = false;
	}else{
		if(!Lag && RoundNr > 10){
			error("AdvanceGame: Keine Kreaturbewegung wegen Lag (Verzögerung: %d msec).\n", Delay);
		}
		Lag = true;
	}

	SendAll();
}

static void SigUsr1Handler(int signr){
	SigUsr1Counter += 1;
}

static void LaunchGame(void){
	SigUsr1Counter = 0;
	handler(SIGUSR1, SigUsr1Handler);
	StartGame();

	print(1, "LaunchGame: Game-Server ist bereit (Pid=%d).\n", getpid());

	// IMPORTANT(fusion): The whole design of the server is to run across a few
	// different processes and communicate via shared memory and signals. Each
	// of these processes are single threaded, meaning that signal handlers are
	// executed concurrently on the SAME thread. You'd still require to synchronize
	// access to large structures to avoid race conditions BUT accessing a few
	// independent integers and booleans like we're doing with `SigAlarmCounter`,
	// `SigUsr1Counter`, and `SaveMapOn` is perfectly safe.

	SaveMapOn = true;
	SigAlarmCounter = 0;
	while(GameRunning()){
		// TODO(fusion): `sigblock` and `sigpause` are deprecated in favour of
		// `sigprocmask` and `sigsuspend`.
		sigblock(sigmask(SIGUSR1));
		while(SigUsr1Counter == 0 && SigAlarmCounter == 0){
			sigpause(0);
		}

		if(SigUsr1Counter > 0){
			SigUsr1Counter = 0;
			ReceiveData();
		}

		int NumBeats = SigAlarmCounter;
		if(NumBeats > 0){
			SigAlarmCounter = 0;
			AdvanceGame(NumBeats * Beat);
		}
	}

	LogoutAllPlayers();
}

static bool DaemonInit(bool NoFork){
	if(!NoFork){
		pid_t Pid = fork();
		if(Pid < 0){
			return true;
		}

		if(Pid != 0){
			exit(EXIT_SUCCESS);
		}

		setsid();
	}

	umask(0177);
	chdir(SAVEPATH);

	int OpenMax = sysconf(_SC_OPEN_MAX);
	if(OpenMax < 0){
		OpenMax = 1024;
	}

	for(int fd = 0; fd < OpenMax; fd += 1){
		close(fd);
	}

	return false;
}

int main(int argc, char **argv){
	bool NoFork = false;
	BeADaemon = false;
	Reboot = true;

	for(int i = 1; i < argc; i += 1){
		if(strcmp(argv[i], "daemon") == 0){
			BeADaemon = true;
		}else if(strcmp(argv[i], "nofork") == 0){
			NoFork = true;
		}
	}

	// TODO(fusion): It doesn't make sense for `DaemonInit` to even return here.
	// It either exits the parent or child process, or let it run.
	if(BeADaemon && DaemonInit(NoFork)){
		return 2;
	}

	puts("Tibia Game-Server\n(c) by CIP Productions, 2003.\n");

	InitAll();
	atexit(ExitAll);

	// TODO(fusion): The original binary does use exceptions but identifying
	// try..catch blocks are not as straightforward as throw statements. I'll
	// leave this one at the top level but we should come back to this problem
	// once we identify all throw statements and how to roughly handle them.
	try{
		LaunchGame();
	}catch(RESULT r){
		error("main: Nicht abgefangene Exception %d.\n", r);
	}catch(const char *str){
		error("main: Nicht abgefangene Exception \"%s\".\n", str);
	}catch(const std::exception &e){
		error("main: Nicht abgefangene Exception %s.\n", e.what());
	}catch(...){
		error("main: Nicht abgefangene Exception unbekannten Typs.\n");
	}

	if(!Reboot){
		print(1, "Beende Game-Server...\n");
	}else{
		UnlockGame();

		char FileName[4096];
		snprintf(FileName, sizeof(FileName), "%s/reboot-daily", BINPATH);
		if(FileExists(FileName)){
			ExitAll();
			print(1, "Starte Game-Server neu...\n");
			execv(FileName, argv);
		}else{
			print(1, "Reboot-Skript existiert nicht.\n");
		}
	}

	return 0;
}
