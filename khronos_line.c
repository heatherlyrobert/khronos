/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



static      char        s_print     [LEN_RECD] = "";



/*====================------------------------------------====================*/
/*===----                     supporting functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
LINE__wipe              (tLINE *a_cur)
{
   /*---(defense)--------------*/
   if (a_cur == NULL)  return -1;
   /*---(master)-------------------------*/
   strlcpy (a_cur->tracker, "·", LEN_TITLE);
   a_cur->recdno      =   -1;
   a_cur->sched       =  NULL;
   strlcpy (a_cur->command, ""   , LEN_RECD);
   /*---(working)--------------*/
   a_cur->rpid        =    0;
   a_cur->start       =    0;
   /*---(estimates)------------*/
   a_cur->est         =    0;
   a_cur->est_min     =    0;
   a_cur->est_max     =    0;    
   /*---(flags)----------------*/
   a_cur->value       =  '-';
   a_cur->track       =  '-';
   a_cur->handoff     =  '-';
   a_cur->strict      =  '-';
   a_cur->lower       =  '-';
   a_cur->upper       =  '-';
   a_cur->remedy      =  '-';
   /*---(feedback)-------------*/
   a_cur->attempts    =    0;
   a_cur->errors      =    0;
   a_cur->complete    =    0;
   a_cur->kills       =    0;
   a_cur->failures    =    0;
   a_cur->earlies     =    0;
   a_cur->lates       =    0;
   /*---(complete)-------------*/
   return 1;
}

char*
LINE__memory            (tLINE *a_cur)
{
   int         n           =    0;
   strlcpy (s_print, "[____.__.___._______._______]", LEN_RECD);
   ++n;  if (a_cur->tracker [0] != '·')         s_print [n] = 'X';
   ++n;  if (a_cur->recdno      >= 0)           s_print [n] = 'X';
   ++n;  if (a_cur->sched       != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->command [0] != '\0')        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->rpid        >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->start       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->est         >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->est_min     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->est_max     >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->value       != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->track       != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->handoff     != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->strict      != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->lower       != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->upper       != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->remedy      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->attempts    >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->errors      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->complete    >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->kills       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->failures    >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->earlies     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->lates       >  0)           s_print [n] = 'X';
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                     allocating and freeing                   ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
LINE__new               (tLINE **a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tLINE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*a_new);
   --rce;  if (*a_new != NULL) {
      DEBUG_INPT  yLOG_snote   ("already set");
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_new = NULL;
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tLINE *) malloc (sizeof (tLINE));
      if (x_tries > 3)   break;
   }
   DEBUG_INPT  yLOG_sint    (x_tries);
   DEBUG_INPT  yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }

   /*---(master)-------------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe");
   rc = LINE__wipe (x_new);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
LINE__free              (tLINE **a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   tLINE      *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (a_old);
   --rce;  if (a_old == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*a_old);
   --rce;  if (*a_old == NULL) {
      DEBUG_INPT   yLOG_snote   ("never set");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(simplify)-----------------------*/
   x_old = *a_old;
   /*---(clear and return)---------------*/
   ySCHED_retire (&(x_old->sched));
   LINE__wipe (x_old);
   free (x_old);
   *a_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      parsing new                             ----===*/
/*====================------------------------------------====================*/
static void  o___PARSE___________o () { return; }

