/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


struct cACCESSOR my;
char       *args[20];
int         failed;

char        testing      = 'n';

char        strtest      [100];

#define     MAX_DURATION    10
#define     MAX_CONTROL     10

struct cDURATION {
   char       name         [15];
   int        dur_min;
   int        dur_exp;
   int        dur_max;
} durations [MAX_DURATION] = {
   /*"123456789012", 123, 123, 123  */
   { "-"           ,   0,   0,   5  },
   { "tiny"        ,   0,   0,   2  },
   { "small"       ,   0,   2,  10  },
   { "medium"      ,   1,   5,  30  },
   { "large"       ,   5,  15,  60  },
   { "huge"        ,  15,  30, 120  },
   { "---null---"  ,   0,   0,   0  },
   { "---null---"  ,   0,   0,   0  },
   { "---null---"  ,   0,   0,   0  },
   { "---null---"  ,   0,   0,   0  },
};
struct cCONTROL  {
   char       name         [15];
   char       importance;
   char       monitoring;
   char       catchup;
   char       busy_delay;
   char       busy_skip;
   char       busy_kill;
} controls [MAX_CONTROL] = {
   /*"123456789012", 123, 123, 123, 123, 123, 123  */
   /* name-------- , imp, mon, cat, del, ski, kil  */
   { "-"           , '0', '-', '-', 'y', 'y', 'y'  },
   { "could"       , '0', '-', '-', 'y', 'y', 'y'  },
   { "like"        , '1', 'y', '-', 'y', 'y', 'y'  },
   { "want"        , '2', 'y', '-', 'y', 'y', '-'  },
   { "need"        , '3', 'y', 'y', 'y', 'y', '-'  },
   { "must"        , '4', 'y', 'y', 'y', '-', '-'  },
   { "absolute"    , '5', 'y', 'y', '-', '-', '-'  },
   { "---null---"  ,   0, '-', '-', '-', '-', '-'  },
   { "---null---"  ,   0, '-', '-', '-', '-', '-'  },
   { "---null---"  ,   0, '-', '-', '-', '-', '-'  },
   /* name-------- , imp, mon, cat, del, ski, kil  */
};



