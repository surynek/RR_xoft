all: debug

debug:	motion_plan_main.o_dbg control_panel_main.o_dbg 
	g++ motion_plan_main.o_dbg -L../../src/glucose -L../../src/core -L../../src/util -L../../src/common -lCommon_RR_dbg -lUtil_RR_dbg -lCore_RR_dbg -lGlucose_RR_dbg -pthread -omotion_plan_RR
	g++ control_panel_main.o_dbg -L../../src/glucose -L../../src/core -L../../src/ui -L../../src/util -L../../src/common -lCommon_RR_dbg -lUtil_RR_dbg -lCore_RR_dbg -lUI_RR_dbg -lGlucose_RR_dbg -pthread -ocontrol_panel_RR

optimized:	motion_plan_main.o_opt control_panel_main.o_opt 
	g++ motion_plan_main.o_opt -L../../src/glucose -L../../src/core -L../../src/util -L../../src/common -lCommon_RR_opt -lUtil_RR_opt -lCore_RR_opt -lGlucose_RR_opt -pthread -omotion_plan_RR
	g++ control_panel_main.o_opt -L../../src/glucose -L../../src/core -L../../src/ui -L../../src/util -L../../src/common -lCommon_RR_opt -lUtil_RR_opt -lCore_RR_opt -lUI_RR_opt -lGlucose_RR_opt -pthread -ocontrol_panel_RR

motion_plan_main.o_dbg:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h motion_plan_main.h motion_plan_main.cpp
	g++ -Wall -Wextra -pedantic -Wno-class-memaccess -Wno-long-long -Wno-unused-result -Wno-sign-compare -Wno-delete-non-virtual-dtor -Wno-stringop-truncation -g -std=c++14 -c -I../../src/glucose -I../../src -I../../src/core -I../../src/util -I../../src/common -omotion_plan_main.o_dbg motion_plan_main.cpp

control_panel_main.o_dbg:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/conio/conio.h ../../src/ui/tui.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h control_panel_main.h control_panel_main.cpp
	g++ -Wall -Wextra -pedantic -Wno-class-memaccess -Wno-long-long -Wno-unused-result -Wno-sign-compare -Wno-delete-non-virtual-dtor -Wno-stringop-truncation -g -std=c++14 -c -I../../src/glucose -I../../src -I../../src/core -I../../src/ui -I../../src/conio -I../../src/util -I../../src/common -ocontrol_panel_main.o_dbg control_panel_main.cpp


motion_plan_main.o_opt:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h motion_plan_main.h motion_plan_main.cpp
	g++ -std=c++0x -c -w -O9 -mtune=native -I../../src/glucose -I../../src -I../../src/core -I../../src/util -I../../src/common -omotion_plan_main.o_opt motion_plan_main.cpp

control_panel_main.o_opt:	../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/common/types.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/util/io.h ../../src/util/geometry.h ../../src/util/statistics.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/core/robot.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h ../../src/conio/conio.h ../../src/ui/tui.h ../../src/glucose/Alg.h ../../src/glucose/Alloc.h ../../src/glucose/BoundedQueue.h ../../src/glucose/Constants.h ../../src/glucose/Dimacs.h ../../src/glucose/Heap.h ../../src/glucose/IntTypes.h ../../src/glucose/Map.h ../../src/glucose/Options.h ../../src/glucose/ParseUtils.h ../../src/glucose/Queue.h ../../src/glucose/Solver.h ../../src/glucose/SolverTypes.h ../../src/glucose/Sort.h ../../src/glucose/System.h ../../src/glucose/Vec.h ../../src/glucose/XAlloc.h ../../src/version.h ../../src/defs.h ../../src/config.h ../../src/compile.h ../../src/result.h control_panel_main.h control_panel_main.cpp
	g++ -std=c++0x -c -w -O9 -mtune=native -I../../src/glucose -I../../src -I../../src/core -I../../src/ui -I../../src/conio -I../../src/util -I../../src/common -ocontrol_panel_main.o_opt control_panel_main.cpp


clean:
	rm -f -r *~ *.o_dbg *.o_opt *.a *.bak output*.txt motion_plan_RR control_panel_RR
