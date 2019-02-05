/*===============================[[ beg-code ]]===============================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                            main                              ----===*/
/*====================------------------------------------====================*/
static void      o___MAIN____________________o (void) {;}

int        /* PURPOSE : main driver ------------------------------------------*/
main (int argc, char *argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   long        x_save      =    0;                       /* last hour ran            */
   long        x_hour      =    0;                       /* curr hour                */
   int         x_min       =    0;                       /* curr minute              */
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (argc, argv);
   if (rc >= 0)  rc = yURG_urgs    (argc, argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc == 0)  rc = PROG_whoami  ();
   if (rc >= 0)  rc = PROG_args    (argc, argv);
   if (rc >= 0)  rc = PROG_begin   ();
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   --rce;  if (rc <  0) {
      PROG_end ();
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(main)---------------------------*/
   switch (my.user_mode) {
   case MODE_DAEMON :
      base_daemon_mode ();
      break;
   case MODE_USER   :
   default          :
      break;
   }
   /*---(wrapup)-------------------------*/
   rc = PROG_end  ();
   /*---(complete)-----------------------*/
   return 0;
}



/*===============================[[ end-code ]]===============================*/
