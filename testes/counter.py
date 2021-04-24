import sys
import time
import os

print(os.getpid())
args = sys.argv

if len(args) == 1:
    print("You forgot to input an argument!")
    exit(1)

time.sleep(10)
value = int(args[1])

for i in range(value):
    print(f"I am iteration #{i+1}")
    time.sleep(2)
    