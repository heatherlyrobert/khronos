/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                        cronwatch file                        ----===*/
/*====================------------------------------------====================*/
static void      o___WATCH___________________o (void) {;};

char
rptg_track              (tLINE *a_line, char a_reason, int a_dur)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        t           [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.n_track);
   f = fopen (my.n_track, "at");
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   /*> if (strcmp (a_message, "end") == 0)   sprintf  (t, "%s %s\n", my.heartbeat, a_reason);   <* 
    *> else                                  sprintf  (t, "%s\n"   , my.heartbeat, a_reason);   <* 
    *> strlrepl (t, "now", a_message, 1, LEN_HUND);                                             <* 
    *> DEBUG_RPTG   yLOG_info    ("watch"     , t);                                             <* 
    *> fputs (t, f);                                                                            <*/
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

/*> char rptg_beg_watch   (void)            { return rptg_track ("BEG", ""); }        <*/
/*> char rptg_end_watch   (char *a_reason)  { return rptg_track ("end", a_reason); }   <*/

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
   char        t           [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_info    ("file"      , my.n_status);
   f = fopen (my.n_status, "wt");
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
   fprintf (f, "##   cron file name             line        cron line tracker          p r n x y   t c t  pid    atm ovr err com kil fai   est min max ear lat    pid  dur ret\n");
   fprintf (f, "##----------------------------  ----  ------------------------------   - - - - -   - - - -----   --- --- --- --- --- ---   --- --- --- --- ---   ----- --- ---\n");
   rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_HEAD, NULL, &x_line);
   while (rc >= 0 && x_line != NULL) {
      DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
      DEBUG_INPT   yLOG_info    ("->title"   , x_line->tracker);
      rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
      x_active = (yDLST_active_check (x_line->tracker) > 0) ? 'y' : '-';
      x_focus  = (yDLST_focus_check  (x_line->tracker) > 0) ? 'y' : '-';
      if (c %  5 == 0)  fprintf (f, "\n");
      fprintf (f, "%-30.30s  %4d  %-30.30s   ",
            x_file->title, x_line->recdno, x_line->tracker);
      fprintf (f, "%c %c %c %c %c %c %c   %c %c %c %5d   %3d %3d %3d %3d %3d %3d   %3d %3d %3d %3d %3d\n",
            x_line->value, x_line->track, x_line->handoff, x_line->strict, x_line->lower, x_line->upper, x_line->remedy);
      fprintf (f, "%c %c %5d   %3d %3d %3d\n",
            x_focus, x_active, x_line->rpid,
            x_line->est, x_line->est_min, x_line->est_max);
      ++c;
      rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_NEXT, NULL, &x_line);
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
   char        x_heartbeat [LEN_HUND];
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "RPTG             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   /*---(complete)-----------------------*/
   return unit_answer;
}




