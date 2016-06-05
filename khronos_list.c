/*====================--------[[ start-of-code ]]---------====================*/

/*===[[ SUMMARY ]]============================================================#

 *   application   : khronos
 *   module        : khronos_list
 *
 */
/*===[[ SUMMARY ]]============================================================#

 *   khronos_list is the linked list support functions to manage the data
 *   associated with crontab files, lines, and running processes
 *
 */
/*============================================================================*/


#include   "khronos.h"


/*---(file linked list)--------*/
tCFILE   *h_cfile;
tCFILE   *t_cfile;
int       m_cfile;
int       n_cfile;
int       m_cline;
int       n_cline;

/*---(fast path linked list)---*/
tCLINE   *fasthead;
tCLINE   *fasttail;
int       nfast;

/*---(processing linked list)--*/
tCLINE   *prochead;
tCLINE   *proctail;
int       nproc;



/*====================------------------------------------====================*/
/*===----                            overall                           ----===*/
/*====================------------------------------------====================*/
static void      o___OVERALL_________________o (void) {;}

char
LIST_purge         (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *x_cfile;                        /* current file                  */
   tCFILE   *x_save;                         /* current file                  */
   /*---(scan every file)--------------------*/
   x_cfile = h_cfile;
   while (x_cfile != NULL) {
      x_save  = x_cfile->next;
      x_cfile = CFILE_purge (x_cfile);
      x_cfile = x_save;
   }
   return -(n_cfile);
}



/*====================------------------------------------====================*/
/*===----                           cron files                         ----===*/
/*====================------------------------------------====================*/
static void      o___CRON_FILES______________o (void) {;}

tCFILE*      /*--> create a new cronfile -----------------[ leaf   [ ------ ]-*/
CFILE_create       (char a_linked)
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tCFILE     *entry       = NULL;
   /*---(create a file)------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   entry = malloc (sizeof (tCFILE));
   DEBUG_DATA   yLOG_spoint  (entry);
   --rce; if (entry == NULL) {
      DEBUG_INPT   yLOG_snote   ("malloc failed");
      DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
      return NULL;
   }
   /*---(overall)------------------------*/
   DEBUG_DATA   yLOG_snote   ("nulling ... overall");
   entry->name     [0] = '\0';
   entry->user     [0] = '\0';
   entry->uid          =  0;
   entry->retire       = '-';
   entry->nlines       =  0;
   /*---(initialize)------------------*/
   DEBUG_DATA   yLOG_snote   ("pointers");
   entry->head         = NULL;
   entry->tail         = NULL;
   entry->next         = NULL;
   entry->prev         = NULL;
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   ++m_cfile;
   /*---(check link request)----------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   if (a_linked == 'y') {
      DEBUG_DATA   yLOG_snote   ("linking");
      CFILE_link (entry);
   }
   /*---(complete)--------------------*/
   return entry;
}

