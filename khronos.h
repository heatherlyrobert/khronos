/*=============================[[ beg-of-code ]]==============================*/

/*===[[ HEADER ]]=============================================================#

 *   focus         : (SA) system_admin
 *   niche         : (ts) scheduler
 *   application   : khronos           (primeval god of time, HP)
 *   purpose       : provide consistent, reliable, time-based job scheduling
 *
 *   base_system   : gnu/linux  (because it's powerful, universal, and hackable)
 *   lang_name     : gnu/ansi-c (because it's right, just, best, and universal)
 *   dependencies  : yLOG only
 *   size          : small (less than 2,000 slocL)
 * 
 *   author        : the_heatherlys
 *   created       : 2010-05
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 *
 *   as always, keep the code simple while defending and logging like crazy
 * 
 */
/*===[[ SUMMARY ]]=============================================================*

 *   khronos is a fast, simplified, modernized, and technical version of the
 *   classic posix-defined crond time-based process scheduler which combines
 *   crond and crontab to allow deeper verification and traceablity.
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
 *   by computer science folks and kept it simple and robust.  bloody brilliant.
 *
 *   but, there are now many implementations of crond screaming out for use,
 *   each of which comes with its own set of creeping featurism, cruft, and
 *   accessibility "improvements."  they all just tend to piss me off as
 *   everything seems to be trending towards the closely coupled, gui-focused,
 *   kitchen-sink mentality that then tends to display well but never gets used.
 *
 *   khronos will attempt to implement the original simplicity, clarity, and
 *   power with updated algorithms, automated testing, strong logging and
 *   monitoring, stronger security, and a few added recovery/notification
 *   features.  we will maintain backward compatibility while focusing on an
 *   automation-intensive, power-user environment for our own personal use.
 *
 *   "do one thing and do it well (securely) and focus on technical power users"
 *
 *   khronos will provide...
 *      - near posix compatibility so it can do the full job on core features
 *      - backwards compatible with existing crontab formats (great design)
 *      - additional, specific job recovery features (don't break compatibility)
 *      - strict glibc/ansi-c so it can be ported to and compilied on anything
 *      - fast and efficient because we want to enable tons of automation
 *      - solid logging and status updates so you never have to guess
 *      - clean, clear code so we can maintain it after long absences
 *      - fullsome unit testing and regression testing suite
 *      - eliminate known and potential security gaps and hacking vectors
 *
 *   khronos will not provide...
 *      - automatic email -- everyone ultimately hates it (security risk)
 *      - alternate shells (we're gonna run pure posix dash)
 *      - extended shell variables (gonna have a spartan shell environment)
 *      - run-time configuration (no, its only for us, we can update code)
 *      - names for days and months, just use the numbers and like it ;)
 *      - special symbols for easly expressible things (@hourly, @weekly)
 *
 *   on a large scale, khronos will not provide the other parts of batch work...
 *      - dependency-based scheduling (like init systems provide)
 *      - event-based launches like @reboot (daemons can and should do this)
 *      - resource-based changes to schedules, such as system load or avail
 *
 *   we don't want to just use the system, we want to *own* it; so that means
 *   we have to fundmentally understand the critical services which only tearing
 *   down and rebuilding can really teach -- even if its more basic in the end.
 *
 *   as luck would have it, dcron (dillon's cron) is a wonderfully straight
 *   forward and clean implementation that we can build on.  it is licenced
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
 *
 *    sys-process/anacron (23k)
 *       - supplements cron to deal with inconsistent uptime, like laptops
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
/*===[[ COMPONENTS ]]==========================================================*

 *   we should never attempt to put all our desired capabilities into one tool
 *   as that only ever leads to tragedy and scope creep.  also, many of these
 *   components should function like quick little filters and wrappers rather
 *   than applications...
 *
 *   khronos     : continuously running job launching daemon
 *      - pure dispatcher with overly strong monitoring and logging
 *      - attempt to build in as few exceptions and mutations as possiible
 *
 *   crontab     : job list submission, review, and validation tool
 *      - simple tool to validate crontab files and place them in the spool
 *      - no need to build it in as it should be runable any time
 *
 *   cronpulse   : heartbeat to verify that the system is currently running
 *      - simple job to place the current datetime stamp in a file/shared mem
 *      - no need to build it in as it can just be scheduled like anything else
 *
 *   _log        : job start/stop logger specifically adapted to cron
 *      - simple wrapper to centrally log job start/stop for monitoring
 *      - no need to build it in as it can just be implemented as a wrapper
 *
 *   cronmon     : on-line monitoring tool
 *      - simple tool to see the status of jobs and what"s coming
 *      - no need to build it in as it is ad-hoc and not always desired
 *
 *   cronline    : graphical timeline of cron jobs and statuses
 *      - simple tool to place jobs on a visual timeline
 *      - no need to build it in as it is ad-hoc and not always desired
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
 *        files in the spool directory
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
 *
 *   potential speed improvements...
 *      - what if crontab created a linked list of only the jobs that could run
 *        in the next 24 hours so that the searching is not by file/line, but
 *        directly with the linked list
 *      - that list could then be refreshed every 24 hours to prepare for the
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
#ifndef YCHRONOS
#define YCHRONOS loaded


/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "0.7d"
#define VER_TXT   "break unit testing accessor out into a separate file"


/*---(headers)--------------------------------------------------*/
#include <yLOG.h>

