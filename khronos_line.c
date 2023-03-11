/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



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
   strlcpy (a_cur->tracker , "·", LEN_TITLE);
   a_cur->recdno      =   -1;
   a_cur->sched       =  NULL;
   strlcpy (a_cur->command , "" , LEN_RECD);
   /*---(working)--------------*/
   a_cur->retire      =  '-';
   a_cur->rpid        =    0;
   a_cur->start       =    0;
   a_cur->force       =  '-';
   /*---(estimates)------------*/
   a_cur->est         =    0;
   a_cur->est_min     =    0;
   a_cur->est_max     =    0;    
   /*---(flags)----------------*/
   a_cur->value       =  '-';
   a_cur->track       =  '-';
   a_cur->rolling     =  '´';
   a_cur->strict      =  '-';
   a_cur->lower       =  '-';
   a_cur->upper       =  '-';
   a_cur->remedy      =  '-';
   /*---(extended)-------------*/
   a_cur->flex        =  '-';
   a_cur->throttle    =  '-';
   a_cur->cpu         =  '-';
   a_cur->disk        =  '-';
   a_cur->net         =  '-';
   /*---(previous)-------------*/
   a_cur->l_rpid      =    0;
   a_cur->l_beg       =    0;
   a_cur->l_end       =    0;
   a_cur->l_dur       =    0;
   a_cur->l_yexec     =  '-';
   a_cur->l_rc        =    0;
   /*---(counts)---------------*/
   a_cur->trks        =  NULL;
   /*> a_cur->c_runs      =    0;                                                     <* 
    *> a_cur->c_skip      =    0;                                                     <* 
    *> a_cur->c_badd      =    0;                                                     <* 
    *> a_cur->c_boom      =    0;                                                     <* 
    *> a_cur->c_kill      =    0;                                                     <* 
    *> a_cur->c_shut      =    0;                                                     <* 
    *> a_cur->c_fail      =    0;                                                     <* 
    *> a_cur->c_pass      =    0;                                                     <* 
    *> a_cur->c_earl      =    0;                                                     <* 
    *> a_cur->c_late      =    0;                                                     <*/
   /*---(complete)-------------*/
   return 1;
}

