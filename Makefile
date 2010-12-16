#*============================---(source-start)---============================*#

#*---(current variables)--------------*#
BASE    = khronos
FRONT   = crontab
DIR     = /home/system/p_gvskav/khronos.heatherly_cron_daemon

#*---(standard variables)-------------*#
#COMP    = gcc -c -std=gnu89 -g -pg -Wall -Wextra
COMP    = tcc -c -g -Wall
INC     = 
#LINK    = gcc
LINK    = tcc
LIBS    = -L/usr/local/libs -lyLOG
LIBS_S  = 
COPY    = cp -f
CLEAN   = rm -f
ECHO    = @echo


tcc                : ${BASE}.h ${BASE}_main.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c ${BASE}.unit
	tcc -o   ${BASE}         ${BASE}_main.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c      ${LIBS}
#	uUNIT    ${BASE}
#	mv ${BASE}_unit.code ${BASE}_unit.c
#	tcc -o   ${BASE}_unit    ${BASE}_unit.c ${BASE}_test.c ${BASE}_tab.c ${BASE}_list.c ${BASE}.c      ${LIBS} -lyUNIT -lyVAR
#	tcc -o   ${FRONT}        ${FRONT}.c     ${BASE}_list.c ${BASE}.c                     ${LIBS}

#*---(MAIN)---------------------------*#
#all                : ${BASE} ${FRONT} ${BASE}_unit


#*---(executables)--------------------*#
#${BASE}          : ${BASE}_main.o ${BASE}_list.o ${BASE}.o
#	${LINK}  -o ${BASE}         ${BASE}_main.o ${BASE}_list.o ${BASE}.o ${LIBS}

#${BASE}_unit     : ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}.o 
#	${LINK}  -o ${BASE}_unit    ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}.o ${LIBS} -lyUNIT -lyVAR 

#${FRONT}         : ${FRONT}.o ${BASE}_list.o
#	${LINK}  -o ${FRONT}        ${FRONT}.o ${BASE}_list.o   ${BASE}.o   ${LIBS}


#*---(objects)------------------------*#
#${BASE}.o        : ${BASE}.h ${BASE}.c
#	${COMP}  ${BASE}.c ${INC}

#${BASE}_main.o   : ${BASE}.h ${BASE}_main.c
#	${COMP}  ${BASE}_main.c

#${BASE}_list.o   : ${BASE}.h ${BASE}_list.c
#	${COMP}  ${BASE}_list.c

#${BASE}_test.o   : ${BASE}.h ${BASE}_test.c
#	${COMP}  ${BASE}_test.c

#${FRONT}.o       : ${BASE}.h ${FRONT}.c
#	${COMP}  ${FRONT}.c ${INC}

#${BASE}_unit.o   : ${BASE}.unit
#	uUNIT    ${BASE}
#	${COMP}  -x c ${BASE}_unit.code
#	mv ${BASE}_unit.code ${BASE}_unit.c
#	${COMP}  ${BASE}_unit.c


#*---(housecleaning)------------------*#
clean              :
	${ECHO}  cleaning out object, backup, and temp files
	${CLEAN} *.o
	${CLEAN} *~
	${CLEAN} temp*
	${CLEAN} ${BASE}
	${CLEAN} ${FRONT}
	${CLEAN} ${BASE}_unit
	${CLEAN} ${BASE}_unit.c

remove             :
	${CLEAN} /usr/sbin/${BASE}
	${CLEAN} /usr/bin/${FRONT}


bigclean           :
	${CLEAN} .*.swp

install            : ${BASE}
	${ECHO}  installing in b_nvdo
	cp -f    ${BASE}    /usr/bin/
	chown    root:root  /usr/bin/${BASE}
	chmod    0711       /usr/bin/${BASE}
	chmod    +s         /usr/bin/${BASE}
	sha1sum  ${BASE}
	~/b_nvdo/call_graph

gotesting          : 
	#mount --bind /bin     ${DIR}/unit_root/bin
	#mount --bind /dev     ${DIR}/unit_root/dev
	#mount --bind /etc     ${DIR}/unit_root/etc
	#mount --bind /home    ${DIR}/unit_root/home
	#mount --bind /lib     ${DIR}/unit_root/lib
	#mount --bind /sbin    ${DIR}/unit_root/sbin
	#mount --bind /usr     ${DIR}/unit_root/usr

untesting          : 
	#mount | grep "${DIR}/unit_root/bin"  | wc -l
	#echo "rc=$?"
	#mount -f  ${DIR}/unit_root/bin   2> /dev/null ||  umount ${DIR}/unit_root/bin
	#mount -f  ${DIR}/unit_root/dev   ||  umount ${DIR}/unit_root/dev
	#mount -f  ${DIR}/unit_root/etc   ||  umount ${DIR}/unit_root/etc
	#mount -f  ${DIR}/unit_root/home  ||  umount ${DIR}/unit_root/home
	#mount -f  ${DIR}/unit_root/lib   ||  umount ${DIR}/unit_root/lib
	#mount -f  ${DIR}/unit_root/sbin  ||  umount ${DIR}/unit_root/sbin
	#mount -f  ${DIR}/unit_root/usr   ||  umount ${DIR}/unit_root/usr


#*============================----(source-end)----============================*#
