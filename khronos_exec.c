/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"




/*====================------------------------------------====================*/
/*===----                         job handling                         ----===*/
/*====================------------------------------------====================*/
static void      o___EXECUTION_______________o (void) {;};

char        /* PURPOSE : create list of jobs to run in a particular period    */
BASE_fast     (clong a_start)                /* time_t of starting hour       */
{
   DEBUG_LOOP   yLOG_enter (__FUNCTION__);
   /*> yLOG_note  ("determine jobs that will run in the next hour");               <*/
   /*---(locals)---------------------------------*/
   int       chrs, cday, cmon, cdow;         /* simplifying temp variables    */
   char      msg[100];                       /* display string                */
   tCFILE   *x_file;                         /* current file                  */
   tCLINE   *x_line;                         /* current line                  */
   /*---(save time)------------------------------*/
   if (a_start    != 0)  my.fast_beg   = a_start;
   /*---(get the date)---------------------------*/
   tTIME    *curr_time = localtime(&my.fast_beg);
   /*---(work out the current time)----------*/
   chrs = curr_time->tm_hour;
   cday = curr_time->tm_mday;
   cmon = curr_time->tm_mon + 1;   /* change to 1-12                          */
   cdow = curr_time->tm_wday;
   if (cdow == 0) cdow = 7;        /* put sunday at the end                   */
   /*---(format the time)------------------------*/
   snprintf(msg, 50, "---- %02dh %02dd %02dm %01dw", chrs, cday, cmon, cdow);
   DEBUG_LOOP   yLOG_info ("fast for", msg);
   /*---(prepare)-------------------------------*/
   nfast     = 0;
   fasthead  = NULL;
   fasttail  = NULL;
   DEBUG_LOOP   yLOG_value ("total"    ,  n_cline);
   /*---(scan every file)--------------------*/
   for (x_file = h_cfile; x_file != NULL; x_file = x_file->next) {
      yLOG_info  ("file"      , x_file->name);
      /*---(scan every line)-----------------*/
      for (x_line = x_file->head; x_line != NULL; x_line = x_line->next) {
         /*---(initialize every line)-----------*/
         x_line->active  = '-';
         x_line->fnext   = NULL;
         x_line->fprev   = NULL;
         /*---(filter for active)---------------*/
         if (x_file->retire  == 'y')     continue;
         if (x_line->deleted == 'y')     continue;
         /*---(filter for time)-----------------*/
         snprintf(msg, 100, "%3d) %s", x_line->recd, x_line->tracker); 
         if (ySCHED_test (&x_line->sched, chrs, ySCHED_ANY) < 0) {
            DEBUG_LOOP   yLOG_info  ("not",     msg);
            continue;
         }
         DEBUG_LOOP   yLOG_info  ("YES",     msg);
         /*---(hook them up)--------------------*/
         if (nfast == 0) {
            fasthead        = x_line;
            fasttail        = x_line;
         } else {
            fasttail->fnext = x_line;
            fasttail        = x_line;
         }
         x_line->active  = 'y';
         ++nfast;
      }
   }
   /*---(log results)---------------------------*/
   DEBUG_LOOP   yLOG_value ("count", nfast);
   DEBUG_LOOP   yLOG_exit  (__FUNCTION__);
   /*---(complete)------------------------------*/
   return 0;
}

char        /* PURPOSE : run through the fast list to find what should run    */
BASE_dispatch      (cint a_min)
{
   DEBUG_LOOP   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       njobs = 0;                      /* number of jobs ready to run   */
   int       ntest = 0;                      /* number of jobs ready to run   */
   tCLINE   *x_line;                         /* current line                  */
   char      msg[200];
   /*---(process all time periods)--------------*/
   DEBUG_LOOP   yLOG_value ("total"    ,  n_cline);
   DEBUG_LOOP   yLOG_value ("potential",  nfast);
   for (x_line = fasthead; x_line != NULL; x_line = x_line->fnext) {
      /*> if (my.silent == 'y') {                                                     <* 
       *>    switch (x_line->recovery) {                                              <* 
       *>    case '-' : continue;  break;                                             <* 
       *>    case 'x' : continue;  break;                                             <* 
       *>    }                                                                        <* 
       *> }                                                                           <*/
      ++ntest;
      snprintf(msg, 200, "%-16.16s,%3d", x_line->file->name, x_line->recd);
      if (x_line->sched.min[a_min]  == '_') {
         if (my.silent == 'n') {
            yLOG_senter ("not");
            yLOG_snote(msg);
            yLOG_snote  ("not sched for min");
            yLOG_sexit  ("not");
         }
         continue;
      }
      if (x_line->rpid        != 0) {
         if (my.silent == 'n') {
            yLOG_senter ("not");
            yLOG_snote(msg);
            yLOG_snote  ("already running");
            yLOG_sexit  ("not");
         }
         continue;
      }
      run (x_line->file, x_line);
      ++njobs;
   }
   DEBUG_LOOP   yLOG_value ("tested", ntest);
   DEBUG_LOOP   yLOG_value ("ran",    njobs);
   DEBUG_LOOP   yLOG_exit  (__FUNCTION__);
   /*---(complete)------------------------------*/
   return 0;
}

char
run        (tCFILE *a_file, tCLINE *a_line)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;                       /* simple return code       */
   int       rpid      = 0;                       /* child pid for execution  */
   tPASSWD  *pass;
   FILE     *output = NULL;
   long      now;                                 /* present datetime         */
   tTIME    *curr_time = NULL;
   char      msg[200];
   char      envp[10][200];
   /*---(run)----------------------------*/
   snprintf(msg, 200, "%-16.16s,%3d", a_line->file->name, a_line->recd);
   rc = yEXEC_run (my.name_exec, msg, a_file->user, a_line->command, SHELL, PATH, yEXEC_FORK);
   if (rc <  0) {
      a_line->rpid       = 0;
      return -1;
   }
   if (rc >= 0) {
      a_line->rpid       = rc;
      a_line->lasttime = time(NULL);
      ++a_line->attempts;
      proclist_add (a_line);
   }
   return 0;
}

char        /* PURPOSE : verify status of running jobs and mark completions   */
check         (void)
{
   yLOG_enter (__FUNCTION__);
   int          rc =0;
   /*---(defense)-------------------------------*/
   if (nproc == 0) {
      yLOG_note  ("no currently running jobs");
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(locals)--------------------------------*/
   tCLINE   *x_line    = NULL;                    /* current line             */
   tCFILE   *x_file    = NULL;                    /* the line's file          */
   tCLINE   *x_pnext   = NULL;                    /* the next line to process */
   int       x_status  = 0;                       /* the line's job status    */
   char      msg[200];
   /*---(prepare)-------------------------------*/
   x_line    = prochead;
   /*---(loop)----------------------------------*/
   while (x_line != NULL) {
      /*---(save the next)----------------------*/
      x_pnext   = x_line->pnext;
      x_file    = x_line->file;
      snprintf(msg, 200, "%-15.15s,%3d", x_line->file->name, x_line->recd);
      rc = yEXEC_check (msg, x_line->rpid);
      /*---(handle running line)----------------*/
      if (rc <= 0)  {
         if (rc <  0)  {
            x_line->lastexit = -1;
            ++x_line->failures;
         }
         proclist_del (x_line);
         if (x_line->deleted == 'y') {
            yLOG_snote ("deleted");
            CLINE_delete  (x_line);
            if (x_file->nlines == 0) CFILE_delete (x_file);
         }
      }
      x_line = x_pnext;
      if (x_line == NULL)    break;
   }
   yLOG_exit  (__FUNCTION__);
   return 0;
}



