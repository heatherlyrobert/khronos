/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



char        g_seq   =  0;
char        g_print     [LEN_RECD] = "";



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
   a_cur->f_seq     =   -1;
   ystrlcpy (a_cur->f_title, "" , LEN_TITLE);
   ystrlcpy (a_cur->f_user , "" , LEN_USER);
   a_cur->f_uid     =   -1;
   ystrlcpy (a_cur->f_note , "-", LEN_TERSE);
   a_cur->f_tried   =    0;
   a_cur->f_built   =    0;
   a_cur->f_warn    =    0;
   a_cur->f_valid   =  '�';
   a_cur->f_retired = '-';
   /*---(complete)-------------*/
   return 1;
}

char*
FILE__memory            (tFILE *a_cur)
{
   int         n           =    0;
   ystrlcpy (g_print, "�____.__.__�", LEN_RECD);
   ++n;  if (a_cur->f_seq       >= 0)           g_print [n] = 'X';
   ++n;  if (a_cur->f_title [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->f_user  [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->f_uid       >= 0)           g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->f_note  [0] != '-')         g_print [n] = 'X';
   ++n;  if (a_cur->f_tried     >  0)           g_print [n] = 'X';
   ++n;  if (a_cur->f_built     >  0)           g_print [n] = 'X';
   ++n;  if (a_cur->f_warn      >  0)           g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->f_valid     != '�')         g_print [n] = 'X';
   ++n;  if (a_cur->f_retired   != '-')         g_print [n] = 'X';
   return g_print;
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
   tFILE      *x_dup       = NULL;
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
   --rce;  if (a_name == NULL || strlen (a_name) <  3) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   DEBUG_INPT   yLOG_point   ("a_user"    , a_user);
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   --rce;  if (a_user == NULL || strlen (a_user) <  3) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   /*---(check for duplicate)------------*/
   rc = yDLST_list_by_name (a_name, NULL, &x_dup);
   DEBUG_INPT   yLOG_point   ("x_dup"     , x_dup);
   --rce;  if (x_dup  != NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create data)--------------------*/
   rc = FILE__new (&x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   DEBUG_INPT   yLOG_info    ("file"      , a_name);
   ystrlcpy (x_file->f_title, a_name, LEN_TITLE);
   DEBUG_INPT   yLOG_info    ("user"      , a_user);
   ystrlcpy (x_file->f_user , a_user, LEN_USER);
   DEBUG_INPT   yLOG_value   ("uid"       , a_uid);
   x_file->f_uid   = a_uid;
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_file->f_title, x_file);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(assign sequence)----------------*/
   x_file->f_seq = g_seq++;
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
   DEBUG_INPT   yLOG_value   ("f_built"   , x_file->f_built);
   --rce;  if (x_file->f_built > 0) {
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
   ystrlcpy (my.f_name, "", LEN_HUND);
   ystrlcpy (my.f_user, "", LEN_USER);
   my.f_uid = -1;
   ystrlcpy (my.f_desc, "", LEN_DESC);
   ystrlcpy (my.f_full, "", LEN_DESC);
   ystrlcpy (my.f_new , "", LEN_DESC);
   return 0;
}

/*> char                                                                                                                                  <* 
 *> FILE_assimilate_OLD    (cchar a_loc, cchar *a_name, char *r_user, char *r_desc)                                                       <* 
 *> {                                                                                                                                     <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                           <* 
 *>    char        rce         =  -10;                                                                                                    <* 
 *>    char        rc          =    0;                                                                                                    <* 
 *>    tFILE      *x_file      = NULL;                                                                                                    <* 
 *>    int         c           =    0;                                                                                                    <* 
 *>    char        x_dir       [LEN_PATH]  = "";                                                                                          <* 
 *>    /+---(header)-------------------------+/                                                                                           <* 
 *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                                                                           <* 
 *>    DEBUG_INPT  yLOG_char    ("a_loc"     , a_loc);                                                                                    <* 
 *>    /+---(default)------------------------+/                                                                                           <* 
 *>    if (r_user != NULL)  ystrlcpy (r_user, ""       , LEN_USER);                                                                        <* 
 *>    if (r_desc != NULL)  ystrlcpy (r_desc, ""       , LEN_DESC);                                                                        <* 
 *>    /+---(parse file)---------------------+/                                                                                           <* 
 *>    --rce;  if (a_loc == YEXEC_CENTRAL)   rc = yEXEC_central    (my.run_as, a_name, &(my.f_user), &(my.f_uid), &(my.f_desc), x_dir);   <* 
 *>    else if    (a_loc == YEXEC_LOCAL  )   rc = yEXEC_acceptable (my.run_as, a_name, &(my.f_user), &(my.f_uid), &(my.f_desc), x_dir);   <* 
 *>    else {                                                                                                                             <* 
 *>       DEBUG_INPT  yLOG_note    ("location is illegal");                                                                               <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
 *>       return rce;                                                                                                                     <* 
 *>    }                                                                                                                                  <* 
 *>    DEBUG_INPT   yLOG_value   ("parse"     , rc);                                                                                      <* 
 *>    --rce;  if (rc < 0) {                                                                                                              <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
 *>       return rce;                                                                                                                     <* 
 *>    }                                                                                                                                  <* 
 *>    /+---(save back)----------------------+/                                                                                           <* 
 *>    if (r_user != NULL)  ystrlcpy (r_user, my.f_user, LEN_USER);                                                                        <* 
 *>    if (r_desc != NULL)  ystrlcpy (r_desc, my.f_desc, LEN_DESC);                                                                        <* 
 *>    /+---(create file)--------------------+/                                                                                           <* 
 *>    yURG_msg ('>', "assimilate file lines...");                                                                                        <* 
 *>    /+> rc = FILE_create (my.f_name, my.f_user, my.f_uid);                             <+/                                             <* 
 *>    rc = FILE_create (a_name, my.f_user, my.f_uid);                                                                                    <* 
 *>    DEBUG_INPT   yLOG_value   ("file"      , rc);                                                                                      <* 
 *>    --rce;  if (rc < 0) {                                                                                                              <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
 *>       return rce;                                                                                                                     <* 
 *>    }                                                                                                                                  <* 
 *>    yURG_msg ('-', "created a list to house the lines");                                                                               <* 
 *>    /+---(get current file)---------------+/                                                                                           <* 
 *>    yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);                                                                                  <* 
 *>    DEBUG_INPT  yLOG_point   ("x_file"    , x_file);                                                                                   <* 
 *>    --rce;  if (x_file == NULL) {                                                                                                      <* 
 *>       yURG_msg ('-', "file failed");                                                                                                  <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
 *>       return rce;                                                                                                                     <* 
 *>    }                                                                                                                                  <* 
 *>    /+---(prepare)------------------------+/                                                                                           <* 
 *>    rc = ySCHED_newfile ();                                                                                                            <* 
 *>    DEBUG_INPT   yLOG_value   ("ySCHED"    , rc);                                                                                      <* 
 *>    --rce;  if (rc < 0) {                                                                                                              <* 
 *>       yURG_msg ('-', "schedule failed");                                                                                              <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
 *>       return rce;                                                                                                                     <* 
 *>    }                                                                                                                                  <* 
 *>    yURG_msg ('-', "initialized the valid date for scheduling");                                                                       <* 
 *>    yURG_msg ('-', "calling auto-reader");                                                                                             <* 
 *>    yURG_msg (' ', "");                                                                                                                <* 
 *>    /+---(pull all lines)-----------------+/                                                                                           <* 
 *>    sprintf (my.f_full, "%s%s", x_dir, a_name);                                                                                        <* 
 *>    DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);                                                                               <* 
 *>    rc = yPARSE_autoread (my.f_full, NULL, LINE_handler);                                                                              <* 
 *>    DEBUG_PROG  yLOG_value   ("read"      , rc);                                                                                       <* 
 *>    --rce;  if (rc <  0) {                                                                                                             <* 
 *>       ystrlcpy (x_file->f_note, "NO FILE" , LEN_TERSE);                                                                                  <* 
 *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
 *>       return rce;                                                                                                                     <* 
 *>    }                                                                                                                                  <* 
 *>    c  = yDLST_line_count (YDLST_LOCAL);                                                                                               <* 
*>    --rce;  if (c != x_file->f_built) {                                                                                                  <* 
   *>       yURG_msg ('>', "all lines read, ERRORS, reviewed %d, accepted %d", x_file->f_built, c);                                           <* 
      *>       yURG_msg (' ', "");                                                                                                             <* 
      *>       ystrlcpy (x_file->f_note, "ERRORS"  , LEN_TERSE);                                                                                  <* 
      *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
      *>       return rce;                                                                                                                     <* 
      *>    } else if (c == 0) {                                                                                                               <* 
         *>       yURG_msg ('>', "all lines read, EMPTY, reviewed %d, accepted %d", x_file->f_built, c);                                            <* 
            *>       yURG_msg (' ', "");                                                                                                             <* 
            *>       ystrlcpy (x_file->f_note, "EMPTY"   , LEN_TERSE);                                                                                  <* 
            *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                   <* 
            *>       return rce;                                                                                                                     <* 
            *>    }                                                                                                                                  <* 
            *>    yURG_msg ('>', "all lines read, SUCCESS, reviewed %d, accepted %d", x_file->f_built, c);                                             <* 
            *>    yURG_msg (' ', "");                                                                                                                <* 
            *>    ystrlcpy (x_file->f_note, "success"  , LEN_TERSE);                                                                                    <* 
            *>    /+---(complete)-----------------------+/                                                                                           <* 
            *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                                                                           <* 
            *>    return 0;                                                                                                                          <* 
            *> }                                                                                                                                     <*/


            /*> char                                                                                                                                    <* 
             *> FILE_assimilate         (cchar a_runas, cchar a_loc, cchar *a_name, char *r_user, char *r_desc)                                         <* 
             *> {                                                                                                                                       <* 
             *>    /+---(locals)-----------+-----+-----+-+/                                                                                             <* 
             *>    char        rce         =  -10;                                                                                                      <* 
             *>    char        rc          =    0;                                                                                                      <* 
             *>    int         x_lists     =    0;                                                                                                      <* 
             *>    int         x_lines     =    0;                                                                                                      <* 
             *>    int         x_seqs      =    0;                                                                                                      <* 
             *>    tFILE      *x_file      = NULL;                                                                                                      <* 
             *>    int         c           =    0;                                                                                                      <* 
             *>    /+---(header)-------------------------+/                                                                                             <* 
             *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                                                                             <* 
             *>    DEBUG_INPT  yLOG_char    ("a_runas"   , a_runas);                                                                                    <* 
             *>    DEBUG_INPT  yLOG_char    ("a_loc"     , a_loc);                                                                                      <* 
             *>    DEBUG_INPT  yLOG_info    ("a_name"     , a_name);                                                                                    <* 
             *>    /+---(default)------------------------+/                                                                                             <* 
             *>    if (r_user != NULL)  ystrlcpy (r_user, ""       , LEN_USER);                                                                          <* 
             *>    if (r_desc != NULL)  ystrlcpy (r_desc, ""       , LEN_DESC);                                                                          <* 
             *>    /+---(default)------------------------+/                                                                                             <* 
             *>    /+> yDLST_purge ();                                                                <+/                                               <* 
             *>    /+> strcpy  (my.f_name, "");                                                       <*                                                <* 
             *>     *> strcpy  (my.f_full, "");                                                       <+/                                               <* 
             *>    /+> my.f_lines = 0;                                                                <*                                                <* 
             *>     *> my.f_group = '-';                                                              <*                                                <* 
             *>     *> my.f_gall  = my.f_gpass = my.f_gfail = 0;                                      <*                                                <* 
             *>     *> my.f_aall  = my.f_apass = my.f_afail = 0;                                      <*                                                <* 
             *>     *> my.f_pall  = my.f_ppass = my.f_pfail = 0;                                      <+/                                               <* 
             *>    /+> strcpy  (my.f_note, "");                                                       <+/                                               <* 
             *>    /+---(parse file)---------------------+/                                                                                             <* 
             *>    /+> if      (a_loc == YJOBS_CENTRAL)   rc = yJOBS_central    (a_runas, a_name, my.f_user, &(my.f_uid), my.f_desc, my.f_dir);   <+/   <* 
             *>    /+> else if (a_loc == YJOBS_LOCAL  )   rc = yJOBS_acceptable (a_runas, a_name, my.f_user, &(my.f_uid), my.f_desc, my.f_dir);   <+/   <* 
             *>    DEBUG_INPT   yLOG_value   ("parse"     , rc);                                                                                        <* 
             *>    --rce;  if (rc < 0) {                                                                                                                <* 
             *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
             *>       return rce;                                                                                                                       <* 
             *>    }                                                                                                                                    <* 
             *>    /+---(save back)----------------------+/                                                                                             <* 
             *>    if (r_user != NULL)  ystrlcpy (r_user, my.f_user, LEN_USER);                                                                          <* 
             *>    if (r_desc != NULL)  ystrlcpy (r_desc, my.f_desc, LEN_DESC);                                                                          <* 
             *>    /+---(create file)--------------------+/                                                                                             <* 
             *>    yURG_msg ('>', "create file entity and add to yDLST...");                                                                            <* 
             *>    yURG_msg ('-', "file is �%s� in %c", a_name, a_loc);                                                                                 <* 
             *>    yURG_msg ('-', "owner is �%s� as %d", my.f_user, my.f_uid);                                                                          <* 
             *>    rc = FILE_create (a_name, my.f_user, my.f_uid);                                                                                      <* 
             *>    DEBUG_INPT   yLOG_value   ("file"      , rc);                                                                                        <* 
             *>    --rce;  if (rc < 0) {                                                                                                                <* 
             *>       yURG_err ('f', "could not create file (%d)", rc);                                                                                 <* 
             *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
             *>       return rce;                                                                                                                       <* 
             *>    }                                                                                                                                    <* 
             *>    yURG_msg ('-', "created a list to house the lines");                                                                                 <* 
             *>    /+---(get current file)---------------+/                                                                                             <* 
             *>    yURG_msg ('-', "find the new/created yDLST entry");                                                                                  <* 
             *>    rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);                                                                               <* 
             *>    DEBUG_INPT  yLOG_point   ("x_file"    , x_file);                                                                                     <* 
             *>    --rce;  if (x_file == NULL) {                                                                                                        <* 
             *>       yURG_err ('f', "returned NULL, could not find to yDLST file (%d)", rc);                                                           <* 
             *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
             *>       return rce;                                                                                                                       <* 
             *>    }                                                                                                                                    <* 
             *>    /+---(read all lines)-----------------+/                                                                                             <* 
             *>    yURG_msg ('-', "calling auto-reader in yPARSE");                                                                                     <* 
             *>    yURG_msg (' ', "");                                                                                                                  <* 
             *>    sprintf (my.f_full, "%s%s", my.f_dir, a_name);                                                                                       <* 
             *>    DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);                                                                                 <* 
             *>    rc = yPARSE_autoread (my.f_full, NULL, LINE_handler);                                                                                <* 
             *>    DEBUG_PROG  yLOG_value   ("read"      , rc);                                                                                         <* 
             *>    --rce;  if (rc <  0) {                                                                                                               <* 
             *>       yURG_err ('F', "found errors (%d)", rc);                                                                                          <* 
             *>       ystrlcpy (x_file->f_note, "NO FILE" , LEN_TERSE);                                                                                    <* 
            *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
            *>       return rce;                                                                                                                       <* 
            *>    }                                                                                                                                    <* 
            *>    /+---(verify the results)-------------+/                                                                                             <* 
            *>    c  = yDLST_line_count (YDLST_LOCAL);                                                                                                 <* 
            *>    --rce;  if (c != x_file->lines) {                                                                                                    <* 
               *>       yURG_err ('F', "all lines read, ERRORS, reviewed %d, accepted %d", x_file->lines, c);                                             <* 
                  *>       yURG_msg (' ', "");                                                                                                               <* 
                  *>       ystrlcpy (x_file->f_note, "ERRORS"  , LEN_TERSE);                                                                                    <* 
                  *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
                  *>       return rce;                                                                                                                       <* 
                  *>    } else if (c == 0) {                                                                                                                 <* 
                     *>       yURG_err ('F', "all lines read, EMPTY, reviewed %d, accepted %d", x_file->lines, c);                                              <* 
                        *>       yURG_msg (' ', "");                                                                                                               <* 
                        *>       ystrlcpy (x_file->f_note, "EMPTY"   , LEN_TERSE);                                                                                    <* 
                        *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
                        *>       return rce;                                                                                                                       <* 
                        *>    }                                                                                                                                    <* 
                        *>    /+---(show success)-------------------+/                                                                                             <* 
                        *>    yURG_msg ('-', "SUCCESS all lines read, reviewed %d, accepted %d", x_file->lines, c);                                                <* 
                        *>    yURG_msg (' ', "");                                                                                                                  <* 
                        *>    ystrlcpy (x_file->f_note, "success"  , LEN_TERSE);                                                                                      <* 
                        *>    /+---(complete)-----------------------+/                                                                                             <* 
                        *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                                                                             <* 
                        *>    return 0;                                                                                                                            <* 
                        *> }                                                                                                                                       <*/


char
FILE_pull_detail        (cchar a_loc, cchar *a_full, cchar *a_fname, cchar *a_fuser, int a_fuid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_full"    , a_full);
   --rce;  if (a_full == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_full"    , a_full);
   ystrlcpy (my.f_full , a_full , LEN_PATH);
   DEBUG_INPT   yLOG_point   ("a_fname"   , a_fname);
   --rce;  if (a_fname == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_fname"   , a_fname);
   ystrlcpy (my.f_name , a_fname, LEN_HUND);
   DEBUG_INPT   yLOG_point   ("a_fuser"   , a_fuser);
   --rce;  if (a_fuser == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_fuser"   , a_fuser);
   ystrlcpy (my.f_user , a_fuser, LEN_USER);
   DEBUG_INPT   yLOG_value   ("a_fuid"    , a_fuid);
   --rce;  if (a_fuid < 0 || a_fuid > 10000) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   my.f_uid  = a_fuid;
   /*---(create file)--------------------*/
   yURG_msg ('>', "verify/load the contents of a file (pull)...");
   yURG_msg ('-', "requested  %2d�%s�", strlen (a_full), a_full);
   yURG_msg ('-', "short name %2d�%s�", strlen (a_fname), a_fname);
   yURG_msg ('-', "ownership  %2d�%s�, uid %d", strlen (a_fuser), a_fuser, a_fuid);
   rc = FILE_create (a_fname, a_fuser, a_fuid);
   DEBUG_INPT   yLOG_value   ("file"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not create file (%d)", rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "created new list to house execution lines");
   /*---(get current file)---------------*/
   rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      yURG_err ('f', "returned NULL, could not find to yDLST file (%d)", rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read all lines)-----------------*/
   yURG_msg ('-', "re-configure yPARSE");
   rc = yPARSE_config (YPARSE_MANUAL, NULL, YPARSE_ONETIME, YPARSE_FIELD, YPARSE_FILL);
   my.t_warning  = 0;
   /*---(read all lines)-----------------*/
   yURG_msg ('-', "calling auto-reader in yPARSE");
   DEBUG_INPT  yLOG_info    ("f_full"     , a_full);
   rc = yPARSE_autoread (a_full, NULL, LINE_handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   --rce;  if (rc <  0) {
      yURG_err ('f', "found errors (%d)", rc);
      /*> ystrlcpy (x_file->f_note, "NO FILE" , LEN_TERSE);                              <*/
      FILE_delete (x_file->f_title);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(final header)-------------------*/
   yURG_msg ('>', "summary of incomming data...");
   x_file->f_warn = my.t_warning;
   DEBUG_PROG  yLOG_value   ("f_warn"    , x_file->f_warn);
   /*---(verify the results)-------------*/
   c  = yDLST_line_count (YDLST_LOCAL);
   --rce;  if (c != x_file->f_tried) {
      yURG_msg ('-', "all lines read, ERRORS, count %d, tried %d, built %d, warns %d", c, x_file->f_tried, x_file->f_built, x_file->f_warn);
      ystrlcpy (x_file->f_note, "ERRORS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   } else if (c == 0) {
      yURG_msg ('-', "all lines read, EMPTY, count %d, tried %d, built %d, warns %d", c, x_file->f_tried, x_file->f_built, x_file->f_warn);
      ystrlcpy (x_file->f_note, "EMPTY"   , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(ckeck warnings)-----------------*/
   if (x_file->f_warn > 0) {
      yURG_msg ('-', "all lines read, WARNINGS, count %d, tried %d, built %d, warns %d", c, x_file->f_tried, x_file->f_built, x_file->f_warn);
      ystrlcpy (x_file->f_note, "warnings", LEN_TERSE);
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 4;
   }
   /*---(show success)-------------------*/
   yURG_msg ('-', "success, all read correctly, count %d, tried %d, built %d, warns %d", c, x_file->f_tried, x_file->f_built, x_file->f_warn);
   ystrlcpy (x_file->f_note, "success"  , LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
FILE_pull               (cchar *a_fname)
{  /*---(design notes)-------------------*/
   /*
    * yJOBS expects a clear return code...
    *    < 0   can not even evaluate data
    *      1   pass
    *      2   known over-rides (still noted)
    *      3   repaired, in some way, to pass
    *      4   outstandard warnings or defaulting
    *      5   data errors
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_loc       =  '-';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_point   ("a_fname"   , a_fname);
   --rce;  if (a_fname == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_fname"   , a_fname);
   /*---(collect data)-------------------*/
   rc = yJOBS_filedata (&(my.run_as), &(my.run_mode), &x_loc, my.f_name, my.f_user, &(my.f_uid), my.f_desc, my.f_dir, my.f_full);
   DEBUG_INPT   yLOG_value   ("filedata"  , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not request file data (%d)", rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_char    ("run_as"    , my.run_as);
   DEBUG_INPT   yLOG_char    ("run_mode"  , my.run_mode);
   DEBUG_INPT   yLOG_char    ("x_loc"     , x_loc);
   DEBUG_INPT   yLOG_info    ("f_name"    , my.f_name);
   DEBUG_INPT   yLOG_info    ("f_user"    , my.f_user);
   DEBUG_INPT   yLOG_value   ("f_uid"     , my.f_uid);
   DEBUG_INPT   yLOG_info    ("f_desc"    , my.f_desc);
   DEBUG_INPT   yLOG_info    ("f_full"    , my.f_full);
   /*---(call detail)--------------------*/
   rc = FILE_pull_detail (x_loc, my.f_full, my.f_name, my.f_user, my.f_uid);
   DEBUG_INPT   yLOG_value   ("detail"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return rc;
}




/*====================------------------------------------====================*/
/*===----                     retirement of files                      ----===*/
/*====================------------------------------------====================*/
static void      o___RETIRE__________________o (void) {;}

/*> char                                                                              <* 
 *> FILE_prune              (void)                                                    <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    int         rc          =    0;                                                <* 
 *>    int         x_running   =    0;                                                <* 
 *>    tLINE      *x_line      = NULL;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(check all lines)----------------+/                                       <* 
 *>    rc = yDLST_list_by_name ("RETIRED", NULL, NULL);                               <* 
 *>    rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);           <* 
 *>    DEBUG_INPT   yLOG_point   ("x_line"    , x_line);                              <* 
 *>    while (x_line != NULL) {                                                       <* 
 *>       /+---(mark retired)----------------+/                                       <* 
 *>       if (x_line->retire == 'y' && x_line->rpid <= 0) {                           <* 
 *>          /+---(destroy inactive)------------+/                                    <* 
 *>          rc = yDLST_line_remove  (x_line->tracker);                               <* 
 *>          DEBUG_INPT   yLOG_value   ("destroy"   , rc);                            <* 
 *>          if (rc < 0) {                                                            <* 
 *>             DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                         <* 
 *>             return rce;                                                           <* 
 *>          }                                                                        <* 
 *>          LINE__free (&x_line);                                                    <* 
 *>          rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);     <* 
 *>          x_running = 0;                                                           <* 
 *>          continue;                                                                <* 
 *>       }                                                                           <* 
 *>       ++x_running;                                                                <* 
 *>       rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DNEXT, NULL, &x_line);        <* 
 *>       /+---(done)------------------------+/                                       <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return x_running;                                                              <* 
 *> }                                                                                 <*/

/*> char                                                                                   <* 
 *> FILE_retire             (char *a_name)                                                 <* 
 *> {                                                                                      <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                            <* 
 *>    char        rce         =  -10;                                                     <* 
 *>    int         rc          =    0;                                                     <* 
 *>    tFILE      *x_retired   = NULL;                                                     <* 
 *>    tFILE      *x_file      = NULL;                                                     <* 
 *>    tLINE      *x_line      = NULL;                                                     <* 
 *>    char        x_tracker   [LEN_TITLE] = "";                                           <* 
 *>    /+---(header)-------------------------+/                                            <* 
 *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                            <* 
 *>    yURG_msg ('>', "retire crontab lines...");                                          <* 
 *>    /+---(defenses)-----------------------+/                                            <* 
 *>    DEBUG_INPT   yLOG_point   ("a_name"    , a_name);                                   <* 
 *>    --rce;  if (a_name == NULL) {                                                       <* 
 *>       yURG_err ('f', "crontab name is null/empty");                                    <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                    <* 
 *>       return rce;                                                                      <* 
 *>    }                                                                                   <* 
 *>    DEBUG_INPT   yLOG_info    ("a_name"    , a_name);                                   <* 
 *>    yURG_msg ('-', "crontab name requested is �%s�", a_name);                           <* 
 *>    /+---(reject retired)-----------------+/                                            <* 
 *>    if (strcmp (a_name, "RETIRED") == 0) {                                              <* 
 *>       DEBUG_INPT   yLOG_note    ("can not retire the RETIRED list");                   <* 
 *>       DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                         <* 
 *>       return 0;                                                                        <* 
 *>    }                                                                                   <* 
 *>    /+---(find retired)-------------------+/                                            <* 
 *>    rc = yDLST_list_by_name ("RETIRED", NULL, &x_retired);                              <* 
 *>    DEBUG_INPT   yLOG_point   ("x_retired" , x_retired);                                <* 
 *>    --rce;  if (x_retired == NULL) {                                                    <* 
 *>       yURG_err ('f', "retired list could not be found");                               <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                    <* 
 *>       return rce;                                                                      <* 
 *>    }                                                                                   <* 
 *>    yURG_msg ('-', "found retired file");                                               <* 
 *>    /+---(find list)----------------------+/                                            <* 
 *>    rc = yDLST_list_by_name (a_name, NULL, &x_file);                                    <* 
 *>    DEBUG_INPT   yLOG_point   ("x_file"    , x_file);                                   <* 
 *>    --rce;  if (x_file == NULL) {                                                       <* 
 *>       yURG_msg ('-', "crontab is not assimilated, nothing to retire");                 <* 
 *>       yURG_msg (' ', "");                                                              <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                    <* 
 *>       return rce;                                                                      <* 
 *>    }                                                                                   <* 
 *>    x_file->f_retired = 'y';                                                            <* 
 *>    yURG_msg ('-', "marking file as retired as precaution");                            <* 
 *>    /+---(check still in use)-------------+/                                            <* 
 *>    DEBUG_INPT   yLOG_value   ("lines"     , x_file->lines);                            <* 
 *>    rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);                <* 
 *>    DEBUG_INPT   yLOG_point   ("x_line"    , x_line);                                   <* 
 *>    while (rc >= 0 && x_line != NULL) {                                                 <* 
 *>       /+---(report out)------------------+/                                            <* 
 *>       DEBUG_INPT   yLOG_value   ("recdno"    , x_line->recdno);                        <* 
 *>       DEBUG_INPT   yLOG_info    ("tracker"   , x_line->tracker);                       <* 
 *>       DEBUG_INPT   yLOG_info    ("command"   , x_line->command);                       <* 
 *>       rc = yDLST_line_remove (x_line->tracker);                                        <* 
 *>       DEBUG_INPT   yLOG_value   ("remove" , rc);                                       <* 
 *>       if (rc < 0) {                                                                    <* 
 *>          yURG_err ('w', "could not remove line �%s� (%d)", x_line->tracker, rc);       <* 
 *>          continue;                                                                     <* 
 *>       }                                                                                <* 
 *>       /+---(inactive)--------------------+/                                            <* 
 *>       if (x_line->rpid <= 0) {                                                         <* 
 *>          DEBUG_INPT   yLOG_note    ("found inactive process, no worries");             <* 
 *>          LINE__free (&x_line);                                                         <* 
 *>          yURG_msg ('-', "line successfully retired");                                  <* 
 *>          rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);          <* 
 *>          continue;                                                                     <* 
 *>       }                                                                                <* 
*>       /+---(mark retired)----------------+/                                            <* 
*>       DEBUG_INPT   yLOG_note    ("found running process, move to retired");            <* 
*>       x_line->retire = 'y';                                                            <* 
*>       /+---(find retired list)--------------+/                                         <* 
*>       rc = yDLST_list_by_name ("RETIRED", NULL, &x_retired);                           <* 
*>       DEBUG_INPT   yLOG_point   ("x_retired" , x_retired);                             <* 
*>       --rce;  if (x_retired == NULL) {                                                 <* 
   *>          yURG_err ('f', "retired list could not be found");                            <* 
      *>          DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                 <* 
      *>          return rce;                                                                   <* 
      *>       }                                                                                <* 
      *>       /+---(add line to retired)------------+/                                         <* 
      *>       rc = yDLST_line_create (x_line->tracker, x_line);                                <* 
      *>       DEBUG_INPT   yLOG_value   ("create"    , rc);                                    <* 
      *>       --rce;  if (rc < 0) {                                                            <* 
         *>          yURG_err ('w', "line could not be created �%s� (%d)", x_line->tracker, rc);   <* 
            *>          continue;                                                                     <* 
            *>       }                                                                                <* 
            *>       /+---(back to original list)----------+/                                         <* 
            *>       rc = yDLST_list_by_name (a_name, NULL, &x_file);                                 <* 
            *>       DEBUG_INPT   yLOG_point   ("x_file"    , x_file);                                <* 
            *>       --rce;  if (x_file == NULL) {                                                    <* 
               *>          yURG_err ('f', "original list could not be found �%s� (%d)", a_name, rc);     <* 
                  *>          DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                 <* 
                  *>          return rce;                                                                   <* 
                  *>       }                                                                                <* 
                  *>       /+---(next line)----------------------+/                                         <* 
                  *>       yURG_msg ('-', "line �%s� running, but successfully moved to retired list");     <* 
                  *>       rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);             <* 
                  *>       DEBUG_INPT   yLOG_point   ("x_line"    , x_line);                                <* 
                  *>       /+---(done)---------------------------+/                                         <* 
                  *>    }                                                                                   <* 
                  *>    /+---(destroy)------------------------+/                                            <* 
                  *>    rc = yDLST_list_destroy (a_name);                                                   <* 
                  *>    DEBUG_INPT   yLOG_value   ("destroy"   , rc);                                       <* 
                  *>    --rce;  if (rc < 0) {                                                               <* 
                     *>       yURG_err ('f', "original list could not be destroyed �%s� (%d)", a_name, rc);    <* 
                        *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                    <* 
                        *>       return rce;                                                                      <* 
                        *>    }                                                                                   <* 
                        *>    yURG_msg ('-', "original list destroyed �%s� ");                                    <* 
                        *>    yURG_msg (' ', "");                                                                 <* 
                        *>    /+---(complete)-----------------------+/                                            <* 
                        *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                            <* 
                        *>    return 0;                                                                           <* 
                        *> }                                                                                      <*/

                        /*> char                                                                                   <* 
                         *> FILE_flush              (void)                                                         <* 
                         *> {                                                                                      <* 
                         *>    char        rc          =    0;                                                     <* 
                         *>    tFILE      *x_file      = NULL;                                                     <* 
                         *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                            <* 
                         *>    rc = yDLST_list_by_cursor (YDLST_DHEAD, NULL, &x_file);                             <* 
                         *>    DEBUG_INPT   yLOG_point   ("x_file"    , x_file);                                   <* 
                         *>    while (rc >= 0 && x_file != NULL) {                                                 <* 
                         *>       /+---(retire)-------------------------+/                                         <* 
                         *>       rc = FILE_retire (x_file->f_title);                                                <* 
                         *>       /+---(next line)----------------------+/                                         <* 
                         *>       yURG_msg ('-', "file �%s� successfully moved to retired list", x_file->f_title);   <* 
                         *>       rc = yDLST_list_by_cursor (YDLST_DHEAD, NULL, &x_file);                          <* 
                         *>       if (rc >= 0 && x_file != NULL && strcmp (x_file->f_title, "RETIRED") == 0) {       <* 
                         *>          rc = yDLST_list_by_cursor (YDLST_DNEXT, NULL, &x_file);                       <* 
                         *>       }                                                                                <* 
                         *>       DEBUG_INPT   yLOG_point   ("x_file"    , x_file);                                <* 
                         *>       /+---(done)---------------------------+/                                         <* 
                         *>    }                                                                                   <* 
                         *>    /+---(complete)-----------------------+/                                            <* 
                         *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                            <* 
                         *>    return 0;                                                                           <* 
                         *> }                                                                                      <*/



                        /*====================------------------------------------====================*/
                        /*===----                      unit test accessor                      ----===*/
                        /*====================------------------------------------====================*/
                        static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
file__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "��";
   char        s           [LEN_RECD]  = "��";
   int         c           =    0;
   tFILE      *x_file      = NULL;
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "FILE             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "file"    )        == 0) {
      sprintf (t, "�%s�", my.f_name);
      snprintf (unit_answer, LEN_HUND, "FILE name        : %2d%-37.37s  %c", strlen (my.f_name), t, my.f_ready);
   }
   else if (strcmp (a_question, "user"    )        == 0) {
      sprintf (t, "�%s�", my.f_user);
      snprintf (unit_answer, LEN_HUND, "FILE user        : %2d%-22.22s  %4d", strlen (my.f_user), t, my.f_uid);
   }
   else if (strcmp (a_question, "desc"    )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE desc        : %2d�%.35s�", strlen (my.f_desc), my.f_desc);
   }
   else if (strcmp (a_question, "new"     )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE new         : %2d�%.35s�", strlen (my.f_new), my.f_new);
   }
   else if (strcmp (a_question, "full"          )  == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE full        : %2d�%.60s�", strlen (my.f_full), my.f_full);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE count       : %d", yDLST_list_count ());
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      rc = yDLST_list_by_index (a_num, NULL, &x_file);
      c  = yDLST_line_count (YDLST_LOCAL);
      if (x_file != NULL) {
         sprintf (t, "%2d�%.30s�", strlen (x_file->f_title), x_file->f_title);
         sprintf (s, "%2d�%.10s�", strlen (x_file->f_user) , x_file->f_user);
         snprintf (unit_answer, LEN_FULL, "FILE entry  (%2d) : %2d  %-34.34s  %-14.14s  %4d  %2d %2d %2d %2d   %s", a_num, x_file->f_seq, t, s, x_file->f_uid, c, x_file->f_tried, x_file->f_built, x_file->f_warn, x_file->f_note);
      } else {
         snprintf (unit_answer, LEN_FULL, "FILE entry  (%2d) :  -   -��                                 -��               -   -  -  -  -   -", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}






