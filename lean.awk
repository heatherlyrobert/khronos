#!/bin/awk -f
BEGIN {
   #printf("testing\n\n");
}

$0 ~ /#include   "khronos.h"/ {
   printf ("#include      \"ukhronos.h\"\n");
}

$0 ~ /CHATTY/ {
   printf ("\n");
}

$0 ~ /DEBUG/ {
   printf ("\n");
}

$0 ~ /TEST/ {
   printf ("\n");
}

$0 !~ /CHATTY/ && $0 !~ /DEBUG/ && $0 !~ /TEST/ && $0 !~ /#include   "khronos.h"/ {
   printf("%s\n", $0);
}
