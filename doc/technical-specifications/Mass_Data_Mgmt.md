# Mass Data Management

[Come back to first page](Technical_Specifications.md)

## Introduction

The role of the mass data management task is to have a single task that uses and manages mass non-volatile memory and therefore avoids competition for the resource and memory corruption. Its action is very simple: it fills the memory with the data it is given and empties the memory when it is asked for the data again.

**Note:** it only handles large volumes of data. Small volumes of data will be stored in RAM and TAPAS context data will be stored in FRAM.

## Specifications

## Description

## Failure Management

The following two tables list the types and subtypes encountered by Mass Data management task :

| Number | Error Type | Description      |
|--------|------------|------------------|
| 0      | No Error   | No error occured |
| 1      | ...        | ...              |

| Number | Error SubType | Description      |
|--------|---------------|------------------|
| 0      | No Error      | No error occured |
| 1      | ...           | ...              |

<span style="color:red"> These two tables must be completed when the Mass Data management task code is created. </span>
