#================================[[ beg-code ]]================================#



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = khronos
# additional standard and outside libraries
LIB_STD    = 
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lySTR_debug -lyEXEC_debug -lySCHED_debug
# directory for production code, no trailing slash
INST_DIR   = /usr/sbin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
install_post       :
	ln --force --physical /usr/sbin/khronos  /usr/sbin/crond
	@sha1sum  /usr/sbin/crond
	ln --force --physical /usr/sbin/khronos  /usr/bin/crontab
	@sha1sum  /usr/bin/crontab

remove_post        :
	rm -f  /usr/sbin/crond
	rm -f  /usr/bin/crontab
	rm -f  /usr/share/man/man1/crond.1.bz2
	rm -f  /usr/share/man/man1/crontab.1.bz2



#================================[[ end-code ]]================================#

#*============================---(source-start)---============================*#

#*---(current variables)--------------*#
#BASE    = khronos
#DEBUG   = khronos_debug
#UNIT    = khronos_unit
#DIR     = /home/system/khronos.job_scheduling_daemon
#*---(compilier variables)------------*#
# must have "-x c" on gcc line so stripped files work with alternate extensions
#COMP    = gcc -c -std=gnu89 -x c -g -pg -Wall -Wextra
#INCS    = -I/usr/local/include 
#LINK    = gcc
#LIBDIR  = -L/usr/local/lib
#LIBS    = ${LIBDIR} -lySCHED       -lyEXEC       -lySTR
#LIBD    = ${LIBDIR} -lySCHED_debug -lyEXEC_debug -lySTR_debug  -lyLOG
#LIBU    = ${LIBDIR} -lySCHED_debug -lyEXEC_debug -lySTR_debug  -lyLOG -lyUNIT -lyVAR
#*---(file lists)---------------------*#
#HEADS   = ${BASE}.h
#OBJS    = ${BASE}_main.os ${BASE}_prog.os ${BASE}_base.os ${BASE}_list.os ${BASE}_tabs.os ${BASE}_test.os
#OBJD    = ${BASE}_main.o  ${BASE}_prog.o  ${BASE}_base.o  ${BASE}_list.o  ${BASE}_tabs.o  ${BASE}_test.o
#OBJU    = ${UNIT}.o       ${BASE}_prog.o  ${BASE}_base.o  ${BASE}_list.o  ${BASE}_tabs.o  ${BASE}_test.o
#*---(make variables)-----------------*#
#COPY    = cp -f
#CLEAN   = rm -f
#PRINT   = @printf
#STRIP   = @grep -v -e " DEBUG_" -e " yLOG_" 



#*---(executables)--------------------*#
#all                : ${BASE} ${UNIT}

#${BASE}            : ${OBJD}
#	${LINK}  -o ${BASE}        ${OBJS}   ${LIBS}
#	${LINK}  -o ${DEBUG}       ${OBJD}   ${LIBD}

#${UNIT}          : ${OBJU}
#	${LINK}  -o ${UNIT}        ${OBJU}   ${LIBU}



#*---(objects)------------------------*#
#${BASE}.o        : ${HEADS}       ${BASE}.c
#	${COMP}    ${BASE}_main.c                        ${INC}
#	${STRIP}   ${BASE}_main.c    > ${BASE}_main.cs
#	${COMP}    ${BASE}_main.cs  -o ${BASE}_main.os   ${INC}
#
#${BASE}_main.o   : ${HEADS}       ${BASE}_main.c
#	${COMP}    ${BASE}_main.c                        ${INC}
#	${STRIP}   ${BASE}_main.c    > ${BASE}_main.cs
#	${COMP}    ${BASE}_main.cs  -o ${BASE}_main.os   ${INC}
#
#${BASE}_prog.o     : ${HEADS}     ${BASE}_prog.c
#	${COMP}    ${BASE}_prog.c                        ${INC}
#	${STRIP}   ${BASE}_prog.c    > ${BASE}_prog.cs
#	${COMP}    ${BASE}_prog.cs  -o ${BASE}_prog.os   ${INC}
#
#${BASE}_base.o     : ${HEADS}     ${BASE}_base.c
#	${COMP}    ${BASE}_base.c                        ${INC}
#	${STRIP}   ${BASE}_base.c    > ${BASE}_base.cs
#	${COMP}    ${BASE}_base.cs  -o ${BASE}_base.os   ${INC}
#
#${BASE}_list.o     : ${HEADS}     ${BASE}_list.c
#	${COMP}    ${BASE}_list.c                        ${INC}
#	${STRIP}   ${BASE}_list.c    > ${BASE}_list.cs
#	${COMP}    ${BASE}_list.cs  -o ${BASE}_list.os   ${INC}
#
#${BASE}_tabs.o     : ${HEADS}     ${BASE}_tabs.c
#	${COMP}    ${BASE}_tabs.c                        ${INC}
#	${STRIP}   ${BASE}_tabs.c    > ${BASE}_tabs.cs
#	${COMP}    ${BASE}_tabs.cs  -o ${BASE}_tabs.os   ${INC}
#
#${BASE}_test.o     : ${HEADS}     ${BASE}_test.c
#	${COMP}    ${BASE}_test.c                        ${INC}
#	${STRIP}   ${BASE}_test.c    > ${BASE}_test.cs
#	${COMP}    ${BASE}_test.cs  -o ${BASE}_test.os   ${INC}
#
#${BASE}_unit.o   : ${BASE}.unit
##	uUNIT    ${BASE}
#	koios    ${BASE}
##	${COMP}  -x c ${BASE}_unit.code
##	mv ${BASE}_unit.code ${BASE}_unit.c
#	${COMP}  ${BASE}_unit.c



