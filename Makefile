all:
	cd src && make
	cp src/main bin/

clean:
	cd src && make clean
	rm -f bin/main
