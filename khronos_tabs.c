/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"


char
tabs_set_path           (cchar *a_user, char a_scope)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strlcpy (my.f_path, "", LEN_PATH);
   /*---(check user)---------------------*/
   if (strcmp (a_user, "ALL") == 0)  rc = file_check_user ("root", LOC_CENTRAL);
   else                              rc = file_check_user (a_user, LOC_CENTRAL);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(build local dirs)---------------*/
   if (a_scope == LOC_LOCAL) {
      if      (my.user_mode == MODE_UNIT)    strlcpy  (my.f_path, DIR_UNIT_USER, LEN_PATH);
      else if (strcmp (a_user, "root") == 0) strlcpy  (my.f_path, DIR_ROOT     , LEN_PATH);
      else                                   snprintf (my.f_path, LEN_PATH, "/home/%s/%s", a_user, DIR_LOCAL);
   }
   /*---(build global dirs)--------------*/
   else if (a_scope == LOC_CENTRAL) {
      if      (my.user_mode == MODE_UNIT)    strlcpy  (my.f_path, DIR_UNIT_CENTRAL, LEN_PATH);
      else                                   strlcpy  (my.f_path, DIR_CENTRAL     , LEN_PATH);
   }
   /*---(summary)------------------------*/
   DEBUG_INPT   yLOG_info    ("f_path"    , my.f_path);
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     reviewing directories                    ----===*/
/*====================------------------------------------====================*/
static void      o___REVIEW__________________o (void) {;}

