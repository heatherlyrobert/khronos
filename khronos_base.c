/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


struct cACCESSOR my;
char       *args[20];
int         failed;

char        testing      = 'n';

char        strtest      [100];


char
BASE_init               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(clear everything)---------------*/
   rc = BASE_wrap ();
   DEBUG_INPT   yLOG_value   ("wrap"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create retired)-----------------*/
   my.f_ready = 'y';
   rc = FILE_create ("RETIRED", "n/a", -1);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get current file)---------------*/
   yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update retired flag)------------*/
   x_file->retired = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE__purge             (char a_type)
{
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   int         n           =    0;
   int         c           =    0;
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(take care of lines)-------------*/
   switch (a_type) {
   case 'A'  :   n = 0;  rc = LINE_purge_global  ();  break;
   case 'a'  :   n = 1;  rc = LINE_purge_global  ();  break;
   case 'p'  :   n = 1;  rc = LINE_prune_global  ();  break;
   case 'r'  :   n = 1;  rc = LINE_retire_global ();  break;
   }
   /*---(take out empty lists)-----------*/
   rc = yDLST_list_by_index (n, NULL, &x_file);
   while (rc >= 0 && x_file != NULL) {
      c  = yDLST_line_count (YDLST_LOCAL);
      DEBUG_INPT   yLOG_complex ("current"   , "%3dn, %-20.20s, %2d#", n, x_file->title, c);
      if (c == 0) {
         DEBUG_INPT  yLOG_note    ("no contents, deleting");
         rc = FILE_delete (x_file->title);
      } else {
         DEBUG_INPT  yLOG_note    ("with contents, skipping");
         ++n;
      }
      rc = yDLST_list_by_index (n, NULL, &x_file);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char BASE_wrap          (void)  { return BASE__purge ('A'); }
char BASE_purge         (void)  { return BASE__purge ('a'); }
char BASE_prune         (void)  { return BASE__purge ('p'); }
char BASE_retire        (void)  { return BASE__purge ('r'); }



char
BASE_execute            (void)
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
   IF_DAEMONY {
      rc  = base_daemon ();
      DEBUG_PROG  yLOG_value   ("daemon"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(startup)------------------------*/
   x_hour  = EXEC_time (0);
   rc      = RPTG_track_beg ();
   DEBUG_PROG  yLOG_value   ("x_hour"    , x_hour);
   rc = TRKS_import   ();
   DEBUG_PROG  yLOG_value   ("import"    , rc);
   rc = RPTG_status   ();
   DEBUG_PROG  yLOG_value   ("status"    , rc);
   /*> catchup();                                                                     <*/
   EXEC_wait_min ();
   DEBUG_PROG  yLOG_note    ("after wait");
   /*---(main loop)----------------------*/
   DEBUG_PROG  yLOG_note    ("entering main loop");
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
         TRKS_export ();
         RPTG_status ();
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
