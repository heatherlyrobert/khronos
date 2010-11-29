/*====================--------[[ start-of-code ]]---------====================*/

/*===[[ SUMMARY ]]============================================================#

 *   application   : chronos
 *   module        : chronos_main
 *   size          : less than 100 slocL
 *
 */
/*===[[ SUMMARY ]]============================================================#

 *   chronos_main is the entry point for chronos and is compilied separately
 *   so that unit testing have be separately linked
 *
 */
/*============================================================================*/


#include  "chronos.h"

#define   MIN         60

/*====================------------------------------------====================*/
/*===----                            driver                            ----===*/
/*====================------------------------------------====================*/

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
      if (my.silent == 'n') yLOG_value ("sleep (s)", inc);
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

char
catchup       (void)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   long      curr      = 0;                       /* curr hour                */
   int       min       = 0;                       /* curr minute              */
   /*---(init)----------------------------------*/
   my.silent = 'y';
   /*---(main loop)-----------------------------*/
   yLOG_note ("catchup specially flagged missed jobs");
   min       = ((my.last_end / 60) % 60) + 1;     /* start right after last   */
   yLOG_value ("first min", min);
   /*> curr      = my.last_end - (my.last_end % (60 * 60));                           <*/
   curr      = my.last_end;
   while (curr < my.this_start) {
      fast (curr);                                /* id jobs for this hour    */
      /*---(cycle minutes)----------------------*/
      while (min < 60 && curr <  my.this_start) {
         /*> yLOG_value ("minute", min);                                              <*/
         dispatch(min);
         min      += 1;
         curr     += 60;
      }
      if (curr >=  my.this_start) break;
      min       = 0;
   }
   yLOG_value ("last min", min - 1);
   /*---(reset)---------------------------------*/
   my.silent = 'n';
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
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
   /*---(catchup)-------------------------------*/
   yLOG_break();
   catchup();
   /*---(main loop)-----------------------------*/
   yLOG_break();
   yLOG_enter("main_loop");
   /*---(get the date)--------------------------*/
   min       = timestamp();
   wait_minute();
   curr      = curr_hours();
   while (1) {
      yLOG_break();
      yLOG_sync ();
      yLOG_note ("hourly break -- check crontabs and reset fast list");
      search('n');                                /* force update once an hour*/
      save      = curr;
      fast (curr);                                /* id jobs for this hour    */
      /*---(cycle minutes)----------------------*/
      while (curr == save) {
         yLOG_break();
         min       = timestamp();
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
