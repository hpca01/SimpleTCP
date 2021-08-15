FROM gcc:latest
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN make
CMD ["./server"]
EXPOSE 8080