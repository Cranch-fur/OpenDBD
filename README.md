# OpenDBD
An open-source project focused on reverse engineering **Dead by Daylight** to ensure its preservation and study. The repository is strictly based on game version **1.6.2**.

## 🔄 Project Status
We are actively working on the codebase. This repository will receive **periodic bulk updates** as we successfully decompile and verify new classes.

## 🗺️ Roadmap
The decompilation is currently planned in the following stages:
1.  **Core Classes [.exe/C++]** Reverse engineering of game-specific logic and classes (e.g., `\Source\DeadByDaylight\DBDGame`).
2.  **Plugins [.exe/C++]** Reverse engineering of game-specific plugins and subsystems (e.g., `\Source\OnlinePresence\`).
3.  **Project Reconstruction** Full reconstruction of the **Unreal Engine Project** files tailored for usability.

## 💬 Q/A
Reverse engineering is a lengthy and complex process that usually takes years to accomplish. However, we have something many other projects didn't: an **Open Source Engine** we can always take a look in to, a `.pdb` (Program Database) file and the power of modern AI technologies (Google AI CORP-PRO) to accomplish most of the repetitive and time-consuming work.

As of now, we are only taking our first steps and preparing to fully dive in. While I cannot yet guarantee we will see the project through to the very end, I am extremely optimistic about the idea of having an **Open Source**, easily editable, and configurable version of Dead by Daylight. Its existence would influence most legacy game versions, as you would be able to port a lot of the 1.6.2 PDB content over to newer (1.9.3) or older (1.4.0) builds.
