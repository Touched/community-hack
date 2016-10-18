import sys

with open(sys.argv[1], 'r') as f:
    contents = f.read()

contents = contents.replace(' __attribute__((visibility("hidden")))', '')

with open(sys.argv[1], 'w') as f:
    f.write(contents)