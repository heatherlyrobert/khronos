/*=============================[[ beg-of-code ]]==============================*/

/*===[[ HEADER ]]=============================================================#

 *   focus         : (SA) system_admin
 *   niche         : (js) job_scheduler
 *   heritage      : khronos-ageraton  (protogenoi of creation and unyielding time)
 *   imagery       : winged serpent (drakon) with three heads -- bull, lion, man
 *   purpose       : provide consistent, reliable, time-based job scheduling
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : ansi-c      (righteous, limitless, universal, and forever)
 *   dependencies  : ySCHED* (must), yLOG+ (optional), dash (shell)
 *   size          : moderate    (less than 5,000 slocL)
 * 
 *   author        : the_heatherlys
 *   created       : 2010-05
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 *
 *   simplicity is prerequisite for reliability and security, but logging
 *   and unit testing can not be neglected
 * 
 */
/*===[[ SUMMARY ]]=============================================================*

 *   khronos is a fast, simplified, modernized, and technical version of the
 *   classic posix-defined crond time-based process scheduler which merges crond
 *   and crontab to allow deeper verification, verbosity, and traceablity.
 *
 */
/*===[[ PURPOSE ]]=============================================================*

 *   crond is a well-worn, familiar, time-tested, and posix-defined daemon that
 *   provides time-based job scheduling.  while this is just one aspect of batch
 *   automation, it is the most commonly used and crond does it exceptionally
 *   well.  many attempts have been made to "do it better", but its still here.
 *
 *   brian kernighan nailed it in the original (1979) by focusing on a real and
 *   definite need by folks that understood automation on low power systems.  he
 *   coupled it with an elegant, expressive, and terse scheduling grammar built
 *   by computer scientists and kept it simple and robust.  hugely brilliant.
 *
 *   but, there are now many implementations of crond screaming out for use,
 *   each of which comes with its own set of creeping featurism, cruft, and
 *   accessibility "improvements."  they all just tend to piss me off as
 *   everything seems to be trending towards the closely coupled, gui-focused,
 *   kitchen-sink mentality that then tends to display well but never gets used.
 *   i don't care if its easy for an idiot to use -- they don't use it anyway ;)
 *
 *   the one thing the advancements, since the original, truly improved were the
 *   algorithms.  the original cron apparently could not scale to a large
 *   number of users, so not that helpful for them.  in our case, we won't have
 *   a ton of different users, but we will have tons of jobs running -- we want
 *   the better algorithms to make up for the primitiveness of our coding ;)
 *
 *   khronos will attempt to implement the original simplicity, clarity, and
 *   power with updated algorithms, automated testing, strong logging and
 *   monitoring, stronger security, and a few added recovery/notification
 *   features.  we will maintain some backward compatibility while focusing on
 *   an automation-intensive, power-user environment for our own personal use.
 *
 *   "do one thing and do it well (securely) and focus on technical power users"
 *
 *   khronos will provide...
 *      - strict glibc/ansi-c so it can be ported to and compilied on anything
 *      - fast and efficient because we want to enable tons of automation
 *      - additional, specific job monitoring and recovery features
 *      - solid, verbose logging and status updates so you never have to guess
 *      - near posix compatibility so it can do the full job on core features
 *      - clean, clear code so we can maintain it after long absences
 *      - fullsome unit testing and regression testing suite
 *      - eliminate known and potential security gaps and hacking vectors
 *      - ability to test/verify crontabs before actual installation
 *
 *   khronos' crontab format will...
 *      - utilize the elegant core scheduling grammar as the original design
 *      - allow both original and the modified heatherly format
 *      - will be 98% backward compatible on the first five fields
 *      - but will use "and" to combine day of month/week (cleaner and stronger)
 *      - add a week field (6) for week-of-year (alternating, etc)
 *      - add a duration field (7) to allow khronos to know max run time
 *      - will add a year column, as admins have trouble keeping long-term notes
 *      - add a title fields for human-readable status reporting
 *      - the command/script field remains last and is now in the tenth position
 *      - add shortcuts for days of week, weekends, etc to increase readability
 *      - add duration expectations to control bad behaviour (min/max/exp)
 *      - add importance to decide what to run in recovery situations
 *
 *   khronos will not provide...
 *      - automatic email -- everyone ultimately hates it (security risk)
 *      - alternate shells (we're gonna run pure posix dash)
 *      - extended shell variables (gonna have a spartan shell environment)
 *      - run-time configuration (no, its only for us, we can update code)
 *      - special symbols for easily expressible things (@hourly, @weekly)
 *
 *   khronos will break backward compatiblilty in the following areas...
 *      - crontabs format will be expanded after the scheduling grammar part
 *      - crontabs will be stored in /var/spool/crontabs (not a big deal)
 *      - crontab names will allow for many files per user (like dcron)
 *      - crontabs will only be pulled from ~/c_quani/crontabs (std for us)
 *      - will not allow -c option as it is a security nightmare (no way)
 *      - will not allow -e option as it is a traceablilty nightmare (no loss)
 *      - will not allow - option as it is even worse than -e (who cares)
 *      - no mail at all (we will use other features to do it right)
 *
 *   on a large scale, khronos will not provide the other parts of batch work...
 *      - dependency-based scheduling (like init systems provide)
 *      - event-based launches like @reboot (daemons can and should do this)
 *      - resource-based changes to schedules, such as system load or avail
 *
 *   as a result of our changes, khronos is not a novice user tool, but neither
 *   was cron as admins can not allow crazy user processes to run all night.
 *   does anyone want users or installers to make decisions about what and when
 *   to run jobs on you system ?!#@!  really!!!
 *
 *   we don't want to just use the system, we want to *own* it; so that means
 *   we have to fundmentally understand the critical services which only tearing
 *   down and rebuilding can really teach -- even if its more basic in the end.
 *
 *   as luck would have it, dcron (dillon's cron) is a wonderfully straight
 *   forward and clean implementation that we can learn from.  it is licenced
 *   under the gnu gpl and so is perfect as our base.  so, we study dcron...
 *
 *   so, as always, there are many stable, accepted, existing programs and
 *   utilities written by better programmers which are likely superior in
 *   speed, size, capability, and reliability; BUT, i would not have learned
 *   nearly as much just using them, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   at the end of the day, this is wicked critical and deserves the attention.
 *   it is also full of learning opportunities ;)
 *
 */
