# SEAL 2.0 Makefile
# Written by Owen Rudge

CFLAGS   = -s -Iinclude
CPPFLAGS = -s -Iinclude

OBJ_DIR = obj

OBJ_LIST = \
$(OBJ_DIR)/dlxload.o \
$(OBJ_DIR)/data/registry.o \
$(OBJ_DIR)/data/dataini.o \
$(OBJ_DIR)/object/view.o \
$(OBJ_DIR)/controls/button.o \
$(OBJ_DIR)/controls/app.o \
$(OBJ_DIR)/controls/inputbox.o \
$(OBJ_DIR)/controls/menus.o \
$(OBJ_DIR)/controls/trackbar.o \
$(OBJ_DIR)/controls/treeview.o \
$(OBJ_DIR)/controls/checkbox.o \
$(OBJ_DIR)/controls/iodlg.o \
$(OBJ_DIR)/controls/dialogs.o \
$(OBJ_DIR)/controls/editor.o \
$(OBJ_DIR)/controls/scroller.o \
$(OBJ_DIR)/controls/stattext.o \
$(OBJ_DIR)/controls/tabbook.o \
$(OBJ_DIR)/engine.o \
$(OBJ_DIR)/compat.o \
$(OBJ_DIR)/safmem.o \
$(OBJ_DIR)/text.o \
$(OBJ_DIR)/object/filter.o \
$(OBJ_DIR)/object/rect.o \
$(OBJ_DIR)/object/object.o \
$(OBJ_DIR)/grfx-f.o \
$(OBJ_DIR)/txts.o \
$(OBJ_DIR)/colors.o \
$(OBJ_DIR)/bmps.o \
$(OBJ_DIR)/skin.o \
$(OBJ_DIR)/drivers.o \
$(OBJ_DIR)/drivers/keyboard.o \
$(OBJ_DIR)/drivers/mouse.o \
$(OBJ_DIR)/drivers/savescr.o \
$(OBJ_DIR)/drivers/sound.o \
$(OBJ_DIR)/screen.o \
$(OBJ_DIR)/list.o \
$(OBJ_DIR)/lnk.o \
$(OBJ_DIR)/files.o \
$(OBJ_DIR)/vfile.o \
$(OBJ_DIR)/ico.o \
$(OBJ_DIR)/stimer.o \
$(OBJ_DIR)/controls/grfx.o \
$(OBJ_DIR)/controls/window.o \
$(OBJ_DIR)/tips.o \
$(OBJ_DIR)/xdldata.o

LIB_LIST = \
lib/libmss.a \
lib/libalttf.a \
lib/gbm.a \
lib/Libjpeg.a \
lib/libldpng.a \
lib/libpng.a \
lib/libz.a \
lib/libaastr.a

XDL_LIST = \
bin/badctrls.xdl \
bin/sfamod.xdl \
bin/sfamp3.xdl \
bin/sfamidi.xdl \
bin/sfawav.xdl \
bin/helpsys.xdl \
bin/cp.xdl \
bin/ds.xdl \
bin/ss.xdl \
bin/effect.xdl \
bin/volctrl.xdl \
bin/fire.sss \
bin/black.sss \
bin/as.xdl \
bin/ssc.xdl \
bin/m.xdl \
bin/cards.xdl

S2A_LIST = \
bin/s2a-bin/calc.s2a \
bin/s2a-bin/cdplayer.s2a \
bin/s2a-bin/s2about.s2a \
bin/s2a-bin/simp.s2a \
bin/s2a-bin/vdev.s2a \
bin/s2a-bin/dc.s2a \
bin/s2a-bin/explorer.s2a \
bin/s2a-bin/help.s2a \
bin/s2a-bin/imager.s2a \
bin/s2a-bin/player.s2a \
bin/s2a-bin/run.s2a \
bin/s2a-bin/smalldit.s2a \
bin/s2a-bin/sregedit.s2a \
bin/sealdos.s2a \
bin/desktop5.s2a \
bin/s2a-bin/fifteen.s2a \
bin/s2a-bin/laby.s2a \
bin/s2a-bin/snake.s2a \
bin/s2a-bin/sol.s2a \
bin/s2a-bin/solvor.s2a \
bin/s2a-bin/tetris.s2a \
bin/s2a-bin/tictac.s2a \
bin/s2a-bin/towofhan.s2a \
bin/s2a-bin/klondike.s2a \
bin/s2a-bin/cruel.s2a \
bin/s2a-bin/golf.s2a \
bin/s2a-bin/dskimage.s2a

