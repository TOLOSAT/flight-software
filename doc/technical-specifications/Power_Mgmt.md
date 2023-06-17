# Power Management

[Come back to first page](Technical_Specifications.md)

## Introduction

The role of the power management task is to control the Electrical Power System (EPS). It must be able to control all the power-related observables (voltages, power produced, power consumed, battery temperature, etc.), but it must also be able to set the EPS parameters. This is the only task communicating with the EPS, so it is responsible for resetting the EPS watchdog and also for satellite passivation at the end of its life.

## Specifications

## Description

## Failure Management

The following two tables list the types and subtypes encountered by Power management task :

| Number | Error Type | Description      |
|--------|------------|------------------|
| 0      | No Error   | No error occured |
| 1      | ...        | ...              |

| Number | Error SubType | Description      |
|--------|---------------|------------------|
| 0      | No Error      | No error occured |
| 1      | ...           | ...              |

<span style="color:red"> These two tables must be completed when the Power management task code is created. </span>
