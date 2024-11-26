<a name="readme-top"></a>

# TurningTurnTable

<div align="center">
	<img src="https://github.com/JoJos1220/TurningTurnTable_private/assets/97045955/add5842e-8f65-4d4a-adc6-8ad197f864d0" alt="20240608_230146">
	<br>
	<b>Figure 1: Overview of the functional turning assembly with happy jellyfish</b>
</div>

[![Codacy Badge][codacy-shield]][codacy-url]
[![PlatformIO CI][build-shield]][build-url]
![License][license-url]
[![Release Version][realease-shield]][release-url]
[![Release Date][releasedate-shield]][releasedate-url]
[![Last commit][lastcommit-shield]][lastcommit-url]
[![Contributors][contributors-shield]][contributors-url]
[![vscode.dev preview][vscode-dev-shield]][vscode-dev-url]
[![Dependabot][depandbot-shield]][depandbot-url]
[![GitHub Actions][githubactions-shield]][githubactions-url]
[![PayPal][Paypal-shield]][paypal-url]
[![Ko-Fi][Ko-Fi-shield]][Ko-Fi-url]

## About this Project and Repository
Welcome to the source code repository for the TurningTurnTable project, built on ESP32 for the dynamic control of the autonomous Turntable.

For a comprehensive assembly guide and additional project details, please refer to the <a href="#contact">Contact</a> section, where you'll find project links leading to detailed instructions on Instructables.

### Features

**Autonomous Mode:** The standout feature is the autonomous mode, which rotates the turntable at a specified angle and triggers the connected device via BLE to take a picture. Depending on the setup, up to 64 pictures of the object can be captured.

**Hand Mode:** Is the automatic mode not suitable for your needs? You can manually control the turntable through the menu, adjusting it to a specific angle or rotating it infinitely in any direction.

**Control by OLED-Display and Encoder:** The turntable is fully controlled through an integrated OLED display and encoder. In addition to switching between Auto and Manual modes, you can set parameters such as stepper acceleration/speed and the stepper angle between pictures. You can also check the BLE connection to your device and take a test screenshot.

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-this-project-and-repository">About this Project/Repository</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
    <li><a href="#support">Support</a></li>
  </ol>
</details>

Happy tinkering and making picutes! 🚀🛠️💡

## Getting Started

Before you begin, ensure you have the following components (or their equivalents) available.

The base component is an ESP32 board with a prototyping shield, similar to an Arduino. You can also use a NodeMCU-specific board, which functions similarly but has different pin definitions. A stepper motor driver and stepper motor are connected to this board for motion control. An OLED display and encoder are installed for operating the assembly. The project uses the internal Arduino power supply and an external 12V power adapter to operate the stepper motor driver. To complete the setup, you will need some M3/M4 screws, nuts, washers, heat inserts, bearings, and cables. For wiring, refer to the pins.h file, which describes the pins used for the specific board and interface.

For detailed assembly instructions, refer to the Instructable guide and the CULTS3D homepage in the <a href="#contact">Contact</a> section.

