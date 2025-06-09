bonus: bonus.o VocabAVL.o
	gcc -o bonus bonus.o VocabAVL.o

bonus.o: bonus.c VocabAVL.h
	gcc -c bonus.c

VocabAVL.o: VocabAVL.c VocabAVL.h
	gcc -c VocabAVL.c

clean:
	rm -f *.o bonus 