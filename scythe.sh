#!/bin/dash
##  this script will be compatible with any POSIX shell, but tuned for dash


##===[[ START HDOC ]]==========================================================#

 #   focus         : (SA) system_admin
 #   niche         : (ba) batch_automation
 #   purpose       : wrapper to provide easy batch job execution monitoring

 #   greek_name    : scythe of khronos
 #   heritage      : protogenoi (first-born) god of creation and unyielding time
 #   imagery       : scythe is used end certain activities
 #
 #   base_system   : gnu/linux
 #   lang_name     : posix shell
 #   dependencies  : none
 #   size          : micro (less than 200 slocL)
 #
 #   author        : the_heatherlys
 #   created       : 2010-05
 #   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 #   end goal      : loosely coupled, strict interface, maintainable, portable
 #
 #
##===[[ PURPOSE ]]=============================================================#

 #   cronlog is a very simple posix shell script wrapper to enable basic
 #   logging of batch/background jobs in order to better understand their
 #   environment, performance, and results.  it is designed to take a messy,
 #   haphazard, tedious, and worrisome task and make it simple, direct,
 #   transparent, repeatable, and maintainable.
 #
 #   the most basic questions that tools such as crond raise are...
 #      - did the job run
 #      - is it still running
 #      - did it complete
 #      - how long did it take
 #      - did it finish successfully
 #      - etc.
 #
 #   cronlog will provide two entries in an central execution database for each
 #   job it monitors -- one logging the start, request, and environment and the
 #   second logging the completion, status, and duration.  the intent of the
 #   database is to be a simple, searchable file that can be used to understand,
 #   report, diagnose, and debug as required.
 #
 #   as always, many existing libraries and utilities have been built by better
 #   programmers which are likely superior in speed, size, capability, and
 #   reliability;  BUT, i would not have learned nearly as much using them, so
 #   i follow the adage...
 #
 #   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 #   seek what they sought ~ Matsuo Basho
 #
 #   dependable automation is the point and key of computers and if i can't
 #   come up with things to use it for, i am a useless moron with no point
 #
 #
##===[[ END HDOC ]]============================================================#


if [ -z $1 ]; then
   exit 99
fi

#---(define programs :: in case path is not set)-----------#
DATE="/bin/date"
BASENAME="/bin/basename"
HOSTNAME="/bin/hostname"
WHOAMI="/usr/bin/whoami"
WHICH="/usr/bin/which"
PRINTF="/usr/bin/printf"

#---(log start)--------------------------------------------#
all_output="/var/log/yLOG.historical/khronos.scythe_logging"

all_host="$($HOSTNAME)"
all_user="$($WHOAMI)"
all_pid="$$"
all_base="$($BASENAME $1)"
all_full="$($WHICH $all_base)"
all_cmd="$*"

beg_date="$($DATE +'%y.%m.%d.%H.%M.%S.%N')"
beg_time=$(printf "%13.13s" $($DATE +%s%N))

#---(run)--------------------------------------------------#
/bin/dash -c "$*"
end_rc="$?"

#---(report)-----------------------------------------------#
end_date="$($DATE +'%y.%m.%d.%H.%M.%S.%N')"
end_time=$($PRINTF "%13.13s" $($DATE +%s%N))
end_dur=$(( $end_time - $beg_time))
x_mil=$(( $end_dur % 1000          ))
x_sec=$(( $end_dur / 1000          ))
x_hrs=$(( $x_sec   / 3600           ))
x_sec=$(( $x_sec   - ($x_hrs * 3600) ))
x_min=$(( $x_sec   / 60             ))
x_sec=$(( $x_sec   - ($x_min * 60)   ))
end_elapse=$($PRINTF "%2dh, %2dm, %2ds, %3dms" "$x_hrs" "$x_min" "$x_sec" "$x_mil")

$PRINTF "%21.21s  %13d  %-15.15s  %-15.15s  %6d  %-15.15s  %-35.35s  %21.21s  %13d  %8d  %21.21s  %5d  %s \n" \
      "$beg_date" "$beg_time" \
      "$all_host" "$all_user" "$all_pid" "$all_base" "$all_full" \
      "$end_date" "$end_time" \
      "$end_dur"  "$end_elapse" "$end_rc" "$all_cmd"  >> "$all_output"

#---(return commands RC)-----------------------------------#
exit "$end_rc"