/*===[[ ALTERNATIVES and COMPONENTS ]]=========================================*

 *   this is a survey of current cron implementations and related tools so we
 *   can get a good sense of what's out there, what it can do, and what problems
 *   they are trying to solve...
 *
 *    sys-process/vixie-cron (57k)
 *       - "standard" cron system for linux distributions (vixie = paul vixie)
 *       - stable and reliable
 *
 *    sys-process/anacron (23k)
 *       - supplements cron to deal with variable uptime, like laptops
 *       - is run daily and looks for jobs for that day or that haven't run yet
 *       - helps with housekeeping, but doesn't schedule tighter than days
 *       - INTERESTING IDEA, we'll see about some features
 *
 *    sys-process/bcron (56k)
 *       - security focused version (b = bruce guetner)
 *       - JUST TIGHTEN THE CRON SYSTEM AS IT IS?
 *
 *    sys-process/cronie (196k)
 *       - slang for follower or yes-man
 *       - very close to the original but with PAM and SELinux enhancements
 *       - I WILL NOT GET CAUGHT IN THE PAM/SELINUX TRAP
 *
 *    kde-base/kcron (1215k)
 *       - kde's task scheduler that runs on top of cron (WTF)
 *       - can set environment variables and jobs
 *       - MASSIVE PIG AT OVER A MEG !!!! plus needs qt and kde !!!!
 *
 *    sys-process/dcron (21k)
 *       - clean, lean cron version (d = matt dillon)
 *       - great base for our code
 *
 *    sys-process/mcron (???)
 *       - written in guile/scheme craziness (m = dale mellor)
 *       - YOU GOTTA BE KIDDING ME, SCHEME ?
 *
 *    sys-process/fcron (539k)
 *       - adds "nice" settings on the job (TOO COMPLEX)
 *       - can run a job once between X and Y time (BUT HOW TO DECIDE WHEN)
 *       - can take system load average into account (not allowed over NN%)
 *       - lots of environment variable setting features (NO, NO, NO)
 *       - "@" to indicate an interval or time relative to fcron's startup (NO)
 *       - "%" to indicate "hourly", "daily", "monthly", etc (MAYBE SHORTCUTS)
 *       - '!' to set a variable like niceness, mailto, load level, etc. (NO)
 *       - allows one user running a job as another ("runas") (SECURITY RISK)
 *       - "serial" to run next jobs finish-to-start (USE BASH SCRIPT &&, ||, !)
 *       - WHOLLY COW ITS HUGE!!!
 *
 *    sys-process/incron (196k)
 *       - inotify based cron system (in = inotify)
 *       - reacts to changes in the file system
 *       - TOTALLY DIFFERENT THING, REJECTED
 *
 *    sys-process/at (96k)
 *       - set a job to run once at a specific time
 *
 *   sys-process/cronbase (---k)
 *      - appears to essentially be the "run-crons" written in shell/bash
 *      - allows jobs to be dropped in a directory and run vs. using a crontab
 *      - it makes packages easier to install and setup a simple cron job
 *      - but, also makes it easy for jobs sneaking in and circumventing review
 *      - this appears to be a "dumification" feature to me (with security risk)
 *      - REJECTED
 *
 */
