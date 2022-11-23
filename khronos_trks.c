/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"


/*
 *
 * these are for summary statistics only actual tracking exists on most
 * jobs to research actual numbers
 *
 * stats
 *   #  tot  ---early---  end
 *      r a  s b o k s f  w p  ---history--->>>
 *  "·  · ·  · · · · · ·  · ·  · ´····´····´····´····´····´····´····´····´····´····´····´····´"
 *
 * durs (for warnings and passes only)
 *   #  e l  < -30     -10        0         10      30 >  --history--->>>
 *  "·  · ·  · · · · · ·········· · ·········· · · · · ·  · ´····´····´····´····´····´····´····´····´····´····´····´····´"
 *
 * both of these use the YSTR_COUNT for counting numbers  ·1-9a-zA-Z
 *
 * the # entry is the chosen rolling number, i.e. 1 to 60 runs.  
 *   -- less for new jobs
 *   -- more for stable ones
 *   -- regardless of setting, 60 runs of history will be kept in detail
 *   -- why not the size is allocated anyway ;)  then changes are easy.
 *
 * history for durs uses < n m l k jihgfedcbq 0 ABCDEFGHIJ K L M N >
 *   -- a is exactly  -1
 *   -- j is exactly -10
 *   -- k is -11 to -15
 *   -- l is -16 to -20
 *   -- m is -25 to -21
 *   -- n is -30 to -26
 *   -- < is less than 30
 *
 * keep history line so i can do rolling stats very easily, and also
 * over-ride to keep only 'x' number whenever i wish
 *
 * actual tracking exists on most jobs to research actual numbers
 *
 * NOTE : i am keeping the detail and totals together in the file.  its
 * duplication and larger, but it allows easier reporting later and an
 * small potential element of data corruption checking.
 *
 */



static char     s_name  [LEN_PATH] = "";
static FILE    *f_trks  = NULL;

static tTRKS   *s_head  = NULL;
static tTRKS   *s_tail  = NULL;
static int      s_count = 0;



/*====================------------------------------------====================*/
/*===----                     supporting functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
TRKS__wipe              (tTRKS *a_cur)
{
   /*---(defense)--------------*/
   if (a_cur == NULL)  return -1;
   /*---(master)-------------------------*/
   strlcpy (a_cur->file   , "" , LEN_TITLE);
   strlcpy (a_cur->tracker, "" , LEN_TITLE);
   strlcpy (a_cur->last   , "" , LEN_HUND);
   strlcpy (a_cur->stats  , "" , LEN_FULL);
   strlcpy (a_cur->durs   , "" , LEN_FULL);
   strlcpy (a_cur->actual , "" , LEN_RECD);
   a_cur->parent  = NULL;
   a_cur->m_prev  = NULL;
   a_cur->m_next  = NULL;
   /*---(complete)-------------*/
   return 1;
}

