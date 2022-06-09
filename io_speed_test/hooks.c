#include <uapi/linux/ptrace.h>

BPF_HASH(bpf_stdio_enter_map);

int bpf_stdio_enter(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0, size;
    u64 * size_ptr = bpf_stdio_enter_map.lookup(&size_key);
    size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_stdio_enter_map.update(&size_key, &size);
    bpf_stdio_enter_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_stdio_leave_map);

int bpf_stdio_leave(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0;
    u64 * size_ptr = bpf_stdio_leave_map.lookup(&size_key);
    u64 size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_stdio_leave_map.update(&size_key, &size);
    bpf_stdio_leave_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_iouring_submit_enter_map);

int bpf_iouring_submit_enter(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0, size;
    u64 * size_ptr = bpf_iouring_submit_enter_map.lookup(&size_key);
    size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_iouring_submit_enter_map.update(&size_key, &size);
    bpf_iouring_submit_enter_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_iouring_submit_leave_map);

int bpf_iouring_submit_leave(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0;
    u64 * size_ptr = bpf_iouring_submit_leave_map.lookup(&size_key);
    u64 size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_iouring_submit_leave_map.update(&size_key, &size);
    bpf_iouring_submit_leave_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_iouring_peek_enter_map);

int bpf_iouring_peek_enter(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0, size;
    u64 * size_ptr = bpf_iouring_peek_enter_map.lookup(&size_key);
    size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_iouring_peek_enter_map.update(&size_key, &size);
    bpf_iouring_peek_enter_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_iouring_peek_leave_map);

int bpf_iouring_peek_leave(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0;
    u64 * size_ptr = bpf_iouring_peek_leave_map.lookup(&size_key);
    u64 size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_iouring_peek_leave_map.update(&size_key, &size);
    bpf_iouring_peek_leave_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_aio_submit_enter_map);

int bpf_aio_submit_enter(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0, size;
    u64 * size_ptr = bpf_aio_submit_enter_map.lookup(&size_key);
    size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_aio_submit_enter_map.update(&size_key, &size);
    bpf_aio_submit_enter_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_aio_submit_leave_map);

int bpf_aio_submit_leave(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0;
    u64 * size_ptr = bpf_aio_submit_leave_map.lookup(&size_key);
    u64 size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_aio_submit_leave_map.update(&size_key, &size);
    bpf_aio_submit_leave_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_aio_peek_enter_map);

int bpf_aio_peek_enter(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0, size;
    u64 * size_ptr = bpf_aio_peek_enter_map.lookup(&size_key);
    size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_aio_peek_enter_map.update(&size_key, &size);
    bpf_aio_peek_enter_map.insert(&size, &time);
    return 0;
}

BPF_HASH(bpf_aio_peek_leave_map);

int bpf_aio_peek_leave(struct pt_regs * ctx) {
    u64 time = bpf_ktime_get_ns(), size_key = 0;
    u64 * size_ptr = bpf_aio_peek_leave_map.lookup(&size_key);
    u64 size = size_ptr == NULL ? 1 : * size_ptr + 1;
    bpf_aio_peek_leave_map.update(&size_key, &size);
    bpf_aio_peek_leave_map.insert(&size, &time);
    return 0;
}