char         /*--> add a cronfile to the list ------------[ leaf   [ ------ ]-*/
CFILE_link         (
      /*---(params)-----------+----------*/
      tCFILE     *a_cfile     )             /* cronfile struct                */
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(header)----------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(defense against null)--------*/
   DEBUG_DATA   yLOG_snote   ("a_cfile");
   DEBUG_DATA   yLOG_spoint  (a_cfile);
   --rce;  if (a_cfile == NULL) {
      DEBUG_DATA   yLOG_snote   ("can't be null");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(defense against double link)-*/
   DEBUG_DATA   yLOG_spoint  (a_cfile->next);
   --rce;  if (a_cfile->next != NULL) {
      DEBUG_DATA   yLOG_snote   ("alreadly linked");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (a_cfile->prev);
   --rce;  if (a_cfile->prev != NULL) {
      DEBUG_DATA   yLOG_snote   ("alreadly linked");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (h_cfile);
   --rce;  if (h_cfile == a_cfile) {
      DEBUG_DATA   yLOG_snote   ("alreadly linked");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(null list)----------------*/
   if (h_cfile == NULL) {
      DEBUG_DATA   yLOG_snote   ("first entry");
      h_cfile       = a_cfile;
      t_cfile       = a_cfile;
   }
   /*---(existing list)------------*/
   else {
      DEBUG_DATA   yLOG_snote   ("add existing");
      a_cfile->prev  = t_cfile;
      t_cfile->next = a_cfile;
      t_cfile       = a_cfile;
   }
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   ++n_cfile;
   /*---(complete)--------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*--> unlink cronfile from the list ---------[ leaf   [ ------ ]-*/
CFILE_unlink       (
      /*---(params)-----------+----------*/
      tCFILE     *a_cfile     )             /* cronline struct                */
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(header)----------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(defense)---------------------*/
   DEBUG_DATA   yLOG_spoint  (a_cfile);
   --rce;  if (a_cfile == NULL) {
      DEBUG_DATA   yLOG_snote   ("a_cfile null");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(defense)---------------------*/
   DEBUG_DATA   yLOG_svalue  ("->nlines"  , a_cfile->nlines);
   --rce;  if (a_cfile->nlines > 0) {
      DEBUG_DATA   yLOG_snote   ("still has lines");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(prev)------------------------*/
   DEBUG_DATA   yLOG_spoint  (a_cfile->prev);
   --rce;  if (a_cfile->prev == NULL && h_cfile != a_cfile) {
      DEBUG_DATA   yLOG_snote   ("improper prev pointer");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   if (a_cfile->prev  == NULL)  h_cfile            = a_cfile->next;
   else                         a_cfile->prev->next = a_cfile->next;
   /*---(next)------------------------*/
   DEBUG_DATA   yLOG_spoint  (a_cfile->next);
   --rce;  if (a_cfile->next == NULL && t_cfile != a_cfile) {
      DEBUG_DATA   yLOG_snote   ("improper next pointer");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   if (a_cfile->next  == NULL)  t_cfile           = a_cfile->prev;
   else                         a_cfile->next->prev = a_cfile->prev;
   /*---(reset)-----------------------*/
   a_cfile->next     = NULL;
   a_cfile->prev     = NULL;
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   --n_cfile;
   /*---(complete)--------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

tCFILE*      /*--> destroy a cronfile --------------------[ leaf   [ ------ ]-*/
CFILE_delete       (
      /*---(params)-----------+----------*/
      tCFILE     *a_cfile     )             /* cronfile struct                */
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(header)----------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)---------------------*/
   DEBUG_DATA   yLOG_point   ("a_cfile"   , a_cfile);
   --rce;  if (a_cfile == NULL) {
      DEBUG_DATA   yLOG_note    ("can't be null");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return a_cfile;
   }
   /*---(defense)---------------------*/
   DEBUG_DATA   yLOG_value   ("->nlines"  , a_cfile->nlines);
   --rce;  if (a_cfile->nlines > 0) {
      DEBUG_DATA   yLOG_note    ("still has lines");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return a_cfile;
   }
   /*---(make sure unlinked)----------*/
   CFILE_unlink (a_cfile);
   /*---(overall)---------------------*/
   DEBUG_DATA   yLOG_note    ("freeing");
   free (a_cfile);
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   --m_cfile;
   /*---(complete)--------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return NULL;
}

tCFILE*      /*--> destroy a cronfile and its lines ------[ leaf   [ ------ ]-*/
CFILE_purge        (
      /*---(params)-----------+----------*/
      tCFILE     *a_cfile     )             /* cronfile struct                */
{  /*---(locals)-----------+-----------+-*/
   tCLINE     *x_cline     = NULL;
   tCLINE     *x_save      = NULL;
   char        rc          = 0;
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_cfile"   , a_cfile);
   --rce;  if (a_cfile == NULL) {
      DEBUG_DATA   yLOG_note    ("can't be null");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return a_cfile;
   }
   /*---(provide context)----------------*/
   DEBUG_DATA   yLOG_info    ("->name"    , a_cfile->name);
   DEBUG_DATA   yLOG_value   ("->nlines"  , a_cfile->nlines);
   /*---(mark retired)----------------*/
   a_cfile->retire   = 'y';
   DEBUG_DATA   yLOG_char    ("->retire"  , a_cfile->retire);
   /*---(process lines)---------------*/
   DEBUG_DATA   yLOG_note    ("check all lines");
   x_cline = a_cfile->head;
   while (x_cline != NULL) {
      DEBUG_DATA   yLOG_point   ("*x_cline"  , x_cline);
      DEBUG_DATA   yLOG_point   ("->file"    , x_cline->file);
      DEBUG_DATA   yLOG_value   ("->recd"    , x_cline->recd);
      DEBUG_DATA   yLOG_value   ("->rpid"    , x_cline->rpid);
      x_cline->deleted = 'y';
      x_save = x_cline->next;
      /*---(running lines)------------*/
      if (x_cline->rpid != 0) {
         DEBUG_DATA   yLOG_note    ("running, but marked for deletion");
         x_cline = x_cline->next;
         continue;
      }
      /*---(non-running line)---------*/
      DEBUG_DATA   yLOG_note    ("not running, freeing now");
      rc = CLINE_delete  (x_cline);
      DEBUG_DATA   yLOG_value   ("rc_del"    , rc);
      x_cline = x_save;
   }
   /*---(clean up)-----------------------*/
   DEBUG_DATA   yLOG_value   ("->nlines"  , a_cfile->nlines);
   if (a_cfile->nlines == 0 && a_cfile->retire == 'y') {
      DEBUG_DATA   yLOG_note    ("empty, deleting file");
      a_cfile = CFILE_delete (a_cfile);
   } else {
      DEBUG_DATA   yLOG_note    ("still has lines, not deleting");
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return  a_cfile;

}

tCFILE*      /*--> destroy a cronfile and lines ----------[ leaf   [ ------ ]-*/
CFILE_find         (
      /*---(params)-----------+----------*/
      char       *a_name      )             /* cronfile name to find          */
{  /*---(locals)-----------+-----------+-*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   tCFILE     *x_cfile     = NULL;
   int         found       = 0;
   /*---(process)------------------------*/
   DEBUG_DATA   yLOG_info    ("a_name"    , a_name);
   for (x_cfile = h_cfile; x_cfile != NULL; x_cfile = x_cfile->next) {
      /*---(context)---------------------*/
      DEBUG_DATA   yLOG_point   ("*x_cfile"  , x_cfile);
      DEBUG_DATA   yLOG_info    ("->name"    , x_cfile->name);
      /*---(filter)----------------------*/
      if (strncmp (a_name, x_cfile->name, LEN_NAME) != 0) {
         DEBUG_DATA   yLOG_note    ("not a match, continue");
         continue; /* name wrong     */
      }
      /*---(found)-----------------------*/
      DEBUG_DATA   yLOG_note    ("match found");
      found = 1;
      break;
   }
   /*---(summarize)-----------------------------*/
   if (found == 0) {
      DEBUG_DATA   yLOG_note    ("cronfile never found");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   /*---(complete)------------------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return  x_cfile;
}



/*====================------------------------------------====================*/
/*===----                           cron lines                         ----===*/
/*====================------------------------------------====================*/
static void      o___CRON_LINES______________o (void) {;}

tCLINE*      /*--> create a new cronline -----------------[ leaf   [ ------ ]-*/
CLINE_create       (tCFILE* a_cfile)
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tCLINE     *entry       = NULL;
   /*---(create a line)------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   entry = malloc (sizeof (tCLINE));
   DEBUG_DATA   yLOG_spoint  (entry);
   --rce; if (entry == NULL) {
      DEBUG_INPT   yLOG_snote   ("malloc failed");
      DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(overall)------------------------*/
   DEBUG_DATA   yLOG_snote   ("nulling ... overall");
   entry->file         = NULL;
   entry->command  [0] = '\0';
   /*---(context)------------------------*/
   DEBUG_DATA   yLOG_snote   ("con");
   entry->recd         =  0;
   entry->tracker  [0] = '\0';
   entry->comment  [0] = '\0';
   /*---(execution)----------------------*/
   DEBUG_DATA   yLOG_snote   ("exec");
   entry->active       = '-';
   entry->deleted      = '-';
   entry->rpid         =  0;
   /*---(monitoring)---------------------*/
   DEBUG_DATA   yLOG_snote   ("mon");
   entry->importance   = '0';
   entry->monitoring   = '-';
   entry->catchup      = '-';
   entry->busy_delay   = 'y';
   entry->busy_skip    = 'y';
   entry->busy_kill    = 'y';
   entry->dur_min      =  0;
   entry->dur_exp      =  0;
   entry->dur_max      =  5;
   /*---(historical)---------------------*/
   DEBUG_DATA   yLOG_snote   ("hist");
   entry->attempts     =  0;
   entry->failures     =  0;
   entry->lasttime     =  0;
   entry->lastexit     =  0;
   /*---(pointers)-----------------------*/
   DEBUG_DATA   yLOG_snote   ("point");
   entry->next         = NULL;
   entry->prev         = NULL;
   entry->fnext        = NULL;
   entry->fprev        = NULL;
   entry->pnext        = NULL;
   entry->pprev        = NULL;
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   ++m_cline;
   /*---(check link request)----------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   if (a_cfile != NULL) {
      DEBUG_DATA   yLOG_snote   ("linking");
      CLINE_link (a_cfile, entry);
   }
   /*---(complete)-----------------------*/
   return entry;
}

char         /*--> link a cronline to a cronfile ---------[ leaf   [ ------ ]-*/
CLINE_link         (
      /*---(params)-----------+----------*/
      tCFILE     *a_cfile     ,             /* cronfile struct                */
      tCLINE     *a_cline     )             /* cronline struct                */
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tCLINE     *x_cline     = NULL;
   /*---(header)----------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(defense)---------------------*/
   DEBUG_DATA   yLOG_snote   ("a_cfile");
   DEBUG_DATA   yLOG_spoint  (a_cfile);
   --rce;  if (a_cfile == NULL) {
      DEBUG_DATA   yLOG_snote   ("can't be null");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_snote   ("a_cline");
   DEBUG_DATA   yLOG_spoint  (a_cline);
   --rce;  if (a_cline == NULL) {
      DEBUG_DATA   yLOG_snote   ("can't be null");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(defense against double link)-*/
   DEBUG_DATA   yLOG_spoint  (a_cline->next);
   --rce;  if (a_cline->next != NULL) {
      DEBUG_DATA   yLOG_snote   ("next alreadly linked");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (a_cline->prev);
   --rce;  if (a_cline->prev != NULL) {
      DEBUG_DATA   yLOG_snote   ("prev alreadly linked");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (a_cline->file);
   --rce;  if (a_cline->file != NULL) {
      DEBUG_DATA   yLOG_snote   ("file alreadly linked");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(null list)----------------*/
   if (a_cfile->head == NULL) {
      DEBUG_DATA   yLOG_snote   ("first entry");
      a_cfile->head        = a_cline;
      a_cfile->tail        = a_cline;
   }
   /*---(existing list)------------*/
   else {
      DEBUG_DATA   yLOG_snote   ("add existing");
      a_cline->prev        = a_cfile->tail;
      a_cfile->tail->next  = a_cline;
      a_cfile->tail        = a_cline;
   }
   /*---(tie line back to file)----*/
   DEBUG_DATA   yLOG_snote   ("file");
   a_cline->file    = a_cfile;
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   ++n_cline;
   a_cfile->nlines += 1;
   /*---(complete)--------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*--> unlink cronline form cronfile ---------[ leaf   [ ------ ]-*/
CLINE_unlink       (
      /*---(params)-----------+----------*/
      tCLINE     *a_cline     )             /* cronline struct                */
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(header)----------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(defense)---------------------*/
   DEBUG_DATA   yLOG_spoint  (a_cline);
   --rce;  if (a_cline == NULL) {
      DEBUG_DATA   yLOG_snote   ("a_cline null");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (a_cline->file);
   --rce;  if (a_cline->file == NULL) {
      DEBUG_DATA   yLOG_snote   ("file pointer is null");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(prev)------------------------*/
   DEBUG_DATA   yLOG_spoint  (a_cline->prev);
   --rce;  if (a_cline->prev == NULL && a_cline->file->head != a_cline) {
      DEBUG_DATA   yLOG_snote   ("improper prev pointer");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   if (a_cline->prev == NULL)   a_cline->file->head = a_cline->next;
   else                         a_cline->prev->next = a_cline->next;
   /*---(next)------------------------*/
   DEBUG_DATA   yLOG_spoint  (a_cline->next);
   --rce;  if (a_cline->next == NULL && a_cline->file->tail != a_cline) {
      DEBUG_DATA   yLOG_snote   ("improper next pointer");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   if (a_cline->next == NULL)   a_cline->file->tail = a_cline->prev;
   else                         a_cline->next->prev = a_cline->prev;
   /*---(overall)---------------------*/
   DEBUG_DATA   yLOG_svalue  ("->nlines"  , a_cline->file->nlines);
   a_cline->file->nlines -= 1;
   if (a_cline->file->nlines <  0) a_cline->file->nlines = 0;
   /*---(fast count)------------------*/
   DEBUG_DATA   yLOG_snote   ("active");
   DEBUG_DATA   yLOG_schar   (a_cline->active);
   if (a_cline->active == 'y') {
      --nfast;
      /*---(fast prev)----------------*/
      if (a_cline->fprev == NULL)  fasthead             = a_cline->fnext;
      else                         a_cline->fprev->fnext = a_cline->fnext;
      /*---(fast next)----------------*/
      if (a_cline->fnext == NULL)  fasttail             = a_cline->fprev;
      else                         a_cline->fnext->fprev = a_cline->fprev;
      /*---(reset)--------------------*/
      a_cline->fnext    = NULL;
      a_cline->fprev    = NULL;
   }
   /*---(reset)-----------------------*/
   a_cline->file     = NULL;
   a_cline->next     = NULL;
   a_cline->prev     = NULL;
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   --n_cline;
   /*---(complete)--------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

tCLINE*      /*--> remove cronline from cronfile ---------[ leaf   [ ------ ]-*/
CLINE_delete       (tCLINE *a_cline)
{
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_point   ("a_cline"    , a_cline);
   /*---(defense)---------------------*/
   if (a_cline == NULL)   {
      DEBUG_DATA   yLOG_note    ("a_cline can not be null");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return  a_cline;
   }
   /*> DEBUG_DATA   yLOG_char    ("->deleted" , a_cline->deleted);                    <*/
   /*> if (a_cline->deleted != 'y') return  NULL;                                     <*/
   DEBUG_DATA   yLOG_value   ("->rpid"    , a_cline->rpid);
   if (a_cline->rpid    !=  0 ) {
      DEBUG_DATA   yLOG_note    ("a_cline is currently running");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return  a_cline;
   }
   /*---(make sure unlinked)----------*/
   CLINE_unlink (a_cline);
   /*---(update totals)---------------*/
   DEBUG_DATA   yLOG_note    ("freeing");
   free (a_cline);
   a_cline = NULL;
   DEBUG_DATA   yLOG_point   ("a_cline"    , a_cline);
   /*---(totals)----------------------*/
   DEBUG_DATA   yLOG_snote   ("totals");
   --m_cline;
   /*---(complete)--------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return NULL;
}

char         /*--> write a full cronline to output -------[ leaf   [ ------ ]-*/
CLINE_show         (tCLINE *a_cline)
{
   printf ("\n");
   printf ("file      ::%p::\n",           a_cline->file);
   printf ("record    ::%d::\n",           a_cline->recd);
   printf ("tracker   ::%s::\n",           a_cline->tracker);
   printf ("comment   ::%s::\n",           a_cline->comment);
   printf ("import    ::%c::\n",           a_cline->importance);
   printf ("monitor   ::%c::\n",           a_cline->monitoring);
   printf ("dur_exp   ::%d::\n",           a_cline->dur_exp);
   printf ("dur_min   ::%d::\n",           a_cline->dur_min);
   printf ("dur_max   ::%d::\n",           a_cline->dur_max);
   printf ("command   ::%s::\n",           a_cline->command);
   printf ("minutes   ::%s::\n",           a_cline->sched.min);
   printf ("hours     ::%s::\n",           a_cline->sched.hrs);
   printf ("days      ::%s::\n",           a_cline->sched.dys);
   printf ("months    ::%s::\n",           a_cline->sched.mos);
   printf ("dow       ::%s::\n",           a_cline->sched.dow);
   printf ("wks       ::%s::\n",           a_cline->sched.wks);
   printf ("yrs       ::%s::\n",           a_cline->sched.yrs);
   printf ("active    ::%c::\n",           a_cline->active);
   printf ("deleted   ::%c::\n",           a_cline->deleted);
   printf ("rpid      ::%d::\n",           a_cline->rpid);
   printf ("attempts  ::%d::\n",           a_cline->attempts);
   printf ("failures  ::%d::\n",           a_cline->failures);
   printf ("lasttime  ::%d::\n",           a_cline->lasttime);
   printf ("lastexit  ::%d::\n",           a_cline->lastexit);
   printf ("\n");
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           proc lines                         ----===*/
/*====================------------------------------------====================*/
static void      o___PROC_LINES______________o (void) {;}

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
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*        /*--> greybox unit test accessor ------------[ leaf   [ ------ ]-*/
LIST_unit          (char *a_question, void *a_point)
{
   /*---(locals)-----------+-----------+-*/
   int         xfore       = 0;        /* counts to verify doubly-linked list */
   int         xback       = 0;        /* counts to verify doubly-linked list */
   tCFILE     *x_cfile     = NULL;
   tCLINE     *x_cline     = NULL;
   int         x_count     = 0;        /* generic counting variable           */
   char        x_shape     [40] = "(empty)";
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "BASE_unit        : question not understood", LEN_TEXT);
   /*---(overall)------------------------*/
   if        (strncmp(a_question, "counters"     , 20)  == 0) {
      /***** always differentiate between malloc'd and linked *****/
      snprintf(unit_answer, LEN_TEXT, "LIST counters    : file %4dm, %4dn;  line %5dm, %5dn", m_cfile, n_cfile, m_cline, n_cline);
   } else if (strncmp(a_question, "running" , 20)    == 0) {
      snprintf (unit_answer, LEN_TEXT, "LIST running     : %4d line, %4d fast, %4d proc", n_cline, nfast, nproc);
   }
   /*---(crontab files)------------------*/
   else if (strncmp(a_question, "files"        , 20)   == 0) {
      /***** always check foreward and backward integrity also *****/
      xfore = xback = 0;
      x_cfile = h_cfile; while (x_cfile != NULL) { ++xfore; x_cfile = x_cfile->next; }
      x_cfile = t_cfile; while (x_cfile != NULL) { ++xback; x_cfile = x_cfile->prev; }
      snprintf(unit_answer, LEN_TEXT, "LIST file list   : n=%4d, f=%4d, b=%4d, h=%-10p, t=%p", n_cfile, xfore, xback, h_cfile, t_cfile);
   }
   /*---(crontab lines)------------------*/
   else if (strncmp(a_question, "lines"        , 20)   == 0) {
      /***** always check foreward and backward integrity also *****/
      xfore = xback = 0;
      x_cfile = a_point;
      if (x_cfile != NULL) {
         x_cline = x_cfile->head; while (x_cline != NULL) { ++xfore; x_cline = x_cline->next; }
         x_cline = x_cfile->tail; while (x_cline != NULL) { ++xback; x_cline = x_cline->prev; }
         snprintf(unit_answer, LEN_TEXT, "LIST %-10p  : n=%4d, f=%4d, b=%4d, h=%-10p, t=%p", x_cfile, x_cfile->nlines, xfore, xback, x_cfile->head, x_cfile->tail);
      } else {
         snprintf(unit_answer, LEN_TEXT, "LIST %-10p  : n=%4d, f=%4d, b=%4d, h=%-10p, t=%p", NULL, 0, 0, 0, NULL, NULL);
      }
   } else if (strncmp (a_question, "tracker"      , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST tracker     : [%-.40s]", x_cline->tracker);
   } else if (strncmp (a_question, "comment"      , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST comment     : %3d [%-.41s]", strllen (x_cline->comment, LEN_COMMENT), x_cline->comment);
   } else if (strncmp (a_question, "command"      , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST command     : %3d [%-.41s]", strllen (x_cline->command, LEN_CMD), x_cline->command);
   } else if (strncmp (a_question, "profile"      , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST profile     : min=%4d, exp=%4d, max=%4d", x_cline->dur_min, x_cline->dur_exp, x_cline->dur_max);
   } else if (strncmp (a_question, "controls"     , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST controls    : imp %c, mon %c, cat %c, del %c, ski %c, kil %c", x_cline->importance, x_cline->monitoring, x_cline->catchup, x_cline->busy_delay, x_cline->busy_skip, x_cline->busy_kill);
   } else if (strncmp (a_question, "sched_min"    , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST minutes     : [%-.45s]", x_cline->sched.min);
   } else if (strncmp (a_question, "sched_hrs"    , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST hours       : [%-.45s]", x_cline->sched.hrs);
   } else if (strncmp (a_question, "sched_dys"    , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST days        : [%-.45s]", x_cline->sched.dys);
   } else if (strncmp (a_question, "sched_mos"    , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST months      : [%-.45s]", x_cline->sched.mos);
   } else if (strncmp (a_question, "sched_dow"    , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST dow         : [%-.45s]", x_cline->sched.dow);
   } else if (strncmp (a_question, "sched_wks"    , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST weeks       : [%-.45s]", x_cline->sched.wks);
   } else if (strncmp (a_question, "sched_yrs"    , 20)   == 0) {
      x_cline = a_point;
      if (x_cline != NULL) snprintf (unit_answer, LEN_TEXT, "LIST years       : [%-.45s]", x_cline->sched.yrs);
   }
   /*---(complex feedback)---------------*/
   else if (strncmp(a_question, "shape"        , 20)  == 0) {
      /*---(scan every file)-------------*/
      for (x_cfile = h_cfile; x_cfile != NULL; x_cfile = x_cfile->next) {
         x_shape [x_count++] = 'F';
         x_shape [x_count  ] = '\0';
         if (x_count >= 35) return 0;
         /*---(scan every line)----------*/
         for (x_cline = x_cfile->head; x_cline != NULL; x_cline = x_cline->next) {
            x_shape [x_count++] = '_';
            x_shape [x_count  ] = '\0';
            if (x_count >= 35) return 0;
         }
      }
      snprintf (unit_answer, LEN_TEXT, "LIST shape (%3d) : %s", n_cfile + n_cline, x_shape);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
LIST_list          (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *x_cfile;                        /* current file                  */
   tCLINE   *x_cline;                        /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---cron-list-----------------------------------\n");
   for (x_cfile = h_cfile; x_cfile != NULL; x_cfile = x_cfile->next) {
      printf("      FILE (%c) %3d = %-20.20s :: %-40.40s\n",
            x_cfile->retire, x_cfile->nlines, x_cfile->user, x_cfile->name);
      /*---(scan every line)-----------------*/
      for (x_cline = x_cfile->head; x_cline != NULL; x_cline = x_cline->next) {
         printf("           line (%c) <<%.50s>>\n", x_cline->deleted, x_cline->command);
      }
   }
   printf("      -----------------------------------------------\n");
   return 0;
}



/*====================---------[[ end-of-code ]]----------====================*/