char*
TRKS__memory            (tTRKS *a_cur)
{
   int         n           =    0;
   strlcpy (g_print, "å___.___.___æ", LEN_RECD);
   ++n;  if (a_cur->file    [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->tracker [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->last    [0] != '\0')        g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->stats   [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->durs    [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->actual  [0] != '\0')        g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->parent      != NULL)        g_print [n] = 'X';
   ++n;  if (a_cur->m_prev      != NULL)        g_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        g_print [n] = 'X';
   return g_print;
}

char
TRKS__tracker           (cchar *a_tracker)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_tracker" , a_tracker);
   --rce;  if (a_tracker == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_tracker" , a_tracker);
   /*---(check tracker)------------------*/
   l = strlen (a_tracker);
   DEBUG_OUTP   yLOG_value   ("l"         , l);
   --rce;  if (l < 3) {
      DEBUG_OUTP   yLOG_note    ("must be at least 3 characters");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (l > 25) {
      DEBUG_OUTP   yLOG_note    ("can not be more than 25 characters");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = strlgood (a_tracker, ySTR_WRITE, LEN_TITLE);
   DEBUG_OUTP   yLOG_value   ("good"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_note    ("found non-writing/special characters (ySTR_WRITE)");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__coded             (char a_type, cchar *a_key, char *a_str)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         i           =    0;
   int         l           =    0;
   int         x_trouble   =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_key"     , a_key);
   --rce;  if (a_key == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_point   ("a_str"     , a_str);
   --rce;  if (a_str == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check tracker)------------------*/
   l = strlen (a_str);
   DEBUG_OUTP   yLOG_value   ("l"         , l);
   --rce;  if (a_type != 'a' && l != strlen (a_key)) {
      DEBUG_OUTP   yLOG_note    ("does not match default length");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   for (i = 0; i < l; ++i) {
      switch (a_key [i]) {
      case ' ' :
         if (a_str [i] != ' ') {
            DEBUG_OUTP   yLOG_complex ("not empty" , "%3d, %c vs %c", i, a_key [i], a_str [i]);
            ++x_trouble;
         }
         break;
      case ')' :
         if (a_str [i] != ')') {
            DEBUG_OUTP   yLOG_complex ("not paren" , "%3d, %c vs %c", i, a_key [i], a_str [i]);
            ++x_trouble;
         }
         break;
      case '¬' :
         break;
      default  :
         if (a_str [i] == ' ') {
            DEBUG_OUTP   yLOG_complex ("not filled", "%3d, %c vs %c", i, a_key [i], a_str [i]);
            ++x_trouble;
         }
         break;
      }
   }
   /*---(check results)------------------*/
   DEBUG_OUTP   yLOG_value   ("x_trouble" , x_trouble);
   --rce;  if (x_trouble > 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append on actuals)--------------*/
   --rce;  if (a_type == 'a') {
      if (l <  strlen (a_key))  strlcat (a_str, KHRONOS_DEFACTUAL + l,  LEN_RECD);
      if (l >  strlen (a_key)) {
         DEBUG_OUTP   yLOG_note    ("actual is longer than default length");
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     allocating and freeing                   ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
trks__new               (tTRKS **r_new, char a_force)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tTRKS      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*r_new);
   --rce;  if (a_force == '-' && *r_new != NULL) {
      DEBUG_INPT  yLOG_snote   ("already set");
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *r_new = NULL;
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tTRKS *) malloc (sizeof (tTRKS));
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
   rc = TRKS__wipe (x_new);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char TRKS__new     (tTRKS **r_new) { return trks__new (r_new, '-'); }
char TRKS__force   (tTRKS **r_new) { return trks__new (r_new, 'y'); }

char
TRKS__free              (tTRKS **b_old)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   tTRKS      *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (b_old);
   --rce;  if (b_old == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*b_old);
   --rce;  if (*b_old == NULL) {
      DEBUG_INPT   yLOG_snote   ("never set");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(simplify)-----------------------*/
   x_old = *b_old;
   /*---(clear and return)---------------*/
   TRKS__wipe (x_old);
   free (x_old);
   *b_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       existance                              ----===*/
/*====================------------------------------------====================*/
static void  o___EXIST___________o () { return; }

char
TRKS__default           (tTRKS *a_cur)
{
   char        i           =    0;
   strlcpy (a_cur->last  , KHRONOS_DEFLAST  , LEN_HUND);
   strlcpy (a_cur->stats , KHRONOS_DEFSTATS , LEN_FULL);
   strlcpy (a_cur->durs  , KHRONOS_DEFDURS  , LEN_FULL);
   strlcpy (a_cur->actual, KHRONOS_DEFACTUAL, LEN_RECD);
   return 0;
}

char
TRKS__by_name           (cchar *a_file, cchar *a_tracker, tTRKS **r_trks)
{
   char        rce         =  -10;
   tTRKS      *x_cur       = NULL;
   x_cur = s_head;
   --rce;  if (r_trks == NULL)  return rce;
   *r_trks = NULL;
   while (x_cur != NULL) {
      if (strcmp (a_file, x_cur->file) == 0) {
         if (strcmp (a_tracker, x_cur->tracker) == 0) {
            *r_trks = x_cur;
            return 1;
         }
      }
      x_cur = x_cur->m_next;
   }
   return 0;
}

char
TRKS__by_index          (int n, tTRKS **r_trks)
{
   char        rce         =  -10;
   int         c           =    0;
   tTRKS      *x_cur       = NULL;
   x_cur = s_head;
   --rce;  if (r_trks == NULL)  return rce;
   *r_trks = NULL;
   while (x_cur != NULL) {
      if (c == n) {
         *r_trks = x_cur;
         return 1;
      }
      ++c;
      x_cur = x_cur->m_next;
   }
   return 0;
}

char
TRKS__hook              (tTRKS *a_cur)
{
   if (s_head == NULL) {
      s_head  = s_tail  = a_cur;
      s_count = 1;
   } else {
      a_cur->m_prev  = s_tail;
      s_tail->m_next = a_cur;
      s_tail         = a_cur;
      ++s_count;
   }
   return 0;
}

char
TRKS__replace           (tTRKS *a_old, tTRKS *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(populate new pointers)----------*/
   a_new->m_prev = a_old->m_prev;
   if (a_old->m_prev != NULL)  a_old->m_prev->m_next = a_new;
   a_new->m_next = a_old->m_next;
   if (a_old->m_next != NULL)  a_old->m_next->m_prev = a_new;
   /*---(ground old pointers)------------*/
   a_old->m_prev = NULL;
   a_old->m_next = NULL;
   /*---(free old)-----------------------*/
   TRKS__free (&a_old);
   /*---(complete)-----------------------*/
   return 0;
}

char
TRKS_purge              (void)
{
   char        rce         =  -10;
   tTRKS      *x_cur       = NULL;
   tTRKS      *x_nxt       = NULL;
   x_cur = s_head;
   while (x_cur != NULL) {
      x_nxt = x_cur->m_next;
      x_cur->m_prev = x_cur->m_next = NULL;
      TRKS__free (&x_cur);
      --s_count;
      x_cur = x_nxt;
   }
   s_head  = s_tail  = NULL;
   return 0;
}

char
TRKS_create             (cchar *a_file, cchar *a_tracker, tLINE *a_line, tTRKS **r_trks)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTRKS      *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_trks != NULL)  *r_trks = NULL;
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file    == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_file"    , a_file);
   DEBUG_OUTP   yLOG_point   ("a_tracker" , a_tracker);
   --rce;  if (a_tracker == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_tracker" , a_tracker);
   DEBUG_OUTP   yLOG_point   ("r_trks"    , r_trks);
   --rce;  if (r_trks == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check tracker)------------------*/
   rc = TRKS__tracker (a_tracker);
   DEBUG_OUTP   yLOG_value   ("tracker"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find)------------------------*/
   rc = TRKS__by_name (a_file, a_tracker, &x_cur);
   if (x_cur != NULL) {
      *r_trks = x_cur;
      return 1;
   }
   /*---(create)----------------------*/
   rc = TRKS__new (&x_cur);
   DEBUG_YJOBS   yLOG_value   ("new"       , rc);
   if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add data)--------------------*/
   strlcpy (x_cur->file   , a_file   , LEN_TITLE);
   strlcpy (x_cur->tracker, a_tracker, LEN_TITLE);
   if (a_line != NULL)  x_cur->parent = a_line;
   TRKS__default (x_cur);
   /*---(hook)------------------------*/
   TRKS__hook (x_cur);
   /*---(save back)-------------------*/
   *r_trks = x_cur;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       file related                           ----===*/
/*====================------------------------------------====================*/
static void  o___FILE____________o () { return; }

char
TRKS__open              (char *a_name, char *a_perms)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   --rce;  if (f_trks  != NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_spoint  (a_name);
   --rce;  if (a_name  == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_snote   (a_name);
   DEBUG_OUTP   yLOG_spoint  (a_perms);
   --rce;  if (a_perms == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_snote   (a_perms);
   --rce;  if (strcmp (a_perms, "rt") != 0 && strcmp (a_perms, "wt") != 0) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   f_trks = fopen (a_name, a_perms);
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   --rce;  if (f_trks == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   strlcpy (s_name, a_name, LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
TRKS__close             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   --rce;  if (f_trks == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = fclose (f_trks);
   DEBUG_OUTP   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   f_trks = NULL;
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   /*---(save)---------------------------*/
   strlcpy (s_name, "", LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       export/import                          ----===*/
/*====================------------------------------------====================*/
static void  o___EXIM____________o () { return; }

char
TRKS__parse             (cchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_HUGE]  = "";
   char        x_file      [LEN_TITLE] = "";
   char        x_tracker   [LEN_TITLE] = "";
   char       *p           = NULL;
   char        n           =    0;
   char       *x_dst       = NULL;
   int         l           =    0;
   tTRKS      *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_recd, a_recd, LEN_HUGE);
   p = strtok (x_recd, "§");
   /*---(parse fields)-------------------*/
   --rce;  while (p != NULL) {
      /*---(parse)-----------------------*/
      switch (n) {
      case  0 : x_dst = x_file;          l = LEN_TITLE;  break;
      case  1 : x_dst = x_tracker;       l = LEN_TITLE;  break;
      case  2 : x_dst = x_cur->last;     l = LEN_HUND;   break;
      case  3 : x_dst = x_cur->stats;    l = LEN_FULL;   break;
      case  4 : x_dst = x_cur->durs;     l = LEN_FULL;   break;
      case  5 : x_dst = x_cur->actual;   l = LEN_RECD;   break;
      default :
                break;  /* too many fields */
      }
      strlcpy  (x_dst, p        , l);
      strltrim (x_dst, ySTR_BOTH, l);
      /*---(enough)----------------------*/
      if (n == 1) {
         rc = TRKS_create (x_file, x_tracker, NULL, &x_cur);
         DEBUG_OUTP   yLOG_value   ("create"    , rc);
         if (rc < 0) {
            DEBUG_OUTP   yLOG_note    ("could not create properly");
            break;
         }
      }
      /*---(next)------------------------*/
      p = strtok (NULL, "§");
      ++n;
      /*---(done)------------------------*/
   }
   /*---(check result)-------------------*/
   DEBUG_OUTP   yLOG_value   ("n"         , n);
   --rce;  if (n < 2)  {
      TRKS__wipe (x_cur);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (n > 6)  {
      DEBUG_OUTP   yLOG_note    ("too many fields, ignoring trailing");
   }
   /*---(data quality checks)------------*/
   if (TRKS__coded ('l', KHRONOS_DEFLAST  , x_cur->last     ) < 0)
      strlcpy (x_cur->last  , KHRONOS_DEFLAST  , LEN_HUND);
   if (TRKS__coded ('s', KHRONOS_DEFSTATS , x_cur->stats    ) < 0)
      strlcpy (x_cur->stats , KHRONOS_DEFSTATS , LEN_FULL);
   if (TRKS__coded ('d', KHRONOS_DEFDURS  , x_cur->durs     ) < 0)
      strlcpy (x_cur->durs  , KHRONOS_DEFDURS  , LEN_FULL);
   if (TRKS__coded ('a' ,KHRONOS_DEFACTUAL, x_cur->actual   ) < 0)
      strlcpy (x_cur->actual, KHRONOS_DEFACTUAL, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS_import_full        (cchar *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_HUGE]  = "";
   int         x_len       =    0;
   tTRKS      *x_cur       = NULL;
   int         n           =    0;
   int         c           =    0;
   int         g           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   rc = TRKS__open (a_file, "rt");
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read all)-----------------------*/
   while (1) {
      /*---(read)------------------------*/
      fgets (x_recd, LEN_HUGE, f_trks);
      DEBUG_YJOBS   yLOG_value   ("length"    , strlen (x_recd));
      if (feof (f_trks)) {
         DEBUG_YJOBS   yLOG_note    ("end of file reached");
         break;
      }
      ++n;
      /*---(filter)----------------------*/
      if (x_recd [0] == '\0')  continue;
      if (x_recd [0] == '#')   continue;
      if (x_recd [0] == ' ')   continue;
      /*---(clean-up)--------------------*/
      strltrim (x_recd, ySTR_BOTH, LEN_HUGE);
      x_len = strlen (x_recd);
      if (x_len <  5)          continue;
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      DEBUG_YJOBS   yLOG_info    ("x_recd"    , x_recd);
      /*---(parse)-----------------------*/
      rc = TRKS__parse (x_recd);
      if (rc < 0) {
         TRKS__free (&x_cur);
         DEBUG_YJOBS   yLOG_note    ("record could not be parsed");
         continue;
      }
      ++g;
      /*---(done)------------------------*/
   }
   /*---(close file)---------------------*/
   rc = TRKS__close ();
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char TRKS_import   (void) { return TRKS_import_full (my.n_trks); }

char
TRKS_export             (void)
{
}



/*====================------------------------------------====================*/
/*===----                         data updates                         ----===*/
/*====================------------------------------------====================*/
static void  o___DATA____________o () { return; }

char
TRKS__count2num         (char a_count)
{
   char       *p           = NULL;
   if (a_count ==   0)   return -10;
   if (a_count == '´')   return   0;
   p = strchr (YSTR_COUNT, a_count);
   if (p == NULL)  return -1;
   return p - YSTR_COUNT;
}

char
TRKS__num2count         (char a_num)
{
   int         l           =    0;
   char       *p           = NULL;
   char        n           =    0;
   n = a_num;
   l = strlen (YSTR_COUNT);
   if (a_num <    0)   return -1;
   if (a_num >=   l)   return -1;
   return YSTR_COUNT [n];
}

char
TRKS__biggun2num        (char a_biggun)
{
   char       *p           = NULL;
   if (a_biggun ==   0)   return  -1;
   if (a_biggun == '´')   return   0;
   p = strchr (YSTR_BIGGUN, a_biggun);
   if (p == NULL)  return -1;
   return p - YSTR_BIGGUN;
}

char
TRKS__num2biggun        (char a_num)
{
   int         l           =    0;
   char       *p           = NULL;
   char        n           =    0;
   n = a_num;
   l = strlen (YSTR_BIGGUN);
   if (a_num <    0)   return -1;
   if (a_num >=   l)   return -1;
   return YSTR_BIGGUN [n];
}

/*    r a  s B O K T  g v G V  f w p  ---history--->>>
 * ·  · ·  · · · · ·  · · · ·  · · ·  ·)´····´····´····´····´····´····´····´····´····´····´····´····´"
 * -123456789-123456789-123456789-123456789-123456789-123456789-123456789
 * 0         1         2         3         4         5         6
 */


#define    KHRONOS_STAT  "[sBOKTgvGVfwp"

char
TRKS__stats             (char *b_str, char a_code)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   char       *p           = NULL;
   char        n           =    0;
   char        x_roll      =    0;
   char        x_hist      =    0;
   char        x_next      =    0;
   char        x_ch        =    0;
   char        x_off       =    0;
   char        x_actv      =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_char    ("a_code"    , a_code);
   --rce;  if (a_code == 0 || strchr (KHRONOS_STAT, a_code) == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = TRKS__coded ('s', KHRONOS_DEFSTATS , b_str);
   DEBUG_OUTP    yLOG_value   ("coded"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   x_roll  = TRKS__count2num (b_str [ 0]);
   if (x_roll > 60)  x_roll = 60;
   DEBUG_OUTP    yLOG_complex ("x_roll"    , "%c, %d", b_str [ 0], x_roll);
   x_actv  = TRKS__count2num (b_str [ 5]);
   DEBUG_OUTP    yLOG_complex ("x_actv"    , "%c, %d", b_str [ 5], x_actv);
   --rce;  if (x_actv <= 0 && a_code != KHRONOS_BEG) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_hist  = TRKS__count2num (b_str [35]);
   x_next  = x_hist + 37;
   DEBUG_OUTP    yLOG_complex ("x_hist"    , "%c, %d, %d", b_str [35], x_hist, x_next);
   l = strlen (KHRONOS_STAT);
   /*---(beginnings)---------------------*/
   --rce;  switch  (a_code) {
   case KHRONOS_BEG  :  x_off =  3;    break;
   }
   /*---(troubles)-----------------------*/
   --rce;  if (x_off == 0)  switch  (a_code) {
   case KHRONOS_SKIP :  x_off =  8;    break;
   case KHRONOS_BADD :  x_off = 10;    break;
   case KHRONOS_BOOM :  x_off = 12;    break;
   case KHRONOS_KILL :  x_off = 14;    break;
   case KHRONOS_TERM :  x_off = 16;    break;
   }
   /*---(interventions)------------------*/
   --rce;  if (x_off == 0)  switch  (a_code) {
   case KHRONOS_LGRA :  x_off = 19;    break;
   case KHRONOS_LVIO :  x_off = 21;    break;
   case KHRONOS_GGRA :  x_off = 23;    break;
   case KHRONOS_GVIO :  x_off = 25;    break;
   }
   /*---(endings)------------------------*/
   --rce;  if (x_off == 0)  switch  (a_code) {
   case KHRONOS_FAIL :  x_off = 28;    break;
   case KHRONOS_WARN :  x_off = 30;    break;
   case KHRONOS_PASS :  x_off = 32;    break;
   }
   /*---(summarize)----------------------*/
   x_ch   = a_code;
   DEBUG_OUTP    yLOG_complex ("values"    , "%c, %d, %c", a_code, x_off, x_ch);
   --rce;  if (x_off == 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update history)-----------------*/
   if (a_code != KHRONOS_BEG) {
      b_str [x_next] = x_ch;
      b_str [35    ] = TRKS__num2count (++x_hist);
   }
   /*---(update stats)-------------------*/
   if (strchr ("]a", a_code) == NULL) {
      n = TRKS__count2num (b_str [x_off]);
      b_str [x_off ] = TRKS__num2count (++n);
   }
   /*---(also update active)-------------*/
   if (a_code == KHRONOS_BEG) {
      n = TRKS__count2num (b_str [5]);
      b_str [5] = TRKS__num2count (++n);
   } else  {
      n = TRKS__count2num (b_str [5]);
      b_str [5] = TRKS__num2count (--n);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__restat_full       (char a_roll, cchar *a_hist, char *r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   char        i           =    0;
   char       *p           =    0;
   char        n           =    0;
   int         x_code      =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_point   ("r_new"     , r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP    yLOG_point   ("a_hist"    , a_hist);
   --rce;  if (a_hist == NULL) {
      strlcpy (r_new, KHRONOS_DEFSTATS, LEN_FULL);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (r_new, KHRONOS_DEFSTATS, LEN_FULL);
   l = strlen (a_hist);
   if (l > 60)  l = 60;
   /*---(update roll)--------------------*/
   n = TRKS__count2num (a_roll);
   DEBUG_OUTP    yLOG_complex ("a_roll"    , "%c, %d", a_roll, n);
   --rce;  if (n < 0) {
      strlcpy (r_new, KHRONOS_DEFSTATS, LEN_FULL);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   r_new [0] = a_roll;
   /*---(run history)--------------------*/
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr ("·´", a_hist [i]) != NULL) {
         DEBUG_OUTP   yLOG_complex ("trailing"  , "at pos %d, found trailing · or ´, ending", i);
         break;
      }
      p = strchr (KHRONOS_STAT, a_hist [i]);
      if (p == NULL || a_hist [i] == KHRONOS_BEG) {
         DEBUG_OUTP   yLOG_complex ("bad char"  , "at pos %d, found bad char (%c), stopping", i, a_hist [i]);
         strlcpy (r_new, KHRONOS_DEFSTATS, LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = TRKS__stats (r_new, KHRONOS_BEG);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_note    ("failed to begin a job");
         strlcpy (r_new, KHRONOS_DEFSTATS, LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_code = a_hist [i];
      rc = TRKS__stats (r_new, x_code);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_complex ("failed"    , "at pos %d, could not update with (%c), stopping", i, a_hist [i]);
         strlcpy (r_new, KHRONOS_DEFSTATS, LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}



#define    KHRONOS_DURS  "<nmlkjihgfedcba0ABCDEFGHIJKLMN>"

/*> < -30     -10        0         10      30 >  --history--->>>                                                    <* 
 *> · · · · · ·········· · ·········· · · · · ·  · ´····´····´····´····´····´····´····´····´····´····´····´····´"   <* 
 *> -123456789-123456789-123456789-123456789-12                                                                     <* 
 *> 0         1         2         3         4                                                                       <*/

/*> ´  · ·  · · · · · ·········· · ·········· · · · · ·  ·)´····´····´····´····´····´····´····´····´····´····´····´····´   <* 
 *> -123456789-123456789-123456789-123456789-123456789-123456789-123456789                                                 <* 
 *> 0         1         2         3         4         5         6                                                          <*/

char
TRKS__durs              (char *b_str, int a_dur, int a_min, int a_est, int a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   char       *p           = NULL;
   char        n           =    0;
   char        x_roll      =    0;
   char        x_hist      =    0;
   char        x_next      =    0;
   char        x_ch        =    0;
   char        x_off       =    0;
   int         x_dur       =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = TRKS__coded ('d', KHRONOS_DEFDURS  , b_str);
   DEBUG_OUTP    yLOG_value   ("coded"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   x_roll  = TRKS__count2num (b_str [ 0]);
   if (x_roll > 60)  x_roll = 60;
   DEBUG_OUTP    yLOG_complex ("x_roll"    , "%c, %d", b_str [ 0], x_roll);
   x_hist  = TRKS__count2num (b_str [53]);
   x_next  = x_hist + 55;
   DEBUG_OUTP    yLOG_complex ("x_hist"    , "%c, %d, %d", b_str [53], x_hist, x_next);
   l = strlen (KHRONOS_DURS);
   x_dur = a_dur - a_est;
   /*---(negatives)----------------------*/
   if      (x_dur  <  -30)  { n = 0;           x_off =  0; }
   else if (x_dur  <  -25)  { n = 1;           x_off =  2; }
   else if (x_dur  <  -20)  { n = 2;           x_off =  4; }
   else if (x_dur  <  -15)  { n = 3;           x_off =  6; }
   else if (x_dur  <  -10)  { n = 4;           x_off =  8; }
   else if (x_dur  <    0)  { n = 15 + x_dur;  x_off = 20 + x_dur; }
   /*---(zero)---------------------------*/
   else if (x_dur  ==   0)  { n = 15;          x_off = 21; }
   /*---(positives)----------------------*/
   else if (x_dur  >   30)  { n = l - 1;       x_off = 42; }
   else if (x_dur  >   25)  { n = l - 2;       x_off = 40; }
   else if (x_dur  >   20)  { n = l - 3;       x_off = 38; }
   else if (x_dur  >   15)  { n = l - 4;       x_off = 36; }
   else if (x_dur  >   10)  { n = l - 5;       x_off = 34; }
   else                     { n = 15 + x_dur;  x_off = 22 + x_dur; }
   /*---(updates)------------------------*/
   x_ch   = KHRONOS_DURS [n];
   x_off += 8;
   DEBUG_OUTP    yLOG_complex ("values"    , "%d, %d, %d, %c", x_dur, n, x_off, x_ch);
   /*---(update history)-----------------*/
   b_str [x_next] = TRKS__num2biggun (a_dur);
   b_str [53    ] = TRKS__num2biggun (++x_hist);
   /*---(update stats)-------------------*/
   n = TRKS__count2num (b_str [x_off]);
   b_str [x_off ] = TRKS__num2count (++n);
   /*---(early/late)---------------------*/
   if (a_dur < a_min) {
      n = TRKS__count2num (b_str [3]);
      b_str [3] = TRKS__num2count (++n);
   }
   if (a_dur > a_max) {
      n = TRKS__count2num (b_str [5]);
      b_str [5] = TRKS__num2count (++n);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__redur_full        (char a_roll, cchar *a_hist, int a_min, int a_est, int a_max, char *r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   char        i           =    0;
   char       *p           =    0;
   char        n           =    0;
   char        x_dur       =    0;
   char        x_trans     [31]  = { -60, -30, -25, -20, -15, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 60 };
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_point   ("r_new"     , r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP    yLOG_point   ("a_hist"    , a_hist);
   --rce;  if (a_hist == NULL) {
      strlcpy (r_new, KHRONOS_DEFDURS, LEN_FULL);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (r_new, KHRONOS_DEFDURS, LEN_FULL);
   l = strlen (a_hist);
   if (l > 60)  l = 60;
   /*---(update roll)--------------------*/
   n = TRKS__count2num (a_roll);
   DEBUG_OUTP    yLOG_complex ("a_roll"    , "%c, %d", a_roll, n);
   --rce;  if (n < 0) {
      strlcpy (r_new, KHRONOS_DEFDURS, LEN_FULL);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   r_new [0] = a_roll;
   /*---(run history)--------------------*/
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr ("·´", a_hist [i]) != NULL) {
         DEBUG_OUTP   yLOG_complex ("trailing"  , "at pos %d, found trailing · or ´, ending", i);
         break;
      }
      p = strchr (YSTR_BIGGUN, a_hist [i]);
      if (p == NULL) {
         DEBUG_OUTP   yLOG_complex ("bad char"  , "at pos %d, found bad char (%c), stopping", i, a_hist [i]);
         strlcpy (r_new, KHRONOS_DEFDURS, LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      n = p - YSTR_BIGGUN;
      rc = TRKS__durs (r_new, n, a_min, a_est, a_max);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_complex ("failed"    , "at pos %d, could not update with (%c), stopping", i, a_hist [i]);
         strlcpy (r_new, KHRONOS_DEFDURS, LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      execution support                       ----===*/
/*====================------------------------------------====================*/
static void      o___EXEC____________________o (void) {;}

char
TRKS_launch             (tTRKS *a_cur, char a_hr, char a_mn)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_point   ("a_cur"     , a_cur);
   --rce;  if (a_cur == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update statistics)--------------*/
   rc = TRKS__stats  (a_cur->stats, KHRONOS_BEG);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update schedule)----------------*/
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS_running            (tTRKS *a_cur, char a_hr, char a_mn)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_point   ("a_cur"     , a_cur);
   --rce;  if (a_cur == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update schedule)----------------*/
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS_complete           (tTRKS *a_cur, char a_hr, char a_mn, char a_code, int a_dur, int a_min, int a_est ,int a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_point   ("a_cur"     , a_cur);
   --rce;  if (a_cur == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update statistics)--------------*/
   rc = TRKS__stats  (a_cur->stats, a_code);
   DEBUG_OUTP    yLOG_value   ("stats"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update duration)----------------*/
   rc = TRKS__durs   (a_cur->durs, a_dur, a_min, a_est, a_max);
   DEBUG_OUTP    yLOG_value   ("durs"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update schedule)----------------*/
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
TRKS__unit              (char *a_question, int a_num)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_exist     =  '-';
   int         x_lines     =    0;
   char        x_heartbeat [LEN_HUND];
   tTRKS      *x_cur       = NULL;
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_HUND]  = "";
   char        r           [LEN_LABEL] = "";
   int         x_fore      =    0;
   int         x_back      =    0;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "RPTG             : question not understood", LEN_HUND);
   /*---(simple requests)----------------*/
   if      (strcmp (a_question, "file"          )  == 0) {
      x_lines = yURG_peek_count (s_name);
      if (x_lines < 0)    x_lines = 0;
      else                x_exist = 'y';
      if (f_trks == NULL)  snprintf (unit_answer, LEN_HUND, "TRKS file        : %c  %3d  null          %2då%sæ", x_exist, x_lines, strlen (s_name), s_name);
      else                 snprintf (unit_answer, LEN_HUND, "TRKS file        : %c  %3d  %-10.10p  %2då%sæ", x_exist, x_lines, f_trks, strlen (s_name), s_name);
   }
   else if (strcmp (a_question, "count"         )  == 0) {
      x_cur = s_head; while (x_cur != NULL) { ++x_fore; x_cur = x_cur->m_next; }
      x_cur = s_tail; while (x_cur != NULL) { ++x_back; x_cur = x_cur->m_prev; }
      snprintf (unit_answer, LEN_HUND, "TRKS count       : %3da, %3df, %3db", s_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "entry"         )  == 0) {
      rc = TRKS__by_index (a_num, &x_cur);
      if (x_cur  == NULL)  snprintf (unit_answer, LEN_HUND, "TRKS entry  (%2d) :  ·åæ                            ·åæ                            · · · ·   ·", a_num);
      else {
         sprintf  (s, "%2då%-.25sæ", strlen (x_cur->file)   , x_cur->file);
         sprintf  (t, "%2då%-.25sæ", strlen (x_cur->tracker), x_cur->tracker);
         strcpy (r, "· · · ·   ·");
         if (strcmp (x_cur->last  , KHRONOS_DEFLAST  ) != 0)  r [ 0] = 'Ï';
         if (strcmp (x_cur->stats , KHRONOS_DEFSTATS ) != 0)  r [ 2] = 'Ï';
         if (strcmp (x_cur->durs  , KHRONOS_DEFDURS  ) != 0)  r [ 4] = 'Ï';
         if (strcmp (x_cur->actual, KHRONOS_DEFACTUAL) != 0)  r [ 6] = 'Ï';
         if (x_cur->parent != NULL)                           r [10] = 'Ï';
         snprintf (unit_answer, LEN_HUND, "TRKS entry  (%2d) : %-29.29s  %-29.29s   %s", a_num, s, t, r);
      }

   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