#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>            /* getpwuid() to look up uid's and get names      */
#include <grp.h>
#include <err.h>
#include <ctype.h>

/*---(communcation files)---------------------------------------*/
#define    LOCKFILE      "/var/run/crond.pid"
#define    PULSER        "/var/log/yLOG/cronpulse.intrarun_last_check"
#define    STUFF         "/var/log/yLOG/cronextra.execution_feedback"
#define    WATCHER       "/var/log/yLOG.historical/cronwatch.interrun_monitoring"

/*---(work files and directories)-------------------------------*/
#define    CRONTABS      "/var/spool/cron/crontabs"
#define    PATH          "/sbin:/bin:/usr/sbin:/usr/bin:/opt/sbin:/opt/bin:/usr/local/sbin:/usr/local/bin"
#define    SHELL         "/bin/dash"

#define    USER            21     /* max user field   */
#define    DESC            51     /* max desc field   */
#define    NAME            72     /* max crontab name */
#define    LEN            100     /* max text field   */
#define    LINE           500     /* max crontab line */
#define    CMD            500     /* max command len  */

#define    HOUR          3600     /* seconds per hour */

extern     char       version;
#define    DAEMON        if (version == 'd')
#define    INTERACTIVE   if (version == 'i')



/*---(debugging)-------------------------*/
#define   DEBUG_T    if (debug_top   == 'y')
#define   DEBUG_A    if (debug_args  == 'y')
#define   DEBUG_I    if (debug_input == 'y')
#define   DEBUG_P    if (debug_proc  == 'y')
#define   TEST       if (testing     == 'y')
extern    char      debug_top;
extern    char      debug_args;
extern    char      debug_input;
extern    char      debug_proc;
extern    char      testing;



/*---(convienence typedefs)-------------------------------------*/
typedef const int        cint;
typedef const long       clong;
typedef const char       cchar;

/*---(structure short names)------------------------------------*/
typedef struct cCFILE    tCFILE;
typedef struct cCLINE    tCLINE;
typedef struct stat      tSTAT;
typedef struct passwd    tPASSWD;
typedef struct tm        tTIME;
typedef struct dirent    tDIRENT;

/*---(structures)-----------------------------------------------*/



struct cACCESSOR
{
   /*---(files)----------------*/
   char      quiet;              /* bool : 0=normal, 1=quiet                  */
   char      updates;            /* bool : 0=normal, 1=quiet                  */
   int       logger;             /* log file so that we don't close it        */
   int       locker;             /* lock file in /var/run                     */
   char      pulse_time [50];    /* last time string written to pulse         */
   char      pulse_begin[50];    /* start of this cron run as string          */
   char      pulse_end  [50];    /* ending of last cron run as string         */
   /*---(idenfiers)------------*/
   char      prog[USER];         /* program name called                       */
   int       uid;                /* user udi of person who called khronos     */
   char      who[USER];          /* user name of person who launched khronos  */
   char      am_root;            /* marked if actually called by root         */
   int       pid;                /* process id of khronos                     */
   int       ppid;               /* parent process id of khronos              */
   int       sid;                /* session id of khronos                     */
   /*---(working variables)----*/
   char      parsed[LEN];        /* representation of the results of parsing  */
   char      name[NAME];         /* name of the current crontab               */
   char      user[USER];         /* user name on current crontab              */
   char      desc[DESC];         /* descrpition for current crontab           */
   char      action[5];          /* crontab action requested                  */
   long      fast_beg;           /* current fast list start                   */
   /*---(context info)---------*/
   int       for_line;           /* line which uses context (next one)        */
   char      title[DESC];        /* title of current cron line                */
   char      duration;           /* -=?, t<m, s<5m, m<20m, L<1h, X<2h, #>2h   */
   char      recovery;           /* -=?, x=no, 1=once, B=batch                */
   char      priority;           /* -=normal                                  */
   char      alerting;           /* -=none                                    */
   long      last_end;           /* last end                                  */
   long      this_start;         /* present start                             */
   /*---(trigger)--------------*/
   char      resync;             /* update crontabs : n=not, -=marked, a=all  */
   char      silent;             /* y=log runs, n=no logging                  */
};
extern    struct cACCESSOR my;


