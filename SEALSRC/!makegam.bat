gcc -g -c grfx.c -o grfx.o
rem dlxmake.exe -x grfx.exp grfx.o
gcc -g -c grfx.c -o grfx.o
dlxgen.exe grfx.dlx grfx.o



gcc -g -c tictac.c -o tictac.o
dlxgen.exe ./programs/games/tictac.exe tictac.o

gcc -g -c solvor.c -o solvor.o
dlxgen.exe ./programs/games/solvor.exe solvor.o



::gcc -g -c .c -o .o
::dlxgen.exe ./programs/games/.exe .o



