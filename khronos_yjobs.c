/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"




/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
khronos_yjobs           (cchar a_req, cchar *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   DEBUG_PROG    yLOG_char    ("a_req"     , a_req);
   DEBUG_PROG    yLOG_info    ("a_data"    , a_data);
   /*---(handle)-------------------------*/
   switch (a_req) {
   case YJOBS_READ     : /* no database */                      break;
   case YJOBS_STATS    : /* no database */                      break;
   case YJOBS_WRITE    : /* no database */                      break;
   case YJOBS_PULL     : rc = FILE_pull (a_data);               break;
   case YJOBS_CLEAR    : /* no database */                      break;
   case YJOBS_LOCALRPT : /* rc = poly_yjobs_localrpt ();   */   break;
   case YJOBS_REPORT   : /* no database */                      break;
   case YJOBS_PURGE    : rc = BASE_purge ();                    break;
   }
   /*---(trouble)------------------------*/
   if (rc < 0) {
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return 0;
}
