prog4: prog4.o VocabBST.o
	gcc -o prog4 prog4.o VocabBST.o

prog4.o: prog4.c VocabBST.h
	gcc -c prog4.c

VocabBST.o: VocabBST.c VocabBST.h
	gcc -c VocabBST.c

clean:
	rm -f *.o prog4
