#co thay doi thi them so vao sau chu dict de tao cac file dict2 dict3 etc....
#tao file dict
CC=gcc
dict: main.c lib/bt-5.0.0/lib/libbt.a
	$(CC) -o dict main.c `pkg-config --libs --cflags gtk+-3.0` lib/bt-5.0.0/lib/libbt.a -rdynamic

#tao dict2
#CC=gcc
#dict2: main.c lib/bt-5.0.0/lib/libbt.a
#	$(CC) -o dict2 main.c `pkg-config --libs --cflags gtk+-3.0` lib/bt-5.0.0/lib/libbt.a -rdynamic


#tao dict3
#CC=gcc
#dict3: main.c lib/bt-5.0.0/lib/libbt.a
#	$(CC) -o dict3 main.c `pkg-config --libs --cflags gtk+-3.0` lib/bt-5.0.0/lib/libbt.a -rdynamic


#cach comment file make: dau "#" vai tro nhu "//" cua C