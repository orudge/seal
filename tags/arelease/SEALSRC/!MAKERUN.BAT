gcc -c dlxload.cc -o dlxload.o

gcc -c program.c -o program.o

gcc -o seal.exe program.o liballeg.a libalttf.a dlxload.o libmss.a