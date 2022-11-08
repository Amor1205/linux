*Von Neumann Architecture*:
*-
1.input device : keyboard, disk, network card, graphics card, camera, microphone,
2.output device : monitor, disk, network card, graphics card, audio,
3.memory : The farther away from CPU, the less efficient and the lower cost.
 register -> cache -> main memory -> local disk (local secondary storage) -> remote secondary storage 
4.calculators & contollers : CPU (has computing ability)


if we take the structure of " extrlnal device & CPU ", because of the barrel effect, computer efficiency will be very low.
So there is memory:  With memory, the CPU is linked to memory instead of extrlnal device.
---------> memory is the core device of the architecture. 
(input + output = IO) 
input = input to memory from disk , output = output to disk from memory. (local IO)
input = input to memory from network card , output = output to network card from memory. (network IO)
-*

*-
Operator System:
"The booted operating system makes snese." In other words: It only makes sense to load the operating system into memory (OS is stored in disk).

Two questions to ask:

----------------------------------------
What is OS?
a software:  A software dedicated to the management of hardware & software resources.

Why is OS?
--for the lower layer: manage software & hardware resources;
--for the upper layer: provide users with a stable, efficient and safe operating environment.
-*


OS cannot be linked to hardware like disks or memory:
 users ----> OS ---->  driver ----> hardware

*describe first, organize later*
How does the operating system manage processes?
still : describe first, organize later.
there is a struct that describes the process(PCB - process control block)

