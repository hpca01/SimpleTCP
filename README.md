## Simple TCP server
As title states, this is a simple TCP server written in C. A good weekend project.

Next is to move on up to HTTP(basically a TCP with "formatted" returns)

**To start project use the Makefile**

```
make
```

You should see warnings about unsed parameters, ignore those. They're the standard `argc` and `argv` that are in `main` functions.

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

If you're curious about headers in a standard GET request, you can  browse to `localhost:8080/index.html`, and you'll see the same message returned.

When you're done and want to clean up the artifacts:
```
make clean
```
