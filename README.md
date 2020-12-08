## Simple TCP server
As title states, this is a simple TCP server written in C.



**To start project use the Makefile**

```
make
```


You will see
```
./server

~~~~~~~~~Waiting to accept a new conn~~~~~~~~

```
After which you can create a seperate terminal and invoke the client:
```
./client

```
The output is a static HTML page w/ client's request included.

When you're done and want to clean up the artifacts:
```
make clean
```

To-Do:

- [x] Write simple TCP server
- [x] Write client
- [x] Parse Headers
- [x] Assemble return with correct HTTP code.
- [x] Add multi-threading, currently using multi-processing.
- [ ] Turn it into a multi-threaded file-server!
