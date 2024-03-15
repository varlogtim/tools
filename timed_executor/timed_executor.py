import argparse
import subprocess
import time
from multiprocessing.pool import ThreadPool


def time_command(index: int, cmd: str) -> None:
    start_time = time.time()
    cmd_sub = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    cmd_sub.communicate()
    delta = time.time() - start_time
    print(f"proc[{index}] returned: {cmd_sub.returncode} in {delta:.8f} seconds")

def main(cmd: str, num: int) -> None:
    tp = ThreadPool(num)
    for ii in range(num):
        tp.apply_async(time_command, (ii, cmd))

    tp.close()
    tp.join()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="timed executor",
        description="execute a command multiple times and time the output",
        epilog="fin"
    )
    parser.add_argument(
        "-n", "--num", required=True, type=int, help="the number of instances to run"
    )
    parser.add_argument(
        "-c", "--command", required=True, help="the command to run"
    )
    args = parser.parse_args()

    main(args.command, args.num)
