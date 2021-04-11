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
   /*---(verify)-------------------------*/
   yURG_msg ('>', "%s", P_ONELINE);
   yURG_msg ('>', "  option --vaudit, check current khronos setup and security");
   yURG_msg (' ', "");
   yURG_msg ('>', "khronos central setup/security...");
   /*---(check on crontab file)-----------------*/
   strlcpy (x_path, "/var", LEN_PATH);
   rc = lstat (x_path, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "/var directory does not exist, FATAL");
      DEBUG_INPT   yLOG_note    ("can not open /var");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISDIR (s.st_mode))  {
      yURG_err ('f', "/var is not a direcotry");
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "/var directory exists");
   /*---(check on crontab file)-----------------*/
   strlcpy (x_path, "/var/spool", LEN_PATH);
   rc = lstat (x_path, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "/var/spool directory does not exist, FATAL");
      DEBUG_INPT   yLOG_note    ("can not open /var");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISDIR (s.st_mode))  {
      yURG_err ('f', "/var/spool is not a direcotry");
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "/var/spool directory exists");
   /*---(check on crontab file)-----------------*/
   strlcpy (x_path, "/var/spool/khronos", LEN_PATH);
   rc = lstat (x_path, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "/var/spool/khronos directory does not exist, FATAL");
      DEBUG_INPT   yLOG_note    ("can not open /var");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISDIR (s.st_mode))  {
      yURG_err ('f', "/var/spool/khronos is not a direcotry");
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "/var/spool/khronos directory exists");
   /*---(ownership)----------------------*/
   --rce;  if (s.st_uid != 0) {
      yURG_err ('f', "/var/spool/khronos not owned by root (security risk)");
      DEBUG_YEXEC  yLOG_note    ("/var/spool/khronos not owned by root (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("ownership is root (private)");
   yURG_msg ('-', "/var/spool/khronos directory ownership is root");
   --rce;  if (s.st_gid != 0) {
      yURG_err ('f', "/var/spool/khronos not group of root (security risk)");
      DEBUG_YEXEC  yLOG_note    ("/var/spool/khronos not group of root (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "/var/spool/khronos directory group ownership is root");
   DEBUG_YEXEC  yLOG_note    ("group ownership is root (private)");
   /*---(permissions)--------------------*/
   DEBUG_ENVI   yLOG_value   ("perms"     , s.st_mode & 00777);
   if  ((s.st_mode & 00777) != 00700)  {
      yURG_err ('f', "/var/spool/khronos root-only read/write/exec 0700 (security risk)");
      DEBUG_ENVI   yLOG_note    ("permissions not 0700 (private to user)");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "/var/spool/khronos directory pemissions are 0700");
   DEBUG_YEXEC  yLOG_note    ("permissions are 0600 (private)");
   yURG_msg ('-', "SUCCESS, khronos basic security measures confirmed");
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
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   /*---(just up/cur dirs)---------------*/
   if      (strcmp (a_name, "." ) == 0 || strcmp (a_name, "..") == 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
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
   return 1;
}

char
TABS__prepare      (cchar *a_regex, cchar a_act, DIR **a_dir, char *a_prefix)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_char    ("a_act"     , a_act);
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
      /*> printf ("fatal, can not open khronos central directory\n");                 <*/
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(user prefix)--------------------*/
   sprintf (a_prefix, "%s.", my.m_user);
   /*---(check security)-----------------*/
   --rce;  switch (a_act) {
   case ACT_AUDIT      : case ACT_CAUDIT     : case ACT_VAUDIT     :
      rc = TABS_security ();
      if (rc < 0) {
         if (a_act == ACT_CAUDIT  )   yURG_msg_live ();
         if (a_act == ACT_CAUDIT  )   yURG_msg ('>', "FAILED, central directory insecure, run --vaudit to identify reasons");
         if (a_act == ACT_VAUDIT  )   yURG_msg ('>', "FAILED, central directory insecure, the reasons are shown above");
         if (a_act == ACT_CAUDIT  )   yURG_msg_mute ();
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      break;
   }
   if (a_act == ACT_CAUDIT  )   yURG_msg_live ();
   if (a_act == ACT_CAUDIT  )   yURG_msg ('>', "SUCCESS, central directory proper and secure");
   if (a_act == ACT_CAUDIT  )   yURG_msg_mute ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}


char         /*--> review and act on global crontabs -------------------------*/
TABS_review        (cchar *a_regex, cchar a_act)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_total     =    0;
   int         x_count     =    0;
   int         x_pass      =    0;
   char        x_prefix    [LEN_USER]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = TABS__prepare (a_regex, a_act, &x_dir, x_prefix);
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
      if (rc != 0)  ++x_total;
      if (rc <= 0) {
         DEBUG_INPT   yLOG_note    ("not a valid crontab file");
         continue;
      }
      /*---(filter by user/accept)--------------*/
      /*> DEBUG_INPT   yLOG_info    ("name"      , x_file->d_name);                   <*/
      /*> rc = FILE_central (x_file->d_name);                                         <* 
       *> DEBUG_INPT   yLOG_value   ("central"   , rc);                               <* 
       *> if (rc < 0) {                                                               <* 
       *>    DEBUG_INPT   yLOG_note    ("not allowed for user, skipping");            <* 
       *>    continue;                                                                <* 
       *> }                                                                           <*/
      /*---(filter using regex)-----------------*/
      rc = yREGEX_exec (x_file->d_name);
      DEBUG_INPT   yLOG_value   ("exec"      , rc);
      if (rc <= 0) {
         DEBUG_INPT   yLOG_note    ("does not match regex, skipping");
         continue;
      }
      /*---(actions)----------------------------*/
      switch (a_act) {
      case ACT_LIST      :
         yURG_msg_live ();
         yURG_msg ('>', "%s", x_file->d_name);
         yURG_msg_mute ();
         rc = 0;
         break;
      case ACT_AUDIT     : case ACT_CAUDIT    : case ACT_VAUDIT    :
         yURG_msg (' ', "");
         rc = TABS_check   (a_act , x_file->d_name);
         break;
      }
      ++x_count;
      if (rc >= 0)  ++x_pass;
      DEBUG_INPT   yLOG_complex ("counts"    , "%d total, %d count, %d pass", x_total, x_count, x_pass);
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_INPT   yLOG_value   ("found"     , x_total);
   DEBUG_INPT   yLOG_value   ("processed" , x_count);
   DEBUG_INPT   yLOG_value   ("passed"    , x_pass);
   /*---(close)--------------------------*/
   rc = closedir (x_dir);
   DEBUG_INPT   yLOG_point   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(summary)------------------------*/
   rc = 1;
   --rce;  if (x_count <= 0) {
      DEBUG_INPT   yLOG_note    ("crontab count is zero");
      if (a_act == ACT_CAUDIT)     yURG_msg_live ();
      if (a_act == ACT_CAUDIT)     yURG_msg ('>', "WARNING, no crontabs found installed in central directory");
      if (a_act == ACT_VAUDIT)     yURG_msg ('>', "WARNING, no crontabs found installed in central directory");
      if (a_act == ACT_CAUDIT)     yURG_msg_mute ();
      rc = 0;
   }
   --rce;  if (x_count != x_pass) {
      DEBUG_INPT   yLOG_note    ("crontab count not equal passed");
      if (a_act == ACT_CAUDIT)     yURG_msg_live ();
      if (a_act == ACT_CAUDIT)     yURG_msg ('>', "WARNING, not all crontabs passed, only %d of %d", x_pass, x_count);
      if (a_act == ACT_VAUDIT)     yURG_msg ('>', "WARNING, not all crontabs passed, only %d of %d", x_pass, x_count);
      if (a_act == ACT_CAUDIT)     yURG_msg_mute ();
      rc = rce;
   }
   --rce;  if (x_total != x_count) {
      DEBUG_INPT   yLOG_note    ("crontab count not equal to total files");
      if (a_act == ACT_CAUDIT)     yURG_msg_live ();
      if (a_act == ACT_CAUDIT)     yURG_msg ('>', "WARNING, garbage file(s) found in central, %d unknown of %d", x_total - x_count, x_total);
      if (a_act == ACT_VAUDIT)     yURG_msg ('>', "WARNING, garbage file(s) found in central, %d unknown of %d", x_total - x_count, x_total);
      if (a_act == ACT_CAUDIT)     yURG_msg_mute ();
      rc = rce;
   }
   if (rc == 1) {
      DEBUG_INPT   yLOG_note    ("all results golden");
      if (a_act == ACT_CAUDIT)     yURG_msg_live ();
      if (a_act == ACT_CAUDIT)     yURG_msg ('>', "SUCCESS, %d file passed all checks", x_pass);
      if (a_act == ACT_VAUDIT)     yURG_msg ('>', "SUCCESS, %d file passed all checks", x_pass);
      if (a_act == ACT_CAUDIT)     yURG_msg_mute ();
      if (x_count > 100) x_count = 100;
      rc = x_count;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
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
/*===----                      incomming crontabs                      ----===*/
/*====================------------------------------------====================*/
static void      o___INCOMMING_______________o (void) {;}

char       /* PURPOSE : install a local crontab file -------------------------*/
TABS_verify             (cchar a_act, cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(verify)-------------------------*/
   yURG_msg ('>', "%s", P_ONELINE);
   yURG_msg ('>', "  option --vverify, check details of crontab file, but do not install");
   yURG_msg (' ', "");
   /*---(verify contents)--------------------*/
   rc = FILE_assimilate (LOC_LOCAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CVERIFY )   yURG_msg_live ();
      if (a_act == ACT_CVERIFY )   yURG_msg ('>', "FAILED, crontab uninstallable, run --vverify to identify reasons");
      if (a_act == ACT_VVERIFY )   yURG_msg ('>', "FAILED, crontab uninstallable, the reasons are shown above");
      if (a_act == ACT_CVERIFY )   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_act == ACT_CVERIFY )   yURG_msg_live ();
   yURG_msg ('>', "SUCCESS, crontab installable, but installation not requested");
   if (a_act == ACT_CVERIFY )   yURG_msg_mute ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : install a local crontab file -------------------------*/
TABS__intake            (cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_new       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "install crontab in central directory...");
   /*---(copy file)--------------------------*/
   snprintf (x_new, LEN_RECD, "%s%s", my.n_central, my.f_new);
   snprintf (x_cmd, LEN_RECD, "cp -f %s %s 2> /dev/null", my.f_name, x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not copy to %s", my.n_central);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_err ('f', "could not copy to %s", my.n_central);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "copied file to %s", my.n_central);
   /*---(change ownership)-------------------*/
   snprintf (x_cmd, LEN_RECD, "chown root:root %s 2> /dev/null", x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not change ownership to root");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_err ('f', "could not change ownership to root");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "changed owner and group to root");
   /*---(change permissions)-----------------*/
   snprintf (x_cmd, LEN_RECD, "chmod 0600 %s 2> /dev/null", x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not change permissions to root-only write/read (0600)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_err ('f', "could not change permissions to root-only write/read (0600)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "changed permissions to root-only write/read (0600)");
   yURG_msg ('-', "SUCCESS, file moved to central");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TABS_install            (cchar a_act, cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(verify)-------------------------*/
   yURG_msg ('>', "%s", P_ONELINE);
   yURG_msg ('>', "  option --vinstall, check details of crontab file and install if clean");
   yURG_msg (' ', "");
   /*---(verify contents)--------------------*/
   rc = FILE_assimilate (LOC_LOCAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CINSTALL)   yURG_msg_live ();
      if (a_act == ACT_CINSTALL)   yURG_msg ('>', "FAILED, crontab not installable, run --vinstall to identify reasons");
      if (a_act == ACT_VINSTALL)   yURG_msg ('>', "FAILED, crontab not installable, the reasons are shown above");
      if (a_act == ACT_CINSTALL)   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(install file)-----------------------*/
   rc = TABS__intake (a_name);
   DEBUG_INPT   yLOG_value   ("intake"    , rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CINSTALL)   yURG_msg_live ();
      if (a_act == ACT_CINSTALL)   yURG_msg ('>', "FAILED, crontab installable, but could not intake, run --vinstall to identify reasons");
      if (a_act == ACT_VINSTALL)   yURG_msg ('>', "FAILED, crontab installable, but could not intake, the reasons are shown above");
      if (a_act == ACT_CINSTALL)   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_act == ACT_CINSTALL)   yURG_msg_live ();
   yURG_msg ('>', "SUCCESS, crontab installed.  restart or khronos --reload to execute");
   if (a_act == ACT_CINSTALL)   yURG_msg_mute ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        central actions                       ----===*/
/*====================------------------------------------====================*/
static void      o___CENTRAL_________________o (void) {;}

char
TABS_check              (cchar a_act, cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(verify)-------------------------*/
   if (a_act == ACT_VCHECK) {
      yURG_msg ('>', "%s", P_ONELINE);
      yURG_msg ('>', "  option --vcheck, check details of installed crontab file");
      yURG_msg (' ', "");
   }
   if (a_act == ACT_VAUDIT) {
      yURG_msg ('>', "AUDITING %s ===================================", a_name);
      yURG_msg (' ', "");
   }
   /*---(verify contents)--------------------*/
   rc = FILE_assimilate (LOC_CENTRAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CCHECK  )   yURG_msg_live ();
      if (a_act == ACT_CAUDIT  )   yURG_msg_live ();
      if (a_act == ACT_CCHECK  )   yURG_msg ('>', "FAILED, crontab not runable, run --vcheck to identify reasons");
      if (a_act == ACT_VCHECK  )   yURG_msg ('>', "FAILED, crontab not runable, the reasons are shown above");
      if (a_act == ACT_CAUDIT  )   yURG_msg ('>', "%-30.30s  FAILED, crontab not runable, run --vcheck [file]", a_name);
      if (a_act == ACT_VAUDIT  )   yURG_msg ('>', "%-30.30s  FAILED, crontab not runable, reasons above", a_name);
      if (a_act == ACT_CCHECK  )   yURG_msg_mute ();
      if (a_act == ACT_CAUDIT  )   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_act == ACT_CCHECK  )   yURG_msg_live ();
   if (a_act == ACT_CAUDIT  )   yURG_msg_live ();
   if (a_act == ACT_CCHECK || a_act == ACT_VCHECK)   yURG_msg ('>', "SUCCESS, installed crontab runable, all lines check");
   if (a_act == ACT_CAUDIT  )   yURG_msg ('>', "%-30.30s  SUCCESS, installed crontab runable, all lines checked", a_name);
   if (a_act == ACT_CCHECK  )   yURG_msg_mute ();
   if (a_act == ACT_CAUDIT  )   yURG_msg_mute ();
   /*---(footer)-------------------------*/
   if (a_act == ACT_VAUDIT) {
      yURG_msg ('>', "COMPLETE %s ===================================", a_name);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      outgoing crontabs                       ----===*/
/*====================------------------------------------====================*/
static void      o___OUTGOING________________o (void) {;}

char
TABS_remove             (cchar a_act, cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_old       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(verify)-------------------------*/
   yURG_msg ('>', "%s", P_ONELINE);
   yURG_msg ('>', "  option --vremove, check central crontab, and remove if clean");
   yURG_msg (' ', "");
   /*---(verify contents)--------------------*/
   rc = FILE_central (a_name);
   DEBUG_INPT   yLOG_value   ("central"   , rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CREMOVE )   yURG_msg_live ();
      if (a_act == ACT_CREMOVE )   yURG_msg ('>', "FAILED, crontab not found/proper, run --vremove to identify reasons");
      if (a_act == ACT_VREMOVE )   yURG_msg ('>', "FAILED, crontab not found/proper, the reasons are shown above");
      if (a_act == ACT_CREMOVE )   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove physical)--------------------*/
   snprintf (x_old, LEN_RECD, "%s%s", my.n_central, my.f_name);
   snprintf (x_cmd, LEN_RECD, "rm -f %s 2> /dev/null", x_old);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   --rce;  if (rc < 0 || WIFEXITED (rc) < 0) {
      if (a_act == ACT_CREMOVE )   yURG_msg_live ();
      if (a_act == ACT_CREMOVE )   yURG_msg ('>', "FAILED, crontab could not be deleted, run --vremove for reasons");
      if (a_act == ACT_VREMOVE )   yURG_msg ('>', "FAILED, crontab could not be deleted, the reasons are shown above");
      if (a_act == ACT_CREMOVE )   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_act == ACT_CREMOVE )   yURG_msg_live ();
   yURG_msg ('>', "SUCCESS, crontab uninstalled.  restart or khronos --reload to retire");
   if (a_act == ACT_CREMOVE )   yURG_msg_mute ();
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
