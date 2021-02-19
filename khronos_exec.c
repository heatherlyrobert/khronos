/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


#define   MIN2SEC     60


/*====================------------------------------------====================*/
/*===----                         job handling                         ----===*/
/*====================------------------------------------====================*/
static void      o___EXECUTION_______________o (void) {;};

long         /*--> set the cron times ----------------------------------------*/
exec_time               (long a_now)
{
   /*---(locals)-----------+-----+-----+-*/
   tTIME      *x_broke     = NULL;
   int         x_year, x_month, x_day;
   char        t           [LEN_LABEL];
   /*---(save)---------------------------*/
   x_year    = my.year;
   x_month   = my.month;
   x_day     = my.day;
   /*---(set time)-----------------------*/
   if (a_now > 0)  my.now  = a_now;
   else            my.now  = time (NULL);
   /*---(break it down)------------------*/
   my.clean  = my.now - (my.now % 3600);
   x_broke   = localtime (&my.now);
   /*---(save values)--------------------*/
   my.minute = x_broke->tm_min;
   my.hour   = x_broke->tm_hour;
   my.day    = x_broke->tm_mday;
   my.month  = x_broke->tm_mon   +   1;
   my.year   = x_broke->tm_year  - 100;
   /*---(heartbeat)----------------------*/
   strftime (t, 20, "%y.%m.%d.%H.%M.%S", x_broke);
   sprintf  (my.heartbeat, "%s  %-10d  now  %d", t, my.now, my.m_pid);
   rptg_heartbeat ();
   /*---(set the date)-------------------*/
   if (x_year != my.year || x_month != my.month || x_day != my.day) {
      ySCHED_config_by_date (my.year, my.month, my.day);
   }
   /*---(complete------------------------*/
   return my.clean;
}

char         /*--> wait until next minute ------------------------------------*/
exec_wait_min           (void)
{
   /* this function is impemented to perform even when signals interrupt      */
   /*---(locals)-----------+-----+-----+-*/
   long        x_now       =    0;          /* present time             */
   long        x_inc       =    0;          /* seconds to sleep         */
   long        x_targ      =    0;          /* time to reach            */
   /*---(process)-------------------------------*/
   x_now = time (NULL);
   while (1) {
      x_inc   = MIN2SEC - (x_now % MIN2SEC) + 1;
      if (x_inc > MIN2SEC)         break;               /* solve SIGHUP after sleep */
      x_targ  = x_now + x_inc;
      DEBUG_LOOP   yLOG_value ("sleep (s)", x_inc);
      sleep (x_inc);
      x_now   = time (NULL);
      if (x_now >= x_targ)       break;               /* solve SIGHUP during sleep*/
   }
   /*---(complete)------------------------------*/
   return 0;
}

int
exec_focus              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(clear all)----------------------*/
   rc = yDLST_focus_clearall ();
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check all files)----------------*/
   DEBUG_INPT   yLOG_value   ("files"     , yDLST_list_count ());
   rc = yDLST_list_by_cursor (YDLST_HEAD, NULL, &x_file);
   while (rc >= 0 && x_file != NULL) {
      /*---(header)----------------------*/
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      DEBUG_INPT   yLOG_info    ("->title"   , x_file->title);
      /*---(check all lines)-------------*/
      DEBUG_INPT   yLOG_value   ("lines"     , yDLST_line_count (YDLST_LOCAL));
      rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_HEAD, NULL, &x_line);
      while (rc >= 0 && x_line != NULL) {
         /*---(header)-------------------*/
         DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
         DEBUG_INPT   yLOG_info    ("->title"   , x_line->tracker);
         DEBUG_INPT   yLOG_value   ("->rpid"    , x_line->rpid);
         /*---(test)---------------------*/
         if (ySCHED_test_by_time (&x_line->sched, my.hour, YSCHED_ANY) < 0) {
            DEBUG_LOOP   yLOG_note  ("not scheduled within current hour, SKIPPING");
            continue;
         }
         /*---(turn it on)---------------*/
         yDLST_focus_on ();
         ++c;
         DEBUG_LOOP   yLOG_note  ("set focused and continue");
         /*---(next)---------------------*/
         rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_NEXT, NULL, &x_line);
         /*---(done)---------------------*/
      }
      /*---(next)------------------------*/
      rc = yDLST_list_by_cursor (YDLST_NEXT, NULL, &x_file);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}

