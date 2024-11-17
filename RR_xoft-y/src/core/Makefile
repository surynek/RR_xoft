all: debug

debug:	libCore_RR_dbg.a 

optimized:	libCore_RR_opt.a 

libCore_RR_dbg.a:	robot.o_dbg 
	ar r libCore_RR_dbg.a robot.o_dbg

robot.o_dbg:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h robot.h robot.cpp
	g++ -Wall -Wextra -pedantic -Wno-class-memaccess -Wno-long-long -Wno-unused-result -Wno-sign-compare -Wno-delete-non-virtual-dtor -Wno-stringop-truncation -g -std=c++14 -c -I../../src -I../../src/core -orobot.o_dbg robot.cpp


libCore_RR_opt.a:	robot.o_opt 
	ar r libCore_RR_opt.a robot.o_opt

robot.o_opt:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h robot.h robot.cpp
	g++ -std=c++0x -c -w -O9 -mtune=native -I../../src -I../../src/core -orobot.o_opt robot.cpp


clean:
	rm -f -r *~ *.o_dbg *.o_opt *.a *.bak output*.txt
