/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



char        g_seq   =  0;
static      char        s_print     [LEN_RECD] = "";



/*====================------------------------------------====================*/
/*===----                     supporting functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
FILE__wipe              (tFILE *a_cur)
{
   /*---(defense)--------------*/
   if (a_cur == NULL)  return -1;
   /*---(master)-------------------------*/
   a_cur->seq   =   -1;
   strlcpy (a_cur->title  , "" , LEN_HUND);
   strlcpy (a_cur->user   , "" , LEN_USER);
   a_cur->uid   =   -1;
   strlcpy (a_cur->note   , "-", LEN_TERSE);
   a_cur->lines =    0;
   /*---(complete)-------------*/
   return 1;
}

char*
FILE__memory            (tFILE *a_cur)
{
   int         n           =    0;
   strlcpy (s_print, "[____.__]", LEN_RECD);
   ++n;  if (a_cur->seq         >= 0)           s_print [n] = 'X';
   ++n;  if (a_cur->title   [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->user    [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->uid         >= 0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->note    [0] != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->lines       >  0)           s_print [n] = 'X';
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                     allocating and freeing                   ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
FILE__new               (tFILE **a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tFILE      *x_new       = NULL;
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
      x_new = (tFILE *) malloc (sizeof (tFILE));
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
   rc = FILE__wipe (x_new);
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
FILE__free              (tFILE **a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   tFILE      *x_old       = NULL;
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
   FILE__wipe (x_old);
   free (x_old);
   *a_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        major actions                         ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char
FILE_create             (char *a_name, char *a_user, int a_uid)
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
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   DEBUG_INPT   yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   /*---(create data)--------------------*/
   rc = FILE__new (&x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   DEBUG_INPT   yLOG_info    ("file"      , a_name);
   strlcpy (x_file->title, a_name, LEN_HUND);
   DEBUG_INPT   yLOG_info    ("user"      , a_user);
   strlcpy (x_file->user , a_user, LEN_USER);
   DEBUG_INPT   yLOG_value   ("uid"       , a_uid);
   x_file->uid   = a_uid;
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_file->title, x_file);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(assign sequence)----------------*/
   x_file->seq   = g_seq++;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     crontab file names                       ----===*/
/*====================------------------------------------====================*/
static void      o___NAMES___________________o (void) {;}

char
FILE__user_local        (cchar *a_path, cchar *a_prefix, char *a_user)
{  /*---(design notes)-------------------*/
   /*
    *  non-active crontabs must be prefixed with "crontab." and be stored in
    *  a valid home directory (including /root).  they can not be anywhere
    *  else including in "/etc" as the security is more shared/liberal.
    *  system-wide crontabs are stored in "/home/machine".
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_root      =  '-';
   char        x_path      [LEN_PATH]  = "";
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (a_user, "", LEN_USER);
   DEBUG_INPT   yLOG_point   ("a_path"    , a_path);
   --rce;  if (a_path == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_path"    , a_path);
   DEBUG_INPT   yLOG_point   ("a_prefix"  , a_prefix);
   --rce;  if (a_prefix == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_prefix"  , a_prefix);
   /*---(naming)-------------------------*/
   --rce;  if (strcmp (a_prefix, "crontab") != 0) {
      DEBUG_INPT   yLOG_note    ("all locals must have user crontab");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("this is a generic (non-active) crontab");
   /*---(right location)-----------------*/
   --rce;  if (strncmp (a_path, my.n_home, strlen (my.n_home)) == 0) {
      DEBUG_INPT   yLOG_note    ("normal user home directory");
      x_root = '-';
   } else if (strncmp (a_path, my.n_root, strlen (my.n_root)) == 0) {
      DEBUG_INPT   yLOG_note    ("root user home directory");
      x_root = 'y';
   } else {
      DEBUG_INPT   yLOG_note    ("crontabs must be stored in user directories");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set user)-----------------------*/
   --rce;  if (x_root == 'y')  strlcpy (a_user, "root", LEN_USER);
   else {
      l = strlen (my.n_home);
      if (strlen (a_path) <= l + 3) {
         DEBUG_INPT   yLOG_note    ("home directory simply too short");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      strlcpy  (x_path, a_path + l, LEN_PATH);
      strldchg (x_path, '/', '\0', LEN_PATH);
      strlcpy  (a_user, x_path, LEN_USER);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> verify a user name ----------------------------------------*/
FILE_user               (cchar *a_prefix, cchar a_loc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   char        x_user      [LEN_USER]  = "";
   int         x_uid       =   -1;
   char        x_home      [LEN_USER]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strlcpy (my.f_user, "", LEN_USER);
   my.f_uid = -1;
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_prefix"  , a_prefix);
   --rce;  if (a_prefix == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_prefix"  , a_prefix);
   DEBUG_INPT   yLOG_value   ("a_loc"     , a_loc);
   DEBUG_INPT   yLOG_info    ("LOC_ALL"   , LOC_ALL);
   --rce;  if (a_loc == 0 || strchr (LOC_ALL, a_loc) == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for locals)---------------*/
   --rce;  if (a_loc == LOC_LOCAL) {
      rc = FILE__user_local (my.f_path, a_prefix, x_user);
      if (rc < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_note    ("this is a non-active crontab");
   } else {
      DEBUG_INPT   yLOG_note    ("this is an active crontab");
      strlcpy (x_user, a_prefix, LEN_USER);
   }
   DEBUG_INPT   yLOG_info    ("x_user"    , x_user);
   /*---(check for globals)--------------*/
   rc = yEXEC_userdata (x_user, &x_uid, NULL, x_home, NULL);
   DEBUG_INPT   yLOG_value   ("userdata"  , rc);
   if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("user is not registered in the system");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check correct source)-----------*/
   DEBUG_INPT   yLOG_value   ("x_uid"     , x_uid);
   DEBUG_INPT   yLOG_info    ("x_home"    , x_home);
   DEBUG_INPT   yLOG_info    ("f_path"    , my.f_path);
   --rce;  if (a_loc == LOC_CENTRAL) {
      if (strcmp (my.f_path, my.n_central) != 0) {
         DEBUG_INPT   yLOG_note    ("looking for active crontabs in non-central directory");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (my.m_uid != 0) {
         DEBUG_INPT   yLOG_note    ("only root can process active crontabs");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   --rce;  if (a_loc == LOC_LOCAL) {
      if (my.m_uid == 0) {
         DEBUG_INPT   yLOG_note    ("root can process any non-active crontabs");
      } else {
         if (strcmp (my.m_user, x_user) != 0) {
            DEBUG_INPT   yLOG_note    ("user is not the owner of the directory");
            DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         l = strlen (x_home);
         DEBUG_INPT   yLOG_complex ("path"      , "%2d[%s]", strlen (my.f_path), my.f_path);
         DEBUG_INPT   yLOG_complex ("home"      , "%2d[%s]", l, x_home);
         if (strncmp (my.f_path, x_home, l) != 0) {
            DEBUG_INPT   yLOG_note    ("crontab not in specific user home sub-directory");
            DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
   }
   /*---(save)---------------------------*/
   strlcpy (my.f_user, x_user, LEN_USER);
   my.f_uid  = x_uid;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> verify a crontab description ------------------------------*/
FILE__description       (cchar *a_desc)
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

char
FILE_parse          (cchar *a_file, cchar a_loc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char       *q           = NULL;
   int         x_len       =    0;
   char        x_name     [LEN_HUND];
   char        x_generic   =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   my.f_ready = '-';
   strlcpy (my.f_name, "", LEN_HUND);
   strlcpy (my.f_user, "", LEN_USER);
   my.f_uid = -1;
   strlcpy (my.f_desc, "", LEN_DESC);
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
   x_len = strllen (a_file, LEN_HUND);
   DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
   DEBUG_INPT   yLOG_value   ("min"       , 7);
   --rce;  if (x_len <  0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("max"       , LEN_HUND);
   --rce;  if (x_len >= LEN_HUND) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse)--------------------------*/
   strlcpy (x_name, a_file, LEN_HUND);
   p = strtok (x_name, ".");
   DEBUG_INPT   yLOG_point   ("p"         , p);
   /*---(check user)---------------------*/
   rc = FILE_user (p, a_loc);
   DEBUG_INPT   yLOG_value   ("check user", rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse again)--------------------*/
   p = strtok (NULL  , ".");
   DEBUG_INPT   yLOG_point   ("p"         , p);
   /*---(check desc)---------------------*/
   rc = FILE__description (p);
   DEBUG_INPT   yLOG_value   ("check desc", rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse again)--------------------*/
   p = strtok (NULL  , ".");
   DEBUG_INPT   yLOG_point   ("p"         , p);
   --rce;  if (p != NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   strlcpy (my.f_name, a_file , LEN_DESC);
   if (a_loc == LOC_CENTRAL)  my.f_ready = 'y';
   /*---(set full name)------------------*/
   if (a_loc == LOC_CENTRAL) {
      snprintf (my.f_full, LEN_RECD, "%s%s.%s", my.n_central, my.f_user, my.f_desc);
   }
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       file info                              ----===*/
/*====================------------------------------------====================*/
static void      o___CHECK___________________o (void) {;}

char
FILE_acceptable         (cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_cwd       [LEN_PATH]  = "";
   char       *p           = NULL;
   char        t           [LEN_HUND]  = "";
   char        x_user      [LEN_PATH]  = "";
   int         x_uid       =   -1;
   char        x_flag      =  '-';
   int         l           =    0;
   int         i           =    0;
   int         c           =    0;
   tSTAT       s;
   /*---(defaults)-----------------------*/
   yURG_clearerror ();
   my.f_ready = '-';
   strlcpy (my.f_name, "", LEN_HUND);
   strlcpy (my.f_user, "", LEN_USER);
   my.f_uid = -1;
   strlcpy (my.f_desc, "", LEN_DESC);
   strlcpy (my.f_new , "", LEN_DESC);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      yURG_error ("crontab name can not be null or empty");
      DEBUG_INPT   yLOG_note    ("crontab name can not be null or empty");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   /*---(path)---------------------------*/
   p = strchr (a_name, '/');
   DEBUG_INPT   yLOG_point   ("/"         , p);
   --rce;  if (p != NULL) {
      yURG_error ("crontab name can not include relative/absolute path (security risk)");
      DEBUG_INPT   yLOG_note    ("crontab name can not include relative/absolute path (security risk)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("cronfile name is path local (pathless)");
   /*---(check directory)----------------*/
   p = getcwd (x_cwd, LEN_PATH);
   DEBUG_INPT   yLOG_spoint  (p);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_cwd"     , x_cwd);
   --rce;  if (my.m_uid != 0) {
      sprintf (t, "%s%s", my.n_home, my.m_user);
      l = strlen (t);
      if (strncmp (x_cwd, t, l) != 0) {
         yURG_error ("user not in or below their own home directory (security risk)");
         DEBUG_INPT   yLOG_note    ("user not in or below their own home directory");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      strlcpy (x_user, my.m_user, LEN_USER);
   }
   --rce;  if (x_flag == '-') {
      sprintf (t, "%s", my.n_root);
      l = strlen (t);
      if (strncmp (x_cwd, t, l) == 0) {
         DEBUG_INPT   yLOG_note    ("root in root user home directory tree");
         x_flag = 'y';
         strlcpy (x_user, "root", LEN_USER);
      } else {
         sprintf (t, "%s", my.n_home);
         l = strlen (t);
         if (strncmp (x_cwd, t, l) == 0) {
            DEBUG_INPT   yLOG_note    ("root in another user home directory tree");
            strlcpy  (x_user, x_cwd + l, LEN_PATH);
            strldchg (x_user, '/', '\0', LEN_PATH);
            x_flag = 'y';
         }
      }
      if (x_flag != 'y') {
         yURG_error ("root, but not in or below a valid user home directory (security risk)");
         DEBUG_INPT   yLOG_note    ("root, but not in or below a valid user home directory");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(verify user)--------------------*/
   DEBUG_INPT   yLOG_info    ("x_user"    , x_user);
   rc = yEXEC_userdata (x_user, &x_uid, NULL, NULL, NULL);
   DEBUG_INPT   yLOG_value   ("userdata"  , rc);
   if (rc < 0) {
      yURG_error ("user directory not associated with a registered user (security risk)");
      DEBUG_INPT   yLOG_note    ("user is not registered in the system");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name quality)-------------------*/
   l = strlen (a_name);
   DEBUG_INPT   yLOG_value   ("l"         , l);
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (LTRS_FILES, a_name [i]) == NULL) {
         yURG_error ("crontab name has an illegal character (%c) at position %d (security risk)", a_name [i], i);
         DEBUG_INPT   yLOG_complex ("bad char"  , "can not include %c at %d", a_name [i], i);
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   DEBUG_INPT   yLOG_note    ("all name characters are acceptable");
   /*---(general length)-----------------*/
   --rce;  if (l <= 10) {
      yURG_error ("crontab description can not be shorter than 3 characters (lazy)");
      DEBUG_INPT   yLOG_note    ("name is too short for prefix and description");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hidden file)--------------------*/
   DEBUG_INPT   yLOG_char    ("first char", a_name [0]);
   --rce;  if (a_name [0] == '.') {
      yURG_error ("crontab can not be a hidden file, lead period (security risk)");
      DEBUG_INPT   yLOG_note    ("crontab can not be a hidden file");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(exactly one period)-------------*/
   c = strldcnt (a_name, '.', LEN_RECD);
   DEBUG_INPT   yLOG_value   ("periods"   , c);
   --rce;  if (c != 1) {
      yURG_error ("crontab name can only have a single separator/period (standard)");
      DEBUG_INPT   yLOG_note    ("name must have exactly one period");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name prefix)--------------------*/
   --rce;  if (strncmp (a_name, "crontab.", 8) != 0) {
      yURG_error ("crontab name must be prefixed with <<crontab.>> (standard)");
      DEBUG_INPT   yLOG_note    ("name does not begin with crontab");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get stats)----------------------*/
   rc = lstat (a_name, &s);
   DEBUG_YEXEC  yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_error ("crontab file could not be located in current working directory");
      DEBUG_INPT   yLOG_note    ("actual crontab file could not be found");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check file type)----------------*/
   --rce;  if (S_ISDIR (s.st_mode))  {
      yURG_error ("crontab file actually refers to a directory (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can not use a directory");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a directory");
   --rce;  if (S_ISLNK (s.st_mode))  {
      yURG_error ("crontab file actually refers to a symbolic link (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can not use a symlink");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a symlink");
   --rce;  if (!S_ISREG (s.st_mode))  {
      yURG_error ("crontab file is not a regular file (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can only use regular files");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("confirmed as regular file");
   /*---(ownership)----------------------*/
   DEBUG_YEXEC  yLOG_complex ("owner"     , "dir %4d, run %4d, own %4d", x_uid, my.m_uid, s.st_uid);
   --rce;  if (x_uid != s.st_uid) {
      yURG_error ("crontab file owner does not match home directory owner (security risk)");
      DEBUG_YEXEC  yLOG_note    ("file owner does not match home directory owner (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (my.m_uid != 0) {
      if (my.m_uid != s.st_uid) {
         yURG_error ("crontab file ownership does not match run user ownership (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner is not runtime user");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YEXEC  yLOG_note    ("runtime user owns this file");
   } else {
      DEBUG_YEXEC  yLOG_note    ("root can handle anything");
   }
   /*---(permissions)--------------------*/
   DEBUG_ENVI   yLOG_value   ("perms"     , s.st_mode & 00777);
   if  ((s.st_mode & 00777) != 00600)  {
      yURG_error ("crontab file permissions are not owner-only read/write 0600 (security risk)");
      DEBUG_ENVI   yLOG_note    ("permissions not 0600 (private to user)");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("permissions are 0600 (private)");
   /*---(update globals)-----------------*/
   strlcpy (my.f_name, a_name, LEN_HUND);
   strlcpy (my.f_user, x_user, LEN_USER);
   my.f_uid   = x_uid;
   strlcpy (my.f_desc, a_name + 8, LEN_DESC);
   sprintf (my.f_new, "%s.%s", my.f_user, my.f_desc);
   my.f_ready = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char          unit_answer [ LEN_HUND ];

char*            /*--> unit test accessor ------------------------------*/
file__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "[]";
   char        s           [LEN_RECD]  = "[]";
   int         c           =    0;
   tFILE      *x_file      = NULL;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "FILE             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "file"    )        == 0) {
      sprintf (t, "[%s]", my.f_name);
      snprintf (unit_answer, LEN_HUND, "FILE name        : %2d%-37.37s  %c", strlen (my.f_name), t, my.f_ready);
   }
   else if (strcmp (a_question, "user"    )        == 0) {
      sprintf (t, "[%s]", my.f_user);
      snprintf (unit_answer, LEN_HUND, "FILE user        : %2d%-22.22s  %4d", strlen (my.f_user), t, my.f_uid);
   }
   else if (strcmp (a_question, "desc"    )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE desc        : %2d[%.35s]", strlen (my.f_desc), my.f_desc);
   }
   else if (strcmp (a_question, "new"     )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE new         : %2d[%.35s]", strlen (my.f_new), my.f_new);
   }
   else if (strcmp (a_question, "full"          )  == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE full        : %2d[%.35s]", strlen (my.f_full), my.f_full);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE count       : %d", yDLST_list_count ());
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      rc = yDLST_list_by_index (a_num, NULL, &x_file);
      c  = yDLST_line_count (YDLST_LOCAL);
      if (x_file != NULL) {
         sprintf (t, "%2d[%.30s]", strlen (x_file->title), x_file->title);
         sprintf (s, "%2d[%.10s]", strlen (x_file->user) , x_file->user);
         snprintf (unit_answer, LEN_HUND, "FILE entry  (%2d) : %2d  %-34.34s  %-14.14s  %4d  %2d %2d  %s", a_num, x_file->seq, t, s, x_file->uid, c, x_file->lines, x_file->note);
      } else {
         snprintf (unit_answer, LEN_HUND, "FILE entry  (%2d) :  -   -[]                                 -[]               -   -  -  -", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}






