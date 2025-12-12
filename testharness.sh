!# /bin/bash
#
# tharn.sh
#
# test harness script for fieldentry C program
#


tput clear

#       1234512345
echo
echo "=============="
echo "==          =="
echo "=============="

tput cup 2 2

if [ $# -ge 1 ]
then
  value="$1"
else
  value=""
fi

newvalue=`./fieldentry "$value" 10`

echo
echo
echo
echo "New value is \"$newvalue\""
echo

exit 0