char
LINE__populate          (tLINE *a_new)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_floor     =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(line number)--------------------*/
   DEBUG_INPT   yLOG_info    ("line"      , my.t_recdno);
   a_new->recdno      =  my.t_recdno;
   /*---(tracker)---------------------*/
   DEBUG_INPT   yLOG_info    ("tracker"   , my.t_tracker);
   strlcpy (a_new->tracker, my.t_tracker, LEN_TITLE);
   /*---(parse schedule)-----------------*/
   DEBUG_INPT   yLOG_info    ("t_sched"   , my.t_schedule);
   rc = ySCHED_create (&(a_new->sched), my.t_schedule);
   DEBUG_INPT   yLOG_value   ("sched_rc"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flags)-----------------------*/
   rc = yEXEC_flags (a_new->est, x_floor, my.t_flags,
         &(a_new->value) , &(a_new->track) , &(a_new->handoff), &(a_new->strict),
         &(a_new->lower) , &(a_new->est_min), 
         &(a_new->upper) , &(a_new->est_max), 
         &(a_new->remedy));
   /*> a_new->value      = my.t_flags [0];                                            <* 
    *> a_new->track      = my.t_flags [2];                                            <* 
    *> a_new->lower      = my.t_flags [4];                                            <* 
    *> a_new->upper      = my.t_flags [6];                                            <* 
    *> a_new->remedy     = my.t_flags [8];                                            <*/
   /*---(save scheduling info)--------*/
   /*> rc = ySCHED_save (&a_new->sched);                                              <* 
    *> DEBUG_INPT   yLOG_value   ("save_rc"   , rc);                                  <* 
    *> --rce;  if (rc  < 0) {                                                         <* 
    *>    DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*> a_new->est        = a_new->sched.dur;                                          <*/
   DEBUG_INPT   yLOG_value   ("est"       , a_new->est);
   if (a_new->est > 0) {
      switch (a_new->lower) {
      case '9' :  a_new->est_min = a_new->est *   0.90;   break;
      case '8' :  a_new->est_min = a_new->est *   0.80;   break;
      case '7' :  a_new->est_min = a_new->est *   0.70;   break;
      case '6' :  a_new->est_min = a_new->est *   0.60;   break;
      case 'h' :  a_new->est_min = a_new->est *   0.50;   break;
      case 'q' :  a_new->est_min = a_new->est *   0.25;   break;
      case 't' :  a_new->est_min = a_new->est *   0.10;   break;
      case 'z' :  a_new->est_min = a_new->est *   0.00;   break;
      case '-' :  a_new->est_min = a_new->est *   0.00;   break;
      default  :  a_new->est_min = a_new->est *   0.50;   break;
      }
      switch (a_new->upper) {
      case '1' :  a_new->est_max = a_new->est *   1.10;   break;
      case '2' :  a_new->est_max = a_new->est *   1.20;   break;
      case '3' :  a_new->est_max = a_new->est *   1.30;   break;
      case '4' :  a_new->est_max = a_new->est *   1.40;   break;
      case 'H' :  a_new->est_max = a_new->est *   1.50;   break;
      case 'D' :  a_new->est_max = a_new->est *   2.00;   break;
      case 'T' :  a_new->est_max = a_new->est *   3.00;   break;
      case 'Q' :  a_new->est_max = a_new->est *   4.00;   break;
      case 'X' :  a_new->est_max = a_new->est *  10.00;   break;
      case 'Z' :  a_new->est_max = a_new->est * 999.00;   break;
      case '-' :  a_new->est_max = a_new->est * 999.00;   break;
      default  :  a_new->est_max = a_new->est *   2.00;   break;
      }
   }
   DEBUG_INPT   yLOG_value   ("est_min"   , a_new->est_min);
   DEBUG_INPT   yLOG_value   ("est_max"   , a_new->est_max);
   /*---(command)---------------------*/
   DEBUG_INPT   yLOG_info    ("command"   , my.t_command);
   strlcpy (a_new->command, my.t_command, LEN_FULL);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

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
   rc = LINE__new (&x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   --rce;  if (x_line == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   rc = LINE__populate (x_line);
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
LINE__prepare            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strlcpy (my.t_schedule, ""         , LEN_RECD);
   strlcpy (my.t_tracker , "n/a"      , LEN_TITLE);
   strlcpy (my.t_duration, "0"        , LEN_TERSE);
   strlcpy (my.t_flags   , "--·---·"  , LEN_TERSE);
   strlcpy (my.t_command , ""         , LEN_FULL);
   my.t_recdno = -1;
   my.t_ready  = '-';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}

char
LINE__original          (int n, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_recd      [LEN_RECD];
   int         x_fields    =    0;
   int         x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(get record)---------------------*/
   DEBUG_INPT   yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy  (x_recd, a_verb, LEN_RECD);
   strltrim (x_recd, ySTR_SINGLE, LEN_RECD);
   DEBUG_INPT   yLOG_info    ("x_recd"    , x_recd);
   /*---(parse)--------------------------*/
   x_fields = strldcnt (x_recd, ' ', LEN_RECD);
   DEBUG_INPT   yLOG_value   ("x_fields"  , x_fields);
   --rce;  if (x_fields < 6) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_pos = strldpos (x_recd, ' ', 5, LEN_RECD);
   DEBUG_INPT   yLOG_value   ("x_pos"     , x_pos);
   x_recd [x_pos] = '\0';
   /*---(save values)--------------------*/
   strlcpy (my.t_schedule, x_recd, LEN_RECD);
   DEBUG_INPT   yLOG_info    ("t_schedule", my.t_schedule);
   strlcpy (my.t_command , x_recd + x_pos + 1, LEN_FULL);
   DEBUG_INPT   yLOG_info    ("t_command" , my.t_command);
   /*---(set to ready)-------------------*/
   my.t_recdno = n;
   my.t_ready  = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
LINE__revised           (int n, uchar *a_verb, int c)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(get record)---------------------*/
   DEBUG_INPT   yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy  (my.t_schedule, a_verb, LEN_RECD);
   strltrim (my.t_schedule, ySTR_SINGLE, LEN_RECD);
   DEBUG_INPT   yLOG_info    ("schedule"  , my.t_schedule);
   --rce;  switch (c) {
   case  2 : rc = yPARSE_scanf ("line", "F"     , my.t_command);  break;
   case  3 : rc = yPARSE_scanf ("line", "3F"    , my.t_tracker , my.t_command);  break;
   case  4 : rc = yPARSE_scanf ("line", "3TF"   , my.t_tracker, my.t_duration, my.t_command);  break;
   case  5 : rc = yPARSE_scanf ("line", "3TTF"  , my.t_tracker, my.t_duration, my.t_flags, my.t_command);  break;
   default : rc = rce; break;
   }
   DEBUG_INPT   yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      LINE__prepare ();
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(set to ready)-------------------*/
   my.t_recdno = n;
   my.t_ready  = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
LINE__parse             (void)
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
   rc = LINE__prepare ();
   DEBUG_INPT   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_fields = rc;
   /*---(original record)----------------*/
   --rce;  if (x_fields == 1) {
      DEBUG_INPT   yLOG_note    ("found original style record (six space delimited parts)");
      /*> rc = LINE__original ();                                                     <*/
      DEBUG_INPT   yLOG_value   ("original"  , rc);
      if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
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
   /*---(duration)-----------------------*/
   if (x_fields > 3) {
      rc = yPARSE_popstr  (my.t_duration);
      DEBUG_INPT   yLOG_value   ("duration"  , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("t_duration", my.t_duration);
   }
   /*---(run-time flags)-----------------*/
   if (x_fields > 4) {
      rc = yPARSE_popstr  (my.t_flags);
      DEBUG_INPT   yLOG_value   ("flags"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("t_flags"   , my.t_flags);
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
/*===----                      handling employment                     ----===*/
/*====================------------------------------------====================*/
static void  o___EMPLOYMENT______o () { return; }

char
LINE__handler           (int n, uchar *a_verb, char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   /*---(check for valid dates)----------*/
   --rce;  if (strncmp (a_verb, ".valid ", 7) == 0) {
      DEBUG_INPT  yLOG_note    ("found a .valid record");
      rc = ySCHED_valid (a_verb);
      DEBUG_INPT   yLOG_value   ("ySCHED"    , rc);
      if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check for normal lines)---------*/
   rc = LINE__prepare ();
   DEBUG_INPT   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(field count)--------------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT   yLOG_char    ("ready"     , rc);
   --rce;  if (rc != 'y') {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("fields"    , c);
   --rce; if (c < 1) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(original style)-----------------*/
   --rce;  if (c == 1) {
      DEBUG_INPT   yLOG_note    ("found original style record (six space delimited parts)");
      rc = LINE__original (n, a_verb);
      DEBUG_INPT   yLOG_value   ("original"  , rc);
      if (rc < 0) {
         LINE__prepare ();
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(revised style)------------------*/
   --rce;  if (c >  1) {
      DEBUG_INPT   yLOG_note    ("found revised style record (2 to 5 fields)");
      rc = LINE__revised  (n, a_verb, c);
      DEBUG_INPT   yLOG_value   ("revised"   , rc);
      if (rc < 0) {
         LINE__prepare ();
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
LINE_assimilate         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(create file)--------------------*/
   rc = FILE_create (my.f_name, my.f_user, my.f_uid);
   DEBUG_INPT   yLOG_value   ("file"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySCHED_newfile ();
   DEBUG_INPT   yLOG_value   ("ySCHED"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(pull all lines)-----------------*/
   DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);
   rc = yPARSE_autoread (my.f_full, NULL, LINE__handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
LINE_assimilate_OLD     (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(create file)--------------------*/
   rc = FILE_create (my.f_name, my.f_user, my.f_uid);
   DEBUG_INPT   yLOG_value   ("file"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open file)----------------------*/
   DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);
   rc = yPARSE_open_in (my.f_full);
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(lines)--------------------------*/
   while (rc >= 0) {
      /*---(read file)-------------------*/
      rc = yPARSE_read (&my.t_recdno, NULL, NULL, NULL);
      DEBUG_INPT   yLOG_value   ("yparse"    , rc);
      if (rc == 0) {
         DEBUG_INPT  yLOG_note    ("end-of-file");
         break;
      }
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(handle line)-----------------*/
      rc = LINE__parse  ();
      DEBUG_INPT   yLOG_value   ("parse"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = line_create  ();
      DEBUG_INPT   yLOG_value   ("data"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(close file)---------------------*/
   rc = yPARSE_close_in ();
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

int
line_prune              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check all lines)----------------*/
   rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DEAD, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   while (x_line != NULL) {
      /*---(mark retired)----------------*/
      /*> x_line->retire = 'y';                                                       <*/
      /*---(check running)---------------*/
      if (x_line->rpid > 1) {
         DEBUG_INPT   yLOG_note    ("found running process");
         ++x_running;
         rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DEXT, NULL, &x_line);
         DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
         continue;
      }
      /*---(destroy inactive)------------*/
      rc = yDLST_line_destroy (x_line->tracker);
      DEBUG_INPT   yLOG_value   ("destroy"   , rc);
      if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(start fresh)-----------------*/
      x_running = 0;
      rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DEAD, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return x_running;
}

char
line_kill              (char *a_file, char *a_line)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_line"    , a_line);
   --rce;  if (a_line == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find list)----------------------*/
   rc = yDLST_list_by_name (a_file, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   if (x_file == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(find line)----------------------*/
   rc = yDLST_line_by_name (YDLST_LOCAL, a_line, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   if (x_line == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check rpid)---------------------*/
   DEBUG_INPT   yLOG_value   ("->rpid"    , x_line->rpid);
   --rce;  if (x_line->rpid <= 1) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(kill)---------------------------*/
   rc = kill (x_line->rpid, SIGKILL);
   DEBUG_INPT   yLOG_value   ("kill"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
   char        rc          =    0;
   char        s           [LEN_USER]  = "[]";
   char        t           [LEN_USER]  = "[]";
   int         c           =    0;
   void       *x_void      = NULL;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   char        x_ready     =  '-';
   int         x_fields    =    0;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "LINE             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "tracker" )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE tracker     : %2d[%.37s]", strlen (my.t_tracker), my.t_tracker);
   }
   else if (strcmp (a_question, "duration")        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE duration    : %2d[%.37s]", strlen (my.t_duration), my.t_duration);
   }
   else if (strcmp (a_question, "recdno"  )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE recdno      : %d", my.t_recdno);
   }
   else if (strcmp (a_question, "schedule")        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE schedule    : %2d[%.37s]", strlen (my.t_schedule), my.t_schedule);
   }
   else if (strcmp (a_question, "flags"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE flags       : %2d[%.37s]", strlen (my.t_flags), my.t_flags);
   }
   else if (strcmp (a_question, "command" )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE command     : %2d[%.37s]", strlen (my.t_command), my.t_command);
   }
   else if (strcmp (a_question, "yparse"  )        == 0) {
      x_ready = yPARSE_ready (&x_fields);
      snprintf (unit_answer, LEN_HUND, "LINE yparse      : %2d  %c", x_fields, x_ready);
   }
   else if (strcmp (a_question, "ready"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE ready       : %c", my.t_ready);
   }
   else if (strcmp (a_question, "flags"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE flags       : %2d[%.37s]", strlen (my.t_flags), my.t_flags);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE count       : %d", yDLST_line_count ('*'));
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      rc = yDLST_line_by_index  (YDLST_GLOBAL, a_num, NULL, &x_line);
      rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
      if (x_line != NULL) {
         sprintf (s, "[%s]", x_file->title);
         sprintf (t, "[%s]", x_line->tracker);
         snprintf (unit_answer, LEN_HUND, "LINE entry  (%2d) : %-20.20s  %-20.20s  %3d", a_num, t, s, strlen (x_line->command));
      } else {
         snprintf (unit_answer, LEN_HUND, "LINE entry  (%2d) : []                                           -1", a_num);
      }
   }
   else if (strcmp (a_question, "runs"    )        == 0) {
      rc = yDLST_line_by_index  (YDLST_GLOBAL, a_num     , NULL, &x_line);
      if (x_line != NULL) {
         sprintf (t, "[%s]", x_line->tracker);
         /*> snprintf (unit_answer, LEN_HUND, "LINE runs   (%2d) : %-17.17s  %2da %2do %2de %2dc %2dk %2df  %3dr", a_num, t, x_line->attempts, x_line->overlaps, x_line->errors, x_line->complete, x_line->kills, x_line->failures, x_line->last_rc);   <*/
      } else {
         snprintf (unit_answer, LEN_HUND, "LINE runs   (%2d) : []                   a   o   e   c   k   f     r", a_num);
      }
   }
   else if (strcmp (a_question, "durs"    )        == 0) {
      rc = yDLST_line_by_index  (YDLST_GLOBAL, a_num     , NULL, &x_line);
      if (x_line != NULL) {
         sprintf (t, "[%s]", x_line->tracker);
         /*> snprintf (unit_answer, LEN_HUND, "LINE durs   (%2d) : %-17.17s  %3dd %3dm %3dx %3dl %2de %2dl", a_num, t, x_line->est, x_line->est_min, x_line->est_max, x_line->last_dur, x_line->earlies, x_line->lates);   <*/
      } else {
         snprintf (unit_answer, LEN_HUND, "LINE durs   (%2d) : []                   a   o   e   c   k   f     r", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
line__unit_rpid        (char *a_file, char *a_line, int a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_line"    , a_line);
   --rce;  if (a_line == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find list)----------------------*/
   rc = yDLST_list_by_name (a_file, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("find"      , x_file);
   if (x_file == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(find line)----------------------*/
   rc = yDLST_line_by_name (YDLST_LOCAL, a_line, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("find"      , x_line);
   if (x_line == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(update rpid)--------------------*/
   DEBUG_INPT   yLOG_value   ("a_rpid"    , a_rpid);
   x_line->rpid = a_rpid;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}






