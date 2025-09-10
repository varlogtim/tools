#!/usr/bin/env python3
import re
import sys
from datetime import datetime, timedelta


# [2023-02-25T07:56:41.841535Z]
time_regex = (r"^\[([0-9]{4}-[0-9]{2}-[0-9]{2}T"
              r"[0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]{1,}Z)\].*")
time_format = "%Y-%m-%dT%H:%M:%S.%fZ"

# Another time format:
# Jul 20 10:33:00
time2_regex = (r"([A-Za-z]{3} [0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2})")
time2_format = "%b %d %H:%M:%S"

last_time = None
time_diff = None
datetimes = []

for line in sys.stdin:
    line = line.rstrip()

    time_diff_str = "?"
    time_match = re.match(time_regex, line)
    time2_match = re.match(time2_regex, line)

    dt = None

    if time_match is not None:
        dt = datetime.strptime(time_match.group(1), time_format)

    if time2_match is not None:
        dt = datetime.strptime(time2_match.group(1), time2_format)

    if dt is not None:
        if last_time is not None:
            time_diff_str = f"{dt - last_time}"
            datetimes.append(dt - last_time)
        last_time = dt

    print(f"{time_diff_str:>16}: {line}")
try:
    average_timedelta_secs = sum([dt.total_seconds() for dt in datetimes]) / len(datetimes)
    td = timedelta(seconds=average_timedelta_secs)
except ZeroDivisionError:
    td = None
print(f"Average time diff: {td}")
