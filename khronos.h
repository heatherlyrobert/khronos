/*=============================[[ beg-of-code ]]==============================*/

/*===[[ HEADER ]]=============================================================#

 *   focus         : (SA) system_admin
 *   niche         : (js) job_scheduler
 *   application   : khronos-ageraton  (primeval god of time [not cronos] HP)
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
#define VER_NUM   "1.2b"
#define VER_TXT   "final github preparation"



/*---(headers)--------------------------------------------------*/
#include  <yLOG.h>
#include  <ySCHED.h>
#include  <yEXEC.h>
#include  <ySTR.h>

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
#define     DIR_CONF         "/etc/"
#define     DIR_RUN          "/var/run/"
#define     DIR_LOG          "/var/log/"
#define     DIR_YLOG         "/var/log/yLOG/"
#define     DIR_YHIST        "/var/log/yLOG.historical/"
#define     DIR_ROOT         "/home/machine/crontabs/"
#define     DIR_LOCAL        "c_quani/crontabs/"

#define    FILE_LOCK     "khronos.pid"
#define    FILE_PULSE    "khronos.heartbeat"
#define    FILE_WATCH    "khronos.interrun_monitoring"
#define    FILE_EXEC     "khronos.execution_feedback"
#define    FILE_STATUS   "khronos.status_reporting"

/*---(work files and directories)-------------------------------*/
#define    CRONTABS      "/var/spool/crontabs"
#define    PATH          "/sbin:/bin:/usr/sbin:/usr/bin:/opt/sbin:/opt/bin:/usr/local/sbin:/usr/local/bin"
#define    SHELL         "/bin/dash"

#define     LEN_TRACKER     16     /* max title field  */
#define     LEN_COMMENT     60     /* max comment field*/
#define     LEN_USER        21     /* max user field   */
#define     LEN_DESC        51     /* max desc field   */
#define     LEN_NAME        72     /* max crontab name */
#define     LEN_FIELD      100     /* max text field   */
#define     LEN_LINE      2000     /* max crontab line */
#define     LEN_CMD        500     /* max command len  */
#define     LEN_TEXT      2000
#define     LEN_DIR        300
#define     LEN_LONG       500
#define     LEN_ACTION       5

#define    HOUR          3600     /* seconds per hour */

extern     char       version;
#define    DAEMON        if (version == 'd')
#define    INTERACTIVE   if (version == 'i')




/*===[[ DEBUGGING SETUP ]]====================================================*/
/* this is my latest standard format, vars, and urgents                       */
/* v3.0b : added signal handling                                (2014-feb-01) */
struct cDEBUG
{
   /*---(handle)-------------------------*/
   int         logger;                 /* log file so that we don't close it  */
   /*---(overall)------------------------*/  /* abcdefghi_kl__opq_stu__x__    */
   /* f = full urgents turns on all standard urgents                          */
   /* k = kitchen sink and turns everything, i mean everything on             */
   /* q = quiet turns all urgents off including the log itself                */
   char        tops;                   /* t) broad structure and context      */
   char        summ;                   /* s) statistics and analytical output */
   /*---(startup/shutdown)---------------*/
   char        args;                   /* a) command line args and urgents    */
   char        conf;                   /* c) configuration handling           */
   char        prog;                   /* p) program setup and teardown       */
   /*---(file processing)----------------*/
   char        inpt;                   /* i) text/data file input             */
   char        inpt_mas;               /* i) text/data file input   (mas/more)*/
   char        outp;                   /* o) text/data file output            */
   char        outp_mas;               /* o) text/data file output  (mas/more)*/
   /*---(event handling)-----------------*/
   char        loop;                   /* l) main program event loop          */
   char        user;                   /* u) user input and handling          */
   char        apis;                   /* z) interprocess communication       */
   char        sign;                   /* x) os signal handling               */
   char        scrp;                   /* b) scripts and batch operations     */
   char        hist;                   /* h) history, undo, redo              */
   /*---(program)------------------------*/
   char        graf;                   /* g) grahpics, drawing, and display   */
   char        data;                   /* d) complex data structure handling  */
   char        envi;                   /* e) environment processing           */
   char        envi_mas;               /* E) environment processing (mas/more)*/
   /*---(specific)-----------------------*/
   /*---(done)---------------------------*/
};
typedef     struct      cDEBUG       tDEBUG;
extern      tDEBUG      debug;