all : $(OBJ_LIST) dlxgen.exe dlxmake.exe dlxinsd.exe bin/seal.exe bin/regedit.exe setup/install.exe $(XDL_LIST) $(S2A_LIST)
	@echo SEAL has been built.

$(OBJ_DIR)/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	gcc $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cc
	gcc $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/libs/%.o: src/libs/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/apps/%.o: src/apps/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/setup/%.o: src/install/%.c
	gcc $(CFLAGS) -Os -c $< -o $@

dlxgen.exe : 
	gcc $(CFLAGS) -o obj/dlxgen.o -c src/dlxgen.c
	gcc -o dlxgen.exe obj/dlxgen.o lib/libz.a

dlxmake.exe : 
	gcc $(CPPFLAGS) -o obj/dlxmake.o -c src/dlxmake.cc
	gcc -o dlxmake.exe obj/dlxmake.o

dlxinsd.exe : 
	gcc $(CFLAGS) -o obj/dlxinsd.o -c src/dlxinsd.c
	gcc -o dlxinsd.exe obj/dlxinsd.o

bin/seal.exe : $(OBJ_LIST)
	gcc -o bin\seal.exe $(OBJ_LIST) $(LIB_LIST) -lalleg
	exe2coff bin\seal.exe
	copy /B CWSDSTUB.EXE+bin\seal bin\seal.exe
	del bin\seal

bin/regedit.exe :
	gcc $(CFLAGS) -c src/data/regedit.c -o obj/regedit.o
	gcc -o bin/regedit.exe obj/regedit.o

bin/badctrls.xdl : $(OBJ_DIR)/libs/badctrls.o $(OBJ_DIR)/libs/beditor.o
	dlxgen -Cbin/badctrls.xdl obj/libs/badctrls.o obj/libs/beditor.o
	dlxinsd bin/badctrls.xdl src/libs/badctrls.dat

bin/s2a-bin/calc.s2a : $(OBJ_DIR)/apps/calc.o
	dlxgen -Cbin/s2a-bin/calc.s2a obj/apps/calc.o
	dlxinsd bin/s2a-bin/calc.s2a src/apps/calc.dat

bin/s2a-bin/cdplayer.s2a : $(OBJ_DIR)/apps/cdplayer.o
#	gcc $(CFLAGS) -fomit-frame-pointer -mpentium -funroll-loops -ffast-math -O3 -Wall -c src/apps/cdplayer.c -o obj/apps/cdplayer.o
	dlxgen -Cbin/s2a-bin/cdplayer.s2a obj/apps/cdplayer.o lib/libbcd.a

bin/sfamod.xdl : $(OBJ_DIR)/libs/sfamod.o
	dlxgen -Cbin/sfamod.xdl obj/libs/sfamod.o lib/libjgmod.a

bin/sfamp3.xdl : $(OBJ_DIR)/libs/sfamp3.o
	dlxgen -Cbin/sfamp3.xdl obj/libs/sfamp3.o lib/libamp.a

bin/sfamidi.xdl : $(OBJ_DIR)/libs/sfamidi.o
	dlxgen -Cbin/sfamidi.xdl obj/libs/sfamidi.o

bin/sfawav.xdl : $(OBJ_DIR)/libs/sfawav.o
	dlxgen -Cbin/sfawav.xdl obj/libs/sfawav.o

bin/s2a-bin/s2about.s2a : $(OBJ_DIR)/apps/s2about.o
	dlxgen -Cbin/s2a-bin/s2about.s2a obj/apps/s2about.o
	dlxinsd bin/s2a-bin/s2about.s2a src/apps/s2about.dat

# TO DO: seperate from simp.c to other files

bin/s2a-bin/simp.s2a :
	gcc $(CFLAGS) -c src/simp/simp.c -o obj/apps/simp.o
	dlxgen -Cbin/s2a-bin/simp.s2a obj/apps/simp.o
	dlxinsd bin/s2a-bin/simp.s2a src/simp/simp.dat

