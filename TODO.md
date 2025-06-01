## TODO NEXT
- MAGIC.CC
- merge creature headers into a single one CR.HH
	- this should help preventing dependency cycles and centralize creature data
		structures and globals in a single place
- CRMAIN.CC

## Stack allocations
Any functions that use `alloca` or some other form of dynamic stack allocations will cause decompiled functions to be an absolute mess. It usually shows up in the decompiled code as both a size computation like `-(VAR + CONST & 0xfffffff0)`, followed by some assignment. It doesn't make total sense without looking at the disassembly. I've encountered ~30 such computations and expect the functions containing them to be amongt the most challenging/annoying to be properly decompiled.

## Estimate
The decompiled file has ~115K lines of C. If we take ~15K lines to be rubbish, this can be round to ~100K. Considering a low estimate of 200 lines per day, the whole process could take up to 500 days which is quite a bit but not impossible. Now considering a high estimate of 1K lines per day, it could take 100 days which is also quite a bit.

## TODO AFTER FIRST PASS
- Trim rough edges.
- Avoid unsafe libc functions like `strcpy`, `strncpy`, `strcat`, `sprintf` etc...
- Handle connections inline with `poll`/`epoll` (probably?).
- Remove exceptions.
- Review signal usage for timing (SIGALRM, etc...).
- Support Windows.
