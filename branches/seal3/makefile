# Seal 3.0 Makefile
# Written by Owen Rudge

CFLAGS   = -s -Iinclude -O3 -DSEAL_PLATFORM_DOS=1 -DSEAL_KERNEL=1
CPPFLAGS = -s -Iinclude -O3 -DSEAL_PLATFORM_DOS=1 -DSEAL_KERNEL=1

OBJ_DIR = obj

OBJ_LIST = \
$(OBJ_DIR)/main.o \
$(OBJ_DIR)/debug.o \
$(OBJ_DIR)/platform/dos/execfile.o \
$(OBJ_DIR)/platform/dos/sxl.o

LIB_LIST = 

all : $(OBJ_LIST) sxlgen.exe bin/seal.exe
	@echo Seal has been built.

$(OBJ_DIR)/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	gcc $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cc
	gcc $(CPPFLAGS) -c $< -o $@

sxlgen.exe : obj/platform/dos/sxlgen.o
#	gcc $(CFLAGS) -o obj/platform/dos/sxlgen.o -c src/platform/dos/sxlgen.c
	gcc -o sxlgen.exe obj/platform/dos/sxlgen.o

bin/seal.exe : $(OBJ_LIST)
	gcc -o bin\seal.exe $(OBJ_LIST) $(LIB_LIST)
	exe2coff bin\seal.exe
	copy /B CWSDSTUB.EXE+bin\seal bin\seal.exe
	del bin\seal

clean :
	del $(subst /,\,$(OBJ_DIR)/platform/*.o)
	del $(subst /,\,$(OBJ_DIR)/platform/dos/*.o)
	del $(subst /,\,$(OBJ_DIR)/*.o)
	del bin\seal.exe
	del sxlgen.exe

compress :
	upx -9 bin/seal.exe sxlgen.exe