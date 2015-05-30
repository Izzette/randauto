randauto:
	gcc -Wall -Ofast -shared -o randauto.so lib/randauto.c

randauto-local:
	gcc -Wall -Ofast -c lib/randauto.c

randauto-install:
	cp randauto.so /usr/lib/randauto.so
	cp randauto.h /usr/include/randauto.h

randbytes:
	gcc -Wall -Ofast -lgmp -lrandauto -o randbytes randbytes.c

randbytes-local:
	gcc -Wall -Ofast -lgmp randauto.o randauto.h -o randbytes randbytes.c

randbytes-install:
	cp randbytes /usr/bin/randbytes

all-local: randauto-local randbytes-local

all-install: randauto-install randbytes randbytes-install

clean:
	rm randauto.so randauto.o randbytes &> /dev/null

