/*===[[ START HDOC ]]=========================================================*/
/*===[[ HEADER ]]=============================================================#

 *   focus         : system_admin
 *   niche         : scheduling
 *   application   : crond
 *   purpose       : provide consistent, reliable, time-based job scheduling
 *
 *   base_system   : gnu/linux
 *   lang_name     : gnu/ansi-c (because it's right, just, best, and universal)
 *   dependencies  : yLOG
 *   size          : small (less than 2,000 slocL)
 * 
 *   author        : the_heatherlys
 *   created       : 2010-05
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */
/*===[[ PURPOSE ]]=============================================================*

 *   crond is a posix-defined daemon that provides time-based job scheduling.
 *   in the best spirit of unix, it was designed to do one thing and do it well.
 *   crond uses a direct, expressive scheduling grammar in clear, clean, and
 *   simple text files to enable reliable, generalized time-based automation.
 *
 *   ---------------------------------------------------------------------------
 *
 *   while crond has easily withstood the test of time, modern variations have
 *   developed a few clever new ideas, a great deal of cruft, and some wicked
 *   feature creep that has made the daemon grow big and complex.  our goal is
 *   to separate out the tools and provide a toolset to accomplish all these
 *   scheduling situations, but to keep each piece clean and tidy.
 *
 *   our crond tries to be a simplified and streamlined version which combines
 *   the original simplicity, clarity, and power with updated algorithms,
 *   automated testing, adjustable logging, and a few added job recovery
 *   features.  the goal is to maintain backward compatibility while
 *   accomodating an automation-intensive, power-user environment for our own
 *   personal use.
 *
 *   crond will provide...
 *      - near posix compatibility so it can do the full job
 *      - backwards compatible existing crontab formats and contents
 *      - strict glibc/ansi-c so it can be ported to and compilied on anything
 *      - fast and efficient because we want to enable tons of automation
 *      - solid logging and status updates to greatly assist monitoring
 *      - clean, clean code so we can maintain it after long absences
 *      - fullsome unit testing and regression testing suite
 *      - eliminate known and potential security gaps and hacking vectors
 *
 *   crond will not provide...
 *      - any automatic email -- everyone hates this in the long run anyway
 *      - names for days and months, just use the numbers and like it
 *      - extended shell variables (gonna have a spartan shell environment)
 *      - alternate shells (we're gonna run dash by default, bash where we must)
 *      - run-time configuration (its only for us, we can update code)
 *      - special symbols for easly expressible things (@hourly, @weekly)
 *
 *   on a large scale, crond will not provide the other parts of batch work...
 *      - dependency-based scheduling (like init systems provide)
 *      - event-based launches like @reboot (daemons can and should do this)
 *      - resource-based changes to schedules, such as system load or avail
 *
 *   we don't want to just use the system, we want to *own* it; so that means
 *   we have to fundmentally understand the critical services which only tearing
 *   down and rebuilding can really teach.
 *
 *   as luck would have it, dcron (dillon's cron) is a wonderfully straight
 *   forward and clean implementation that we can build on.  it is licenced
 *   under the gnu gpl and so is perfect as our base.  so, we fork dcron...
 *
 *   so, as always, there are many, stable, accepted, existing programs and
 *   utilities written by better programmers which are likely superior in
 *   speed, size, capability, and reliability; BUT, i would not have learned
 *   nearly as much just using them, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   at the end of the day, this is wicked critical and deserves the attention
 *
 */
/*===[[ COMPONENTS ]]==========================================================*

 *   we should never attempt to put all our desired capabilities into one tool
 *   as that only ever leads to tragedy and scope creep.  also, many of these
 *   components should function like quick little filters and wrappers rather
 *   than applications...
 *
 *   crond       : continuously running job launching daemon
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
 *   validated rigorously in such that it would be very difficult to exploit the
 *   program.
 *
 *   basic limits...
 *      - file names : 3-71 char, [A-Za-z0-9_], with '.' separating user.desc
 *      - user names : 1-20 characters and [A-Za-z0-9_] only
 *      - descripion : 1-50 characters and [A-Za-z0-9_] only
 *      - commands   : no longer than 2000 characters
 *      - records    : no longer than 2500 characters
 *      - files      : unlimited per user
 *      - lines      : less than 1000 per file
 *      - VARS       : none
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
 *   system maintenance...
 *      - cleansing temp and cache directories
 *      - checking user accounts
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
 */
