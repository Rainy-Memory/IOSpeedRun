top: run.c
	clang -I /usr/share/liburing/src/include -L /usr/lib/aarch64-linux-gnu -luring -lrt -laio -o run run.c
clean: 
	sudo rm *.txt
	sudo rm run