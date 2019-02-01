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
   strlcpy (x_new->tracker, "n/a", LEN_NAME);
   x_new->recdno      =    0;
   strlcpy (x_new->command, ""   , LEN_NAME);
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
   DEBUG_INPT   yLOG_info    ("tracker"   , my.t_tracker);
   strlcpy (a_line->tracker, my.t_tracker, LEN_NAME);
   /*---(parse schedule)-----------------*/
   DEBUG_INPT   yLOG_info    ("t_sched"   , my.t_schedule);
   rc = ySCHED_parse (NULL, my.t_schedule);
   DEBUG_INPT   yLOG_value   ("sched_rc"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save scheduling info)--------*/
   rc = ySCHED_save (&a_line->sched);
   DEBUG_INPT   yLOG_value   ("save_rc"   , rc);
   --rce;  if (rc  < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flags)-----------------------*/
   a_line->importance = my.t_flags [0];
   a_line->limit      = my.t_flags [2];
   a_line->flag3      = my.t_flags [4];
   a_line->flag4      = my.t_flags [6];
   a_line->flag5      = my.t_flags [8];
   /*---(command)---------------------*/
   DEBUG_INPT   yLOG_info    ("command"   , my.t_command);
   strlcpy (a_line->command, my.t_command, LEN_RECD);
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
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   rc = line__populate (x_line);
   DEBUG_INPT   yLOG_value   ("populate"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create list)--------------------*/
   rc = yDLST_line_create (x_line->tracker, x_line);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
line_parse              (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_fields    =    0;
   char        x_recd      [LEN_RECD];
   int         x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   my.t_ready = '-';
   strlcpy (my.t_tracker , "n/a"      , LEN_NAME);
   strlcpy (my.t_schedule, ""         , LEN_RECD);
   strlcpy (my.t_flags   , "- - - - -", LEN_NAME);
   strlcpy (my.t_command , ""         , LEN_RECD);
   /*---(field count)--------------------*/
   rc = yPARSE_ready (&x_fields);
   DEBUG_INPT   yLOG_value   ("ready"     , rc);
   --rce;  if (rc != 'y') {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("fields"    , x_fields);
   --rce; if (x_fields < 1) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(adjust original record)---------*/
   if (x_fields == 1) {
      DEBUG_INPT   yLOG_note    ("found original style record (six space delimited parts)");
      rc = yPARSE_popstr  (x_recd);
      DEBUG_INPT   yLOG_value   ("original"  , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      strltrim (x_recd, ySTR_SINGLE, LEN_RECD);
      x_fields = strldcnt (x_recd, ' ', LEN_RECD);
      DEBUG_INPT   yLOG_value   ("x_fields"  , x_fields);
      if (x_fields < 5) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_pos = strldpos (x_recd, ' ', 5, LEN_RECD);
      DEBUG_INPT   yLOG_value   ("x_pos"     , x_pos);
      x_recd [x_pos] = '\0';
      strlcpy (my.t_schedule, x_recd, LEN_NAME);
      DEBUG_INPT   yLOG_info    ("t_schedule", my.t_schedule);
      strlcpy (my.t_command , x_recd + x_pos + 1, LEN_NAME);
      DEBUG_INPT   yLOG_info    ("t_command" , my.t_command);
      my.t_ready = 'y';
      /*---(complete)-----------------------*/
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(schedule string)----------------*/
   rc = yPARSE_popstr  (my.t_schedule);
   strltrim (my.t_schedule, ySTR_SINGLE, LEN_RECD);
   DEBUG_INPT   yLOG_value   ("schedule"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("t_schedule", my.t_schedule);
   /*---(tracker/title)------------------*/
   if (x_fields > 2) {
      rc = yPARSE_popstr  (my.t_tracker);
      DEBUG_INPT   yLOG_value   ("tracker"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("tracker"   , my.t_tracker);
   }
   /*---(run-time flags)-----------------*/
   if (x_fields > 3) {
      rc = yPARSE_popstr  (my.t_flags);
      DEBUG_INPT   yLOG_value   ("flags"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("flags"     , my.t_flags);
   }
   /*---(command)------------------------*/
   rc = yPARSE_popstr  (my.t_command);
   DEBUG_INPT   yLOG_value   ("command"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("command"   , my.t_command);
   /*---(set ready)----------------------*/
   my.t_ready = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
line__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_USER]  = "[]";
   char        t           [LEN_USER]  = "[]";
   int         c           =    0;
   void       *x_void      = NULL;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   char        x_ready     =  '-';
   int         x_fields    =    0;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "LINE             : question not understood", LEN_TEXT);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "tracker" )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE tracker     : %2d[%.37s]", strlen (my.t_tracker), my.t_tracker);
   }
   else if (strcmp (a_question, "recdno"  )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE recdno      : %d", my.t_recdno);
   }
   else if (strcmp (a_question, "schedule")        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE schedule    : %2d[%.37s]", strlen (my.t_schedule), my.t_schedule);
   }
   else if (strcmp (a_question, "flags"   )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE flags       : %2d[%.37s]", strlen (my.t_flags), my.t_flags);
   }
   else if (strcmp (a_question, "command" )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE command     : %2d[%.37s]", strlen (my.t_command), my.t_command);
   }
   else if (strcmp (a_question, "yparse"  )        == 0) {
      x_ready = yPARSE_ready (&x_fields);
      snprintf (unit_answer, LEN_TEXT, "LINE yparse      : %2d  %c", x_fields, x_ready);
   }
   else if (strcmp (a_question, "ready"   )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE ready       : %c", my.t_ready);
   }
   else if (strcmp (a_question, "flags"   )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE flags       : %2d[%.37s]", strlen (my.t_flags), my.t_flags);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "LINE count       : %d", yDLST_line_count ());
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      x_line = (tLINE *) yDLST_line_entry (a_num, &x_void);
      x_file = (tFILE *) x_void;
      if (x_line != NULL) {
         sprintf (s, "[%s]", x_file->title);
         sprintf (t, "[%s]", x_line->tracker);
         snprintf (unit_answer, LEN_TEXT, "LINE entry  (%2d) : %-20.20s  %-20.20s  %3d", a_num, t, s, strlen (x_line->command));
      } else {
         snprintf (unit_answer, LEN_TEXT, "LINE entry  (%2d) : []                                           -1", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}






