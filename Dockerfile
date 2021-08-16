FROM gcc:latest
COPY . /usr/src/myapp
LABEL Name=vsc_cbuild
RUN apt-get -y update
RUN apt-get -y install gdb gdbserver
WORKDIR /usr/src/myapp
RUN make
CMD ["./server"]
EXPOSE 8080