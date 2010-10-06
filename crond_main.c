/*====================--------[[ start-of-code ]]---------====================*/
#include  "cron.h"

#define   MIN         60

/*====================------------------------------------====================*/
/*===----                            driver                            ----===*/
/*====================------------------------------------====================*/

char        /* PURPOSE : display the current time and return the minute       */
show_time     (void)
{
   FILE     *pulser;
   long      now       = time(NULL);
   tTIME    *curr      = localtime(&now);
   char      msg[60];
   strftime(msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr);
   yLOG_info  ("time",   msg);
   pulser = fopen(PULSER, "w");
   if (pulser != NULL) {
      strftime(msg, 50, "%y.%m.%d.%H.%M.%S.%U   %s", curr);
      fprintf(pulser, "%s\n", msg);
      fclose(pulser);
   }
   return curr->tm_min;
}

char        /* PURPOSE : reliably wait until the next crond trigger moment    */
wait_minute   (void)
{
   /* this function is impemented to perform even when signals interrupt      */
   /*---(locals)--------------------------------*/
   long      now       = 0;                       /* present time             */
   long      inc       = 0;                       /* seconds to sleep         */
   long      targ      = 0;                       /* time to reach            */
   /*---(process)-------------------------------*/
   now       = time(NULL);
   while (1) {
      inc   = MIN - (now % MIN) + 1;
      if (inc > MIN)         break;               /* solve SIGHUP after sleep */
      targ  = now + inc;
      yLOG_value ("sleep (s)", inc);
      sleep(inc);
      now       = time(NULL);
      if (now >= targ)       break;               /* solve SIGHUP during sleep*/
   }
   /*---(complete)------------------------------*/
   return 0;
}

long        /* PURPOSE : return the time at the beginning of the current hour */
curr_hours    (void)
{
   long      now       = time(NULL);
   long      curr      = now - (now % 3600);
   return curr;
}

int
main          (void)
{
   /*---(locals)--------------------------------*/
   long      save      = 0;                       /* last hour ran            */
   long      curr      = 0;                       /* curr hour                */
   int       min       = 0;                       /* curr minute              */
   int       rc        = 0;
   /*---(init)----------------------------------*/
   rc = initialize(0);
   daemonize();
   yLOG_break();
   prepare();
   /*---(main loop)-----------------------------*/
   yLOG_break();
   yLOG_enter("main_loop");
   /*---(get the date)--------------------------*/
   min       = show_time();
   wait_minute();
   curr      = curr_hours();
   while (1) {
      yLOG_break();
      yLOG_sync ();
      yLOG_note ("hourly break -- check crontabs and reset fast list");
      search('n');                                /* force update once an hour*/
      save      = curr;
      fast (curr, 1);                             /* id jobs for next hour    */
      /*---(cycle minutes)----------------------*/
      while (curr == save) {
         yLOG_break();
         min       = show_time();
         yLOG_value ("minute", min);
         search('?');                             /* update only if SIGHUP    */
         check();
         dispatch(min);
         wait_minute();
         curr      = curr_hours();
      }
   }
   yLOG_exit("main_loop");
   yLOG_break();
   /*---(complete)------------------------------*/
   terminate("", 0);
   return 0;
}

/*====================---------[[ end-of-code ]]----------====================*/
