#*============================---(source-start)---============================*#

#*---(current variables)--------------*#
BASE    = khronos
MICRO   = ukhronos
DIR     = /home/system/p_gvskav/khronos.heatherly_cron_daemon

#*---(standard variables)-------------*#
COMP    = tcc -c -g -Wall
INC     = 
LINK    = tcc
LIBS    = -L/usr/local/libs -lyLOG
COPY    = cp -f
CLEAN   = rm -f
ECHO    = @echo


tcc                : ${BASE}.h ${BASE}_main.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c ${BASE}.unit
	# application
	tcc -o   ${BASE}         ${BASE}_main.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c      ${LIBS}
	# unit testing
	uUNIT    ${BASE}
	mv ${BASE}_unit.code ${BASE}_unit.c
	tcc -o   ${BASE}_unit    ${BASE}_unit.c ${BASE}_list.c ${BASE}_tab.c ${BASE}.c ${BASE}_test.c      ${LIBS} -lyUNIT -lyVAR

lean               : 
	./lean.awk ${BASE}.h       > ${MICRO}.h
	./lean.awk ${BASE}.c       > ${MICRO}.c
	./lean.awk ${BASE}_main.c  > ${MICRO}_main.c
	./lean.awk ${BASE}_list.c  > ${MICRO}_list.c
	./lean.awk ${BASE}_tab.c   > ${MICRO}_tab.c
	tcc -o     ${MICRO}         ${MICRO}_main.c ${MICRO}_list.c ${MICRO}_tab.c ${MICRO}.c


#*---(MAIN)---------------------------*#
#all                : ${BASE} ${BASE}_unit


#*---(executables)--------------------*#
#${BASE}          : ${BASE}_main.o ${BASE}_list.o ${BASE}.o
#	${LINK}  -o ${BASE}         ${BASE}_main.o ${BASE}_list.o ${BASE}.o ${LIBS}

#${BASE}_unit     : ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}.o 
#	${LINK}  -o ${BASE}_unit    ${BASE}_unit.o ${BASE}_test.o ${BASE}_list.o ${BASE}.o ${LIBS} -lyUNIT -lyVAR 


#*---(objects)------------------------*#
#${BASE}.o        : ${BASE}.h ${BASE}.c
#	${COMP}  ${BASE}.c ${INC}

#${BASE}_main.o   : ${BASE}.h ${BASE}_main.c
#	${COMP}  ${BASE}_main.c

#${BASE}_list.o   : ${BASE}.h ${BASE}_list.c
#	${COMP}  ${BASE}_list.c

#${BASE}_test.o   : ${BASE}.h ${BASE}_test.c
#	${COMP}  ${BASE}_test.c


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
	${CLEAN} ${BASE}_unit
	${CLEAN} ${BASE}_unit.c

remove             :
	${CLEAN} /usr/sbin/${BASE}


bigclean           :
	${CLEAN} .*.swp

install            : ${BASE}
	${ECHO}  installing in b_nvdo
	# application
	cp -f    ${BASE}    /usr/bin/
	chown    root:root  /usr/bin/${BASE}
	chmod    0711       /usr/bin/${BASE}
	chmod    +s         /usr/bin/${BASE}
	sha1sum  ${BASE}
	# call graph
	~/b_nvdo/call_graph
	# documentation
	rm -f       /usr/share/man/man1/${BASE}.1.bz2
	cp -f       ${BASE}.1    /usr/share/man/man1/
	bzip2       /usr/share/man/man1/${BASE}.1
	chmod 0644  /usr/share/man/man1/${BASE}.1.bz2

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
