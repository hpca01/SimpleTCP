## Simple TCP server
As title states, this is a simple TCP server written in C.



**To start project use the Makefile**

```
make
```
Then
```
./server
```
You will see
```
~~~~~~~~~Waiting to accept a new conn~~~~~~~~
```
After which you can create a seperate terminal and invoke the client:
```
./client
```
**OR** navigate on your browser to:
```
localhost:8080/<name of file in files directory>
```

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
- [x] Turn it into a multi-threaded file-server! -- Only works w/ jpeg, jpg, html, pdf files for now...
- [x] Scaffold a memory map call
- [ ] Read all files being served into memory using MMAP
- [ ] Organize into a struct for easy querying and returning.
- [ ] Write out client to do a load test
- [ ] Valgrind Test on server after client is written
