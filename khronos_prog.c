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
static char   s_act  = '-';
static char   s_file  [LEN_PATH] = "";



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
PROG_version            (void)
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
   snprintf (my.n_central     , LEN_PATH, "%s%s", DIR_UNIT, "khronos/");
   snprintf (my.n_home        , LEN_PATH, "%s"  , DIR_UNIT);
   snprintf (my.n_root        , LEN_PATH, "%s%s", DIR_UNIT, "root");
   snprintf (my.n_heartbeat   , LEN_PATH, "%s%s", "/tmp/" , FILE_HEARTBEAT);
   snprintf (my.n_track       , LEN_PATH, "%s%s", "/tmp/" , FILE_TRACK);
   snprintf (my.n_exec        , LEN_PATH, "%s%s", "/tmp/" , FILE_EXEC);
   snprintf (my.n_status      , LEN_PATH, "%s%s", "/tmp/" , FILE_STATUS);
   return 0;
}

char         /*--: pre-argument program initialization ---[ leaf   [ ------ ]-*/
PROG__init              (void)
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
   /*---(yURG output control)------------*/
   yURG_msg_none ();
   yURG_err_std  ();
   yURG_err_live ();
   ySCHED_config_by_date (11,  5, 14);    /* MUST FIX FIX FIX */
   /*---(begin)--------------------------*/
   PROG__files_normal ();
   g_seq = 0;
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
   /*---(retirement list)----------------*/
   rc = FILE_init  ();
   DEBUG_PROG  yLOG_value   ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG__setaction         (cchar a_act, cchar *a_file)
{
   s_act = a_act;
   strlcpy (s_file, a_file, LEN_PATH);
   /*> printf ("s_act = %c, s_file = %s\n", s_act, s_file);                           <*/
   return 0;
}

#define    TWOARG         if (two_arg == 1)
#define    ELSEONE(r)     else { yURG_err ('F', "action å%sæ requires a file name as an argument", a);  rc = r; }

