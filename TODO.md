## TODO NEXT
- Split main.hh -> common.hh, shm.hh, time.hh, util.hh
- TReadStream/TWriteStream
- TReadBuffer/TWriteBuffer
- TReadBinaryFile/TWriteBinaryFile
- TCreature
- TPlayer
- TNonPlayer

## Estimate
The decompiled file has ~115K lines of C. If we take ~15K lines to be rubbish, this can be round to ~100K. Considering a low estimate of 200 lines per day, the whole process could take up to 500 days which is quite a bit but not impossible. Now considering a high estimate of 1K lines per day, it could take 100 days which is also quite a bit.

## TODO AFTER FIRST PASS
- Trim some rough edges.
- Avoid unsafe libc functions like `strcpy`, `strncpy`, `strcat`, `sprintf` etc...
- Handle connections inline with `poll` (probably?).
- Review signal usage for timing (SIGALRM, etc...).
- Support Windows.
