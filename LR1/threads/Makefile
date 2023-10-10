TARGET = thread-example
DQ1 = Q1
DQ2 = Q2
DQ3 = Q3
DQ4 = Q4

SRCS = thread.c

CC=gcc
RM=rm
CFLAGS= -g -Wall
LIBS=-lpthread
INCLUDE_DIR="."

all: Q1 Q2 Q3 Q4

def: ${TARGET}

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS} ${LIBS} -o ${TARGET}

Q1: ${DQ1}

${DQ1}: ${SRCS}
	${CC} ${CFLAGS} -DATASK -I${INCLUDE_DIR} ${SRCS} ${LIBS} -o ${DQ1}

Q2: ${DQ2}

${DQ2}: ${SRCS}
	${CC} ${CFLAGS} -DBTASK -I${INCLUDE_DIR} ${SRCS} ${LIBS} -o ${DQ2}

Q3: ${DQ3}

${DQ3}: ${SRCS}
	${CC} ${CFLAGS} -DBTASK -DCTASK -I${INCLUDE_DIR} ${SRCS} ${LIBS} -o ${DQ3}

Q4: ${DQ4}

${DQ4}: ${SRCS}
	${CC} ${CFLAGS} -DBTASK -DCTASK -DDTASK -I${INCLUDE_DIR} ${SRCS} ${LIBS} -o ${DQ4}

clean:
	${RM} -f *o ${TARGET} Q1 Q2 Q3 Q4