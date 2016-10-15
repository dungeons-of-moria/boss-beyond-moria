boss: boss.pas inc/*.inc
	fpc boss.pas -O3 -gl -XS
clean:
	rm -f boss boss.o
