# Telemetry and Telecommand Flow

[Come back to first page](Technical_Specifications.md)

## Introduction

What we call Telemetry and Telecommand Flow is a set of tasks enabling the satellite to communicate with the ground. First, we define telecommands and telemetry:
- TeleCommands (TC) are messages sent from the ground to the satellite. The purpose of these messages is to control the satellite or acquire data.
- TeleMetry (TM) are messages sent from the satellite to the ground. These messages contain information about the satellite and its mission. Some are responses to telecomands.

These telecommands and telemetry devices comply with the ECSS Protocol Utilization Standard (PUS). The main contributions of this standard (and of the space packet protocol from which it derives) are :
- TMs and TCs specify the application ID (APID) to which the message is addressed.
- TMs and TCs have an identification number which enables them to be traced.
- TMs and TCs are grouped into services and sub-services which allow actions of the same type to be grouped together.
- TMs are time-stamped.

For more information on the PUS, please refer to the ECSS document ECSS-E-ST-70-41C (available free of charge on the Internet).

The satellite must therefore be able to :
1. Receive telecommands.
2. Interpret and execute these telecommands.
3. Build and transmit telemetry.
4. Receive telecommands to be executed at a specific time.
5. Store telemetry while waiting to be in vision of the ground station.

## Specifications

| Reference      | Name                   | Rational       | Description                                                                          |
|----------------|------------------------|----------------|--------------------------------------------------------------------------------------|
| T-TAPAS-400-00 | ACK management service | T-TAPAS-025-00 | The satellite must be able to provide an acknowledgement management service (PUS 1). |

| Reference      | Name                            | Rational       | Description                                                                      |
|----------------|---------------------------------|----------------|----------------------------------------------------------------------------------|
| T-TAPAS-401-00 | Housekeeping management service | T-TAPAS-025-00 | The satellite must be able to provide a housekeeping management service (PUS 3). |

| Reference      | Name                      | Rational       | Description                                                                 |
|----------------|---------------------------|----------------|-----------------------------------------------------------------------------|
| T-TAPAS-402-00 | Events management service | T-TAPAS-025-00 | The satellite must be able to provide an events management service (PUS 5). |

| Reference      | Name                      | Rational       | Description                                                                |
|----------------|---------------------------|----------------|----------------------------------------------------------------------------|
| T-TAPAS-403-00 | Memory management service | T-TAPAS-025-00 | The satellite must be able to provide a memory management service (PUS 6). |

| Reference      | Name                    | Rational       | Description                                                              |
|----------------|-------------------------|----------------|--------------------------------------------------------------------------|
| T-TAPAS-404-00 | Time management service | T-TAPAS-025-00 | The satellite must be able to provide a time management service (PUS 9). |

| Reference      | Name                        | Rational       | Description                                                                   |
|----------------|-----------------------------|----------------|-------------------------------------------------------------------------------|
| T-TAPAS-405-00 | Schedule management service | T-TAPAS-025-00 | The satellite must be able to provide a schedule management service (PUS 11). |

| Reference      | Name                                  | Rational       | Description                                                                              |
|----------------|---------------------------------------|----------------|------------------------------------------------------------------------------------------|
| T-TAPAS-406-00 | On-board retrieval management service | T-TAPAS-025-00 | The satellite must be able to provide an on-board retrieval management service (PUS 15). |

| Reference      | Name                    | Rational       | Description                                                               |
|----------------|-------------------------|----------------|---------------------------------------------------------------------------|
| T-TAPAS-407-00 | Test management service | T-TAPAS-025-00 | The satellite must be able to provide a test management service (PUS 17). |

| Reference      | Name                      | Rational       | Description                                                      |
|----------------|---------------------------|----------------|------------------------------------------------------------------|
| T-TAPAS-408-00 | SALAMI management service | T-TAPAS-025-00 | The satellite must be able to provide SALAMI management service. |

| Reference      | Name                    | Rational       | Description                                                    |
|----------------|-------------------------|----------------|----------------------------------------------------------------|
| T-TAPAS-409-00 | MISO management service | T-TAPAS-025-00 | The satellite must be able to provide MISO management service. |

| Reference      | Name                    | Rational       | Description                                                    |
|----------------|-------------------------|----------------|----------------------------------------------------------------|
| T-TAPAS-410-00 | AOCS management service | T-TAPAS-025-00 | The satellite must be able to provide AOCS management service. |

