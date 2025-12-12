!# /bin/bash
#
# @(!--#) @(#) testharness.sh, sversion 0.1.0, fversion 001, 12-december-2025
#
# test harness script for fieldentry C program
#

if [ $# -ge 1 ]
then
  value="$1"
else
  value=""
fi

tput clear

echo "============"
echo "=          ="
echo "============"

tput cup 1 1

value=`./fieldentry "$value" 10`

tput cup 4 0
echo "Value is \"$value\""

exit 0
