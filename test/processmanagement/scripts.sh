sc1
2448  ->bash       2392  ->ppid
2392  ->login

sc2
2631  ->bash       2593 ->ppid

sc3
2657 ->bash        2574 ->ppid

sc3
2712 ->bash        2395 ->ppid

renice -6 -p 2473 2772 2542 2577
renice -6 -p 2772 2542 2577
ps -ely
S   UID   PID  PPID  C PRI  NI   RSS    SZ WCHAN  TTY          TIME CMD

current bash only -:> 
ps -l