char
base_every_min          (int a_min)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(break)--------------------------*/
   DEBUG_LOOP  yLOG_break();
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   DEBUG_LOOP  yLOG_value   ("a_min"     , a_min);
   /*> x_min     = BASE_timestamp();                                            <*/
   exec_check    ();
   exec_dispatch (a_min);
   /*> BASE_status   ();                                                        <*/
   exec_wait_min ();
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
base_every_hour         (int a_hour)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(break)--------------------------*/
   DEBUG_LOOP  yLOG_break();
   DEBUG_LOOP  yLOG_sync ();
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   DEBUG_LOOP  yLOG_note    ("hourly break -- check crontabs and reset fast list");
   DEBUG_LOOP  yLOG_value   ("a_hour"    , a_hour);
   /*---(just in case)-------------------*/
   rc = tabs_global  ("ALL", ACT_HUP);  /* just in case a signal was missed */
   DEBUG_LOOP  yLOG_value   ("global"    , rc);
   /*---(reset focus)--------------------*/
   rc = exec_focus   ();
   DEBUG_LOOP  yLOG_value   ("focus"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
base_daemon_mode        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   long        x_save      =    0;                       /* last hour ran            */
   long        x_hour      =    0;                       /* curr hour                */
   int         x_min       =    0;                       /* curr minute              */
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(daemonize)----------------------*/
   rc  = base_daemon ();
   DEBUG_PROG  yLOG_value   ("daemon"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(startup)------------------------*/
   x_hour  = exec_time (0);
   DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
   tabs_global ("ALL", ACT_LOAD);
   rptg_status ();
   /*> catchup();                                                                     <*/
   exec_wait_min ();
   /*---(main loop)----------------------*/
   while (1) {
      x_hour = x_save = exec_time (0);
      DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
      rc     = base_every_hour (x_hour);
      DEBUG_PROG  yLOG_value   ("hourly"    , rc);
      while (x_hour == x_save) {
         DEBUG_PROG  yLOG_value   ("minute"    , my.minute);
         rc     = base_every_min (my.minute);
         DEBUG_PROG  yLOG_value   ("minutely"  , rc);
         x_hour = exec_time (0);
         DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
         rptg_status ();
      }
   }
   /*---(complete------------------------*/
   DEBUG_PROG  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         housekeeping                         ----===*/
/*====================------------------------------------====================*/
static void      o___HOUSEKEEPING____________o (void) {;}

void             /* [------] receive signals ---------------------------------*/
base_comm          (int a_signal, siginfo_t *a_info, void *a_nada)
{
   /*---(catch)--------------------------*/
   switch (a_signal) {
   case  SIGHUP:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGHUP MEANS REFRESH CRONTABS BEFORE NEXT RUN");
      my.resync = 'y';
      break;
   case  SIGTERM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGTERM means terminate daemon");
      rptg_end_watch ();
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGSEGV:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGSEGV means daemon blew up");
      rptg_end_watch ();
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGABRT:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGABRT means daemon blew up");
      rptg_end_watch ();
      yEXEC_term    ("EXITING", 99);
      break;
   }
   /*---(complete)-----------------------*/
   return;
}

char
base_check_dir          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_len       =    0;
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   my.alt_dir = 'e';
   /*---(defenses)-----------------------*/
   DEBUG_ENVI   yLOG_snote   (my.dir_central);
   x_len = strlen (my.dir_central);
   DEBUG_ENVI   yLOG_sint    (x_len);
   if (x_len < 10) {
      DEBUG_ENVI   yLOG_snote   ("crondir too short (<10)");
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(existance)----------------------*/
   rc = lstat (my.dir_central, &s);
   DEBUG_ENVI   yLOG_sint    (rc);
   --rce;  if  (rc < 0) {
      DEBUG_ENVI   yLOG_snote   ("crondir not found");
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(directory)----------------------*/
   DEBUG_ENVI   yLOG_sint    (S_ISDIR (s.st_mode));
   --rce;  if  (!S_ISDIR (s.st_mode))  {
      DEBUG_ENVI   yLOG_snote   ("crondir not directory");
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ownership)----------------------*/
   DEBUG_ENVI   yLOG_sint    (s.st_uid);
   --rce;  if  (s.st_uid  != 0)  {
      DEBUG_ENVI   yLOG_snote   ("owner not root");
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(group)--------------------------*/
   DEBUG_ENVI   yLOG_sint    (s.st_gid);
   --rce;  if  (s.st_gid  != 0)  {
      DEBUG_ENVI   yLOG_snote   ("group not root");
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(permissions)--------------------*/
   DEBUG_ENVI   yLOG_sint    (s.st_mode & 00777);
   if  ((s.st_mode & 00777) != 00700)  {
      DEBUG_ENVI   yLOG_snote   ("permissions not 0700");
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(change to safe location)--------*/
   rc = chdir (my.dir_central);
   DEBUG_ENVI   yLOG_sint    (rc);
   if (rc < 0) {
      DEBUG_ENVI   yLOG_snote   ("could not move to central dir");
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(note alternate dir)-------------*/
   if (strcmp (my.dir_central, DIR_CENTRAL) != 0) {
      DEBUG_ENVI   yLOG_snote   ("using alternative dir");
      my.alt_dir = 'y';
   } else {
      DEBUG_ENVI   yLOG_snote   ("using alternative dir");
      my.alt_dir = '-';
   }
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*--> daemonize the program -------------------------------------*/
base_daemon        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   int         x_uid       =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(check for other)----------------*/
   x_running = yEXEC_find ("khronos", NULL);
   DEBUG_ENVI   yLOG_value   ("x_running" , x_running);
   --rce;  if (x_running > 1) {
      printf ("khronos already running in daemon mode\n");
      DEBUG_ENVI   yLOG_note    ("khronos already running in daemon mode");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_running = yEXEC_find ("khronos_debug", NULL);
   DEBUG_ENVI   yLOG_value   ("x_running" , x_running);
   --rce;  if (x_running > 1) {
      printf ("khronos already running in daemon mode\n");
      DEBUG_ENVI   yLOG_note    ("khronos already running in daemon mode");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for user)-----------------*/
   DEBUG_ENVI   yLOG_value   ("my.uid"    , my.uid);
   --rce;  if (my.uid != 0) {
      printf ("only root can run khronos in daemon mode\n");
      DEBUG_ENVI   yLOG_note    ("only root can run khronos in daemon mode");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(fork off and die)---------------*/
   DEBUG_ENVI   yLOG_value   ("logger"    , yURG_debug.logger);
   rc = yEXEC_daemon (yURG_debug.logger, &my.pid);
   DEBUG_ENVI   yLOG_value   ("daemon"    , rc);
   --rce;  if (rc < 0) {
      printf ("khronos could not be daemonized\n");
      DEBUG_ENVI   yLOG_note    ("khronos could not be daemonized");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(signals)-------------------------------*/
   yEXEC_signal (YEXEC_HARD, YEXEC_NO, YEXEC_NO, base_comm);
   DEBUG_ENVI   yLOG_value   ("signals"   , rc);
   --rce;  if (rc < 0) {
      printf ("khronos sigals could not be set properly\n");
      DEBUG_ENVI   yLOG_note    ("khronos signals cound not be set properly");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)------------------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
base__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_RECD];
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "BASE             : question not understood", LEN_TEXT);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "central"       )  == 0) {
      sprintf (t, "[%s]", my.dir_central);
      snprintf (unit_answer, LEN_TEXT, "BASE central     : %2d%-38.38s  %c", strlen (my.dir_central), t, my.alt_dir);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*=============================[[ end-of-code ]]==============================*/
