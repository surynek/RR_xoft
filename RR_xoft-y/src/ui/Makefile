all: debug

debug:	libUI_RR_dbg.a 

optimized:	libUI_RR_opt.a 

libUI_RR_dbg.a:	tui.o_dbg 
	ar r libUI_RR_dbg.a tui.o_dbg

tui.o_dbg:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/conio/conio.h tui.h tui.cpp
	g++ -Wall -Wextra -pedantic -Wno-class-memaccess -Wno-long-long -Wno-unused-result -Wno-sign-compare -Wno-delete-non-virtual-dtor -Wno-stringop-truncation -g -std=c++14 -c -I../../src -I../../src/conio -I../../src/ui -otui.o_dbg tui.cpp


libUI_RR_opt.a:	tui.o_opt 
	ar r libUI_RR_opt.a tui.o_opt

tui.o_opt:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/conio/conio.h tui.h tui.cpp
	g++ -std=c++0x -c -w -O9 -mtune=native -I../../src -I../../src/conio -I../../src/ui -otui.o_opt tui.cpp


clean:
	rm -f -r *~ *.o_dbg *.o_opt *.a *.bak output*.txt
