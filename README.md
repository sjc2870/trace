# trace
A simple function graph tracer
USAGE:
ubuntu@linux-lab:/labs/linux-lab/test/test_instrument-functions$ gcc trace.c -finstrument-functions -ldl -g
ubuntu@linux-lab:/labs/linux-lab/test/test_instrument-functions$ ./a.out
-> main at trace.c:100
-> bar at trace.c:95
<- /* end of bar at trace.c:95
<- /* end of main at trace.c:100
