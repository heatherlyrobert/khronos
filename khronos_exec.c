/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


#define   MIN2SEC     60



/*====================------------------------------------====================*/
/*===----                       signal handling                        ----===*/
/*====================------------------------------------====================*/
static void      o___SIGNALS_________________o (void) {;};

void             /* [------] receive signals ---------------------------------*/
EXEC_comm          (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc)
{
   /*---(catch)--------------------------*/
   DEBUG_PROG  yLOG_value    ("signal", a_signal);
   switch (a_signal) {
   case  SIGHUP:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGHUP MEANS REFRESH CRONTABS BEFORE NEXT RUN");
      my.resync = 'y';
      break;
   case  SIGTERM:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGTERM means terminate daemon");
      RPTG_track_sig (a_signal, a_name, a_desc);
      RPTG_track_end ();
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGSEGV:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGSEGV means daemon blew up");
      RPTG_track_sig (a_signal, a_name, a_desc);
      RPTG_track_end ();
      yEXEC_term    ("EXITING", 99);
      break;
   case  SIGABRT:
      DEBUG_PROG  yLOG_info     ("SIGNAL", "SIGABRT means daemon blew up");
      RPTG_track_sig (a_signal, a_name, a_desc);
      RPTG_track_end ();
      yEXEC_term    ("EXITING", 99);
      break;
   default      :
      DEBUG_PROG  yLOG_info     ("SIGNAL", "unknown signal recieved");
      break;
   }
   /*---(complete)-----------------------*/
   return;
}