char       /* PURPOSE : process the command line arguments -------------------*/
PROG__args              (int a_argc, char *a_argv[])
{
   /*---(locals)-------------------------*/
   char      rce       =  -10;
   char      rc        =    0;
   char     *a         = NULL;         /* current argument                    */
   int       i         = 0;            /* loop iterator -- arguments          */
   int       len       = 0;            /* argument length                     */
   char      two_arg   = 0;
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_TOPS   yLOG_value   ("a_argc"    , a_argc);
   if (a_argc == 1) {
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(program name)--------------------------*/
   strlcpy (my.m_prog, a_argv [0], LEN_DESC);
   DEBUG_TOPS   yLOG_info    ("prog name" , my.m_prog);
   /*---(defaults)------------------------------*/
   s_act      = '-';
   strlcpy (s_file, "", LEN_PATH);
   /*---(process)------------------------*/
   --rce;  for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      if (a == NULL) {
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_TOPS   yLOG_complex ("curr"      , "#%02d, %s", i, a);
      if (i < a_argc - 1) two_arg = 1; else two_arg = 0;
      len = strlen (a);
      /*---(skip debugging)--------------*/
      if      (a[0] == '@')                     continue;
      my.user_mode = MODE_USER;
      /*---(usage/help)------------------*/
      if      (strcmp (a, "--version"      ) == 0 || strcmp (a, "-V") == 0) {
         PROG_version ();
         printf ("%s\n", verstring);
         DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      else if (strcmp (a, "--help"     ) == 0)    PROG__usage();
      /*---(local)-----------------------*/
      else if (strcmp (a, "--verify"   ) == 0)  { TWOARG  PROG__setaction (ACT_VERIFY  , a_argv [++i]); ELSEONE(-1); }
      else if (strcmp (a, "--cverify"  ) == 0)  { TWOARG  PROG__setaction (ACT_CVERIFY , a_argv [++i]); ELSEONE(-2); }
      else if (strcmp (a, "--vverify"  ) == 0)  { TWOARG  PROG__setaction (ACT_VVERIFY , a_argv [++i]); ELSEONE(-3); }
      /*---(incomming)-------------------*/
      else if (strcmp (a, "--install"  ) == 0)  { TWOARG  PROG__setaction (ACT_INSTALL , a_argv [++i]); ELSEONE(-4); }
      else if (strcmp (a, "--cinstall" ) == 0)  { TWOARG  PROG__setaction (ACT_CINSTALL, a_argv [++i]); ELSEONE(-5); }
      else if (strcmp (a, "--vinstall" ) == 0)  { TWOARG  PROG__setaction (ACT_VINSTALL, a_argv [++i]); ELSEONE(-6); }
      /*---(outgoing)--------------------*/
      else if (strcmp (a, "--remove"   ) == 0)  { TWOARG  PROG__setaction (ACT_REMOVE  , a_argv [++i]); ELSEONE(-7); }
      else if (strcmp (a, "--cremove"  ) == 0)  { TWOARG  PROG__setaction (ACT_CREMOVE , a_argv [++i]); ELSEONE(-8); }
      else if (strcmp (a, "--vremove"  ) == 0)  { TWOARG  PROG__setaction (ACT_VREMOVE , a_argv [++i]); ELSEONE(-9); }
      /*---(central)---------------------*/
      else if (strcmp (a, "--count"    ) == 0)  { PROG__setaction (ACT_COUNT   , ""); }
      else if (strcmp (a, "--list"     ) == 0)  { PROG__setaction (ACT_LIST    , ""); }
      else if (strcmp (a, "--check"    ) == 0)  { TWOARG  PROG__setaction (ACT_CHECK   , a_argv [++i]); ELSEONE(-10); }
      else if (strcmp (a, "--ccheck"   ) == 0)  { TWOARG  PROG__setaction (ACT_CCHECK  , a_argv [++i]); ELSEONE(-11); }
      else if (strcmp (a, "--vcheck"   ) == 0)  { TWOARG  PROG__setaction (ACT_VCHECK  , a_argv [++i]); ELSEONE(-12); }
      /*---(auditing)--------------------*/
      else if (strcmp (a, "--audit"    ) == 0)  { PROG__setaction (ACT_AUDIT   , ""); }
      else if (strcmp (a, "--caudit"   ) == 0)  { PROG__setaction (ACT_CAUDIT  , ""); }
      else if (strcmp (a, "--vaudit"   ) == 0)  { PROG__setaction (ACT_VAUDIT  , ""); }
      /*---(speciality)------------------*/
      else if (strcmp (a, "--reload"   ) == 0)  { PROG__setaction (ACT_RELOAD, "");     }
      /*---(unknown)---------------------*/
      else  {
         yURG_err ('F', "requested action å%sæ not understood or incomplete", a);
         rc = -99;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}

char       /* PURPOSE : display usage help information -----------------------*/
PROG__usage             (void)
{
   printf ("see man pages for a better understanding of khronos...\n");
   printf ("  man 1 khronos      command-line initiation, use, and options\n");
   printf ("  man 5 khronos      structure of config, files, and streams\n");
   printf ("  man 7 khronos      decision rationale, scope, and objectives\n");
   exit   (0);
}

char             /* [------] post-argument program initialization ------------*/
PROG__begin             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_char    ("s_act"     , s_act);
   /*---(set output routing)-------------*/
   switch (s_act) {
   case ACT_VERIFY   : case ACT_INSTALL  : case ACT_REMOVE   :
   case ACT_AUDIT    : case ACT_CHECK    : case ACT_LIST     :
      DEBUG_INPT  yLOG_note    ("silent, turning off all output and errors");
      yURG_msg_std  (); yURG_msg_mute ();
      yURG_err_std  (); yURG_err_mute ();
      break;
   case ACT_CVERIFY  : case ACT_CINSTALL : case ACT_CREMOVE  :
   case ACT_CAUDIT   : case ACT_CCHECK   :
      DEBUG_INPT  yLOG_note    ("confirm, muting output and errors until summary");
      yURG_msg_std  (); yURG_msg_mute ();
      yURG_err_std  (); yURG_err_mute ();
      break;
   case ACT_VVERIFY  : case ACT_VINSTALL : case ACT_VREMOVE  :
   case ACT_VAUDIT   : case ACT_VCHECK   :
      DEBUG_INPT  yLOG_note    ("verbose, turning on all output and errors");
      yURG_msg_std  (); yURG_msg_live ();
      yURG_err_std  (); yURG_err_live ();
      break;
   }
   /*---(check security)-----------------*/
   --rce;  switch (s_act) {
   case ACT_AUDIT      : case ACT_CAUDIT     : case ACT_VAUDIT     :
      DEBUG_INPT  yLOG_note    ("audit, caudit, and vaudit require root privlege");
      if (my.m_uid != 0) {
         yURG_err ('F', "--audit, --caudit, and --vaudit require root privlege");
         DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] post-argument program initialization ------------*/
PROG__final             (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_debugging          (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   DEBUG_TOPS  yLOG_value   ("debugging" , rc);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
PROG_startup            (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = PROG__init   ();
   if (rc >= 0)  rc = PROG__args   (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__begin  ();
   if (rc >= 0)  rc = PROG__final  ();
   if (a_unit == 'y') {
      my.user_mode = MODE_UNIT;
      PROG__unit_prepare ();
      yURG_msg_tmp ();
      yURG_err_tmp ();
   }
   if (a_unit == 'k') {
      yURG_msg_tmp ();
      yURG_err_tmp ();
   }
   DEBUG_TOPS  yLOG_value   ("startup"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
PROG_driver             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(route action)-------------------*/
   --rce;  switch (s_act) {
   case ACT_VERIFY     : case ACT_CVERIFY    : case ACT_VVERIFY    :
      rc = TABS_verify   (s_act, s_file);
      break;
   case ACT_INSTALL    : case ACT_CINSTALL   : case ACT_VINSTALL   :
      rc = TABS_install  (s_act, s_file);
      break;
   case ACT_CHECK      : case ACT_CCHECK     : case ACT_VCHECK     :
      rc = TABS_check    (s_act, s_file);
      break;
   case ACT_REMOVE     : case ACT_CREMOVE    : case ACT_VREMOVE    :
      rc = TABS_remove   (s_act, s_file);
      break;
   case ACT_LIST       : case ACT_COUNT      :
      rc = TABS_review  ("*", s_act);
      break;
   case ACT_AUDIT      : case ACT_CAUDIT     : case ACT_VAUDIT     :
      rc = TABS_review  ("*", s_act);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
PROG_term               (void)
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
PROG_end                (void)
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

char*            /*--> unit test accessor ------------------------------*/
prog__unit              (char *a_question)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "PROG             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "action"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG action      : %c  %2d å%sæ", s_act, strlen (s_file), s_file);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
PROG__unit_mkdir        (char *a_dir, char *a_own, char *a_perms)
{
   char        x_cmd       [LEN_RECD]  = "";
   /*> sprintf (x_cmd, "rm -fr %s > /dev/null", a_dir);                               <* 
    *> system  (x_cmd);                                                               <*/
   sprintf (x_cmd, "mkdir %s    > /dev/null  2>&1", a_dir);
   system  (x_cmd);
   sprintf (x_cmd, "chown %s %s > /dev/null  2>&1", a_own  , a_dir);
   system  (x_cmd);
   sprintf (x_cmd, "chmod %s %s > /dev/null  2>&1", a_perms, a_dir);
   system  (x_cmd);
   return 0;
}

char
PROG__unit_prepare      (void)
{
   char        x_cmd       [LEN_RECD]  = "";
   char        x_home      [LEN_PATH];
   char        x_dir       [LEN_RECD]  = "";
   PROG__files_unit ();
   /*---(directories)--------------------*/
   getcwd (x_home, LEN_PATH);
   chdir  ("/tmp");
   sprintf (x_dir, "%s", DIR_UNIT);
   PROG__unit_mkdir (x_dir, "root:users"    , "0777");
   sprintf (x_dir, "%skhronos" , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "root:root"     , "0700");
   sprintf (x_dir, "%sroot"    , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "root:root"     , "0700");
   sprintf (x_dir, "%smember"  , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "member:root"   , "0700");
   sprintf (x_dir, "%smachine" , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "machine:root"  , "0700");
   sprintf (x_dir, "%smonkey"  , DIR_UNIT);
   PROG__unit_mkdir (x_dir, "monkey:root"   , "0700");
   chdir  (x_home);
   /*---(complete)-----------------------*/
   return 0;
}

char
PROG__unit_cleanup      (void)
{
   char        x_cmd       [LEN_RECD];
   char        x_home      [LEN_PATH];
   /*---(directories)--------------------*/
   getcwd (x_home, LEN_PATH);
   chdir  ("/tmp");
   sprintf (x_cmd, "rm -fr %s   > /dev/null  2>&1", DIR_UNIT);
   system  (x_cmd);
   strlcpy (my.n_central, DIR_CENTRAL, LEN_PATH);
   chdir  (x_home);
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   char        rc          =    0;
   int         x_argc      =    1;
   char       *x_argv [1]  = { "khronos" };
   rc = PROG_debugging (x_argc, x_argv, 'y');
   rc = PROG_startup   (x_argc, x_argv, 'y');
   return rc;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   char        rc          =    0;
   int         x_argc      =    5;
   char       *x_argv [5]  = { "khronos_unit", "@@kitchen", "@@yparse", "@@ydlst", "@@ysched"  };
   rc = PROG_debugging (x_argc, x_argv, 'y');
   rc = PROG_startup   (x_argc, x_argv, 'y');
   return rc;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   PROG__unit_cleanup ();
   PROG_end       ();
   return 0;
}



/*====================---------[[ end-of-code ]]----------====================*/
