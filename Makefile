#*============================---(source-start)---============================*#

#*---(current variables)--------------*#
BASE    = khronos
DIR     = /home/system/p_gvskav/khronos.heatherly_cron_daemon

#*---(standard variables)-------------*#
#COMP    = tcc -c -g -Wall
COMP    = gcc -c -g -std=gnu89 -Wall
INC     = 
#LINK    = tcc
LINK    = gcc
LIBS    = -L/usr/local/libs -lyLOG -lySCHED
COPY    = cp -f
CLEAN   = rm -f
PRINT   = @printf


tcc                : ${BASE}.h ${BASE}_main.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c ${BASE}.unit
	# application
	tcc -o   ${BASE}         ${BASE}_main.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c      ${LIBS}
	# unit testing
#	uUNIT    ${BASE}
#	mv ${BASE}_unit.code ${BASE}_unit.c
#	tcc -o   ${BASE}_unit    ${BASE}_unit.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c ${BASE}_test.c      ${LIBS} -lyUNIT -lyVAR



#*---(MAIN)---------------------------*#
#all                : ${BASE} ${BASE}_unit


#*---(executables)--------------------*#
#${BASE}          : ${BASE}_main.o ${BASE}_list.o ${BASE}_tab.o ${BASE}.o
#	${LINK}  -o ${BASE}         ${BASE}_main.o ${BASE}_list.o ${BASE}_tab.o ${BASE}.o ${LIBS}

#${BASE}_unit     : ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}_tab.o ${BASE}.o 
#	${LINK}  -o ${BASE}_unit    ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}_tab.o ${BASE}.o ${LIBS} -lyUNIT -lyVAR 


#*---(objects)------------------------*#
#${BASE}.o        : ${BASE}.h ${BASE}.c
#	${COMP}  ${BASE}.c ${INC}

#${BASE}_main.o   : ${BASE}.h ${BASE}_main.c
#	${COMP}  ${BASE}_main.c

#${BASE}_list.o   : ${BASE}.h ${BASE}_list.c
#	${COMP}  ${BASE}_list.c

#${BASE}_tab.o    : ${BASE}.h ${BASE}_tab.c
#	${COMP}  ${BASE}_tab.c

#${BASE}_test.o   : ${BASE}.h ${BASE}_test.c
#	${COMP}  ${BASE}_test.c


#${BASE}_unit.o   : ${BASE}.unit
#	uUNIT    ${BASE}
#	${COMP}  -x c ${BASE}_unit.code
#	mv ${BASE}_unit.code ${BASE}_unit.c
#	${COMP}  ${BASE}_unit.c


#*---(housecleaning)------------------*#
clean              :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "cleaning out local object, backup, and temp files\n"
	${CLEAN} *.o
	${CLEAN} *~
	${CLEAN} temp*
	${CLEAN} ${BASE}
	${CLEAN} ${BASE}_unit
	${CLEAN} ${BASE}_unit.c

bigclean           :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "clean out local swap files\n"
	${CLEAN} .*.swp

install            : ${BASE}
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "install ${BASE} into production\n"
	# application
	cp -f     ${BASE}    /usr/bin/
	chown     root:root  /usr/bin/${BASE}
	chmod     0711       /usr/bin/${BASE}
	chmod     +s         /usr/bin/${BASE}
	@sha1sum  ${BASE}
	call_graph
	# documentation
	rm -f       /usr/share/man/man1/${BASE}.1.bz2
	cp -f       ${BASE}.1    /usr/share/man/man1/
	bzip2       /usr/share/man/man1/${BASE}.1
	chmod 0644  /usr/share/man/man1/${BASE}.1.bz2

remove             :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "remove ${BASE} from production\n"
	${CLEAN} /usr/sbin/${BASE}


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
