# IOSpeedRun

Operating System DIY HOMEWORK 1.

This repository explored speedup from eBPF and evaluate different IO speed.

For details, see my [Study Memo](StudyMemo.md).

## io_speed_test

By `TEST_SIZE=100, BATCH_SIZE=16, BUFFER_SIZE=4096` we get the following result:

| stdio    | io_uring  | linux_aio |
| -------- | --------- | --------- |
| 42296.74 | 32,192.77 | 22,732.65 |

We can see `io_uring` accelerate I/O spee, but not as good as linux_aio. That is within expectations since the major contrubute of `io_uring` is not speed but its scalability.

## socket_speedup

We use `eBPF` to speedup socket within localhost.

| Without eBPF | With eBPF  |
| ------------ | ---------- |
| 18.636279    | 16.0347025 |

