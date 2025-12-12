!# /bin/bash
#
# @(!--#) @(#) formsentry.sh, sversion 0.1.0, fversion 001, 12-december-2025
#
# a demo using the fieldentry command
#

set -u

PATH=/bin:/usr/bin:`pwd`
export PATH

scriptname=`basename $0`

fname=""
sname=""
title=""
scode=1

tput clear

echo "Ways to greet people"
echo "--------------------"
echo
echo "First name ........: $fname"
echo "Surname ...........: $sname"
echo "Title .............: $title"
echo "Salutation code ...: $scode"

while true
do
  tput cup 3 21
  fname=`fieldentry "$fname" 15`

  tput cup 4 21
  sname=`fieldentry "$sname" 15`

  tput cup 5 21
  title=`fieldentry "$title" 15`

  tput cup 6 21
  scode=`fieldentry "$scode" 15`

  if [ "$scode" == "q" ]
  then
    break
  fi

  case "$scode" in
    1)
      result="Dear $title $fname $sname"
      ;;
    2)
      result="Hello $fname $sname"
      ;;
    3)
      result="Hiya $fname"
      ;;
    4)
      result="The Fonz says \"Heeey...... $title `echo $sname | cut -c1`\""
      ;;
    *)
      result="** Invalid salutation code **"
      ;;
  esac

  tput cup 8 0
  echo "                                                    "

  tput cup 8 0
  echo "$result"
done

tput clear

exit 0
