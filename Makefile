
CFLAGS=-pg -g -O0
#CFLAGS=-O3

all: unidec gtkunidec
	
unidec: universal-decoder.c decodefunc.c filefunc.c
	$(CC) $(CFLAGS) -o unidec universal-decoder.c

gtkunidec: gtk-decoder.c decodefunc.c filefunc.c
	$(CC) gtk-decoder.c `pkg-config --libs --cflags gtk+-3.0` -o gtkunidec

clean: cleantest
	$(RM) unidec
	$(RM) gtkunidec

test: testu
	
testu: unidec
	./unidec -a 3 10 -a 4 3 -a 5 4 -a 6 11 -a 7 5 -a 8 9 -a 9 6 -a 10 8 -a 11 7 \
		 -d 0 3 -d 1 2 -d 2 4 -d 3 1 -d 4 5 -d 5 0 \
			-o capture-decoded.bin -i ./testroms/Capture_II_1986.raw
	diff -s capture-decoded.bin ./testroms/Capture_II_1986.bin

	./unidec -d 1 2 -d 2 1 -o stardos-decoded.bin -i ./testroms/StarDosCartRomV1-4.bin
	diff -s stardos-decoded.bin ./testroms/StarDosCartRomV1-4-decoded.bin
	./unidec -a 10 11 -a 11 10 -o stardos1541-decoded.bin -i ./testroms/stardos1541romv1-4.bin
	diff -s stardos1541-decoded.bin ./testroms/stardos1541romv1-4-decoded.bin

	./unidec -a 8 12 -a 9 10 -a 10 11 -a 11 9 -a 12 8 \
		 -d 0 2 -d 2 3 -d 3 0 -d 4 5 -d 5 7 -d 6 4 -d 7 6 \
			-o printing-modul-decoded.bin -i ./testroms/printing-modul.bin
	diff -s printing-modul-decoded.bin ./testroms/printing-modul-decoded.bin

	./unidec -a 2 10 -a 3 2 -a 4 3 -a 5 11 -a 6 4 -a 7 9 -a 8 5 -a 9 8 -a 10 6 -a 11 7 -a 12 13 -a 13 12 \
		 -d 0 3 -d 1 2 -d 2 4 -d 3 1 -d 4 0 \
			-o warpspeed128-decoded.bin -i ./testroms/C128WarpSpeedV2-1987.raw
	diff -s warpspeed128-decoded.bin ./testroms/C128WarpSpeedV2-1987.bin

	./unidec --verbose -a 0 10 -a 1 11 -a 2 9 -a 3 8 -a 4 0 -a 5 1 -a 6 2 -a 7 3 -a 8 4 -a 9 5 -a 10 6 -a 11 7 -a 12 12 \
			-a 13 13 -a 14 14 -a 15 15 -a 16 16 \
			-d 0 3 -d 1 2 -d 2 1 -d 3 0 -d 4 4 -d 5 5 -d 6 6 -d 7 7 \
		-o hugo-decoded.bin -i ./testroms/hugo.bin
	diff -s hugo-decoded.bin ./testroms/hugo-decoded.bin

	./unidec --verbose -a 0 8 -a 1 9 -a 2 13 -a 3 11 -a 4 10 -a 5 7 -a 7 5 -a 8 4 -a 9 0 -a 10 1 -a 11 2 -a 12 3 -a 13 12 \
		-o whizrom-decoded.bin -i ./testroms/whizrom.bin

	make cleantest

testwhiz: gtkunidec
	./gtkunidec -a 0 8 -a 1 9 -a 2 13 -a 3 11 -a 4 10 -a 5 7 -a 7 5 -a 8 4 -a 9 0 -a 10 1 -a 11 2 -a 12 3 -a 13 12 \
		-o whizrom-decoded.bin -i ./testroms/whizrom.bin

	make cleantest

# use the largest rom that we have for testing gtk ui
testgtk: gtkunidec
	./gtkunidec --verbose -a 0 10 -a 1 11 -a 2 9 -a 3 8 -a 4 0 -a 5 1 -a 6 2 -a 7 3 -a 8 4 -a 9 5 -a 10 6 -a 11 7 -a 12 12 \
			-a 13 13 -a 14 14 -a 15 15 -a 16 16 \
			-d 0 3 -d 1 2 -d 2 1 -d 3 0 -d 4 4 -d 5 5 -d 6 6 -d 7 7 \
		-o hugo-decoded.bin -i ./testroms/hugo.bin
	make cleantest

cleantest:
	$(RM) capture-decoded.bin
	$(RM) stardos-decoded.bin
	$(RM) stardos1541-decoded.bin
	$(RM) printing-modul-decoded.bin
	$(RM) warpspeed128-decoded.bin
	$(RM) hugo-decoded.bin
