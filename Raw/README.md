# Raw
This directory contains the first iteration of the decompiled code. The process leverages a hybrid approach using static analysis tools and AI.

## Workflow
1.  **Input:** Disassembly and pseudo-code are extracted using **Binary Ninja** (Pseudo C) and **IDA** (Pseudo Code).
2.  **Generation:** **Google Gemini** processes these inputs to generate the initial C/C++ codebase.
3.  **Verification:** A human contributor reviews the AI-generated code and performs manual adjustments where necessary.

Thanks to modern technology, the decompilation speed has increased by up to **1000%**. This approach significantly reduces development time and preserves the sanity of everyone involved.