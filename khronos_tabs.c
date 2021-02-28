/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"


char
TABS_security           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSTAT       s;
   char        x_path      [LEN_PATH]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   yURG_message ("khronos central setup/security...");
   /*---(check on crontab file)-----------------*/
   strlcpy (x_path, "/var", LEN_PATH);
   rc = lstat (x_path, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_error ("/var directory does not exist, FATAL");
      DEBUG_INPT   yLOG_note    ("can not open /var");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISDIR (s.st_mode))  {
      yURG_error ("/var is not a direcotry");
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  /var directory exists");
   /*---(check on crontab file)-----------------*/
   strlcpy (x_path, "/var/spool", LEN_PATH);
   rc = lstat (x_path, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_error ("/var/spool directory does not exist, FATAL");
      DEBUG_INPT   yLOG_note    ("can not open /var");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISDIR (s.st_mode))  {
      yURG_error ("/var/spool is not a direcotry");
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  /var/spool directory exists");
   /*---(check on crontab file)-----------------*/
   strlcpy (x_path, "/var/spool/khronos", LEN_PATH);
   rc = lstat (x_path, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_error ("/var/spool/khronos directory does not exist, FATAL");
      DEBUG_INPT   yLOG_note    ("can not open /var");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISDIR (s.st_mode))  {
      yURG_error ("/var/spool/khronos is not a direcotry");
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  /var/spool/khronos directory exists");
   /*---(ownership)----------------------*/
   --rce;  if (s.st_uid != 0) {
      yURG_error ("/var/spool/khronos not owned by root (security risk)");
      DEBUG_YEXEC  yLOG_note    ("/var/spool/khronos not owned by root (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("ownership is root (private)");
   yURG_message ("  /var/spool/khronos directory ownership is root");
   --rce;  if (s.st_gid != 0) {
      yURG_error ("/var/spool/khronos not group of root (security risk)");
      DEBUG_YEXEC  yLOG_note    ("/var/spool/khronos not group of root (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  /var/spool/khronos directory group ownership is root");
   DEBUG_YEXEC  yLOG_note    ("group ownership is root (private)");
   /*---(permissions)--------------------*/
   DEBUG_ENVI   yLOG_value   ("perms"     , s.st_mode & 00777);
   if  ((s.st_mode & 00777) != 00700)  {
      yURG_error ("/var/spool/khronos root-only read/write/exec 0700 (security risk)");
      DEBUG_ENVI   yLOG_note    ("permissions not 0700 (private to user)");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  /var/spool/khronos directory pemissions are 0700");
   DEBUG_YEXEC  yLOG_note    ("permissions are 0600 (private)");
   yURG_message ("  SUCCESS, khronos basic security measures confirmed");
   yURG_message ("");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                     reviewing directories                    ----===*/
/*====================------------------------------------====================*/
static void      o___REVIEW__________________o (void) {;}

char
TABS__filter            (cchar *a_name, cchar *a_prefix)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   int         i           =    0;
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   /*---(name quality)-------------------*/
   l = strlen (a_name);
   DEBUG_INPT   yLOG_value   ("l"         , l);
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (LTRS_FILES, a_name [i]) == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(hidden file)--------------------*/
   DEBUG_INPT   yLOG_char    ("first char", a_name [0]);
   --rce;  if (a_name [0] == '.') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(user prefix)--------------------*/
   --rce;  if (my.m_uid != 0) {
      if (strncmp (a_name, a_prefix, strlen (a_prefix)) != 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TABS__prepare      (cchar *a_regex, cchar a_action, DIR **a_dir, char *a_prefix)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_char    ("a_action"  , a_action);
   /*---(regex)--------------------------*/
   DEBUG_INPT   yLOG_point   ("a_regex"   , a_regex);
   --rce;  if (a_regex == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_regex"   , a_regex);
   rc = yREGEX_comp (a_regex);
   DEBUG_INPT   yLOG_value   ("comp"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("could not compile search");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open dir)-----------------------*/
   DEBUG_INPT   yLOG_info    ("n_central" , my.n_central);
   DEBUG_INPT   yLOG_point   ("a_dir"     , a_dir);
   --rce;  if (a_dir == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *a_dir = opendir (my.n_central);
   DEBUG_INPT   yLOG_point   ("*a_dir"    , *a_dir);
   --rce;  if (*a_dir == NULL) {
      RUN_USER     printf ("fatal, can not open khronos central directory\n");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(user prefix)--------------------*/
   sprintf (a_prefix, "%s.", my.m_user);
   /*---(output control)-----------------*/
   switch (a_action) {
   case ACT_LIST    : yURG_stdout ();   break;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}


char         /*--> review and act on global crontabs -------------------------*/
TABS_review        (cchar *a_regex, cchar a_action)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_count     =    0;
   int         x_total     =    0;
   char        x_prefix    [LEN_USER]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = TABS__prepare (a_regex, a_action, &x_dir, x_prefix);
   DEBUG_INPT   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0)  {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_INPT   yLOG_note    ("walk through directory files");
   while ((x_file = readdir (x_dir)) != NULL) {
      /*---(simple filtering)-------------------*/
      rc = TABS__filter (x_file->d_name, x_prefix);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("not a valid crontab file");
         continue;
      }
      /*---(filter by user/accept)--------------*/
      ++x_total;
      DEBUG_INPT   yLOG_info    ("name"      , x_file->d_name);
      rc = FILE_central (x_file->d_name);
      DEBUG_INPT   yLOG_value   ("central"   , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("not allowed for user, skipping");
         continue;
      }
      /*---(filter using regex)-----------------*/
      rc = yREGEX_exec (x_file->d_name);
      DEBUG_INPT   yLOG_value   ("exec"      , rc);
      if (rc <= 0) {
         DEBUG_INPT   yLOG_note    ("does not match regex, skipping");
         continue;
      }
      /*---(actions)----------------------------*/
      switch (a_action) {
      case ACT_REMOVE    :
         TABS_remove  (x_file->d_name);
         break;
      case ACT_LIST      :
         yURG_message ("%s", x_file->d_name);
         break;
      case ACT_FULLCHECK :
         rc = TABS_check   (x_file->d_name);
         yURG_stdout ();
         if (rc < 0)  yURG_message ("%-20.20s  FAILED", x_file->d_name);
         else         yURG_message ("%-20.20s  passed", x_file->d_name);
         yURG_noout  ();
         break;
      }
      ++x_count;
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_INPT   yLOG_value   ("found"     , x_total);
   DEBUG_INPT   yLOG_value   ("processed" , x_count);
   /*---(close)--------------------------*/
   rc = closedir (x_dir);
   DEBUG_INPT   yLOG_point   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return (x_count % 100);
}



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
tabs_clear_extfiles     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_name      [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(clear new)--------------------------*/
   snprintf (x_name, LEN_RECD, "%s%s.%s.NEW", my.n_central, my.m_user, my.f_desc);
   tabs__remove (x_name);
   /*---(clear del)--------------------------*/
   snprintf (x_name, LEN_RECD, "%s%s.%s.DEL", my.n_central, my.m_user, my.f_desc);
   tabs__remove (x_name);
   /*---(clear base)-------------------------*/
   snprintf (x_name, LEN_RECD, "%s%s.%s"    , my.n_central, my.m_user, my.f_desc);
   tabs__remove (x_name);
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
/*===----                       crontab actions                        ----===*/
/*====================------------------------------------====================*/
static void      o___ACTIONS_________________o (void) {;}

char
tabs_rename             (char a_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_src       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(temp source name)---------------*/
   DEBUG_INPT   yLOG_char    ("a_ext"     , a_ext);
   --rce;  if (a_ext == ACT_NEW) {
      snprintf (x_src, LEN_RECD, "%s%s.%s.NEW", my.n_central, my.f_user, my.f_desc);
   } else if (a_ext == ACT_DEL) {
      snprintf (x_src, LEN_RECD, "%s%s.%s.DEL", my.n_central, my.f_user, my.f_desc);
   } else {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_src"     , x_src);
   /*---(update naming)------------------*/
   strlcpy  (my.f_ext , ""      , LEN_SHORT);
   snprintf (my.f_name, LEN_HUND, "%s.%s", my.f_user, my.f_desc);
   snprintf (my.f_full, LEN_RECD, "%s%s.%s", my.n_central, my.f_user, my.f_desc);
   /*---(move file)--------------------------*/
   if (a_ext == ACT_NEW) {
      snprintf (x_cmd, LEN_RECD, "mv %s %s", x_src, my.f_full);
   } else {
      snprintf (x_cmd, LEN_RECD, "rm %s", x_src);
   }
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
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : install a local crontab file -------------------------*/
TABS__incomming         (char a_act, cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_new       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(verify)-------------------------*/
   DEBUG_INPT  yLOG_char    ("a_act"     , a_act);
   switch (a_act) {
   case ACT_VERIFY  : case ACT_VERINST :
      DEBUG_INPT  yLOG_note    ("turning on detailed error messages");
      yURG_stdout ();
      yURG_stderr ();
      break;
   }
   yURG_message ("%s %s", P_NAMESAKE, P_SUBJECT);
   switch (a_act) {
   case ACT_VERIFY  :
      yURG_message ("  --verify, check details of crontab file, but not install");
      break;
   case ACT_VERINST :
      yURG_message ("  --verinst, check details of crontab file and install if clean");
      break;
   }
   yURG_message ("");
   /*---(verify contents)--------------------*/
   rc = FILE_assimilate (LOC_LOCAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      yURG_stdout  ();
      switch (a_act) {
      case ACT_VERIFY  : case ACT_VERINST :
         yURG_message ("VERIFY failed, crontab uninstallable");
         yURG_message ("");
         break;
      case ACT_INSTALL :
         yURG_message ("crontab uninstallable, run --verify to identify reasons");
         yURG_message ("");
         break;
      }
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check verify only)------------------*/
   switch (a_act) {
   case ACT_VERIFY  :
      yURG_message ("requested only VERIFY, so no installation");
      yURG_message ("");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
      break;
   case ACT_VERINST :
      yURG_message ("requested VERIFY and INSTALL, so continuing to install");
      yURG_message ("");
      break;
   }
   yURG_message ("install crontab in central directory...");
   /*---(copy file)--------------------------*/
   snprintf (x_new, LEN_RECD, "%s%s", my.n_central, my.f_new);
   snprintf (x_cmd, LEN_RECD, "cp -f %s %s 2> /dev/null", my.f_name, x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_message ("  -- FATAL, could not copy to %s", my.n_central);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_message ("  -- FATAL, could not copy to %s", my.n_central);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- copied file to %s", my.n_central);
   /*---(change ownership)-------------------*/
   snprintf (x_cmd, LEN_RECD, "chown root:root %s 2> /dev/null", x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_message ("  -- FATAL, could not change ownership to root");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_message ("  -- FATAL, could not change ownership to root");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- changed owner and group to root");
   /*---(change permissions)-----------------*/
   snprintf (x_cmd, LEN_RECD, "chmod 0600 %s 2> /dev/null", x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_message ("  -- could not change permissions to root-only write/read (0600)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_message ("  -- could not change permissions to root-only write/read (0600)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("  -- changed permissions to root-only write/read (0600)");
   yURG_message ("");
   yURG_message ("SUCCESS, crontab installed.  restart or --hup khronos to make active.");
   yURG_message ("");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char TABS_install  (cchar *a_name) { return TABS__incomming (ACT_INSTALL, a_name); }
char TABS_verify   (cchar *a_name) { return TABS__incomming (ACT_VERIFY , a_name); }
char TABS_verinst  (cchar *a_name) { return TABS__incomming (ACT_VERINST, a_name); }

char       /* PURPOSE : install a local crontab file -------------------------*/
TABS__central           (char a_act, cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_new       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(verify)-------------------------*/
   DEBUG_INPT  yLOG_char    ("a_act"     , a_act);
   switch (a_act) {
   case ACT_AUDIT   :
      DEBUG_INPT  yLOG_note    ("turning on detailed error messages");
      yURG_stdout ();
      yURG_stderr ();
      break;
   }
   yURG_message ("%s %s", P_NAMESAKE, P_SUBJECT);
   switch (a_act) {
   case ACT_AUDIT   :
      yURG_message ("  --audit, re-check details of already installed crontab file");
      break;
   }
   yURG_message ("");
   /*---(verify contents)--------------------*/
   rc = FILE_assimilate (LOC_CENTRAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      yURG_stdout  ();
      switch (a_act) {
      case ACT_AUDIT   :
         yURG_message ("AUDIT failed, crontab not clean.  please fix original and --install");
         yURG_message ("");
         break;
      }
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_message ("");
   yURG_message ("SUCCESS, crontab audited successfully.");
   yURG_message ("");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char TABS_audit    (cchar *a_name) { return TABS__central (ACT_AUDIT, a_name); }
char TABS_check    (cchar *a_name) { return TABS__central (ACT_CHECK, a_name); }

char
TABS_remove             (cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_old       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(check name)-------------------------*/
   rc = FILE_central (a_name);
   DEBUG_INPT   yLOG_value   ("central"   , rc);
   --rce;  if (rc < 0 || my.f_ready != 'y') {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy file)--------------------------*/
   snprintf (x_old, LEN_RECD, "%s%s", my.n_central, my.f_name);
   snprintf (x_cmd, LEN_RECD, "rm -f %s 2> /dev/null", x_old);
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
      snprintf (unit_answer, LEN_HUND, "TABS who         : %2d[%.35s]", strlen (my.m_user), my.m_user);
   }
   else if (strcmp (a_question, "uid"           )  == 0) {
      snprintf (unit_answer, LEN_HUND, "TABS uid         : %d"        , my.m_uid);
   }
   else if (strcmp (a_question, "entry"         )  == 0) {
      /*---(find entry)---------------------*/
      x_dir  = opendir (my.n_central);
      if (x_dir == NULL) {
         snprintf (unit_answer, LEN_HUND, "TABS entry  (%2d) : can not open dir", a_num);
         return unit_answer;
      }
      x_file = readdir (x_dir);
      while (x_file != NULL) {
         rc = FILE_central (x_file->d_name);
         if (rc >= 0)  {
            if (c >= a_num)  break;
            ++c;
         }
         x_file = readdir (x_dir);
      }
      if (x_file != NULL) {
         snprintf (unit_answer, LEN_HUND, "TABS entry  (%2d) : [%s]"        , a_num, x_file->d_name);
      } else {
         snprintf (unit_answer, LEN_HUND, "TABS entry  (%2d) : []"          , a_num);
      }
      rc = closedir (x_dir);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*====================---------[[ end-of-code ]]----------====================*/
