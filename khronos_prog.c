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
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   snprintf (my.n_central     , LEN_PATH, "%s"  , DIR_CENTRAL);
   snprintf (my.n_home        , LEN_PATH, "%s"  , "/home/");
   snprintf (my.n_root        , LEN_PATH, "%s"  , "/root");
   snprintf (my.n_heartbeat   , LEN_PATH, "%s%s", DIR_RUN  , FILE_HEARTBEAT);
   snprintf (my.n_track       , LEN_PATH, "%s%s", DIR_YHIST, FILE_TRACK);
   snprintf (my.n_status      , LEN_PATH, "%s%s", DIR_YLOG , FILE_STATUS);
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG__files_unit        (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   snprintf (my.n_central     , LEN_PATH, "%s%s", DIR_UNIT, "khronos/");
   snprintf (my.n_home        , LEN_PATH, "%s"  , DIR_UNIT);
   snprintf (my.n_root        , LEN_PATH, "%s%s", DIR_UNIT, "root");
   snprintf (my.n_heartbeat   , LEN_PATH, "%s%s", DIR_UNIT, FILE_HEARTBEAT);
   snprintf (my.n_track       , LEN_PATH, "%s%s", DIR_UNIT, FILE_TRACK);
   snprintf (my.n_status      , LEN_PATH, "%s%s", DIR_UNIT, FILE_STATUS);
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      startup functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char         /*--: pre-argument program initialization ---[ leaf   [ ------ ]-*/
PROG__init              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_home      [LEN_HUND]  = "";
   char       *p           = NULL;
   /*---(log header)---------------------*/
   DEBUG_TOPS   yLOG_info    ("oneline" , P_ONELINE);
   DEBUG_TOPS   yLOG_info    ("purpose" , P_PURPOSE);
   DEBUG_TOPS   yLOG_info    ("imagery" , P_IMAGERY);
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
   my.run_as   = IAM_KHRONOS;
   my.run_mode = ACT_NONE;
   strcpy (my.run_file, "");
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

char       /* PURPOSE : process the command line arguments -------------------*/
PROG__args              (int a_argc, char *a_argv[])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *a           = NULL;         /* current argument                    */
   char       *b           = NULL;          /* next argument                  */
   int         i           =    0;            /* loop iterator -- arguments          */
   int         len         =    0;            /* argument length                     */
   char        two_arg     =    0;
   int         x_args      =    0;          /* argument count                 */
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "command line arguments handling...");
   yURG_msg ('-', "total of %d arguments, including name", a_argc);
   /*---(program name)--------------------------*/
   strlcpy (my.m_prog, a_argv [0], LEN_DESC);
   DEBUG_TOPS   yLOG_info    ("prog name" , my.m_prog);
   /*---(check for no args)--------------*/
   DEBUG_TOPS   yLOG_value   ("a_argc"    , a_argc);
   if (a_argc == 1) {
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(walk args)-----------------------------*/
   for (i = 1; i < a_argc; ++i) {
      /*---(prepare)---------------------*/
      a = a_argv [i];
      if (a == NULL) {
         yURG_err ('f', "arg %d is NULL", i);
         DEBUG_TOPS   yLOG_note    ("FATAL, found a null argument, really bad news");
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      /*---(debugging--------------------*/
      if (a [0] == '@')       continue;
      /*---(two arg check)---------------*/
      ++x_args;
      DEBUG_ARGS  yLOG_info     ("argument"  , a);
      rc = yEXEC_args_handle (&(my.run_as), &(my.run_mode), my.run_file, &i, a, b);
      if (rc < 0)  break;
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
   DEBUG_INPT  yLOG_char    ("my.run_mode"     , my.run_mode);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] post-argument program initialization ------------*/
PROG__final             (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(set output routing)-------------*/
   yEXEC_final (my.m_uid);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_debugging          (int a_argc, char *a_argv[])
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
PROG_startup            (int a_argc, char *a_argv[])
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
   if (my.run_as == IAM_UKHRONOS)  PROG__unit_prepare ();
   DEBUG_TOPS  yLOG_value   ("startup"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       run-time routing                       ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

char
PROG_driver             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(route action)-------------------*/
   --rce;  switch (my.run_mode) {
   case ACT_VERIFY     : case ACT_CVERIFY    : case ACT_VVERIFY    :
      rc = yEXEC_act_verify  (my.run_as, my.run_mode, P_ONELINE, my.run_file, FILE_assimilate);
      break;
   case ACT_INSTALL    : case ACT_CINSTALL   : case ACT_VINSTALL   :
      rc = yEXEC_act_install (my.run_as, my.run_mode, P_ONELINE, my.run_file, FILE_assimilate, my.f_new);
      break;
   case ACT_CHECK      : case ACT_CCHECK     : case ACT_VCHECK     :
      rc = yEXEC_act_check   (my.run_as, my.run_mode, P_ONELINE, my.run_file, FILE_assimilate);
      break;
   case ACT_REMOVE     : case ACT_CREMOVE    : case ACT_VREMOVE    :
      rc = yEXEC_act_remove  (my.run_as, my.run_mode, P_ONELINE, my.run_file);
      break;
   case ACT_LIST       : case ACT_COUNT      :
      rc = yEXEC_act_review  (my.run_as, my.run_mode, P_ONELINE, my.m_user, my.m_uid, "*", FILE_assimilate);
      break;
   case ACT_AUDIT      : case ACT_CAUDIT     : case ACT_VAUDIT     :
      rc = yEXEC_act_review  (my.run_as, my.run_mode, P_ONELINE, my.m_user, my.m_uid, "*", FILE_assimilate);
      break;
   case ACT_DAEMON     : case ACT_CDAEMON    : case ACT_VDAEMON    :
      rc = yEXEC_act_review  (my.run_as, my.run_mode, P_ONELINE, my.m_user, my.m_uid, "*", FILE_assimilate);
      /* launch daemon, unless security breach or no crontabs */
      /*  damaged crontabs don't effect this one as they did not assimilate */
      break;
   case ACT_PRICKLY    : case ACT_CPRICKLY   : case ACT_VPRICKLY   :
      rc = yEXEC_act_review  (my.run_as, my.run_mode, P_ONELINE, my.m_user, my.m_uid, "*", FILE_assimilate);
      /* then, if passed perfectly, launch in daemon mode */
      /* ANYTHING damaged or funky caused a no launch     */
      break;
   case ACT_NORMAL     : case ACT_CNORMAL    : case ACT_VNORMAL    :
      rc = yEXEC_act_review  (my.run_as, my.run_mode, P_ONELINE, my.m_user, my.m_uid, "*", FILE_assimilate);
      /* then, run in foreground                          */
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      shutdown functions                      ----===*/
/*====================------------------------------------====================*/
static void      o___SHUTDOWN________________o (void) {;}

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
   PROG__unit_cleanup ();
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
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_HUND]  = "";
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "PROG             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "mode"          )  == 0) {
      yEXEC_iam  (my.run_as  , s);
      yEXEC_mode (my.run_mode, t);
      snprintf (unit_answer, LEN_HUND, "PROG mode        : (%c) %-18.18s, (%c) %-18.18s, å%sæ", my.run_as, s, my.run_mode, t, my.run_file);
   }
   else if (strcmp (a_question, "action"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG action      : %c  %c  %2då%sæ", my.run_as, my.run_mode, strlen (my.run_file), my.run_file);
   }
   else if (strcmp (a_question, "n_central"     )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG central     : %2då%sæ", strlen (my.n_central  ), my.n_central);
   }
   else if (strcmp (a_question, "n_home"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG home        : %2då%sæ", strlen (my.n_home     ), my.n_home);
   }
   else if (strcmp (a_question, "n_root"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG root        : %2då%sæ", strlen (my.n_root     ), my.n_root);
   }
   else if (strcmp (a_question, "n_heartbeat"   )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG heartbeat   : %2då%sæ", strlen (my.n_heartbeat), my.n_heartbeat);
   }
   else if (strcmp (a_question, "n_track"       )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG tracking    : %2då%sæ", strlen (my.n_track    ), my.n_track);
   }
   else if (strcmp (a_question, "n_status"      )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG status      : %2då%sæ", strlen (my.n_status   ), my.n_status);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
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
   yURG_mkdir (x_dir, "root"   , "root"  , "0755");
   sprintf (x_dir, "%skhronos" , DIR_UNIT);
   yURG_mkdir (x_dir, "root"   , "root"  , "0700");
   sprintf (x_dir, "%sroot"    , DIR_UNIT);
   yURG_mkdir (x_dir, "root"   , "root"  , "0700");
   sprintf (x_dir, "%smember"  , DIR_UNIT);
   yURG_mkdir (x_dir, "member" , "root"  , "0700");
   sprintf (x_dir, "%smachine" , DIR_UNIT);
   yURG_mkdir (x_dir, "machine", "root"  , "0700");
   sprintf (x_dir, "%smonkey"  , DIR_UNIT);
   yURG_mkdir (x_dir, "monkey" , "root"  , "0700");
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
   yURG_rmdir (DIR_UNIT);
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
   rc = PROG_debugging (x_argc, x_argv);
   rc = PROG_startup   (x_argc, x_argv);
   return rc;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   char        rc          =    0;
   int         x_argc      =    5;
   char       *x_argv [5]  = { "khronos_unit", "@@kitchen", "@@yparse", "@@ydlst", "@@ysched"  };
   rc = PROG_debugging (x_argc, x_argv);
   rc = PROG_startup   (x_argc, x_argv);
   return rc;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   PROG_end       ();
   return 0;
}



/*====================---------[[ end-of-code ]]----------====================*/
