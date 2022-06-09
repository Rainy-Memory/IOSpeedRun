参考：

[https://ebpf.io](https://ebpf.io/)

eBPF全称为*Extended Berkeley Packet Filter，*是一种将用户态的程序直接嵌入内核执行的手段。由于内核态的默认可以访问所有的对象，因此嵌入用户态程序需要慎之又慎。eBPF提供了一套验证的机制使得程序可以被验证其安全性。io_uring是一种Linux下的异步IO模块，旨在提供一种更加高效的IO处理接口。

在本DIY大作业中，你需要对eBPF和io_uring有一定的了解，任务目标如下：

1. 了解eBPF的验证机制，熟悉代码嵌入内核之后的操作；

2. 了解异步IO的主要方式，并掌握使用io_uring；

3. 具有自主设计实验比对多种实现方式的性能差异。

这个任务最终的呈现目标为展示eBPF带来的性能提升以及io_uring带来的吞吐量提升。