char         /*--> review and act on global crontabs -------------------------*/
tabs_global        (cchar *a_user, cchar a_action)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_count     =    0;
   int         x_total     =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   DEBUG_INPT   yLOG_char    ("a_action"  , a_action);
   DEBUG_INPT   yLOG_info    ("ACT_ALL"   , ACT_ALL);
   --rce;  if (strchr (ACT_ALL, a_action) == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for all user)-------------*/
   --rce;  if (strcmp (a_user, "ALL") == 0 && my.am_root != 'y') {
      RUN_USER     printf ("warn, must be logged in as root to use the --all option\n");
      DEBUG_INPT   yLOG_note    ("must be root to use the --all option");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(build dir name)-----------------*/
   rc = tabs_set_path (a_user, LOC_CENTRAL);
   DEBUG_INPT   yLOG_value   ("set path"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open dir)-----------------------*/
   DEBUG_INPT   yLOG_info    ("f_path"    , my.f_path);
   x_dir = opendir (my.f_path);
   DEBUG_INPT   yLOG_point   ("x_dir"     , x_dir);
   --rce;  if (x_dir == NULL) {
      RUN_USER     printf ("fatal, can not open crontab directory\n");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_INPT   yLOG_note    ("walk through directory files");
   x_file = readdir (x_dir);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   while (x_file  != NULL) {
      /*---(filter by name)---------------------*/
      DEBUG_INPT   yLOG_info    ("name"      , x_file->d_name);
      rc = file_parse_name (x_file->d_name, LOC_CENTRAL);
      DEBUG_INPT   yLOG_value   ("parse"     , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("not a crontab, SKIPPING");
         ++x_total;
         x_file = readdir (x_dir);
         DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
         continue;
      }
      /*---(root/daemon actions)----------------*/
      if (strcmp (a_user, "ALL") == 0) {
         switch (a_action) {
         case ACT_PURGE : tabs_delete (x_file->d_name);       break;
         case ACT_LIST  : printf ("%s\n", x_file->d_name);    break;
         case ACT_HUP   : if (strcmp (my.f_ext, "NEW") == 0) {
                             tabs_rename  (ACT_NEW);
                             file_request ();
                          }
                          else if (strcmp (my.f_ext, "DEL") == 0) {
                             tabs_rename  (ACT_DEL);
                             file_retire  ();
                          }
                          else {
                             --x_count;
                          }
                          break;
         case ACT_NONE  : break;
         }
         ++x_count;
      }
      /*---(normal user actions)----------------*/
      else if (strcmp (my.f_user, a_user) == 0) {
         switch (a_action) {
         case ACT_PURGE : tabs_delete (x_file->d_name);   break;
         case ACT_LIST  : printf ("%s\n", x_file->d_name); break;
         case ACT_NONE  : break;
         }
         ++x_count;
      }
      /*---(next)------------------------*/
      ++x_total;
      x_file = readdir (x_dir);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
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

char         /*--> review and act on local crontabs --------------------------*/
tabs_local         (cchar *a_user, cchar a_action)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_count     =    0;
   int         x_total     =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   DEBUG_INPT   yLOG_char    ("a_action"  , a_action);
   DEBUG_INPT   yLOG_info    ("ACT_ALL"   , ACT_ALL);
   --rce;  if (strchr (ACT_ALL, a_action) == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(build dir name)-----------------*/
   rc = tabs_set_path (a_user, LOC_LOCAL);
   DEBUG_INPT   yLOG_value   ("set path"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open dir)-----------------------*/
   x_dir = opendir (my.f_path);
   DEBUG_INPT   yLOG_point   ("x_dir"      , x_dir);
   --rce;  if (x_dir == NULL) {
      RUN_USER     printf ("fatal, can not open local crontab directory\n");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_INPT   yLOG_note    ("walk through directory files");
   x_file = readdir (x_dir);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   while (x_file  != NULL) {
      /*---(check name)-------------------------*/
      rc = file_parse_name (x_file->d_name, LOC_LOCAL);
      DEBUG_INPT   yLOG_value   ("parse"     , rc);
      /*---(filter files)-----------------------*/
      if (rc >= 0) {
         switch (a_action) {
         case ACT_INST  :  tabs_install (x_file->d_name);    break;
         case ACT_LIST  :  printf ("%s\n", x_file->d_name);  break;
         case ACT_NONE  :  break;
         }
         ++x_count;
      }
      /*---(next)------------------------*/
      ++x_total;
      x_file = readdir (x_dir);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
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
   snprintf (x_name, LEN_RECD, "%s%s.%s.NEW", my.dir_central, my.who, my.f_desc);
   tabs__remove (x_name);
   /*---(clear del)--------------------------*/
   snprintf (x_name, LEN_RECD, "%s%s.%s.DEL", my.dir_central, my.who, my.f_desc);
   tabs__remove (x_name);
   /*---(clear base)-------------------------*/
   snprintf (x_name, LEN_RECD, "%s%s.%s"    , my.dir_central, my.who, my.f_desc);
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
      snprintf (x_src, LEN_RECD, "%s%s.%s.NEW", my.dir_central, my.f_user, my.f_desc);
   } else if (a_ext == ACT_DEL) {
      snprintf (x_src, LEN_RECD, "%s%s.%s.DEL", my.dir_central, my.f_user, my.f_desc);
   } else {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_src"     , x_src);
   /*---(update naming)------------------*/
   strlcpy  (my.f_ext , ""      , LEN_ACT);
   snprintf (my.f_name, LEN_NAME, "%s.%s", my.f_user, my.f_desc);
   snprintf (my.f_full, LEN_RECD, "%s%s.%s", my.dir_central, my.f_user, my.f_desc);
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
tabs_install            (cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_name      [LEN_RECD]  = "";
   char        x_full      [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(check name)-------------------------*/
   rc = file_parse_name (a_name, LOC_LOCAL);
   DEBUG_INPT   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(build dir name)-----------------*/
   rc = tabs_set_path (my.who, LOC_LOCAL);
   DEBUG_INPT   yLOG_value   ("set path"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify source)------------------*/
   snprintf (x_name, LEN_RECD, "%s%s", my.f_path, my.f_name);
   DEBUG_INPT   yLOG_info    ("x_name"    , x_name);
   rc = tabs__verify  (x_name);
   DEBUG_INPT   yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   snprintf (x_full, LEN_RECD, "%s%s.%s.NEW", my.dir_central, my.who, my.f_desc);
   DEBUG_INPT   yLOG_info    ("x_full"    , x_full);
   /*---(clear files)------------------------*/
   rc = tabs_clear_extfiles  ();
   DEBUG_INPT   yLOG_value   ("clear"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy file to crontab dir)-----------*/
   snprintf (x_cmd, LEN_RECD, "cp %s %s", x_name, x_full);
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
   /*---(update file)------------------------*/
   rc = tabs__notify (x_full);
   DEBUG_INPT   yLOG_value   ("notify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : install a local crontab file -------------------------*/
tabs_delete             (cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_name      [LEN_RECD]  = "";
   char        x_full      [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(check name)-------------------------*/
   rc = file_parse_name (a_name, LOC_LOCAL);
   DEBUG_INPT   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify source)------------------*/
   snprintf (x_full, LEN_RECD, "%s%s.%s.DEL", my.dir_central, my.who, my.f_desc);
   DEBUG_INPT   yLOG_info    ("x_full"    , x_full);
   /*---(clear files)------------------------*/
   rc = tabs_clear_extfiles  ();
   DEBUG_INPT   yLOG_value   ("clear"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy file to crontab dir)-----------*/
   snprintf (x_cmd, LEN_RECD, "touch %s", x_full);
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
   /*---(update file)------------------------*/
   rc = tabs__notify (x_full);
   DEBUG_INPT   yLOG_value   ("notify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /* PURPOSE : test the contents of a crontab file ------------------*/
crontab_test       (cchar *a_source)
{
   /*---(locals)--------------------------------*/
   char      dir_name[LEN_PATH] = "";
   int       rc        = 0;
   char      x_file[500]= "";          /* file name                           */
   /*---(begin)---------------------------------*/
   DEBUG_INPT   printf("crontab_test()...\n");
   testing = 'y';
   failed  = 0;
   /*---(set source directory)------------------*/
   if (strcmp(my.who, "root") == 0) snprintf(dir_name, LEN_PATH, "/home/machine/crontabs/");
   else                             snprintf(dir_name, LEN_PATH, "/home/%s/c_quani/crontabs/", my.who);
   /*---(move to local directory)--------*/
   DEBUG_INPT   printf("   moved to local crontab directory\n");
   if (chdir(dir_name) < 0) {
      DEBUG_INPT   printf("   ");
      printf("could not move to crontab directory\n\n");
      return -4;
   }
   /*---(break up the file name)-------------*/
   snprintf (x_file, 500, "%s.%s", "crontab", a_source);
   DEBUG_INPT   printf("   starting with <<%s>>\n", x_file);
   rc = file_parse_name (x_file, '-');
   if (rc <  0) {
      DEBUG_INPT   printf("   ");
      printf("crontab name is found, but format is not valid [%d]\n\n", rc);
      return -2;
   }
   DEBUG_INPT   printf("   found properly named file\n");
   /*---(verify source file)-----------------*/
   rc = tabs__verify (x_file);
   if (rc != 1) {
      DEBUG_INPT   printf("   ");
      printf("crontab source file (%s) not found [%d]\n\n", x_file, rc);
      return -1;
   }
   /*---(assimilate)-------------------------*/
   assimilate(x_file);
   /*---(complete)------------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          specialty                           ----===*/
/*====================------------------------------------====================*/
static void      o___SPECIALTY_______________o (void) {;}

char
tabs_user               (cchar *a_user)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(check for root)-----------------*/
   DEBUG_INPT   yLOG_char    ("am root"   , my.am_root);
   --rce;  if (my.am_root != 'y') {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check name)---------------------*/
   rc = file_check_user (a_user, LOC_VERIFY);
   DEBUG_INPT   yLOG_value   ("check"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

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
   strlcpy  (unit_answer, "TABS             : question not understood", LEN_TEXT);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "file"          )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "TABS file        : %2d[%.35s]", strlen (my.f_name), my.f_name);
   }
   else if (strcmp (a_question, "user"          )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "TABS user        : %2d[%.35s]", strlen (my.f_user), my.f_user);
   }
   else if (strcmp (a_question, "desc"          )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "TABS desc        : %2d[%.35s]", strlen (my.f_desc), my.f_desc);
   }
   else if (strcmp (a_question, "ext"           )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "TABS ext         : %2d[%.35s]", strlen (my.f_ext), my.f_ext);
   }
   else if (strcmp (a_question, "path"          )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "TABS path        : %2d[%.35s]", strlen (my.f_path), my.f_path);
   }
   else if (strcmp (a_question, "who"           )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "TABS who         : %2d[%.35s]", strlen (my.who), my.who);
   }
   else if (strcmp (a_question, "uid"           )  == 0) {
      snprintf (unit_answer, LEN_TEXT, "TABS uid         : %d"        , my.uid);
   }
   else if (strcmp (a_question, "entry"         )  == 0) {
      /*---(find entry)---------------------*/
      x_dir  = opendir (my.dir_central);
      if (x_dir == NULL) {
         snprintf (unit_answer, LEN_TEXT, "TABS entry  (%2d) : can not open dir", a_num);
         return unit_answer;
      }
      x_file = readdir (x_dir);
      while (x_file != NULL) {
         rc = file_parse_name (x_file->d_name, LOC_CENTRAL);
         if (rc >= 0)  {
            if (c >= a_num)  break;
            ++c;
         }
         x_file = readdir (x_dir);
      }
      if (x_file != NULL) {
         snprintf (unit_answer, LEN_TEXT, "TABS entry  (%2d) : [%s]"        , a_num, x_file->d_name);
      } else {
         snprintf (unit_answer, LEN_TEXT, "TABS entry  (%2d) : []"          , a_num);
      }
      rc = closedir (x_dir);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*====================---------[[ end-of-code ]]----------====================*/
