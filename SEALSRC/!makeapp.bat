
gcc -g -c imager.c -o imager.o
dlxgen.exe ./programs/media/imager.exe imager.o

::goto end

gcc -g -c dc.c -o dc.o
dlxgen.exe ./programs/apps/dc.exe dc.o

::goto end

gcc -g -c cdplayer.c -o cdplayer.o
dlxgen.exe ./programs/media/cdplayer.exe cdplayer.o libbcd.a

gcc -g -c ./library/src/soundlib/sound.c -o ./library/src/soundlib/sound.o
dlxgen.exe ./programs/media/sound.exe ./library/src/soundlib/sound.o

gcc -g -c smalldit.c -o smalldit.o
dlxgen.exe ./programs/apps/smalldit.exe smalldit.o

gcc -g -c hpage.c -o hpage.o
dlxgen.exe ./programs/apps/hpage.exe hpage.o


gcc -g -c visdjgpp.c -o visdjgpp.o
dlxgen.exe ./programs/apps/visdjgpp.exe visdjgpp.o

gcc -g -c setscr.c -o setscr.o
dlxgen.exe ./settings/setscr.exe setscr.o

:end
