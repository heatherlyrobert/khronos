/*===========================[[ start-of-code ]]==============================*/
#include      "cron.h"


/*---(file linked list)--------*/
tCFILE   *cronhead;
tCFILE   *crontail;
int       nfile  = 0;
int       nentry = 0;

/*---(fast path linked list)---*/
tCLINE   *fasthead;
tCLINE   *fasttail;
int       nfast  = 0;

/*---(processing linked list)--*/
tCLINE   *prochead;
tCLINE   *proctail;
int       nproc  = 0;




/*====================------------------------------------====================*/
/*===----                         housekeeping                         ----===*/
/*====================------------------------------------====================*/
static void      o___HOUSEKEEPING____________o (void) {;};

char        /* PURPOSE : early initialization                          */
initialize    (const char a_quiet)
{
   /*---(defense)-------------------------------*/
   if (getuid() != 0) {
      printf("crond can only be run by root\n");
      exit (-1);
   }
   if      (a_quiet == 0)  { my.quiet = 0; my.updates = 0; }
   else if (a_quiet == 1)  { my.quiet = 0; my.updates = 1; }
   else                    { my.quiet = 1; my.updates = 1; }
   /*---(begin)---------------------------------*/
   my.logger = yLOG_begin("crond", my.quiet);
   if (my.logger < 1) {
      printf("crond : can not start logger, FATAL\n");
      exit(1);
   }
   yLOG_info  ("purpose",  "consistent, reliable time-based job scheduling");
   yLOG_info  ("version",  "0.2f - building cronfile entries and SLL");
   yLOG_info  ("cli args", "none");
   /*---(init)----------------------------------*/
   yLOG_enter (__FUNCTION__);
   my.uid = getuid();
   yLOG_value ("uid num", my.uid);
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : exit on completion                            */
terminate     (const char *a_func, const int a_exit)
{
   if (strcmp(a_func, "") != 0) yLOG_exit  (a_func);
   /*---(complete)------------------------------*/
   yLOG_end   ();
   if (a_exit > 0) exit(a_exit);
   return 0;
}

void        /* PURPOSE : handle signals                                */
communicate   (const int a_signal)
{
   switch (a_signal) {
   case  SIGHUP:
      yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH CRONTABS BEFORE NEXT RUN");
      my.resync = '-';
      break;
   case  SIGTERM:
      yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon");
      terminate("EXITING", 99);
      break;
   case  SIGSEGV:
      yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up");
      terminate("EXITING", 99);
      break;
   }
   signals();    /* must reset signals after use         */
   /*---(complete)------------------------------*/
   return;
}

char        /* PURPOSE : setup signal handling                         */
signals       (void)
{
   signal(SIGCHLD,  SIG_IGN);        /* ignore child          */
   signal(SIGTSTP,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGTTOU,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGTTIN,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGHUP,   communicate);    /* hangup means refresh  */
   signal(SIGTERM,  communicate);    /* catch a kill          */
   signal(SIGSEGV,  communicate);    /* catch a segfault      */
   return 0;
}

char        /* PURPOSE : daemonize the program                         */
daemonize     (void)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       i    = 0;                       /* loop iterator                 */
   int       fd   = 0;                       /* file descriptor               */
   int      status  = 0;
   /*---(fork off and die)----------------------*/
   yLOG_info("foad", "fork off and die");
   my.pid = fork();
   if (my.pid < 0) {         /* error               */
      yLOG_info  ("fork",   "creation of child FAILED");
      terminate(__FUNCTION__, 1);
   }
   if (my.pid > 0) {         /* parent process      */
      yLOG_info  ("PARENT", "exiting parent");
      exit(0);
   }
   wait4(my.pid, &status, 0, NULL);
   /*---(fix the umask)-------------------------*/
   yLOG_info  ("umask",  "reset the default file permissions");
   umask(0);
   /*---(close off all descriptors)-------------*/
   yLOG_info  ("fds",    "close all inherited file descriptors");
   for (i = 0; i < 256; ++i) {
      if (i == my.logger) continue;
      close(i);
   }
   /*---(tie std fds to the bitbucket)----------*/
   yLOG_info  ("std fds",   "redirect stdin, stdout, and stderr to /dev/null");
   fd = open("/dev/null", O_RDWR);
   if (fd < 0) {
      yLOG_info  ("fds",    "creation of safe fd FAILED");
      terminate(__FUNCTION__, 2);
   }
   dup2(fd, 0);
   dup2(fd, 1);
   dup2(fd, 2);
   /*---(obtain a new process group)------------*/
   yLOG_info  ("session", "create a new process/session");
   my.sid = setsid();
   if (my.sid < 0) {
      yLOG_info  ("sid",    "creation FAILED");
      terminate(__FUNCTION__, 3);
   }
   /*---(change to safe location)---------------*/
   yLOG_info  ("location", "change current dir to a safe place");
   if (chdir(CRONTABS) < 0) {
      yLOG_info  ("cd",    "change directory FAILED");
      terminate(__FUNCTION__, 4);
   }
   /*---(record process info)-------------------*/
   my.pid  = getpid();
   my.ppid = getppid();
   yLOG_value ("new pid",  my.pid);
   yLOG_value ("new ppid", my.ppid);
   if (my.ppid != 1) {
      /*---(fork off and die)----------------------*/
      yLOG_info("foad", "fork off and die again");
      my.pid = fork();
      if (my.pid < 0) {         /* error               */
         yLOG_info  ("fork",   "creation of child FAILED");
         terminate(__FUNCTION__, 1);
      }
      if (my.pid > 0) {         /* parent process      */
         exit(0);
      }
      my.pid  = getpid();
      my.ppid = getppid();
      yLOG_value ("and pid",  my.pid);
      yLOG_value ("and ppid", my.ppid);
      if (my.ppid != 1) {
         yLOG_info  ("ppid",  "not owned by init, FAILED");
         terminate(__FUNCTION__, 5);
      }
   }
   yLOG_info  ("ppid",  "owned by init, success");
   /*---(signals)-------------------------------*/
   yLOG_info  ("signals",  "setup signal handlers");
   signals();
   /*---(run file)------------------------------*/
   yLOG_info  ("lockfile", "test for single crond instance");
   lock();
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : ensure single threading                       */
lock          (void)
{
   int        rc = 0;
   struct flock lk;
   my.locker = open(LOCKFILE, O_RDWR | O_CREAT, 0644);
   if (my.locker < 0) {
      yLOG_info  ("FAILURE", "crond already running (can not open file)");
      terminate("FAILURE", 7);
   }
   lk.l_type   = F_WRLCK;           /* exclusive lock            */
   lk.l_whence = 0;                 /* zero offset               */
   lk.l_start  = 0L;                /* start at BOF              */
   lk.l_len    = 0L;                /* whole file                */
   rc = fcntl(my.locker, F_SETLK, &lk);    /* non-blocking              */
   if (rc != 0) {
      yLOG_info  ("FAILURE", "crond already running (can not lock file)");
      terminate("FAILURE", 8);
   }
   yLOG_info  ("lock",     "write final pid to /var/run/crond.pid");
   char _msg[50];
   snprintf(_msg, 50, "%d\n", my.pid);
   write(my.locker, _msg, strlen(_msg));
   fsync(my.locker);
   return 0;
}

char        /* PURPOSE : update cronwatch interrun monitoring file     */
watch         (void)
{
   if (my.updates) return 0;
   FILE    *pulser;
   FILE    *watcher;
   char     buffer[50];
   int      len;
   /*---(get the last stop time)-----------------*/
   pulser = fopen(PULSER, "r");
   fgets(buffer, 50 , pulser);
   fclose(pulser);
   len    = strlen(buffer) - 1;
   buffer[len] = '\0';
   yLOG_info  ("last end", buffer);
   /*---(write the last end time)----------------*/
   watcher = fopen(WATCHER, "a");
   fprintf(watcher, "%s   end\n", buffer);
   /*---(get the date)---------------------------*/
   time_t      time_date = time(NULL);
   struct tm*  curr_time = localtime(&time_date);
   /*---(format the time)------------------------*/
   char  _time[50];
   strftime(_time, 50, "%y.%m.%d.%H.%M.%S.%U   %s", curr_time);
   yLOG_info  ("this beg", _time);
   /*---(write the time)-------------------------*/
   fprintf(watcher, "%s   BEG\n", _time);
   fflush(watcher);
   fclose(watcher);
   /*---(complete)-------------------------------*/
   return 0;
}

char        /* PURPOSE : final preparation for run                     */
prepare       (void)
{
   yLOG_info("watcher", "update watcher to /var/log/yLOG/cronwatch");
   watch();
   /*> yLOG_info("pulser",  "start pulsing to /var/log/yLOG/cronpulse");              <* 
    *> pulse();                                                                       <*/
   search('a');
   yLOG_value ("read in", nfile);
   my.resync = 'n';
   return 0;
}


/*====================------------------------------------====================*/
/*===----                        crontab updates                       ----===*/
/*====================------------------------------------====================*/
static void      o___CRONTABS________________o (void) {;};

char        /* PURPOSE : search for and process crontab updates               */
search_OLD    (cchar a_scope)
{
   if (a_scope == '?' && my.resync == 'n')  return -1;
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tDIRENT  *den;
   DIR      *dir;
   char     *name;
   int       count = 0;
   /*---(save scope)-----------------------------*/
   if (a_scope    != '?')  my.resync   = a_scope;
   /*---(open dir)------------------------------*/
   dir = opendir(CRONTABS);
   if (dir == NULL) {
      yLOG_info("crondir", "FATAL, not found");
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   if (my.resync == 'a') yLOG_info("scope", "clear and load all files");
   else                  yLOG_info("scope", "look only for updated files");
   while ((den = readdir(dir)) != NULL) {
      /*---(filter files)-----------------------*/
      if (den->d_name[0] ==  '.')                             continue;
      if (den->d_name[0] != '#') {
         if (my.resync == 'a') {
            assimilate (den->d_name);
            ++count;
         }
         continue;
      }
      if (strlen(den->d_name) < 2) continue;
      ++count;
      /*---(rename)-----------------------------*/
      name = den->d_name + 1;
      rename(den->d_name, name);
      /*---(process)----------------------------*/
      assimilate(name);
   }
   closedir(dir);
   yLOG_value ("count",  count);
   /*---(complete)------------------------------*/
   my.resync = 'n';
   yLOG_exit  (__FUNCTION__);
   /*---(run fast)------------------------------*/
   if (count > 0) fast (0, 0);
   /*---(complete)------------------------------*/
   return 0;
}

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
   char      x_file[NAME];
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
      len       = strlen(den->d_name);
      if (len < 3)                continue;       /* one char each name/desc  */
      if (len >= NAME - 1)        continue;       /* max acceptable length    */
      strncpy(x_file, den->d_name, NAME);
      if (x_file[len - 1] == '~') continue;       /* backup file              */
      /*---(changes)----------------------------*/
      strncpy(my.action, "", 5);
      if (len > 7) {
         if (strncmp(x_file + len - 3, "DEL", 5) == 0) {
            yLOG_info ("DEL",   x_file);
            x_file[len - 4] = '\0';
            strncpy(my.action, "DEL", 5);
            assimilate(x_file);
         }
         if (strncmp(x_file + len - 3, "NEW", 5) == 0) {
            yLOG_info ("NEW",   x_file);
            x_file[len - 4] = '\0';
            rename(den->d_name, x_file);
            strncpy(my.action, "NEW", 5);
            assimilate(x_file);
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
   my.resync = 'n';
   yLOG_exit  (__FUNCTION__);
   /*---(run fast)------------------------------*/
   if (count > 0) fast (0, 0);
   /*---(complete)------------------------------*/
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
   rc      = name (a_name, '-');                  /* vaildate name            */
   if (rc <  0) {
      yLOG_warn  ("name",    "file name is not valid");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   /*---(elimintate exiting copy)---------------*/
   rc      = retire (my.name);
   if (strncmp(my.action, "DEL", 5) == 0) {
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(process new one)-----------------------*/
   f = fopen(a_name, "r");
   if (f == NULL) {
      yLOG_info  ("FAILED",  "can not open the file");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   rc = create (my.name, my.user, &curr);
   if (rc >= 0) rc = inventory (curr, f);
   /*---(complete)------------------------------*/
   fclose(f);
   yLOG_exit  (__FUNCTION__);
   return rc;
}

char        /* PURPOSE : validate the crontab name                            */
name          (cchar *a_name, cchar a_loc)
{
   yLOG_enter (__FUNCTION__);
   yLOG_info  ("CRONTAB",   a_name);
   /*---(locals)--------------------------------*/
   char     *p;                                   /* pointer  for strtok      */
   int       len;                                 /* string's length          */
   int       i    = 0;                            /* loop iterator -- char    */
   char      legal[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
   char      x_name[NAME];        /* name of the current crontab               */
   /*---(defense)-------------------------------*/
   if (a_name[0] == '.') {
      yLOG_warn  ("name",  "can not process hidden crontab names");
      yLOG_exit  (__FUNCTION__);
      return -8;
   }
   len = strlen(a_name);
   if (len >= NAME) {
      yLOG_warn  ("name",  "crontab name too long to be processed");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   strncpy(x_name, a_name, NAME);
   /*---(get user name)-------------------------*/
   p = strtok(x_name, ".");
   if (p == NULL) {
      yLOG_warn  ("user",  "crontab file does not have a user name");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   len = strlen(p);
   if (len >= USER) {
      yLOG_warn  ("user",  "user name too long to be processed");
      yLOG_exit  (__FUNCTION__);
      return -3;
   }
   for (i = 0; i < len; ++i) {
      if (strchr(legal, p[i]) != NULL) continue;
      yLOG_warn  ("user",  "user name has illegal characters");
      yLOG_exit  (__FUNCTION__);
      return -4;
   }
   strncpy(my.user, p, USER);
   /*---(get crontab description)---------------*/
   p = strtok(NULL,   ".");
   if (p == NULL) {
      yLOG_warn  ("desc",  "crontab file does not have a description");
      yLOG_exit  (__FUNCTION__);
      return -5;
   }
   len = strlen(p);
   if (len >= DESC) {
      yLOG_warn  ("desc",  "decription too long to be processed");
      yLOG_exit  (__FUNCTION__);
      return -9;
   }
   for (i = 0; i < len; ++i) {
      if (strchr(legal, p[i]) != NULL) continue;
      yLOG_warn  ("desc",  "crontab description has illegal characters");
      yLOG_exit  (__FUNCTION__);
      return -6;
   }
   strncpy(my.desc, p, DESC);
   /*---(defense, improper format)--------------*/
   p = strtok(NULL,   ".");
   if (a_loc == 'c') {
      if (p != NULL && strcmp(p, "NEW") != 0 && strcmp(p, "DEL") != 0) {
         yLOG_info  ("name",  "crontab name is illegally suffixed");
         yLOG_exit  (__FUNCTION__);
         return -7;
      }
   } else {
      if (p != NULL) {
         yLOG_info  ("name",  "crontab name is illegally suffixed");
         yLOG_exit  (__FUNCTION__);
         return -7;
      }
   }
   /*---(complete)------------------------------*/
   strncpy(my.name, a_name, NAME);
   yLOG_info  ("success", "crontab name is legal");
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
retire        (cchar *a_name)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tCFILE   *file;
   tCLINE   *line;
   tCFILE   *save;
   int       found = 0;
   /*---(process)-------------------------------*/
   for (file = cronhead; file != NULL; file = file->next) {
      yLOG_info ("current", file->name);
      /*---(defense)-------------------------*/
      if (file->retire   == 'y')                  continue; /* already retired*/
      if (strncmp(a_name, file->name, NAME) != 0) continue; /* name wrong     */
      /*---(mark head)-----------------------*/
      file->retire   = 'y';
      found = 1;
      /*---(process lines)-------------------*/
      /*> printf("   FOR :: file=%s, lines=%d, retire=%c\n", file->name, file->nlines, file->retire);   <*/
      line = file->head;
      while (line != NULL) {
         line->deleted = 'y';
         /*---(running lines)-------------------*/
         if (line->rpid != 0) {
            /*> printf("   active job, mark\n");                                      <*/
            yLOG_bullet (line->recd, "running, but marking for deletion");
            line = line->next;
         }
         /*---(non-running line)----------------*/
         else {
            /*> printf("   inactive job, delete\n");                                  <*/
            yLOG_bullet (line->recd, "freeing now");
            cronline_del (line);
            line = file->head;
         }
         if (line == NULL) break;
      }
      /*> printf("   AFT :: file=%s, lines=%d, retire=%c\n", file->name, file->nlines, file->retire);   <*/
      if (file->nlines == 0 && file->retire == 'y') {
         /*> printf("   now empty file\n");                                           <*/
         save = file->next;
         cronfile_del (file);
         file = save;
      }
      if (file == NULL) break;
   }
   /*---(summarize)-----------------------------*/
   if (found == 0) {
      yLOG_warn  ("crontab",  "not found, continuing");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   yLOG_info  ("crontab",  "found and retired existing crontab");
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return  0;
}

char        /* PURPOSE : delete all cronfiles and cronlines                   */
purge         (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file = cronhead;                /* current file                  */
   int       rc   = 0;
   /*---(scan every file)--------------------*/
   while (file != NULL) {
      /*> printf("purging %s\n", file->name);                                         <*/
      rc = retire(file->name);
      if (rc < 0)       break;
      file = cronhead;
      if (file == NULL) break;
   }
   return 0;
}

char
create        (cchar *a_name, cchar *a_user, tCFILE **a_curr)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tPASSWD  *pass;
   tCFILE   *file = NULL;
   /*---(create the space)----------------------*/
   file          = malloc(sizeof(tCFILE));
   yLOG_point ("malloc",     file);
   if (file == NULL) {
      yLOG_warn  ("cronfile",  "malloc could not create space");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   /*---(base data)-----------------------------*/
   strncpy(file->name, a_name, NAME);
   strncpy(file->user, a_user, USER);
   yLOG_info ("user is", file->user);
   /*---(get uid)-------------------------------*/
   pass = getpwnam(a_user);                       /* get password entry       */
   if (pass == NULL) {
      yLOG_warn  ("user",  "user name not a valid account on the system");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   file->uid       = pass->pw_uid;
   yLOG_value ("UID",   file->uid);
   /*---(initialize the rest)-------------------*/
   file->retire    = 'n';
   /*---(link into file dll)--------------------*/
   cronfile_add (file);
   /*---(finalize)------------------------------*/
   (*a_curr) = file;
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
inventory     (tCFILE *a_cfile, FILE *a_source)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   char      buffer[LINE];
   char      backup[LINE];
   int       len   = 0;
   int       recds = 0;
   char     *xmin, *xhrs, *xday, *xmon, *xdow;
   char     *xtmp, *xcmd;
   int       rc;
   tCLINE   *cline;
   /*---(run input lines)-----------------------*/
   yLOG_point ("cfile",    a_cfile);
   while (1) {
      /*---(get the line)-----------------------*/
      fgets(buffer, LINE, a_source);
      if (feof(a_source))    break;
      len = strlen(buffer) - 1;
      buffer[len] = '\0';
      ++recds;
      /*---(defense)----------------------------*/
      if (buffer[0] == '#') {
         context (recds, buffer);
         continue;
      }
      if (len       <  10 )  continue;
      /*---(read)-------------------------------*/
      strncpy(backup, buffer, LINE);
      xmin = strtok(buffer, " ");
      xhrs = strtok(NULL  , " ");
      xday = strtok(NULL  , " ");
      xmon = strtok(NULL  , " ");
      xdow = strtok(NULL  , " ");
      xtmp = strtok(NULL  , " ");
      xcmd = backup + (xtmp - buffer);
      yLOG_break();
      /*---(create a line)----------------------*/
      cline = malloc(sizeof(tCLINE));
      if (cline == NULL) {
         yLOG_info  ("malloc", "failed");
         yLOG_exit  (__FUNCTION__);
         return -1;
      }
      yLOG_info  ("malloc", "good to go");
      /*---(link into line dll)-----------------*/
      cronline_add (a_cfile, cline);
      /*---(fill in master data)----------------*/
      cline->recd         = recds;
      if (recds == my.for_line + 1) {
         strncpy(cline->title, my.title, 20);
         cline->duration = my.duration;
         cline->resched  = my.resched;
         yLOG_delim ("title",   cline->title);
         yLOG_char  ("dur",     cline->duration);
         yLOG_char  ("resched", cline->resched);
      }
      /*---(parse)------------------------------*/
      rc = 0;
      if (rc == 0)  rc = parse (xmin, cline->min, 59, 0, "minutes");
      if (rc == 0)  rc = parse (xhrs, cline->hrs, 23, 0, "hours");
      if (rc == 0)  rc = parse (xday, cline->day, 31, 0, "days");
      if (rc == 0)  rc = parse (xmon, cline->mon, 12, 0, "months");
      if (rc == 0)  rc = parse (xdow, cline->dow,  6, 0, "weekdays");
      if (rc != 0) {
         yLOG_error ("parsing" , "line failed and is deleted");
         free(cline);
         continue;
      }
      /*---(final data)-------------------------*/
      yLOG_info ("cmd",  xcmd);
      strncpy(cline->cmd, xcmd, CMD);
   }
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return  0;
}

char               /* PURPOSE : read job context lines for additional data ---*/
context            (int a_recd, cchar *a_data)
{
   /*---(locals)--------------------------------*/
   int       len      = 0;
   int       i        = 0;                   /* loop iterator                 */
   char      legal[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
   char     *p;
   /*---(basic formatting)----------------------*/
   if (a_data[0]   != '#')            return 0;
   len = strlen(a_data);
   if (len <  80)                     return 0;
   /*---(formatting clues)----------------------*/
   if (a_data[47]  != '[')            return 0;
   if (a_data[68]  != ']')            return 0;
   if (a_data[69]  != '[')            return 0;
   if (a_data[79]  != ']')            return 0;
   /*---(parse)---------------------------------*/
   my.for_line = a_recd;
   my.duration = '-';
   my.resched  = '-';
   if (strchr("-SMLBZ", a_data[71]) != NULL)     my.duration = a_data[71];
   if (strchr("-qhtc",  a_data[73]) != NULL)     my.resched  = a_data[73];
   strncpy(my.title, a_data + 48, 21);
   my.title[20] = '\0';
   for (i = 19; i >= 0; --i) {
      if ((p = strchr(legal, my.title[i])) != NULL) break;
      my.title[i] = '\0';
   }
   /*---(complete)------------------------------*/
   return 0;
}

char
parse         (char *a_input, char *a_array, int a_max, int a_off, char *a_name)
{
   int       i    = 0;                       /* loop iterator                 */
   char     *list  = NULL;
   char     *step  = NULL;
   char     *range = NULL;
   char     *beg   = NULL;
   char     *end   = NULL;
   char     *weird = NULL;
   char     *s1    = NULL;
   char     *s2    = NULL;
   char     *s3    = NULL;
   int       step_inc = 0;
   int       beg_val  = 0;
   int       end_val  = 0;
   int       len      = 0;
   int       rc       = 0;
   for (i = 0; i <= a_max; ++i) {
      a_array[i]   = 0;
      my.parsed[i] = '_';
   }
   list  = strtok_r(a_input, ",", &s1);
   while (list != NULL) {
      /*> yLOG_info("list",    list);                                                 <*/
      range = strtok_r(list,  "/", &s2);
      /*> yLOG_info("range",   range);                                                <*/
      /*---(handle step)------------------------*/
      step_inc = 1;
      step     = strtok_r(NULL,  "/", &s2);
      /*> yLOG_info("step",     step);                                                <*/
      if (step != NULL) {
         rc = convert("step", step, 1, a_max);
         if (rc >= 1) step_inc = rc;
         else         return -10 + rc;
      }
      /*---(handle range)-----------------------*/
      if (range[0] == '-')  return -24;
      beg   = strtok_r(range, "-", &s3);
      /*> yLOG_info("beg",      beg);                                                 <*/
      len   = strlen(a_input);
      if (len == 1 && beg[0] == '*') {
         beg_val = a_off;
         end_val = a_max;
      } else {
         rc = convert("beg", beg, a_off, a_max);
         if (rc >= 0) {
            beg_val = rc;
            end_val = rc;
         } else         return -20 + rc;
         end   = strtok_r(NULL,  "-", &s3);
         /*> yLOG_info("end",      end);                                              <*/
         if (end != NULL) {
            rc = convert("end", end, beg_val, a_max);
            if (rc >= 0) end_val = rc;
            else         return -30 + rc;
         }
      }
      weird    = strtok_r(NULL,  "/", &s2);
      if (weird != NULL)  return -10;
      weird    = strtok_r(NULL,  "-", &s3);
      if (weird != NULL)  return -20;
      if (step_inc > 1 && (step_inc > (end_val - beg_val))) return -15;
      /*---(update)-----------------------------*/
      for (i = beg_val; i <= end_val; i += step_inc) {
         a_array[i] = 1;
      }
      /*---(next in list)-----------------------*/
      list  = strtok_r(NULL,  ",", &s1);
   }
   /*---(display)-------------------------------*/
   for (i = a_off; i <= a_max; ++i) {
      if (a_array[i] == 1) my.parsed[i] = '1';
      else                 my.parsed[i] = '_';
   }
   my.parsed[a_max + 1] = '\0';
   yLOG_info (a_name,   my.parsed);
   /*---(complete)------------------------------*/
   return  0;
}

int
convert       (const char *a_field, const char *a_input, const int a_min, const int a_max)
{
   int       i        = 0;
   int       len      = 0;
   int       value    = 0;
   if (a_input == NULL) return -1;
   len   = strlen(a_input);
   if (len > 2) {
      yLOG_info ("focus",    a_field);
      yLOG_info ("issue",    "value too long");
      yLOG_info ("input",    a_input);
      yLOG_info ("FAILED",   "whole line was rejected");
      return -2;
   }
   for (i = 0; i < len; ++i) {
      if (!isdigit(a_input[i])) {
         yLOG_info ("focus",    a_field);
         yLOG_info ("issue",    "value contains non-numeric values");
         yLOG_info ("input",    a_input);
         yLOG_info ("FAILED",   "whole line was rejected");
         return -3;
      }
   }
   value = atoi(a_input);
   if (value < a_min) {
      yLOG_info ("focus",    a_field);
      yLOG_info ("issue",    "value is less than 1");
      yLOG_info ("input",    a_input);
      yLOG_value("value",    value);
      yLOG_info ("FAILED",   "whole line was rejected");
      return -4;
   }
   if (value > a_max) {
      yLOG_info ("focus",    a_field);
      yLOG_info ("issue",    "value is tool large");
      yLOG_info ("input",    a_input);
      yLOG_value("value",    value);
      yLOG_info ("FAILED",   "whole line was rejected");
      return -5;
   }
   return value;
}



/*====================------------------------------------====================*/
/*===----                         job handling                         ----===*/
/*====================------------------------------------====================*/
static void      o___EXECUTION_______________o (void) {;};

char        /* PURPOSE : create list of jobs to run in a particular period    */
fast          (
      clong     a_start,                     /* time_t of starting hour       */
      cint      a_duration)                  /* number of hours to consider   */
{
   yLOG_enter (__FUNCTION__);
   yLOG_note  ("determine jobs that will run in the next hour");
   /*---(locals)---------------------------------*/
   int       chrs, cday, cmon, cdow;         /* simplifying temp variables    */
   char      msg[100];                       /* display string                */
   tCFILE   *x_file;                         /* current file                  */
   tCLINE   *x_line;                         /* current line                  */
   /*---(save time)------------------------------*/
   if (a_start    != 0)  my.fast_beg   = a_start;
   if (a_duration != 0)  my.fast_dur   = a_duration;
   /*---(get the date)---------------------------*/
   tTIME    *curr_time = localtime(&my.fast_beg);
   /*---(format the time)------------------------*/
   strftime(msg, 50, "---- %Hh %dd %mm  %ww", curr_time);
   yLOG_info ("fast for", msg);
   /*---(work out the current time)----------*/
   chrs = curr_time->tm_hour;
   cday = curr_time->tm_mday;
   cmon = curr_time->tm_mon;
   cdow = curr_time->tm_wday;
   /*---(prepare)-------------------------------*/
   nfast     = 0;
   fasthead  = NULL;
   fasttail  = NULL;
   /*---(scan every file)--------------------*/
   for (x_file = cronhead; x_file != NULL; x_file = x_file->next) {
      /*---(scan every line)-----------------*/
      for (x_line = x_file->head; x_line != NULL; x_line = x_line->next) {
         /*---(initialize)----------------------*/
         x_line->active  = 0;
         x_line->fnext   = NULL;
         x_line->fprev   = NULL;
         /*---(filter for active)---------------*/
         if (x_file->retire  == 'y')     continue;
         if (x_line->deleted == 'y')     continue;
         /*---(filter for time)-----------------*/
         if (x_line->hrs[chrs]  == 0)                               continue;
         if (x_line->mon[cmon]  == 0)                               continue;
         if (x_line->day[cday]  == 0 && x_line->dow[cdow]  == 0  )  continue;
         /*---(output)--------------------------*/
         snprintf(msg, 100, "%3d) %2dh %2dd %2dm %2dw %.30s (%d)", nfast,
               x_line->hrs[chrs], x_line->day[cday], x_line->mon[cmon],
               x_line->dow[cdow], x_file->name,      x_line->recd);
         yLOG_info  ("add",     msg);
         /*---(hook them up)--------------------*/
         if (nfast == 0) {
            fasthead        = x_line;
            fasttail        = x_line;
         } else {
            fasttail->fnext = x_line;
            fasttail        = x_line;
         }
         x_line->active  = 1;
         ++nfast;
      }
   }
   /*---(complete)------------------------------*/
   yLOG_value ("count", nfast);
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : run through the fast list to find what should run    */
dispatch      (cint a_min)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       njobs = 0;                      /* number of jobs ready to run   */
   int       ntest = 0;                      /* number of jobs ready to run   */
   tCLINE   *x_line;                         /* current line                  */
   char      msg[200];
   /*---(process all time periods)--------------*/
   yLOG_value ("potential",  nfast);
   for (x_line = fasthead; x_line != NULL; x_line = x_line->fnext) {
      ++ntest;
      snprintf(msg, 200, "%-16.16s,%3d", x_line->file->name, x_line->recd);
      if (x_line->min[a_min]  == 0) {
         yLOG_senter ("not");
         yLOG_snote(msg);
         yLOG_snote  ("not sched for min");
         yLOG_sexit  ();
         continue;
      }
      if (x_line->rpid        != 0) {
         yLOG_senter ("not");
         yLOG_snote(msg);
         yLOG_snote  ("already running");
         yLOG_sexit  ();
         continue;
      }
      run (x_line->file, x_line);
      ++njobs;
   }
   yLOG_value ("tested", ntest);
   yLOG_value ("ran",    njobs);
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
run        (tCFILE *a_file, tCLINE *a_line)
{
   yLOG_senter ("RUN");
   /*---(locals)--------------------------------*/
   int       rc        = 0;                       /* simple return code       */
   int       rpid      = 0;                       /* child pid for execution  */
   tPASSWD  *pass;
   FILE     *output = NULL;
   long      now;                                 /* present datetime         */
   tTIME    *curr_time = NULL;
   char      msg[200];
   char      envp[10][200];
   /*---(prepare)-------------------------------*/
   a_line->rpid   = 0;
   /*---(fork off to execute)-------------------*/
   snprintf(msg, 200, "%-16.16s,%3d", a_line->file->name, a_line->recd);
   yLOG_snote(msg);
   yLOG_snote("fork");
   rpid = fork();
   if (rpid < 0) {                        /* error                            */
      yLOG_snote ("FAILURE");
      yLOG_sexit ();
      return -1;
   }
   if (rpid > 0) {
      yLOG_svalue ("pid", rpid);
      /*---(link into proc dll)-----------------*/
      a_line->rpid       = rpid;
      proclist_add (a_line);
      /*---(log and exit)-----------------------*/
      yLOG_snote  ("SUCCESS");
      yLOG_sexit  ();
      return 0;              /* parent moves on to next task     */
   }
   output = fopen("/home/dotsuu/z_gehye/cronpulse.txt", "w");
   /*---(display header for debugging)----------*/
   fprintf(output, "=== crond : the heatherly cron system ==================================begin===\n");
   /*---(get the date)-----------------------*/
   now = time(NULL);
   curr_time = localtime(&now);
   strftime(msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);
   fprintf(output, "start     : %s\n",   msg);
   /*---(get user information)------------------*/
   pass = getpwnam(a_file->user);
   fprintf(output, "pass      : got the user information from passwd\n");
   if (pass == NULL) {
      exit (-2);
   }
   /*---(set execution environment)-------------*/
   rc = setenv("USER",                 pass->pw_name, 1);
   snprintf(envp[0], 200, "USER=%s",   pass->pw_name);
   fprintf(output, "USER set  : %s\n", pass->pw_name);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("HOME",                 pass->pw_dir,  1);
   snprintf(envp[1], 200, "HOME=%s",   pass->pw_dir);
   fprintf(output, "HOME set  : %s\n", pass->pw_dir);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("SHELL",                SHELL, 1);
   snprintf(envp[2], 200, "SHELL=%s",  SHELL);
   fprintf(output, "SHELL set : %s\n", SHELL);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("PATH",                 PATH, 1);
   snprintf(envp[3], 200, "PATH=%s",   PATH);
   fprintf(output, "PATH set  : %s\n", PATH);
   if (rc <  0) {
      exit (-3);
   }
   /*---(set permossions)-----------------------*/
   rc = initgroups(a_file->user, pass->pw_gid);
   fprintf(output, "groups    : read initgroups\n");
   if (rc <  0) {
      exit (-3);
   }
   rc = setregid(pass->pw_gid,         pass->pw_gid);
   snprintf(envp[4], 200, "GID=%d",    pass->pw_gid);
   fprintf(output, "GID set   : %d\n", pass->pw_gid);
   if (rc <  0) {
      return (-3);
   }
   rc = setreuid(pass->pw_uid, pass->pw_uid);
   snprintf(envp[5], 200, "UID=%d",    pass->pw_uid);
   fprintf(output, "UID set   : %d\n", pass->pw_uid);
   if (rc <  0) {
      return (-3);
   }
   /*---(set current dir)-----------------------*/
   rc = chdir(pass->pw_dir);
   fprintf(output, "chdir     : %s\n", pass->pw_dir);
   if (rc <  0) {
      return (-3);
   }
   /*---(close log)-----------------------------*/
   fprintf(output, "execl     : %.50s\n", a_line->cmd);
   fprintf(output, "========================================================================begin===\n");
   fflush (output);
   fclose (output);
   system("set > /home/dotsuu/z_gehye/environ.txt");
   /*---(execute)-------------------------------*/
   envp[6][0] = '\0';
   rc = execl(SHELL, SHELL, "-c", a_line->cmd, NULL, NULL);
   /*> rc = execle(SHELL, SHELL, "-c", a_line->cmd, NULL, envp);                      <*/
   /*---(they should never come back)-----------*/
   _exit (-3);    /* must use _exit to get out properly                       */
}

char        /* PURPOSE : verify status of running jobs and mark completions   */
check         (void)
{
   yLOG_enter (__FUNCTION__);
   /*---(defense)-------------------------------*/
   if (nproc == 0) {
      yLOG_note  ("no currently running jobs");
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(locals)--------------------------------*/
   int       rc        = 0;                       /* return code              */
   tCLINE   *x_line    = NULL;                    /* current line             */
   tCFILE   *x_file    = NULL;                    /* the line's file          */
   tCLINE   *x_pnext   = NULL;                    /* the next line to process */
   int       x_status  = 0;                       /* the line's job status    */
   char      msg[200];
   /*---(prepare)-------------------------------*/
   yLOG_value ("before", nproc);
   x_line    = prochead;
   /*---(loop)----------------------------------*/
   while (x_line != NULL) {
      /*---(save the next)----------------------*/
      x_pnext   = x_line->pnext;
      x_file    = x_line->file;
      /*---(output header)----------------------*/
      yLOG_senter("chk");
      snprintf(msg, 200, "%-15.15s,%3d", x_line->file->name, x_line->recd);
      yLOG_snote (msg);
      yLOG_sint  (x_line->rpid);
      /*---(check status)-----------------------*/
      rc = wait4(x_line->rpid, &x_status, WNOHANG, NULL);
      /*---(handle running line)----------------*/
      if (rc ==  0) {
         yLOG_snote ("still running");
      }
      /*---(handle completed line)--------------*/
      else {
         /*---(log status code)-----------------*/
         if (WIFEXITED(x_status)) {
            yLOG_snote ("exited");
            if      (WEXITSTATUS(x_status) == 0) yLOG_snote ("normal");
            else if (WEXITSTATUS(x_status) >  0) yLOG_snote ("positive");
            else                                 yLOG_snote ("FAILURE");
         } else {
            yLOG_snote ("TERMINATED");
         }
         /*---(clear from proc list)------------*/
         yLOG_snote ("clearing");
         proclist_del (x_line);
         /*---(clear from proc list)------------*/
         if (x_line->deleted == 'y') {
            yLOG_snote ("deleted");
            cronline_del (x_line);
            if (x_file->nlines == 0) cronfile_del (x_file);
         }
      }
      yLOG_sexit();
      x_line = x_pnext;
      if (x_line == NULL)    break;
   }
   yLOG_value ("after", nproc);
   yLOG_exit  (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          dll support                         ----===*/
/*====================------------------------------------====================*/
static void      o___LINKED_LISTS____________o (void) {;};

char        /* PURPOSE : add a cronfile to the file list                      */
cronfile_add (tCFILE *a_file)
{
   /*---(initialize)------------------*/
   a_file->head      = NULL;
   a_file->tail      = NULL;
   /*---(link into a_file dll)--------------------*/
   a_file->next      = NULL;
   a_file->prev      = NULL;
   if (cronhead == NULL) {
      cronhead       = a_file;
      crontail       = a_file;
   } else {
      a_file->prev   = crontail;
      crontail->next = a_file;
      crontail       = a_file;
   }
   /*---(overall)---------------------*/
   ++nfile;
   a_file->nlines    = 0;
   /*---(complete)--------------------*/
   return 0;
}

char        /* PURPOSE : remove a cronfile from the file list                 */
cronfile_del (tCFILE *a_file)
{
   if (a_file->nlines > 0) return -1;
   /*---(prev)------------------------*/
   if (a_file->prev  == NULL) cronhead           = a_file->next;
   else                       a_file->prev->next = a_file->next;
   /*---(next)------------------------*/
   if (a_file->next  == NULL) crontail           = a_file->prev;
   else                       a_file->next->prev = a_file->prev;
   /*---(overall)---------------------*/
   --nfile;
   if (nfile < 0) nfile = 0;
   free(a_file);
   /*---(complete)--------------------*/
   return 0;
}

char        /* PURPOSE : add a line to the cronfile list                      */
cronline_add (tCFILE *a_file, tCLINE *a_line)
{
   /*---(initialize)------------------*/
   a_line->file         = a_file;
   a_line->rpid         = 0;
   a_line->active       = 0;
   a_line->deleted      = 'n';
   strcpy(a_line->title, "");
   a_line->resched      = '-';
   a_line->duration     = '-';
   /*---(null pointers)---------------*/
   a_line->next         = NULL;
   a_line->prev         = NULL;
   /*---(null list)-------------------*/
   if (a_file->head == NULL) {
      a_file->head        = a_line;
      a_file->tail        = a_line;
   }
   /*---(existing list)---------------*/
   else {
      a_line->prev        = a_file->tail;
      a_file->tail->next  = a_line;
      a_file->tail        = a_line;
   }
   /*---(other pointers)--------------*/
   a_line->fnext        = NULL;
   a_line->fprev        = NULL;
   a_line->pnext        = NULL;
   a_line->pprev        = NULL;
   /*---(overall)---------------------*/
   a_file->nlines += 1;
   ++nentry;
   /*---(complete)--------------------*/
   return 0;
}

char        /* PURPOSE : remove a line from the cronfile list                 */
cronline_del (tCLINE *a_line)
{
   if (a_line->deleted != 'y') return -1;
   if (a_line->rpid    !=  0 ) return -2;
   /*---(prev)------------------------*/
   if (a_line->prev == NULL)   a_line->file->head = a_line->next;
   else                        a_line->prev->next = a_line->next;
   /*---(next)------------------------*/
   if (a_line->next == NULL)   a_line->file->tail = a_line->prev;
   else                        a_line->next->prev = a_line->prev;
   /*---(fast count)------------------*/
   if (a_line->active != 0) {
      --nfast;
      /*---(fast prev)-------------------*/
      if (a_line->fprev == NULL)  fasthead             = a_line->fnext;
      else                        a_line->fprev->fnext = a_line->fnext;
      /*---(fast next)-------------------*/
      if (a_line->fnext == NULL)  fasttail             = a_line->fprev;
      else                        a_line->fnext->fprev = a_line->fprev;
   }
   /*---(overall)---------------------*/
   a_line->file->nlines -= 1;
   if (a_line->file->nlines <  0) a_line->file->nlines = 0;
   free(a_line);
   --nentry;
   /*---(complete)--------------------*/
   return 0;
}

char        /* PURPOSE : add a line to the running process list               */
proclist_add (tCLINE *a_curr)
{
   /*---(initialize)------------------*/
   a_curr->pnext      = NULL;
   a_curr->pprev      = NULL;
   /*---(null list)-------------------*/
   if (prochead == NULL) {
      prochead          = a_curr;
      proctail          = a_curr;
      nproc             = 1;
   }
   /*---(existing list)---------------*/
   else {
      proctail->pnext   = a_curr;
      a_curr->pprev     = proctail;
      proctail          = a_curr;
      ++nproc;
   }
   /*---(complete)--------------------*/
   return 0;
}

char        /* PURPOSE : remove a line from the running process list          */
proclist_del (tCLINE *a_curr)
{
   /*---(prev)------------------------*/
   if (a_curr->pprev == NULL)  prochead             = a_curr->pnext;
   else                        a_curr->pprev->pnext = a_curr->pnext;
   /*---(next)------------------------*/
   if (a_curr->pnext == NULL)  proctail             = a_curr->pprev;
   else                        a_curr->pnext->pprev = a_curr->pprev;
   /*---(overall)---------------------*/
   a_curr->rpid = 0;
   --nproc;
   if (nproc < 0) nproc = 0;
   /*---(complete)--------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*
unit_accessor(char *a_question, int a_num)
{
   if        (strcmp(a_question, "parsed")      == 0) {
      snprintf(unit_answer, LEN_TEXT, "parsed frequency : %.35s", my.parsed);
   } else if (strcmp(a_question, "name")        == 0) {
      snprintf(unit_answer, LEN_TEXT, "crontab name     : %.35s", my.name);
   } else if (strcmp(a_question, "user")        == 0) {
      snprintf(unit_answer, LEN_TEXT, "user name        : %.35s", my.user);
   } else if (strcmp(a_question, "desc")        == 0) {
      snprintf(unit_answer, LEN_TEXT, "description      : %.35s", my.desc);
   } else if (strcmp(a_question, "cron count")  == 0) {
      snprintf(unit_answer, LEN_TEXT, "cron counters    : %4df %5de", nfile, nentry);
   } else if (strcmp(a_question, "counters")    == 0) {
      snprintf(unit_answer, LEN_TEXT, "counters         : %4d line, %4d fast, %4d proc", nentry, nfast, nproc);
   } else if (strcmp(a_question, "file list")   == 0) {
      snprintf(unit_answer, LEN_TEXT, "cronfile list    : %3d %10p %10p", nfile, cronhead, crontail);
   } else if (strcmp(a_question, "cron shape")  == 0) {
      char   xshape[40] = "(empty)";
      shape(xshape);
      snprintf(unit_answer, LEN_TEXT, "cron shape (%3d) : %s", nfile + nentry, xshape);
   } else {
      snprintf(unit_answer, LEN_TEXT, "unknown          : unit_accessor question not defined");
   }
   return unit_answer;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
shape         (char *a_shape)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file;                           /* current file                  */
   tCLINE   *line;                           /* current line                  */
   int       count = 0;
   /*---(scan every file)--------------------*/
   for (file = cronhead; file != NULL; file = file->next) {
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
list_cron     (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file;                           /* current file                  */
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---cron-list-----------------------------------\n");
   for (file = cronhead; file != NULL; file = file->next) {
      printf("      FILE (%c) %3d = %-20.20s :: %-40.40s\n",
            file->retire, file->nlines, file->user, file->name);
      /*---(scan every line)-----------------*/
      for (line = file->head; line != NULL; line = line->next) {
         printf("           line (%c) <<%.50s>>\n", line->deleted, line->cmd);
      }
   }
   printf("      -----------------------------------------------\n");
   return 0;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
list_fast     (void)
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

char        /* PURPOSE : specifically built list to aid unit testing          */
list_proc     (void)
{
   /*---(locals)---------------------------------*/
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---proc-list-----------------------------------\n");
   for (line = prochead; line != NULL; line = line->pnext) {
      printf("      (%6d) <<%.50s>>\n", line->rpid, line->cmd);
   }
   printf("      -----------------------------------------------\n");
   return 0;
}


/*=============================[[ end-of-code ]]==============================*/