/*===[[ SISO : STRICT IN, STRICT OUT ]]========================================*

 *   with many applications, the hacking and security failure exploits happen
 *   because designers and programmers try to make the input mechanisms as
 *   understanding and flexible as possible.  on the one hand, this enables
 *   applications to adapt to new uses and live forever; but, on the other, it
 *   makes them extraordinarily vulnerable to attack and misuse.
 *
 *   so, we will be using a different model where inputs are limited and
 *   validated rigorously in such that it would be more difficult to exploit.
 *
 *   basic limits...
 *      - file names : 3-71 char, [A-Za-z0-9_], with '.' separating user.desc
 *      - user names : 1-20 characters and [A-Za-z0-9_] only
 *      - descripion : 1-50 characters and [A-Za-z0-9_] only
 *      - commands   : no longer than 250 characters
 *      - records    : no longer than 250 characters
 *      - files      : unlimited per user
 *      - lines      : less than 1000 per file
 *      - VARS       : none
 *      - SHELL      : /bin/dash
 *      - PATH       : /bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin
 *
 */
/*===[[ EXAMPLES ]]============================================================*

 *   one of the major failings of the documentation and nearly all web sites
 *   about cron and crontab is the almost completely lack of real world
 *   examples.  without useful and relevant examples, many tools just die or go
 *   into disuse.  worse yet, folks start teaching other application to wake
 *   themselves up periodically to do some task -- wasting resources.
 *
 *   cleansing temp and cache directories
 *
 *   checking user accounts
 *
 *   getting latest map images on cloud cover
 *
 *   getting the latest government statistics
 *
 *   getting most recent headlines and/or news rather than continuously
 *
 *   bringing down email on regular updates
 *
 *   syncronizing local content with network sites
 *
 *   remove firefox cache files
 *
 *   hourly chimes
 *
 *   indexing files
 *
 *   checking for root kits and/or viruses
 *
 */
/*===[[ DESIGN DECISIONS ]]====================================================*

 *   crontab updates...
 *      - it is too easy to lose the original crontab entries if you can edit
 *        them with "-e" so eliminate that option
 *      - we have seen several options for triggering a reread of the crontab
 *        files in the spool directory...
 *      1 dcron saves a file called "cron.update" with a list of users that
 *        have updated their cron files (could lose the file)
 *      2 many crons change the date on the crontab directory to trigger a
 *        reread of the files (also requires crontab)
 *      3 we could save all updated crontabs with an extention of ".updated"
 *        so that we can only look for those
 *      4 we could us a prefix of on the files such as "#" and only look for
 *        because its even faster
 *      - although option four is easy, i don't like allowing special characters
 *        into file names and so we won't do it
 *      - we will suffix crontab files with ".NEW" for ones to be read and then
 *        ".DEL" for files that should be purged and then deleted
 *      - then, khronos will only respond to a SIGHUP or a restart to read the
 *        files for updates (looking only at .NEW and .DEL)
 *
 *   potential speed improvements...
 *      - what if crontab created a linked list of only the jobs that could run
 *        in the next hour so that the searching is not by file/line, but
 *        directly with the linked list
 *      - that list could then be refreshed every hour to prepare for the
 *        next period
 *      - the list could really cover any period so its just a trade off of
 *        frequency of update to the speed of the test cycle
 *
 *   monitoring...
 *      - crond is typically just a process that runs without anyone paying
 *        any attention unless jobs stop completing -- hours or days later
 *      - i would like to create a mechanism whereby cron can somehow make it
 *        obvious if it is not running (someone watching the watcher ;)
 *      - i would also like to have an easy time knowing when cron was up and
 *        active versus down without manual work or research required
 *
 *   crontab editing...
 *      - i really don't like using crontab to edit the crontabs directly in the
 *        spool directory as it does not result in version control
 *      - i also don't think cron needs to be able to help users edit other
 *        folks crontabs when they could just "su" over and do it themselves
 *      - but, crontab should still be able to list an active crontab, delete
 *        any specific one, take a new/updated one or over stdin
 *
 *   crontab naming...
 *      - usually crontabs are named after the users that submit them, but i
 *        like dillon's idea that a user should be able to have multiple
 *      - with that idea, names could be named anything, but i like the idea of
 *        doing "<username>.<description>" so that they sort easily
 *
 *   security changes...
 *      - do not take any names with any characters outside a very basic range,
 *        meaning [A-Za-z0-9_], not even a dash, no spaces, nothing else
 *      - length of names are restricted to avoid potential issuse, so we are
 *        going to start with 50 characters
 *
 *
 */