char*
LINE__memory            (tLINE *a_cur)
{
   int         n           =    0;
   strlcpy (g_print, "å____.____.___._______._____.______._æ", LEN_RECD);
   ++n;  if (a_cur->tracker [0]  != '·')         g_print [n] = 'X';
   ++n;  if (a_cur->recdno       >= 0)           g_print [n] = 'X';
   ++n;  if (a_cur->sched        != NULL)        g_print [n] = 'X';
   ++n;  if (a_cur->command [0]  != '\0')        g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->retire       != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->rpid         >  0)           g_print [n] = 'X';
   ++n;  if (a_cur->start        >  0)           g_print [n] = 'X';
   ++n;  if (a_cur->force        != '-')         g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->est          >  0)           g_print [n] = 'X';
   ++n;  if (a_cur->est_min      >  0)           g_print [n] = 'X';
   ++n;  if (a_cur->est_max      >  0)           g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->value        != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->track        != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->rolling      != '´')         g_print [n] = 'X';
   ++n;  if (a_cur->strict       != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->lower        != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->upper        != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->remedy       != '-')         g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->flex         != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->throttle     != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->cpu          != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->disk         != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->net          != '-')         g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->l_rpid       != 0)           g_print [n] = 'X';
   ++n;  if (a_cur->l_beg        != 0)           g_print [n] = 'X';
   ++n;  if (a_cur->l_end        != 0)           g_print [n] = 'X';
   ++n;  if (a_cur->l_dur        != 0)           g_print [n] = 'X';
   ++n;  if (a_cur->l_yexec      != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->l_rc         != 0)           g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->trks         != NULL)        g_print [n] = 'X';
   /*> ++n;  if (a_cur->c_skip       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_badd       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_boom       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_kill       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_shut       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_fail       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_pass       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_earl       != 0)           g_print [n] = 'X';               <* 
    *> ++n;  if (a_cur->c_late       != 0)           g_print [n] = 'X';               <*/
   return g_print;
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
LINE_dup                (tLINE *a_orig, tLINE **a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tLINE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
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
   /*---(create new)---------------------*/
   rc = LINE__new (&x_new);
   DEBUG_INPT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(master)-------------------------*/
   DEBUG_INPT   yLOG_note    ("master data");
   strlcpy (x_new->tracker  , a_orig->tracker  , LEN_TITLE);
   x_new->recdno      = a_orig->recdno;
   x_new->sched       = a_orig->sched;
   strlcpy (x_new->command  , a_orig->command  , LEN_RECD);
   /*---(working)------------------------*/
   DEBUG_INPT   yLOG_note    ("working");
   x_new->rpid        = a_orig->rpid;
   x_new->start       = a_orig->start;
   x_new->retire      = a_orig->retire;
   /*---(estimates)----------------------*/
   DEBUG_INPT   yLOG_note    ("estimates");
   x_new->est         = a_orig->est;
   x_new->est_min     = a_orig->est_min;
   x_new->est_max     = a_orig->est_max;
   /*---(flags)--------------------------*/
   DEBUG_INPT   yLOG_note    ("flags");
   x_new->value       = a_orig->value;
   x_new->track       = a_orig->track;
   x_new->rolling     = a_orig->rolling;
   x_new->strict      = a_orig->strict;
   x_new->lower       = a_orig->lower;
   x_new->upper       = a_orig->upper;
   x_new->remedy      = a_orig->remedy;
   /*---(extended)-----------------------*/
   DEBUG_INPT   yLOG_note    ("extended");
   x_new->flex        = a_orig->flex;
   x_new->throttle    = a_orig->throttle;
   x_new->cpu         = a_orig->cpu;
   x_new->disk        = a_orig->disk;
   x_new->net         = a_orig->net;
   /*---(save back)----------------------*/
   DEBUG_INPT   yLOG_note    ("save back");
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
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
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_point   ("a_old"     , a_old);
   --rce;  if (a_old == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_point   ("*a_old"    , *a_old);
   --rce;  if (*a_old == NULL) {
      DEBUG_INPT   yLOG_note    ("never set");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
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
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      parsing new                             ----===*/
/*====================------------------------------------====================*/
static void  o___PARSE___________o () { return; }

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
   strlcpy (my.t_schedule, ""             , LEN_RECD);
   strlcpy (my.t_tracker , ""             , LEN_TITLE);
   strlcpy (my.t_duration, "0"            , LEN_TERSE);
   strlcpy (my.t_flags   , "--·---·-·---" , LEN_LABEL);
   strlcpy (my.t_command , ""             , LEN_FULL);
   my.t_recdno = -1;
   my.t_ready  = '-';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return c;
}

char
LINE__flagfix           (void)
{
   int         l           =   0;
   char        t           [LEN_LABEL] = "";
   char        x_def       [LEN_LABEL] = "--·---·-·---";
   strlcpy (t, my.t_flags, LEN_LABEL);
   l = strlen (t);
   sprintf (my.t_flags, "%s%s", t, x_def + l);
   return 0;
}

char
LINE__original          (int n, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_recd      [LEN_RECD];
   int         c           =    0;
   int         x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('-', "found an original-style record");
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
   c = strldcnt (x_recd, ' ', LEN_RECD);
   DEBUG_INPT   yLOG_value   ("c"         , c);
   --rce;  if (c < 5) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_pos = strldpos (x_recd, ' ', 4, LEN_RECD);
   DEBUG_INPT   yLOG_value   ("x_pos"     , x_pos);
   x_recd [x_pos] = '\0';
   /*---(save values)--------------------*/
   strlcpy (my.t_schedule, x_recd, LEN_RECD);
   DEBUG_INPT   yLOG_info    ("t_schedule", my.t_schedule);
   strlcpy (my.t_command , x_recd + x_pos + 1, LEN_FULL);
   DEBUG_INPT   yLOG_info    ("t_command" , my.t_command);
   /*---(set to ready)-------------------*/
   LINE__flagfix ();
   my.t_recdno = n;
   my.t_ready  = 'y';
   yURG_msg ('-', "schedule  %s", my.t_schedule);
   yURG_msg ('-', "command   %s", my.t_command);
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
   char        l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('-', "found a revised-style record");
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
   case  3 : rc = yPARSE_scanf ("line", "3F"    , my.t_tracker, my.t_command);  break;
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
   /*---(shortened specialty lines)------*/
   if (strcmp (my.t_tracker, "") == 0 && my.t_command [0] == '.') {
      strlcpy (my.t_tracker, my.t_command, LEN_TITLE);
      strcpy  (my.t_command, "");
   }
   /*---(check special)------------------*/
   DEBUG_INPT   yLOG_info    ("tracker"   , my.t_tracker);
   if (my.t_tracker [0] == '.') {
      strlcpy (my.t_duration, ""             , LEN_TERSE);
      strlcpy (my.t_flags   , ""             , LEN_LABEL);
      strlcpy (my.t_command , ""             , LEN_FULL);
   }
   /*---(default in minutes)-------------*/
   l = strlen (my.t_duration);
   if (my.t_duration > 1) {
      if (strchr (YSTR_NUMBER, my.t_duration [l - 1]) != NULL)  strlcat (my.t_duration, "m", LEN_TERSE);
   }
   /*---(set to ready)-------------------*/
   LINE__flagfix ();
   my.t_recdno = n;
   my.t_ready  = 'y';
   yURG_msg ('-', "schedule  %s", my.t_schedule);
   yURG_msg ('-', "tracker   %s", my.t_tracker);
   yURG_msg ('-', "duration  %s", my.t_duration);
   yURG_msg ('-', "flags     %s", my.t_flags);
   yURG_msg ('-', "command   %s", my.t_command);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}


char
LINE__populate          (tLINE *a_new, int n, char *a_schedule, char *a_tracker, char *a_duration, char *a_flags, char *a_command)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_floor     =    0;
   char        t           [LEN_LABEL] = "";
   int         x_line      =    0;
   char        x_sect      [LEN_LABEL] = "";
   char        x_focus     [LEN_LABEL] = "";
   char        x_issue     [LEN_LABEL] = "";
   int         x_pos       =    0;
   int         x_len       =    0;
   char        x_terse     [LEN_HUND]  = "";
   char        x_fancy     [LEN_RECD]  = "";
   tFILE      *x_file      = NULL;
   int         l           = 0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("sched"     , a_schedule);
   --rce;  if (a_schedule == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("command"   , a_command);
   --rce;  if (a_command == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get the file)-------------------*/
   yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(line number)--------------------*/
   DEBUG_INPT   yLOG_value   ("line"      , n);
   a_new->recdno      =  n;
   /*---(parse schedule)-----------------*/
   DEBUG_INPT   yLOG_info    ("sched"     , a_schedule);
   rc = ySCHED_create (&(a_new->sched), a_schedule);
   DEBUG_INPT   yLOG_value   ("sched_rc"  , rc);
   ySCHED_feedback (&x_line, x_sect, x_focus, x_issue, &x_pos, &x_len, x_fancy);
   yURG_msg ('-', "fancy     %s", x_fancy);
   --rce;  if (rc < 0) {
      yURG_err ('f', "%d, %s, %s, %s, %d, %d", x_line, x_sect, x_focus, x_issue, x_pos, x_len);
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("MY RAW"    , ySCHED_raw (a_new->sched));
   /*---(feedback)--------------------*/
   rc = ySCHED_details (my.s_min, my.s_hrs, my.s_dys, my.s_mos, my.s_dow, my.s_wks, my.s_yrs);
   yURG_msg ('-', "min       %s", my.s_min);
   yURG_msg ('-', "hrs       %s", my.s_hrs);
   yURG_msg ('-', "dys       %s", my.s_dys);
   yURG_msg ('-', "mos       %s", my.s_mos);
   yURG_msg ('-', "dow       %s", my.s_dow);
   yURG_msg ('-', "wks       %s", my.s_wks);
   yURG_msg ('-', "yrs       %s", my.s_yrs);
   /*---(tracker)---------------------*/
   DEBUG_INPT   yLOG_info    ("tracker"   , a_tracker);
   /*> DEBUG_INPT   yLOG_value   ("strlen"    , strlen (a_tracker));                  <*/
   sprintf (t, "line %d", n);
   if (a_tracker == NULL)                  strlcpy (a_new->tracker, t, LEN_TITLE);
   else if (a_tracker [0] == '\0')         strlcpy (a_new->tracker, t, LEN_TITLE);
   else                                    strlcpy (a_new->tracker, a_tracker, LEN_TITLE);
   DEBUG_INPT   yLOG_info    ("tracker"   , a_new->tracker);
   /*---(specialty)-------------------*/
   --rce;  if (a_new->tracker [0] == '.') {
      DEBUG_INPT  yLOG_info    ("control"   ,  a_new->tracker);
      if        (strcmp (a_new->tracker, ".valid"   ) == 0) {
         DEBUG_INPT  yLOG_note    ("validity record, .valid, turn on/active");
         yURG_msg ('-', "validity record, .valid, turn on/active");
         x_file->valid = 'y';
      } else if (strcmp (a_new->tracker, ".retire"  ) == 0) {
         DEBUG_INPT  yLOG_note    ("validity record, .retire, turn off");
         yURG_msg ('-', "validity record, .retire, turn on/active");
         x_file->valid = '·';
      } else if (strcmp (a_new->tracker, ".blackout") == 0) {
         DEBUG_INPT  yLOG_note    ("validity record, .blackout, turn off");
         yURG_msg ('-', "validity record, .blackout, turn on/active");
         x_file->valid = '·';
      } else if (strcmp (a_new->tracker, ".only"    ) == 0) {
         DEBUG_INPT  yLOG_note    ("validity record, .only, turn on");
         yURG_msg ('-', "validity record, .only, turn on/active");
         x_file->valid = 'y';
      } else if (strncmp (a_new->tracker, ".graceful", 9) == 0) {
         DEBUG_INPT  yLOG_note    ("shutdown record, .graceful, turn on");
         DEBUG_INPT  yLOG_note    ("shutdown record, .graceful");
         a_new->est = 180;
      } else if (strncmp (a_new->tracker, ".violent" , 8) == 0) {
         DEBUG_INPT  yLOG_note    ("shutdown record, .violent, turn on");
         DEBUG_INPT  yLOG_note    ("shutdown record, .violent");
         a_new->est = 60;
      } else {
         DEBUG_INPT  yLOG_note    ("control record not recognized");
         yURG_err ('f', "%s is not a recognized control record", a_new->tracker);
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(duration)--------------------*/
   --rce;  if (a_duration != NULL) {
      rc = yEXEC_dur_in_sec (a_duration, &(a_new->est));
      DEBUG_INPT   yLOG_value   ("dur"       , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "seconds   %d", a_new->est);
   }
   /*---(flags)-----------------------*/
   --rce;  if (a_flags != NULL) {
      rc = yEXEC_flags_more (a_new->est, x_floor, a_flags,
            &(a_new->value)   , &(a_new->track) , &(a_new->rolling), &(a_new->strict),
            &(a_new->lower)   , &(a_new->est_min), 
            &(a_new->upper)   , &(a_new->est_max), 
            &(a_new->remedy)  , &(a_new->flex),
            &(a_new->throttle), &(a_new->cpu),
            &(a_new->disk)    , &(a_new->net));
      yEXEC_flags_feedback (x_terse, x_fancy);
      yURG_msg ('-', "fancy     %s", x_terse);
      yURG_msg ('-', "details   %s", x_fancy);
      DEBUG_INPT   yLOG_value   ("est"       , a_new->est);
      DEBUG_INPT   yLOG_value   ("est_min"   , a_new->est_min);
      DEBUG_INPT   yLOG_value   ("est_max"   , a_new->est_max);
   }
   /*---(command)---------------------*/
   DEBUG_INPT   yLOG_info    ("command"   , a_command);
   rc = yEXEC_runable (a_new->tracker, x_file->user, a_command, YEXEC_FULL);
   DEBUG_INPT   yLOG_value   ("runnable"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   switch (a_command [0]) {
   case '!' : yURG_err ('w', "demanded OFF-PATH executable with !-bang (security risk)"); break;
   case '@' : yURG_err ('w', "demanded SYMLINK executable with @-worm (security risk)"); break;
   }
   yURG_msg ('-', "executable is found, real, absolute, and runable");
   strlcpy (a_new->command, a_command, LEN_FULL);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
LINE__create            (int n, char *a_schedule, char *a_tracker, char *a_duration, char *a_flags, char *a_command)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tLINE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(create data)--------------------*/
   rc = LINE__new (&x_new);
   DEBUG_INPT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   rc = LINE__populate (x_new, n, a_schedule, a_tracker, a_duration, a_flags, a_command);
   DEBUG_INPT   yLOG_value   ("populate"  , rc);
   --rce;  if (rc < 0) {
      LINE__free (&x_new);
      yURG_msg (' ', "");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create line)--------------------*/
   rc = yDLST_line_create (x_new->tracker, x_new);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      LINE__free (&x_new);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("MY RAW_2"  , ySCHED_raw (x_new->sched));
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
LINE_handler            (int n, uchar *a_verb, char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   tTRKS      *x_cur       = NULL;
   char        x_focus     [LEN_LABEL] = "";
   char        x_issue     [LEN_LABEL] = "";
   int         x_pos       =    0;
   int         x_len       =    0;
   char        x_fancy     [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "handle line number %d...", n);
   /*---(increment count)----------------*/
   rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ++x_file->lines;
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
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
   /*---(create actual line)-------------*/
   rc = LINE__create (my.t_recdno, my.t_schedule, my.t_tracker, my.t_duration, my.t_flags, my.t_command);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce; if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(get current line)------------*/
   rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_CURR, NULL, &x_line);
   DEBUG_INPT  yLOG_point   ("x_line"    , x_line);
   --rce;  if (x_line == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create tracker)--------------*/
   rc = TRKS_create (x_file->title, x_line->tracker, x_line, &(x_line->trks));
   DEBUG_INPT   yLOG_value   ("trks"      , rc);
   --rce; if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(final message)------------------*/
   yURG_msg ('-', "SUCCESS, created a new line from the crontab entry");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      handling employment                     ----===*/
/*====================------------------------------------====================*/
static void  o___EMPLOYMENT______o () { return; }

int
LINE__purge             (char a_scope, char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_retired   = NULL;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(find retired)-------------------*/
   rc = yDLST_list_by_name ("RETIRED", NULL, &x_retired);
   DEBUG_INPT   yLOG_point   ("x_retired" , x_retired);
   --rce;  if (x_retired == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check all lines)----------------*/
   rc = yDLST_line_by_index  (a_scope, n, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   while (x_line != NULL && rc >= 0) {
      /*---(get file)--------------------*/
      rc = yDLST_line_list (NULL, &x_file);
      DEBUG_INPT   yLOG_value   ("list"      , rc);
      if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(header)----------------------*/
      DEBUG_INPT   yLOG_complex ("current"   , "%3dn, %-20.20s, %-10.10p, %2d#, %5dr, %c, %s", n, x_file->title, x_line, x_line->recdno, x_line->rpid, x_line->retire, x_line->tracker);
      /*---(skip retired)----------------*/
      if (a_type != 'a' && x_line->retire == 'y') {
         DEBUG_INPT   yLOG_note    ("found retired process, skipping");
         ++n;
      }
      /*---(save running)----------------*/
      else if (a_type == 'p' && x_line->rpid > 1) {
         DEBUG_INPT   yLOG_note    ("found running process, keep");
         ++n;
      }
      /*---(retire)----------------------*/
      else if (a_type == 'r' && x_line->rpid > 1) {
         DEBUG_INPT   yLOG_note    ("found running process, retire");
         rc = yDLST_line_remove (x_line->tracker);
         DEBUG_INPT   yLOG_value   ("remove"    , rc);
         rc = yDLST_list_by_name ("RETIRED", NULL, NULL);
         DEBUG_INPT   yLOG_value   ("retired"   , rc);
         rc = yDLST_line_create (x_line->tracker, x_line);
         DEBUG_INPT   yLOG_value   ("create"    , rc);
         x_line->retire = 'y';
         rc = yDLST_list_by_name (x_file->title, NULL, NULL);
         DEBUG_INPT   yLOG_value   ("x_file"    , rc);
      }
      /*---(destroy)---------------------*/
      else {
         DEBUG_INPT   yLOG_note    ("not running, destroy");
         rc = yDLST_line_destroy (x_line->tracker);
         DEBUG_INPT   yLOG_value   ("destroy"   , rc);
         if (rc < 0) {
            DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
      /*---(next)------------------------*/
      rc = yDLST_line_by_index  (a_scope, n, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return n;
}

int LINE_purge_global  (void) { return LINE__purge (YDLST_GLOBAL, 'a'); }
int LINE_prune_global  (void) { return LINE__purge (YDLST_GLOBAL, 'p'); }
int LINE_retire_global (void) { return LINE__purge (YDLST_GLOBAL, 'r'); }

/*> int LINE_purge_local   (void) { return LINE__purge (YDLST_LOCAL , 'a'); }         <*/
/*> int LINE_prune_local   (void) { return LINE__purge (YDLST_LOCAL , 'p'); }         <*/
/*> int LINE_retire_local  (void) { return LINE__purge (YDLST_LOCAL , 'r'); }         <*/



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

static char  s_str       [LEN_LABEL] = "";

char*
line__unit_str          (int n, int v, char *s)
{
   if (v == 0) {
      if (n > 1)  sprintf (s_str, "%*.*s·%s", n-1, n-1, "          ", s);
      else        sprintf (s_str, "·%s", s);
   }
   else {
      if (n == 1 && v >  9)  v = 9;
      if (n == 2 && v > 99)  v = 99;
      sprintf (s_str, "%*d%s", n, v, s);
   }
   return s_str;
}

char*            /*--> unit test accessor ------------------------------*/
line__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_LABEL] = "åæ";
   char        s           [LEN_LABEL] = "åæ";
   char        r           [LEN_LABEL] = "åæ";
   char        u           [LEN_LABEL] = "åæ";
   char        x           [LEN_RECD]  = "";
   char        x_stats     [LEN_TITLE] = "";
   char        x_durs      [LEN_TITLE] = "";
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
      snprintf (unit_answer, LEN_HUND, "LINE tracker     : %2då%.37sæ", strlen (my.t_tracker), my.t_tracker);
   }
   else if (strcmp (a_question, "duration")        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE duration    : %2då%.37sæ", strlen (my.t_duration), my.t_duration);
   }
   else if (strcmp (a_question, "recdno"  )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE recdno      : %d", my.t_recdno);
   }
   else if (strcmp (a_question, "schedule")        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE schedule    : %2då%.37sæ", strlen (my.t_schedule), my.t_schedule);
   }
   else if (strcmp (a_question, "flags"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE flags       : %2då%.37sæ", strlen (my.t_flags), my.t_flags);
   }
   else if (strcmp (a_question, "command" )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE command     : %2då%.37sæ", strlen (my.t_command), my.t_command);
   }
   else if (strcmp (a_question, "yparse"  )        == 0) {
      x_ready = yPARSE_ready (&x_fields);
      snprintf (unit_answer, LEN_HUND, "LINE yparse      : %2d  %c", x_fields, x_ready);
   }
   else if (strcmp (a_question, "ready"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE ready       : %c", my.t_ready);
   }
   else if (strcmp (a_question, "flags"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE flags       : %2då%.37sæ", strlen (my.t_flags), my.t_flags);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "LINE count       : %d", yDLST_line_count (YDLST_GLOBAL));
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      rc = yDLST_line_by_index  (YDLST_GLOBAL, a_num, NULL, &x_line);
      rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
      if (x_line != NULL && x_file != NULL) {
         sprintf (t, "%2då%-.12sæ", strlen (x_line->tracker), x_line->tracker);
         sprintf (s, "%2då%-.12sæ", strlen (x_line->command), x_line->command);
         sprintf (u, "%3d %7d %7d", x_line->est, x_line->est_min, x_line->est_max);
         sprintf (r, "%c %c %c  %c %c %c %c", x_line->value, x_line->track, x_line->rolling, x_line->strict, x_line->lower, x_line->upper, x_line->remedy);
         snprintf (unit_answer, LEN_HUND, "LINE entry  (%2d) : %2d/%2d  %-16.16s  %-16.16s  %s   %s", a_num, x_file->seq, x_line->recdno, t, s, u, r);
      } else {
         snprintf (unit_answer, LEN_HUND, "LINE entry  (%2d) :  -/ -   -åæ               -åæ                -       -       -   · · ·  · · · ·", a_num);
      }
   }
   else if (strcmp (a_question, "runs"    )        == 0) {
      rc = yDLST_line_by_index  (YDLST_GLOBAL, a_num     , NULL, &x_line);
      strcpy  (x_stats, "· ·  · · · · ·  · · · ·  · · ·");
      strcpy  (x_durs , "· ·");
      if (x_line != NULL) {
         sprintf (t, "å%-.15sæ", x_line->tracker);
         sprintf (x, "%c %c ", yDLST_focus_check (x_line->tracker) ? 'y' : '-', yDLST_active_check (x_line->tracker) ? 'y' : '-');
         strcat  (x, line__unit_str (10, x_line->start  , " "));
         strcat  (x, line__unit_str ( 5, x_line->rpid   , " "));
         /*    r a  s B O K T  g v G V  f w p  ---history--->>> */
         /*         skipaa              fail
          *           badd                warn
          *             boom                pass
          *               kill
          *                 term
          */
         /*   2r 1s ·b ·m 1k ·x ·f ·p ·e ·l  */
         /*
          *   · 2  1 · · 1 ·
          *
          *
          *
          *
          *
          */
   /*> case KHRONOS_SKIP :  x_off =  8;    break;                                     <* 
    *> case KHRONOS_BADD :  x_off = 10;    break;                                     <* 
    *> case KHRONOS_BOOM :  x_off = 12;    break;                                     <* 
    *> case KHRONOS_KILL :  x_off = 14;    break;                                     <* 
    *> case KHRONOS_TERM :  x_off = 16;    break;                                     <* 
    *> case KHRONOS_LGRA :  x_off = 19;    break;                                     <* 
    *> case KHRONOS_LVIO :  x_off = 21;    break;                                     <* 
    *> case KHRONOS_GGRA :  x_off = 23;    break;                                     <* 
    *> case KHRONOS_GVIO :  x_off = 25;    break;                                     <* 
    *> case KHRONOS_FAIL :  x_off = 28;    break;                                     <* 
    *> case KHRONOS_WARN :  x_off = 30;    break;                                     <* 
    *> case KHRONOS_PASS :  x_off = 32;    break;                                     <*/
         if (x_line->trks != NULL)   strlcpy (x_stats, x_line->trks->stats + 3, 31);
         if (x_line->trks != NULL)   strlcpy (x_durs , x_line->trks->durs  + 3,  4);
         /*> strcat  (x, line__unit_str ( 1, x_line->c_runs , "r "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_skip , "s "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_badd , "b "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_boom , "m "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_kill , "k "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_shut , "x "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_fail , "f "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_pass , "p "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_earl , "e "));                 <* 
          *> strcat  (x, line__unit_str ( 1, x_line->c_late , "l "));                 <*/
         strcat  (x_durs , line__unit_str ( 9, x_line->l_rpid , " "));
         strcat  (x_durs , line__unit_str ( 4, x_line->l_dur  , ""));
         snprintf (unit_answer, LEN_RECD, "LINE runs   (%2d) : %-17.17s %s  %s  %s %4d", a_num, t, x, x_stats, x_durs, x_line->l_rc);
      } else {
         snprintf (unit_answer, LEN_RECD, "LINE runs   (%2d) : åæ                - -          ·     ·   %s  · ·        ·    ·    ·", a_num, x_stats);
      }
   }
   else if (strcmp (a_question, "durs"    )        == 0) {
      rc = yDLST_line_by_index  (YDLST_GLOBAL, a_num     , NULL, &x_line);
      if (x_line != NULL) {
         sprintf (t, "å%sæ", x_line->tracker);
         /*> snprintf (unit_answer, LEN_HUND, "LINE durs   (%2d) : %-17.17s  %3dd %3dm %3dx %3dl %2de %2dl", a_num, t, x_line->est, x_line->est_min, x_line->est_max, x_line->last_dur, x_line->earlies, x_line->lates);   <*/
         snprintf (unit_answer, LEN_HUND, "LINE durs   (%2d) : åæ                   a   o   e   c   k   f     r", a_num);
      } else {
         snprintf (unit_answer, LEN_HUND, "LINE durs   (%2d) : åæ                   a   o   e   c   k   f     r", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
LINE_setrpid           (char *a_file, char *a_line, int a_rpid)
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






