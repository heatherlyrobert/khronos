/*====================--------[[ start-of-code ]]---------====================*/

/*===[[ SUMMARY ]]============================================================#

 *   application   : khronos
 *   module        : khronos_main
 *   size          : less than 100 slocL
 *
 */
/*===[[ SUMMARY ]]============================================================#

 *   khronos_main is the entry point for khronos and is compilied separately
 *   so that unit testing have be separately linked
 *
 */
/*============================================================================*/


#include   "khronos.h"

#define   MIN         60

char          unit_answer [ LEN_TEXT ];



/*====================------------------------------------====================*/
/*===----                      utility routines                        ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        /* PURPOSE : reliably wait until the next crond trigger moment    */
wait_minute   (void)
{
   /* this function is impemented to perform even when signals interrupt      */
   /*---(locals)--------------------------------*/
   long      now       = 0;                       /* present time             */
   long      inc       = 0;                       /* seconds to sleep         */
   long      targ      = 0;                       /* time to reach            */
   /*---(process)-------------------------------*/
   now       = time(NULL);
   while (1) {
      inc   = MIN - (now % MIN) + 1;
      if (inc > MIN)         break;               /* solve SIGHUP after sleep */
      targ  = now + inc;
      DEBUG_LOOP   yLOG_value ("sleep (s)", inc);
      sleep(inc);
      now       = time(NULL);
      if (now >= targ)       break;               /* solve SIGHUP during sleep*/
   }
   /*---(complete)------------------------------*/
   return 0;
}

long        /* PURPOSE : return the time at the beginning of the current hour */
curr_hours    (void)
{
   long      now;
   long      curr;
   tTIME    *cbroke;       /* current unix broke-down date                */
   /*> int       cyrs;         /+ current year                                +/      <* 
    *> int       cwks;         /+ current week in the year                    +/      <* 
    *> int       cmon;         /+ current month in the year                   +/      <* 
    *> int       cday;         /+ current day in the month                    +/      <* 
    *> int       cdow;         /+ current dow of the week                     +/      <*/
   /*---(adjust for offset)--------------*/
   now       = time(NULL);
   curr      = now - (now % 3600);
   cbroke    = localtime(&now);
   /*---(set the date)-------------------*/
   ySCHED_setdate (cbroke->tm_year - 100, cbroke->tm_mon + 1, cbroke->tm_mday);
   return curr;
}

