CXX = g++
CXXFLAGS = -Wall -pthread -std=c++11 -g

OBJS = main.o shared_datastruc.o powertrain_producer.o powertrain_consumer.o powered_chassis.o log.o

all: carassemble

carassemble: $(OBJS)
	$(CXX) $(CXXFLAGS) -o carassemble $(OBJS)

main.o: main.cpp shared_datastruc.h powertrain_producer.h powertrain_consumer.h powered_chassis.h car_assemble.h log.h
	$(CXX) $(CXXFLAGS) -c main.cpp

shared_datastruc.o: shared_datastruc.cpp shared_datastruc.h car_assemble.h
	$(CXX) $(CXXFLAGS) -c shared_datastruc.cpp

powertrain_producer.o: powertrain_producer.cpp powertrain_producer.h shared_datastruc.h car_assemble.h log.h
	$(CXX) $(CXXFLAGS) -c powertrain_producer.cpp

powertrain_consumer.o: powertrain_consumer.cpp powertrain_consumer.h shared_datastruc.h car_assemble.h log.h
	$(CXX) $(CXXFLAGS) -c powertrain_consumer.cpp

powered_chassis.o: powered_chassis.cpp powered_chassis.h shared_datastruc.h car_assemble.h log.h
	$(CXX) $(CXXFLAGS) -c powered_chassis.cpp

log.o: log.cpp log.h
	$(CXX) $(CXXFLAGS) -c log.cpp

clean:
	rm -f *.o carassemble
