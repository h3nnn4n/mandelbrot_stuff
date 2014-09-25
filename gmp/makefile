all:
	gcc mandel.c -o mandel -lm -mtune=native -march=native -Ofast -lgmp

icc:
	icc mandel.c -o mandel -mtune=native -march=native -ip -ipo -parallel -Ofast -lgmp

clean:
	-rm -rvf core mandel
