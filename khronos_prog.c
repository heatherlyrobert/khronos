/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"


/*> khronos-anileis (merciless time) primary batch-automation daemon                  <* 
 *> khronos --daemon --abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrs                  <*/

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
   ystrlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   ystrlcpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   ystrlcpy (t, "[cbang      ]", 15);
#else
   ystrlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char
PROG_vershow       (void)
{
   printf ("%s\n", PROG_version ());
   exit (0);
}


char
PROG__files_normal      (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   snprintf (my.n_central     , LEN_PATH, "%s"  , DIR_CENTRAL);
   snprintf (my.n_home        , LEN_PATH, "%s"  , "/home/");
   snprintf (my.n_root        , LEN_PATH, "%s"  , "/root");
   snprintf (my.n_heartbeat   , LEN_PATH, "%s%s", DIR_RUN  , FILE_HEARTBEAT);
   snprintf (my.n_activity    , LEN_PATH, "%s%s", DIR_HOME , FILE_ACTIVITY);
   snprintf (my.n_status      , LEN_PATH, "%s%s", DIR_HOME , FILE_STATUS);
   snprintf (my.n_trks        , LEN_PATH, "%s%s", DIR_HOME , FILE_TRACKERS);
   snprintf (my.n_usage       , LEN_PATH, "%s%s", DIR_HOME , FILE_USAGE);
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG__files_unit        (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   snprintf (my.n_central     , LEN_PATH, "%s%s", DIR_UNIT, "spool/khronos/");
   snprintf (my.n_home        , LEN_PATH, "%s%s", DIR_UNIT, "home/");
   snprintf (my.n_root        , LEN_PATH, "%s%s", DIR_UNIT, "root");
   snprintf (my.n_heartbeat   , LEN_PATH, "%s%s", DIR_UNIT, FILE_HEARTBEAT);
   snprintf (my.n_activity    , LEN_PATH, "%s%s", DIR_UNIT, FILE_ACTIVITY);
   snprintf (my.n_status      , LEN_PATH, "%s%s", DIR_UNIT, FILE_STATUS);
   snprintf (my.n_trks        , LEN_PATH, "%s%s", DIR_UNIT, FILE_TRACKERS);
   snprintf (my.n_usage       , LEN_PATH, "%s%s", DIR_UNIT, FILE_USAGE);
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char PROG_reset              (void) { return yJOBS_reset (&(my.run_as), &(my.run_mode), my.run_file); }



/*====================------------------------------------====================*/
/*===----                       pre-initialization                     ----===*/
/*====================------------------------------------====================*/
static void      o___PREINIT_________________o (void) {;}

char       /*----: very first setup ------------------s-----------------------*/
PROG__header            (void)
{
   /*---(header)----------------------*/
   DEBUG_PROG   yLOG_enter (__FUNCTION__);
   /*---(versioning)------------------*/
   DEBUG_PROG   yLOG_info     ("khronos" , PROG_version    ());
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_note     ("custom core");
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version      ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version     ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version      ());
   DEBUG_PROG   yLOG_note     ("custom other");
   DEBUG_PROG   yLOG_info     ("yPARSE"  , yPARSE_version    ());
   DEBUG_PROG   yLOG_info     ("yDLST"   , yDLST_version     ());
   DEBUG_PROG   yLOG_info     ("yREGEX"  , yREGEX_version    ());
   DEBUG_PROG   yLOG_note     ("job control");
   DEBUG_PROG   yLOG_info     ("ySCHED"  , ySCHED_version    ());
   DEBUG_PROG   yLOG_info     ("yEXEC"   , yEXEC_version     ());
   DEBUG_PROG   yLOG_info     ("yJOBS"   , yJOBS_version     ());
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
PROG_debugging          (int a_argc, char *a_argv [], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(clear)--------------------------*/
   yURG_all_tmp   ();
   /*---(start logger)-------------------*/
   rc = yURG_logger  (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process urgents)----------------*/
   rc = yURG_urgs    (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process urgents)----------------*/
   rc = PROG__header ();
   DEBUG_PROG   yLOG_value    ("header"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      startup functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char       /*----: very first setup ------------------s-----------------------*/
PROG__init              (int a_argc, char *a_argv [], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_home      [LEN_HUND]  = "";
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(yURG output control)------------*/
   /*> yURG_msg_none ();                                                              <*/
   /*> yURG_err_std  ();                                                              <*/
   /*> yURG_err_live ();                                                              <*/
   /*> yURG_err_none ();                                                              <*/
   /*> yURG_msg_none ();                                                              <*/
   yURG_err_mute ();
   yURG_msg_mute ();
   yURG_msg ('>', "initialization");
   ySCHED_date (11,  5, 14);    /* MUST FIX FIX FIX */
   /*> my.start = 0;                                                                  <*/
   /*> my.ucnt  = 0;                                                                  <*/
   /*> my.uavg  = 0.0;                                                                <*/
   /*> my.m_stime = my.m_utime = 0;                                                   <*/
   ystrlcpy (my.elapsed, "", LEN_HUND);
   ystrlcpy (my.usage  , "", LEN_HUND);
   my.actuals = '�';
   my.year = my.month = my.day = my.hour = my.minute = 0;;
   /*---(defaults)-----------------------*/
   my.run_as   = IAM_KHRONOS;
   my.run_mode = ACT_NONE;
   ystrlcpy (my.run_file, "", LEN_PATH);
   /*---(begin)--------------------------*/
   g_seq = 0;
   rc = yJOBS_runas (a_argv [0], &(my.run_as), P_FOCUS, P_NICHE, P_SUBJECT, P_PURPOSE, P_NAMESAKE, P_PRONOUNCE, P_HERITAGE, P_BRIEFLY, P_IMAGERY, P_REASON, P_ONELINE, P_HOMEDIR, P_BASENAME, P_FULLPATH, P_SUFFIX, P_CONTENT, P_SYSTEM, P_LANGUAGE, P_COMPILER, P_CODESIZE, P_DEPENDS, P_AUTHOR, P_CREATED, P_VERMAJOR, P_VERMINOR, P_VERNUM, P_VERTXT, P_DEFINE, P_POTENTIAL, P_SCOPE, P_EXAMPLE, P_TROUBLE, P_NEWMIND, P_SUMMARY, P_GREEK, NULL);
   DEBUG_PROG  yLOG_value   ("runas"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_char    ("run_as"    , my.run_as);
   if (my.run_as == IAM_KHRONOS)  PROG__files_normal ();
   else                           PROG__files_unit   ();
   rc = yDLST_init ();
   rc = yPARSE_config (YPARSE_MANUAL, NULL, YPARSE_ONETIME, YPARSE_FIELD, YPARSE_FILL);
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&(my.m_pid), &(my.m_ppid), &(my.m_uid), NULL, &my.m_root, &my.m_who, 'n', NULL, NULL, NULL);
   DEBUG_PROG   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG   yLOG_value   ("m_pid"     , my.m_pid);
   DEBUG_PROG   yLOG_value   ("m_ppid"    , my.m_ppid);
   DEBUG_PROG   yLOG_value   ("m_uid"     , my.m_uid);
   DEBUG_PROG   yLOG_info    ("m_who"     , my.m_who);
   /*---(get current path)---------------*/
   p = getcwd (my.m_path, LEN_PATH);
   DEBUG_PROG   yLOG_spoint  (p);
   --rce;  if (p == NULL) {
      DEBUG_PROG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG   yLOG_info    ("m_path"    , my.m_path);
   /*---(retirement list)----------------*/
   rc = BASE_init  ();
   DEBUG_PROG  yLOG_value   ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(purge existing trackers)--------*/
   rc = TRKS_purge ();
   DEBUG_PROG  yLOG_value   ("purge"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(usage)--------------------------*/
   USAGE_init ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : process the command line arguments -------------------*/
PROG__args              (int a_argc, char *a_argv [], char a_unit)
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
   int         x_max       =    0;
   char        x_noise     =  '-';
   /*---(begin)--------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "command line arguments handling...");
   yURG_msg ('-', "total of %d arguments, including name", a_argc);
   /*---(program name)--------------------------*/
   ystrlcpy (my.m_prog, a_argv [0], LEN_DESC);
   DEBUG_PROG   yLOG_info    ("prog name" , my.m_prog);
   /*---(check for no args)--------------*/
   DEBUG_PROG   yLOG_value   ("a_argc"    , a_argc);
   if (a_argc == 1) {
      DEBUG_PROG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare for trouble)-------------------*/
   x_noise = yJOBS_noise (a_argc, a_argv);
   DEBUG_ARGS  yLOG_char     ("x_noise"   , x_noise);
   /*---(walk args)-----------------------------*/
   for (i = 1; i < a_argc; ++i) {
      /*---(prepare)---------------------*/
      a = a_argv [i];
      DEBUG_ARGS  yLOG_info     ("argument"  , a);
      if (a == NULL) {
         yURG_err ('f', "arg %d is NULL", i);
         DEBUG_PROG   yLOG_note    ("FATAL, found a null argument, really bad news");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      DEBUG_ARGS  yLOG_info     ("extra"     , b);
      /*---(debugging--------------------*/
      if (a [0] == '@')       continue;
      /*---(two arg check)---------------*/
      ++x_args;
      rc = yJOBS_argument (&i, a, b, &(my.run_as), &(my.run_mode), my.run_file);
      DEBUG_ARGS  yLOG_value    ("rc"        , rc);
      if (rc > 0)  {
         DEBUG_PROG   yLOG_note    ("yJOBS handled option");
         continue;
      }
      if (rc == -1) {
         DEBUG_PROG   yLOG_note    ("yJOBS handled, but option not allowed");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      if (rc == -2) {
         DEBUG_PROG   yLOG_note    ("yJOBS handled option, but needs file");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      /*---(local args)------------------*/
      rc = 0;
      if      (strcmp  (a, "--defactual") == 0)    my.actuals = 'd';
      else if (strcmp  (a, "--plan"     ) == 0)    my.actuals = 'p';
      else if (strcmp  (a, "--unplan"   ) == 0)    my.actuals = 'u';
      else if (strcmp  (a, "--replan"   ) == 0)    my.actuals = 'r';
      else if (strcmp  (a, "--replan"   ) == 0)    my.actuals = 'r';
      else if (strcmp  (a, "--replan"   ) == 0)    my.actuals = 'r';
      else if (strncmp (a, "--abcdef", 8) == 0)    ;
      else if (strncmp (a, "--123456", 8) == 0)    ;
      else                                         rc = rce;
      /*---(bottom line)-----------------*/
      if (rc < 0) {
         DEBUG_PROG   yLOG_note    ("no yJOBS or local option found, bad");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      /*---(done)------------------------*/
   }
   /*---(max name)-----------------------*/
   IF_RUNNING {
      rc = yEXEC_maxname (a_argc, a_argv, &x_max);
      printf ("len = %d\n", x_max);
      ystrlcpy (a_argv [0], P_ONELINE, x_max);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return rc;
}

char             /* [------] post-argument program initialization ------------*/
PROG__begin             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_char    ("my.run_mode"     , my.run_mode);
   DEBUG_INPT  yLOG_char    ("my.run_as"       , my.run_as);
   /*> yJOBS_final (my.m_uid);                                                        <*/
   if (my.run_as == IAM_UKHRONOS)  PROG__unit_prepare ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_startup            (int a_argc, char *a_argv [], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)----------------------*/
   yURG_stage_check (YURG_BEG);
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = PROG__init   (a_argc, a_argv, a_unit);
   DEBUG_PROG   yLOG_value    ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(arguments)----------------------*/
   rc = PROG__args   (a_argc, a_argv, a_unit);
   DEBUG_PROG   yLOG_value    ("args"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(begin)--------------------------*/
   rc = PROG__begin  ();
   DEBUG_PROG   yLOG_value    ("begin"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   yURG_stage_check (YURG_MID);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        program shutdown                      ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

int        /* PURPOSE : main driver ------------------------------------------*/
PROG_main               (int a_argc, char *a_argv [], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        rc_final    =    0;
   long        x_save      =    0;                       /* last hour ran            */
   long        x_hour      =    0;                       /* curr hour                */
   int         x_min       =    0;                       /* curr minute              */
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(pre_startup)--------------------*/
   if (rc >= 0)  rc = PROG_debugging (a_argc, a_argv, a_unit);
   DEBUG_PROG  yLOG_value   ("debugging" , rc);
   if (rc >= 0)  rc = PROG_startup   (a_argc, a_argv, a_unit);
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   /*---(defense)------------------------*/
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      PROG_shutdown (a_unit);
      return rce;
   }
   /*---(main)---------------------------*/
   rc_final = yJOBS_driver (P_ONELINE, khronos_yjobs);
   DEBUG_PROG   yLOG_value    ("driver"    , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      PROG_shutdown (a_unit);
      return rce;
   }
   /*---(run)----------------------------*/
   IF_RUNNING {
      IF_NORMAL  yURG_msg ('>', "requested an actual run in NORMAL mode...");
      IF_STRICT  yURG_msg ('>', "requested an actual run in STRICT mode...");
      rc = BASE_execute ();
      DEBUG_PROG   yLOG_value    ("execute"   , rc);
      rc_final = rc;
   }
   DEBUG_PROG   yLOG_value    ("rc_final"  , rc_final);
   /*---(wrapup)-------------------------*/
   rc = PROG_shutdown  (a_unit);
   /*---(complete)-----------------------*/
   return rc_final;
}



/*====================------------------------------------====================*/
/*===----                        program shutdown                      ----===*/
/*====================------------------------------------====================*/
static void      o___SHUTDOWN________________o (void) {;}

char
PROG_term               (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   int   rc = 0;
   /*> rc = setuid(my.uid);                                                           <*/
   /*> printf("; fatal, program terminated\n");                                       <*/
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   exit (-1);
   return 0;
}

char       /*----: drive the program closure activities ----------------------*/
PROG__end               (char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(shutdown)--------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   if (a_unit != 'y')  yDLST_wrap ();
   /*> PROG__unit_cleanup ();                                                         <*/
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_shutdown           (char a_unit)
{
   /*---(stage-check)--------------------*/
   yURG_stage_check (YURG_END);
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   PROG__end (a_unit);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   DEBUG_PROG   yLOGS_end    ();
   return 0;
}


/*====================---------[[ end-of-code ]]----------====================*/
