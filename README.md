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

rtn -> Hello Stranger!
```

If you're curious about headers in a standard GET request, you can  browse to `localhost:8080/index.html`, and you'll see the same message returned(not done yet).

When you're done and want to clean up the artifacts:
```
make clean
```

- [x] Write simple TCP server
- [x] Write client
- [x] Parse Headers
- [x] Assemble return with correct HTTP code.
- [x] Add multi-threading, currently using multi-processing.
- [ ] Turn it into a multi-threaded file-server! :D