| Reference      | Name                     | Rational       | Description                                                     |
|----------------|--------------------------|----------------|-----------------------------------------------------------------|
| T-TAPAS-411-00 | Power management service | T-TAPAS-025-00 | The satellite must be able to provide Power management service. |

| Reference      | Name                       | Rational       | Description                                                       |
|----------------|----------------------------|----------------|-------------------------------------------------------------------|
| T-TAPAS-412-00 | Thermal management service | T-TAPAS-025-00 | The satellite must be able to provide Thermal management service. |

| Reference      | Name                          | Rational       | Description                                                          |
|----------------|-------------------------------|----------------|----------------------------------------------------------------------|
| T-TAPAS-413-00 | Gravimetry management service | T-TAPAS-025-00 | The satellite must be able to provide Gravimetry management service. |

| Reference      | Name                       | Rational       | Description                                                       |
|----------------|----------------------------|----------------|-------------------------------------------------------------------|
| T-TAPAS-414-00 | Iridium management service | T-TAPAS-025-00 | The satellite must be able to provide Iridium management service. |

| Reference      | Name               | Rational       | Description                                                                                                  |
|----------------|--------------------|----------------|--------------------------------------------------------------------------------------------------------------|
| T-TAPAS-415-00 | TC Acknowledgement | T-TAPAS-025-00 | All TCs must be aknowledged for proper reception and execution (i.e. TM(1,1), TM(1,2), TM(1,7) and TM(1,8)). |

| Reference      | Name        | Rational       | Description                                                                                                                     |
|----------------|-------------|----------------|---------------------------------------------------------------------------------------------------------------------------------|
| T-TAPAS-420-00 | TC Receiver | T-TAPAS-026-00 | The TC receiver must check the validity of the TC, then acknowledge receipt and route the TC to the task which will execute it. |

| Reference      | Name      | Rational       | Description                                                                                                                                                                                |
|----------------|-----------|----------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| T-TAPAS-421-00 | TM Sender | T-TAPAS-026-00 | The TM Sender is responsible for collecting all the TMs, putting them into frames, storing them in packet stores and then lowering them to the ground when the ground station is in sight. |

| Reference      | Name         | Rational       | Description                                                                                                                                                                   |
|----------------|--------------|----------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| T-TAPAS-422-00 | TC Scheduler | T-TAPAS-026-00 | The TC Scheduler is responsible for retrieving the TCs that need to be executed at a specific time, storing them in a scheduler and then releasing them at the required time. |

## Description

In order to successfully receive and execute remote commands and then transmit telemetry, it is necessary to have several tasks that deal with these different activities. For this reason, we have created the following tasks in order to make the processing more fluid. So we have :
- TC Receiver, which is responsible for receiving the telecommands, checking their validity, producing the acknowledgement or non-acknowledgement of correct reception (i.e. TM(1,1) or TM(1,2)), then routing the telecommands to the activity responsible for processing them according to the APID, its service and sub-service.
- TM Sender, which is responsible for retrieving all the telemetry from the satellite, putting it in the form of a "transfer frame" (fixed size of 1115 bytes) then encoded in Reed-Solomon format before being stored in non-volatile memory in the form of a packet store (PS). It is also responsible for retrieving these packet stores and returning them to the ground when the ground station is in visibility.
- TC Scheduler, which is responsible for storing the programmed TCs and then resending them at the requested time.
- Most tasks are able to receive a TC, execute it and send TMs.

The operation of the TM/TC flow can therefore be summarised as follows:

<img src="../images/TMTC_Flow_Graph.png" width=100% />

### TC Receiver

As explained above, this task must be carried out in the following order:
1. Check for the presence of TCs, and recover them if there are any.
2. Check the validity of the TC.
3. If the TC is valid, send an acknowledgement, otherwise send a non-acknowledgement.
4. Send the TC to the task responsible for processing it. To do this, the task will use a routing table based on a routing key. The routing key is made up of the APID, the service and the sub-service.

This task must be executed periodically with a relatively high frequency and a high priority to ensure that no TCs are lost, even if the DMA uses a buffer to store a few TCs in advance.

### TM Sender

As explained above, this task must be carried out in the following order:
- If the satellite is not in transmission mode :
    1. Retrieve all TMs from its buffers;
    2. Put them in the form of encoded frames using the Reed-Solomon algorithm.
    3. Store them in NVM blind packets.
