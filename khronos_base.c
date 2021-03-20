/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


struct cACCESSOR my;
char       *args[20];
int         failed;

char        testing      = 'n';

char        strtest      [100];



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
   /*> rc = tabs_global  ("ALL", ACT_HUP);  /+ just in case a signal was missed +/    <*/
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
   /*> rc      = rptg_beg_watch ();                                                   <*/
   DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
   /*> tabs_global ("ALL", ACT_LOAD);                                                 <*/
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
base_comm          (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc)
{
   /*---(catch)--------------------------*/
   switch (a_signal) {
   case  SIGHUP:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGHUP MEANS REFRESH CRONTABS BEFORE NEXT RUN");
      my.resync = 'y';
      break;
   case  SIGTERM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGTERM means terminate daemon");
      /*> rptg_end_watch ("SIGTERM");                                                 <*/
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGSEGV:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGSEGV means daemon blew up");
      /*> rptg_end_watch ("SEGSEGV");                                                 <*/
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGABRT:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGABRT means daemon blew up");
      /*> rptg_end_watch ("SIGABRT");                                                 <*/
      yEXEC_term    ("EXITING", 99);
      break;
   default      :
      DEBUG_PROG  yLOG_info     ("SIGNAL", "unknown signal recieved");
      break;
   }
   /*---(complete)-----------------------*/
   return;
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
   /*---(check for user)-----------------*/
   DEBUG_ENVI   yLOG_value   ("my.m_uid"  , my.m_uid);
   --rce;  if (my.m_uid != 0) {
      printf ("only root can run khronos in daemon mode\n");
      DEBUG_ENVI   yLOG_note    ("only root can run khronos in daemon mode");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(fork off and die)---------------*/
   DEBUG_ENVI   yLOG_value   ("logger"    , yLOGS_lognum ());
   rc = yEXEC_daemon (yLOGS_lognum (), &(my.m_pid));
   DEBUG_ENVI   yLOG_value   ("daemon"    , rc);
   --rce;  if (rc < 0) {
      printf ("khronos could not be daemonized\n");
      DEBUG_ENVI   yLOG_note    ("khronos could not be daemonized");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(signals)-------------------------------*/
   yEXEC_signal (YEXEC_HARD, YEXEC_NO, YEXEC_NO, base_comm, "stdsig");
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
   strlcpy  (unit_answer, "BASE             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "central"       )  == 0) {
      sprintf (t, "[%s]", my.n_central);
      snprintf (unit_answer, LEN_HUND, "BASE central     : %2d%-38.38s  %c", strlen (my.n_central), t, my.alt_dir);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*=============================[[ end-of-code ]]==============================*/
