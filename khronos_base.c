/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


struct cACCESSOR my;
char       *args[20];
int         failed;

char        testing      = 'n';

char        strtest      [100];

#define     MAX_DURATION    10
#define     MAX_CONTROL     10

struct cDURATION {
   char       name         [15];
   int        dur_min;
   int        dur_exp;
   int        dur_max;
} durations [MAX_DURATION] = {
   /*"123456789012", 123, 123, 123  */
   { "-"           ,   0,   0,   5  },
   { "tiny"        ,   0,   0,   2  },
   { "small"       ,   0,   2,  10  },
   { "medium"      ,   1,   5,  30  },
   { "large"       ,   5,  15,  60  },
   { "huge"        ,  15,  30, 120  },
   { "---null---"  ,   0,   0,   0  },
   { "---null---"  ,   0,   0,   0  },
   { "---null---"  ,   0,   0,   0  },
   { "---null---"  ,   0,   0,   0  },
};
struct cCONTROL  {
   char       name         [15];
   char       importance;
   char       monitoring;
   char       catchup;
   char       busy_delay;
   char       busy_skip;
   char       busy_kill;
} controls [MAX_CONTROL] = {
   /*"123456789012", 123, 123, 123, 123, 123, 123  */
   /* name-------- , imp, mon, cat, del, ski, kil  */
   { "-"           , '0', '-', '-', 'y', 'y', 'y'  },
   { "could"       , '0', '-', '-', 'y', 'y', 'y'  },
   { "like"        , '1', 'y', '-', 'y', 'y', 'y'  },
   { "want"        , '2', 'y', '-', 'y', 'y', '-'  },
   { "need"        , '3', 'y', 'y', 'y', 'y', '-'  },
   { "must"        , '4', 'y', 'y', 'y', '-', '-'  },
   { "absolute"    , '5', 'y', 'y', '-', '-', '-'  },
   { "---null---"  ,   0, '-', '-', '-', '-', '-'  },
   { "---null---"  ,   0, '-', '-', '-', '-', '-'  },
   { "---null---"  ,   0, '-', '-', '-', '-', '-'  },
   /* name-------- , imp, mon, cat, del, ski, kil  */
};


/*====================------------------------------------====================*/
/*===----                          utilities                           ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITIES_______________o (void) {;}

void      PROG_signal       (int a_signal, siginfo_t *a_info, void *a_nada) {return;};





/*====================------------------------------------====================*/
/*===----                         housekeeping                         ----===*/
/*====================------------------------------------====================*/
static void      o___HOUSEKEEPING____________o (void) {;}

/*> void       /+----: PURPOSE : handle signals ----------------------------------+/   <* 
 *> communicate        (int a_signal)                                                  <* 
 *> {                                                                                  <* 
 *>    /+---(catch)--------------------------+/                                        <* 
 *>    switch (a_signal) {                                                             <* 
 *>    case  SIGHUP:                                                                   <* 
 *>       yLOG_info     ("SIGNAL", "SIGHUP MEANS REFRESH CRONTABS BEFORE NEXT RUN");   <* 
 *>       my.resync = '-';                                                             <* 
 *>       break;                                                                       <* 
 *>    case  SIGTERM:                                                                  <* 
 *>       yLOG_info     ("SIGNAL", "SIGTERM means terminate daemon");                  <* 
 *>       BASE_endwatch ();                                                            <* 
 *>       yEXEC_term    ("EXITING", 99);                                               <* 
 *>       break;                                                                       <* 
 *>    case  SIGSEGV:                                                                  <* 
 *>       yLOG_info     ("SIGNAL", "SIGSEGV means daemon blew up");                    <* 
 *>       BASE_endwatch ();                                                            <* 
 *>       yEXEC_term    ("EXITING", 99);                                               <* 
 *>       break;                                                                       <* 
 *>    case  SIGABRT:                                                                  <* 
 *>       yLOG_info     ("SIGNAL", "SIGABRT means daemon blew up");                    <* 
 *>       BASE_endwatch ();                                                            <* 
 *>       yEXEC_term    ("EXITING", 99);                                               <* 
 *>       break;                                                                       <* 
 *>    }                                                                               <* 
 *>    /+---(reset)--------------------------+/                                        <* 
 *>    signals();                                                                      <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return;                                                                         <* 
 *> }                                                                                  <*/

