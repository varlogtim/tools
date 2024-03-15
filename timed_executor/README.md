# Time Executor
A script which executes a command a specified number of times in parallel and records the return code and execution time.

# Use Cases:
- Evaluating response times of endpoints.
- Evaluating failure times for executables.
- Evaluating concurrency behavior.

# Help:

```
$ python timed_executor.py --help
usage: timed executor [-h] -n NUM -c COMMAND

execute a command multiple times and time the output

optional arguments:
  -h, --help            show this help message and exit
  -n NUM, --num NUM     the number of instances to run
  -c COMMAND, --command COMMAND
                        the command to run

fin
```

# Example usage:
Here I am `curl`'ing a local REST server which appears to have some errors and some time delays.
```
$ python timed_executor.py -n 10 -c "curl -s -f http://localhost:5000/widgets"
proc[0] returned: 22 in 0.01071239 seconds
proc[4] returned: 22 in 0.00939918 seconds
proc[6] returned: 22 in 0.01034927 seconds
proc[3] returned: 0 in 1.01066327 seconds
proc[8] returned: 0 in 1.01179647 seconds
proc[5] returned: 22 in 2.01113915 seconds
proc[9] returned: 22 in 2.01043415 seconds
proc[2] returned: 22 in 6.01668167 seconds
proc[1] returned: 0 in 7.01701427 seconds
proc[7] returned: 0 in 7.01877928 seconds
```
