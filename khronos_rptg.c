/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"




/*====================------------------------------------====================*/
/*===----                        cronpulse file                        ----===*/
/*====================------------------------------------====================*/
static void      o___PULSE___________________o (void) {;};

char         /*--: update cronpulse with current time ----[ leaf   [ ------ ]-*/
BASE_timestamp     (void)
{
   /*---(locals)-------------------------*/
   FILE     *pulser    = NULL;
   long      now       = 0;
   tTIME    *curr      = NULL;
   /*---(get time)-----------------------*/
   now       = time      (NULL);
   curr      = localtime (&now);
   /*---(log to pulser)------------------*//*===fat=beg===*/
   strftime (my.pulse_time , 50, "%y'%m'%d'%H'%M'%S'%U   %s", curr);
   if (my.silent != 'y') {
      /*---(open)------------------------*/
      pulser = fopen (my.name_pulser, "w");
      if (pulser != NULL) {
         fprintf (pulser, "%s   now   %d\n", my.pulse_time, my.pid);
         fclose  (pulser);
      } else {
         yLOG_warn  ("PULSER", "cronpulse file can not be openned");
         return -1;
      }
      yLOG_info  ("time",   my.pulse_time);
   }
   /*---(complete)-----------------------*//*===fat=end===*/
   return curr->tm_min;
}

long               /* PURPOSE : read the last time from cronpulse ------------*/
BASE_lastpulse     (void)
{
   /*---(locals)--------------------------------*/
   FILE     *pulser    = NULL;
   long      xtime     = 0;
   long      final     = 0;
   char      xbuf[55]  = "";
   int       len       = 0;
   char      trash[50] = "";
   int       rc        = 0;
   /*---(default to now)-------------------------*/
   final = time(NULL);
   /*---(open)-----------------------------------*/
   pulser  = fopen(my.name_pulser, "r");
   if (pulser  != NULL) {
      /*---(get the last stop time)--------------*/
      fgets(xbuf, 50 , pulser);
      /*---(clean it up)-------------------------*/
      len    = strllen (xbuf, 55) - 1;
      if (xbuf[len] == '\n') xbuf[len] = '\0';
      /*---(pull out the epoch)------------------*/
      rc = sscanf(xbuf, "%s %ld", trash, &xtime);
      if (rc == 2 && final >= xtime && xtime > 0) {
         final = xtime;
         snprintf (my.pulse_time , 50, "%s   %ld", trash, xtime);
      }
      /*---(close)-------------------------------*/
      fclose(pulser);
   }
   /*---(complete)-------------------------------*/
   return final;
}



/*====================------------------------------------====================*/
/*===----                        cronwatch file                        ----===*/
/*====================------------------------------------====================*/
static void      o___WATCH___________________o (void) {;};

char               /* PURPOSE : update cronwatch interrun monitoring file     */
BASE_endwatch      (void)
{
   if (my.updates) return 0;
   /*---(locals)---------------------------------*/
   FILE      *watcher;
   /*---(get the last stop time)-----------------*/
   watcher = fopen(my.name_watcher, "a");
   /*---(write the last end time)----------------*/
   BASE_timestamp();
   snprintf(my.pulse_end  , 50, "%s   end   %d", my.pulse_time, my.pid);
   if   (watcher != NULL) {
      fprintf(watcher, "%s\n", my.pulse_end);
      fclose(watcher);
   }
   else yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   /*---(complete)-------------------------------*/
   return 0;
}

char               /* PURPOSE : update cronwatch interrun monitoring file     */
BASE_begwatch      (void)
{
   if (my.updates) return 0;
   /*---(locals)---------------------------------*/
   FILE      *watcher;
   int        len;
   char       trash[50];
   char       xtime[50];
   struct tm *curr_time;
   time_t     time_date;
   /*---(get the last stop time)-----------------*/
   /*> lastrun();                                                                     <*/
   watcher = fopen (my.name_watcher, "a");
   /*---(get a start time)--------------------*/
   BASE_timestamp();
   yLOG_info  ("this beg", my.pulse_time);
   /*---(get missed time)---------------------*/
   /*> sscanf(my.pulse_time , "%s %ld", trash, &my.this_start);                       <* 
    *> my.fast_beg = my.this_start;                                                   <* 
    *> yLOG_value ("missed secs", my.this_start - my.last_end);                       <*/
   /*---(write the time)-------------------------*/
   snprintf(my.pulse_begin, 50, "%s   BEG   %d", my.pulse_time, my.pid);
   if (watcher != NULL) {
      fprintf(watcher, "%s\n", my.pulse_begin);
      fclose(watcher);
   }
   else yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   /*---(complete)-------------------------------*/
   return 0;
}

