/*===============================[[ beg-code ]]===============================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                            main                              ----===*/
/*====================------------------------------------====================*/
static void      o___MAIN____________________o (void) {;}

int        /* PURPOSE : main driver ------------------------------------------*/
main (int argc, char *argv[])
{
   /*---(locals)-------------------------*/
   long      save      = 0;                       /* last hour ran            */
   long      curr      = 0;                       /* curr hour                */
   int       min       = 0;                       /* curr minute              */
   int       rc        = 0;
   /*---(init)---------------------------*/
   if (rc == 0)   rc = PROG_urgs    (argc, argv);
   if (rc == 0)   rc = PROG_init    ();
   if (rc == 0)   rc = PROG_whoami  ();
   if (rc == 0)   rc = PROG_args    (argc, argv);
   if (rc == 0)   rc = PROG_begin   ();
   if (rc != 0) {
      PROG_end     ();
      return -1;
   }
   if (version == 'i') {
      PROG_end     ();
      return 0;
   }
   /*---(khronos init)-------------------*/
   /*> rc = initialize(0);                                                            <*/
   rc = daemonize();
   yLOG_break();
   curr      = curr_hours();
   rc = prepare();
   /*---(catchup)------------------------*/
   yLOG_break();
   catchup();
   /*---(main loop)----------------------*/
   yLOG_break();
   yLOG_enter("main_loop");
   /*---(get the date)-------------------*/
   min       = BASE_timestamp();
   wait_minute();
   curr      = curr_hours();
   while (1) {
      yLOG_break();
      yLOG_sync ();
      /*> yLOG_note ("hourly break -- check crontabs and reset fast list");           <*/
      search('n');                                /* force update once an hour*/
      save      = curr;
      BASE_fast (curr);                           /* id jobs for this hour    */
      /*---(cycle minutes)----------------------*/
      while (curr == save) {
         yLOG_break();
         min       = BASE_timestamp();
         yLOG_value ("minute", min);
         check();
         if (my.resync == '-') {
            search('?');                             /* update only if SIGHUP    */
            BASE_fast (curr);                           /* id jobs for this hour    */
            my.resync = 'n';
         }
         BASE_dispatch (min);
         BASE_status   ();
         wait_minute   ();
         curr      = curr_hours();
      }
   }
   yLOG_exit("main_loop");
   yLOG_break();
   /*---(complete)------------------------------*/
   terminate("", 0);
   return 0;
}



/*===============================[[ end-code ]]===============================*/
