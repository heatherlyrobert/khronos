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

tDEBUG      debug;

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

char         /*--: wholesale urgents change --------------[ leaf   [ ------ ]-*/
PROG_urgsmass      (char a_set, char a_extra)
{
   /*---(overall)------------------------*/
   debug.tops     = a_set;
   debug.summ     = a_set;
   /*---(startup/shutdown)---------------*/
   debug.args     = a_set;
   debug.conf     = a_set;
   debug.prog     = a_set;
   /*---(file processing)----------------*/
   debug.inpt     = a_set;
   debug.inpt_mas = a_set;
   debug.outp     = a_set;
   debug.outp_mas = a_set;
   /*---(event handling)-----------------*/
   debug.loop     = a_set;
   debug.user     = a_set;
   debug.apis     = a_set;
   debug.sign     = a_set;
   debug.scrp     = a_set;
   debug.hist     = a_set;
   /*---(program)------------------------*/
   debug.graf     = a_set;
   debug.data     = a_set;
   debug.envi     = a_set;
   debug.envi_mas = a_set;
   /*---(specific)-----------------------*/
   if (a_extra == 'y') {
      ;;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: process command line urgents ----------[ ------ [ ------ ]-*/
PROG_urgs          (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;        /* loop iterator -- arguments          */
   char       *a           = NULL;     /* current argument                    */
   int         x_len       = 0;        /* argument length                     */
   int         x_total     = 0;
   int         x_urgs      = 0;
   char        x_logger    = '-';
   /*---(default urgents)----------------*/
   PROG_urgsmass ('-', 'y');
   /*---(logger preprocessing)-----------*/
   debug.logger = -1;
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (a[0] != '@')  continue;
      x_logger   = 'y';
      if       (strcmp ("@a"        , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@@args"    , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@f"        , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@@full"    , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@k"        , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@@kitchen" , a) == 0)  debug.args = 'y';
      else if ((strcmp ("@q", a) == 0) || (strcmp ("@@quiet", a) == 0)) {
         x_logger = 'n';
         PROG_urgsmass ('-', 'y');
         break;
      }
   }
   /*---(startup logging)----------------*/
   if (x_logger == 'y') {
      debug.tops = 'y';
      debug.logger = yLOG_begin ("khronos" , yLOG_SYSTEM, yLOG_NOISE);
      DEBUG_TOPS   yLOG_info     ("purpose" , "consistent, reliable time-based job scheduling");
      DEBUG_TOPS   yLOG_info     ("khronos" , PROG_version   ());
      DEBUG_TOPS   yLOG_info     ("ySCHED"  , ySCHED_version ());
      DEBUG_TOPS   yLOG_info     ("yEXEC"   , yEXEC_version  ());
      DEBUG_TOPS   yLOG_info     ("ySTR"    , ySTR_version   ());
      DEBUG_TOPS   yLOG_info     ("yLOG"    , yLOG_version   ());
   } else {
      debug.logger = yLOG_begin ("khronos", yLOG_SYSTEM, yLOG_QUIET);
   }
   /*---(walk through urgents)-----------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   for (i = 1; i < argc; ++i) {
      /*---(prepare)---------------------*/
      a        = argv[i];
      x_len    = strllen (a, LEN_LINE);
      ++x_total;
      /*---(filter)----------------------*/
      if (a[0] != '@')  continue;
      ++x_urgs;
      DEBUG_ARGS  yLOG_info  ("urgent"    , a);
      /*---(overall)---------------------*/
      if      (strncmp(a, "@t"           ,10) == 0)  debug.tops = 'y';
      else if (strncmp(a, "@@top"        ,10) == 0)  debug.tops = 'y';
      else if (strncmp(a, "@s"           ,10) == 0)  debug.tops = debug.summ  = 'y';
      else if (strncmp(a, "@@summ"       ,10) == 0)  debug.tops = debug.summ  = 'y';
      /*---(startup/shutdown)------------*/
      else if (strncmp(a, "@a"           ,10) == 0)  debug.tops = debug.args  = 'y';
      else if (strncmp(a, "@@args"       ,10) == 0)  debug.tops = debug.args  = 'y';
      else if (strncmp(a, "@c"           ,10) == 0)  debug.tops = debug.conf  = 'y';
      else if (strncmp(a, "@@conf"       ,10) == 0)  debug.tops = debug.conf  = 'y';
      else if (strncmp(a, "@p"           ,10) == 0)  debug.tops = debug.prog  = 'y';
      else if (strncmp(a, "@@prog"       ,10) == 0)  debug.tops = debug.prog  = 'y';
      /*---(file processing)-------------*/
      else if (strncmp(a, "@i"           ,10) == 0)  debug.tops = debug.inpt  = 'y';
      else if (strncmp(a, "@@inpt"       ,10) == 0)  debug.tops = debug.inpt  = 'y';
      else if (strncmp(a, "@I"           ,10) == 0)  debug.tops = debug.inpt  = debug.inpt_mas = 'y';
      else if (strncmp(a, "@@INPT"       ,10) == 0)  debug.tops = debug.inpt  = debug.inpt_mas = 'y';
      else if (strncmp(a, "@o"           ,10) == 0)  debug.tops = debug.outp  = 'y';
      else if (strncmp(a, "@@outp"       ,10) == 0)  debug.tops = debug.outp  = 'y';
      else if (strncmp(a, "@O"           ,10) == 0)  debug.tops = debug.outp  = debug.outp_mas = 'y';
      else if (strncmp(a, "@@OUTP"       ,10) == 0)  debug.tops = debug.outp  = debug.outp_mas = 'y';
      /*---(processing)------------------*/
      else if (strncmp(a, "@l"           ,10) == 0)  debug.tops = debug.loop  = 'y';
      else if (strncmp(a, "@@loop"       ,10) == 0)  debug.tops = debug.loop  = 'y';
      else if (strncmp(a, "@u"           ,10) == 0)  debug.tops = debug.user  = 'y';
      else if (strncmp(a, "@@user"       ,10) == 0)  debug.tops = debug.user  = 'y';
      else if (strncmp(a, "@z"           ,10) == 0)  debug.tops = debug.apis  = 'y';
      else if (strncmp(a, "@@apis"       ,10) == 0)  debug.tops = debug.apis  = 'y';
      else if (strncmp(a, "@x"           ,10) == 0)  debug.tops = debug.sign  = 'y';
      else if (strncmp(a, "@@sign"       ,10) == 0)  debug.tops = debug.sign  = 'y';
      else if (strncmp(a, "@b"           ,10) == 0)  debug.tops = debug.scrp  = 'y';
      else if (strncmp(a, "@@scrp"       ,10) == 0)  debug.tops = debug.scrp  = 'y';
      else if (strncmp(a, "@h"           ,10) == 0)  debug.tops = debug.hist  = 'y';
      else if (strncmp(a, "@@hist"       ,10) == 0)  debug.tops = debug.hist  = 'y';
      /*---(program)---------------------*/
      else if (strncmp(a, "@g"           ,10) == 0)  debug.tops = debug.graf  = 'y';
      else if (strncmp(a, "@@graf"       ,10) == 0)  debug.tops = debug.graf  = 'y';
      else if (strncmp(a, "@d"           ,10) == 0)  debug.tops = debug.data  = 'y';
      else if (strncmp(a, "@@data"       ,10) == 0)  debug.tops = debug.data  = 'y';
      else if (strncmp(a, "@e"           ,10) == 0)  debug.tops = debug.envi  = 'y';
      else if (strncmp(a, "@@envi"       ,10) == 0)  debug.tops = debug.envi  = 'y';
      else if (strncmp(a, "@E"           ,10) == 0)  debug.tops = debug.envi  = debug.envi_mas = 'y';
      else if (strncmp(a, "@@ENVI"       ,10) == 0)  debug.tops = debug.envi  = debug.envi_mas = 'y';
      /*---(complex)---------------------*/
      else if (strncmp(a, "@f"           ,10) == 0)  PROG_urgsmass ('y', '-');
      else if (strncmp(a, "@@full"       ,10) == 0)  PROG_urgsmass ('y', '-');
      else if (strncmp(a, "@k"           ,10) == 0)  PROG_urgsmass ('y', 'y');
      else if (strncmp(a, "@@kitchen"    ,10) == 0)  PROG_urgsmass ('y', 'y');
      /*---(specific)--------------------*/
      /*---(unknown)--------------------*/
      else {
         DEBUG_ARGS  yLOG_note    ("urgent not understood");
      }
      /*---(done)-----------------------*/
   }
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of urgent processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("urgents"   , x_urgs);
   DEBUG_ARGS   yLOG_note    ("standard urgents");
   DEBUG_ARGS   yLOG_char    ("tops"      , debug.tops);
   DEBUG_ARGS   yLOG_char    ("summ"      , debug.summ);
   DEBUG_ARGS   yLOG_char    ("args"      , debug.args);
   DEBUG_ARGS   yLOG_char    ("conf"      , debug.conf);
   DEBUG_ARGS   yLOG_char    ("prog"      , debug.prog);
   DEBUG_ARGS   yLOG_char    ("inpt"      , debug.inpt);
   DEBUG_ARGS   yLOG_char    ("INPT"      , debug.inpt_mas);
   DEBUG_ARGS   yLOG_char    ("outp"      , debug.outp);
   DEBUG_ARGS   yLOG_char    ("OUTP"      , debug.outp_mas);
   DEBUG_ARGS   yLOG_char    ("loop"      , debug.loop);
   DEBUG_ARGS   yLOG_char    ("user"      , debug.user);
   DEBUG_ARGS   yLOG_char    ("apis"      , debug.apis);
   DEBUG_ARGS   yLOG_char    ("sign"      , debug.sign);
   DEBUG_ARGS   yLOG_char    ("scrp"      , debug.scrp);
   DEBUG_ARGS   yLOG_char    ("hist"      , debug.hist);
   DEBUG_ARGS   yLOG_char    ("graf"      , debug.graf);
   DEBUG_ARGS   yLOG_char    ("data"      , debug.data);
   DEBUG_ARGS   yLOG_char    ("envi"      , debug.envi);
   DEBUG_ARGS   yLOG_char    ("ENVI"      , debug.envi_mas);
   DEBUG_ARGS   yLOG_note    ("specialty urgents");
   DEBUG_ARGS   yLOG_note    ("none yet");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: pre-argument program initialization ---[ leaf   [ ------ ]-*/
PROG_init          (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(begin)--------------------------*/
   my.uid = getuid();
   DEBUG_PROG   yLOG_value ("uid num", my.uid);
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
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(locals)-------------------------*/
   char     *a         = NULL;         /* current argument                    */
   int       i         = 0;            /* loop iterator -- arguments          */
   int       len       = 0;            /* argument length                     */
   char      two_arg   = 0;
   /*---(begin)--------------------------*/
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
   version = 'd';
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (i < argc - 1) two_arg = 1; else two_arg = 0;
      len = strllen (a, LEN_LINE);
      /*---(skip debugging)--------------*/
      if      (a[0] == '@')                     continue;
      version = 'i';
      /*---(version)---------------------*/
      if      (strcmp (a, "--version"      ) == 0 || strcmp (a, "-V") == 0) {
         PROG_version ();
         printf ("%s\n", verstring);
         return -1;
      }
      /*---(interactive)-----------------*/
      else if (strcmp(a, "-i"        ) == 0)    version = 'i';
      /*---(usage/help)------------------*/
      else if (strcmp(a, "-h"        ) == 0)    PROG_usage();
      else if (strcmp(a, "--help"    ) == 0)    PROG_usage();
      /*---(lists)-----------------------*/
      else if (strcmp(a, "-l"        ) == 0)    crontab_proc  (my.who, 'c');
      else if (strcmp(a, "--list"    ) == 0)    crontab_proc  (my.who, 'l');
      else if (strcmp(a, "--all"     ) == 0)    crontab_proc  ("ALL" , 'l');
      else if (strcmp(a, "--here"    ) == 0)    TABS_local    ('l');
      /*---(installing)------------------*/
      else if (a[0] != '-'           )          crontab_inst  (a);
      else if (strcmp(a, "--test"    ) == 0)  { TWOARG  crontab_test  (argv[++i]); }
      else if (strcmp(a, "--reload"  ) == 0)    TABS_local    ('i');
      /*---(removing)--------------------*/
      else if (strcmp(a, "-d"        ) == 0)  { TWOARG  crontab_del   (argv[++i]); }
      else if (strcmp(a, "-r"        ) == 0)  { TWOARG  crontab_del   (argv[++i]); }
      else if (strcmp(a, "--purge"   ) == 0)    crontab_proc  (my.who, 'p');
      else if (strcmp(a, "--cleanse" ) == 0)    crontab_proc  ("ALL" , 'p');
      /*---(user switches)---------------*/
      else if (strcmp(a, "-u"        ) == 0)  { TWOARG  crontab_user  (argv[++i]); }
      else if (strcmp(a, "--user"    ) == 0)  { TWOARG  crontab_user  (argv[++i]); }
      /*---(warnings)--------------------*/
      else if (strcmp(a, "-c"        ) == 0)    crontab_dir   ();
      else if (strcmp(a, "-e"        ) == 0)    crontab_edit  ();
      else if (strcmp(a, "-"         ) == 0)    crontab_stdin ();
      /*---(unknown)---------------------*/
      else    printf("requested action not understood or incomplete\n");
   }
   /*> if      (argc == 1)                                   crontab_stdin ();                          <* 
    *> else if (argc >= 2 && argv[1][0] != '-')              crontab_inst  (argv[1]);                   <* 
    *> else if (argc >= 2 && strncmp(a, "--system",10) == 0) printf("feature not implemented yet\n");   <* 
    *> else if (argc >= 2 && strncmp(a, "--test",  10) == 0) printf("feature not implemented yet\n");   <* 
    *> else    printf("requested action not understood or incomplete\n");                               <*/
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
   rc = setuid(my.uid);
   printf("\n");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   DEBUG_TOPS   yLOG_end     ();
   exit (0);
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
PROG_testfiles     (void)
{
   snprintf (my.name_pulser , 200, "%s%s", "/tmp/" , FILE_PULSE);
   snprintf (my.name_watcher, 200, "%s%s", "/tmp/" , FILE_WATCH);
   snprintf (my.name_locker , 200, "%s%s", "/tmp/" , FILE_LOCK);
   snprintf (my.name_exec   , 200, "%s%s", "/tmp/" , FILE_EXEC);
   snprintf (my.name_status , 200, "%s%s", "/tmp/" , FILE_STATUS);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
PROG_testquiet     (void)
{
   char       *x_args [1]  = { "khronos" };
   PROG_urgs      (1, x_args);
   PROG_init      ();
   PROG_whoami    ();
   PROG_testfiles ();
   PROG_args      (1, x_args);
   PROG_begin     ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
PROG_testloud      (void)
{
   char       *x_args [2]  = { "khronos", "@@kitchen"    };
   PROG_urgs      (2, x_args);
   PROG_init      ();
   PROG_whoami    ();
   PROG_testfiles ();
   PROG_args      (2, x_args);
   PROG_begin     ();
   return 0;
}



/*====================---------[[ end-of-code ]]----------====================*/
