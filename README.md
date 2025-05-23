# NBM_Tracer
~~NotBitcoinMiner(NBM)~~ Raytracer

Simple Networked Raytracer that distributes raytracing
over many computers using TCP.

![png](demo.png)

> NOTE: only works for linux for now....

## Client

To run the client download the latest binary:

```bash
chmod +x ./NBM_Tracer_Client
./NBM_Tracer_Client <ip> <port>
```

## Server

```bash
chmod +x ./NBM_Tracer_Server
```

## Building

run cmake

```bash
git clone ...
mkdir build
cd build
cmake ..
make
```