# TO DO: seperate from vdev.c to other files

bin/s2a-bin/vdev.s2a :
	gcc $(CFLAGS) -c src/vdev/vdev.c -o obj/apps/vdev.o
	dlxgen -Cbin/s2a-bin/vdev.s2a obj/apps/vdev.o

bin/s2a-bin/dc.s2a : $(OBJ_DIR)/apps/dc.o
	dlxgen -Cbin/s2a-bin/dc.s2a obj/apps/dc.o

bin/s2a-bin/explorer.s2a : $(OBJ_DIR)/apps/explorer.o
	dlxgen -Cbin/s2a-bin/explorer.s2a obj/apps/explorer.o

bin/s2a-bin/help.s2a : $(OBJ_DIR)/apps/help.o
	dlxgen -Cbin/s2a-bin/help.s2a obj/apps/help.o

bin/s2a-bin/imager.s2a : $(OBJ_DIR)/apps/imager.o
	dlxgen -Cbin/s2a-bin/imager.s2a obj/apps/imager.o

bin/s2a-bin/player.s2a : $(OBJ_DIR)/apps/player.o
	dlxgen -Cbin/s2a-bin/player.s2a obj/apps/player.o

bin/s2a-bin/run.s2a : $(OBJ_DIR)/apps/run.o
	dlxgen -Cbin/s2a-bin/run.s2a obj/apps/run.o

bin/sealdos.s2a : $(OBJ_DIR)/sealdos.o
	dlxgen -Cbin/sealdos.s2a obj/sealdos.o

bin/s2a-bin/smalldit.s2a : $(OBJ_DIR)/apps/smalldit.o
	dlxgen -Cbin/s2a-bin/smalldit.s2a obj/apps/smalldit.o

bin/s2a-bin/sregedit.s2a : $(OBJ_DIR)/apps/sregedit.o
	dlxgen -Cbin/s2a-bin/sregedit.s2a obj/apps/sregedit.o

bin/helpsys.xdl : $(OBJ_DIR)/apps/helpsys.o
	dlxgen -Cbin/helpsys.xdl obj/apps/helpsys.o

bin/desktop5.s2a : $(OBJ_DIR)/apps/desktop5.o
	dlxgen -Cbin/desktop5.s2a obj/apps/desktop5.o

bin/cp.xdl : $(OBJ_DIR)/libs/cp.o
	dlxgen -Cbin/cp.xdl obj/libs/cp.o

bin/ds.xdl : $(OBJ_DIR)/libs/ds.o
	dlxgen -Cbin/ds.xdl obj/libs/ds.o

bin/ss.xdl : $(OBJ_DIR)/libs/ss.o
	dlxgen -Cbin/ss.xdl obj/libs/ss.o

bin/as.xdl : $(OBJ_DIR)/libs/as.o
	dlxgen -Cbin/as.xdl obj/libs/as.o

bin/ssc.xdl : $(OBJ_DIR)/libs/ssc.o
	dlxgen -Cbin/ssc.xdl obj/libs/ssc.o

bin/m.xdl : $(OBJ_DIR)/libs/m.o
	dlxgen -Cbin/m.xdl obj/libs/m.o

bin/effect.xdl : $(OBJ_DIR)/libs/effect.o
	dlxgen -Cbin/effect.xdl obj/libs/effect.o

bin/volctrl.xdl : $(OBJ_DIR)/libs/volctrl.o
	dlxgen -Cbin/volctrl.xdl obj/libs/volctrl.o

bin/fire.sss : $(OBJ_DIR)/sss/fire.o
	dlxgen -Cbin/fire.sss obj/sss/fire.o

bin/black.sss : $(OBJ_DIR)/sss/black.o
	dlxgen -Cbin/black.sss obj/sss/black.o

bin/cards.xdl : $(OBJ_DIR)/libs/cards.o
	dlxgen -Cbin/cards.xdl obj/libs/cards.o
	dlxinsd bin/cards.xdl src/libs/cards.dat

bin/s2a-bin/fifteen.s2a : $(OBJ_DIR)/apps/games/fifteen.o
	dlxgen -Cbin/s2a-bin/fifteen.s2a obj/apps/games/fifteen.o

