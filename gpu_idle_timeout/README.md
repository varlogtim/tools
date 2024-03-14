# GPU Idle Timeout
Launch a process and watch GPU usage. If usage drops below a threshhold for specified time window, the process will be killed.

# Help
```
$ python gpu_idle_timeout.py --help
usage: gpu_idle_timeout [-h] [-t THRESHHOLD_PERCENTAGE] [-s SAMPLE_FREQ] [-n NUM_SAMPLES] [-d DELAY_SAMPLES] [-x] ...

Launches a process and terminates when GPU usage drops below threshhold during sample window. Note: requires 'nvidia-smi' binary to be accessible in PATH

positional arguments:
  exec_command_and_args
                        Command and arguments to execute

optional arguments:
  -h, --help            show this help message and exit
  -t THRESHHOLD_PERCENTAGE, --threshhold-percentage THRESHHOLD_PERCENTAGE
                        Threshhold in percentage of GPU usage which triggers a failure. Must be greater than 0. (default: 1)
  -s SAMPLE_FREQ, --sample-freq SAMPLE_FREQ
                        How frequently, in seconds, to sample the GPU usage. Must be greater than 0. (default: 5)
  -n NUM_SAMPLES, --num-samples NUM_SAMPLES
                        Number of samples in window used to evaluate GPU usage. Must be greater than 0. (default: 60)
  -d DELAY_SAMPLES, --delay-samples DELAY_SAMPLES
                        Number of samples to delay before evaluating GPU usage. (default: 12)
  -x, --debug           actually apply the changes (default: False)
```

# Description

1. We launch the `exec_command_and_args` in a subprocess.
2. We wait SAMPLE_FREQ * DELAY_SAMPLES seconds before starting to monitor. This is to account for instances in which the `exec_command_and_args` takes some time to start using the GPU.
3. Every SAMPLE_FREQ seconds we record the GPU usage for all GPUs on the system.
4. We do not do any evaluation of the usage metrics until NUM_SAMPLES samples have been collected
5. Once the above condition is true, we check if the GPU usage was below THRESHHOLD_PERCENTAGE on *all* samples. If so, we exit.


# Example usage:

In this example I have specified the following I am executing `grep foo`, which will just hang indefinitely, to show how this operates.

```
$ python gpu_idle_timeout.py -t 10 -s 10 -n 5 -d 3 -x grep foo
2024-03-14 15:53:17,743: INFO: gpu_idle_timeout: Starting GPU idle watcher: threshhold_percentage=10%, sample_freq=10, num_samples=5, delay_samples=3, window_size=50
2024-03-14 15:53:27,753: INFO: gpu_idle_timeout: waiting for delay samples. 20 seconds left
2024-03-14 15:53:37,763: INFO: gpu_idle_timeout: waiting for delay samples. 10 seconds left
2024-03-14 15:53:47,793: DEBUG: gpu_idle_timeout: GPU-a5b02c30-9c95-cb81-e41e-ca204d57aa3b usage 0%
2024-03-14 15:53:47,793: DEBUG: gpu_idle_timeout: GPU-b805d632-775b-c673-fe68-26b26996cd09 usage 0%
2024-03-14 15:53:57,821: DEBUG: gpu_idle_timeout: GPU-a5b02c30-9c95-cb81-e41e-ca204d57aa3b usage 0%
2024-03-14 15:53:57,821: DEBUG: gpu_idle_timeout: GPU-b805d632-775b-c673-fe68-26b26996cd09 usage 0%
2024-03-14 15:54:07,850: DEBUG: gpu_idle_timeout: GPU-a5b02c30-9c95-cb81-e41e-ca204d57aa3b usage 0%
2024-03-14 15:54:07,850: DEBUG: gpu_idle_timeout: GPU-b805d632-775b-c673-fe68-26b26996cd09 usage 0%
2024-03-14 15:54:17,880: DEBUG: gpu_idle_timeout: GPU-a5b02c30-9c95-cb81-e41e-ca204d57aa3b usage 0%
2024-03-14 15:54:17,880: DEBUG: gpu_idle_timeout: GPU-b805d632-775b-c673-fe68-26b26996cd09 usage 0%
2024-03-14 15:54:27,910: DEBUG: gpu_idle_timeout: GPU-a5b02c30-9c95-cb81-e41e-ca204d57aa3b usage 0%
2024-03-14 15:54:27,911: ERROR: gpu_idle_timeout: GPU-a5b02c30-9c95-cb81-e41e-ca204d57aa3b usage below threshhold 10% for last 50 seconds. Killing process.
2024-03-14 15:54:27,911: ERROR: gpu_idle_timeout: process still alive, killing.
```
