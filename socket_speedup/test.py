import os
import sys
import random
import string
from timeit import default_timer as timer

def PrintState(text):
    def decorator(func):
        def wrapper(*args, **kwarg):
            text_dot = text + '...'
            text_end = text + ' finished'
            print(f'--------------- {text_dot:^45} ---------------')
            t = timer()
            r = func(*args, **kwarg)
            t_min = timer() - t
            text_end += (f' in {t_min:.2f} seconds.')
            print(f'--------------- {text_end:^45} ---------------')
            return r
        return wrapper
    return decorator

def ex(cmd):
    os.system(cmd)

TEST_SIZE = 10000
CLIENT_RECEIVED_FILE_NAME = 'client_received.txt'
CLIENT_SENT_FILE_NAME = 'client_sent.txt'

def random_string(len):
    return ''.join(random.choice(string.ascii_letters) for _ in range(len))

def test_socket(msg):
    for _ in range(TEST_SIZE):
        s = random_string(10)
        #ex(f'echo "{s}"')# >> {msg}_{CLIENT_SENT_FILE_NAME}')
        ex(f'./client {s}')# >> {msg}_{CLIENT_RECEIVED_FILE_NAME}')

@PrintState('Testing with eBPF')
def test_with_ebpf():
    test_socket('with_ebpf')

@PrintState('Testing without eBPF')
def test_without_ebpf():
    test_socket('without_ebpf')    

if __name__ == '__main__':
    if sys.argv[1] == 'eBPF':
        ex('sudo sh load.sh')
        test_with_ebpf()
        ex('sudo sh unload.sh')
    else:
        test_without_ebpf()