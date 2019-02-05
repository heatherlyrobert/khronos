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
   sprintf  (my.heartbeat, "%s  %-10d  now  %d", t, my.now, my.pid);
   rptg_heartbeat ();
   /*---(set the date)-------------------*/
   if (x_year != my.year || x_month != my.month || x_day != my.day) {
      ySCHED_setdate (my.year, my.month, my.day);
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
   for (x_file = yDLST_list_seek (YDLST_HEAD); x_file != NULL; x_file = yDLST_list_seek (YDLST_NEXT)) {
      /*---(header)----------------------*/
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      DEBUG_INPT   yLOG_info    ("->title"   , x_file->title);
      DEBUG_INPT   yLOG_char    ("->retire"  , x_file->retire);
      /*---(check for retired)-----------*/
      if (x_file->retire != '-') {
         DEBUG_INPT   yLOG_note    ("file retired, SKIPPING");
         continue;
      }
      /*---(check all lines)-------------*/
      DEBUG_INPT   yLOG_value   ("lines"     , yDLST_line_count ());
      for (x_line = yDLST_line_seek (YDLST_HEAD); x_line != NULL; x_line = yDLST_line_seek (YDLST_NEXT)) {
         /*---(header)-------------------*/
         DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
         DEBUG_INPT   yLOG_info    ("->title"   , x_line->tracker);
         DEBUG_INPT   yLOG_value   ("->retire"  , x_line->rpid);
         /*---(test)---------------------*/
         if (ySCHED_test (&x_line->sched, my.hour, ySCHED_ANY) < 0) {
            DEBUG_LOOP   yLOG_note  ("not scheduled within current hour, SKIPPING");
            continue;
         }
         /*---(turn it on)---------------*/
         yDLST_focus_on ();
         ++c;
         DEBUG_LOOP   yLOG_note  ("set focused and continue");
         /*---(done)---------------------*/
      }
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
   for (x_line = yDLST_focus_seek (YDLST_HEAD); x_line != NULL; x_line = yDLST_focus_seek (YDLST_NEXT)) {
      /*---(test)------------------------*/
      if (ySCHED_test (&x_line->sched, my.hour, a_min) < 0) {
         DEBUG_LOOP   yLOG_note  ("not scheduled on this minute");
         continue;
      }
      /*---(get file)--------------------*/
      x_file = yDLST_line_list ();
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
         continue;
      }
      /*---(activate)--------------------*/
      yDLST_active_on ();
      ++c;
      /*---(run)-------------------------*/
      snprintf (t, 200, "%-16.16s,%3d", x_file->title, x_line->recdno);
      DEBUG_INPT   yLOG_info    ("t"         , t);
      DEBUG_INPT   yLOG_info    ("->tracking", x_line->tracking);
      if (x_line->tracking != 'y')    sprintf (x_cmd, "%s", x_line->command);
      else                            sprintf (x_cmd, "scythe %s", x_line->command);
      DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
      x_rpid = yEXEC_run (my.name_exec, t, x_file->user, x_cmd, SHELL, PATH, yEXEC_FORK);
      DEBUG_INPT   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <  0) {
         DEBUG_INPT  yLOG_note    ("could not launch");
         ++x_line->errors;
         x_line->rpid       = -2;
         continue;
      }
      /*---(update line)-----------------*/
      x_line->rpid       = x_rpid;
      x_line->start      = my.now;
      DEBUG_INPT  yLOG_note    ("launched, move to next");
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
   for (x_line = yDLST_active_seek (YDLST_HEAD); x_line != NULL; x_line = yDLST_active_seek (YDLST_NEXT)) {
      /*---(get file)--------------------*/
      x_file = yDLST_line_list ();
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      if (x_file == NULL) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->title"   , x_file->title);
      /*---(check)-----------------------*/
      snprintf (t, 200, "%-16.16s,%3d", x_file->title, x_line->recdno);
      DEBUG_INPT   yLOG_info    ("t"         , t);
      rc = yEXEC_check (t, x_line->rpid, &x_return);
      DEBUG_INPT   yLOG_value   ("check"     , rc);
      if (rc == YEXEC_RUNNING) {
         DEBUG_INPT   yLOG_note    ("still running, next");
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
      if      (x_dur < x_line->dur_min)  ++x_line->earlies;
      else if (x_dur > x_line->dur_max)  ++x_line->lates;
      /*---(set last data)---------------*/
      x_line->last_rpid  = x_line->rpid;
      x_line->last_time  = x_line->start;
      x_line->last_rc    = x_return;
      x_line->last_dur   = x_dur;
      /*---(reset run data)--------------*/
      x_line->rpid       =  0;
      x_line->start      =  0;
      /*---(reset lists)-----------------*/
      yDLST_active_off ();
      ++c;
      DEBUG_INPT  yLOG_note    ("collected, next");
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}


/*> char        /+ PURPOSE : run through the fast list to find what should run    +/            <* 
 *> BASE_dispatch      (cint a_min)                                                             <* 
 *> {                                                                                           <* 
 *>    DEBUG_LOOP   yLOG_enter (__FUNCTION__);                                                  <* 
 *>    /+---(locals)--------------------------------+/                                          <* 
 *>    int       njobs = 0;                      /+ number of jobs ready to run   +/            <* 
 *>    int       ntest = 0;                      /+ number of jobs ready to run   +/            <* 
 *>    tCLINE   *x_line;                         /+ current line                  +/            <* 
 *>    char      msg[200];                                                                      <* 
 *>    /+---(process all time periods)--------------+/                                          <* 
 *>    DEBUG_LOOP   yLOG_value ("total"    ,  n_cline);                                         <* 
 *>    DEBUG_LOOP   yLOG_value ("potential",  nfast);                                           <* 
 *>    for (x_line = fasthead; x_line != NULL; x_line = x_line->fnext) {                        <* 
 *>       /+> if (my.silent == 'y') {                                                     <*    <* 
 *>        *>    switch (x_line->recovery) {                                              <*    <* 
 *>        *>    case '-' : continue;  break;                                             <*    <* 
 *>        *>    case 'x' : continue;  break;                                             <*    <* 
 *>        *>    }                                                                        <*    <* 
 *>        *> }                                                                           <+/   <* 
 *>       ++ntest;                                                                              <* 
 *>       snprintf(msg, 200, "%-16.16s,%3d", x_line->file->name, x_line->recd);                 <* 
 *>       if (x_line->sched.min[a_min]  == '_') {                                               <* 
 *>          if (my.silent == 'n') {                                                            <* 
 *>             yLOG_senter ("not");                                                            <* 
 *>             yLOG_snote(msg);                                                                <* 
 *>             yLOG_snote  ("not sched for min");                                              <* 
 *>             yLOG_sexit  ("not");                                                            <* 
 *>          }                                                                                  <* 
 *>          continue;                                                                          <* 
 *>       }                                                                                     <* 
 *>       if (x_line->rpid        != 0) {                                                       <* 
 *>          if (my.silent == 'n') {                                                            <* 
 *>             yLOG_senter ("not");                                                            <* 
 *>             yLOG_snote(msg);                                                                <* 
 *>             yLOG_snote  ("already running");                                                <* 
 *>             yLOG_sexit  ("not");                                                            <* 
 *>          }                                                                                  <* 
 *>          continue;                                                                          <* 
 *>       }                                                                                     <* 
 *>       run (x_line->file, x_line);                                                           <* 
 *>       ++njobs;                                                                              <* 
 *>    }                                                                                        <* 
 *>    DEBUG_LOOP   yLOG_value ("tested", ntest);                                               <* 
 *>    DEBUG_LOOP   yLOG_value ("ran",    njobs);                                               <* 
 *>    DEBUG_LOOP   yLOG_exit  (__FUNCTION__);                                                  <* 
 *>    /+---(complete)------------------------------+/                                          <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char        /+ PURPOSE : create list of jobs to run in a particular period    +/         <* 
 *> BASE_fast     (clong a_start)                /+ time_t of starting hour       +/         <* 
 *> {                                                                                        <* 
 *>    DEBUG_LOOP   yLOG_enter (__FUNCTION__);                                               <* 
 *>    /+> yLOG_note  ("determine jobs that will run in the next hour");               <+/   <* 
 *>    /+---(locals)---------------------------------+/                                      <* 
 *>    int       chrs, cday, cmon, cdow;         /+ simplifying temp variables    +/         <* 
 *>    char      msg[100];                       /+ display string                +/         <* 
 *>    tCFILE   *x_file;                         /+ current file                  +/         <* 
 *>    tCLINE   *x_line;                         /+ current line                  +/         <* 
 *>    /+---(save time)------------------------------+/                                      <* 
 *>    if (a_start    != 0)  my.fast_beg   = a_start;                                        <* 
 *>    /+---(get the date)---------------------------+/                                      <* 
 *>    tTIME    *curr_time = localtime(&my.fast_beg);                                        <* 
 *>    /+---(work out the current time)----------+/                                          <* 
 *>    chrs = curr_time->tm_hour;                                                            <* 
 *>    cday = curr_time->tm_mday;                                                            <* 
 *>    cmon = curr_time->tm_mon + 1;   /+ change to 1-12                          +/         <* 
 *>    cdow = curr_time->tm_wday;                                                            <* 
 *>    if (cdow == 0) cdow = 7;        /+ put sunday at the end                   +/         <* 
 *>    /+---(format the time)------------------------+/                                      <* 
 *>    snprintf(msg, 50, "---- %02dh %02dd %02dm %01dw", chrs, cday, cmon, cdow);            <* 
 *>    DEBUG_LOOP   yLOG_info ("fast for", msg);                                             <* 
 *>    /+---(prepare)-------------------------------+/                                       <* 
 *>    nfast     = 0;                                                                        <* 
 *>    fasthead  = NULL;                                                                     <* 
 *>    fasttail  = NULL;                                                                     <* 
 *>    DEBUG_LOOP   yLOG_value ("total"    ,  n_cline);                                      <* 
 *>    /+---(scan every file)--------------------+/                                          <* 
 *>    for (x_file = h_cfile; x_file != NULL; x_file = x_file->next) {                       <* 
 *>       yLOG_info  ("file"      , x_file->name);                                           <* 
 *>       /+---(scan every line)-----------------+/                                          <* 
 *>       for (x_line = x_file->head; x_line != NULL; x_line = x_line->next) {               <* 
 *>          /+---(initialize every line)-----------+/                                       <* 
 *>          x_line->active  = '-';                                                          <* 
 *>          x_line->fnext   = NULL;                                                         <* 
 *>          x_line->fprev   = NULL;                                                         <* 
 *>          /+---(filter for active)---------------+/                                       <* 
 *>          if (x_file->retire  == 'y')     continue;                                       <* 
 *>          if (x_line->deleted == 'y')     continue;                                       <* 
 *>          /+---(filter for time)-----------------+/                                       <* 
 *>          snprintf(msg, 100, "%3d) %s", x_line->recd, x_line->tracker);                   <* 
 *>          if (ySCHED_test (&x_line->sched, chrs, ySCHED_ANY) < 0) {                       <* 
 *>             DEBUG_LOOP   yLOG_info  ("not",     msg);                                    <* 
 *>             continue;                                                                    <* 
 *>          }                                                                               <* 
 *>          DEBUG_LOOP   yLOG_info  ("YES",     msg);                                       <* 
 *>          /+---(hook them up)--------------------+/                                       <* 
 *>          if (nfast == 0) {                                                               <* 
 *>             fasthead        = x_line;                                                    <* 
 *>             fasttail        = x_line;                                                    <* 
 *>          } else {                                                                        <* 
 *>             fasttail->fnext = x_line;                                                    <* 
 *>             fasttail        = x_line;                                                    <* 
 *>          }                                                                               <* 
 *>          x_line->active  = 'y';                                                          <* 
 *>          ++nfast;                                                                        <* 
 *>       }                                                                                  <* 
 *>    }                                                                                     <* 
 *>    /+---(log results)---------------------------+/                                       <* 
 *>    DEBUG_LOOP   yLOG_value ("count", nfast);                                             <* 
 *>    DEBUG_LOOP   yLOG_exit  (__FUNCTION__);                                               <* 
 *>    /+---(complete)------------------------------+/                                       <* 
 *>    return 0;                                                                             <* 
 *> }                                                                                        <*/

/*> char                                                                                             <* 
 *> run        (tCFILE *a_file, tCLINE *a_line)                                                      <* 
 *> {                                                                                                <* 
 *>    /+---(locals)--------------------------------+/                                               <* 
 *>    int       rc        = 0;                       /+ simple return code       +/                 <* 
 *>    int       rpid      = 0;                       /+ child pid for execution  +/                 <* 
 *>    tPASSWD  *pass;                                                                               <* 
 *>    FILE     *output = NULL;                                                                      <* 
 *>    long      now;                                 /+ present datetime         +/                 <* 
 *>    tTIME    *curr_time = NULL;                                                                   <* 
 *>    char      msg[200];                                                                           <* 
 *>    char      envp[10][200];                                                                      <* 
 *>    /+---(run)----------------------------+/                                                      <* 
 *>    snprintf(msg, 200, "%-16.16s,%3d", a_line->file->name, a_line->recd);                         <* 
 *>    rc = yEXEC_run (my.name_exec, msg, a_file->user, a_line->command, SHELL, PATH, yEXEC_FORK);   <* 
 *>    if (rc <  0) {                                                                                <* 
 *>       a_line->rpid       = 0;                                                                    <* 
 *>       return -1;                                                                                 <* 
 *>    }                                                                                             <* 
 *>    if (rc >= 0) {                                                                                <* 
 *>       a_line->rpid       = rc;                                                                   <* 
 *>       a_line->lasttime = time(NULL);                                                             <* 
 *>       ++a_line->attempts;                                                                        <* 
 *>       proclist_add (a_line);                                                                     <* 
 *>    }                                                                                             <* 
 *>    return 0;                                                                                     <* 
 *> }                                                                                                <*/



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
exec__unit              (char *a_question, int a_num)
{
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "EXEC             : question not understood", LEN_TEXT);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "time"    )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "EXEC time        : %-10d, %02dy %02dm %02dd %02dh %02dm, %d", my.now, my.year, my.month, my.day, my.hour, my.minute, my.clean);
   }
   else if (strcmp (a_question, "pid"     )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "EXEC pid         : %d", my.pid);
   }
   else if (strcmp (a_question, "focused" )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "EXEC focused     : %d", yDLST_focus_count ());
   }
   else if (strcmp (a_question, "active"  )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "EXEC active      : %d", yDLST_active_count ());
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
exec__unit_kill         (int a_rpid)
{
}





