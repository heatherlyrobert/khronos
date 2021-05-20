/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


/*-----------------------------------------------
 *
 *  21.11.30.02.15.00  12345  BEG
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         skip      ·     ·    ·  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         pass  12345   120    0  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  SIGTERM
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         fail  12345    60  -12  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         boom  12345    60   11  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         kill  12345   240    9  å21.11.30.02.15.01æ
 *  21.11.30.02.15.02  12345  END
 *
 *
 *
 */



/*====================------------------------------------====================*/
/*===----                          exec tracker                        ----===*/
/*====================------------------------------------====================*/
static void      o___TRACKER_________________o (void) {;};

static FILE    *f_track = NULL;
static int      s_line  = 0;

char
rptg__track_open        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f_track);
   --rce;  if (f_track != NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_snote   (my.n_track);
   f_track = fopen (my.n_track, "at");
   DEBUG_RPTG   yLOG_spoint  (f_track);
   --rce;  if (f_track == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
rptg__track_close       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f_track);
   --rce;  if (f_track == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = fclose (f_track);
   DEBUG_RPTG   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   f_track = NULL;
   DEBUG_RPTG   yLOG_spoint  (f_track);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RPTG_track_exec         (tFILE *a_file, tLINE *a_line, char a_reason, char a_note)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_RPTG   yLOG_point   ("a_line"    , a_line);
   --rce;  if (a_line == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(non-track)----------------------*/
   DEBUG_RPTG   yLOG_char    ("tracking"  , a_line->track);
   if (a_line->track == '-') {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(open)---------------------------*/
   rc = rptg__track_open ();
   DEBUG_RPTG   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(breaks)-------------------------*/
   if (s_line % 5 == 0) {
      fprintf (f_track, "\n##-mm-dd-hh-mm-ss-d-wy-doy --ppid  ---file----------------------- line ---tracker-------------------  ----- -pid- -dur- -rc-  -time- --note--\n");
   }
   ++s_line;
   /*---(prefix)-------------------------*/
   fprintf (f_track, "%-26.26s  %5d  %-30.30s %4d %-30.30s ",
         my.heartbeat, my.m_pid,
         a_file->title, a_line->recdno, a_line->tracker);
   /*---(message)------------------------*/
   switch (a_reason) {
   case T_RUN  :
      fprintf (f_track, "····> %5d", a_line->rpid);
      break;
   case T_SKIP :
      fprintf (f_track, "SKIP");
      break;
   case T_BADD :
      fprintf (f_track, "BADD");
      break;
   case T_BOOM :
      fprintf (f_track, "BOOM  ");
      break;
   case T_KILL :
      fprintf (f_track, "KILL  ");
      break;
   case T_SHUT :
      fprintf (f_track, "SHUT  ");
      break;
   case T_FAIL :
      fprintf (f_track, "FAIL  ");
      break;
   case T_PASS :
      fprintf (f_track, "- - - ");
      break;
   default     :
      break;
   }
   /*---(suffix)-------------------------*/
   if (strchr ("rsb", a_reason) == NULL) {
      fprintf (f_track, "%5d %5d %4d  ", a_line->l_rpid, a_line->l_dur, a_line->l_rc);
      switch (a_note) {
      case T_EARL :
         fprintf (f_track, "EARLY  ");
         break;
      case T_LATE :
         fprintf (f_track, "LATE   ");
         break;
      default     :
         fprintf (f_track, "OTD    ");   /* on-time delivery */
         break;
      }
   }
   /*---(suffix)-------------------------*/
   if (strchr ("okx" , a_reason) != NULL) {
      switch (a_line->l_rc) {
      case SIGTERM :  fprintf (f_track, "sigterm");   break;
      case SIGKILL :  fprintf (f_track, "sigkill");   break;
      case SIGQUIT :  fprintf (f_track, "sigquit");   break;
      case SIGABRT :  fprintf (f_track, "sigabrt");   break;
      case SIGSEGV :  fprintf (f_track, "sigsegv");   break;
      case SIGFPE  :  fprintf (f_track, "sigfpe" );   break;
      case SIGILL  :  fprintf (f_track, "sigill" );   break;
      case SIGBUS  :  fprintf (f_track, "sigbus" );   break;
      case SIGPIPE :  fprintf (f_track, "sigpipe");   break;
      case SIGSYS  :  fprintf (f_track, "sigsys" );   break;
      default      :  fprintf (f_track, "sig????");   break;
      }
   }
   /*---(trailing)-----------------------*/
   fprintf (f_track, "\n");
   /*---(close)--------------------------*/
   rc = rptg__track_close ();
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
rptg__track_ends        (char *a_end)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(globals)------------------------*/
   s_line = 0;
   /*---(open)---------------------------*/
   rc = rptg__track_open ();
   DEBUG_RPTG   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prefix)-------------------------*/
   fprintf (f_track, "\n");
   fprintf (f_track, "%-26.26s  %5d  %s\n",
         my.heartbeat, my.m_pid, a_end);
   if (strncmp (a_end, "BEG", 3) != 0)  fprintf (f_track, "\n");
   /*---(close)--------------------------*/
   rc = rptg__track_close ();
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char RPTG_track_beg     (void)        { return rptg__track_ends ("BEG ----- --------- --------- --------- --------- --------- --------- --------- --------- --------- -----]"); }
char RPTG_track_end     (void)        { return rptg__track_ends ("END ----- --------- --------- --------- --------- --------- --------- --------- --------- --------- -----]"); }
char RPTG_track_sig     (char *a_sig) { return rptg__track_ends (a_sig); }



/*====================------------------------------------====================*/
/*===----                           status report                      ----===*/
/*====================------------------------------------====================*/
static void      o___STATUS__________________o (void) {;};

char
RPTG_minute             (void)
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
   fprintf (f, "#!/usr/sbin/khronos\n");
   fprintf (f, "##   %s\n", P_ONELINE);
   fprintf (f, "##   inventory with major tracking data, updated every minute on logging ram disk (fast)\n");
   fprintf (f, "##   heartbeat to verify update å%sæ\n", my.heartbeat);
   fprintf (f, "##                                                                 f a                                                                    v t h s m m r  f t c d n                         \n");
   fprintf (f, "##                                                                 o c |---- curr -----| |---------- counts ---------| |---- last -----|  a k n t i a e  l h p s e  |------ timings ------|\n");
   fprintf (f, "##          file               line           tracker              c t   start     rpid  rn sk bd bo ki sh fa ps er la  rpid   dur   rc   l r d r n x m  x r u k t    dur     min     max  \n");
   fprintf (f, "##---------------------------- ---- ------------------------------ - - ---------- ------ -- -- -- -- -- -- -- -- -- -- ------ ----- ----  - - - - - - -  - - - - -  ------- ------- -------\n");
   rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_HEAD, NULL, &x_line);
   while (rc >= 0 && x_line != NULL) {
      DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
      DEBUG_INPT   yLOG_info    ("->title"   , x_line->tracker);
      rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
      x_active = (yDLST_active_check (x_line->tracker) > 0) ? 'y' : '-';
      x_focus  = (yDLST_focus_check  (x_line->tracker) > 0) ? 'y' : '-';
      if (c %  5 == 0)  fprintf (f, "\n");
      fprintf (f, "%-30.30s %4d %-30.30s ",
            x_file->title, x_line->recdno, x_line->tracker);
      fprintf (f, "%c %c %10d %6d ",
            x_focus, x_active, x_line->start, x_line->rpid);
      fprintf (f, "%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d ",
            x_line->c_runs, x_line->c_skip, x_line->c_badd, x_line->c_boom, x_line->c_kill, x_line->c_shut, x_line->c_fail, x_line->c_pass, x_line->c_earl, x_line->c_late);
      fprintf (f, "%6d %5d %4d  ",
            x_line->l_rpid, x_line->l_dur, x_line->l_rc);
      fprintf (f, "%c %c %c %c %c %c %c  ",
            x_line->value, x_line->track, x_line->handoff, x_line->strict, x_line->lower, x_line->upper, x_line->remedy);
      fprintf (f, "%c %c %c %c %c  ",
            x_line->flex, x_line->throttle, x_line->cpu, x_line->disk, x_line->net, x_line->upper, x_line->remedy);
      fprintf (f, "%7d %7d %7d\n",
            x_line->est, x_line->est_min, x_line->est_max);
      ++c;
      rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_NEXT, NULL, &x_line);
   }
   fprintf (f, "\n## end-of-file.  %d lines.  done, finito, completare, whimper [Ï´···\n", c);
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




