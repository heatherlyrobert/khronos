#*============================---(source-start)---============================*#

#*---(current variables)--------------*#
BASE    = chronos
FRONT   = crontab
DIR     = /home/system/p_gvskav/chronos.heatherly_cron_daemon

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
all                : ${BASE} ${FRONT} ${BASE}_unit


#*---(executables)--------------------*#
${BASE}          : ${BASE}_main.o ${BASE}_list.o ${BASE}.o
	${LINK}  -o ${BASE}         ${BASE}_main.o ${BASE}_list.o ${BASE}.o ${LIBS}

${BASE}_unit     : ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}.o 
	${LINK}  -o ${BASE}_unit    ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}.o ${LIBS} -lyUNIT -lyVAR 

${FRONT}         : ${FRONT}.o ${BASE}_list.o
	${LINK}  -o ${FRONT}        ${FRONT}.o ${BASE}_list.o   ${BASE}.o   ${LIBS}


#*---(objects)------------------------*#
${BASE}.o        : ${BASE}.h ${BASE}.c
	${COMP}  ${BASE}.c ${INC}

${BASE}_main.o   : ${BASE}.h ${BASE}_main.c
	${COMP}  ${BASE}_main.c

${BASE}_list.o   : ${BASE}.h ${BASE}_list.c
	${COMP}  ${BASE}_list.c

${BASE}_test.o   : ${BASE}.h ${BASE}_test.c
	${COMP}  ${BASE}_test.c

${FRONT}.o       : ${BASE}.h ${FRONT}.c
	${COMP}  ${FRONT}.c ${INC}

${BASE}_unit.o   : ${BASE}.unit
	uUNIT    ${BASE}
	${COMP}  -x c ${BASE}_unit.code


#*---(housecleaning)------------------*#
clean              :
	${ECHO}  cleaning out object, backup, and temp files
	${CLEAN} *.o
	${CLEAN} *~
	${CLEAN} temp*
	${CLEAN} ${BASE}

bigclean           :
	${CLEAN} .*.swp

install            : ${BASE}
	${ECHO}  installing in b_nvdo
	cp -f    ${BASE}    /usr/sbin/
	chown    root:root  /usr/sbin/${BASE}
	chmod    0700       /usr/sbin/${BASE}
	sha1sum  ${BASE}
	cp -f    ${FRONT}    /usr/bin/
	chown    root:root  /usr/bin/${FRONT}
	chmod    0711       /usr/bin/${FRONT}
	chmod    +s         /usr/bin/${FRONT}
	sha1sum  ${FRONT}
	~/b_nvdo/call_graph

gotesting          : 
	mount --bind /bin     ${DIR}/unit_root/bin
	#mount --bind /dev     ${DIR}/unit_root/dev
	#mount --bind /etc     ${DIR}/unit_root/etc
	#mount --bind /home    ${DIR}/unit_root/home
	#mount --bind /lib     ${DIR}/unit_root/lib
	#mount --bind /sbin    ${DIR}/unit_root/sbin
	#mount --bind /usr     ${DIR}/unit_root/usr

untesting          : 
	mount | grep "${DIR}/unit_root/bin"  | wc -l
	echo "rc=$?"
	#mount -f  ${DIR}/unit_root/bin   2> /dev/null ||  umount ${DIR}/unit_root/bin
	#mount -f  ${DIR}/unit_root/dev   ||  umount ${DIR}/unit_root/dev
	#mount -f  ${DIR}/unit_root/etc   ||  umount ${DIR}/unit_root/etc
	#mount -f  ${DIR}/unit_root/home  ||  umount ${DIR}/unit_root/home
	#mount -f  ${DIR}/unit_root/lib   ||  umount ${DIR}/unit_root/lib
	#mount -f  ${DIR}/unit_root/sbin  ||  umount ${DIR}/unit_root/sbin
	#mount -f  ${DIR}/unit_root/usr   ||  umount ${DIR}/unit_root/usr


#*============================----(source-end)----============================*#