/*> char       /+l---: PURPOSE : setup signal handling ---------------------------+/   <* 
 *> signals            (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(set)----------------------------+/                                        <* 
 *>    signal(SIGCHLD,  SIG_IGN);        /+ ignore children       +/                   <* 
 *>    signal(SIGTSTP,  SIG_IGN);        /+ ignore tty signals    +/                   <* 
 *>    signal(SIGTTOU,  SIG_IGN);        /+ ignore tty signals    +/                   <* 
 *>    signal(SIGTTIN,  SIG_IGN);        /+ ignore tty signals    +/                   <* 
 *>    signal(SIGHUP,   communicate);    /+ hangup means refresh  +/                   <* 
 *>    signal(SIGTERM,  communicate);    /+ catch a kill          +/                   <* 
 *>    signal(SIGSEGV,  communicate);    /+ catch a segfault      +/                   <* 
 *>    signal(SIGABRT,  communicate);    /+ catch a abort         +/                   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char               /* PURPOSE : daemonize the program ------------------------*/
daemonize          (void)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       i    = 0;                       /* loop iterator                 */
   int       fd   = 0;                       /* file descriptor               */
   int      status  = 0;
   /*---(fork off and die)----------------------*/
   RUN_DAEMON {
      yLOG_info  ("mode", "daemon mode requested");
      /*---(defense)------------------------*/
      if (getuid() != 0) {
         yLOG_info  ("FATAL"     , "not running as root");
         printf("FATAL : khronos can only be daemonized by root\n");
         exit (-1);
      }
      yEXEC_daemon (yLOG_lognum, &my.pid);
   } else {
      yLOG_info  ("mode", "foreground mode requested");
   }
   /*---(check security on crontabs)------------*/
   struct    stat s;
   int       rc = 0;
   rc = lstat(CRONTABS, &s);
   if  (rc < 0) {
      yLOG_info  ("crondir", "directory does not exist");
      yEXEC_term (__FUNCTION__, 6);
   }
   if  (!S_ISDIR(s.st_mode))  {
      yLOG_info  ("crondir", "not a directory");
      yEXEC_term (__FUNCTION__, 6);
   }
   yLOG_info  ("crondir", "directory existance verified");
   if  (s.st_uid  != 0)  {
      yLOG_info  ("crondir", "directory not owned by root");
      yEXEC_term (__FUNCTION__, 6);
   }
   if  (s.st_gid  != 0)  {
      yLOG_info  ("crondir", "directory group is not root");
      yEXEC_term (__FUNCTION__, 6);
   }
   if  ((s.st_mode & 00777) != 00700)  {
      yLOG_info  ("crondir", "directory permissions not 0700");
      yEXEC_term (__FUNCTION__, 6);
   }
   yLOG_info  ("security", "directory owned by root:root and 0700");
   /*---(change to safe location)---------------*/
   yLOG_info  ("location", "change current dir to a safe place");
   if (chdir(CRONTABS) < 0) {
      yLOG_info  ("cd",    "change directory FAILED");
      yEXEC_term (__FUNCTION__, 4);
   }
   /*---(signals)-------------------------------*/
   yLOG_info  ("signals",  "setup signal handlers");
   yEXEC_signal (yEXEC_SOFT, 'n', yEXEC_CNO, NULL);
   /*---(run file)------------------------------*/
   yLOG_info  ("unique"  , "test for single instance");
   lock();
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char       /* ---- : ensure single threading of the daemon -------------------*/
lock               (void)
{
   if (yEXEC_find ("khronos", NULL) > 1) {
      yLOG_info  ("FAILURE", "khronos already running");
      yEXEC_term ("FAILURE", 7);
   }
   return 0;
}

