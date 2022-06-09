# for tutorials, see https://github.com/iovisor/bcc/blob/master/docs/tutorial_bcc_python_developer.md

from bcc import BPF, USDT
import sys

def eval(enter_map, leave_map):
    if len(enter_map) != len(leave_map):
        print('unmatched enter/leave map!')
        exit(1)
    l = len(enter_map)
    # tot = 0
    le = [i.value for i in enter_map.values()]
    ll = [i.value for i in leave_map.values()]
    elapse = [ll[i] - le[i] for i in range(l)]
    tot = sum(elapse)
    # for i in range(l):
    #     tot += ll[i] - le[i]
    return tot / l

def main():
    if len(sys.argv) < 2:
        print(f'Usage: {sys.argv[0]} <pid>')
        exit(1)

    u = USDT(pid=int(sys.argv[1]))
    u.enable_probe(probe='stdio_enter', fn_name='bpf_stdio_enter')
    u.enable_probe(probe='stdio_leave', fn_name='bpf_stdio_leave')

    with open('hooks.c', 'r') as f:
        b = BPF(text=f.read(), usdt_contexts=[u])
    
    b.attach_uprobe(name='uring', sym='io_uring_submit', fn_name='bpf_iouring_submit_enter')
    b.attach_uretprobe(name='uring', sym='io_uring_submit', fn_name='bpf_iouring_submit_leave')
    b.attach_uprobe(name='uring', sym='io_uring_peek_batch_cqe', fn_name='bpf_iouring_peek_enter')
    b.attach_uretprobe(name='uring', sym='io_uring_peek_batch_cqe', fn_name='bpf_iouring_peek_leave')
    b.attach_uprobe(name="aio", sym="io_submit", fn_name="bpf_aio_submit_enter")
    b.attach_uretprobe(name="aio", sym="io_submit", fn_name="bpf_aio_submit_leave")
    b.attach_uprobe(name="aio", sym="io_getevents", fn_name="bpf_aio_peek_enter")
    b.attach_uretprobe(name="aio", sym="io_getevents", fn_name="bpf_aio_peek_leave")

    try:
        while True:
            pass
    except KeyboardInterrupt:
        print()

    stdio_elapse = eval(b['bpf_stdio_enter_map'], b['bpf_stdio_leave_map'])
    iouring_submit_elapse = eval(b['bpf_iouring_submit_enter_map'], b['bpf_iouring_submit_leave_map'])
    iouring_peek_elapse = eval(b['bpf_iouring_peek_enter_map'], b['bpf_iouring_peek_leave_map'])
    aio_submit_elapse = eval(b['bpf_aio_submit_enter_map'], b['bpf_aio_submit_leave_map'])
    aio_peek_elapse = eval(b['bpf_aio_peek_enter_map'], b['bpf_aio_peek_leave_map'])

    print('-' * 91)
    print(f"| {'stdio':^15} | {'iouring_submit':^15} | {'iouring_peek':^15} | {'aio_submit':^15} | {'aio_peek':^15} |")
    print(f"| {stdio_elapse:^15.2f} | {iouring_submit_elapse:^15.2f} | {iouring_peek_elapse:^15.2f} | {aio_submit_elapse:^15.2f} | {aio_peek_elapse:^15.2f} |")
    print('-' * 91)

if __name__ == '__main__':
    main()