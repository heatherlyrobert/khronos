/*====================--------[[ start-of-code ]]---------====================*/

/*===[[ SUMMARY ]]============================================================#

 *   application   : khronos
 *   module        : khronos_list
 *   size          : less than 100 slocL
 *
 */
/*===[[ SUMMARY ]]============================================================#

 *   khronos_list is the linked list support functions to manage the data
 *   associated with crontab files, lines, and running processes
 *
 */
/*============================================================================*/


#include  "khronos.h"


/*---(file linked list)--------*/
tCFILE   *cronhead;
tCFILE   *crontail;
int       nfile;
int       nentry;

/*---(fast path linked list)---*/
tCLINE   *fasthead;
tCLINE   *fasttail;
int       nfast;

/*---(processing linked list)--*/
tCLINE   *prochead;
tCLINE   *proctail;
int       nproc;



/*====================------------------------------------====================*/
/*===----                           cron files                         ----===*/
/*====================------------------------------------====================*/
static void      o___CRON_FILES______________o (void) {;}

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
   /*---(defense)---------------------*/
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



/*====================------------------------------------====================*/
/*===----                           cron lines                         ----===*/
/*====================------------------------------------====================*/
static void      o___CRON_LINES______________o (void) {;}

char        /* PURPOSE : add a line to the cronfile list                      */
cronline_add (tCFILE *a_file, tCLINE *a_line)
{
   /*---(initialize)------------------*/
   a_line->file         = a_file;
   a_line->rpid         = 0;
   a_line->active       = 0;
   a_line->deleted      = 'n';
   a_line->title[0]     = '\0';
   a_line->duration     = '-';
   a_line->recovery     = '-';
   a_line->priority     = '-';
   a_line->alerting     = '-';
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
   /*---(defense)---------------------*/
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



/*====================---------[[ end-of-code ]]----------====================*/
