/*===============================[[ beg-code ]]===============================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                            main                              ----===*/
/*====================------------------------------------====================*/
static void      o___MAIN____________________o (void) {;}

int        /* PURPOSE : main driver ------------------------------------------*/
main                    (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   long        x_save      =    0;                       /* last hour ran            */
   long        x_hour      =    0;                       /* curr hour                */
   int         x_min       =    0;                       /* curr minute              */
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = PROG_debugging (a_argc, a_argv);
   DEBUG_PROG  yLOG_value   ("debugging" , rc);
   if (rc >= 0)  rc = PROG_startup   (a_argc, a_argv);
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   --rce;  if (rc <  0) {
      PROG_end ();
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(main)---------------------------*/
   /*> rc = PROG_driver ();                                                           <*/
   rc = yJOBS_driver (my.run_as, my.run_mode, P_ONELINE, my.run_file, my.m_who, my.m_uid, FILE_assimilate, BASE_execute);
   /*---(wrapup)-------------------------*/
   PROG_end  ();
   /*---(complete)-----------------------*/
   return rc;
}



/*===============================[[ end-code ]]===============================*/
