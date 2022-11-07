SRC=main.cpp src/Server.cpp src/Request.cpp src/RequestBacklog.cpp
OPTIONS=-pthread

main:
	rm ./target/main || true
	${CXX} -std=c++2a ${SRC} ${OPTIONS} -o ./target/main

run:
	./target/main

cleanRun:
	make main
	make run

debug:
	rm ./target/main || true
	${CXX} -std=c++2a ${SRC} ${OPTIONS} -g -o ./target/main
	gdb ./target/main
	