#define     DEBUG_TOPS          if (debug.tops      == 'y')
#define     DEBUG_SUMM          if (debug.summ      == 'y')
#define     DEBUG_ARGS          if (debug.args      == 'y')
#define     DEBUG_CONF          if (debug.conf      == 'y')
#define     DEBUG_PROG          if (debug.prog      == 'y')
#define     DEBUG_INPT          if (debug.inpt      == 'y')
#define     DEBUG_INPTM         if (debug.inpt_mas  == 'y')
#define     DEBUG_OUTP          if (debug.outp      == 'y')
#define     DEBUG_OUTPM         if (debug.outp_mas  == 'y')
#define     DEBUG_LOOP          if (debug.loop      == 'y')
#define     DEBUG_USER          if (debug.user      == 'y')
#define     DEBUG_APIS          if (debug.apis      == 'y')
#define     DEBUG_SIGN          if (debug.sign      == 'y')
#define     DEBUG_SCRP          if (debug.scrp      == 'y')
#define     DEBUG_HIST          if (debug.hist      == 'y')
#define     DEBUG_GRAF          if (debug.graf      == 'y')
#define     DEBUG_DATA          if (debug.data      == 'y')
#define     DEBUG_ENVI          if (debug.envi      == 'y')
#define     DEBUG_ENVIM         if (debug.envi_mas  == 'y')

/*---(debugging)-------------------------*/
#define     TEST        if (testing     == 'y')
extern      char        testing;
extern      int         failed;

#define     EXTRA

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

/*---(structures)-----------------------------------------------*/



struct cACCESSOR
{
   /*---(files)----------------*/
   char        quiet;              /* bool : 0=normal, 1=quiet                  */
   char        updates;            /* bool : 0=normal, 1=quiet                  */
   /*---(files)----------------*/
   char        name_pulser  [200]; /* pulser file name                          */
   char        name_watcher [200]; /* watcher file name                         */
   char        name_locker  [200]; /* run lock file name                        */
   char        name_exec    [200]; /* execution file name                       */
   char        name_status  [200]; /* status update file name                   */
   /*---(pulse)----------------*/
   char        pulse_time   [ 50]; /* last time string written to pulse         */
   char        pulse_begin  [ 50]; /* start of this cron run as string          */
   char        pulse_end    [ 50]; /* ending of last cron run as string         */
   /*---(idenfiers)------------*/
   char        prog        [LEN_USER];         /* program name called                       */
   int         uid;                /* user udi of person who called khronos     */
   char        who         [LEN_USER];          /* user name of person who launched khronos  */
   char        am_root;            /* marked if actually called by root         */
   int         pid;                /* process id of khronos                     */
   int         ppid;               /* parent process id of khronos              */
   int         sid;                /* session id of khronos                     */
   /*---(working variables)----*/
   char        parsed      [LEN_FIELD];     /* representation of the results of parsing  */
   char        name        [LEN_NAME];      /* name of the current crontab               */
   char        user        [LEN_USER];      /* user name on current crontab              */
   char        desc        [LEN_DESC];      /* descrpition for current crontab           */
   char        action      [LEN_ACTION];    /* crontab action requested                  */
   long        fast_beg;                    /* current fast list start                   */
   /*---(context info)---------*/
   int         for_line;                    /* line which uses context (next one)        */
   char        tracker     [LEN_DESC];      /* title of current cron line                */
   long        last_end;                    /* last end                                  */
   long        this_start;                  /* present start                             */
   /*---(trigger)--------------*/
   char        resync;                      /* update crontabs : n=not, -=marked, a=all  */
   char        silent;                      /* y=log runs, n=no logging                  */
};
extern    struct cACCESSOR my;



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
char        PROG_urgsmass      (char a_set, char a_extra);
char        PROG_urgs          (int argc, char *argv[]);
char        PROG_init          (void);
char        PROG_whoami        (void);
char        PROG_args          (int argc, char *argv[]);
char        PROG_usage         (void);
char        PROG_begin         (void);
char        PROG_term          (void);
char        PROG_end           (void);
/*---(unit testing)---------*/
char*       PROG_getter        (char *a_question, int a_num);
char        PROG_testfiles     (void);
char        PROG_testquiet     (void);
char        PROG_testloud      (void);


/*> char      initialize    (cchar);                                                  <*/
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
char        BASE_name          (cchar*, cchar);
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
/*---directories-------*/
char        crontab_proc       (cchar*, cchar);
char        TABS_local         (cchar a_action);
char        crontab_verify     (cchar*, char);
/*---actions-----------*/
char        crontab_inst       (cchar*);
char        crontab_del        (cchar*);
char        crontab_cat        (cchar*);
char        crontab_test       (cchar*);
/*---specialty---------*/
char        crontab_help       (void);
char        crontab_user       (cchar*);
char        crontab_hup        (void);
/*---stubs-------------*/
char        crontab_stdin      (void);
char        crontab_edit       (void);
char        crontab_dir        (void);



void      PROG_signal       (int a_signal, siginfo_t *a_info, void *a_nada);

#endif
/*=============================[[ end-of-code ]]==============================*/
