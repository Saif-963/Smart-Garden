# Smart-Garden
Developed a smart garden automation system using Arduino that integrates security, environment monitoring, and automatic control featuresThe system utilizes multiple sensors and actuators, including an IR sensor, LDR, LM35 temperature sensor, servo motor, fan, and LEDs, to manage garden access and environmental conditions.

Key Features:
Access Control: Uses a 4x3 keypad for password authentication. Motion detection via an IR sensor triggers the garden door to open. Only users with the correct password can fully access the system.

LCD Display: A 16x2 LCD provides real-time feedback, displaying temperature, lighting level, fan status, and access notifications.

Environmental Monitoring:

LM35 sensor reads the ambient temperature.

LDR sensor detects light intensity.

Actuator Control:

Servo motor opens and closes the garden gate based on motion detection.

Fan is activated when temperature exceeds 25°C and flashes a warning LED above 35°C.

LED lighting is automatically dimmed or brightened depending on ambient light.

Security Alerts: Wrong password attempts trigger an alert by blinking an LED.
