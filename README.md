# AoS Patcher
This program patches ace of spades client to auto load [libaos.dll](https://github.com/DryByte/aos.dll) (you can modify that if you want).

### Build
Tested with gcc and mingw-gcc
```
gcc -o patcher ./main.c
```

### Running
```
./patcher <path to client.exe>
```