char
EXEC_linekill          (char *a_file, char *a_line, char a_sig)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_line"    , a_line);
   --rce;  if (a_line == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find list)----------------------*/
   rc = yDLST_list_by_name (a_file, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   if (x_file == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(find line)----------------------*/
   rc = yDLST_line_by_name (YDLST_LOCAL, a_line, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   if (x_line == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check rpid)---------------------*/
   DEBUG_INPT   yLOG_value   ("->rpid"    , x_line->rpid);
   --rce;  if (x_line->rpid <= 1) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(kill)---------------------------*/
   rc = kill (x_line->rpid, a_sig);
   DEBUG_INPT   yLOG_value   ("kill"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       marking executables                    ----===*/
/*====================------------------------------------====================*/
static void      o___MARKING_________________o (void) {;};

char
EXEC_mark_done          (char a_yexec, int a_return)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   int         x_mins      =    0;
   int         x_sec       =    0;
   int         x_msec      =    0;
   char        x_reason    =  '-';
   char        x_note      =  '-';
   /*---(get current)--------------------*/
   yDLST_active_by_cursor (YDLST_CURR, NULL, &x_line);
   --rce;  if (x_line      == NULL)  return rce;
   yDLST_line_list (NULL, &x_file);
   --rce;  if (x_file      == NULL)  return rce;
   /*---(duration)-----------------------*/
   x_sec  = my.now - x_line->start;
   if (x_sec  <  0)  x_sec  = 0;
   x_mins = x_sec / 60;
   x_msec = x_sec * 1000;
   if (x_msec <= 0)  x_msec = 1;
   /*---(log results)--------------------*/
   switch (a_yexec)  {
   case YEXEC_NOSUCH  : case YEXEC_NOTREAL : case YEXEC_NOCHMOD :
   case YEXEC_BADLOG  : case YEXEC_NOTEXEC : case YEXEC_NOPERM  :
      /*> if (x_line->c_badd < 99)  ++x_line->c_badd;                                 <*/
      x_reason = KHRONOS_BADD;
      break;
   case YEXEC_SEGV    : case YEXEC_USER    : case YEXEC_LIMIT   :
   case YEXEC_DIED    : case YEXEC_ERROR   :
      /*> if (x_line->c_boom < 99)  ++x_line->c_boom;                                 <*/
      x_reason = KHRONOS_BOOM;
      break;
   case YEXEC_KILLED  :
      if (x_line->force == 'g' && a_return == SIGTERM) {
         /*> if (x_line->c_shut < 99)  ++x_line->c_shut;                              <*/
         x_reason = KHRONOS_TERM;
      } else if (x_line->force == 'k' && a_return == SIGKILL) {
         /*> if (x_line->c_shut < 99)  ++x_line->c_shut;                              <*/
         x_reason = KHRONOS_TERM;
      } else {
         /*> if (x_line->c_kill < 99)  ++x_line->c_kill;                              <*/
         x_reason = KHRONOS_KILL;
      }
      break;
   case YEXEC_NORMAL  : case YEXEC_WARNING :
      if (x_line->force == 'g') {
         /*> if (x_line->c_shut < 99)  ++x_line->c_shut;                              <*/
         x_reason = KHRONOS_TERM;
      } else {
         /*> if (x_line->c_pass < 99)  ++x_line->c_pass;                              <*/
         x_reason = KHRONOS_PASS;
      }
      break;
   case YEXEC_FAILURE : default            :
      if (x_line->force == 'g') {
         /*> if (x_line->c_shut < 99)  ++x_line->c_shut;                              <*/
         x_reason = KHRONOS_TERM;
      } else {
         /*> if (x_line->c_fail < 99)  ++x_line->c_fail;                              <*/
         x_reason = KHRONOS_FAIL;
      }
      break;
   }
   TRKS_complete (x_line->trks, my.hour, my.minute, x_reason, x_mins, x_line->est_min / 60, x_line->est / 60, x_line->est_max / 60);
   /*---(early/late)---------------------*/
   if      (x_msec < x_line->est_min) {
      /*> if (x_line->c_earl < 99)   ++x_line->c_earl;                                <*/
      if (a_yexec == 'n')        a_yexec = '<';
      x_note   = KHRONOS_EARL;
   }
   else if (x_msec > x_line->est_max) {
      /*> if (x_line->c_late < 99)   ++x_line->c_late;                                <*/
      if (a_yexec == 'n')        a_yexec = '>';
      x_note   = KHRONOS_LATE;
   }
   /*---(set last data)------------------*/
   if (x_sec <= 0)  x_sec = 1;
   x_line->l_rpid     = x_line->rpid;
   x_line->l_beg      = x_line->start;
   x_line->l_end      = my.now;
   x_line->l_dur      = x_sec;
   x_line->l_yexec    = a_yexec;
   x_line->l_rc       = a_return;
   /*---(report complete)----------------*/
   RPTG_track_exec (x_file, x_line, x_reason, x_note);
   /*---(reset run data)-----------------*/
   x_line->rpid       =  0;
   x_line->start      =  0;
   x_line->force      = '-';
   /*---(reset lists)--------------------*/
   yDLST_active_off ();
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       timekeeping functions                  ----===*/
/*====================------------------------------------====================*/
static void      o___TIMEKEEP________________o (void) {;};

long         /*--> set the cron times ----------------------------------------*/
EXEC_time               (long a_now)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTIME      *x_broke     = NULL;
   int         x_year, x_month, x_day;
   char        t           [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   /*---(save)---------------------------*/
   x_year    = my.year;
   x_month   = my.month;
   x_day     = my.day;
   /*---(set time)-----------------------*/
   DEBUG_LOOP   yLOG_value   ("a_now"     , a_now);
   if (a_now > 0)  my.now  = a_now;
   else            my.now  = time (NULL);
   DEBUG_LOOP   yLOG_value   ("my.now"    , my.now);
   /*---(break it down)------------------*/
   my.clean  = my.now - (my.now % 3600);
   DEBUG_LOOP   yLOG_value   ("my.clean"  , my.clean);
   x_broke   = localtime (&my.now);
   /*---(save values)--------------------*/
   my.minute = x_broke->tm_min;
   my.hour   = x_broke->tm_hour;
   my.day    = x_broke->tm_mday;
   my.month  = x_broke->tm_mon   +   1;
   my.year   = x_broke->tm_year  - 100;
   /*---(heartbeat)----------------------*/
   rc = yEXEC_heartbeat (my.m_pid, my.now, NULL, my.n_heartbeat, my.heartbeat);
   DEBUG_LOOP   yLOG_value   ("yEXEC"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set the date)-------------------*/
   if (x_year != my.year || x_month != my.month || x_day != my.day) {
      ySCHED_date (my.year, my.month, my.day);
   }
   /*---(complete------------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return my.clean;
}

char         /*--> wait until next minute ------------------------------------*/
EXEC_wait_min           (void)
{
   /*---(design notes)-------------------*/
   /*
    *  always returns at :01 seconds after the minute, just-in-case.
    *  must handle signal interruptions by updating countdown.
    *
    *  may not be right if a signal takes 60+ seconds to handle.
    *  update check to look for status that it should be running, check that
    *  never launched and mark to catch-up if high value (not little stuff).
    *  will need this anyway for flexible launches.
    *
    */
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



/*====================------------------------------------====================*/
/*===----                          every hour                          ----===*/
/*====================------------------------------------====================*/
static void      o___HOUR____________________o (void) {;};

char
EXEC_every_hour         (int a_hour)
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
   /*---(reset focus)--------------------*/
   rc = EXEC_focus   ();
   DEBUG_LOOP  yLOG_value   ("focus"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
EXEC__focus_file        (tFILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tLINE      *x_line      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(header)----------------------*/
   DEBUG_INPT   yLOG_point   ("x_file"    , a_file);
   DEBUG_INPT   yLOG_info    ("->title"   , a_file->title);
   if (strcmp (a_file->title, "RETIRED") == 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check all lines)-------------*/
   DEBUG_INPT   yLOG_value   ("lines"     , yDLST_line_count (YDLST_LOCAL));
   rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
   --rce;  while (rc >= 0 && x_line != NULL) {
      /*---(header)-------------------*/
      DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
      DEBUG_INPT   yLOG_info    ("->title"   , x_line->tracker);
      DEBUG_INPT   yLOG_value   ("->rpid"    , x_line->rpid);
      /*---(test)---------------------*/
      DEBUG_INPT   yLOG_info    ("MY_RAW_3"  , ySCHED_raw (x_line->sched));
      rc = ySCHED_test (x_line->sched, my.hour, YSCHED_ANY, NULL);
      DEBUG_INPT  yLOG_value   ("ySCHED"    , rc);
      if (rc == 0) {
         DEBUG_LOOP   yLOG_note  ("not scheduled within current hour, SKIPPING");
      } else if (rc <  0) {
         DEBUG_LOOP   yLOG_note  ("can not test/update schedule, FATAL");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      } else {
         DEBUG_LOOP   yLOG_note  ("set focused and continue");
         yDLST_focus_on ();
         ++c;
      }
      /*---(next)---------------------*/
      rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DNEXT, NULL, &x_line);
      /*---(done)---------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}

int
EXEC_focus              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_value   ("my.hour"   , my.hour);
   /*---(clear all)----------------------*/
   rc = yDLST_focus_clearall ();
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check all files)----------------*/
   DEBUG_INPT   yLOG_value   ("files"     , yDLST_list_count ());
   rc = yDLST_list_by_cursor (YDLST_DHEAD, NULL, &x_file);
   while (rc >= 0 && x_file != NULL) {
      c += EXEC__focus_file (x_file);
      /*---(next)------------------------*/
      rc = yDLST_list_by_cursor (YDLST_DNEXT, NULL, &x_file);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                         every minute                         ----===*/
/*====================------------------------------------====================*/
static void      o___MINUTE__________________o (void) {;};

char
EXEC_every_min          (int a_min)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(break)--------------------------*/
   DEBUG_LOOP  yLOG_break();
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   DEBUG_LOOP  yLOG_value   ("a_min"     , a_min);
   EXEC_check    ();
   EXEC_dispatch (a_min);
   EXEC_wait_min ();
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 0;
}

int
EXEC_check              (void)
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
   char        x_reason    =  '-';
   char        x_note      =  '-';
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
      /*---(prepare)---------------------*/
      x_reason = '-';
      x_note   = '-';
      /*---(get file)--------------------*/
      yDLST_line_list (NULL, &x_file);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      if (x_file == NULL) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->title"   , x_file->title);
      /*---(check)-----------------------*/
      DEBUG_INPT   yLOG_info    ("tracker"   , x_line->tracker);
      snprintf (t, 200, "%-16.16s,%3d", x_file->title, x_line->recdno);
      DEBUG_INPT   yLOG_info    ("t"         , t);
      rc = yEXEC_verify (t, x_line->rpid, &x_return);
      DEBUG_INPT   yLOG_value   ("check"     , rc);
      /*---(if running)------------------*/
      if (rc == YEXEC_RUNNING) {
         DEBUG_INPT   yLOG_note    ("still running, next");
         x_dur  = (my.now - x_line->start) * 1000;
         if (x_dur < 0)  x_dur = 0;
         x_line->force = yEXEC_timing (x_line->rpid, x_line->strict, x_line->est_max, x_dur, 2 * 60 * 1000, 0);
      }
      /*---(if done)---------------------*/
      else {
         EXEC_mark_done (rc, x_return);
         ++c;
         DEBUG_INPT  yLOG_note    ("collected, next");
      }
      /*---(next)------------------------*/
      rc = yDLST_active_by_cursor (YDLST_NEXT, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}

char
EXEC__prep_files        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(walk files)---------------------*/
   rc = yDLST_list_by_cursor (YDLST_HEAD, NULL, &x_file);
   while (x_file != NULL && rc >= 0) {
      DEBUG_INPT   yLOG_info    ("title"     , x_file->title);
      x_file->valid = '·';
      rc = yDLST_list_by_cursor (YDLST_NEXT, NULL, &x_file);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
EXEC_dispatch           (int a_min)
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
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_value   ("a_min"     , a_min);
   if (a_min < 0)  a_min = my.minute;
   /*---(prep files)---------------------*/
   EXEC__prep_files ();
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
      DEBUG_INPT   yLOG_info    ("->tracker" , x_line->tracker);
      rc = ySCHED_test (x_line->sched, my.hour, a_min, NULL);
      DEBUG_INPT   yLOG_value   ("test"      , rc);
      if (rc <= 0) {
         if (rc == 0) {
            DEBUG_LOOP   yLOG_note  ("not scheduled on this minute");
         } else {
            DEBUG_LOOP   yLOG_note  ("BOOM, can not test");
         }
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
      /*---(check specialty)-------------*/
      if (x_line->tracker [0] == '.') {
         DEBUG_INPT  yLOG_info    ("control"   ,  x_line->tracker);
         if        (strcmp (x_line->tracker, ".valid"   ) == 0) {
            DEBUG_INPT  yLOG_note    ("validity record, .valid, turn on/active");
            x_file->valid = 'y';
         } else if (strcmp (x_line->tracker, ".retire"  ) == 0) {
            DEBUG_INPT  yLOG_note    ("validity record, .retire, turn off");
            x_file->valid = '·';
         } else if (strcmp (x_line->tracker, ".blackout") == 0) {
            DEBUG_INPT  yLOG_note    ("validity record, .blackout, turn off");
            x_file->valid = '·';
         } else {
            DEBUG_INPT  yLOG_note    ("control record not recognized");
         }
         rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      if (x_file->valid != 'y') {
         DEBUG_INPT  yLOG_note    ("file not currently valid/active");
         rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      /*---(check for running)-----------*/
      TRKS_launch (x_line->trks, my.hour, a_min);
      /*> if (x_line->c_runs < 99)  ++x_line->c_runs;                                 <*/
      DEBUG_INPT   yLOG_value   ("rpid"      , x_line->rpid);
      if (x_line->rpid > 1) {
         DEBUG_INPT  yLOG_note    ("already running, do not duplicate");
         /*> if (x_line->c_skip < 99)  ++x_line->c_skip;                              <*/
         RPTG_track_exec (x_file, x_line, KHRONOS_SKIP, '-');
         TRKS_complete (x_line->trks, my.hour, a_min, KHRONOS_SKIP, x_line->est_min, 0, x_line->est, x_line->est_max);
         rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      /*---(activate)--------------------*/
      yDLST_active_on ();
      ++c;
      /*---(run)-------------------------*/
      snprintf (t, 200, "%-16.16s,%3d", x_file->title, x_line->recdno);
      DEBUG_INPT   yLOG_info    ("t"         , t);
      DEBUG_INPT   yLOG_char    ("->track", x_line->track);
      sprintf (x_cmd, "%s", x_line->command);
      /*> if (x_line->track != 'y')    sprintf (x_cmd, "%s", x_line->command);          <* 
       *> else                         sprintf (x_cmd, "scythe %s", x_line->command);   <*/
      DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
      x_rpid = yEXEC_full (t, x_file->user, x_cmd, YEXEC_DASH, YEXEC_FULL, YEXEC_FORK, NULL);
      DEBUG_INPT   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <  0) {
         DEBUG_INPT  yLOG_note    ("line/process could not launch");
         RPTG_track_exec (x_file, x_line, KHRONOS_BADD, '-');
         x_line->rpid       = -2;
         /*> if (x_line->c_badd < 99)  ++x_line->c_badd;                              <*/
         TRKS_complete (x_line->trks, my.hour, a_min, KHRONOS_BADD, x_line->est_min, 0, x_line->est, x_line->est_max);
         rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
         continue;
      }
      /*---(update line)-----------------*/
      x_line->rpid       = x_rpid;
      x_line->start      = my.now;
      /*---(report)----------------------*/
      RPTG_track_exec (x_file, x_line, KHRONOS_RUN , '-');
      /*---(next)------------------------*/
      DEBUG_INPT  yLOG_note    ("launched, move to next");
      rc = yDLST_focus_by_cursor (YDLST_NEXT, NULL, &x_line);
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
   char        x_heartbeat [LEN_HUND];
   int         c           =    0;
   tLINE      *o           = NULL;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "EXEC             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "time"    )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC time        : %-10d, %02dy %02dm %02dd %02dh %02dm, %d", my.now, my.year, my.month, my.day, my.hour, my.minute, my.clean);
   }
   else if (strcmp (a_question, "pid"     )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC pid         : %d", my.m_pid);
   }
   else if (strcmp (a_question, "f_count" )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC f_count     : %d", yDLST_focus_count ());
   }
   else if (strcmp (a_question, "f_entry" )        == 0) {
      yDLST_focus_by_cursor (YDLST_DHEAD, NULL, &o);
      while (1) {
         if (o == NULL)  break;
         if (c == a_num) break;
         yDLST_focus_by_cursor (YDLST_DNEXT, NULL, &o);
         ++c;
      }
      if (o == NULL)   snprintf (unit_answer, LEN_HUND, "EXEC f_entry(%2d) : --/%2d  --åæ", a_num, yDLST_focus_count ());
      else             snprintf (unit_answer, LEN_HUND, "EXEC f_entry(%2d) : %2d/%2d  %2då%sæ", a_num, a_num, yDLST_focus_count (), strlen (ySCHED_raw (o->sched)), ySCHED_raw (o->sched));
   }
   else if (strcmp (a_question, "a_count" )        == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC a_count     : %d", yDLST_active_count ());
   }
   else if (strcmp (a_question, "heartbeat"     )  == 0) {
      snprintf (unit_answer, LEN_HUND, "EXEC heartbeat   : %2då%sæ", strlen (my.heartbeat), my.heartbeat);
   }
   else if (strcmp (a_question, "lastbeat"      )  == 0) {
      yEXEC_heartbeat_check (my.n_heartbeat, x_heartbeat);
      snprintf (unit_answer, LEN_HUND, "EXEC lastbeat    : %2då%sæ", strlen (x_heartbeat) , x_heartbeat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
exec__unit_kill         (int a_rpid)
{
}





