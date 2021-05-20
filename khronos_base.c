/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


struct cACCESSOR my;
char       *args[20];
int         failed;

char        testing      = 'n';

char        strtest      [100];



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
   x_hour  = EXEC_time (0);
   rc      = RPTG_track_beg ();
   DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
   RPTG_minute   ();
   /*> catchup();                                                                     <*/
   EXEC_wait_min ();
   /*---(main loop)----------------------*/
   while (1) {
      x_hour = x_save = EXEC_time (0);
      DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
      rc     = EXEC_every_hour (x_hour);
      DEBUG_PROG  yLOG_value   ("hourly"    , rc);
      while (x_hour == x_save) {
         DEBUG_PROG  yLOG_value   ("minute"    , my.minute);
         rc     = EXEC_every_min (my.minute);
         DEBUG_PROG  yLOG_value   ("minutely"  , rc);
         x_hour = EXEC_time (0);
         DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
         RPTG_minute ();
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
   yEXEC_signal (YEXEC_HARD, YEXEC_NO, YEXEC_NO, EXEC_comm, "stdsig");
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


/*=============================[[ end-of-code ]]==============================*/
