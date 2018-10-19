all: genlang

genlang: scanner parser
	g++ src/*.cc src/*.cpp -o genlang -Iincludes -g

scanner:
	flex -o src/genlang.scanner.cc lexyacc/genlang.l

parser:
	bison -d -o src/genlang.parser.cc lexyacc/genlang.y
	mv src/genlang.parser.hh includes/genlang.parser.hh

clean:
	rm genlang src/genlang.parser.cc src/genlang.scanner.cc includes/genlang.parser.hh

test: genlang
	cat example.txt | ./genlang
