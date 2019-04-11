#!/bin/bash
if [ $# -ge 1 ]; then
   MYCUSTOM='   '
   for var in "$@"; do
      input="$(echo $var | tr '[a-z]' '[A-Z]')"
      line1="#ifndef ${input}_H"
      line2="#define ${input}_H"
      line3="#endif"
      echo $line1 > ${var}.h
      echo "${MYCUSTOM}"$line2 >> ${var}.h
      echo $line3 >> ${var}.h
   done
else
   echo Usage: [headerName]
fi
