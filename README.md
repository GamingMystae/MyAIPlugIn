# MyAIPlugin

**MyAIPlugin** is a custom **Unreal Engine 5.4 AI behavior plugin prototype** created during the **second semester of my second year in college**.

The goal of the project was to **modify and extend an Unreal Engine AI-related subsystem** by building a custom plugin around AI behavior control.  
Instead of relying only on traditional setup and hardcoded AI tuning, I focused on exposing AI behavior through **plugin settings**, **runtime controls**, and a **debug UI workflow** using sliders, checkboxes, and behavior options.

This was one of the most technically ambitious projects I created during my second year.

---

## Project Goal

The core idea behind **MyAIPlugin** was to explore how Unreal Engine AI behavior systems could be made more flexible and easier to configure at runtime.

For this project, I chose to focus on **AI behavior control**, especially around systems that are normally configured indirectly through Unreal’s AI framework.  
My goal was to create a plugin that would let the user adjust AI behavior more directly through:

- configurable plugin settings
- runtime debug controls
- patrol mode switching
- combat behavior switching
- editable movement and timing values
- checkbox / slider-based debugging workflow

Although I had bigger plans for this plugin, I did not have enough time to fully complete every feature I originally intended.  
Even so, this project became one of my most difficult and technically interesting Unreal Engine assignments.

---

## Overview

**MyAIPlugin** is a runtime Unreal Engine plugin that introduces a custom AI control framework built around:

- a custom AI controller
- configurable AI states
- multiple patrol modes
- combat behavior states
- patrol path and spline path actors
- a runtime settings object
- a debug UI manager
- a debug widget for changing values during gameplay

The plugin is designed to support AI characters that can patrol, chase, attack, and be debugged through a more user-facing workflow than a purely hardcoded setup.

---

## Core Features

### 1. Custom AI State System
The plugin defines custom AI enums for:

- **AI State**
  - None
  - Idle
  - Patrolling
  - Chasing
  - Attacking
  - Fleeing

- **Patrol Mode**
  - Random
  - Waypoints
  - Spline Path

- **Combat Style**
  - Passive
  - Defensive
  - Aggressive

This gives the AI system a clearer structure and makes it easier to expose behavior options through settings and UI.

---

### 2. Custom AI Controller
At the center of the plugin is a custom `AMyAIController` that supports:

- **random patrol**
- **waypoint patrol**
- **spline-based patrol**
- **chase behavior**
- **attack behavior**
- patrol mode refresh/reset logic
- timer-based pacing for patrol and attack flow

This controller was designed as a more configurable alternative to a simple fixed AI setup.

---

### 3. Patrol Path and Spline Path Support
The plugin includes two reusable actor types for patrol behavior:

#### `APatrolPath`
- stores manually assigned patrol points
- supports optional per-point wait times
- validates whether the path is correctly configured

#### `ASplinePath`
- uses a spline component for path-based movement
- supports per-point wait times
- visualizes wait times in editor construction using debug draw

This made it possible to test different patrol styles inside the same plugin system.

---

### 4. Runtime AI Settings
The plugin includes a configurable settings object: `UMyAIPluginSettings`.

It exposes gameplay values such as:

- enable / disable patrol
- patrol radius
- patrol interval
- patrol movement speed
- patrol mode
- spline ping-pong option
- spline wait time
- default combat style
- enable / disable chase
- enable / disable attack
- chase distance
- attack range
- attack cooldown
- debug log toggles

These settings are registered under:

**Project Settings > Plugins > My AI Plugin**

This allowed the plugin to act more like a configurable engine extension rather than a one-off AI script.

---

### 5. Runtime Debug UI
One of the main ideas of the project was to expose AI behavior through a more visual interface.

The plugin includes:

- `ADebugUIManager`
- `UDebugAISettingsWidget`

The debug manager can:

- spawn the debug widget
- assign a test AI controller reference
- toggle the UI visibility at runtime
- switch between input modes
- apply patrol and combat changes live

The debug widget is designed to synchronize UI controls such as:

- checkboxes
- sliders
- patrol mode options
- combat style options
- debug toggles

The debug UI is toggled in runtime using the **T key**.

---

### 6. AI Character Integration
The plugin includes an `ANPC_Character` class that can be assigned either:

