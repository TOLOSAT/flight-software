# Common Automated Recording of New Events (CARNE)

[Come back to first page](UserManual.md)

## Introduction

Common Automated Recording of New Events (CARNE), along with SALAMI and MISO, is one of the three major tasks of TAPAS. Its role is to check that the satellite is behaving as expected. In fact, events are messages that signal that the satellite is leaving or is about to leave its nominal behaviour. An event does not represent a software error, unlike what SALAMI and MISO monitor.

## Specifications

## Description

CARNE, like MISO, was created to avoid SALAMI being the only task to monitor. SALAMI keeps the task execution monitoring, CARNE monitors the events taking place in the satellite and MISO monitors the OS. The advantages of separating these three activities are more dynamic scheduling (SALAMI does not monopolise the time available) and more effective failure detection.

## Failure Management

The following two tables list the types and subtypes encountered by CARNE:

| Number | Error Type | Description      |
|--------|------------|------------------|
| 0      | No Error   | No error occured |
| 1      | ...        | ...              |

| Number | Error SubType | Description      |
|--------|---------------|------------------|
| 0      | No Error      | No error occured |
| 1      | ...           | ...              |

<span style="color:red"> These two tables must be completed when the CARNE code is created. </span>
