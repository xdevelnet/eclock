all:
	@echo Use "make fcgi" command to compile as fastcgi program.
fcgi:
	cc --std=c99 main.c -o eclock -Wall -I/usr/local/include -I../cdfel/src/ -L/usr/local/lib -lfcgi
