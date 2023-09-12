CFLAGS=-DHAVE_CONFIG_H -D_LARGEFILE64_SOURCE=1 -g -Wall -fPIC -D_GNU_SOURCE -g -O2 -Werror


all: LustreOss 
LustreOss: lustreapi 
		g++ -g3 -fPIC  -I./src/ -c *.cc -std=c++14
		g++ -shared -L$(XRD_PATH)/lib -Wl,-soname,LibXrdLustreOss.so.4,--export-dynamic -o LibXrdLustreOss.so *.o libLust.a -lXrdCl -L$(LUSTRE_PATH)/lib -llustreapi
lustreapi:
	gcc -std=c99 -I$(LUSTRE_PATH)/include $(CFLAGS) -c  lfs.c
	ar rcs  libLust.a lfs.o 
clean:
	-rm *.o *.a LibXrdLustreOss.so test
clang-format:
	clang-format -i -style=file lfs.c LustreOss.cc LustreOss.hh qsStruct.h