/*===[[ SECURITY ]]============================================================*

 *   below is a list of security alerts relating to cron packages
 *
 *   2009-06 ubuntu
 *      - did not properly check setgid and initgroups return codes
 *      - could lead to security escalation
 *
 *   1996-12 department of energy
 *      - buffer overrun potential reading CLI from users
 *      - buffer overrun potential on setting environment variables
 *      - local users can gain root privileges
 *
 *   2000-11 all
 *      - crontab directory not secured from users so they can update crontabs
 *      - local users can gain root privileges
 *      * properly secure /var/spool/cron to 0700 (instead of 0755)
 *
 *   2007-10 aix
 *      - buffer overrun on reading crontabs
 *      - local users can gain root privileges
 *      * limit line length and number of lines to prevent hacking !!!
 *
 *   2001-01 freebsd
 *      - users can use crontab to read other folks crontabs
 *      - security violation and potential updates allowing privilege escalation
 *      * properly secure /var/spool/cron to 0700 (instead of 0755)
 *
 *   2001-05 vixie-cron
 *      - crontab does not drop privileges before allowing crontab editing
 *      - security violation and potential updates allowing privilege escalation
 *      * do not allow editing of crontabs with crontab (traceability issue) !!!
 *
 *   unknown
 *      - cron missets environment variable and sends mail to wrong address
 *      - security violation and potential loss of access/secured information
 *      * do not allow the setting of environment variables !!!
 *
 */
/*===[[ TODOS ]]===============================================================*

 *   TODO : handle system time changes -- look at dcron's logic
 *
 */
/*===[[ END HDOC ]]===========================================================*/


/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YKHRONOS_hguard
#define YKHRONOS_hguard loaded



/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "1.4d"
#define VER_TXT   "retirements working as well as NEW and DEL in unit testing"



/*---(headers)--------------------------------------------------*/
#include  <yLOG.h>
#include  <yDLST.h>
#include  <yPARSE.h>
#include  <ySCHED.h>
#include  <yEXEC.h>
#include  <ySTR.h>
#include  <yURG.h>

#include  <unistd.h>
#include  <sys/unistd.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/ioctl.h>
#include  <sys/wait.h>
#include  <sys/resource.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <string.h>
#include  <errno.h>
#define   _XOPEN_SOURCE
#include  <time.h>
#include  <dirent.h>
#include  <fcntl.h>
#include  <pwd.h>            /* getpwuid() to look up uid's and get names      */
#include  <grp.h>
#include  <err.h>
#include  <ctype.h>

/*---(communcation files)---------------------------------------*/
#define     DIR_CONF                "/etc/"
#define     DIR_RUN                 "/var/run/"
#define     DIR_LOG                 "/var/log/"
#define     DIR_YLOG                "/var/log/yLOG/"
#define     DIR_YHIST               "/var/log/yLOG.historical/"
#define     DIR_ROOT                "/home/machine/crontabs/"
#define     DIR_LOCAL               "c_quani/crontabs/"
#define     DIR_CENTRAL             "/var/spool/crontabs/"

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#define     DIR_UNIT_CENTRAL        "/tmp/khronos_test/"
#define     DIR_UNIT_USER           "/tmp/khronos_test/user/"


