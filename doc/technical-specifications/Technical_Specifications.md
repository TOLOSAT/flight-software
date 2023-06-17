# TAPAS Technical Specifications

## Introduction

TOLOSAT Autonomous Payload & Avionic Software (TAPAS) is the flight software for the TOLOSAT nanosatellite. It guarantees the satellite's autonomy and mission success. This is why it includes both avionics and payload management functionalities. In addition, due to the absence of redundancy and specific components, TAPAS must monitor itself and manage events and errors occurring throughout its mission.

The purpose of this manual is to explain in detail how TAPAS works, its various functions and specifications. The section [TAPAS Internal Architecture](Internal_Architecture.md) describes the internal architecture of TAPAS and explains in general terms how each of the activities is coordinated and what their purpose is. All the other sections describe the functionality of each activity and its specification.
## Summary

- [TAPAS Internal Architecture](Internal_Architecture.md)
- [Satellite Life Analysis & Mode Integration](SALAMI.md)
- [Monitoring & Internal Software Monitoring](MISO.md)
- [Common Automated Recording of New Events](CARNE.md)
- [Telecommand & Telemetry Flow](TM_TC_Flow.md)
- [HouseKeeping Management](HouseKeeping_Mgmt.md)
- [AOCS Management](AOCS_Mgmt.md)
- [Power Management](Power_Mgmt.md)
- [Thermal Management](Thermal_Mgmt.md)
- [Gravimetry Payload Management](Gravimetry_Mgmt.md)
- [Iridium Payload Management](Iridium_Mgmt.md)