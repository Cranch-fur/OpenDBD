# UDump
This directory contains Unreal Engine header dumps generated for **Dead by Daylight 1.6.2**, using a utility developed by Oleg Shabunov.

## Scope & Limitations
Please note that this dump interacts only with the Unreal Engine Reflection System.
* It **does not** analyze the native C++ executable code.
* It **only** contains functions, properties, and structs exposed to the Blueprints Virtual Machine (e.g., `UFunction`, `UProperty`, `UStruct`).