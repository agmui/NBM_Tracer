# NBM_Tracer
~~NotBitcoinMiner(NBM)~~ Raytracer

Simple Networked Raytracer that distributes raytracing
over many computers using TCP.
Built for RHIT Networks class project.

![png](demo.png)

> NOTE: only works for linux for now....

## Client

To run the client download the latest [binary](https://github.com/agmui/NBM_Tracer/releases/download/v1.0.0/NBM_Tracer_Client)

```bash
chmod +x ./NBM_Tracer_Client
./NBM_Tracer_Client <ip> <port>
```

### Arguments:
*  `<ip>`: ip of server
*  `<port>`: port number to join server on
 
## Server

To run the client download the latest [binary](https://github.com/agmui/NBM_Tracer/releases/download/v1.0.0/NBM_Tracer_Server)

```bash
chmod +x ./NBM_Tracer_Server
./NBM_Tracer_Server <port> <RES> <.obj> <.mtl> </tmp/.mtl>  
```
### Arguments:
*  `<port>`: port number for clients to join on
*  `<RES>`: square resolution to render at
* `<.obj>`: file to render  
* `<.mtl>`: file paired with the obj file  
* `</tmp/.mtl>`: location on where obj will be stored on client side  

## Building

run cmake

```bash
git clone ...
mkdir build
cd build
cmake ..
make
```