#define     FILE_LOCK               "khronos.pid"
#define     FILE_PULSE              "khronos.heartbeat"
#define     FILE_WATCH              "khronos.interrun_monitoring"
#define     FILE_EXEC               "khronos.execution_feedback"
#define     FILE_STATUS             "khronos.status_reporting"

#define     LOC_CENTRAL     'C'
#define     LOC_LOCAL       'L'
#define     LOC_VERIFY      'V'
#define     LOC_ALL         "CLV"

#define     ACT_PURGE       'p'
#define     ACT_LIST        'l'
#define     ACT_INST        'i'
#define     ACT_NONE        '-'
#define     ACT_HUP         'H'
#define     ACT_ALL         "pliH-"

#define     ACT_NEW         'N'
#define     ACT_DEL         'D'

/*---(work files and directories)-------------------------------*/
#define    CRONTABS      "/var/spool/crontabs"
#define    PATH          "/sbin:/bin:/usr/sbin:/usr/bin:/opt/sbin:/opt/bin:/usr/local/sbin:/usr/local/bin"
#define    SHELL         "/bin/dash"


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/

#define     LEN_RECD          2000 /* heatherly standard to slow hacker BS    */

#define     LEN_PATH           300 /* heatherly standard to slow hacker BS    */

#define     LEN_NAME            78 /* max crontab name                        */
#define     LEN_USER            21 /* max user field                          */
#define     LEN_DESC            51 /* max desc field                          */
#define     LEN_ACT              5 /* max desc field                          */

#define     LEN_TRACKER         30 /* max tack title field                    */
#define     LEN_COMMAND        250 /* heatherly standard to slow hacker BS    */
#define     LEN_FLAGS           10 /* tracking and control flags              */




#define     LEN_COMMENT     60     /* max comment field*/
#define     LEN_FIELD      100     /* max text field   */
#define     LEN_LINE      2000     /* max crontab line */
#define     LEN_CMD        500     /* max command len  */
#define     LEN_TEXT      2000
#define     LEN_LONG       500
#define     LEN_ACTION       5

#define    HOUR          3600     /* seconds per hour */


#define    MODE_USER     'i'
#define    MODE_DAEMON   'd'
#define    MODE_UNIT     'u'

#define    RUN_DAEMON    if (my.user_mode == MODE_DAEMON) 
#define    RUN_USER      if (my.user_mode == MODE_USER) 

#define    FILE_ACTIVE   '-'
#define    FILE_RETIRE   'y'
#define    FILE_REPLACE  'R'



/*---(debugging)-------------------------*/
#define     TEST        if (testing     == 'y')
extern      char        testing;
extern      int         failed;


extern      char        unit_answer [LEN_TEXT];
extern      char      verstring    [500];


/*---(convienence typedefs)-------------------------------------*/
typedef const int        cint;
typedef unsigned int     uint;
typedef const long       clong;
typedef const char       cchar;

/*---(structure short names)------------------------------------*/
typedef struct cCFILE    tCFILE;
typedef struct cCLINE    tCLINE;
typedef struct stat      tSTAT;
typedef struct passwd    tPASSWD;
typedef struct tm        tTIME;
typedef struct dirent    tDIRENT;
typedef struct flock     tFLOCK;


typedef struct cFILE     tFILE;
typedef struct cLINE     tLINE;
/*---(structures)-----------------------------------------------*/



