## TODO NEXT
- SENDING.CC
- RECEIVING.CC
- QUERY.CC
- HOUSES.CC			<- GAME
- CRNONPL.CC		<- GAME
- MOVEUSE.CC		<- GAME
- READER.CC
- WRITER.CC
- DBFUNCS.CC

## Stack allocations
Any functions that use `alloca` or some other form of dynamic stack allocations will cause decompiled functions to be an absolute mess. It usually shows up in the decompiled code as both a size computation like `-(VAR + CONST & 0xfffffff0)`, followed by some assignment. It doesn't make total sense without looking at the disassembly. I've encountered ~30 such computations and expect the functions containing them to be amongt the most challenging/annoying to be properly decompiled.

## Exceptions
I didn't dive into how exceptions are handled, but it seems that the ones related to creature actions have the outter most endpoint at `TCreature::Execute`. There can also be some checkpoints in between to modify a `RESULT` value or handle a failure.

## Synchronization
I'm not sure whether synchronization is done properly. The implementation of the first few `crplayer.cc` functions left me with a taste of race conditions although integer loads on x86 are generally atomic.

## Threading
After some initial dive into `communication.cc`, it has become clear that the original codebase relied on `LinuxThreads` which, among its quirks, assigns different PIDs to different threads, explaining the usage of `getpid()`. It also seems it could lack automatically managed thread stacks which explains `CommunicationThreadStacks`.

We'll have to address this BEFORE trying to run the server.

## Estimate
The decompiled file has ~115K lines of C. If we take ~15K lines to be rubbish, this can be round to ~100K. Considering a low estimate of 200 lines per day, the whole process could take up to 500 days which is quite a bit but not impossible. Now considering a high estimate of 1K lines per day, it could take 100 days which is also quite a bit.

## TODO AFTER FIRST PASS
- Review dividing comments. I feel like the current "//========" blends in too easily, making it hard to see.
- Trim rough edges.
- Replace unsafe libc functions like `strcpy`, `strncpy`, `strcat`, `sprintf` etc...
- Handle connections inline with `poll`/`epoll` (probably?).
- Remove exceptions.
	- Would be desirable but I feel it could change too much of the original code flow while also
		littering functions with early return checks like `if(err != NOERROR) { return err }` although
		we could also have some macros to simplify this since we know we only use `RESULT` for errors.
		But then would we be in a better place?
- Review signal usage for timing (SIGALRM, etc...).
- Support Windows.