struct cCFILE {      /* SLL, singly linked-list of crontab files              */
   char      name[NAME];     /* name of cronfile                              */
   char      user[USER];     /* username to execute jobs                      */
   int       uid;            /* uid of user to execute jobs                   */
   char      retire;         /* file should be retired, but lines are running */
   int       nlines;         /* number of lines in file                       */
   tCLINE   *head;           /* head of this file's line linked list          */
   tCLINE   *tail;           /* tail of this file's line linked list          */
   tCFILE   *prev;           /* next file in file doubly linked list          */
   tCFILE   *next;           /* next file in file doubly linked list          */
};

struct cCLINE {
   tCFILE   *file;           /* link back up to the parent                    */
   int       recd;           /* crontab record number                         */
   char      cmd[CMD];       /* shell command                                 */
   char      active;         /* line is on the fast list                      */
   char      deleted;        /* line should be retired, but is running        */
   int       rpid;           /* running=pid, ready=0                          */
   char      min[60];        /* bools : launch on minute 0-59                 */
   char      hrs[24];        /* bools : launch on hour   0-23                 */
   char      day[32];        /* bools : launch on day    1-31                 */
   char      mon[12];        /* bools : launch on month  0-11                 */
   char      dow[7];         /* bools : launch on day of week 0-6, beg sun    */
   char      title[DESC];    /* user assigned title of current cron line      */
   char      duration;       /* -=<m, S=<5m,  M=<20m,  L=<60m,  B=>60m        */
   char      recovery;       /* -=no, q=+15m, h=+1hr,  t=+2hr,  c=cumm        */
   char      priority;       /* -=normal                                      */
   char      alerting;       /* -=none                                        */
   tCLINE   *next;           /* next line in line's doubly linked list        */
   tCLINE   *prev;           /* next line in line's doubly linked list        */
   tCLINE   *fnext;          /* next line in the fast path linked list        */
   tCLINE   *fprev;          /* prev line in the fast path linked list        */
   tCLINE   *pnext;          /* next line in the processing linked list       */
   tCLINE   *pprev;          /* prev line in the processing linked list       */
};


/*---(file linked list)--------*/
extern    tCFILE   *cronhead;
extern    tCFILE   *crontail;
extern    int       nfile;
extern    int       nentry;

/*---(fast path linked list)---*/
extern    tCLINE   *fasthead;
extern    tCLINE   *fasttail;
extern    int       nfast;

/*---(processing linked list)--*/
extern    tCLINE   *prochead;
extern    tCLINE   *proctail;
extern    int       nproc;


/*---(prototypes)-----------------------------------------------*/

int       main               (int argc, char *argv[]);
char      prog_urgent        (int argc, char *argv[]);
char      prog_whoami        (void);
char      prog_args          (int argc, char *argv[]);
char      prog_usage         (void);
char      prog_begin         (void);
char      prog_term          (void);
char      prog_end           (void);


char      initialize    (cchar);
char      terminate     (cchar*, cint);;
void      communicate   (cint);
char      signals       (void);
char      daemonize     (void);
char      lock          (void);
char      pulse         (void);
char      watch_beg     (void);
char      watch_end     (void);
char      prepare       (void);

long      pulse_last    (void);
long      lastrun       (void);
char      timestamp     (void);

char      search        (cchar);
char      assimilate    (cchar*);
char      name          (cchar*, cchar);
char      purge         (void);
char      retire        (cchar*);
char      create        (cchar*, cchar*, tCFILE**);
char      inventory     (tCFILE*, FILE*);
char      context       (int, cchar*);
char      parse         (char*, char *, int, int, char*);
int       convert       (cchar*, cchar*, cint, cint);

char      fast          (clong);
char      dispatch      (cint);
char      run           (tCFILE*, tCLINE*);
char      check         (void);

char      cronfile_add  (tCFILE*);
char      cronfile_del  (tCFILE*);
char      cronline_add  (tCFILE*, tCLINE*);
char      cronline_del  (tCLINE*);
char      proclist_add  (tCLINE*);
char      proclist_del  (tCLINE*);


char*     unit_accessor (char*, int);
char      shape         (char*);
char      list_cron     (void);
char      list_fast     (void);
char      list_proc     (void);



char      crontab_proc    (cchar*, cchar);
char      crontab_local   (cchar);
char      crontab_verify  (cchar*, char);

char      crontab_inst    (cchar*);
char      crontab_del     (cchar*);
char      crontab_test    (cchar*);
char      crontab_cat     (cchar*);

char      crontab_help    (void);
char      crontab_user    (cchar*);
char      crontab_hup     (void);

char      crontab_stdin   (void);
char      crontab_edit    (void);
char      crontab_dir     (void);

#endif
/*=============================[[ end-of-code ]]==============================*/