struct cACCESSOR
{
   /*---(files)----------------*/
   char        quiet;              /* bool : 0=normal, 1=quiet                */
   char        updates;            /* bool : 0=normal, 1=quiet                */
   char        user_mode;          /* interactive, daemon, or unittest        */
   /*---(files)----------------*/
   char        dir_central  [LEN_PATH]; /* crontabs global directory               */
   char        dir_local    [LEN_PATH]; /* crontabs local directory                */
   char        name_pulser  [LEN_PATH]; /* pulser file name                        */
   char        name_watcher [LEN_PATH]; /* watcher file name                       */
   char        name_locker  [LEN_PATH]; /* run lock file name                      */
   char        name_exec    [LEN_PATH]; /* execution file name                     */
   char        name_status  [LEN_PATH]; /* status update file name                 */
   /*---(pulse)----------------*/
   char        pulse_time   [ 50]; /* last time string written to pulse       */
   char        pulse_begin  [ 50]; /* start of this cron run as string        */
   char        pulse_end    [ 50]; /* ending of last cron run as string       */
   /*---(idenfiers)------------*/
   char        prog        [LEN_USER];         /* program name called         */
   int         uid;                /* user udi of person who called khronos   */
   char        who         [LEN_USER];          /* user name of person who launched khronos  */
   char        am_root;            /* marked if actually called by root       */
   int         pid;                /* process id of khronos                   */
   int         ppid;               /* parent process id of khronos            */
   int         sid;                /* session id of khronos                   */
   /*---(files)----------------*/
   char        f_path      [LEN_PATH];      /* crontab file path              */
   char        f_ready;                     /* crontab name checks out        */
   char        f_name      [LEN_NAME];      /* crontab file name              */
   char        f_user      [LEN_USER];      /* crontab user name              */
   char        f_desc      [LEN_DESC];      /* crontab description            */
   int         f_uid;                       /* crontab execution uid          */
   char        f_ext       [LEN_ACT];       /* crontab extention              */
   char        f_full      [LEN_RECD];      /* crontab fully qualified name   */
   /*---(file deprecated)------*/
   char        name        [LEN_NAME];      /* name of the current crontab    */
   char        action      [LEN_ACTION];    /* crontab action requested       */
   char        full        [LEN_RECD];      /* full name (path and all)       */
   /*---(lines)----------------*/
   char        t_ready;                     /* task line checks out           */
   int         t_recdno;                    /* task line number in crontab    */
   char        t_schedule  [LEN_RECD];      /* task schedule requested        */
   char        t_tracker   [LEN_TRACKER];   /* task name                      */
   char        t_flags     [LEN_FLAGS];     /* task behavior flags            */
   char        t_command   [LEN_COMMAND];   /* task command to execute        */
   /*---(working variables)----*/
   char        parsed      [LEN_FIELD];     /* representation of the results of parsing  */
   long        fast_beg;                    /* current fast list start        */
   /*---(context info)---------*/
   int         for_line;                    /* line which uses context (next one)        */
   long        last_end;                    /* last end                       */
   long        this_start;                  /* present start                  */
   /*---(trigger)--------------*/
   char        resync;                      /* update crontabs : n=not, -=marked, a=all  */
   char        silent;                      /* y=log runs, n=no logging       */
};
extern    struct cACCESSOR my;


struct cFILE {
   /*---(master)---------------*/
   char        title       [LEN_NAME];      /* name of the cronfile           */
   int         uid;                         /* execution user uid             */
   /*---(working)--------------*/
   char        retire;                      /* marked for retirement          */
   /*---(done)-----------------*/
};

struct cLINE {
   /*---(master)---------------*/
   char        tracker     [LEN_NAME];      /* description of task            */
   int         recdno;                      /* line number in crontab         */
   tSCHED      sched;                       /* schedule structure             */
   char        command     [LEN_RECD];      /* shell command                  */
   /*---(working)--------------*/
   int         dur;                         /* duration from ysched           */
   int         dur_min;                     /* calculated min duration        */
   int         dur_max;                     /* calculated max duration        */
   int         rpid;                        /* pid if executing               */
   int         start;                       /* time started                   */
   char        retire;                      /* marked for retirement          */
   /*---(flags)----------------*/
   char        importance;                  /* on a H-M-L scale               */
   char        concern;                     /* notify after ? failures        */
   char        lower;                       /* lower limit on duration        */
   char        upper;                       /* upper limit on duration        */
   char        delay;                       /* delay handling flag            */
   /*---(feedback)-------------*/
   int         attempts;                    /* number of times launched       */
   int         failures;                    /* number of times ended well     */
   int         last_rpid;                   /* last rpid used                 */
   long        last_time;                   /* timestamp of last run          */
   int         last_exit;                   /* return code of last run        */
   /*---(done)-----------------*/
};


