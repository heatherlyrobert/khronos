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
   a_cur->seq     =   -1;
   strlcpy (a_cur->title  , "" , LEN_HUND);
   strlcpy (a_cur->user   , "" , LEN_USER);
   a_cur->uid     =   -1;
   strlcpy (a_cur->note   , "-", LEN_TERSE);
   a_cur->lines   =    0;
   a_cur->retired =  '-';
   /*---(complete)-------------*/
   return 1;
}

char*
FILE__memory            (tFILE *a_cur)
{
   int         n           =    0;
   strlcpy (s_print, "[____.__._]", LEN_RECD);
   ++n;  if (a_cur->seq         >= 0)           s_print [n] = 'X';
   ++n;  if (a_cur->title   [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->user    [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->uid         >= 0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->note    [0] != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->lines       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->retired     != '-')         s_print [n] = 'X';
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

char
FILE_delete             (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   /*---(find list)----------------------*/
   rc = yDLST_list_by_name (a_name, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check still in use)-------------*/
   DEBUG_INPT   yLOG_value   ("lines"     , x_file->lines);
   --rce;  if (x_file->lines > 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(destroy)------------------------*/
   rc = yDLST_list_destroy (a_name);
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
/*===----                    contirm good crontabs                     ----===*/
/*====================------------------------------------====================*/
static void      o___CONFIRM_________________o (void) {;}

FILE__defaults          (void)
{
   yURG_err_clear ();
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
FILE__naming            (char a_loc, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   int         i           =    0;
   int         c           =    0;
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      yURG_err ('f', "crontab name can not be null or empty");
      DEBUG_INPT   yLOG_note    ("crontab name can not be null or empty");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   yURG_msg ('-', "name å%sæ is not empty/null", a_name);
   /*---(path)---------------------------*/
   p = strchr (a_name, '/');
   DEBUG_INPT   yLOG_point   ("/"         , p);
   --rce;  if (p != NULL) {
      yURG_err ('f', "crontab name can not include relative/absolute path (security risk)");
      DEBUG_INPT   yLOG_note    ("crontab name can not include relative/absolute path (security risk)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("cronfile name is path local (pathless)");
   yURG_msg ('-', "crontab name is pathless/local");
   /*---(name quality)-------------------*/
   l = strlen (a_name);
   DEBUG_INPT   yLOG_value   ("l"         , l);
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (LTRS_FILES, a_name [i]) == NULL) {
         yURG_err ('f', "crontab name has an illegal character (%c) at position %d (security risk)", a_name [i], i);
         DEBUG_INPT   yLOG_complex ("bad char"  , "can not include %c at %d", a_name [i], i);
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   DEBUG_INPT   yLOG_note    ("all name characters are acceptable");
   yURG_msg ('-', "all the file name characters are legal [A-Za-z0-9_.]");
   /*---(hidden file)--------------------*/
   DEBUG_INPT   yLOG_char    ("first char", a_name [0]);
   --rce;  if (a_name [0] == '.') {
      yURG_err ('f', "crontab can not be a hidden file, lead period (security risk)");
      DEBUG_INPT   yLOG_note    ("crontab can not be a hidden file");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "crontab is not a hidden file (no lead period)");
   /*---(exactly one period)-------------*/
   c = strldcnt (a_name, '.', LEN_RECD);
   DEBUG_INPT   yLOG_value   ("periods"   , c);
   --rce;  if (c < 1) {
      yURG_err ('f', "crontab name does not have at least one separator/period (standard)");
      DEBUG_INPT   yLOG_note    ("name must have exactly one separator/period");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "has at least one period/separator");
   /*---(description length)-------------*/
   p = strchr (a_name, '.');
   c = p - a_name;
   --rce;  if (c < 3) {
      yURG_err ('f', "crontab prefix can not be shorter than 3 characters (illegal)");
      DEBUG_INPT   yLOG_note    ("prefix too short (< 3 chars)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   c = l - c - 1;
   --rce;  if (c < 3) {
      yURG_err ('f', "crontab description can not be shorter than 3 characters (lazy)");
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
      yURG_err ('f', "can not obtain current working directory");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   if (a_loc == LOC_LOCAL)  strlcat (a_dir, "/", LEN_PATH);
   DEBUG_INPT   yLOG_info    ("a_dir"     , a_dir);
   yURG_msg ('-', "current working directory is %s", a_dir);
   /*---(check normal install)-----------*/
   --rce;  if (a_loc == LOC_LOCAL && my.m_uid != 0) {
      sprintf (t, "%s%s", my.n_home, my.m_user);
      l = strlen (t);
      if (strncmp (a_dir, t, l) != 0) {
         yURG_err ('f', "user not in or below their own home directory (security risk)");
         DEBUG_INPT   yLOG_note    ("user not in or below their own home directory");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      strlcpy (a_user, my.m_user, LEN_USER);
      yURG_msg ('-', "normal user crontab in or below home directory");
   }
   /*---(check root install)-------------*/
   --rce;  if (a_loc == LOC_LOCAL && my.m_uid == 0) {
      DEBUG_INPT   yLOG_note    ("root user verifying");
      sprintf (t, "%s", my.n_root);
      l = strlen (t);
      DEBUG_INPT   yLOG_complex ("cdir"      , "%2d å%sæ vs å%sæ", l, t, a_dir);
      if (strncmp (a_dir, t, l) == 0) {
         DEBUG_INPT   yLOG_note    ("root in root user home directory tree");
         strlcpy (a_user, "root", LEN_USER);
         yURG_msg ('-', "root user crontab in or below root home directory");
      } else {
         sprintf (t, "%s", my.n_home);
         l = strlen (t);
         if (strncmp (a_dir, t, l) == 0) {
            DEBUG_INPT   yLOG_note    ("root in another user home directory tree");
            strlcpy  (x_user, a_dir + l, LEN_PATH);
            strldchg (x_user, '/', '\0', LEN_PATH);
            strlcpy (a_user, x_user, LEN_USER);
            yURG_msg ('-', "root user crontab in or below %s users home directory", x_user);
         } else {
            yURG_err ('f', "root, but not in or below a valid user home directory (security risk)");
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
      yURG_msg ('-', "central cronfile prefix is %s", a_user);
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   /*---(check for registered)-----------*/
   rc = yEXEC_userdata (a_user, a_uid, NULL, NULL, NULL);
   DEBUG_INPT   yLOG_value   ("userdata"  , rc);
   if (rc < 0) {
      if (a_loc == LOC_LOCAL) {
         yURG_err ('f', "user directory not associated with a registered user (security risk)");
      } else {
         yURG_err ('f', "user prefix not associated with a registered user (security risk)");
      }
      DEBUG_INPT   yLOG_note    ("user is not registered in the system");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "file user is registered with system, %s, uid %d", a_user, *a_uid);
   /*---(name prefix)--------------------*/
   --rce;  if (a_loc == LOC_LOCAL) {
      if (strncmp (a_name, "khronos.", 8) != 0) {
         yURG_err ('f', "local crontab name must be prefixed with åkhronos.æ (standard)");
         DEBUG_INPT   yLOG_note    ("name does not begin with crontab");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "name is prefixed appropriately with åkhronos.æ");
   }
   --rce;  if (a_loc == LOC_CENTRAL) {
      if( my.m_uid != 0) {
         if (strcmp (my.m_user, a_user) != 0) {
            yURG_err ('f', "run-time user å%sæ can not review this file", my.m_user);
            DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         yURG_msg ('-', "run-time user has the appropriate access");
      } else {
         yURG_msg ('-', "run-time is root, so all access allowed");
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
      yURG_err ('f', "crontab file could not be located in %s", a_dir);
      DEBUG_INPT   yLOG_note    ("actual crontab file could not be found");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "located the actual file in %s", a_dir);
   /*---(check file type)----------------*/
   --rce;  if (S_ISDIR (s.st_mode))  {
      yURG_err ('f', "crontab file actually refers to a directory (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can not use a directory");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a directory");
   --rce;  if (S_ISLNK (s.st_mode))  {
      yURG_err ('f', "crontab file actually refers to a symbolic link (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can not use a symlink");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a symlink");
   --rce;  if (!S_ISREG (s.st_mode))  {
      yURG_err ('f', "crontab file is not a regular file (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can only use regular files");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("confirmed as regular file");
   yURG_msg ('-', "file is confirmed as a regular file, not a symlink/directory");
   /*---(ownership)----------------------*/
   DEBUG_YEXEC  yLOG_complex ("owner"     , "user %4d, run %4d, own %4d", a_uid, my.m_uid, s.st_uid);
   --rce;  if (a_loc == LOC_LOCAL) {
      if (s.st_uid != a_uid) {
         yURG_err ('f', "crontab file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (my.m_uid == 0) {
         yURG_msg ('-', "running as root, can verify/install any user crontab");
      } else if (my.m_uid == s.st_uid) {
         yURG_msg ('-', "run-time user matches file ownership");
      } else {
         yURG_err ('f', "run-time user does not match crontab file owner (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   --rce;  if (a_loc == LOC_CENTRAL) {
      if (my.m_uid == 0) {
         yURG_msg ('-', "running as root, can audit any files");
      } else if (strcmp (my.m_user, a_user) == 0) {
         yURG_msg ('-', "prefix matches user %s, can audit this file", my.m_user);
      } else {
         yURG_err ('f', "user prefix in crontab name does not match run-time user (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner is not runtime user");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (s.st_uid != 0 || s.st_gid != 0) {
         yURG_err ('f', "central crontab file owner/group is not root (security risk)");
         DEBUG_YEXEC  yLOG_note    ("central contab file owner/group is not root (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "central crontab owner and group is root");
   }
   /*---(permissions)--------------------*/
   DEBUG_ENVI   yLOG_value   ("perms"     , s.st_mode & 00777);
   if  ((s.st_mode & 00777) != 00600)  {
      yURG_err ('f', "crontab file permissions are not owner-only read/write 0600 (security risk)");
      DEBUG_ENVI   yLOG_note    ("permissions not 0600 (private to user)");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("permissions are 0600 (private)");
   yURG_msg ('-', "file permissions confirmed as owner-only write/read (0600)");
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
   yURG_msg ('>', "file acceptabliity...");
   /*---(defaults)-----------------------*/
   FILE__defaults ();
   /*---(naming)-------------------------*/
   rc = FILE__naming (LOC_LOCAL, a_name);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      /*> yURG_msg ('-', "naming check failed (%d)", rc);                             <*/
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify user)--------------------*/
   rc = FILE__location (LOC_LOCAL, a_name, x_user, &x_uid, x_cwd);
   /*> yURG_err ("rc = %d", rc);                                                  <*/
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      /*> yURG_msg ('-', "location check failed (%d)", rc);                           <*/
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stats)--------------------------*/
   rc = FILE__stats (LOC_LOCAL, x_cwd, a_name, x_user, x_uid);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      /*> yURG_msg ('-', "file stats check failed (%d)", rc);                         <*/
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
   yURG_msg ('-', "SUCCESS, crontab file acceptable");
   yURG_msg (' ', "");
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
   /*---(defaults)-----------------------*/
   FILE__defaults ();
   yURG_msg ('>', "crontab file audit...");
   /*---(naming)-------------------------*/
   rc = FILE__naming (LOC_CENTRAL, a_name);
   --rce;  if (rc < 0) {
      /*> yURG_msg ('-', "naming check failed (%d)", rc);                             <*/
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify user)--------------------*/
   rc = FILE__location (LOC_CENTRAL, a_name, x_user, &x_uid, x_cwd);
   /*> yURG_err ("rc = %d", rc);                                                  <*/
   --rce;  if (rc < 0) {
      /*> yURG_msg ('-', "location check failed (%d)", rc);                           <*/
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stats)--------------------------*/
   rc = FILE__stats (LOC_CENTRAL, my.n_central, a_name, x_user, x_uid);
   --rce;  if (rc < 0) {
      /*> yURG_msg ('-', "file stats check failed (%d)", rc);                         <*/
      yURG_msg (' ', "");
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
   yURG_msg ('-', "SUCCESS, crontab file audit complete");
   yURG_msg (' ', "");
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
   yURG_msg ('>', "assimilate file lines...");
   rc = FILE_create (my.f_name, my.f_user, my.f_uid);
   DEBUG_INPT   yLOG_value   ("file"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "created a list to house the lines");
   /*---(get current file)---------------*/
   yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      yURG_msg ('-', "file failed");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySCHED_newfile ();
   DEBUG_INPT   yLOG_value   ("ySCHED"    , rc);
   --rce;  if (rc < 0) {
      yURG_msg ('-', "schedule failed");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "initialized the valid date for scheduling");
   yURG_msg ('-', "calling auto-reader");
   yURG_msg (' ', "");
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
      yURG_msg ('>', "all lines read, ERRORS, reviewed %d, accepted %d", x_file->lines, c);
      yURG_msg (' ', "");
      strlcpy (x_file->note, "ERRORS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   } else if (c == 0) {
      yURG_msg ('>', "all lines read, EMPTY, reviewed %d, accepted %d", x_file->lines, c);
      yURG_msg (' ', "");
      strlcpy (x_file->note, "EMPTY"   , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('>', "all lines read, SUCCESS, reviewed %d, accepted %d", x_file->lines, c);
   yURG_msg (' ', "");
   strlcpy (x_file->note, "success"  , LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     retirement of files                      ----===*/
/*====================------------------------------------====================*/
static void      o___RETIRE__________________o (void) {;}

char
FILE_init               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(create retired)-----------------*/
   my.f_ready = 'y';
   rc = FILE_create ("RETIRED", "n/a", -1);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get current file)---------------*/
   yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update retired flag)------------*/
   x_file->retired = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_prune              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check all lines)----------------*/
   rc = yDLST_list_by_name ("RETIRED", NULL, NULL);
   rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   while (x_line != NULL) {
      /*---(mark retired)----------------*/
      if (x_line->retire == 'y' && x_line->rpid <= 0) {
         /*---(destroy inactive)------------*/
         rc = yDLST_line_remove  (x_line->tracker);
         DEBUG_INPT   yLOG_value   ("destroy"   , rc);
         if (rc < 0) {
            DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         LINE__free (&x_line);
         rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
         x_running = 0;
         continue;
      }
      ++x_running;
      rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DNEXT, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return x_running;
}

char
FILE_retire             (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   char        x_tracker   [LEN_TITLE] = "";
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "retire crontab lines...");
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      yURG_err ('f', "crontab name is null/empty");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   yURG_msg ('-', "crontab name requested is å%sæ", a_name);
   /*---(find list)----------------------*/
   rc = yDLST_list_by_name (a_name, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      yURG_msg ('-', "crontab is not assimilated, nothing to retire");
      yURG_msg (' ', "");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_file->retired = 'y';
   yURG_msg ('-', "marking file as retired as precaution");
   /*---(check still in use)-------------*/
   DEBUG_INPT   yLOG_value   ("lines"     , x_file->lines);
   rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   while (rc >= 0 && x_line != NULL) {
      /*---(report out)------------------*/
      DEBUG_INPT   yLOG_value   ("recdno"    , x_line->recdno);
      DEBUG_INPT   yLOG_info    ("tracker"   , x_line->tracker);
      DEBUG_INPT   yLOG_info    ("command"   , x_line->command);
      rc = yDLST_line_remove (x_line->tracker);
      DEBUG_INPT   yLOG_value   ("remove" , rc);
      if (rc < 0) {
         yURG_err ('w', "could not remove line å%sæ (%d)", x_line->tracker, rc);
         continue;
      }
      /*---(inactive)--------------------*/
      if (x_line->rpid <= 0) {
         DEBUG_INPT   yLOG_note    ("found inactive process, no worries");
         LINE__free (&x_line);
         yURG_msg ('-', "line å%sæ successfully retired");
         rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
         continue;
      }
      /*---(mark retired)----------------*/
      DEBUG_INPT   yLOG_note    ("found running process, move to retired");
      x_line->retire = 'y';
      /*---(find retired list)--------------*/
      rc = yDLST_list_by_name ("RETIRED", NULL, &x_file);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      --rce;  if (x_file == NULL) {
         yURG_err ('f', "retired list could not be found");
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(add line to retired)------------*/
      rc = yDLST_line_create (x_line->tracker, x_line);
      DEBUG_INPT   yLOG_value   ("create"    , rc);
      --rce;  if (rc < 0) {
         yURG_err ('w', "line could not be created å%sæ (%d)", x_line->tracker, rc);
         continue;
      }
      /*---(back to original list)----------*/
      rc = yDLST_list_by_name (a_name, NULL, &x_file);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      --rce;  if (x_file == NULL) {
         yURG_err ('f', "original list could not be found å%sæ (%d)", a_name, rc);
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(next line)----------------------*/
      yURG_msg ('-', "line å%sæ running, but successfully moved to retired list");
      rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
      DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
      /*---(done)---------------------------*/
   }
   /*---(destroy)------------------------*/
   rc = yDLST_list_destroy (a_name);
   DEBUG_INPT   yLOG_value   ("destroy"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "original list could not be destroyed å%sæ (%d)", a_name, rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "original list destroyed å%sæ ");
   yURG_msg (' ', "");
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






