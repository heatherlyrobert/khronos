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


char          unit_answer [ LEN_HUND ];



/*====================------------------------------------====================*/
/*===----                      utility routines                        ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}


/*> char                                                                                         <* 
 *> catchup       (void)                                                                         <* 
 *> {                                                                                            <* 
 *>    DEBUG_LOOP   yLOG_enter (__FUNCTION__);                                                   <* 
 *>    /+---(locals)--------------------------------+/                                           <* 
 *>    long      curr      = 0;                       /+ curr hour                +/             <* 
 *>    int       min       = 0;                       /+ curr minute              +/             <* 
 *>    /+---(init)----------------------------------+/                                           <* 
 *>    /+---(main loop)-----------------------------+/                                           <* 
 *>    DEBUG_LOOP   yLOG_note ("catchup specially flagged missed jobs");                         <* 
 *>    min       = ((my.last_end / 60) % 60) + 1;     /+ start right after last   +/             <* 
 *>    DEBUG_LOOP   yLOG_value ("first min", min);                                               <* 
 *>    /+> curr      = my.last_end - (my.last_end % (60 * 60));                           <+/    <* 
 *>    curr      = my.last_end;                                                                  <* 
 *>    while (curr < my.this_start) {                                                            <* 
 *>       /+> BASE_fast (curr);                           /+ id jobs for this hour    +/   <+/   <* 
 *>       /+---(cycle minutes)----------------------+/                                           <* 
 *>       while (min < 60 && curr <  my.this_start) {                                            <* 
 *>          DEBUG_LOOP   yLOG_value ("minute", min);                                            <* 
 *>          /+> BASE_dispatch (min);                                                     <+/    <* 
 *>          min      += 1;                                                                      <* 
 *>          curr     += 60;                                                                     <* 
 *>       }                                                                                      <* 
 *>       if (curr >=  my.this_start) break;                                                     <* 
 *>       min       = 0;                                                                         <* 
 *>    }                                                                                         <* 
 *>    DEBUG_LOOP   yLOG_value ("last min", min - 1);                                            <* 
 *>    /+---(reset)---------------------------------+/                                           <* 
 *>    /+---(complete)------------------------------+/                                           <* 
 *>    DEBUG_LOOP   yLOG_exit  (__FUNCTION__);                                                   <* 
 *>    return 0;                                                                                 <* 
 *> }                                                                                            <*/



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
   strlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strlcpy (t, "[cbang      ]", 15);
#else
   strlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char
PROG__files_normal      (void)
{
   snprintf (my.n_central     , LEN_PATH, "%s"  , DIR_CENTRAL);
   snprintf (my.n_home        , LEN_PATH, "%s"  , "/home/");
   snprintf (my.n_root        , LEN_PATH, "%s"  , "/root");
   snprintf (my.n_heartbeat   , LEN_PATH, "%s%s", DIR_RUN  , FILE_HEARTBEAT);
   snprintf (my.n_track       , LEN_PATH, "%s%s", DIR_YHIST, FILE_TRACK);
   snprintf (my.n_exec        , LEN_PATH, "%s%s", DIR_YLOG , FILE_EXEC);
   snprintf (my.n_status      , LEN_PATH, "%s%s", DIR_YLOG , FILE_STATUS);
   return 0;
}

char
PROG__files_unit        (void)
{
   snprintf (my.n_central     , LEN_PATH, "%s%s", DIR_UNIT, "crontabs/");
   snprintf (my.n_home        , LEN_PATH, "%s"  , DIR_UNIT);
   snprintf (my.n_root        , LEN_PATH, "%s%s", DIR_UNIT, "root");
   snprintf (my.n_heartbeat   , LEN_PATH, "%s%s", "/tmp/" , FILE_HEARTBEAT);
   snprintf (my.n_track       , LEN_PATH, "%s%s", "/tmp/" , FILE_TRACK);
   snprintf (my.n_exec        , LEN_PATH, "%s%s", "/tmp/" , FILE_EXEC);
   snprintf (my.n_status      , LEN_PATH, "%s%s", "/tmp/" , FILE_STATUS);
   return 0;
}

