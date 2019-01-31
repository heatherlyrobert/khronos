/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"

char
data_assimilate         (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(create file)--------------------*/
   rc = file_create ();
   DEBUG_INPT   yLOG_value   ("file"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(open file)----------------------*/
   DEBUG_INPT  yLOG_info    ("my.full"   , my.full);
   rc = yPARSE_open_in (my.full);
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(lines)--------------------------*/
   while (rc >= 0) {
      rc = yPARSE_read (&my.recdno, NULL);
      DEBUG_INPT   yLOG_value   ("yparse"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exit    (__FUNCTION__);
         return rce;
      }
      rc = line_retrieve ();
      DEBUG_INPT   yLOG_value   ("data"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exit    (__FUNCTION__);
         return rce;
      }
      rc = line_create  ();
      DEBUG_INPT   yLOG_value   ("data"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(close file)---------------------*/
   rc = yPARSE_close_in ();
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
data_retire             (void)
{
}