![20240707_104415](https://github.com/JoJos1220/TurningTurnTable_private/assets/97045955/09ae8746-ba03-49b8-b9f8-012695f01e56)

**Figure 2: Hardware Setup -> ESP32 "Arduino", NEMA 17 Pancake Stepper, Stepper-Motor Driver Board and some connector cables**
_____________________________________________________________________________________________________________
### Preparing 3D-printed parts

The STL files and a detailed 3D printing overview of the project are available for download on Cults3D:

Link to Cults3D: [![Cults3D][cults3d-shield]][cults3d-url]

Feel free to explore my other projects as well 🚀.

_____________________________________________________________________________________________________________

After preparing the parts, start by assembling the base plate. Insert the heat inserts into their designated spots and then place the bearings into the gaps on the base plate. Next, attach the front plate mount. For the front plate, install the OLED display and the encoder, securing them with screws and heat inserts. Once the hardware assembly is complete, move on to the electronics, starting with the Arduino-like ESP32 board and the prototyping shield. Then, install the stepper motor and the stepper motor driver. Depending on the stepper motor used, either print the alternative stepper motor adapter plate for the NEMA 17 Pancake Stepper or mount the 28BYJ-48 directly in place. Finally, complete the assembly by adding heat inserts to the top part and screwing the gear into place.

![20240610_195708](https://github.com/JoJos1220/TurningTurnTable_private/assets/97045955/6a7f185e-af64-4c54-9e0e-f220f1c365e0)

**Figure 3a: Hardware Schematics**

![Electrical_Wiring_Schematic](https://github.com/JoJos1220/TurningTurnTable_private/assets/97045955/f1f520b2-62a7-44f9-9073-60708838191a)

**Figure 3b: Electrical Wiring Schematics**

<p align="right">(<a href="#readme-top">back to top</a>)</p>

_____________________________________________________________________________________________________________

# Comissioning
Before flashing the software onto the ESP32, pre-define your specific hardware setup in the "platformio.ini" file and set the project-specific parameters in the "TurningTurnTablePARAMETERS.h" file.

The most critical sections in the "platformio.ini" file:

```
[platformio]
default_envs = esp32ARDUINO ; Selecting your specific Default Environment for Software Build

[env:esp32xxxxxxxxxxxx]
build_flags = 
	-D ESP32dev_ARDUINO				; Defining Electronic Board
	-D _17HS08					; Defining Used Stepper Type in FULLStep Mode(_17HS08 or _28BYJ48)
							; Stepper Operation Mode is Setup in TurningTurnTablePARAMETERS.h

```

Most specific sections in the TurningTurnTablePARAMETERS.h file:

```
// Defining Stepper Operation Mode on your specific connection/operation Purpose
#define StepperOperation 2                              // 0 == FULL4WIRE; 1 == HALF4WIRE; 2 == DRIVER Stage with 16 MicroSteps

```


When you power on the turntable, it should start up, displaying the boot screen on the OLED display. The turntable is now ready for operation, and you can control it using the encoder and your smartphone.

![000_RestartScreen](https://github.com/JoJos1220/TurningTurnTable_private/assets/97045955/bbe95326-75c8-4588-8123-16dcb9207b01)

**Figure 4: OLED-Boot Screen**

## Roadmap

The initial release of this project introduces a fully functional prototype to the open-source community.

Future updates will incorporate additional features based on community feedback, and the roadmap will be continuously updated.

- Enhancing support for various displays

- Remote control functionality for turntable operation

- Integration of internal power source capabilities

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

Everybody is welcome to contribute the project - regardless of the experience level!

1) fork the repository
2) clone your fork on your PC
3) create a branch for your changes
4) add you changes
5) commit and push
6) create a pull request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## License

Creative Commons Attribution Share Alike 4.0

[![License: CC BY-NC-ND 4.0](https://img.shields.io/badge/License-CC_BY--NC--ND_4.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-nd/4.0/)

See also [LICENSE](LICENSE.md) for more information about license rights and limitations.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contact

Project Link [![Github][github-shield]][github-url]

Project Link [![Instructables][instructables-shield]][instructables-url]

Projekt Link [![Cults3D][cults3d-shield]][cults3d-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Acknowledgments
For more detailed information about the Project, please check the Instructables Link given within the Contact section.

## Support

You Like the Project and want to Support me and my work?

Well, I like coffee ;) Maybe we got a deal?

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)][Ko-Fi-url]