char         /*--: pre-argument program initialization ---[ leaf   [ ------ ]-*/
PROG_init          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_home      [LEN_HUND]  = "";
   char       *p           = NULL;
   /*---(log header)---------------------*/
   DEBUG_TOPS   yLOG_info    ("purpose" , "provide consistent, reliable, time-based job scheduling");
   DEBUG_TOPS   yLOG_info    ("namesake", "winged serpent khronos is the god of creation and unyielding time");
   DEBUG_TOPS   yLOG_info    ("khronos" , PROG_version    ());
   DEBUG_TOPS   yLOG_info    ("yLOG"    , yLOGS_version   ());
   DEBUG_TOPS   yLOG_info    ("yURG"    , yURG_version    ());
   DEBUG_TOPS   yLOG_info    ("yDLST"   , yDLST_version   ());
   DEBUG_TOPS   yLOG_info    ("yPARSE"  , yPARSE_version  ());
   DEBUG_TOPS   yLOG_info    ("ySCHED"  , ySCHED_version  ());
   DEBUG_TOPS   yLOG_info    ("yEXEC"   , yEXEC_version   ());
   DEBUG_TOPS   yLOG_info    ("ySTR"    , ySTR_version    ());
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(begin)--------------------------*/
   PROG__files_normal ();
   g_seq = 0;
   yURG_stderr ();
   my.user_mode = MODE_DAEMON;
   rc = yDLST_init ();
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("§");
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&(my.m_pid), &(my.m_ppid), &(my.m_uid), &my.m_root, &my.m_user, 'n');
   DEBUG_TOPS   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_TOPS   yLOG_value   ("m_pid"     , my.m_pid);
   DEBUG_TOPS   yLOG_value   ("m_ppid"    , my.m_ppid);
   DEBUG_TOPS   yLOG_value   ("m_uid"     , my.m_uid);
   DEBUG_TOPS   yLOG_info    ("m_user"    , my.m_user);
   /*---(get current path)---------------*/
   p = getcwd (my.m_path, LEN_PATH);
   DEBUG_TOPS   yLOG_spoint  (p);
   --rce;  if (p == NULL) {
      DEBUG_TOPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_TOPS   yLOG_info    ("m_path"    , my.m_path);
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
   DEBUG_TOPS   yLOG_value   ("argc"      , argc);
   if (argc == 1) {
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(program name)--------------------------*/
   strlcpy (my.m_prog, argv [0], LEN_DESC);
   DEBUG_TOPS   yLOG_value   ("prog name" , my.m_prog);
   /*---(process)------------------------*/
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (i < argc - 1) two_arg = 1; else two_arg = 0;
      len = strlen (a);
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
      else if (strcmp(a, "--list"    ) == 0)    tabs_global  (my.m_user, 'l');
      else if (strcmp(a, "--all"     ) == 0)    tabs_global  ("ALL" , 'l');
      else if (strcmp(a, "--here"    ) == 0)    tabs_local   (my.m_user, 'l');
      /*---(installing)------------------*/
      else if (a[0] != '-'           )          tabs_install (a);
      /*> else if (strcmp(a, "--test"    ) == 0)  { TWOARG  crontab_test  (argv[++i]); }   <*/
      else if (strcmp(a, "--reload"  ) == 0)    tabs_local   (my.m_user, 'i');
      /*---(removing)--------------------*/
      else if (strcmp(a, "-d"        ) == 0)  { TWOARG  tabs_delete   (argv[++i]); }
      else if (strcmp(a, "-r"        ) == 0)  { TWOARG  tabs_delete   (argv[++i]); }
      else if (strcmp(a, "--purge"   ) == 0)    tabs_global  (my.m_user, 'p');
      else if (strcmp(a, "--cleanse" ) == 0)    tabs_global  ("ALL" , 'p');
      /*---(user switches)---------------*/
      else if (strcmp(a, "-u"        ) == 0)  { TWOARG  tabs_user     (argv[++i]); }
      else if (strcmp(a, "--user"    ) == 0)  { TWOARG  tabs_user     (argv[++i]); }
      /*---(warnings)--------------------*/
      else if (strcmp(a, "-l"        ) == 0)    tabs_cat_stub   ();
      else if (strcmp(a, "-c"        ) == 0)    tabs_dir_stub   ();
      else if (strcmp(a, "-e"        ) == 0)    tabs_edit_stub  ();
      else if (strcmp(a, "-"         ) == 0)    tabs_stdin_stub ();
      /*---(for testing)-----------------*/
      else if (strcmp (a, "--who"    ) == 0) {
         printf ("launched as %s (%d)\n", my.m_user, my.m_uid);
         exit (0);
      }
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
   /*> rc = setuid(my.uid);                                                           <*/
   /*> printf("; fatal, program terminated\n");                                       <*/
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   exit (-1);
   return 0;
}

char
PROG_end           (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   yDLST_wrap ();
   /*> rc = setuid(my.uid);                                                           <*/
   /*> printf("\n");                                                                  <*/
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   DEBUG_TOPS   yLOGS_end    ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

/*> char*            /+ [------] unit test accessor ------------------------------+/      <* 
 *> PROG_getter        (char *a_question, int a_num)                                      <* 
 *> {                                                                                     <* 
 *>    /+---(locals)-------------------------+/                                           <* 
 *>    int       xfore  = 0, xback  = 0;  /+ counts to verify doubly-linked list +/       <* 
 *>    tCFILE   *xlist    = NULL;                                                         <* 
 *>    /+---(prepare)------------------------+/                                           <* 
 *>    strlcpy  (unit_answer, "PROG_getter      : question not understood", LEN_HUND);    <* 
 *>    /+---(pulser)-------------------------+/                                           <* 
 *>    if        (strncmp(a_question, "pulse_name"     , 20)   == 0) {                    <* 
 *>       snprintf (unit_answer, LEN_HUND, "PROG pulse name  : %.35s", my.name_pulser);   <* 
 *>    }                                                                                  <* 
 *>    /+---(complete)-----------------------+/                                           <* 
 *>    return unit_answer;                                                                <* 
 *> }                                                                                     <*/

char
PROG__unit_mkdir        (char *a_dir, char *a_own, char *a_perms)
{
   char        x_cmd       [LEN_RECD]  = "";
   sprintf (x_cmd, "rm -fr %s > /dev/null", a_dir);
   system  (x_cmd);
   sprintf (x_cmd, "mkdir %s > /dev/null", a_dir);
   system  (x_cmd);
   sprintf (x_cmd, "chown %s %s > /dev/null", a_own  , a_dir);
   system  (x_cmd);
   sprintf (x_cmd, "chmod %s %s > /dev/null", a_perms, a_dir);
   system  (x_cmd);
   return 0;
}

char
PROG__unit_prepare      (void)
{
   char        x_cmd       [LEN_RECD]  = "";
   char        x_dir       [LEN_RECD]  = "";
   PROG__files_unit ();
   chdir  ("/tmp");
   sprintf (x_dir, "%s", DIR_UNIT);
   PROG__unit_mkdir (x_dir, "root:users"    , "0777");
   sprintf (x_dir, "%s/crontabs", DIR_UNIT);
   PROG__unit_mkdir (x_dir, "root:root"     , "0700");
   sprintf (x_dir, "%s/root"    , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "root:root"     , "0700");
   sprintf (x_dir, "%s/member"  , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "member:root"   , "0700");
   sprintf (x_dir, "%s/machine" , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "machine:root"  , "0700");
   sprintf (x_dir, "%s/monkey"  , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "monkey:root"   , "0700");
   return 0;
}

char
PROG__unit_cleanup      (void)
{
   char        x_cmd       [LEN_RECD];
   chdir  ("/tmp");
   sprintf (x_cmd, "rm -fr %s > /dev/null", DIR_UNIT);
   system  (x_cmd);
   strlcpy (my.n_central, DIR_CENTRAL, LEN_PATH);
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   int         x_argc      = 1;
   char       *x_argv [1]  = { "khronos" };
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   PROG__unit_prepare ();
   yURG_noerror ();
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
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   PROG__unit_prepare ();
   yURG_noerror ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   PROG_final     ();
   my.user_mode = MODE_UNIT;
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   PROG__unit_cleanup ();
   PROG_end       ();
   return 0;
}



/*====================---------[[ end-of-code ]]----------====================*/
