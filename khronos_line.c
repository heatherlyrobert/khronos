/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                      fundamental actions                     ----===*/
/*====================------------------------------------====================*/
static void  o___PRIMATIVE_______o () { return; }

tLINE*
line__new               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tLINE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(create)-------------------------*/
   while (++x_tries < 10) {
      x_new = (tLINE *) malloc (sizeof (tLINE));
      if (x_new != NULL)     break;
   }
   DEBUG_INPT   yLOG_sint    (x_tries);
   DEBUG_INPT   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return NULL;
   }
   /*---(wipe)---------------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe");
   x_new->tracker [0] = '\0';
   x_new->recdno      =    0;
   x_new->command [0] = '\0';
   x_new->retire      =  '-';
   x_new->rpid        =    0;
   x_new->importance  =  '-';
   x_new->limit       =  '-';
   x_new->flag3       =  '-';
   x_new->flag4       =  '-';
   x_new->flag5       =  '-';
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return x_new;
}

char
line__populate          (tLINE *a_line)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(tracker)---------------------*/
   DEBUG_INPT   yLOG_info    ("tracker"   , my.tracker);
   strlcpy (a_line->tracker, my.tracker, LEN_NAME);
   /*---(parse schedule)-----------------*/
   DEBUG_INPT   yLOG_info    ("schedule"  , my.schedule);
   rc = ySCHED_parse (NULL, my.schedule);
   DEBUG_INPT   yLOG_value   ("sched_rc"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(save scheduling info)--------*/
   rc = ySCHED_save (&a_line->sched);
   DEBUG_INPT   yLOG_value   ("save_rc"   , rc);
   --rce;  if (rc  < 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(flags)-----------------------*/
   a_line->importance = my.flags [0];
   a_line->limit      = my.flags [2];
   a_line->flag3      = my.flags [4];
   a_line->flag4      = my.flags [6];
   a_line->flag5      = my.flags [8];
   /*---(command)---------------------*/
   DEBUG_INPT   yLOG_info    ("command"   , my.command);
   strlcpy (a_line->command, my.command, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        major actions                         ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char
line_create             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(create data)--------------------*/
   x_line = line__new ();
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   --rce;  if (x_line == NULL) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(populate)-----------------------*/
   rc = line__populate (x_line);
   DEBUG_INPT   yLOG_value   ("populate"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_line->tracker, x_line);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
line_retrieve           (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_fields    =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(field count)--------------------*/
   rc = yPARSE_ready (&x_fields);
   DEBUG_INPT   yLOG_value   ("ready"     , rc);
   --rce;  if (rc != 'y') {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("fields"    , x_fields);
   if (x_fields < 2) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(schedule string)----------------*/
   rc = yPARSE_popstr  (my.schedule);
   DEBUG_INPT   yLOG_value   ("schedule"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("schedule"  , my.schedule);
   /*---(tracker/title)------------------*/
   if (x_fields > 2) {
      rc = yPARSE_popstr  (my.tracker);
      DEBUG_INPT   yLOG_value   ("tracker"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("tracker"   , my.tracker);
   }
   /*---(run-time flags)-----------------*/
   if (x_fields > 3) {
      rc = yPARSE_popstr  (my.flags);
      DEBUG_INPT   yLOG_value   ("flags"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("flags"     , my.flags);
   }
   /*---(command)------------------------*/
   rc = yPARSE_popstr  (my.command);
   DEBUG_INPT   yLOG_value   ("command"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("command"   , my.command);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



