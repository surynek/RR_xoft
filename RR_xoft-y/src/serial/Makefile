all: debug

debug:	serial_test.cpp
	g++ -std=c++0x -w -g -mtune=native -oserial_test serial_test.cpp

optimized:	serial_test.cpp
		g++ -std=c++0x -w -O9 -mtune=native -oserial_test serial_test.cpp
clean:
	rm -f -r *~ *.o_dbg *.o_opt *.a *.bak output*.txt serial_test
