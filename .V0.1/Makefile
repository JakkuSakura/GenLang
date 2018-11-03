all: genlang

genlang: scanner parser
	g++ src/*.cc src/*.cpp -o bin/genlang -Iincludes -O2

scanner:
	flex -o src/genlang.scanner.cc src/lexyacc/genlang.l

parser:
	bison -d -o src/genlang.parser.cc src/lexyacc/genlang.y
	mv src/genlang.parser.hh includes/genlang.parser.hh

.IGNORE: clean

clean:
	rm bin/genlang src/genlang.parser.cc src/genlang.scanner.cc includes/genlang.parser.hh
	rm -r ./export

test: genlang
	cat examples/example.txt | bin/genlang

export: clean genlang
	mkdir ./export
	cp -r ./bin/* ./gstl ./export/
