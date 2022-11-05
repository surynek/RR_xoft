all: debug

debug:	motion_plan_main.o_dbg 
	g++ motion_plan_main.o_dbg -L../../src/core -L../../src/glucose -L../../src/common -L../../src/util -lCommon_rrOST_dbg -lUtil_rrOST_dbg -lCore_rrOST_dbg -lGlucose_rrOST_dbg -pthread -omotion_plan_rrOST

optimized:	motion_plan_main.o_opt 
	g++ motion_plan_main.o_opt -L../../src/core -L../../src/glucose -L../../src/common -L../../src/util -lCommon_rrOST_opt -lUtil_rrOST_opt -lCore_rrOST_opt -lGlucose_rrOST_opt -pthread -omotion_plan_rrOST

motion_plan_main.o_dbg:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h motion_plan_main.h motion_plan_main.cpp
	g++ -Wall -Wextra -pedantic -Wno-class-memaccess -Wno-long-long -Wno-unused-result -Wno-sign-compare -Wno-delete-non-virtual-dtor -g -std=c++0x -c -I../../src -I../../src/glucose -I../../src/core -I../../src/common -I../../src/util -omotion_plan_main.o_dbg motion_plan_main.cpp


motion_plan_main.o_opt:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h motion_plan_main.h motion_plan_main.cpp
	g++ -std=c++0x -c -w -O9 -mtune=native -I../../src -I../../src/glucose -I../../src/core -I../../src/common -I../../src/util -omotion_plan_main.o_opt motion_plan_main.cpp


clean:
	rm -f -r *~ *.o_dbg *.o_opt *.a *.bak output*.txt motion_plan_rrOST