/*===[[ DESIGN DECISIONS ]]====================================================*

 *   crontab updates...
 *      - it is too easy to lose the original crontab entries if you can edit
 *        them with "-e" so eliminate that option
 *      - we have seen several options for triggering a reread of the crontab
 *        files in the spool directory
 *      1 dcron saves a file called "cron.update" with a list of users that
 *        have updated their cron files (using crontab)
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
 *        obvious if it is not running (something watching the watcher ;)
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
 *        doing "<username>_<description>" so that they sort easily
 *
 *   security changes...
 *      - do not take any names with any characters outside a very basic range,
 *        meaning [A-Za-z0-9_], not even a dash, no spaces, nothing else
 *      - length of names are restricted to avoid potential issuse, so we are
 *        going to start with 50 characters
 *
 *
 */
/*===[[ DCRON ]]===============================================================*

 *   command line arguments...
 *      - logging level        : we will not make configurable
 *      - debugging level      : will use yLOG
 *      - foreground mode      : not an option
 *
 *
 *
 *   code size...
 *      - main       200
 *      - job        200
 *      - database   600
 *      - crontab    300
 *      - subs       150
 *      - defs       100
 *      - TOTAL     1500
 *
 *   our goal is to come in at 1000
 *
 */
/*===[[ TODOS ]]===============================================================*

 *   TODO : handle system time changes -- look at dcron's logic
 *
 *   TODO : work out lauching in dash (perfect POSIX) only
 *
 *   TODO : cut down environment variables
 *
 */
/*===[[ END HDOC ]]===========================================================*/


/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YCRON
#define YCRON loaded


/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "0.5b"
#define VER_TXT   "up and running again with cronpulse and passing unit test"


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
#define    PULSER        "/var/log/yLOG.program_logging/cronpulse.intrarun_last_check"
#define    WATCHER       "/var/log/yLOG.program_logging/cronwatch.interrun_monitoring"

/*---(work files and directories)-------------------------------*/
#define    CRONTABS      "/var/spool/cron/crontabs"
#define    CRONLOGS      "/var/log/cron"
#define    CRONCONF      "/etc"
#define    CRONSYS       "/etc"
#define    CRONUPDATE    "cron.update"
#define    PATH          "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin"
#define    SHELL         "/bin/dash"

#define    USER            21     /* max user field   */
#define    DESC            51     /* max desc field   */
#define    NAME            72     /* max crontab name */
#define    LEN            100     /* max text field   */
#define    LINE          2500     /* max crontab line */
#define    CMD           2000     /* max command len  */

#define    HOUR          3600     /* seconds per hour */


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
   char      pulse_time[50];     /* last time string written to pulse         */
   /*---(idenfiers)------------*/
   int       uid;                /* user of person who launches crond         */
   int       pid;                /* process id of crond                       */
   int       ppid;               /* parent process id of crond                */
   int       sid;                /* session id of crond                       */
   /*---(working variables)----*/
   char      parsed[LEN];        /* representation of the results of parsing  */
   char      name[NAME];         /* name of the current crontab               */
   char      user[USER];         /* user name on current crontab              */
   char      desc[DESC];         /* descrpition for current crontab           */
   char      action[5];          /* crontab action requested                  */
   long      fast_beg;           /* current fast list start                   */
   int       fast_dur;           /* current fast list duration                */
   /*---(context info)---------*/
   int       for_line;           /* line which uses context (next one)        */
   char      title[DESC];        /* title of current cron line                */
   char      duration;           /* -=<m, s=1-5m, m=5-20m, l=20-60m, b=>60m   */
   char      recovery;           /* -=never, c=cumm, f=fixed, w=+2hrs         */
   char      priority;           /* -=normal                                  */
   char      alerting;           /* -=none                                    */
   /*---(trigger)--------------*/
   char      resync;             /* update crontabs : n=not, -=marked, a=all  */
} my;


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


/*---(prototypes)-----------------------------------------------*/
char      initialize    (cchar);
char      terminate     (cchar*, cint);;
void      communicate   (cint);
char      signals       (void);
char      daemonize     (void);
char      lock          (void);
char      pulse         (void);
char      watch         (void);
char      prepare       (void);

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

char      fast          (clong, cint);
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


#endif
/*=============================[[ end-of-code ]]==============================*/
