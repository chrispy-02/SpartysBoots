# SpartysBoots

A logic-circuit puzzle game built in **C++** with a **wxWidgets** GUI. Players build working digital circuits from logic gates — **AND, OR, NOT, NAND, SR flip-flop, and D flip-flop** — to sort products on a conveyor belt and complete each level. It was a four-person Michigan State University **CSE 335 – Object-Oriented Software Design** team project, inspired by the 1982 Warner Bros. game *Rocky's Boots*.

▶️ **See the game in action (team gameplay video):** https://www.youtube.com/watch?v=d-wygT7Cytc

---

## What the game does

Each level drops products onto a conveyor belt past a sensor. The player wires up logic gates to read the sensor outputs and decide which products to kick off the belt, scoring points for correct decisions. Levels grow in complexity, eventually requiring stateful circuits built from flip-flops. The whole simulation runs live in an event-driven wxWidgets window with draggable gates, animated beams, and a scoreboard.

## Tech stack

- **C++17** — core game and simulation logic
- **wxWidgets** — cross-platform event-driven GUI
- **CMake** (≥ 3.24) — build configuration and automation
- **Google Test** — unit testing (fetched automatically via CMake `FetchContent`)
- **Doxygen** — API documentation generation
- **XML** — data-driven level definitions

## Features

- Event-driven **wxWidgets** GUI with draggable, wirable components
- Combinational logic gates: **AND, OR, NOT, NAND**
- Sequential logic gates: **SR flip-flop** and **D flip-flop**
- **Sparty kicking mechanic** — kicks products off the belt when triggered by the wired circuit
- **Scoreboard and scoring system** — tracks level and game score against per-level scoring rules
- **Conveyor belt** and **conveyor control panel** gameplay systems
- **XML-based level loading** for data-driven, easily authored levels
- Modular, object-oriented architecture (visitor pattern, separated game/view layers, dedicated library target)
- **Unit tests** with Google Test (`Tests/`)
- **Doxygen** documentation support (`Doxyfile.doxy`)

## My contributions — Christopher Nguyen

This was a collaborative team project. My specific contributions — reflected by the `@author` tags in the source — center on the **Scoreboard**, **ScoringVisitor**, and **Sparty** classes:

- **Implemented the `Scoreboard` class** and its XML loading (position, scoring values, instruction text) and rendering (level score, game score, border styling, instruction display).
- **Built the score-display logic** for level score, game score, and instruction text.
- **Implemented the `ScoringVisitor` class** that determines product scoring outcomes (kicked, missed, correctly ignored, and already-scored products).
- **Implemented (or co-implemented) `Sparty`** — XML loading, image-based rendering, input-pin/wire-connection behavior, kick-animation timing, and product-kick triggering logic.
- Supported object-oriented implementation work within a four-person team project.

## Team & attribution

SpartysBoots was built by a **four-person team** in MSU CSE 335: **Bob Wilson**, **Elijah Porter**, **Christopher Nguyen**, and **Zaid Quorah**. Credit for the project is shared across the team; the "My contributions" section above describes only my own work.

The **game design and all image assets were provided by the MSU course staff / professors** — the team wrote the C++ code. The game is inspired by *Rocky's Boots* (1982).

## Asset & licensing note

The images in `images/` (including the Sparty mascot and other sprites) and the level definitions in `levels/` were **provided by the MSU CSE 335 course staff** and are included here for educational and portfolio-demonstration purposes only. They are **not independently licensed** for redistribution — "Sparty" and related marks belong to Michigan State University. The original source code is the work of the student team. No separate open-source license is applied to this repository because it bundles course-provided assets.

## Building locally

> Note: builds are not verified as part of this portfolio mirror; the steps below reflect the existing CMake configuration.

**Prerequisites:** a C++17 compiler, CMake ≥ 3.24, and **wxWidgets** (components: `core base xrc html xml`) installed and discoverable by `find_package`.

```bash
cmake -S . -B build
cmake --build build
```

Google Test is downloaded automatically by CMake, and the unit-test target (`Tests_run`) is built alongside the game.

## Portfolio note

This repository is a **cleaned public portfolio mirror** of the original CSE 335 team project. It was published with a **fresh Git history** (the original GitLab history and any private course/grading metadata were not carried over) to present the work cleanly while preserving honest team attribution.
