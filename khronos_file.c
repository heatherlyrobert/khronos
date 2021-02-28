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
/*===----                    contirm good crontabs                     ----===*/
/*====================------------------------------------====================*/
static void      o___CONFIRM_________________o (void) {;}

FILE__defaults          (void)
{
   yURG_clearerror ();
   my.f_ready = '-';
   strlcpy (my.f_name, "", LEN_HUND);
   strlcpy (my.f_user, "", LEN_USER);
   my.f_uid = -1;
   strlcpy (my.f_desc, "", LEN_DESC);
   strlcpy (my.f_full, "", LEN_DESC);
   strlcpy (my.f_new , "", LEN_DESC);
   return 0;
}

char
FILE__naming            (char a_loc, uchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   int         i           =    0;
   int         c           =    0;
   char       *p           = NULL;
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      yURG_error ("crontab name can not be null or empty");
      DEBUG_INPT   yLOG_note    ("crontab name can not be null or empty");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   yURG_message ("  -- name <<%s>> is not empty/null", a_name);
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
   yURG_message ("  -- crontab name is pathless/local");
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
   yURG_message ("  -- all the file name characters are legal [A-Za-z0-9_.]");
   /*---(hidden file)--------------------*/
   DEBUG_INPT   yLOG_char    ("first char", a_name [0]);
   --rce;  if (a_name [0] == '.') {
      yURG_error ("crontab can not be a hidden file, lead period (security risk)");
      DEBUG_INPT   yLOG_note    ("crontab can not be a hidden file");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- crontab is not a hidden file (no lead period)");
   /*---(exactly one period)-------------*/
   c = strldcnt (a_name, '.', LEN_RECD);
   DEBUG_INPT   yLOG_value   ("periods"   , c);
   --rce;  if (c < 1) {
      yURG_error ("crontab name does not have at least one separator/period (standard)");
      DEBUG_INPT   yLOG_note    ("name must have exactly one separator/period");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- has at least one period/separator");
   /*---(description length)-------------*/
   --rce;  if (a_loc == LOC_LOCAL && l <= 10) {
      yURG_error ("crontab description can not be shorter than 3 characters (lazy)");
      DEBUG_INPT   yLOG_note    ("description too short (< 3 chars)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE__location          (char a_loc, uchar *a_name, char *a_user, int *a_uid, char *a_dir)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_cwd       [LEN_PATH]  = "";
   char       *p           = NULL;
   char        t           [LEN_HUND]  = "";
   char        x_user      [LEN_PATH]  = "";
   int         x_uid       =   -1;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check directory)----------------*/
   p = getcwd (a_dir, LEN_PATH);
   DEBUG_INPT   yLOG_spoint  (p);
   --rce;  if (p == NULL) {
      yURG_error ("can not obtain current working directory");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   if (a_loc == LOC_LOCAL)  strlcat (a_dir, "/", LEN_PATH);
   DEBUG_INPT   yLOG_info    ("a_dir"     , a_dir);
   yURG_message ("  -- current working directory is %s", a_dir);
   /*---(check normal install)-----------*/
   --rce;  if (a_loc == LOC_LOCAL && my.m_uid != 0) {
      sprintf (t, "%s%s", my.n_home, my.m_user);
      l = strlen (t);
      if (strncmp (a_dir, t, l) != 0) {
         yURG_error ("user not in or below their own home directory (security risk)");
         DEBUG_INPT   yLOG_note    ("user not in or below their own home directory");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      strlcpy (a_user, my.m_user, LEN_USER);
      yURG_message ("  -- normal user crontab in or below home directory");
   }
   /*---(check root install)-------------*/
   --rce;  if (a_loc == LOC_LOCAL && my.m_uid == 0) {
      sprintf (t, "%s", my.n_root);
      l = strlen (t);
      if (strncmp (a_dir, t, l) == 0) {
         DEBUG_INPT   yLOG_note    ("root in root user home directory tree");
         strlcpy (a_user, "root", LEN_USER);
         yURG_message ("  -- root user crontab in or below root home directory");
      } else {
         sprintf (t, "%s", my.n_home);
         l = strlen (t);
         if (strncmp (a_dir, t, l) == 0) {
            DEBUG_INPT   yLOG_note    ("root in another user home directory tree");
            strlcpy  (x_user, a_dir + l, LEN_PATH);
            strldchg (x_user, '/', '\0', LEN_PATH);
            strlcpy (a_user, x_user, LEN_USER);
            yURG_message ("  -- root user crontab in or below %s users home directory", x_user);
         } else {
            yURG_error ("root, but not in or below a valid user home directory (security risk)");
            DEBUG_INPT   yLOG_note    ("root, but not in or below a valid user home directory");
            DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
   }
   /*---(check central audit)------------*/
   --rce;  if (a_loc == LOC_CENTRAL) {
      /*---(verify user)--------------------*/
      strlcpy  (x_user, a_name, LEN_USER);
      strldchg (x_user, '.', '\0', LEN_USER);
      strlcpy  (a_user, x_user, LEN_USER);
      yURG_message ("  -- central cronfile prefix is %s", a_user);
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   /*---(check for registered)-----------*/
   rc = yEXEC_userdata (a_user, a_uid, NULL, NULL, NULL);
   DEBUG_INPT   yLOG_value   ("userdata"  , rc);
   if (rc < 0) {
      if (a_loc == LOC_LOCAL) {
         yURG_error ("user directory not associated with a registered user (security risk)");
      } else {
         yURG_error ("user prefix not associated with a registered user (security risk)");
      }
      DEBUG_INPT   yLOG_note    ("user is not registered in the system");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- file user is registered with system, %s, uid %d", a_user, *a_uid);
   /*---(name prefix)--------------------*/
   --rce;  if (a_loc == LOC_LOCAL) {
      if (strncmp (a_name, "khronos.", 8) != 0) {
         yURG_error ("crontab name must be prefixed with <<khronos.>> (standard)");
         DEBUG_INPT   yLOG_note    ("name does not begin with crontab");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_message ("  -- name is prefixed appropriately with <<khronos.>>");
   }
   --rce;  if (a_loc == LOC_CENTRAL && my.m_uid != 0) {
      if (strcmp (my.m_user, a_user) != 0) {
         yURG_error   ("  -- FATAL, run-time user <<%s>> can not review this file", my.m_user);
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE__stats             (char a_loc, uchar *a_dir, uchar *a_name, uchar *a_user, int a_uid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_full      [LEN_PATH]  = "";
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(get stats)----------------------*/
   sprintf (x_full, "%s%s", a_dir, a_name);
   rc = lstat (x_full, &s);
   DEBUG_YEXEC  yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_error ("crontab file could not be located in %s", a_dir);
      DEBUG_INPT   yLOG_note    ("actual crontab file could not be found");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- located the actual file in %s", a_dir);
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
   yURG_message ("  -- file is confirmed as a regular file, not a symlink/directory");
   /*---(ownership)----------------------*/
   DEBUG_YEXEC  yLOG_complex ("owner"     , "user %4d, run %4d, own %4d", a_uid, my.m_uid, s.st_uid);
   --rce;  if (a_loc == LOC_LOCAL) {
      if (s.st_uid != a_uid) {
         yURG_error ("crontab file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (my.m_uid == 0) {
         yURG_message ("  -- running as root, can verify/install any user crontab");
      } else if (my.m_uid == s.st_uid) {
         yURG_message ("  -- run-time user matches file ownership");
      } else {
         yURG_error ("run-time user does not match crontab file owner (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   --rce;  if (a_loc == LOC_CENTRAL) {
      if (my.m_uid == 0) {
         yURG_message ("  -- running as root, can audit any files");
      } else if (strcmp (my.m_user, a_user) == 0) {
         yURG_message ("  -- prefix matches user %s, can audit this file", my.m_user);
      } else {
         yURG_error ("user prefix in crontab name does not match run-time user (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner is not runtime user");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (s.st_uid != 0 || s.st_gid != 0) {
         yURG_error ("central crontab file owner/group is not root (security risk)");
         DEBUG_YEXEC  yLOG_note    ("central contab file owner/group is not root (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_message ("  -- central crontab owner and group is root");
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
   yURG_message ("  -- file permissions confirmed as owner-only write/read (0600)");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

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
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_message ("file acceptabliity...");
   /*---(defaults)-----------------------*/
   FILE__defaults ();
   /*---(naming)-------------------------*/
   rc = FILE__naming (LOC_LOCAL, a_name);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify user)--------------------*/
   rc = FILE__location (LOC_LOCAL, a_name, x_user, &x_uid, x_cwd);
   /*> yURG_error   ("rc = %d", rc);                                                  <*/
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stats)--------------------------*/
   rc = FILE__stats (LOC_LOCAL, x_cwd, a_name, x_user, x_uid);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update globals)-----------------*/
   strlcpy (my.f_name, a_name, LEN_HUND);
   strlcpy (my.f_user, x_user, LEN_USER);
   my.f_uid   = x_uid;
   strlcpy (my.f_desc, a_name + 8, LEN_DESC);
   sprintf (my.f_full, "%s%s", x_cwd, a_name);
   sprintf (my.f_new, "%s.%s", my.f_user, my.f_desc);
   my.f_ready = 'y';
   yURG_message ("  -- SUCCESS");
   yURG_message ("");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_central            (cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   int         l           =    0;
   int         i           =    0;
   char        x_cwd       [LEN_PATH]  = "";
   char        x_full      [LEN_PATH]  = "";
   char        x_user      [LEN_PATH]  = "";
   int         x_uid       =   -1;
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(naming)-------------------------*/
   rc = TABS_security ();
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defaults)-----------------------*/
   FILE__defaults ();
   yURG_message ("crontab file audit...");
   /*---(naming)-------------------------*/
   rc = FILE__naming (LOC_CENTRAL, a_name);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify user)--------------------*/
   rc = FILE__location (LOC_CENTRAL, a_name, x_user, &x_uid, x_cwd);
   /*> yURG_error   ("rc = %d", rc);                                                  <*/
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stats)--------------------------*/
   rc = FILE__stats (LOC_CENTRAL, my.n_central, a_name, x_user, x_uid);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update globals)-----------------*/
   strlcpy (my.f_name, a_name, LEN_HUND);
   strlcpy (my.f_user, x_user, LEN_HUND);
   my.f_uid   =  x_uid;
   strlcpy (my.f_desc, a_name + strlen (x_user) + 1 , LEN_DESC);
   sprintf (my.f_full, "%s%s", my.n_central, a_name);
   my.f_ready = 'y';
   yURG_message ("  -- SUCCESS");
   yURG_message ("");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_assimilate        (char a_loc, cchar *a_full)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(parse file)---------------------*/
   if      (a_loc == LOC_CENTRAL)   rc = FILE_central    (a_full);
   else if (a_loc == LOC_LOCAL  )   rc = FILE_acceptable (a_full);
   DEBUG_INPT   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create file)--------------------*/
   yURG_message ("assimilate file lines...");
   rc = FILE_create (my.f_name, my.f_user, my.f_uid);
   DEBUG_INPT   yLOG_value   ("file"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- created a list to house the lines");
   /*---(get current file)---------------*/
   yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      yURG_message ("  -- file failed");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySCHED_newfile ();
   DEBUG_INPT   yLOG_value   ("ySCHED"    , rc);
   --rce;  if (rc < 0) {
      yURG_message ("  -- schedule failed");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- initialized the valid date for scheduling");
   yURG_message ("  -- calling auto-reader");
   yURG_message ("");
   /*---(pull all lines)-----------------*/
   DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);
   rc = yPARSE_autoread (my.f_full, NULL, LINE_handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   --rce;  if (rc <  0) {
      strlcpy (x_file->note, "NO FILE" , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   c  = yDLST_line_count (YDLST_LOCAL);
   --rce;  if (c != x_file->lines) {
      yURG_message ("ERRORS, reviewed %d, accepted %d", x_file->lines, c);
      yURG_message ("");
      strlcpy (x_file->note, "ERRORS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   } else if (c == 0) {
      yURG_message ("EMPTY, reviewed %d, accepted %d", x_file->lines, c);
      yURG_message ("");
      strlcpy (x_file->note, "EMPTY"   , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("SUCCESS, reviewed %d, accepted %d", x_file->lines, c);
   yURG_message ("");
   strlcpy (x_file->note, "success"  , LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
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
      snprintf (unit_answer, LEN_HUND, "FILE full        : %2d[%.60s]", strlen (my.f_full), my.f_full);
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






