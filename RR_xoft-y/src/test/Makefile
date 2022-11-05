all: debug

debug:	basic_test.o_dbg 
	g++ basic_test.o_dbg -L../../src/common -L../../src/glucose -L../../src/core -L../../src/util -lUtil_rrOST_dbg -lCore_rrOST_dbg -lCommon_rrOST_dbg -lGlucose_rrOST_dbg -pthread -otest_basic_rrOST

optimized:	basic_test.o_opt 
	g++ basic_test.o_opt -L../../src/common -L../../src/glucose -L../../src/core -L../../src/util -lUtil_rrOST_opt -lCore_rrOST_opt -lCommon_rrOST_opt -lGlucose_rrOST_opt -pthread -otest_basic_rrOST

basic_test.o_dbg:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h basic_test.h basic_test.cpp
	g++ -Wall -Wextra -pedantic -Wno-class-memaccess -Wno-long-long -Wno-unused-result -Wno-sign-compare -Wno-delete-non-virtual-dtor -g -std=c++0x -c -I../../src -I../../src/glucose -I../../src/common -I../../src/core -I../../src/util -obasic_test.o_dbg basic_test.cpp


basic_test.o_opt:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h basic_test.h basic_test.cpp
	g++ -std=c++0x -c -w -O9 -mtune=native -I../../src -I../../src/glucose -I../../src/common -I../../src/core -I../../src/util -obasic_test.o_opt basic_test.cpp


clean:
	rm -f -r *~ *.o_dbg *.o_opt *.a *.bak output*.txt test_basic_rrOST