- If the satellite is in transmit mode :
    1. Retrieve all TMs from its buffers;
    2. Encode them into frames using the Reed-Solomon algorithm.
    3. Send them down to the ground.
    4. Empty the NVM store packets towards the ground if the satellite has been instructed to do so.

### TC Scheduler

As explained above, this task must be carried out in the following order :
1. Retrieve the TCs from service 11. Unpack them, retrieve the TC inside and the time at which it was executed.
2. Store the TCs in the NVME in the form of a linked list, which we will call schedule according to their execution time. We will acquire the execution of the TC for service 11 at this time.
3. Scan the TCs in the schedule in NVM. Recover the TCs whose execution time is greater than or equal to the current time.
4. Send the available TCs to the TM sender.

### TM/TC Process & Specific Task

Some tasks can perform TCs, in which case the task must :
1. Check the presence of TCs at the start of the period.
2. Process the TC.
3. Send an execution acknowledgement or non-acknowledgement TM after executing the TC.
4. Send a TM if required by the remote control.

Otherwise tasks can send telemetry without any TC being received, the principle remains the same these TM are sent to the TM Sender.

The TM/TC process task is there to execute all the TCs that do not have a specific task to execute them (like services 6, 9 and 17).

## List of PUS Services & Subservices Used

Here is a table showing all the PUS services used on TAPAS :

| Service | Name                           |
|:-------:|--------------------------------|
|    1    | Request Verification           |
|    3    | Housekeeping                   |
|    5    | Event Reporting                |
|    6    | Memory management              |
|    9    | Time Management                |
|   11    | Time Based Scheduling          |
|   15    | On-Board Storage and Retrieval |
|   17    | Test                           |
|   160   | SALAMI PUS Service             |
|   161   | MISO PUS Service               |
|   176   | AOCS PUS Service               |
|   177   | Power PUS Service              |
|   178   | Thermal PUS Service            |
|   192   | Gravimetry PUS Service         |
|   193   | Iridium PUS Service            |

Note: services above 128 are mission-specific services according to the PUS standard.

Here is a table showing all telemetries and telecommands used on TAPAS :

| Service | Subservice | TM/TC | Purpose                                                      |
|:-------:|:----------:|:-----:|--------------------------------------------------------------|
|    1    |     1      |  TM   | Successful Acceptance                                        |
|    1    |     2      |  TM   | Failed Acceptance                                            |
|    1    |     7      |  TM   | Successful Execution                                         |
|    1    |     8      |  TM   | Failed Execution                                             |
|    3    |     5      |  TC   | Enable Housekeeping                                          |
|    3    |     6      |  TC   | Disable Housekeeping                                         |
|    3    |     25     |  TM   | Housekeeping Parameter Report                                |
|    5    |     1      |  TM   | Informative Event Report                                     |
|    5    |     2      |  TM   | Low Severity Event Report                                    |
|    5    |     3      |  TM   | Medium Severity Event Report                                 |
|    5    |     4      |  TM   | High Severity Event Report                                   |
|    6    |     1      |  TC   | Load Object Memory Data                                      |
|    6    |     3      |  TC   | Dump Object Memory Data                                      |
|    6    |     4      |  TM   | Dumped Object Memory Data Report                             |
|    9    |     2      |  TM   | CUC time report                                              |
|    9    |    129     |  TC   | Set On-Board Time                                            |
|   11    |     1      |  TC   | Enable Time-Based Schedule                                   |
|   11    |     2      |  TC   | Disable Time-Based Schedule                                  |
|   11    |     3      |  TC   | Reset Time-Based Schedule                                    |
|   11    |     4      |  TC   | Add Activity to the Time-Based Schedule                      |
|   15    |     9      |  TC   | Start the by-time-range retrieval of packet stores           |
|   15    |     11     |  TC   | Delete the content of packet stores up to the specified time |
|   17    |     1      |  TC   | Connection Test (Ping)                                       |
|   17    |     2      |  TM   | Connection Test Answer (Pong)                                |

## Failure Management

The following two tables list the types and subtypes encountered by TM/TC management tasks :

| Number | Error Type | Description      |
|--------|------------|------------------|
| 0      | No Error   | No error occured |
| 1      | ...        | ...              |

| Number | Error SubType | Description      |
|--------|---------------|------------------|
| 0      | No Error      | No error occured |
| 1      | ...           | ...              |

<span style="color:red"> These two tables must be completed when TM/TC management tasks code is created. </span>
