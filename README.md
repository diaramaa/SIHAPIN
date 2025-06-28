<div id="top">

<!-- HEADER STYLE: CLASSIC -->
<div align="center">


# SIHAPIN

<em>Empowering Smart Water Solutions for a Better Future</em>

<!-- BADGES -->
<img src="https://img.shields.io/github/last-commit/diaramaa/SIHAPIN?style=flat&logo=git&logoColor=white&color=0080ff" alt="last-commit">
<img src="https://img.shields.io/github/languages/top/diaramaa/SIHAPIN?style=flat&color=0080ff" alt="repo-top-language">
<img src="https://img.shields.io/github/languages/count/diaramaa/SIHAPIN?style=flat&color=0080ff" alt="repo-language-count">

<em>Built with the tools and technologies:</em>

<img src="https://img.shields.io/badge/ReadMe-018EF5.svg?style=flat&logo=ReadMe&logoColor=white" alt="ReadMe">
<img src="https://img.shields.io/badge/C++-00599C.svg?style=flat&logo=C++&logoColor=white" alt="C++">

</div>
<br>

---

## Table of Contents

- [Overview](#overview)
- [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
    - [Usage](#usage)
    - [Testing](#testing)
- [Features](#features)
- [Project Structure](#project-structure)
    - [Project Index](#project-index)

---

## Overview

SIHAPIN is a comprehensive IoT development framework tailored for environmental sensing and remote device management on ESP32 platforms. It streamlines the integration of sensors like TDS, EC, temperature, and more, enabling real-time data collection, calibration, and control within a modular architecture.

**Why SIHAPIN?**

This project aims to simplify the creation of scalable, reliable IoT solutions for water quality and environmental monitoring. The core features include:

- 🧩 **🔧 Modular Design:** Organizes code with private libraries and shared headers for maintainability and reuse.
- 🌐 **🚀 Remote Connectivity:** Supports MQTT and Blynk for seamless remote monitoring and control.
- ⚙️ **🔬 Sensor Calibration:** Provides tools for accurate water quality measurements with temperature compensation.
- 🎛️ **🤖 Automated Actions:** Enables scheduled operations like fish feeding via servo control.
- 📊 **📈 Real-Time Data:** Collects, displays, and transmits sensor data for environmental insights.

---

## Features

|      | Component       | Details                                                                                     |
| :--- | :-------------- | :------------------------------------------------------------------------------------------ |
| ⚙️  | **Architecture**  | <ul><li>Embedded firmware for microcontroller (likely Arduino-based)</li><li>Single firmware project</li></ul> |
| 🔩 | **Code Quality**  | <ul><li>Uses PlatformIO for build management</li><li>Modular code with separate header and source files</li></ul> |
| 📄 | **Documentation** | <ul><li>Includes `readme` and `keywords.txt` for IDE integration</li><li>Basic project info provided</li></ul> |
| 🔌 | **Integrations**  | <ul><li>PlatformIO for build and dependency management</li><li>Standard Arduino libraries implied</li></ul> |
| 🧩 | **Modularity**    | <ul><li>Code organized into multiple files (`.ino`, `.h`, `.cpp`)</li><li>Header files for abstraction</li></ul> |
| 🧪 | **Testing**       | <ul><li>No explicit testing framework detected</li><li>Potential for unit tests in separate files</li></ul> |
| ⚡️  | **Performance**   | <ul><li>Optimized for microcontroller constraints</li><li>Uses minimal dependencies</li></ul> |
| 🛡️ | **Security**      | <ul><li>Limited security features; typical for embedded firmware</li><li>Potential for secure communication protocols not evident</li></ul> |
| 📦 | **Dependencies**  | <ul><li>Core dependencies include Arduino core and PlatformIO configuration</li><li>Additional libraries likely managed via `platformio.ini`</li></ul> |

---

## Project Structure

```sh
└── SIHAPIN/
    ├── include
    │   └── README
    ├── lib
    │   ├── GravityTDS-master
    │   ├── GravityTDS-master.zip
    │   └── README
    ├── platformio.ini
    ├── src
    │   ├── creds.h
    │   └── main.cpp
    └── test
        └── README
```

---

### Project Index

<details open>
	<summary><b><code>SIHAPIN/</code></b></summary>
	<!-- __root__ Submodule -->
	<details>
		<summary><b>__root__</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ __root__</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/platformio.ini'>platformio.ini</a></b></td>
					<td style='padding: 8px;'>- Defines the configuration for an ESP32-based IoT platform utilizing Arduino framework, integrating various sensors and communication libraries<br>- Facilitates device setup, sensor management, and network connectivity, enabling seamless data collection and remote monitoring within the overall system architecture<br>- Ensures consistent build and deployment processes for the embedded environment.</td>
				</tr>
			</table>
		</blockquote>
	</details>
	<!-- lib Submodule -->
	<details>
		<summary><b>lib</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ lib</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/lib/README'>README</a></b></td>
					<td style='padding: 8px;'>- Defines project-specific private libraries, organizing source code into separate directories for modular compilation and linking<br>- Facilitates seamless integration of custom libraries into the overall architecture, enabling efficient code reuse and dependency management within the PlatformIO build system<br>- Ensures clear structure and maintainability of reusable components across the project.</td>
				</tr>
			</table>
			<!-- GravityTDS-master Submodule -->
			<details>
				<summary><b>GravityTDS-master</b></summary>
				<blockquote>
					<div class='directory-path' style='padding: 8px 0; color: #666;'>
						<code><b>⦿ lib.GravityTDS-master</b></code>
					<table style='width: 100%; border-collapse: collapse;'>
					<thead>
						<tr style='background-color: #f8f9fa;'>
							<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
							<th style='text-align: left; padding: 8px;'>Summary</th>
						</tr>
					</thead>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/lib/GravityTDS-master/keywords.txt'>keywords.txt</a></b></td>
							<td style='padding: 8px;'>- Defines syntax highlighting rules for the Gravity TDS sensor library, facilitating clear and consistent code editing<br>- It categorizes keywords, methods, functions, and constants to enhance readability and maintainability within the broader sensor integration framework<br>- This mapping supports developers in efficiently working with the sensors codebase by visually distinguishing key components.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/lib/GravityTDS-master/GravityTDS.cpp'>GravityTDS.cpp</a></b></td>
							<td style='padding: 8px;'>- Provides an interface for measuring and calibrating waters Total Dissolved Solids (TDS) using an analog sensor<br>- Facilitates sensor initialization, real-time TDS and EC value acquisition, temperature compensation, and calibration via serial commands<br>- Integrates persistent storage for calibration factors, ensuring accurate, consistent readings within the broader water quality monitoring architecture.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/lib/GravityTDS-master/LICENSE'>LICENSE</a></b></td>
							<td style='padding: 8px;'>- The provided code file contains the license information for the project, specifically the GNU Lesser General Public License (LGPL) version 2.1<br>- Its primary purpose is to define the licensing terms under which the projects code can be used, shared, and modified<br>- Within the overall architecture, this license file ensures that the project remains open and freely accessible, promoting collaboration and reuse, particularly for library components that can be integrated into other software<br>- It underscores the projects commitment to software freedom while clarifying legal permissions and obligations for users and contributors.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/lib/GravityTDS-master/GravityTDS.h'>GravityTDS.h</a></b></td>
							<td style='padding: 8px;'>- Provides an interface for measuring and calibrating water quality parameters, specifically TDS and EC, using an analog sensor<br>- Facilitates temperature compensation, calibration, and data retrieval, integrating seamlessly into larger water monitoring systems<br>- Supports accurate, real-time water quality assessment within the overall architecture of environmental sensing and IoT-based water management solutions.</td>
						</tr>
					</table>
					<!-- GravityTDSExample Submodule -->
					<details>
						<summary><b>GravityTDSExample</b></summary>
						<blockquote>
							<div class='directory-path' style='padding: 8px 0; color: #666;'>
								<code><b>⦿ lib.GravityTDS-master.GravityTDSExample</b></code>
							<table style='width: 100%; border-collapse: collapse;'>
							<thead>
								<tr style='background-color: #f8f9fa;'>
									<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
									<th style='text-align: left; padding: 8px;'>Summary</th>
								</tr>
							</thead>
								<tr style='border-bottom: 1px solid #eee;'>
									<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/lib/GravityTDS-master/GravityTDSExample/GravityTDSExample.ino'>GravityTDSExample.ino</a></b></td>
									<td style='padding: 8px;'>- Provides functionality to measure and calibrate water TDS levels using an analog sensor, enabling real-time monitoring of water quality<br>- Integrates temperature compensation for accurate readings and supports calibration procedures<br>- Serves as a core component within the larger water quality assessment system, facilitating data collection and analysis for environmental or industrial applications.</td>
								</tr>
							</table>
						</blockquote>
					</details>
				</blockquote>
			</details>
		</blockquote>
	</details>
	<!-- include Submodule -->
	<details>
		<summary><b>include</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ include</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/include/README'>README</a></b></td>
					<td style='padding: 8px;'>- Defines shared declarations and macro definitions to facilitate consistent interface management across multiple source files, ensuring maintainability and reducing errors<br>- Serves as a central repository for project-wide header information, streamlining integration and updates within the overall architecture<br>- Enhances modularity by enabling seamless inclusion of common interfaces in the source code.</td>
				</tr>
			</table>
		</blockquote>
	</details>
	<!-- test Submodule -->
	<details>
		<summary><b>test</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ test</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/test/README'>README</a></b></td>
					<td style='padding: 8px;'>- Provides guidance for executing and managing unit tests within the PlatformIO environment, ensuring individual MCU modules and related components are verified for correctness<br>- Supports early detection of issues during development, contributing to overall code quality and reliability across the embedded system architecture<br>- Facilitates structured testing practices aligned with the projects modular design.</td>
				</tr>
			</table>
		</blockquote>
	</details>
	<!-- src Submodule -->
	<details>
		<summary><b>src</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ src</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/src/main.cpp'>main.cpp</a></b></td>
					<td style='padding: 8px;'>- Facilitates real-time monitoring and control of an integrated environmental sensor system on an ESP32 platform<br>- Collects data from multiple sensors, updates a display, and communicates insights via MQTT and Blynk, enabling remote management of parameters like temperature, humidity, pH, TDS, and energy consumption<br>- Coordinates automated actions such as fish feeding through scheduled servo operations, supporting comprehensive environmental management.</td>
				</tr>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='https://github.com/diaramaa/SIHAPIN/blob/master/src/creds.h'>creds.h</a></b></td>
					<td style='padding: 8px;'>- Defines Wi-Fi credentials and Blynk platform authentication details to enable seamless connectivity and remote control capabilities within the project<br>- Serves as a centralized configuration point, facilitating secure communication between the device and cloud services, and supporting the overall architecture of IoT device management and data exchange.</td>
				</tr>
			</table>
		</blockquote>
	</details>
</details>

---

## Getting Started

### Prerequisites

This project requires the following dependencies:

- **Programming Language:** CPP
- **Package Manager:** Cmake

### Installation

Build SIHAPIN from the source and install dependencies:

1. **Clone the repository:**

    ```sh
    ❯ git clone https://github.com/diaramaa/SIHAPIN
    ```

2. **Navigate to the project directory:**

    ```sh
    ❯ cd SIHAPIN
    ```

3. **Install the dependencies:**

**Using [cmake](https://isocpp.org/):**

```sh
❯ cmake . && make
```
**Using [cmake](https://isocpp.org/):**

```sh
❯ cmake . && make
```

### Usage

Run the project with:

**Using [cmake](https://isocpp.org/):**

```sh
./SIHAPIN
```
**Using [cmake](https://isocpp.org/):**

```sh
./SIHAPIN
```

### Testing

Sihapin uses the {__test_framework__} test framework. Run the test suite with:

**Using [cmake](https://isocpp.org/):**

```sh
ctest
```
**Using [cmake](https://isocpp.org/):**

```sh
ctest
```

---

<div align="left"><a href="#top">⬆ Return</a></div>

---