char        /* PURPOSE : final preparation for run                     */
prepare       (void)
{
   yLOG_info     ("watcher", "update watcher to /var/log/yLOG/cronwatch");
   BASE_begwatch ();
   search        ('a');
   yLOG_value    ("read in", n_cfile);
   my.resync  = 'n';
   /*---(complete)-------------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        crontab updates                       ----===*/
/*====================------------------------------------====================*/
static void      o___CRONTABS________________o (void) {;}

char        /* PURPOSE : search for and process crontab updates               */
search        (cchar a_scope)
{
   if (a_scope == '?' && my.resync == 'n')  return -1;
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tDIRENT  *den;
   DIR      *dir;
   int       count     = 0;
   int       len       = 0;
   char        x_file      [LEN_NAME];
   /*---(save scope)-----------------------------*/
   if (a_scope    != '?')  my.resync   = a_scope;
   /*---(open dir)------------------------------*/
   dir = opendir(CRONTABS);
   if (dir == NULL) {
      yLOG_info("crondir", "FATAL, not found");
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(report scope)--------------------------*/
   if (my.resync == 'a') yLOG_info("scope", "clear and load all files");
   else                  yLOG_info("scope", "look only for updated files");
   /*---(loop through files)--------------------*/
   while ((den = readdir(dir)) != NULL) {
      /*---(defense)----------------------------*/
      len       = strllen (den->d_name, LEN_NAME);
      if (len < 3)                continue;       /* one char each name/desc  */
      if (len >= LEN_NAME - 1)        continue;       /* max acceptable length    */
      strlcpy (x_file, den->d_name, LEN_NAME);
      if (x_file[len - 1] == '~') continue;       /* backup file              */
      /*---(changes)----------------------------*/
      strlcpy (my.action, "", LEN_ACTION);
      if (len > 7) {
         if (strncmp   (x_file + len - 3, "DEL", LEN_ACTION) == 0) {
            yLOG_info  ("DEL",   x_file);
            x_file [len - 4] = '\0';
            strlcpy    (my.action, "DEL", LEN_ACTION);
            assimilate (x_file);
         }
         if (strncmp    (x_file + len - 3, "NEW", LEN_ACTION) == 0) {
            yLOG_info  ("NEW",   x_file);
            x_file [len - 4] = '\0';
            rename     (den->d_name, x_file);
            strlcpy    (my.action, "NEW", LEN_ACTION);
            assimilate (x_file);
         }
      }
      ++count;
      /*---(normal files)-----------------------*/
      if (my.resync != 'a')       continue;
      ++count;
      assimilate(x_file);
   }
   closedir(dir);
   yLOG_value ("count",  count);
   /*---(delete)--------------------------------*/
   system("rm -f *.DEL");
   /*---(complete)------------------------------*/
   /*> my.resync = 'n';                                                               <*/
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : validate a single crontab name and process it        */
assimilate    (cchar *a_name)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   char      rc   = 0;                            /* simple return code       */
   FILE     *f    = NULL;                         /* crontab file decriptor   */
   tCFILE   *curr = NULL;                         /* current crontab struct   */
   /*---(check file name)-----------------------*/
   rc      = file_parse_name (a_name, '-');             /* vaildate name            */
   if (rc <  0) {
      TEST  printf("   crontab is misnamed\n");
      yLOG_warn  ("name",    "file name is not valid");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   TEST  printf("   name is good = <<%s>>\n", a_name);
   TEST  strlcpy (my.f_user, my.who, LEN_USER);
   /*---(check on crontab file)-----------------*/
   struct    stat s;
   rc = lstat(a_name, &s);
   if  (rc < 0) {
      TEST  printf("   crontab could not be found\n");
      yLOG_info  ("FAILED",  "crontab file does not exist");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab found\n");
   if  (!S_ISREG(s.st_mode))  {
      TEST  printf("   crontab is not a regular file\n");
      yLOG_info  ("FAILED",  "crontab is not a regular file");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab is a normal file\n");
   yLOG_info  ("file stat", "crontab is a normal file");
   /*---(elimintate exiting copy)---------------*/
   rc      = BASE_retire (my.name);
   if (strncmp(my.action, "DEL", LEN_ACTION) == 0) {
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(process new one)-----------------------*/
   f = fopen(a_name, "r");
   if (f == NULL) {
      TEST  printf("   crontab could not be openned\n");
      yLOG_info  ("FAILED",  "can not open the file");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab openned successfully\n");
   ySCHED_reset();
   rc = BASE_create (my.f_name, my.f_user, &curr);
   if (rc >= 0) rc = BASE_inventory (curr, f);
   /*---(complete)------------------------------*/
   fclose(f);
   yLOG_exit  (__FUNCTION__);
   return rc;
}

char
BASE_retire   (cchar *a_name)
{
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   tCFILE     *file;
   tCLINE     *line;
   tCFILE     *save;
   int         found       = 0;
   char        rc          = 0;
   /*---(process)------------------------*/
   DEBUG_DATA   yLOG_info    ("a_name"    , a_name);
   for (file = h_cfile; file != NULL; file = file->next) {
      DEBUG_DATA   yLOG_point   ("*file"     , file);
      DEBUG_DATA   yLOG_info    ("->name"    , file->name);
      DEBUG_DATA   yLOG_value   ("->nlines"  , file->nlines);
      /*---(defense)---------------------*/
      DEBUG_DATA   yLOG_char    ("->retire"  , file->retire);
      if (file->retire   == 'y')                  continue; /* already retired*/
      if (strncmp(a_name, file->name, LEN_NAME) != 0) continue; /* name wrong     */
      /*---(mark head)-------------------*/
      DEBUG_DATA   yLOG_note    ("match found");
      file->retire   = 'y';
      DEBUG_DATA   yLOG_char    ("->retire"  , file->retire);
      found = 1;
      /*---(process lines)---------------*/
      DEBUG_DATA   yLOG_note    ("check all lines");
      line = file->head;
      while (line != NULL) {
         DEBUG_DATA   yLOG_point   ("*line"     , line);
         DEBUG_DATA   yLOG_point   ("->file"    , line->file);
         DEBUG_DATA   yLOG_value   ("->recd"    , line->recd);
         DEBUG_DATA   yLOG_value   ("->rpid"    , line->rpid);
         line->deleted = 'y';
         /*---(running lines)------------*/
         if (line->rpid != 0) {
            DEBUG_DATA   yLOG_note    ("running, but marking for deletion");
            line = line->next;
         }
         /*---(non-running line)---------*/
         else {
            DEBUG_DATA   yLOG_note    ("freeing now");
            rc = CLINE_delete  (line);
            DEBUG_DATA   yLOG_value   ("rc_del"    , rc);
            line = file->head;
         }
      }
      DEBUG_DATA   yLOG_value   ("->nlines"  , file->nlines);
      if (file->nlines == 0 && file->retire == 'y') {
         save = file->next;
         CFILE_delete (file);
         file = save;
      }
      if (file == NULL) break;
   }
   /*---(summarize)-----------------------------*/
   if (found == 0) {
      DEBUG_DATA   yLOG_info    ("crontab",  "existing not found, continuing");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   DEBUG_DATA   yLOG_info    ("crontab",  "found and retired existing crontab");
   /*---(complete)------------------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return  0;
}

char        /* PURPOSE : delete all cronfiles and cronlines                   */
BASE_purge    (void)
{
   /*> /+---(locals)---------------------------------+/                                         <* 
    *> tCFILE   *file = h_cfile;                /+ current file                  +/            <* 
    *> int       rc   = 0;                                                                      <* 
    *> /+---(scan every file)--------------------+/                                             <* 
    *> while (file != NULL) {                                                                   <* 
    *>    /+> printf("purging %s\n", file->name);                                         <+/   <* 
    *>    rc = BASE_retire (file->name);                                                        <* 
    *>    if (rc < 0)       break;                                                              <* 
    *>    file = h_cfile;                                                                      <* 
    *>    if (file == NULL) break;                                                              <* 
    *> }                                                                                        <*/
   LIST_purge ();
   return 0;
}

char
BASE_create   (cchar *a_name, cchar *a_user, tCFILE **a_curr)
{
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tPASSWD  *pass = NULL;
   tCFILE   *file = NULL;
   /*---(create the space)----------------------*/
   file          = CFILE_create (UNLINKED);
   /*---(base data)-----------------------------*/
   strlcpy (file->user, a_user, LEN_USER);
   strlcpy (file->name, a_name, LEN_NAME);
   DEBUG_DATA   yLOG_info ("user"      , file->user);
   DEBUG_DATA   yLOG_info ("name"      , file->name);
   /*---(get uid)-------------------------------*/
   pass = getpwnam(a_user);                       /* get password entry       */
   if (pass == NULL) {
      DEBUG_DATA   yLOG_warn    ("user",  "user name not a valid account on the system");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   file->uid       = pass->pw_uid;
   DEBUG_DATA   yLOG_value ("UID",   file->uid);
   /*---(initialize the rest)-------------------*/
   file->retire    = 'n';
   /*---(link into file dll)--------------------*/
   CFILE_link   (file);
   /*---(finalize)------------------------------*/
   (*a_curr) = file;
   /*---(complete)------------------------------*/
   TEST  printf("   done\n\n");
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> parse a crontab version C record ------[ ------ [ ------ ]-*/
BASE_parse         (
      /*---(params)-----------+----------*/
      tCFILE     *a_cfile     ,        /* cronfile struct pointer             */
      char        a_type      ,        /* type of cronline format             */
      int         a_line      ,        /* line number from cronfile source    */
      char       *a_recd      )        /* record from crontab file            */
{  /*---(locals)-----------+-----------+-*/
   int         rc;                          /* generic return code            */
   char        rce         = -10;           /* return code for errors         */
   char       *p           = NULL;          /* strtok pointer                 */
   char       *q           = "\x1F";        /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   tCLINE     *x_cline     = NULL;          /* new line                       */
   int         i           = 0;             /* generic counter                */
   /*---(run input lines)----------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_point   ("*a_cfile"  , a_cfile);
   DEBUG_INPT   yLOG_char    ("a_type"    , a_type);
   DEBUG_INPT   yLOG_value   ("a_line"    , a_line);
   DEBUG_INPT   yLOG_point   ("*a_recd"   , a_recd);
   /*---(defense)------------------------*/
   --rce;  if (a_recd  == NULL) {
      DEBUG_INPT   yLOG_note    ("no record passed");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_recd"    , a_recd);
   --rce;  if (a_line  <= 0) {
      DEBUG_INPT   yLOG_note    ("line number from source file negative/invalid");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   if (a_cfile == NULL) {
      DEBUG_INPT   yLOG_note    ("no cron file struct pointer passed, test run");
   } else {
      DEBUG_INPT   yLOG_note    ("cron file struct pointer passed, creation run");
   }
   /*---(parse schedule)-----------------*/
   p = strtok_r (a_recd, q, &s);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_note    ("no scheduling grammar found");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("raw_sched" , p);
   rc = ySCHED_parse (NULL, p);
   DEBUG_INPT   yLOG_value   ("sched_rc"  , rc);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_note    ("scheduling grammar did not parse properly");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create a line)---------------*/
   x_cline = CLINE_create (NULL);
   DEBUG_INPT   yLOG_point   ("*x_cline"  , x_cline);
   --rce; if (x_cline == NULL) {
      DEBUG_INPT   yLOG_note    ("malloc for new entry failed");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_cline->recd         = a_line;
   /*---(save scheduling info)--------*/
   rc = ySCHED_save (&x_cline->sched);
   DEBUG_INPT   yLOG_value   ("save_rc"   , rc);
   --rce;  if (rc  < 0) {
      x_cline = CLINE_delete (x_cline);
      DEBUG_INPT   yLOG_note    ("grammar did not save properly");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(tracker)---------------------*/
   if (a_type >= 'B') {
      p = strtok_r (NULL , q, &s);
      --rce; if (p == NULL) {
         x_cline = CLINE_delete (x_cline);
         DEBUG_INPT   yLOG_note    ("parsing tracker failed");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      strltrim (p, ySTR_BOTH, LEN_LINE);
      strlcpy (x_cline->tracker, p, LEN_TRACKER);
      DEBUG_INPT   yLOG_info    ("tracker"   , x_cline->tracker);
   }
   /*---(description)-----------------*/
   if (a_type >= 'C') {
      p = strtok_r (NULL , q, &s);
      --rce; if (p == NULL) {
         x_cline = CLINE_delete (x_cline);
         DEBUG_INPT   yLOG_note    ("parsing description failed");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      strltrim (p, ySTR_BOTH, LEN_LINE);
      strlcpy (x_cline->comment, p, LEN_COMMENT);
      DEBUG_INPT   yLOG_info    ("comment"   , x_cline->comment);
   }
   /*---(profile)---------------------*/
   if (a_type >= 'D') {
      p = strtok_r (NULL , q, &s);
      --rce; if (p == NULL) {
         x_cline = CLINE_delete (x_cline);
         DEBUG_INPT   yLOG_note    ("parsing duration failed");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      strltrim (p, ySTR_BOTH, LEN_LINE);
      DEBUG_INPT   yLOG_info    ("profile"   , p);
      for (i = 0; i < MAX_DURATION; ++i) {
         if (durations [i].name [0] != p [0])     continue;
         if (strcmp (durations [i].name, p) != 0) continue;
         x_cline->dur_min    = durations [i].dur_min;
         x_cline->dur_exp    = durations [i].dur_exp;
         x_cline->dur_max    = durations [i].dur_max;
      }
      DEBUG_INPT   yLOG_value   ("dur_min"   , x_cline->dur_min);
      DEBUG_INPT   yLOG_value   ("dur_exp"   , x_cline->dur_exp);
      DEBUG_INPT   yLOG_value   ("dur_max"   , x_cline->dur_max);
   }
   /*---(control)---------------------*/
   if (a_type >= 'D') {
      p = strtok_r (NULL , q, &s);
      --rce; if (p == NULL) {
         x_cline = CLINE_delete (x_cline);
         DEBUG_INPT   yLOG_note    ("parsing control failed");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      strltrim (p, ySTR_BOTH, LEN_LINE);
      DEBUG_INPT   yLOG_info    ("controls"  , p);
      for (i = 0; i < MAX_CONTROL; ++i) {
         if (controls [i].name [0] != p [0])     continue;
         if (strcmp (controls [i].name, p) != 0) continue;
         x_cline->importance = controls [i].importance;
         x_cline->monitoring = controls [i].monitoring;
         x_cline->catchup    = controls [i].catchup;
         x_cline->busy_delay = controls [i].busy_delay;
         x_cline->busy_skip  = controls [i].busy_skip;
         x_cline->busy_kill  = controls [i].busy_kill;
      }
      DEBUG_INPT   yLOG_char    ("importance", x_cline->importance);
      DEBUG_INPT   yLOG_char    ("monitoring", x_cline->monitoring);
      DEBUG_INPT   yLOG_char    ("catchup"   , x_cline->catchup);
   }
   /*---(final data)-------------------------*/
   p = strtok_r (NULL , q, &s);
   --rce; if (p == NULL) {
      x_cline = CLINE_delete (x_cline);
      DEBUG_INPT   yLOG_note    ("parsing command failed");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   strltrim (p, ySTR_BOTH, LEN_LINE);
   strlcpy (x_cline->command, p, LEN_CMD);
   DEBUG_INPT   yLOG_info    ("command"   , x_cline->command);
   /*---(link into the file)----------*/
   if (a_cfile != NULL) {
      rc = CLINE_link (a_cfile, x_cline);
      DEBUG_INPT   yLOG_value   ("init_rc"   , rc);
      --rce;  if (rc  < 0) {
         x_cline = CLINE_delete (x_cline);
         DEBUG_INPT   yLOG_note    ("cronline did not attach properly");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_INPT   yLOG_point   ("->head"    , a_cfile->head);
      DEBUG_INPT   yLOG_point   ("->tail"    , a_cfile->tail);
      DEBUG_INPT   yLOG_value   ("->nlines"  , a_cfile->nlines);
   }
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return  0;
}

char         /*--> read and parse crontab lines ----------[ ------ [ ------ ]-*/
BASE_inventory     (
      /*---(params)-----------+----------*/
      tCFILE     *a_cfile     ,        /* pointer to khronos cronfile struct  */
      FILE       *a_source    )        /* actual crontab file pointer         */
{  /*---(locals)-----------+-----------+-*/
   int         rc          =   0;           /* generic return code            */
   char        rce         = -10;           /* return code for errors         */
   char        x_recd      [LEN_LINE];          /* input record                   */
   int         x_len       = 0;             /* length of input record         */
   int         recds       = 0;
   tCLINE     *cline;
   char       *p           = NULL;          /* strtok pointer                 */
   char       *q           = "\x1F";        /* strtok delimiters              */
   char       *s           = NULL;          /* strtok context                 */
   int         x_field     = 0;             /* field count                    */
   /*---(run input lines)----------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("*a_cfile"  , a_cfile);
   --rce;  if (a_cfile == NULL) {
      DEBUG_INPT   yLOG_error   ("cronfile"  , "no cronfile structure passed to function");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("->name"    , a_cfile->name);
   DEBUG_INPT   yLOG_point   ("*a_source" , a_source);
   --rce;  if (a_source == NULL) {
      DEBUG_INPT   yLOG_error   ("source"    , "no crontab file pointer passed to function");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(process lines)------------------*/
   while (1) {
      /*---(get the line)----------------*/
      fgets (x_recd, LEN_LINE - 5, a_source);
      if (feof (a_source))             break;
      x_len = strllen (x_recd, LEN_LINE) - 1;
      x_recd [x_len] = '\0';
      if (x_recd [x_len - 1] == '\x1F') x_recd [--x_len] = '\0';
      ++recds;
      DEBUG_INPT   yLOG_info    ("x_recd"    , x_recd);
      DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
      /*---(filter non-cron lines)-------*/
      if (x_recd [0] == ' ') {
         DEBUG_INPT   yLOG_note    ("line can not start with a space, skipping");
         continue;
      }
      if (x_recd [0] == '#') {
         DEBUG_INPT   yLOG_note    ("line starts with a comment indicator, skipping");
         continue;
      }
      if (x_len        <  10 ) {
         DEBUG_INPT   yLOG_note    ("line too short, skipping");
         continue;
      }
      /*---(get stats)-------------------*/
      x_field = strldcnt (x_recd, '', LEN_LINE);
      DEBUG_INPT   yLOG_value   ("fields"    , x_field);
      /*---(check for original format)---*/
      if (x_field == 0) {
         DEBUG_INPT   yLOG_note    ("found original (---) record");
         x_field = strldcnt (x_recd, ' ', LEN_LINE);
         DEBUG_INPT   yLOG_value   ("fields"    , x_field);
         if (x_field >= 5) {
            x_recd [strldpos (x_recd, ' ', 5, LEN_LINE)] = '\x1F';
            BASE_parse (a_cfile, 'A', recds, x_recd);
         }
      } else if (x_field == 1) {
         DEBUG_INPT   yLOG_note    ("found delimeted (-A-) record");
         BASE_parse (a_cfile, 'A', recds, x_recd);
      } else if (x_field == 2) {
         DEBUG_INPT   yLOG_note    ("found extended (-B-) record");
         BASE_parse (a_cfile, 'B', recds, x_recd);
      } else if (x_field == 3) {
         DEBUG_INPT   yLOG_note    ("found commented (-C-) record");
         BASE_parse (a_cfile, 'C', recds, x_recd);
      } else if (x_field == 5) {
         DEBUG_INPT   yLOG_note    ("found fullsome (-D-) record");
         BASE_parse (a_cfile, 'D', recds, x_recd);
      } else {
         DEBUG_INPT   yLOG_error   ("version"   , "version not understood");
      }
   }
   DEBUG_INPT   yLOG_point   ("*a_cfile"  , a_cfile);
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return  0;
}
/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
BASE_unit          (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   int       xfore  = 0, xback  = 0;  /* counts to verify doubly-linked list */
   tCFILE   *xlist    = NULL;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "BASE_unit        : question not understood", LEN_TEXT);
   /*---(pulse)--------------------------*/
   if        (strncmp(a_question, "cronpulse", 20)   == 0) {
      snprintf (unit_answer, LEN_TEXT, "BASE_pulse       : :%s   now   %d:", my.pulse_time, my.pid);
   }
   /*---(watch)--------------------------*/
   else if   (strncmp(a_question, "cronbegin", 20)   == 0) {
      snprintf (unit_answer, LEN_TEXT, "BASE_begwatch    : :%s:", my.pulse_begin, my.pid);
   } else if (strncmp(a_question, "cronend",   20)   == 0) {
      snprintf (unit_answer, LEN_TEXT, "BASE_endwatch    : :%s:", my.pulse_end  , my.pid);
   }
   /*---(crontab name)-------------------*/
   else if   (strncmp(a_question, "name", 20)        == 0) {
      snprintf (unit_answer, LEN_TEXT, "BASE_name name   : %.35s", my.f_name);
   } else if (strncmp(a_question, "user", 20)        == 0) {
      snprintf (unit_answer, LEN_TEXT, "BASE_name user   : %.35s", my.f_user);
   } else if (strncmp(a_question, "desc", 20)        == 0) {
      snprintf (unit_answer, LEN_TEXT, "BASE_name desc   : %.35s", my.f_desc);
   }
   /*---(crontab files)------------------*/
   else if   (strncmp(a_question, "cron count", 20)  == 0) {
      /***** always differentiate between malloc'd and linked *****/
      snprintf (unit_answer, LEN_TEXT, "cron counters    : file %4dm, %4dn;  line %5dm, %5dn", m_cfile, n_cfile, m_cline, n_cline);
   } else if (strncmp(a_question, "counters", 20)    == 0) {
      snprintf (unit_answer, LEN_TEXT, "counters         : %4d line, %4d fast, %4d proc", n_cline, nfast, nproc);
   } else if (strncmp(a_question, "file list", 20)   == 0) {
      /***** always check foreward and backward integrity also *****/
      xlist = h_cfile; while (xlist != NULL) { ++xfore; xlist = xlist->next; }
      xlist = t_cfile; while (xlist != NULL) { ++xback; xlist = xlist->prev; }
      snprintf (unit_answer, LEN_TEXT, "cronfile list    : n=%4d, f=%4d, b=%4d, h=%10p, t=%10p", n_cfile, xfore, xback, h_cfile, t_cfile);
   } else if (strncmp(a_question, "cron shape", 20)  == 0) {
      char   xshape[40] = "(empty)";
      BASE_unitshape (xshape);
      snprintf (unit_answer, LEN_TEXT, "cron shape (%3d) : %s", n_cfile + n_cline, xshape);
   }
   /*---(string testing)-----------------*/
   else if (strncmp(a_question, "string"    , 20)  == 0) {
      snprintf (unit_answer, LEN_TEXT, "BASE string      : [%s]", strtest);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
BASE_unitshape     (char *a_shape)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file;                           /* current file                  */
   tCLINE   *line;                           /* current line                  */
   int       count = 0;
   /*---(scan every file)--------------------*/
   for (file = h_cfile; file != NULL; file = file->next) {
      a_shape[count++] = 'F';
      a_shape[count  ] = '\0';
      if (count >= 35) return 0;
      /*---(scan every line)-----------------*/
      for (line = file->head; line != NULL; line = line->next) {
         a_shape[count++] = '_';
         a_shape[count  ] = '\0';
         if (count >= 35) return 0;
      }
   }
   return 0;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
BASE_unitfile      (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file;                           /* current file                  */
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---cron-list-----------------------------------\n");
   for (file = h_cfile; file != NULL; file = file->next) {
      printf("      FILE (%c) %3d = %-20.20s :: %-40.40s\n",
            file->retire, file->nlines, file->user, file->name);
      /*---(scan every line)-----------------*/
      for (line = file->head; line != NULL; line = line->next) {
         printf("           line (%c) <<%.50s>>\n", line->deleted, line->command);
      }
   }
   printf("      -----------------------------------------------\n");
   return 0;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
BASE_unitproc      (void)
{
   /*---(locals)---------------------------------*/
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---proc-list-----------------------------------\n");
   for (line = prochead; line != NULL; line = line->pnext) {
      printf("      (%6d) <<%.50s>>\n", line->rpid, line->command);
   }
   printf("      -----------------------------------------------\n");
   return 0;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
BASE_unitfast     (void)
{
   /*---(locals)---------------------------------*/
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---fast-list-----------------------------------\n");
   for (line = fasthead; line != NULL; line = line->fnext) {
      printf("      %-20.20s (%3d) %-40.40s\n", line->file->user, line->recd, line->file->name);
   }
   printf("      -----------------------------------------------\n");
   return 0;
}




/*=============================[[ end-of-code ]]==============================*/
