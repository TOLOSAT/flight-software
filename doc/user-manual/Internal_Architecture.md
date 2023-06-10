# TAPAS Internal Architecture Isolation

[Come back to first page](UserManual.md)

## Introduction

As mentioned above, TAPAS is the flight software that manages the avionic and payloads. It must function autonomously when not in visibility of the ground segment, but it must also be capable of being piloted remotely by an operator. 
TAPAS' main activities are as follows:
- Avionic management (thermal, scao, power and mass memory).
- Payload management.
- Telecommand and Telemeasure flow.
- Internal management.
Internal management covers all the activities that ensure the software's autonomy, performance and robustness.
## Specifications

This section contains the global specifications. The internal specifications for each activity will be specified in their respective sub-sections.

| Reference      | Name     | Rational | Description                                                               |
|----------------|----------|----------|---------------------------------------------------------------------------|
| T-TAPAS-001-00 | Autonomy | N/A      | TAPAS must be autonomous and function even in the absence of an operator. |

| Reference      | Name           | Rational | Description                                      |
|----------------|----------------|----------|--------------------------------------------------|
| T-TAPAS-002-00 | Remote control | N/A      | TAPAS can be controlled remotely by an operator. |

| Reference      | Name | Rational | Description                                                                      |
|----------------|------|----------|----------------------------------------------------------------------------------|
| T-TAPAS-003-00 | FDIR | N/A      | TAPAS must have Failure Detection, Identification and Recovery (FDIR) mechanisms |

| Reference      | Name                  | Rational | Description                                                                   |
|----------------|-----------------------|----------|-------------------------------------------------------------------------------|
| T-TAPAS-004-00 | Operator vs. Software | N/A      | The operator's decision always takes precedence over the software's decision. |

| Reference      | Name      | Rational | Description                        |
|----------------|-----------|----------|------------------------------------|
| T-TAPAS-005-00 | Safe Boot | N/A      | TAPAS must be guaranteed to start. |

| Reference      | Name        | Rational | Description                                                   |
|----------------|-------------|----------|---------------------------------------------------------------|
| T-TAPAS-006-00 | End of life | N/A      | The end-of-life of the satellite must be guaranteed by TAPAS. |

| Reference      | Name         | Rational       | Description                                                           |
|----------------|--------------|----------------|-----------------------------------------------------------------------|
| T-TAPAS-010-00 | Adaptability | T-TAPAS-001-00 | TAPAS must have several execution modes to handle different contexts. |

| Reference      | Name       | Rational                       | Description                                                                  |
|----------------|------------|--------------------------------|------------------------------------------------------------------------------|
| T-TAPAS-011-00 | Robustness | T-TAPAS-001-00, T-TAPAS-003-00 | If an error or a certain events occur, TAPAS must change its execution mode. |

| Reference      | Name                | Rational       | Description                                            |
|----------------|---------------------|----------------|--------------------------------------------------------|
| T-TAPAS-012-00 | Autonomous Avionic | T-TAPAS-001-00 | TAPAS must ensure that avionics operate automatically. |

| Reference      | Name               | Rational       | Description                                            |
|----------------|--------------------|----------------|--------------------------------------------------------|
| T-TAPAS-013-00 | Autonomous Payload | T-TAPAS-001-00 | TAPAS must ensure that payloads operate automatically. |

| Reference      | Name         | Rational       | Description                                                 |
|----------------|--------------|----------------|-------------------------------------------------------------|
| T-TAPAS-014-00 | Data Storage | T-TAPAS-001-00 | TAPAS must be able to save its data in non-volatile memory. |

| Reference      | Name         | Rational       | Description                                                                     |
|----------------|--------------|----------------|---------------------------------------------------------------------------------|
| T-TAPAS-015-00 | PUS Protocol | T-TAPAS-002-00 | Exchanges between TAPAS and the ground must respect PUS (cf. ECSS-E-ST-70-41C). |

| Reference      | Name       | Rational       | Description                                                                                                                         |
|----------------|------------|----------------|-------------------------------------------------------------------------------------------------------------------------------------|
| T-TAPAS-016-00 | TM/TC Flow | T-TAPAS-002-00 | TMs and TCs must be managed in a continuous flow using several tasks: a reception task, several execution tasks and a sending task. |

| Reference      | Name         | Rational       | Description                                                        |
|----------------|--------------|----------------|--------------------------------------------------------------------|
| T-TAPAS-017-00 | Scheduled TC | T-TAPAS-002-00 | TCs can be stored so that they can be executed at a specific time. |

| Reference      | Name       | Rational       | Description                                   |
|----------------|------------|----------------|-----------------------------------------------|
| T-TAPAS-018-00 | TM Storage | T-TAPAS-002-00 | TMs must be stored until they become visible. |

| Reference      | Name         | Rational       | Description                                                                                                                     |
|----------------|--------------|----------------|---------------------------------------------------------------------------------------------------------------------------------|
| T-TAPAS-019-00 | Housekeeping | T-TAPAS-002-00 | The satellite's observables must be brought down regularly in the form of TMs to report the satellite's status to the operator. |

