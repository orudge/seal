:: Use msound.bat to make sfa*.dlx
:: Use msound.bat p to make the sounddriver
::

if "%1"=="p" goto player

gcc -g -c dlxsound.c -o dlxsound.o
dlxgen.exe dlxsound.dlx dlxsound.o

goto end

gcc -g -c sfamidi.c -o sfamidi.o
dlxgen.exe sfamidi.dlx sfamidi.o


gcc -g -c sfamp3.c -o sfamp3.o
dlxgen.exe sfamp3.dlx sfamp3.o libamp.a

goto end

:player

gcc -g -c player.c -o player.o
dlxgen.exe player.exe player.o

:end