.. _disco:

Disco
###########

Overview
********

A simple sample that can be used on thingy91 to make the led lights flash, and the buzzer play "take on me".

Building and Running
********************

This application can be build and flashed by using VScode extention from nordic. 
.. Thingy must be connected to a dev kit

or

This application can be built and executed on QEMU as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/hello_world
   :host-os: unix
   :board: qemu_x86
   :goals: run
   :compact:

To build for another board, change "qemu_x86" above to that board's name.

