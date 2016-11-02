CFLAGS=-DHAVE_CONFIG_H -D_LARGEFILE64_SOURCE=1 -g -Wall -fPIC -D_GNU_SOURCE -g -O2 -Werror


all: LustreOss 
LustreOss: lustreapi 
		g++ -g3 -fPIC  -I$(XRD_PATH)/include/xrootd -I./src/ -c *.cc -std=c++11
		g++ -shared -L$(XRD_PATH)/lib -Wl,-soname,LibLustreOss.so.4,--export-dynamic -o LibLustreOss.so *.o libLust.a -lXrdCl -L$(LUSTRE_PATH)/trunk/lib -llustreapi
lustreapi:
	gcc -std=c99 -I$(LUSTRE_PATH)/trunk/include $(CFLAGS) -c  lfs.c
	ar rcs  libLust.a lfs.o 
test:
	gcc -std=c99 -I$(LUSTRE_PATH)/trunk/include $(CFLAGS) -c  test.c
	gcc -o test test.o -L$(LUSTRE_PATH)/trunk/lib -llustreapi
clean:
	-rm *.o *.a LibLustreOss.so test
