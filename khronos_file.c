/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                      fundamental actions                     ----===*/
/*====================------------------------------------====================*/
static void  o___PRIMATIVE_______o () { return; }

tFILE*
file__new               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tFILE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(create)-------------------------*/
   while (++x_tries < 10) {
      x_new = (tFILE *) malloc (sizeof (tFILE));
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
   x_new->title [0] = '\0';
   x_new->uid       =    0;
   x_new->retire    =  '-';
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return x_new;
}



/*====================------------------------------------====================*/
/*===----                        major actions                         ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char
file_create             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_char    ("f_ready"   , my.f_ready);
   --rce;  if (my.f_ready != 'y') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create data)--------------------*/
   x_file = file__new ();
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   DEBUG_INPT   yLOG_info    ("file"      , my.f_name);
   strlcpy (x_file->title, my.f_name, LEN_NAME);
   DEBUG_INPT   yLOG_value   ("uid"       , my.f_uid);
   x_file->uid   = my.f_uid;
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_file->title, x_file);
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
file_destroy            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_char    ("f_ready"   , my.f_ready);
   --rce;  if (my.f_ready != 'y') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(destroy list)-------------------*/
   rc = yDLST_list_destroy (my.f_name);
   DEBUG_INPT   yLOG_value   ("destroy"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      handling employment                     ----===*/
/*====================------------------------------------====================*/
static void  o___EMPLOYMENT______o () { return; }

char
file_assimilate         (void)
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
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open file)----------------------*/
   DEBUG_INPT  yLOG_info    ("my.full"   , my.full);
   rc = yPARSE_open_in (my.full);
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(lines)--------------------------*/
   while (rc >= 0) {
      rc = yPARSE_read (&my.t_recdno, NULL);
      DEBUG_INPT   yLOG_value   ("yparse"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = line_parse  ();
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



/*====================------------------------------------====================*/
/*===----                      handling retirement                     ----===*/
/*====================------------------------------------====================*/
static void  o___RETIREMENT______o () { return; }

int
file__retire_prune      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check all lines)----------------*/
   x_line = yDLST_line_seek (YDLST_HEAD);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   while (x_line != NULL) {
      /*---(check running)---------------*/
      if (x_line->rpid > 1) {
         DEBUG_INPT   yLOG_note    ("found running process");
         ++x_running;
         x_line = yDLST_line_seek (YDLST_NEXT);
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
      x_line = yDLST_line_seek (YDLST_HEAD);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return x_running;
}

char
file_retire_scan        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   char        x_replace   =  '-';
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check all lines)----------------*/
   x_file = yDLST_list_seek (YDLST_HEAD);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   while (x_file != NULL) {
      /*---(check for active)------------*/
      if (x_file->retire == '-') {
         x_file = yDLST_list_seek (YDLST_NEXT);
         DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
         continue;
      }
      /*---(set replace flag)------------*/
      if (x_file->retire == 'R')  x_replace = 'y';
      else                        x_replace = '-';
      /*---(prune lines)-----------------*/
      x_running = file__retire_prune ();
      DEBUG_INPT   yLOG_value   ("x_running" , x_running);
      if (x_running < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(still running)---------------*/
      x_running = file__retire_prune ();
      if (x_running > 0) {
         DEBUG_INPT   yLOG_note    ("still has running jobs");
         x_file = yDLST_list_seek (YDLST_NEXT);
         DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
         continue; 
      }
      /*---(ready to destroy)------------*/
      rc = file_destroy ();
      DEBUG_INPT   yLOG_value   ("destroy"   , x_running);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(replace)---------------------*/
      if (x_replace == 'R') {
         DEBUG_INPT   yLOG_note    ("must reload the file");
      }
      /*---(start again)-----------------*/
      x_file = yDLST_list_seek (YDLST_HEAD);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
file_delete             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_char    ("f_ready"   , my.f_ready);
   --rce;  if (my.f_ready != 'y') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe the ext files)-------------*/
   tabs_clear_extfiles ();
   /*---(find list)----------------------*/
   x_file = yDLST_list_find (my.f_name);
   DEBUG_INPT   yLOG_point   ("find"      , x_file);
   if (x_file == NULL) {
      DEBUG_INPT   yLOG_note    ("crontab is not installed, request complete");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(retire)-------------------------*/
   DEBUG_INPT   yLOG_note    ("marked retired, start retirement process");
   x_file->retire = 'y';
   rc = file_retire_scan ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
file_request            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_char    ("f_ready"   , my.f_ready);
   --rce;  if (my.f_ready != 'y') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe the ext files)-------------*/
   tabs_clear_extfiles ();
   /*---(find list)----------------------*/
   x_file = yDLST_list_find (my.f_name);
   DEBUG_INPT   yLOG_point   ("find"      , x_file);
   if (x_file == NULL) {
      DEBUG_INPT   yLOG_note    ("no existing file, assimilate now");
      file_assimilate ();
   }
   DEBUG_INPT   yLOG_note    ("existing file, assimilate after retirement");
   x_file->retire = 'R';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     crontab file names                       ----===*/
/*====================------------------------------------====================*/
static void      o___NAMES___________________o (void) {;}

char         /*--> verify a user name ----------------------------------------*/
file_check_user         (cchar *a_user, cchar a_loc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   int         i           =    0;
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
   tPASSWD    *x_pass      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strlcpy (my.f_user, "", LEN_USER);       /* dup action for unit testing */
   my.f_uid = -1;
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   DEBUG_INPT   yLOG_value   ("a_loc"     , a_loc);
   DEBUG_INPT   yLOG_info    ("LOC_ALL"   , LOC_ALL);
   --rce;  if (a_loc == 0 || strchr (LOC_ALL, a_loc) == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_char    ("a_loc"     , a_loc);
   /*---(check length)-------------------*/
   x_len = strlen (a_user);
   DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
   DEBUG_INPT   yLOG_value   ("min"       , 3);
   --rce;  if (x_len <=  3) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("max"       , LEN_USER);
   --rce;  if (x_len >= LEN_USER) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check letters)------------------*/
   --rce;  for (i = 0; i < x_len; ++i) {
      if (strchr (x_valid, a_user [i]) != NULL)  continue;
      DEBUG_INPT   yLOG_value   ("position"  , i);
      DEBUG_INPT   yLOG_char    ("illegal"   , a_user [i]);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for locals)---------------*/
   --rce;  if (a_loc == LOC_LOCAL) {
      if (strcmp (a_user, "crontab") != 0) {
         DEBUG_INPT   yLOG_note    ("all locals, must have user crontab");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_note    ("this is a generic (non-active) crontab");
   }
   /*---(check validity)-----------------*/
   --rce;  if (a_loc != LOC_LOCAL) {
      x_pass    = getpwnam (a_user);
      DEBUG_INPT   yLOG_point   ("x_pass"    , x_pass);
      --rce;  if (x_pass == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_note    ("this is a valid central crontab user");
   }
   /*---(save)---------------------------*/
   switch (a_loc) {
   case LOC_VERIFY  :
      strlcpy (my.who , a_user, LEN_USER);
      my.uid    = x_pass->pw_uid;
      break;
   case LOC_CENTRAL :
      strlcpy (my.f_user, a_user, LEN_USER);
      my.f_uid  = x_pass->pw_uid;
      break;
   case LOC_LOCAL   :
      strlcpy (my.f_user, a_user, LEN_USER);
      break;
   }
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> verify a crontab description ------------------------------*/
file_check_desc         (cchar *a_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   int         i           =    0;
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strlcpy (my.f_desc, "", LEN_USER);       /* dup action for unit testing */
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_desc"    , a_desc);
   --rce;  if (a_desc == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_desc"    , a_desc);
   /*---(check length)-------------------*/
   x_len = strlen (a_desc);
   DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
   DEBUG_INPT   yLOG_value   ("min"       , 3);
   --rce;  if (x_len <=  3) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("max"       , LEN_DESC);
   --rce;  if (x_len >= LEN_DESC) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check letters)------------------*/
   --rce;  for (i = 0; i < x_len; ++i) {
      if (strchr (x_valid, a_desc [i]) != NULL)  continue;
      DEBUG_INPT   yLOG_value   ("position"  , i);
      DEBUG_INPT   yLOG_char    ("illegal"   , a_desc [i]);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   strlcpy (my.f_desc, a_desc, LEN_DESC);
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> verify a crontab description ------------------------------*/
file_check_ext          (cchar *a_ext, cchar a_loc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strlcpy (my.f_ext , "", LEN_USER);       /* dup action for unit testing */
   /*---(denfense)-----------------------*/
   DEBUG_INPT   yLOG_value   ("a_loc"     , a_loc);
   DEBUG_INPT   yLOG_info    ("LOC_ALL"   , LOC_ALL);
   --rce;  if (a_loc == 0 || strchr (LOC_ALL, a_loc) == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_char    ("a_loc"     , a_loc);
   /*---(check local)--------------------*/
   DEBUG_INPT   yLOG_point   ("a_ext"     , a_ext);
   --rce;  if (a_loc != LOC_CENTRAL && a_ext != NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   if (a_ext == NULL) {
      DEBUG_INPT   yLOG_note    ("no extension given");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_INPT   yLOG_info    ("a_ext"     , a_ext);
   /*---(check length)-------------------*/
   x_len = strlen (a_ext);
   DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len !=  3) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check options)------------------*/
   --rce;  if (strcmp (a_ext, "NEW") != 0 && strcmp (a_ext, "DEL") != 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   strlcpy (my.f_ext , a_ext , LEN_DESC);
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : validate the crontab name                            */
file_parse_name     (cchar *a_file, cchar a_loc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char       *q           = NULL;
   int         x_len       =    0;
   char        x_name     [LEN_NAME];
   char        x_generic   =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   my.f_ready = '-';
   strlcpy (my.f_name, "", LEN_NAME);
   strlcpy (my.f_user, "", LEN_NAME);
   my.f_uid = -1;
   strlcpy (my.f_desc, "", LEN_NAME);
   strlcpy (my.f_ext , "", LEN_NAME);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_file"    , a_file);
   DEBUG_INPT   yLOG_char    ("first char", a_file [0]);
   --rce;  if (a_file [0] == '.') {
      DEBUG_INPT   yLOG_note    ("will not process hidden crontabs");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len = strllen (a_file, LEN_NAME);
   DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
   DEBUG_INPT   yLOG_value   ("min"       , 7);
   --rce;  if (x_len <  0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("max"       , LEN_NAME);
   --rce;  if (x_len >= LEN_NAME) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse)--------------------------*/
   strlcpy (x_name, a_file, LEN_NAME);
   p = strtok (x_name, ".");
   DEBUG_INPT   yLOG_point   ("p"         , p);
   /*---(check user)---------------------*/
   rc = file_check_user (p, a_loc);
   DEBUG_INPT   yLOG_value   ("check user", rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse again)--------------------*/
   p = strtok (NULL  , ".");
   DEBUG_INPT   yLOG_point   ("p"         , p);
   /*---(check desc)---------------------*/
   rc = file_check_desc (p);
   DEBUG_INPT   yLOG_value   ("check desc", rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse again)--------------------*/
   p = strtok (NULL  , ".");
   DEBUG_INPT   yLOG_point   ("p"         , p);
   /*---(check ext)----------------------*/
   rc = file_check_ext  (p, a_loc);
   DEBUG_INPT   yLOG_value   ("check ext" , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   strlcpy (my.f_name, a_file , LEN_DESC);
   if (a_loc == LOC_CENTRAL)  my.f_ready = 'y';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
file__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_USER]  = "[]";
   int         c           =    0;
   tFILE      *x_file      = NULL;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "FILE             : question not understood", LEN_TEXT);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "file"    )        == 0) {
      sprintf (t, "[%s]", my.f_name);
      snprintf (unit_answer, LEN_TEXT, "FILE name        : %2d%-37.37s  %c", strlen (my.f_name), t, my.f_ready);
   }
   else if (strcmp (a_question, "user"    )        == 0) {
      sprintf (t, "[%s]", my.f_user);
      snprintf (unit_answer, LEN_TEXT, "FILE user        : %2d%-22.22s  %4d", strlen (my.f_user), t, my.f_uid);
   }
   else if (strcmp (a_question, "desc"    )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "FILE desc        : %2d[%.35s]", strlen (my.f_desc), my.f_desc);
   }
   else if (strcmp (a_question, "ext"     )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "FILE ext         : %2d[%.35s]", strlen (my.f_ext), my.f_ext);
   }
   else if (strcmp (a_question, "full"          )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "FILE full        : %2d[%.35s]", strlen (my.f_full), my.f_full);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_TEXT, "FILE count       : %d", yDLST_list_count ());
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      x_file = (tFILE *) yDLST_list_entry (a_num, &c);
      if (x_file != NULL) {
         sprintf (t, "[%s]", x_file->title);
         snprintf (unit_answer, LEN_TEXT, "FILE entry  (%2d) : %2d%-30.30s  %4d  %d", a_num, strlen (x_file->title), t, x_file->uid, c);
      } else {
         snprintf (unit_answer, LEN_TEXT, "FILE entry  (%2d) :  0[]                                -1  0", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}






