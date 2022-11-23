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
   /*---(debugging)----------------------*/
   rc = PROG_urgents (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("urgents"   , rc);
   if (rc <  0) { PROG_shutdown (); return -1; }
   /*---(initialization)-----------------*/
   rc = PROG_startup (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("startup"   , rc);
   if (rc <  0) { PROG_shutdown (); return -2; }
   /*---(main)---------------------------*/
   rc = yJOBS_driver (P_ONELINE, khronos_yjobs);
   DEBUG_PROG   yLOG_value    ("driver"    , rc);
   /*---(run)----------------------------*/
   IF_RUNNING {
      rc = BASE_execute ();
      DEBUG_PROG   yLOG_value    ("execute"   , rc);
   }
   /*---(wrapup)-------------------------*/
   rc = PROG_shutdown  ();
   DEBUG_PROG   yLOG_value    ("shutdown"  , rc);
   /*---(complete)-----------------------*/
   return rc;
}



/*===============================[[ end-code ]]===============================*/