| Reference      | Name                              | Rational       | Description                                                                                                          |
|----------------|-----------------------------------|----------------|----------------------------------------------------------------------------------------------------------------------|
| T-TAPAS-050-00 | Mode Management & Task Monitoring | T-TAPAS-010-00 | TAPAS must have a task that monitors the tasks and changes the satellite mode according to the state of these tasks. |

| Reference      | Name                         | Rational       | Description                                                                                              |
|----------------|------------------------------|----------------|----------------------------------------------------------------------------------------------------------|
| T-TAPAS-051-00 | Internal Software Monitoring | T-TAPAS-011-00 | TAPAS must have a task that constantly monitors the execution of the OS in order to detect errors in it. |

| Reference      | Name             | Rational       | Description                                                                    |
|----------------|------------------|----------------|--------------------------------------------------------------------------------|
| T-TAPAS-052-00 | Event Monitoring | T-TAPAS-011-00 | TAPAS must have a task that monitors the events taking place in the satellite. |

| Reference      | Name                                     | Rational       | Description                                                                                                                                              |
|----------------|------------------------------------------|----------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| T-TAPAS-053-00 | Internal Software and Event Misbehaviour | T-TAPAS-050-00 | The TAPAS tasks responsible for monitoring events and internal software must notify the life analysis and mode management partition of any misbehaviour. |

| Reference     | Name         | Rational       | Description                                                             |
|---------------|--------------|----------------|-------------------------------------------------------------------------|
| T-TAPAS-80-00 | AOCS Manager | T-TAPAS-012-00 | TAPAS must have a task that manage attitude and orbite controle system. |

| Reference     | Name          | Rational       | Description                                      |
|---------------|---------------|----------------|--------------------------------------------------|
| T-TAPAS-81-00 | Power Manager | T-TAPAS-012-00 | TAPAS must have a task that manage power system. |

| Reference     | Name            | Rational       | Description                                        |
|---------------|-----------------|----------------|----------------------------------------------------|
| T-TAPAS-82-00 | Thermal Manager | T-TAPAS-012-00 | TAPAS must have a task that manage thermal system. |

| Reference      | Name               | Rational       | Description                                            |
|----------------|--------------------|----------------|--------------------------------------------------------|
| T-TAPAS-100-00 | Gravimetry Manager | T-TAPAS-013-00 | TAPAS must have a task that manage Gravimetry Payload. |

| Reference      | Name            | Rational       | Description                                  |
|----------------|-----------------|----------------|----------------------------------------------|
| T-TAPAS-101-00 | Iridium Manager | T-TAPAS-013-00 | TAPAS must have a task that Iridium Payload. |


## Description

As mentioned in the introduction, TAPAS has several activities to carry out: avionic management, payload management, telecommand and telemeasure flow, internal management. In order to carry out these tasks, TAPAS relies on tasks. Each activity is made up of one or more tasks. The tasks interact with each other, in particular via buffers that store messages until they are read by the next task.

The internal management of TAPAS is based on a triad:
- SALAMI (SAtellite Life Analysis & Mode Integration), whose role is to control task execution (life analysis) and manage the satellite's modes. 
- MISO (Monitoring & Internal Software Analysis), whose role is to monitor the OS.
- CARNE (Common Automated Recording of New Events), whose role is to record all satellite events.

CARNE and MISO can notify SALAMI of a problem so that it can change the satellite's mode or initiate a restart. These tasks communicate with the rest of TAPAS using buffers. Life buffers are linked to SALAMI, while event buffers are linked to CARNE.

Avionic management is based on three tasks:
- AOCS Manager, which is responsible for AOCS.
- Power Manager, which is responsible for power management.
- Thermal Manager, which is responsible for thermal management.

Payload management is based on two tasks:
- Gravimetry Manager, which is responsible for Gravimetry payload.
- Iridium Manager, which is responsible for Iridium payload.

Telecommand (TC) and Telemeasure (TM) Flow is based on a large number of tasks:
- TC Receiver, which is responsible for receiving TCs, checking their validity, generating an acknowledgement and then sending the TC to downstream tasks.
- TC Process, which is responsible for executing all TCs that do not have a specific task.
- TC Scheduler, which is responsible for receiving TCs that are to be executed at a given time.
- TM Sender, which is responsible for retrieving TMs, packaging them in the correct format for the transmitter and then storing them until the satellite is within sight of the ground station.
- Housekeeping Manager, who is responsible for receiving the observables from the satellite and generating the housekeeping TMs.
- And finally, all the other tasks that can receive their own TCs and transmit their own TMs.

Between each of these tasks there are buffers to store the CTs and TMs while waiting for the next task to retrieve them.

We can summarise the operation of the internal software with the following graph, which shows all the tasks and the buffers that link them.

<img src="../images/Internal_Architecture_Graph.png" width=100% />