bin/s2a-bin/laby.s2a : $(OBJ_DIR)/apps/games/laby.o
	dlxgen -Cbin/s2a-bin/laby.s2a obj/apps/games/laby.o

bin/s2a-bin/snake.s2a : $(OBJ_DIR)/apps/games/snake.o
	dlxgen -Cbin/s2a-bin/snake.s2a obj/apps/games/snake.o
	dlxinsd bin/s2a-bin/snake.s2a src/apps/games/snake.dat

bin/s2a-bin/sol.s2a : $(OBJ_DIR)/apps/games/sol.o
	dlxgen -Cbin/s2a-bin/sol.s2a obj/apps/games/sol.o

bin/s2a-bin/solvor.s2a : $(OBJ_DIR)/apps/games/solvor.o
	dlxgen -Cbin/s2a-bin/solvor.s2a obj/apps/games/solvor.o

bin/s2a-bin/tetris.s2a : $(OBJ_DIR)/apps/games/tetris.o
	dlxgen -Cbin/s2a-bin/tetris.s2a obj/apps/games/tetris.o

bin/s2a-bin/tictac.s2a : $(OBJ_DIR)/apps/games/tictac.o
	dlxgen -Cbin/s2a-bin/tictac.s2a obj/apps/games/tictac.o

bin/s2a-bin/towofhan.s2a : $(OBJ_DIR)/apps/games/towofhan.o
	dlxgen -Cbin/s2a-bin/towofhan.s2a obj/apps/games/towofhan.o

bin/s2a-bin/klondike.s2a : $(OBJ_DIR)/apps/games/klondike.o
	dlxgen -Cbin/s2a-bin/klondike.s2a obj/apps/games/klondike.o

bin/s2a-bin/cruel.s2a : $(OBJ_DIR)/apps/games/cruel.o
	dlxgen -Cbin/s2a-bin/cruel.s2a obj/apps/games/cruel.o

bin/s2a-bin/golf.s2a : $(OBJ_DIR)/apps/games/golf.o
	dlxgen -Cbin/s2a-bin/golf.s2a obj/apps/games/golf.o

bin/s2a-bin/dskimage.s2a : $(OBJ_DIR)/apps/dskimage.o
	dlxgen -Cbin/s2a-bin/dskimage.s2a obj/apps/dskimage.o

setup/install.exe : $(OBJ_DIR)/setup/cmdparse.o $(OBJ_DIR)/setup/install.o $(OBJ_DIR)/setup/scrpages.o
	gcc -Os -o setup/install.exe $(OBJ_DIR)/setup/cmdparse.o $(OBJ_DIR)/setup/install.o $(OBJ_DIR)/setup/scrpages.o
	strip setup/install.exe
	exe2coff setup/install.exe
	copy /B CWSDSTUB.EXE+setup\install setup\install.exe
	del setup\install

clean :
	del $(subst /,\,$(OBJ_DIR)/libs/*.o)
	del $(subst /,\,$(OBJ_DIR)/apps/games/*.o)
	del $(subst /,\,$(OBJ_DIR)/apps/*.o)
	del $(subst /,\,$(OBJ_DIR)/object/*.o)
	del $(subst /,\,$(OBJ_DIR)/controls/*.o)
	del $(subst /,\,$(OBJ_DIR)/drivers/*.o)
	del $(subst /,\,$(OBJ_DIR)/setup/*.o)
	del $(subst /,\,$(OBJ_DIR)/data/*.o)
	del $(subst /,\,$(OBJ_DIR)/sss/*.o)
	del $(subst /,\,$(OBJ_DIR)/*.o)
	del setup\install.exe
	del bin\regedit.exe
	del bin\seal.exe
	del dlxgen.exe
	del dlxinsd.exe
	del dlxmake.exe

clean-xdl :
	del $(subst /,\,bin/s2a-bin/*.s2a)
	del $(subst /,\,bin/*.s2a)
	del $(subst /,\,bin/*.xdl)
	del $(subst /,\,bin/*.sss)

compress :
	upx -9 bin/seal.exe bin/regedit.exe dlxgen.exe dlxinsd.exe dlxmake.exe setup/install.exe