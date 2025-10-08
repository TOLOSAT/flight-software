# TAPAS Technical Specifications

## Introduction

TOLOSAT Autonomous Payload & Avionic Software (TAPAS) is the flight software for the TOLOSAT nanosatellite. It guarantees the satellite's autonomy and mission success. This is why it includes both avionics and payload management functionalities. In addition, due to the absence of redundancy and specific components, TAPAS must monitor itself and manage events and errors occurring throughout its mission.

The purpose of this manual is to explain in detail how TAPAS works, its various functions and specifications. The section [TAPAS Software Architecture](Software_Architecture.md) describes the internal architecture of TAPAS and explains in general terms how each of the activities is coordinated and what their purpose is. All the other sections describe the functionality of each activity and its specification.
## Summary

Here are the components of TAPAS linked to its internal functioning :
- [Software Architecture](Software_Architecture.md)
- [HouseKeeping Management](HouseKeeping_Mgmt.md)
- [Telecommand & Telemetry Flow](TM_TC_Flow.md)

Here are the interfaces between Hardware and Software :
- [Hardware & Software Interfaces](Hardware_Software_Interfaces.md)
