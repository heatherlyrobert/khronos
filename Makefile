#================================[[ beg-code ]]================================#



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = khronos
# additional standard and outside libraries
LIB_STD    = 
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lyDLST_debug -lyPARSE_debug -lySTR_debug -lyEXEC_debug -lySCHED_debug -lyREGEX_debug
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
	chmod u+s /usr/sbin/khronos
	ln --force --physical /usr/sbin/khronos  /usr/sbin/crond
	@sha1sum  /usr/sbin/crond
	ln --force --physical /usr/sbin/khronos  /usr/bin/crontab
	@sha1sum  /usr/bin/crontab
	cp scythe.sh /usr/sbin/scythe
	chown root   /usr/sbin/scythe
	chmod 0755   /usr/sbin/scythe

remove_post        :
	rm -f  /usr/sbin/crond
	rm -f  /usr/bin/crontab
	rm -f  /usr/share/man/man1/crond.1.bz2
	rm -f  /usr/share/man/man1/crontab.1.bz2



#================================[[ end-code ]]================================#
