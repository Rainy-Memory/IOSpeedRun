# StudyMemo



## Environment Configuration

### BCC

From totorial [here](https://github.com/iovisor/bcc/blob/master/INSTALL.md#ubuntu---source). Default source code failed on my machine, but issue [here](https://github.com/iovisor/bcc/blob/master/INSTALL.md#ubuntu---source) solved my problem. Notice that in release [0.24.0](https://github.com/iovisor/bcc/releases/tag/v0.24.0) we need to download `bcc-src-with-submodule`.

Then enable USDT by

```shell
$ sudo apt-get install systemtap-sdt-dev
```

### IO Library

Notice to replace `aarch64` with your architecture. In most cases it will be `x86_64`.

```shell
$ sudo apt-get install libaio-dev
$ git clone https://github.com/axboe/liburing.git
$ cd liburing && make && make all
$ sudo cp -r ../liburing /usr/share
$ sudo cp src/liburing.so.2.2 /usr/lib/aarch64-linux-gnu/liburing.so
$ sudo cp src/liburing.so.2.2 /usr/lib/aarch64-linux-gnu/liburing.so.2
```



## Intuitions

### What is eBPF

eBPF (extended Berkely Packet Filter), is a method to execute program originally in userspase directly in kernel. Considering security, it is not hard to see that this method have some restriction to program. In specific, eBPF have a `verifier` to verify whether the code is safe to run in kernel.

In this project, we use eBPF to:

1. Time different kinds of I/O (a timer of syscalls)
2. Accelerate socket message exchange in localhost

### What is BCC

BCC is a toolkit for eBPF. In this project, we use BCC to register some probe function for syscall in kernel.

A probe function is like a python `@` wrapper, will be called before / after some specified syscall, and context of syscall is available for read, so we could use it to time and other useful functions. A sample could be found [here](https://github.com/iovisor/bcc/blob/master/examples/hello_world.py). To use BCC toolkit, c code and python code have some format restrictions, if we write c code in the format of BCC, our code is guaranteed to pass the `verifier` of eBPF and thus could run in kernel. To study these format, see [here](https://github.com/iovisor/bcc/blob/master/docs/tutorial_bcc_python_developer.md). (`uprobe` is triggered before function and `uretprobe` is triggered before function return.)

### What is io_uring

io_uring is an asynchronous library which support all kinds of I/O, unlike linux aio could only handle `O_DIRECT` files. By implementing two shared (by kernel and user) data structure `Submission Queue (SQ)` and `Completion Queue (CQ)`, one program using io_uring could fetch events complete status corresponding submission without step into kernel. In the same time, by design, io_uring will never block and is flexible and extensible (I haven't figure out why yet), which allows way more programs using asynchronous I/O (linux aio only support database applications which use `O_DIRECT` files).



## Tips

Probe function only activate when python script is running in another session.

Linux-aio only work for `O_DIRECT` files.

(In bpf c code) judge `ptr == NULL` before using it. This is an implementation for `map.containsKey()`.



## Blogs

[Brief introduction to eBPF and io_uring](https://thenewstack.io/how-io_uring-and-ebpf-will-revolutionize-programming-in-linux)

[An intro to USDT](https://blog.csdn.net/Longyu_wlz/article/details/109902171)

[Another intro to bcc](https://zhuanlan.zhihu.com/p/523964499)

[Linux Native aio](https://cloud.tencent.com/developer/article/1810604)

[using eBPF to accelerate socket](https://arthurchiao.art/blog/socket-acceleration-with-ebpf-zh/)