- a `PatrolPath`
- or a `SplinePath`

This allows the same AI plugin framework to be used with different patrol setups depending on the character or level.

---

## Technical Focus

This project helped me practice:

- Unreal Engine plugin structure
- module-based architecture
- exposing systems through Project Settings
- creating configurable AI behavior frameworks
- designing custom AI states and patrol modes
- runtime AI tuning
- timer-based AI logic
- AI controller architecture
- spline and waypoint path systems
- debug UI integration with gameplay code
- using C++ to extend Unreal Engine systems in a more tool-oriented way

---

## Why This Project Was Challenging

This was one of the hardest repositories I built because it moved beyond simply “making an NPC work.”

Instead, the challenge was to build a **plugin-based AI framework** that tried to give the user more direct control over AI behavior using:

- settings
- toggles
- sliders
- debug widgets
- reusable AI support actors

That made the project more similar to an **engine modification / gameplay systems tool** than a standard game prototype.

---

## Current Scope

The repository currently demonstrates:

- a runtime plugin module
- custom AI state enums
- custom AI controller structure
- patrol path and spline path actors
- AI settings registered in Unreal Project Settings
- a runtime debug UI manager
- a debug widget structure for syncing gameplay settings
- NPC integration with patrol/spline assignment

It represents a strong prototype/foundation for a more advanced plugin, even though not every planned feature was fully completed.

---

## Tech Stack

- **Engine:** Unreal Engine 5.4
- **Language:** C++
- **Framework / Systems:** Unreal Engine Plugin System, AIController, Navigation System, Gameplay Tasks, UMG, Slate, Project Settings integration
- **Project Type:** College assignment / engine modification prototype
- **Focus Area:** AI behavior tooling and runtime configurability

---

## Main Classes

### `AMyAIController`
Custom AI controller supporting:
- random patrol
- waypoint patrol
- spline patrol
- chase behavior
- attack logic
- patrol mode refresh/reset
- timer-based behavior flow

### `UMyAIPluginSettings`
Configurable settings object for:
- patrol settings
- combat settings
- debug settings
- plugin-wide runtime tuning

### `ADebugUIManager`
Handles:
- spawning and toggling the debug widget
- switching UI/game input mode
- updating runtime AI settings
- refreshing patrol behavior after changes

### `UDebugAISettingsWidget`
Widget class intended to:
- sync runtime settings to UI
- expose AI controls through checkboxes / sliders / options
- connect user input with plugin behavior changes

### `ANPC_Character`
AI character class that can use:
- `APatrolPath`
- `ASplinePath`

### `APatrolPath`
Stores:
- waypoint arrays
- optional per-point wait times
- path validation

### `ASplinePath`
Stores:
- spline-based routes
- per-point wait times
- editor visualization support

---

## What I Learned

Through **MyAIPlugin**, I learned a lot about how Unreal Engine systems can be extended beyond normal gameplay scripting.

This project helped me understand:

- how to structure a runtime plugin
- how to expose plugin settings to users
- how to build AI behavior systems that are easier to tune
- how to connect runtime UI with engine-side gameplay systems
- how difficult it is to design reusable systems rather than one-off features
- how engine-level thinking is different from game-level thinking

It also taught me a lot about scope management, because my original idea was bigger than what I could complete in the available time.

---

## Project Status

**Prototype / partial implementation**  
Created as a **second-year second-semester Unreal Engine subsystem modification project**.

The plugin already contains a strong technical foundation, but it was not fully completed to the full scope I originally planned.

---

## Future Improvements

Planned or possible future improvements include:

- more complete UI widget binding
- richer editor-side tools
- more advanced AI state transitions
- expanded debug visualization
- profile saving/loading for AI presets
- better multi-NPC assignment workflows
- editor utility integration
- more advanced combat and flee logic
- stronger documentation and example maps

---

## Author

**George Bolias**  
Game Programmer / Unreal Engine Developer

- Portfolio: [https://www.georgebolias.com/](https://www.georgebolias.com/)
- LinkedIn: [https://www.linkedin.com/in/georgebolias/](https://www.linkedin.com/in/georgebolias/)
- GitHub: [https://github.com/GamingMystae](https://github.com/GamingMystae)