struct cCFILE {
   /*---(context)------------------------*/
   char        user        [LEN_USER];      /* username to execute jobs       */
   char        name        [LEN_NAME];      /* name of cronfile               */
   /*---(working)------------------------*/
   int         uid;                         /* uid of user to execute jobs    */
   char        retire;                      /* file retired, but lines running*/
   int         nlines;                      /* number of lines in file        */
   /*---(linked lists)-------------------*/
   tCLINE     *head;                        /* head of line linked list       */
   tCLINE     *tail;                        /* tail of line linked list       */
   tCFILE     *prev;                        /* next file doubly linked list   */
   tCFILE     *next;                        /* next file doubly linked list   */
   /*---(done)---------------------------*/
};



struct cCLINE {
   /*---(link to file)-------------------*/
   tCFILE     *file;                        /* link back up to the parent          */
   /*---(basics)-------------------------*/
   tSCHED      sched;                       /* schedule structure                  */
   char        command     [LEN_CMD];       /* shell command                       */
   /*---(context)------------------------*/
   int         recd;                        /* crontab record number               */
   char        tracker     [LEN_TRACKER];   /* long-term reference for entry  */
   char        comment     [LEN_COMMENT];   /* descriptive text               */
   /*---(execution)----------------------*/
   char        active;                      /* line is on the fast list            */
   char        deleted;                     /* line should be retired, but running */
   int         rpid;                        /* running=pid, ready=0                */
   /*---(profile)------------------------*/
   int         dur_exp;                     /* expected duration                   */
   int         dur_min;                     /* minimum duration                    */
   int         dur_max;                     /* maximum duration                    */
   /*---(controls)-----------------------*/
   char        importance;                  /* 0-5 levels of importance            */
   char        monitoring;                  /* should its status be reported       */
   char        catchup;                     /* must khronos rerun a miss           */
   char        busy_delay;                  /* delay start time if system is busy  */
   char        busy_skip;                   /* skip a run if the system is busy    */
   char        busy_kill;                   /* kill running job if system is busy  */
   /*---(historical)---------------------*/
   int         attempts;                    /* number of times it has been launched        */
   int         failures;                    /* number of times it has not ended well       */
   long        lasttime;                    /* timestamp of last run                       */
   char        lastexit;                    /* return code of last run                     */
   /*---(linked lists)-------------------*/
   tCLINE     *next;                        /* next line in line's doubly linked list      */
   tCLINE     *prev;                        /* next line in line's doubly linked list      */
   tCLINE     *fnext;                       /* next line in the fast path linked list      */
   tCLINE     *fprev;                       /* prev line in the fast path linked list      */
   tCLINE     *pnext;                       /* next line in the processing linked list     */
   tCLINE     *pprev;                       /* prev line in the processing linked list     */
};

/*---(file linked list)--------*/
extern    tCFILE   *h_cfile;   /* head cfile                                  */
extern    tCFILE   *t_cfile;   /* tail cfile                                  */
extern    tCFILE   *c_cfile;   /* current cfile                               */
extern    int       m_cfile;   /* total malloc'd cfile structures             */
extern    int       n_cfile;   /* total linked cfile structures               */
extern    int       m_cline;   /* total malloc'd cline structures             */
extern    int       n_cline;   /* total linked cline structures               */

/*---(fast path linked list)---*/
extern    tCLINE   *fasthead;
extern    tCLINE   *fasttail;
extern    int       nfast;

/*---(processing linked list)--*/
extern    tCLINE   *prochead;
extern    tCLINE   *proctail;
extern    int       nproc;


/*---(prototypes)-----------------------------------------------*/

int         main               (int argc, char *argv[]);


/*===[[ KHRONOS_PROG.C ]]=====================================================*/
/*---(utility)--------------*/
char        wait_minute        (void);
long        curr_hours         (void);
char        catchup            (void);
/*---(program)--------------*/
char*       PROG_version       (void);
char        PROG_init          (void);
char        PROG_whoami        (void);
char        PROG_args          (int argc, char *argv[]);
char        PROG_usage         (void);
char        PROG_begin         (void);
char        PROG_final         (void);
char        PROG_term          (void);
char        PROG_end           (void);
/*---(unit testing)---------*/
char*       PROG_getter        (char *a_question, int a_num);

char        prog__unit_files   (void);
char        prog__unit_quiet   (void);
char        prog__unit_loud    (void);
char        prog__unit_end     (void);


char      terminate     (cchar*, cint);;
void      communicate   (cint);
char      signals       (void);
char      daemonize     (void);
char      lock          (void);
char      pulse         (void);
char      BASE_status   (void);
char      BASE_begwatch (void);
char      BASE_endwatch (void);
char      prepare       (void);