#*---(housecleaning)------------------*#
#clean              :
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "cleaning out local object, backup, and temp files\n"
#	${CLEAN} ${BASE}
#	${CLEAN} ${BASE}*.o
#	${CLEAN} ${BASE}*.cs
#	${CLEAN} ${BASE}*.os
#	${CLEAN} ${BASE}*.Sc
#	${CLEAN} ${BASE}*.So
#	${CLEAN} *~
#	${CLEAN} temp*
#	#${CLEAN} ${BASE}_unit
#	${CLEAN} ${BASE}_unit.c
#	${CLEAN} ${DEBUG}
#	#---(complete)------------------------#

#bigclean           :
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "clean out local swap files\n"
#	${CLEAN} .*.swp
	#---(complete)------------------------#

#remove             :
#	#---(all versions)--------------------#
#	${CLEAN}  /usr/bin/${BASE}
#	${CLEAN}  /usr/local/bin/${DEBUG}
#	${CLEAN}  /usr/bin/crond
#	${CLEAN}  /usr/bin/crontab
#	#---(documentation)-------------------#
#	rm -f     /usr/share/man/man1/${BASE}.1.bz2
#	rm -f     /usr/share/man/man1/${DEBUG}.1.bz2
#	rm -f     /usr/share/man/man1/crond.1.bz2
#	rm -f     /usr/share/man/man1/crontab.1.bz2
#	#---(complete)------------------------#

#install            : ${BASE}
#	#---(production version)--------------#
#	${COPY}   ${BASE}    /usr/bin/
#	chown     root:root  /usr/bin/${BASE}
#	chmod     0711       /usr/bin/${BASE}
#	chmod     +s         /usr/bin/${BASE}
#	@sha1sum  ${BASE}
#	#---(debug version)-------------------#
#	${COPY}  ${DEBUG}    /usr/local/bin/
#	chown     root:root  /usr/local/bin/${DEBUG}
#	chmod     0711       /usr/local/bin/${DEBUG}
#	chmod     +s         /usr/local/bin/${DEBUG}
#	#---(provide other links)-------------#
#	ln --force --physical /usr/bin/${BASE}  /usr/bin/crond
#	ln --force --physical /usr/bin/${BASE}  /usr/bin/crontab
#	#---(documentation)-------------------#
#	rm -f     /usr/share/man/man1/${BASE}.1.bz2
#	cp -f     ${BASE}.1    /usr/share/man/man1/
#	bzip2     /usr/share/man/man1/${BASE}.1
#	chmod     0644  /usr/share/man/man1/${BASE}.1.bz2
#	#---(provide other links)-------------#
#	ln --force --physical /usr/share/man/man1/${BASE}.1.bz2  /usr/share/man/man1/${DEBUG}.1.bz2
#	ln --force --physical /usr/share/man/man1/${BASE}.1.bz2  /usr/share/man/man1/crontab.1.bz2
#	ln --force --physical /usr/share/man/man1/${BASE}.1.bz2  /usr/share/man/man1/crontab.1.bz2
#	#---(complete)------------------------#

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
