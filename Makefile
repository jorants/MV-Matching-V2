all:
	cd src && make
	mkdir -p bin
	cp src/main bin/

clean:
	cd src && make clean
	rm -f bin/main
