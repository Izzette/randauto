randauto:
	gcc -Wall -Ofast -shared -o librandauto.so lib/randauto.c

randauto-local:
	gcc -Wall -Ofast -c lib/randauto.c

randauto-install:
	cp librandauto.so /usr/lib/librandauto.so
	cp randauto.h /usr/include/randauto.h

randbytes:
	gcc -Wall -Ofast -lrandauto -o randbytes randbytes.c

randbytes-local:
	gcc -Wall -Ofast randauto.o randauto.h -o randbytes randbytes.c

randbytes-install:
	cp randbytes /usr/bin/randbytes

all-local: randauto-local randbytes-local

clean:
	rm -f librandauto.so randauto.o randbytes &> /dev/null

