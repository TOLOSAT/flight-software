## TAPAS Internal Architecture Isolation

[Come back to first page](UserManual.md)

### Introduction

As mentioned above, TAPAS is the flight software that manages the platform and payloads. It must function autonomously when not in visibility of the ground segment, but it must also be capable of being piloted remotely by an operator. 
TAPAS' main activities are as follows:
- Platform management (thermal, scao, power and mass memory).
- Payload management.
- Command and control.
- Internal management.
Internal management covers all the activities that ensure the software's autonomy, performance and robustness.
### Specifications

This section contains the global specifications. The internal specifications for each activity will be specified in their respective sub-sections.

| Reference      | Name     | Rational | Description                                                               |
|----------------|----------|----------|---------------------------------------------------------------------------|
| T-TAPAS-001-00 | Autonomy | N/A      | TAPAS must be autonomous and function even in the absence of an operator. |

| Reference      | Name           | Rational | Description                                      |
|----------------|----------------|----------|--------------------------------------------------|
| T-TAPAS-002-00 | Remote control | N/A      | TAPAS can be controlled remotely by an operator. |

| Reference      | Name                  | Rational | Description                                                                   |
|----------------|-----------------------|----------|-------------------------------------------------------------------------------|
| T-TAPAS-003-00 | Operator vs. Software | N/A      | The operator's decision always takes precedence over the software's decision. |

| Reference      | Name      | Rational | Description                        |
|----------------|-----------|----------|------------------------------------|
| T-TAPAS-004-00 | Safe Boot | N/A      | TAPAS must be guaranteed to start. |

| Reference      | Name        | Rational | Description                                                   |
|----------------|-------------|----------|---------------------------------------------------------------|
| T-TAPAS-005-00 | End of life | N/A      | The end-of-life of the satellite must be guaranteed by TAPAS. |

| Reference      | Name         | Rational       | Description                                                           |
|----------------|--------------|----------------|-----------------------------------------------------------------------|
| T-TAPAS-010-00 | Adaptability | T-TAPAS-001-00 | TAPAS must have several execution modes to handle different contexts. |

| Reference      | Name       | Rational       | Description                                                                  |
|----------------|------------|----------------|------------------------------------------------------------------------------|
| T-TAPAS-011-00 | Robustness | T-TAPAS-001-00 | If an error or a certain events occur, TAPAS must change its execution mode. |

| Reference      | Name | Rational       | Description                                                                      |
|----------------|------|----------------|----------------------------------------------------------------------------------|
| T-TAPAS-012-00 | FDIR | T-TAPAS-001-00 | TAPAS must have Failure Detection, Identification and Recovery (FDIR) mechanisms |

| Reference      | Name                | Rational       | Description                                            |
|----------------|---------------------|----------------|--------------------------------------------------------|
| T-TAPAS-013-00 | Autonomous Platform | T-TAPAS-001-00 | TAPAS must ensure that avionics operate automatically. |

| Reference      | Name               | Rational       | Description                                            |
|----------------|--------------------|----------------|--------------------------------------------------------|
| T-TAPAS-014-00 | Autonomous Payload | T-TAPAS-001-00 | TAPAS must ensure that payloads operate automatically. |

| Reference      | Name         | Rational       | Description                                                                     |
|----------------|--------------|----------------|---------------------------------------------------------------------------------|
| T-TAPAS-015-00 | PUS Protocol | T-TAPAS-002-00 | Exchanges between TAPAS and the ground must respect PUS (cf. ECSS-E-ST-70-41C). |

### Description

<img src="../images/Internal_Architecture_Graph.png" width=100% />

### Failure Detection Identification & Recovery