char
exec_dispatch           (int a_min)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_rpid      =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   int         c           =    0;
   char        t           [LEN_RECD];
   char        x_cmd       [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check count)--------------------*/
   DEBUG_INPT   yLOG_value   ("focused"   , yDLST_focus_count ());
   if (yDLST_focus_count () <= 0) {
      DEBUG_INPT  yLOG_note    ("no lines focused, nothing to do");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check all files)----------------*/
   rc = yDLST_focus_by_cursor (YDLST_HEAD, NULL, &x_line);
   while (rc >= 0 && x_line != NULL) {
      /*---(test)------------------------*/
      if (ySCHED_test_by_time (&x_line->sched, my.hour, a_min) < 0) {
         DEBUG_LOOP   yLOG_note  ("not scheduled on this minute");
         rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      /*---(get file)--------------------*/
      yDLST_line_list (NULL, &x_file);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      if (x_file == NULL) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->title"   , x_file->title);
      /*---(check for running)-----------*/
      ++x_line->attempts;
      DEBUG_INPT   yLOG_value   ("rpid"      , x_line->rpid);
      if (x_line->rpid > 1) {
         DEBUG_INPT  yLOG_note    ("already running, do not duplicate");
         ++x_line->overlaps;
         rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      /*---(activate)--------------------*/
      yDLST_active_on ();
      ++c;
      /*---(run)-------------------------*/
      snprintf (t, 200, "%-16.16s,%3d", x_file->title, x_line->recdno);
      DEBUG_INPT   yLOG_info    ("t"         , t);
      DEBUG_INPT   yLOG_info    ("->track", x_line->track);
      if (x_line->track != 'y')    sprintf (x_cmd, "%s", x_line->command);
      else                            sprintf (x_cmd, "scythe %s", x_line->command);
      DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
      x_rpid = yEXEC_full (t, x_file->user, x_cmd, YEXEC_DASH, YEXEC_FULL, YEXEC_FORK, my.name_exec);
      DEBUG_INPT   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <  0) {
         DEBUG_INPT  yLOG_note    ("could not launch");
         ++x_line->errors;
         x_line->rpid       = -2;
         rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      /*---(update line)-----------------*/
      x_line->rpid       = x_rpid;
      x_line->start      = my.now;
      DEBUG_INPT  yLOG_note    ("launched, move to next");
      /*---(next)------------------------*/
      rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}

int
exec_check              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_return    =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   int         c           =    0;
   char        t           [LEN_RECD];
   int         x_dur       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check count)--------------------*/
   DEBUG_INPT   yLOG_value   ("actives"   , yDLST_active_count ());
   if (yDLST_active_count () <= 0) {
      DEBUG_INPT  yLOG_note    ("no lines running/active, nothing to do");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check all files)----------------*/
   rc = yDLST_active_by_cursor (YDLST_HEAD, NULL, &x_line);
   while (rc >= 0 && x_line != NULL) {
      /*---(get file)--------------------*/
      yDLST_line_list (NULL, &x_file);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      if (x_file == NULL) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->title"   , x_file->title);
      /*---(check)-----------------------*/
      snprintf (t, 200, "%-16.16s,%3d", x_file->title, x_line->recdno);
      DEBUG_INPT   yLOG_info    ("t"         , t);
      rc = yEXEC_verify (t, x_line->rpid, &x_return);
      DEBUG_INPT   yLOG_value   ("check"     , rc);
      if (rc == YEXEC_RUNNING) {
         DEBUG_INPT   yLOG_note    ("still running, next");
         rc = yDLST_active_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      ++x_line->complete;
      /*---(log results)-----------------*/
      switch (rc) {
      case YEXEC_ERROR   :
         DEBUG_INPT  yLOG_note    ("error, failed to launch");
         --x_line->complete;
         ++x_line->errors;
         break;
      case YEXEC_KILLED  :
         DEBUG_INPT  yLOG_note    ("terminated early");
         ++x_line->kills;
         break;
      case YEXEC_NORMAL  :
         DEBUG_INPT  yLOG_note    ("normal ending");
         break;
      case YEXEC_WARNING :
         DEBUG_INPT  yLOG_note    ("ended ok, but with warning");
         break;
      case YEXEC_FAILURE :
         DEBUG_INPT  yLOG_note    ("ended badly");
         ++x_line->failures;
         break;
      }
      /*---(early/late)------------------*/
      x_dur              = (my.now - x_line->start) / 60;
      if      (x_dur < x_line->est_min)  ++x_line->earlies;
      else if (x_dur > x_line->est_max)  ++x_line->lates;
      /*---(set last data)---------------*/
      /*> x_line->last_rpid  = x_line->rpid;                                          <* 
       *> x_line->last_time  = x_line->start;                                         <* 
       *> x_line->last_rc    = x_return;                                              <* 
       *> x_line->last_dur   = x_dur;                                                 <*/
      /*---(reset run data)--------------*/
      x_line->rpid       =  0;
      x_line->start      =  0;
      /*---(reset lists)-----------------*/
      yDLST_active_off ();
      ++c;
      DEBUG_INPT  yLOG_note    ("collected, next");
      /*---(next)------------------------*/
      rc = yDLST_active_by_cursor (YDLST_NEXT, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
exec__unit              (char *a_question, int a_num)
{
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "EXEC             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "time"    )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC time        : %-10d, %02dy %02dm %02dd %02dh %02dm, %d", my.now, my.year, my.month, my.day, my.hour, my.minute, my.clean);
   }
   else if (strcmp (a_question, "pid"     )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC pid         : %d", my.m_pid);
   }
   else if (strcmp (a_question, "focused" )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC focused     : %d", yDLST_focus_count ());
   }
   else if (strcmp (a_question, "active"  )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC active      : %d", yDLST_active_count ());
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
exec__unit_kill         (int a_rpid)
{
}





