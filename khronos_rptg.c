/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"




/*====================------------------------------------====================*/
/*===----                        cronpulse file                        ----===*/
/*====================------------------------------------====================*/
static void      o___PULSE___________________o (void) {;};

char         /*--> write the heartbeat ---------------------------------------*/
rptg_heartbeat          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.name_heartbeat);
   f = fopen (my.name_heartbeat, "wt");
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   DEBUG_RPTG   yLOG_info    ("heartbeat" , my.heartbeat);
   fputs (my.heartbeat, f);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read the last heartbeat -----------------------------------*/
rptg__unit_heartbeat   (char *a_heartbeat)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.name_heartbeat);
   f = fopen (my.name_heartbeat, "rt");
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   fgets (a_heartbeat, LEN_FIELD, f);
   DEBUG_RPTG   yLOG_info    ("heartbeat" , a_heartbeat);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        cronwatch file                        ----===*/
/*====================------------------------------------====================*/
static void      o___WATCH___________________o (void) {;};

char
rptg__watch             (char *a_message)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        t           [LEN_FIELD];
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.name_watcher);
   f = fopen (my.name_watcher, "at");
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   strlcpy  (t, my.heartbeat, LEN_FIELD);
   strlrepl (t, "now", "BEG", 1, LEN_FIELD);
   DEBUG_RPTG   yLOG_info    ("watch"     , t);
   fputs (t, f);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char rptg_beg_watch   (void)  { return rptg__watch ("BEG"); }
char rptg_end_watch   (void)  { return rptg__watch ("end"); }

char
rptg_status             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   FILE       *f           = NULL;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   char        x_active    =  '-';
   char        x_focus     =  '-';
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.name_status);
   f = fopen (my.name_status, "wt");
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk all lines)-----------------*/
   fprintf (f, "##/usr/sbin/khronos (daemon mode monitoring)\n");
   fprintf (f, "##   khronos-ageraton -- powerful, reliable, trackable, time-based job scheduling\n");
   fprintf (f, "##   daemon list of all crontab lines and all major tracking information\n");
   fprintf (f, "##   updated every minute on logging ram disk (fast)\n");
   fprintf (f, "##\n");
   fprintf (f, "##   heartbeat [%s]\n", my.heartbeat);
   fprintf (f, "##                                                                     i t m m d   r f a                                                                      \n");
   fprintf (f, "##                                                                     m k i a l   e o c  cur    -------success--------    ------timings------   -----last----\n");
   fprintf (f, "##   cron file name             line        cron line tracker          p r n x y   t c t  pid    atm ovr err com kil fai   dur min max ear lat    pid  dur ret\n");
   fprintf (f, "##----------------------------  ----  ------------------------------   - - - - -   - - - -----   --- --- --- --- --- ---   --- --- --- --- ---   ----- --- ---\n");
   for (x_line = yDLST_line_fullseek (YDLST_HEAD); x_line != NULL; x_line = yDLST_line_fullseek (YDLST_NEXT)) {
      DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
      DEBUG_INPT   yLOG_info    ("->title"   , x_line->tracker);
      x_file   = (tFILE *) yDLST_line_list ();
      x_active = (yDLST_active_check (x_line->tracker, NULL) > 0) ? 'y' : '-';
      x_focus  = (yDLST_focus_check  (x_line->tracker, NULL) > 0) ? 'y' : '-';
      if (c %  5 == 0)  fprintf (f, "\n");
      fprintf (f, "%-30.30s  %4d  %-30.30s   %c %c %c %c %c   %c %c %c %5d   %3d %3d %3d %3d %3d %3d   %3d %3d %3d %3d %3d   %5d %3d %3d\n",
            x_file->title, x_line->recdno, x_line->tracker,
            x_line->importance, x_line->tracking, x_line->lower, x_line->upper, x_line->delay,
            x_line->retire, x_focus, x_active, x_line->rpid,
            x_line->attempts, x_line->overlaps, x_line->errors,
            x_line->complete, x_line->kills, x_line->failures,
            x_line->dur, x_line->dur_min, x_line->dur_max,
            x_line->earlies, x_line->lates,
            x_line->last_rpid, x_line->last_dur, x_line->last_rc);
      ++c;
   }
   fprintf (f, "\n## done, finito, complete with %d lines\n", c);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
rptg__unit              (char *a_question, int a_num)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_heartbeat [LEN_FIELD];
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "RPTG             : question not understood", LEN_TEXT);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "heartbeat"     )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "RPTG heartbeat   : %s", my.heartbeat);
   }
   else if (strcmp (a_question, "lastbeat"      )  == 0) {
      rptg__unit_heartbeat (x_heartbeat);
      snprintf (unit_answer, LEN_TEXT, "RPTG lastbeat    : %s", x_heartbeat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