long               /* PURPOSE : read the last time from cronpulse ------------*/
lastrun       (void)
{
   /*---(locals)--------------------------------*/
   FILE     *watcher   = NULL;
   long      xtime     = 0;
   char      xbuf[55]  = "";
   int       len       = 0;
   char      trash[50] = "";
   int       rc        = 0;
   tTIME    *curr      = NULL;
   /*---(default to now)-------------------------*/
   my.last_end = time(NULL);
   /*---(open)-----------------------------------*/
   watcher = fopen (my.name_watcher, "r");
   if (watcher != NULL) {
      /*---(get the last stop time)--------------*/
      fgets(xbuf, 50 , watcher);
      /*---(clean it up)-------------------------*/
      len    = strllen (xbuf, 55) - 1;
      if (xbuf[len] == '\n') xbuf[len] = '\0';
      /*---(pull out the epoch)------------------*/
      rc = sscanf(xbuf, "%s %ld", trash, &xtime);
      if (rc == 2 && xtime < my.last_end && xtime > 0)  my.last_end = xtime;
      /*---(close)-------------------------------*/
      fclose(watcher);
   } else {
      yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   }
   /*---(save if off)----------------------------*/
   curr  = localtime(&my.last_end);
   strftime(my.pulse_time , 50, "%y'%m'%d'%H'%M'%S'%U   %s", curr);
   yLOG_info  ("last end", my.pulse_time);
   /*---(complete)-------------------------------*/
   return my.last_end;
}

char       /* PURPOSE : generate a current status file -----------------------*/
BASE_status        (void)
{
   /*---(locals)-------------------------*/
   tCFILE     *x_cfile     = NULL;          /* current file                   */
   tCLINE     *x_cline     = NULL;          /* current line                   */
   FILE       *f_status    = NULL;          /* status file                    */
   int         x_count     = 0;
   char       *q           = "\x1F";        /* strtok delimiter               */
   /*---(open)---------------------------*/
   f_status  = fopen (my.name_status, "w");
   if (f_status  == NULL)  return -1;
   /*---(write header)-------------------*/
   fprintf (f_status, "# khronos status reporting file\n");
   fprintf (f_status, "# version : [%s] %s\n", VER_NUM, VER_TXT);
   fprintf (f_status, "# updated : %s\n", my.pulse_time);
   fprintf (f_status, "\n");
   /*---(scan every file)--------------------*/
   for (x_cfile = h_cfile; x_cfile != NULL; x_cfile = x_cfile->next) {
      /*---(scan every line)-----------------*/
      fprintf (f_status, "### %s user----------------- %s name----------------------------------------------- %s ret %s",
            q, q, q, q);
      fprintf (f_status, " line %s tracker-------- %s del %s fst %s now %s",
            q, q, q, q, q);
      fprintf (f_status, " runs %s fail %s last %s\n",
            q, q, q);
      for (x_cline = x_cfile->head; x_cline != NULL; x_cline = x_cline->next) {
         if ((x_count % 3) == 0)  printf ("\n");
         ++x_count;
         fprintf (f_status, "%-3d %s %-21.21s %s %-51.51s %s  %c  %s",
               x_count, q, x_cfile->user, q, x_cfile->name, q, x_cfile->retire, q);
         fprintf (f_status, " %4d %s %-15.15s %s",
               x_cline->recd, q, x_cline->tracker, q);
         fprintf (f_status, "  %c  %s  %c  %s  %c  %s",
               x_cline->deleted, q, x_cline->active, q,
               (x_cline->rpid > 0) ? 'y' : '-', q);
         fprintf (f_status, " %4d %s %4d %s %4d %s\n",
               x_cline->attempts, q, x_cline->failures, q, x_cline->lastexit);
      }
      fprintf (f_status, "\n");
   }
   /*---(close)-------------------------------*/
   fclose (f_status);
   return 0;
}
