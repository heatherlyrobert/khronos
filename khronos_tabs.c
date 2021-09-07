/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                       support functions                      ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;}

char       /*--> verify file existance ---------------------------------------*/
tabs__verify            (cchar *a_full)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_full"    , a_full);
   --rce;  if (a_full == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_full"    , a_full);
   /*---(check on crontab file)-----------------*/
   rc = stat (a_full, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISDIR (s.st_mode))  {
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISLNK (s.st_mode))  {
      DEBUG_INPT   yLOG_note    ("can not use a symlink");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISREG (s.st_mode))  {
      DEBUG_INPT   yLOG_note    ("can only use regular files");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : install a local crontab file -------------------------*/
tabs__remove            (cchar *a_full)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(check)--------------------------*/
   DEBUG_INPT   yLOG_info    ("looking"   , a_full);
   rc = tabs__verify (a_full);
   DEBUG_INPT   yLOG_value   ("verify"    , rc);
   --rce;  if (rc <  0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove)-------------------------*/
   rc = remove   (a_full);
   DEBUG_INPT   yLOG_value   ("remove"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : install a local crontab file -------------------------*/
tabs__notify            (cchar *a_full)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(change to root ownership)-----------*/
   snprintf (x_cmd, LEN_RECD, "chown root:root %s", a_full);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change to strict permissions)-------*/
   snprintf (x_cmd, LEN_RECD, "chmod 0700 %s", a_full);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(send update)------------------------*/
   DEBUG_INPT   yLOG_note    ("send HUP signal to khronos");
   tabs_hup ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          specialty                           ----===*/
/*====================------------------------------------====================*/
static void      o___SPECIALTY_______________o (void) {;}

char
tabs_hup                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_pid       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(find khronos)--------------------------*/
   rc = yEXEC_find ("khronos", &x_pid);
   DEBUG_INPT   yLOG_value   ("find"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("could not execute yEXEC_find");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (rc == 0) {
      DEBUG_INPT   yLOG_note    ("khronos is not running in daemon mode");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_pid <= 1) {
      DEBUG_INPT   yLOG_note    ("did not find a valid pid for khronos");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(send signal)---------------------------*/
   rc = kill (x_pid, SIGHUP);
   DEBUG_INPT   yLOG_value   ("sighup"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("signal could not be sent");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         stub functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___STUBS___________________o (void) {;}

char       /* PURPOSE : list contents of installed crontab -------------------*/
tabs_cat_stub           (void)
{
   printf ("khronos will NEVER allow anyone to review installed crontabs (security risk)\n");
   return -1;
}

char
tabs_dir_stub           (void)
{
   printf ("khronos will NEVER allow anyone to change the crontab directory (security risk)\n");
   return -1;
}

char
tabs_stdin_stub         (void)
{
   printf ("khronos will NEVER allow reading from stdin (no traceability)\n");
   return -1;
}

char
tabs_edit_stub          (void)
{
   printf ("khronos will NEVER allow editing installed crontabs (no traceability)\n");
   return -1;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
tabs__unit              (char *a_question, int a_num)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_count     =    0;
   int         x_total     =    0;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "TABS             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "file"          )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS file        : %2d[%.35s]", strlen (my.f_name), my.f_name);
   }
   else if (strcmp (a_question, "user"          )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS user        : %2d[%.35s]", strlen (my.f_user), my.f_user);
   }
   else if (strcmp (a_question, "desc"          )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS desc        : %2d[%.35s]", strlen (my.f_desc), my.f_desc);
   }
   else if (strcmp (a_question, "ext"           )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS ext         : %2d[%.35s]", strlen (my.f_ext), my.f_ext);
   }
   else if (strcmp (a_question, "path"          )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS path        : %2d[%.35s]", strlen (my.f_path), my.f_path);
   }
   else if (strcmp (a_question, "who"           )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS who         : %2d[%.35s]", strlen (my.m_who), my.m_who);
   }
   else if (strcmp (a_question, "uid"           )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS uid         : %d"        , my.m_uid);
   }
   else if (strcmp (a_question, "entry"         )  == 0) {
      /*---(find entry)---------------------*/
      /*> x_dir  = opendir (my.n_central);                                                                  <* 
       *> if (x_dir == NULL) {                                                                              <* 
       *>    snprintf (unit_answer, LEN_HUND, "TABS entry  (%2d) : can not open dir", a_num);               <* 
       *>    return unit_answer;                                                                            <* 
       *> }                                                                                                 <* 
       *> x_file = readdir (x_dir);                                                                         <* 
       *> while (x_file != NULL) {                                                                          <* 
       *>    rc = FILE_central (x_file->d_name);                                                            <* 
       *>    if (rc >= 0)  {                                                                                <* 
       *>       if (c >= a_num)  break;                                                                     <* 
       *>       ++c;                                                                                        <* 
       *>    }                                                                                              <* 
       *>    x_file = readdir (x_dir);                                                                      <* 
       *> }                                                                                                 <* 
       *> if (x_file != NULL) {                                                                             <* 
       *>    snprintf (unit_answer, LEN_HUND, "TABS entry  (%2d) : [%s]"        , a_num, x_file->d_name);   <* 
       *> } else {                                                                                          <* 
       *>    snprintf (unit_answer, LEN_HUND, "TABS entry  (%2d) : []"          , a_num);                   <* 
       *> }                                                                                                 <* 
       *> rc = closedir (x_dir);                                                                            <*/
      snprintf (unit_answer, LEN_HUND, "TABS entry  (%2d) : []"          , a_num);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*====================---------[[ end-of-code ]]----------====================*/
