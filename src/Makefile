CXXFLAGS = -Wall -Wno-sign-compare -g -std=c++0x

OBJECTS = Segment.o Instance.o Engine.o Statistics.o Path.o Connectivity.o Location.o ActivityManager.o PathData.o

default:	experiment verification

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test2: test2.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

assign2test: assign2test.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test-cases: test-cases.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

UnitedStatesCities: UnitedStatesCities.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

experiment:	experiment.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

verification: verification.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o test1 test2 assign2test test-cases example UnitedStatesCities experiment $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h
Engine.o: Engine.cpp Engine.h
Segment.o: Segment.cpp Segment.h
Location.o: Location.cpp Location.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
test2.o: test2.cpp
assign2test.o: assign2test.cpp
test-cases.o: test-cases.cpp
UnitedStatesCities.o: UnitedStatesCities.cpp
Statistics.o: Statistics.cpp Statistics.h
Path.o: Path.cpp Path.h
Connectivity.o: Connectivity.cpp Connectivity.h
ActivityManager.o: Activity.h ActivityImpl.h ActivityManager.h ActivityManager.cpp ActivityManagerImpl.h
PathData.o: PathData.h PathData.cpp
experiment.o: experiment.cpp
verification.o: verification.cpp
