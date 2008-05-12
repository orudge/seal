gcc -g -c pngsup.c -o pngsup.o
dlxgen.exe pngsup.dlx pngsup.o libldpng.a libpng.a libz.a

::goto end

gcc -g -c desktop.c -o desktop.o
rem dlxmake.exe -x desktop.exp desktop.o
gcc -g -c desktop.c -o desktop.o
dlxgen.exe desktop.dlx desktop.o
dlxinsd.exe desktop.dlx desktop.dat

::goto end

gcc -g -c gbmsup.c -o gbmsup.o
dlxgen.exe gbmsup.dlx gbmsup.o gbm.a


gcc -g -c jpegsup.c -o jpegsup.o
dlxgen.exe jpegsup.dlx jpegsup.o libjpeg.a

gcc -g -c dialogs.c -o dialogs.o
rem dlxmake.exe -x dialogs.exp dialogs.o
gcc -g -c dialogs.c -o dialogs.o
dlxgen.exe dialogs.dlx dialogs.o

gcc -g -c app.c -o app.o
rem dlxmake.exe -x app.exp app.o
gcc -g -c app.c -o app.o
dlxgen.exe app.dlx app.o
dlxinsd.exe app.dlx app.dat

gcc -g -c button.c -o button.o
rem dlxmake.exe -x button.exp button.o
gcc -g -c button.c -o button.o
dlxgen.exe button.dlx button.o

gcc -g -c menus.c -o menus.o
rem dlxmake.exe -x menus.exp menus.o
gcc -g -c menus.c -o menus.o
dlxgen.exe menus.dlx menus.o

gcc -g -c scroller.c -o scroller.o
rem dlxmake.exe -x scroller.exp scroller.o
gcc -g -c scroller.c -o scroller.o
dlxgen.exe scroller.dlx scroller.o

gcc -g -c iodlg.c -o iodlg.o
rem dlxmake.exe -x iodlg.exp iodlg.o
gcc -g -c iodlg.c -o iodlg.o
dlxgen.exe iodlg.dlx iodlg.o

gcc -g -c editor.c -o editor.o
rem dlxmake.exe -x editor.exp editor.o
gcc -g -c editor.c -o editor.o
dlxgen.exe editor.dlx editor.o

gcc -g -c load_htm.c -o load_htm.o
rem dlxmake.exe -x load_htm.exp load_htm.o
gcc -g -c load_htm.c -o load_htm.o
dlxgen.exe load_htm.dlx load_htm.o
dlxinsd.exe load_htm.dlx load_htm.dat

gcc -g -c htmlwin.c -o htmlwin.o
rem dlxmake.exe -x htmlwin.exp htmlwin.o
gcc -g -c htmlwin.c -o htmlwin.o
dlxgen.exe htmlwin.dlx htmlwin.o
dlxinsd.exe htmlwin.dlx htmlwin.dat

rem gcc -g -c grfx.c -o grfx.o
rem dlxmake.exe -x grfx.exp grfx.o
rem gcc -g -c grfx.c -o grfx.o
rem dlxgen.exe grfx.dlx grfx.o

rem gcc -g -c taskobj.c -o taskobj.o
rem dlxmake.exe -x taskobj.exp taskobj.o
rem gcc -g -c taskobj.c -o taskobj.o
rem dlxgen.exe taskobj.dlx taskobj.o

:end
