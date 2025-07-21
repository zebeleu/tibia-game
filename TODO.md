## TODO

## Synchronization
I'm not sure whether synchronization is done properly. The implementation of the first few `crplayer.cc` functions left me with a taste of race conditions although integer loads on x86 are generally atomic.

## TODO (LEFTOVER)
- Include `x == 0xFFFF` inside `CheckVisibility`?
- The map container uses type id zero but it seems to also be used as a "no type"
  id which makes me want to add some `isNone()`, `isNull()`, `isVoid()` check to
  `ObjectType` to be used as an alias when pertinent.
- Make some `TNonplayer` random step function? One version that does a random step
  and another that does a random step keeping some distance from a certain position.

- I realized that `Change` with an `ObjectType` parameter was being called where
  the version with `INSTANCEATTRIBUTE` parameter was expected due to `ObjectType`
  having an implicit conversion constructor from `int`. I think it is best to make
  all converting constructors explicit to avoid these types of errors, although
  there aren't that many function overloads.

- Define a few constants like `MAX_NAME_LENGTH`, `MAX_SKILLS`, and `MAX_OPEN_CONTAINERS`
  instead of relying on `NARRAY`.

## TODO AFTER FIRST PASS
- Remove VTABLE comments.
- Split NPC, Monster, and Player into separate header files.
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
