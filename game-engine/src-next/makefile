CC = g++

CFLAGS = -Wall

MAIN_TEST = main.test.o
MAIN = main.o
COMPONENT = component/component.o
ENTITY    = entity/entity.o
MONITOR   = monitor/monitor.o
SYSTEM    = system/system.o system/systemManager.o

COMPONENT_DEPS = component/component.hpp component/components/health.hpp component/components/kick.hpp
ENTITY_DEPS    = entity/entity.hpp
MONITOR_DEPS   = monitor/monitor.hpp
SYSTEM_DEPS    = system/system.hpp system/systems/systemHealth.hpp system/systems/systemKick.hpp       system/systemManager.hpp

OBJS = ${COMPONENT}      ${ENTITY}      ${MONITOR}      ${SYSTEM}      
DEPS = ${COMPONENT_DEPS} ${ENTITY_DEPS} ${MONITOR_DEPS} ${SYSTEM_DEPS}

clear:
	bash -c "clear"

main: ${MAIN} ${DEPS}
	${CC} -o main ${MAIN} ${CFLAGS}

clean:
	rm -f ${OBJS} ${MAIN} ${MAIN_TEST}

all: clear main clean

test: ${MAIN_TEST} ${DEPS}
	${CC} -o test ${MAIN_TEST} ${CFLAGS}

all_test: clear test clean

.PHONY: clear clean main all

