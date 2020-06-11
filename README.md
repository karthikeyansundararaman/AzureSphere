# AzureSphere
AzureSphere powered SUPER CLEANING BOT

Problem Statement

The Main objective of this project is to evaluate Avnet Azure Sphere MT3620 development kit by utilizing most of its available GPIOs/I2C terminals without using an another slave board like Arduino, etc., for mounting sensors or for handling motor drivers. This thought of mine also favours a super access to  features like (i) Defense in depth (ii) Deployment flexibility (iii) OTA updates (iv) Automatic security updates as well-documented by the Manufacturer. Hence a 100% compatibility towards over-the-air update in the form of cloud deployment by making the board talk with Azure Cloud Platform ensures easy update from a remote location. An unique feature of Azure sphere complying with richest security standards in the industry is also a critical requirement for the prevailing security concerns around IoT deployments. To support my thought of developing AzureSphere-Only Master Board- based project, a BOT is proposed capable of performing floor sanitizing using UV LEDs by sensing the objects around with its Ultrasonic sensors. Its named “SUPER CLEAN BOT” and let me craft its storyboard now. 

Home Hygiene Assist
SUPER CLEAN BOT as it sounds is a cleaning bot designed with a primary focus on self hygiene due to the recent outbreak. It performs the process of Home Floor Sanitizing by illuminating the UV LEDs over the floor for a pre-defined period of time and this process is aided by ultrasonic sensors mounted on the BOT sides in all four directions to sense the object around for its obstruct-free movement.  Though it is a demonstration of Proof-of-Concept, the exact benefits of using the UV light in the way as proposed is to be ascertained separately. Also necessary precautions to be taken while handling UV LEDs and always remain aware of harmful exposures to UV radiations. Few advisories from WHO and other resources shall be refereed before proceeding further 
Reference1: https://www.who.int/health-topics/ultraviolet-radiation#tab=tab_1
Reference2: https://www.who.int/uv/publications/en/occupational_risk.pdf

SUPER CLEAN BOT
Construction
A simple architecture of the bot is shown below which includes the following components
1)	BOT’s brain behind
a)	Azure Sphere MT3620
 
2)	BOT’s taste buds - External Sensing
a)	Ultrasonic Sensors
b)	Avnet Azure Sphere – Handy OnBoard Sensors – Accelerometer, Gyroscope, Temperature, Light Intensity sensors
 
3)	BOT’s Movement Aid
a)	4 Wheeled Robotic chassis driven by  H-Bridge based driver board

4)	BOT’s  Hunger Boost
a)	Power Source 1: 
5V USB source from a mobile battery bank is used to power the MT3620 through its micro USB port.
	
b)	Power Source 2:
7-12V Li-Po used exclusively to power hungry motors.

5)	BOT’s Human Interface
a)	128 x 64 OLED Display
b)	Azure IoT Dashboard (Work-In-Progress)*

6)	BOT’s Assignment
a)	UV LED mount plate with 5mm LED (3Nos)


 
Working Demo: https://youtu.be/88mr-1gAgaQ
To enable AutoMode – Press User Button A & Button B together. Sanitizing process starts with default period of 60seconds. To restart press “Reset” button.


Conclusion:
Though its still a work in progress, many fine tuning is required in the areas of algorithm, offloading the sensor workload to M4 processors, connecting to cloud and provisioning remote sideloading. Will keep you updated.




