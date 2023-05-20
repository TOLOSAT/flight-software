# TAPAS Core
The core of TAPAS consists of the application part of the flight software. We distinguish the main from the tasks. The main part contains all the codes related to the flight software setup and to the basic functionality of TAPAS. On its side, the tasks are the real application elements of the flight software: we find there the tasks of management of the software (mode management, error management, ...) the tasks of management of the avionic and the tasks of management of the payloads.

<div style="text-align:center"><img src="../doc/images/Core_Organization_Graph.png" width=50% /></div>

## Main
### Contents

The main folder contains the software main, the initialization functions, the interrupts and the task and buffer management functions.
The role of main is to initialize the HALs and the OS and then to launch the scheduler.
The functions of management of the tasks make it possible in particular to create, suspend and resume the tasks.
This folder also contains configuration files on which the management of tasks and buffers is based.

### Adding Tasks

To add a task you simply need to modify the tasks_conf.h and tasks_conf.c configuration files.
In the tasks_conf.h file you need to add a task in the TASKS_ENUM enum. This number will be the reference of the task from now on: we can use it for all the task management.
In the tasks_conf.c file, add the configuration of this task to the configuration table g_tasks_conf. You must then specify in order: the reference of the task (from TASKS_ENUM), its handler (function for executing the task), the argument of the handler (as a pointer) and the attributes of the task. For the attributes, we only indicate the priority of the task and the size of the stack.

### Adding Buffer

To add a buffer you simply need to modify the buffers_conf.h and buffers_conf.c configuration files.
In the buffers_conf.h file you have to add a buffer in the BUFFERS_ENUM enum. This number will be the reference of the buffer from now on: we can use it for all buffer management.
In the buffers_conf.c file, you must add the configuration of this task to the g_buffers_conf configuration table. It is then necessary to specify in order: the reference of the buffer (from BUFFERS_ENUM), the maximum size of a message, the maximum number of messages and the attributes of the buffer. Concerning the attributes we leave a null pointer and the bone will automatically set the attributes.

## Tasks 

At the moment there are no spots that have been created for TAPAS. The present tasks are dummy tasks which are only used for the validation of the tools.

