# fieldentry - A small C program to allow a user to enter ASCII text into a fixed width field

The `fieldentry` command is designed to be used in a shell script in a
fashion similar to:

```
tput clear
tput cup 2 10
value=`fieldentry "default" 20`
tput cup 4 0
echo $value
```

The `fieldentry` command accepts input from the keyboard which allows the
user to add, delete and otherwise modify the initial value (in the above
example the string "default"). When the user presses the return key the
value entered is displayed on standard output. So in the above example
the entered value is saved in the `value` shell variable where it can
be used in further processing in the shell script.

## Examples

Look at the bash shell scripts `testharness.sh` and `formsentry.sh` in the
repository to see the `fieldentry` command at work.

## Restrictions

Only plain ASCII characters can be used. Accented characters, currency
symbols such as £ and any other character that is not a 7 bit printable
ASCII character CANNOT be entered using the `fieldentry` command.

I think to call this a bug is unfair but calling it a feature is lazy so
I have settled on a compromise of calling it a restriction. Your milage
may vary :-]

----------------
End of README.md
