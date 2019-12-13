all:
		gcc -o token `pkg-config --cflags --libs glib-2.0` token.c `pkg-config --libs glib-2.0` -I../include ../lib/libstemmer.o 
		gcc -o predictor `pkg-config --cflags --libs glib-2.0` predictor.c `pkg-config --libs glib-2.0` -lm

clean:
		rm -f predictor
