#*============================---(source-start)---============================*#

#*---(current variables)--------------*#
BASE    = cron
DAEMON  = crond
FRONT   = crontab

#*---(standard variables)-------------*#
COMP    = gcc -c -std=gnu89 -g -pg -Wall -Wextra
INC     = 
LINK    = gcc 
LIBS    = -lyLOG
LIBS_S  = 
COPY    = cp -f
CLEAN   = rm -f
ECHO    = @echo


#*---(MAIN)---------------------------*#
all                : ${DAEMON} ${FRONT} ${DAEMON}_unit


#*---(executables)--------------------*#
${DAEMON}          : ${DAEMON}_main.o ${DAEMON}.o
	${LINK}  -o ${DAEMON}       ${DAEMON}_main.o ${DAEMON}.o ${LIBS}

${FRONT}           : ${FRONT}.o ${DAEMON}.o
	${LINK}  -o ${FRONT}        ${FRONT}.o ${DAEMON}.o ${LIBS}

${DAEMON}_unit     : ${DAEMON}_unit.o ${DAEMON}.o 
	${LINK}  -o ${DAEMON}_unit  ${DAEMON}_unit.o ${DAEMON}.o ${LIBS} -lyUNIT -lyVAR 


#*---(objects)------------------------*#
${DAEMON}.o        : ${BASE}.h ${DAEMON}.c
	${COMP}  ${DAEMON}.c ${INC}

${DAEMON}_main.o   : ${BASE}.h ${DAEMON}_main.c
	${COMP}  ${DAEMON}_main.c

${FRONT}.o         : ${BASE}.h ${FRONT}.c
	${COMP}  ${FRONT}.c

${DAEMON}_unit.o   : ${DAEMON}.unit
	uUNIT    ${DAEMON}
	${COMP}  -x c ${DAEMON}_unit.code


#*---(housecleaning)------------------*#
clean              :
	${ECHO}  cleaning out object, backup, and temp files
	${CLEAN} *.o
	${CLEAN} *~
	${CLEAN} temp*

bigclean           :
	${CLEAN} .*.swp

install            : ${DAEMON}
	${ECHO}  installing in b_nvdo
	${COPY}  ${DAEMON}  /usr/sbin/
	chmod    0700       /usr/sbin/${DAEMON}
	chown    root:root  /usr/sbin/${DAEMON}
	${COPY}  ${FRONT}   /usr/bin/
	chmod    0711       /usr/sbin/${DAEMON}
	chown    root:users /usr/bin/${FRONT}
	chmod    +s         /usr/bin/${FRONT}



#*============================----(source-end)----============================*#