char
catchup       (void)
{
   DEBUG_LOOP   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   long      curr      = 0;                       /* curr hour                */
   int       min       = 0;                       /* curr minute              */
   /*---(init)----------------------------------*/
   my.silent = 'y';
   /*---(main loop)-----------------------------*/
   DEBUG_LOOP   yLOG_note ("catchup specially flagged missed jobs");
   min       = ((my.last_end / 60) % 60) + 1;     /* start right after last   */
   DEBUG_LOOP   yLOG_value ("first min", min);
   /*> curr      = my.last_end - (my.last_end % (60 * 60));                           <*/
   curr      = my.last_end;
   while (curr < my.this_start) {
      BASE_fast (curr);                           /* id jobs for this hour    */
      /*---(cycle minutes)----------------------*/
      while (min < 60 && curr <  my.this_start) {
         DEBUG_LOOP   yLOG_value ("minute", min);
         BASE_dispatch (min);
         min      += 1;
         curr     += 60;
      }
      if (curr >=  my.this_start) break;
      min       = 0;
   }
   DEBUG_LOOP   yLOG_value ("last min", min - 1);
   /*---(reset)---------------------------------*/
   my.silent = 'n';
   /*---(complete)------------------------------*/
   DEBUG_LOOP   yLOG_exit  (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   standard program functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char      verstring    [500];

char*        /*--: return versioning information ---------[ ------ [ ------ ]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strlcpy (t, "[tcc built]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc  ]", 15);
#elif  __CBANG__  > 0
   strlcpy (t, "[cbang    ]", 15);
#else
   strlcpy (t, "[unknown  ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, VER_NUM, VER_TXT);
   return verstring;
}

char         /*--: pre-argument program initialization ---[ leaf   [ ------ ]-*/
PROG_init          (void)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   /*---(log header)---------------------*/
   DEBUG_TOPS   yLOG_info    ("purpose" , "provide consistent, reliable, time-based job scheduling");
   DEBUG_TOPS   yLOG_info    ("namesake", "winged serpent khronos is the god of creation and unyielding time");
   DEBUG_TOPS   yLOG_info    ("khronos" , PROG_version    ());
   DEBUG_TOPS   yLOG_info    ("yLOG"    , yLOG_version    ());
   DEBUG_TOPS   yLOG_info    ("yURG"    , yURG_version    ());
   DEBUG_TOPS   yLOG_info    ("yDLST"   , yDLST_version   ());
   DEBUG_TOPS   yLOG_info    ("yPARSE"  , yPARSE_version  ());
   DEBUG_TOPS   yLOG_info    ("ySCHED"  , ySCHED_version  ());
   DEBUG_TOPS   yLOG_info    ("yEXEC"   , yEXEC_version   ());
   DEBUG_TOPS   yLOG_info    ("ySTR"    , ySTR_version    ());
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(begin)--------------------------*/
   my.uid = getuid();
   snprintf (my.dir_central , 200, "%s"  , DIR_CENTRAL);
   snprintf (my.name_pulser , 200, "%s%s", DIR_YLOG , FILE_PULSE);
   snprintf (my.name_watcher, 200, "%s%s", DIR_YHIST, FILE_WATCH);
   snprintf (my.name_locker , 200, "%s%s", DIR_RUN  , FILE_LOCK);
   snprintf (my.name_exec   , 200, "%s%s", DIR_YLOG , FILE_EXEC);
   snprintf (my.name_status , 200, "%s%s", DIR_YLOG , FILE_STATUS);
   my.silent  = 'n';
   m_cfile  = 0;
   n_cfile  = 0;
   m_cline  = 0;
   n_cline  = 0;
   nfast    = 0;
   nproc    = 0;
   my.user_mode = MODE_DAEMON;
   rc = yDLST_init ();
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("§");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : figure out who launched us and how -------------------*/
PROG_whoami        (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(locals)-------------------------*/
   tPASSWD    *x_pass      = NULL;         /* passwd data structure               */
   int         x_len       = 0;            /* user name length                    */
   int         rc          = 0;            /* generic return code                 */
   /*---(begin)--------------------------*/
   DEBUG_ARGS   printf("PROG_whoami()...\n");
   /*---(get real uid)-------------------*/
   my.uid    = getuid();
   DEBUG_ARGS   printf("   caller uid   = %d\n",    my.uid);
   /*---(pull user name)-----------------*/
   x_pass    = getpwuid(my.uid);
   if (x_pass == NULL) {
      printf("can not retreive your user information from the system\n");
      PROG_term();
   }
   DEBUG_ARGS   printf("   caller name  = <<%s>>\n",    x_pass->pw_name);
   /*---(check user name)----------------*/
   x_len      = strllen (x_pass->pw_name, 20);
   if (x_len < 1) {
      printf("your user name can not be empty\n");
      PROG_term();
   }
   if (x_len > 20) {
      printf("user name is too long\n");
      PROG_term();
   }
   strlcpy(my.who, x_pass->pw_name, 20);
   /*---(check for root user)--------------*/
   my.am_root = 'n';
   if (my.uid == 0)   my.am_root = 'y';
   DEBUG_ARGS   printf("   am i root    = %c\n",    my.am_root);
   /*---(change uid/permissions)-----------*/
   if (my.am_root != 'y') {
      rc = setuid(0);
      if (rc != 0) {
         printf("could not gain root authority\n");
         PROG_term();
      }
      DEBUG_ARGS   printf("   successfully changed to root\n");
   }
   /*---(log pid info)---------------------*/
   my.pid  = getpid();
   DEBUG_ARGS   printf("   current pid  = %d\n",    my.pid);
   my.ppid = getppid();
   DEBUG_ARGS   printf("   current ppid = %d\n",    my.ppid);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

#define    TWOARG      if (two_arg == 1)

char       /* PURPOSE : process the command line arguments -------------------*/
PROG_args          (int argc, char *argv[])
{
   /*---(locals)-------------------------*/
   char     *a         = NULL;         /* current argument                    */
   int       i         = 0;            /* loop iterator -- arguments          */
   int       len       = 0;            /* argument length                     */
   char      two_arg   = 0;
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_ARGS   printf("PROG_args()...\n");
   DEBUG_ARGS   printf("   processing %d args\n", argc);
   if (argc == 1) {
      DEBUG_ARGS   printf("   calling without args invokes the daemon\n");
      return 0;
   }
   /*---(program name)--------------------------*/
   strlcpy(my.prog, argv[0], LEN_USER);
   DEBUG_ARGS   printf("   prog name = <<%s>>\n\n", my.prog);
   /*---(process)------------------------*/
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (i < argc - 1) two_arg = 1; else two_arg = 0;
      len = strllen (a, LEN_LINE);
      /*---(skip debugging)--------------*/
      if      (a[0] == '@')                     continue;
      my.user_mode = MODE_USER;
      /*---(version)---------------------*/
      if      (strcmp (a, "--version"      ) == 0 || strcmp (a, "-V") == 0) {
         PROG_version ();
         printf ("%s\n", verstring);
         return -1;
      }
      /*---(usage/help)------------------*/
      else if (strcmp(a, "-h"        ) == 0)    PROG_usage();
      else if (strcmp(a, "--help"    ) == 0)    PROG_usage();
      /*---(lists)-----------------------*/
      else if (strcmp(a, "--list"    ) == 0)    tabs_global  (my.who, 'l');
      else if (strcmp(a, "--all"     ) == 0)    tabs_global  ("ALL" , 'l');
      else if (strcmp(a, "--here"    ) == 0)    tabs_local   (my.who, 'l');
      /*---(installing)------------------*/
      else if (a[0] != '-'           )          tabs_install (a);
      else if (strcmp(a, "--test"    ) == 0)  { TWOARG  crontab_test  (argv[++i]); }
      else if (strcmp(a, "--reload"  ) == 0)    tabs_local   (my.who, 'i');
      /*---(removing)--------------------*/
      else if (strcmp(a, "-d"        ) == 0)  { TWOARG  tabs_delete   (argv[++i]); }
      else if (strcmp(a, "-r"        ) == 0)  { TWOARG  tabs_delete   (argv[++i]); }
      else if (strcmp(a, "--purge"   ) == 0)    tabs_global  (my.who, 'p');
      else if (strcmp(a, "--cleanse" ) == 0)    tabs_global  ("ALL" , 'p');
      /*---(user switches)---------------*/
      else if (strcmp(a, "-u"        ) == 0)  { TWOARG  tabs_user     (argv[++i]); }
      else if (strcmp(a, "--user"    ) == 0)  { TWOARG  tabs_user     (argv[++i]); }
      /*---(warnings)--------------------*/
      else if (strcmp(a, "-l"        ) == 0)    tabs_cat_stub   ();
      else if (strcmp(a, "-c"        ) == 0)    tabs_dir_stub   ();
      else if (strcmp(a, "-e"        ) == 0)    tabs_edit_stub  ();
      else if (strcmp(a, "-"         ) == 0)    tabs_stdin_stub ();
      /*---(unknown)---------------------*/
      else    printf("requested action not understood or incomplete\n");
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : display usage help information -----------------------*/
PROG_usage         (void)
{
   printf ("\n");
   printf ("usage : khronos [URGENTS] [OPTIONS] [<FILE>]\n");
   printf ("the winged serpent khronos is the primeval god of creation and unyielding time\n");
   printf ("\n");
   printf ("khronos is a fast, simplified, modernized, and technical version of the\n");
   printf ("classic posix-defined crond time-based process scheduler which merges\n");
   printf ("crond and crontab to allow deeper verification, verbosity, and traceability\n");
   printf ("\n");
   printf ("crontab file naming: \"crontab.<desc>\"\n");
   printf ("   crontab.         all crontab files must begin with this prefix\n");
   printf ("   <desc>           unique specifier of file [A-Za-z0-9_]\n");
   printf ("\n");
   printf ("posix/standard crontab options implemented:\n");
   printf ("   <file>           install/replace a crontab file\n");
   printf ("   -l               list all your installed crontabs to stdout\n");
   printf ("   -d <desc>        delete one of your crontabs by name\n");
   printf ("   -r <desc>        delete one of your crontabs by name\n");
   printf ("   -u <user>        act as another user (root only)\n");
   printf ("\n");
   printf ("posix/standard crontab options rejected:\n");
   printf ("   - or null        install from stdin (no traceability, over-write risk)\n");
   printf ("   -e               edit your crontab (no traceability or backup)\n");
   printf ("   -c <dir>         change the crontab dir (no way, security risk)\n");
   printf ("\n");
   printf ("extended crontab options:\n");
   printf ("   --all            list all installed crontabs (root only)\n");
   printf ("   --list           list all your installed crontabs\n");
   printf ("   --here           list all local crontabs that could be installed\n");
   printf ("   --test <desc>    test a crontab file for formatting correctness\n");
   printf ("   --system <desc>  install system crontab (root only)\n");
   printf ("   --purge          delete all your installed crontabs\n");
   printf ("   --cleanse        delete all installed crontabs (root only)\n");
   printf ("   --reload         delete all then install your crontabs\n");
   printf ("   --help, -h       print usage information\n");
   printf ("\n");
   printf ("extended crond options rejected:\n");
   printf ("   -n               foreground operation (using logging instead)\n");
   printf ("   -x <option>      debugging options (using @ urgents instead)\n");
   printf ("   -p               allows weaker security on crontabs (no, no)\n");
   printf ("   -m <script>      special email handling (not useful anyway)\n");
   printf ("\n");
   printf ("heatherly debugging \"urgents\"\n");
   printf ("   @a               verbosely traces argument parsing\n");
   printf ("   @p               verbosely traces interactive/crontab logic\n");
   printf ("\n");
   printf ("changes to crontab file naming rules\n");
   printf ("   - must be formatted as \"crontab.<description>\"\n");
   printf ("   - our version allows multiple crontabs per user (different descritions)\n");
   printf ("   - description is '.' plus 1 to 50 characters\n");
   printf ("   - valid characters in the description are [A-Za-z0-9_] only\n");
   printf ("   - if you have only one crontab, likely just call it \"crontab.base\"\n");
   printf ("   - options above only require <desc> part of crontab name\n");
   printf ("\n");
   printf ("NOTE : all arguments will be processed in the order supplied\n");
   printf ("\n");
   exit   (0);
}

char             /* [------] post-argument program initialization ------------*/
PROG_begin         (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] post-argument program initialization ------------*/
PROG_final         (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_term          (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   int   rc = 0;
   rc = setuid(my.uid);
   printf("; fatal, program terminated\n");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   exit (-1);
   return 0;
}

char
PROG_end           (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   int   rc = 0;
   yDLST_wrap ();
   rc = setuid(my.uid);
   printf("\n");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   DEBUG_TOPS   yLOG_end     ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
PROG_getter        (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   int       xfore  = 0, xback  = 0;  /* counts to verify doubly-linked list */
   tCFILE   *xlist    = NULL;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "PROG_getter      : question not understood", LEN_TEXT);
   /*---(pulser)-------------------------*/
   if        (strncmp(a_question, "pulse_name"     , 20)   == 0) {
      snprintf (unit_answer, LEN_TEXT, "PROG pulse name  : %.35s", my.name_pulser);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char       /*----: set up program test file locations ------------------------*/
prog__unit_files   (void)
{
   char        x_cmd       [LEN_RECD];
   snprintf (my.name_pulser , 200, "%s%s", "/tmp/" , FILE_PULSE);
   snprintf (my.name_watcher, 200, "%s%s", "/tmp/" , FILE_WATCH);
   snprintf (my.name_locker , 200, "%s%s", "/tmp/" , FILE_LOCK);
   snprintf (my.name_exec   , 200, "%s%s", "/tmp/" , FILE_EXEC);
   snprintf (my.name_status , 200, "%s%s", "/tmp/" , FILE_STATUS);
   chdir  ("/tmp");
   sprintf (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT_USER   );
   system  (x_cmd);
   rmdir  (DIR_UNIT_USER);
   sprintf (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT_CENTRAL);
   system  (x_cmd);
   rmdir  (DIR_UNIT_CENTRAL);
   strlcpy (my.dir_central, DIR_UNIT_CENTRAL, LEN_PATH);
   sprintf (x_cmd, "mkdir %s > /dev/null", DIR_UNIT_CENTRAL);
   system  (x_cmd);
   mkdir  (DIR_UNIT_CENTRAL, 0777);
   sprintf (x_cmd, "mkdir %s > /dev/null", DIR_UNIT_USER   );
   system  (x_cmd);
   mkdir  (DIR_UNIT_USER   , 0777);
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   int         x_argc      = 1;
   char       *x_argv [1]  = { "khronos" };
   yURG_logger    (x_argc, x_argv);
   PROG_init      ();
   yURG_urgs      (x_argc, x_argv);
   PROG_whoami    ();
   prog__unit_files ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   PROG_final     ();
   my.user_mode = MODE_UNIT;
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   int         x_argc      = 5;
   char       *x_argv [5]  = { "khronos_unit", "@@kitchen", "@@yparse", "@@ydlst", "@@ysched"  };
   yURG_logger    (x_argc, x_argv);
   PROG_init      ();
   yURG_urgs      (x_argc, x_argv);
   PROG_whoami    ();
   prog__unit_files ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   PROG_final     ();
   my.user_mode = MODE_UNIT;
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   char        x_cmd       [LEN_RECD];
   chdir  ("/tmp");
   sprintf (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT_USER   );
   system  (x_cmd);
   rmdir  (DIR_UNIT_USER);
   sprintf (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT_CENTRAL);
   system  (x_cmd);
   rmdir  (DIR_UNIT_CENTRAL);
   strlcpy (my.dir_central, DIR_CENTRAL, LEN_PATH);
   PROG_end       ();
   return 0;
}



/*====================---------[[ end-of-code ]]----------====================*/
