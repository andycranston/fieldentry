# fieldentry - A small C program to allow a user to enter ASCII text into a fixed width field

The `fieldentry` command is designed to be used in a shell script
in a fashion similar to:

```
tput clear
tput cup 2 10
value=`fieldentry "default" 20`
tput cup 4 0
echo $value
```

The fieldentry command accepts input from the keyboard
which allows the user to add, delete and otherwise
modify the intial value (in the above example the string "default").
When the user presses the return key the value entered is displayed
on standard output. So in the above example the entered value
is saved in the `value` shell variable where it can be used in
further process in the shell script.

## Command line arguments

...
...

Readme needs further work - remind me if I forget :-]