long        BASE_lastpulse     (void);
char        BASE_timestamp     (void);
long      lastrun       (void);

char        search             (cchar);
char        assimilate         (cchar*);
char        BASE_purge         (void);
char        BASE_retire        (cchar*);
char        BASE_create        (cchar*, cchar*, tCFILE**);
char        BASE_inventory     (tCFILE*, FILE*);
char        context            (int, cchar*);
char        parse              (char*, char *, int, int, char*);
int         convert            (cchar*, cchar*, cint, cint);
char*       BASE_unit          (char*, int);
char        BASE_unitfile      (void);
char        BASE_unitproc      (void);
char        BASE_unitfast      (void);
char        BASE_unitshape     (char*);

char        BASE_fast          (clong);
char        BASE_dispatch      (cint);
char      run           (tCFILE*, tCLINE*);
char      check         (void);





/*===[[ KHRONOS_LIST.C ]]======================================*/
#define     UNLINKED       '-'
#define     LINKED         'y'
/*---(overall)---------*/
char        LIST_purge         (void);
/*---(cronfiles)-------*/
tCFILE*     CFILE_create       (char    a_linked);
char        CFILE_link         (tCFILE* a_cfile);
char        CFILE_unlink       (tCFILE* a_cfile);
tCFILE*     CFILE_delete       (tCFILE* a_cfile);
tCFILE*     CFILE_purge        (tCFILE* a_cfile);
tCFILE*     CFILE_find         (char*   a_name);
/*---(cronlines)-------*/
tCLINE*     CLINE_create       (tCFILE* a_cfile);
char        CLINE_link         (tCFILE* a_cfile, tCLINE* a_cline);
char        CLINE_unlink       (tCLINE* a_cline);
tCLINE*     CLINE_delete       (tCLINE* a_cline);
char        CLINE_show         (tCLINE *a_cline);
/*---(unittest)--------*/
char*       LIST_unit          (char *a_question, void *a_file);
char        LIST_list          (void);


char      proclist_add  (tCLINE*);
char      proclist_del  (tCLINE*);


char*     unit_accessor (char*, int);
/*> char      shape         (char*);                                                  <* 
 *> char      list_cron     (void);                                                   <* 
 *> char      list_fast     (void);                                                   <* 
 *> char      list_proc     (void);                                                   <*/



/*===[[ KHRONOS_TABS.C ]]======================================*/
/*---(names)-----------*/
char        tabs_set_path           (cchar *a_user, char a_scope);
/*---(review)----------*/
char        tabs_global             (cchar* a_user, cchar a_action);
char        tabs_local              (cchar* a_user, cchar a_action);
/*---support-----------*/
char        tabs__verify            (cchar *a_full);
char        tabs__remove            (cchar *a_full);
/*---actions-----------*/
char        tabs_rename             (char a_ext);
char        tabs_clear_extfiles     (void);
char        tabs_user               (cchar *a_user);
char        tabs_install            (cchar *a_name);
char        tabs_delete             (cchar *a_name);
char        crontab_test       (cchar*);
char        tabs_hup                (void);
/*---specialty---------*/
char        crontab_help       (void);
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---stubs-------------*/
char        tabs_cat_stub           (void);
char        tabs_dir_stub           (void);
char        tabs_stdin_stub         (void);
char        tabs_edit_stub          (void);
char*       tabs__unit              (char *a_question, int a_num);



char        data_assimilate         (void);
char        data_retire             (void);

tFILE*      file__new               (void);
char        file_create             (void);
char        file_destroy            (void);
char        file_retire_scan        (void);
char        file_retire             (void);
char        file_check_user         (cchar *a_user, cchar a_loc);
char        file_check_desc         (cchar *a_desc);
char        file_check_ext          (cchar *a_ext, cchar a_loc);
char        file_parse_name         (cchar* a_file, cchar a_loc);

tLINE*      line__new               (void);
char        line__populate          (tLINE *a_line);
char        line_create             (void);
char        line_parse              (void);
char        line_assimilate         (void);
int         line_prune              (void);
char        line__unit_rpid         (char *a_file, char *a_line, int a_rpid);



void      PROG_signal       (int a_signal, siginfo_t *a_info, void *a_nada);

#endif
/*=============================[[ end-of-code ]]==============================*/