[![Donate with PayPal](https://raw.githubusercontent.com/stefan-niedermann/paypal-donate-button/master/paypal-donate-button.png)][paypal-url]


<div align="center">
	<img src="https://github.com/JoJos1220/TurningTurnTable_private/assets/97045955/400a323a-97e6-44a3-b350-c1d8766c46f5" alt="20240608_230343">
	<br>
	<b>Figure 5: Happy-Turning-Puppy :)</b>
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
[instructables-shield]: https://img.shields.io/badge/Instructables-jojo__1220-blue?logo=instructables
[instructables-url]: https://www.instructables.com/360-Smartphone-Photography-With-BLE-ESP32-Controll/
[github-shield]: https://img.shields.io/badge/GitHub-Jojos1220-black?logo=github
[github-url]: https://github.com/JoJos1220/TurningTurnTable
[cults3d-shield]: https://img.shields.io/badge/cults3D-JOJO__1220-pink.svg?logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAARCAYAAAA7bUf6AAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAB3RJTUUH6AcIBTkVJMVsCgAAAkxJREFUOI2llD9oU1EUxr97X9KkIY0+bU1an2QwVLCQdKhU8E9FBefgYKFUHBzSwUmqoxBwEaFDh2QpdBAcpFksSlsEU8Q6RMQ/TUtIwJS2aVPS1sYkTV/ePQ7PJn1JFcRvu/ee+7vnfPfcy4iI8J8y/W2xXCDks4TyT8DhZGg5wf4NEg2rmA2pcLgY7K0MPzIEWWG4+cQCh6sORofo4wuVQv4SfXtdMcx/iqgU9BYom9QM8w2Q+akKhfylhsB9fXim0si1omGd15exMFNBt98EczNDcbvR894BE9q7OFLvxZ89Wf4sEJ/WMPNUHzvPcNx4YIa7R6rGyCcZVr5o1e2GTCaGy5BPMdwZt+L+22YEIlZYWw43fjUuEA2rxkwmhsvYSBECEWs10CYz9I9akIkLA2BrhXD1XhMiD8s1yG6ekIhquPvcino12Vi1lHRMQ+47YSMp0HlFwuUhM7IJoUPSMYGjHRxtp40+7+4Q1hMC7h6p2jcA0D9qgdkKeC5I+DpZ0SFbywSL3ZhBNKRiNqzi9pgFANAXMOPcLRO2VwkdXfphssKQz5JurOLj2FwyXqfi4/Bckgy3YpNZFQAA8WkNssJ/Q7wcSjfHq8d7NYiXY2eNsPhGa/BpX3PjKjwXpVrb57OCgt4CzU/VWj0RrdDI9SKl5oztn0sLmgyWaWygRERE7OBXsJ4QePloD4UcQfFx2FsZku80bC4ROvskHHMzZOIC6ZjA+UETegfNcDgZDJCDsLVFAaHpZR1x6ZszCwLH3RztZznsbbWX/Au343MY0yo+PwAAAABJRU5ErkJggg==
[cults3d-url]: https://cults3d.com/de/modell-3d/gadget/360-autonomus-bluetooth-controlled-turningturntable
[vscode-dev-shield]: https://img.shields.io/badge/preview%20in-vscode.dev-blue
[vscode-dev-url]: https://open.vscode.dev/JoJos1220/TurningTurnTable

[license-url]:https://img.shields.io/github/license/JoJos1220/TurningTurnTable?style=flat-square
[codacy-shield]: https://app.codacy.com/project/badge/Grade/6d7d1f912c0549589f19d4fdf015cab0
[codacy-url]:https://app.codacy.com/gh/JoJos1220/TurningTurnTable/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade
[build-shield]: https://github.com/JoJos1220/TurningTurnTable/actions/workflows/workflow.yml/badge.svg
[build-url]: https://github.com/JoJos1220/TurningTurnTable/actions/workflows/workflow.yml
[contributors-shield]: https://img.shields.io/github/contributors/JoJos1220/TurningTurnTable
[contributors-url]: https://github.com/JoJos1220/TurningTurnTable/graphs/contributors
[realease-shield]: https://img.shields.io/github/release/JoJos1220/TurningTurnTable.svg?style=plastic
[release-url]: https://github.com/JoJos1220/TurningTurnTable/releases/latest
[releasedate-shield]: https://img.shields.io/github/release-date/JoJos1220/TurningTurnTable.svg?style=plastic
[releasedate-url]: https://github.com/JoJos1220/TurningTurnTable/releases/latest/
[lastcommit-shield]: https://img.shields.io/github/last-commit/JoJos1220/TurningTurnTable?style=plastic
[lastcommit-url]: https://github.com/JoJos1220/TurningTurnTable/tree
[depandbot-shield]: https://img.shields.io/badge/dependabot-025E8C?style=for-the-badge&logo=dependabot&logoColor=white
[depandbot-url]:https://github.com/JoJos1220/TurningTurnTable/actions/workflows/dependabot.yml
[githubactions-shield]: https://img.shields.io/badge/github%20actions-%232671E5.svg?style=for-the-badge&logo=githubactions&logoColor=white
[githubactions-url]:https://github.com/JoJos1220/TurningTurnTable/actions
[paypal-shield]: https://img.shields.io/badge/PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white
[paypal-url]: https://www.paypal.com/donate/?hosted_button_id=8CTAKMUENCF46
[Ko-Fi-shield]: https://img.shields.io/badge/Ko--fi-F16061?style=for-the-badge&logo=ko-fi&logoColor=white
[Ko-Fi-url]: https://ko-fi.com/G2G3OAILE
