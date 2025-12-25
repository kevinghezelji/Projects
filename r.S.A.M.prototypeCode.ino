#include <Servo.h>

bool demoRunning = false; // Globals
int currentWeek = 24;
int sweepHour, sweepMinute;
int panelAngle, panelAngleMirror;
int baseAngleMain, baseAngleMirror;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 3000;
const float LATTITUDE = 45.42;

#define baseMotorPin            3    // Base Servo pin definition 1
#define baseMotorMirrorPin      5    // Base Servo pin definition 2
#define panelMotorPin           6    // Panel Servo pin definition 1
#define panelMotorMirrorPin     9    // Panel Servo pin definition 2
#define voltageSensorPin        A0   // Voltage read pin definition

// R1 = 47k, R2 = 33k
const float R1 = 47000.0; // Voltage Divider Constants
const float R2 = 33000.0;
const float voltageScale = (R1 + R2) / R2; // Comes out to roughly 2.4

const int baseOffsetMain = -10; // Offset needed due to Servo's being out of sync
const int baseOffsetMirror = 0;

Servo baseMotor;        // Servo Objects
Servo baseMotorMirror;
Servo panelMotor;
Servo panelMotorMirror;

const float weeklyDeclination[] = // Solar Declination (In Degrees) for the 52 weeks of 2025
{
 -23.0, -22.3, -21.5, -20.5, -19.3, -18.0, -16.5, -14.9, -13.1, -11.2, -9.2, -7.1, -5.0, -2.8, -0.6, 1.7,
  4.0, 6.3, 8.6, 10.8, 13.0, 15.1, 17.1, 19.0, 20.8, 22.4, 23.1, 23.4, 23.4, 23.1, 22.4, 21.5, 20.3, 18.9, 17.3, 15.6,
  13.7, 11.8, 9.7, 7.6, 5.4, 3.1, 0.9, -1.4, -3.7, -6.0, -8.3, -10.5, -12.7, -14.8, -16.8, -18.7
 };

const float weeklySunrise[] = // Surise hours (Military Time/24 hour clock) for 52 weeks of 2025
{
  7.67, 7.58, 7.42, 7.25, 7.00, 6.75, 6.42, 6.08, 6.75, 6.42, 6.08, 5.75, 5.42, 5.17, 4.92, 4.75,
  4.58, 4.50, 4.42, 4.42, 4.42, 4.50, 4.58, 4.67, 4.83, 5.00, 5.17, 5.33, 5.50, 5.67, 5.83, 6.00,
  6.17, 6.33, 6.50, 6.67, 6.83, 7.00, 7.17, 7.33, 6.50, 6.75, 7.00, 7.17, 7.33, 7.50, 7.58, 7.67,
  7.67, 7.75, 7.75, 7.75
};

const float weeklySunset[] = // Sunset hours (Military Time/24 hour clock) for 52 weeks of 2025
{
    16.67, 16.92, 17.17, 17.50, 17.75, 18.08, 18.42, 18.75, 19.08, 19.42, 19.75, 20.00, 20.25, 20.42, 20.58, 20.67,
    20.75, 20.75, 20.75, 20.67, 20.58, 20.50, 20.33, 20.17, 20.00, 19.75, 19.50, 19.25, 19.00, 18.75, 18.50, 18.25,
    18.00, 17.75, 17.50, 17.25, 17.00, 16.75, 16.58, 16.42, 17.33, 17.17, 17.00, 16.92, 16.83, 16.75, 16.67, 16.67,
    16.67, 16.67, 16.75, 16.75
};

// Forward declarations
int calculateHorizontalAngle(int week, int hour, int minute);
void setBaseMotors(int sunAngle);
void calculateAndSetElevation(int week, int hour, int minute);

void updateSolarPosition(int week, int hour, int minute)
{
    int horizontalAngle = calculateHorizontalAngle(week, hour, minute);    // Horizontal Motion (Base Rotation)
    setBaseMotors(horizontalAngle);
    calculateAndSetElevation(week, hour, minute);    // Vertical Motion (Panel Tilt)
    int adcValue = analogRead(voltageSensorPin);        // Sensor reading
    float voltage = (adcValue / 1023.0) * 5.0 * voltageScale;    // Convert raw data into voltage value

    Serial.print("Week ");    // Print to screen for DEMO users to see
    Serial.print(week + 1);
    Serial.print(" | ");
    if (hour < 10) Serial.print("0");
    Serial.print(hour);
    Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.print(minute);
    Serial.print(" | Base Rotation:");
    Serial.print(horizontalAngle);
    Serial.print(" | Panel Tilt:");
    Serial.print(panelAngle);
    Serial.print(" | Voltage:");
    Serial.print(voltage, 2);
    Serial.println("V");
}

void setBaseMotors(int sunAngle)
{
    baseAngleMain = map(sunAngle, 0, 180, 30, 150) + baseOffsetMain;    // follows sun path from 0-180
    baseAngleMirror = map(sunAngle, 0, 180, 150, 30) + baseOffsetMirror;    // Mirrored

    baseAngleMain = constrain(baseAngleMain, 30, 150);        // Safety constraints (30 degree buffer)
    baseAngleMirror = constrain(baseAngleMirror, 30, 150);

    baseMotor.write(baseAngleMain);                 // Pass information to Servos
    baseMotorMirror.write(baseAngleMirror);
}

int calculateHorizontalAngle(int week, int hour, int minute)
{
    float sunrise = weeklySunrise[week];
    float sunset = weeklySunset[week];

    int sunriseMinute = (int)(sunrise * 60); // Conversion from hour to minutes
    int sunsetMinute = (int)(sunset * 60);
    int currentMinute = hour * 60 + minute;

    int totalDaylightMinutes = sunsetMinute - sunriseMinute;    // Safety mechanism to stay at 0 during 0 sunlight hours
    int elapsedMinutes = currentMinute - sunriseMinute;

    if (elapsedMinutes < 0) elapsedMinutes = 0;
    if (elapsedMinutes > totalDaylightMinutes) elapsedMinutes = totalDaylightMinutes; // Find percentage of day that has passed

    float fractionOfDay = (float)elapsedMinutes / (float)totalDaylightMinutes; // Convert percentage into angle

    int horizontalAngle = (int)(fractionOfDay * 180.0);
    return horizontalAngle;    // Return angle
}

void calculateAndSetElevation(int week, int hour, int minute)
{
    float sunrise = weeklySunrise[week];
    float sunset = weeklySunset[week];

    int sunriseMinute = (int)(sunrise * 60); // Conversion from hour to minutes
    int sunsetMinute = (int)(sunset * 60);
    int currentMinute = hour * 60 + minute;

    if (currentMinute < sunriseMinute || currentMinute > sunsetMinute) // Safety mechanism for nighttime
    {
        panelAngle = 0;                // Set panel flat to clear debris
        panelAngleMirror = 180;
        panelMotor.write(panelAngle);
        panelMotorMirror.write(panelAngleMirror);
        return;
    }

    int solarNoonMinute = (sunriseMinute + sunsetMinute) / 2;                        // Degrees moved since noon
    float hourAngle = ((float)(currentMinute - solarNoonMinute) / 60.0) * 15.0;

    float latRadians = LATTITUDE * PI / 180.0;           // Radian conversion for Trig operations
    float decRadians = weeklyDeclination[week] * PI / 180.0;
    float hourRadians = hourAngle * PI / 180.0;
    float sinElevation = sin(latRadians) * sin(decRadians) + cos(latRadians) * cos(decRadians) * cos(hourRadians);

    sinElevation = constrain(sinElevation, -1.0, 1.0);        // Hold within domain and range of sin
    float elevationDegree = asin(sinElevation) * 180.0 / PI;    // Convert back to degrees

    float boostedElevation = constrain(elevationDegree * 1.2, 0, 90);    // Boost movement for Demo

    const int panelMinAngle = 10;        // Set safety buffers
    const int panelMaxAngle = 80;
    panelAngle = map((int)boostedElevation, 0, 90, panelMinAngle, panelMaxAngle);

    panelAngleMirror = 180 - panelAngle;    // Mirror calculations
    panelMotor.write(panelAngle);            // Pass information to Servos
    panelMotorMirror.write(panelAngleMirror);
}

void setup()
{
    Serial.begin(9600); // Open comms

    baseMotor.attach(baseMotorPin);           // Attach all Servo motors
    baseMotorMirror.attach(baseMotorMirrorPin);
    panelMotor.attach(panelMotorPin);
    panelMotorMirror.attach(panelMotorMirrorPin);

    baseAngleMain = 90;    // Safe Start postions to ensure no damage to frame (In Degrees)
    baseAngleMirror = 90;
    panelAngle = 20;
    panelAngleMirror = 160; // Panels mirrored, -20 from each axis 0-180

    Serial.println("Weekly Solar Tracker DEMO"); // Print statement
}

void loop()
{
    if (Serial.available() > 0) // Checks for Serial Commands from python app
    {
        char cmd = Serial.read();
        if (cmd == '\n' || cmd == '\r' || cmd == 'S' || cmd == 's') // Commands from user
        {
            demoRunning = !demoRunning; // Toggles ON/OFF
            if (demoRunning)            // Check if DEMO is on
            {
                Serial.println("DEMO Started");
                currentWeek = 24;                                        // Random Week Chosen for Demo
                sweepHour = (int)weeklySunrise[currentWeek];             // Set Time for that week
                sweepMinute = (int)((weeklySunrise[currentWeek] - sweepHour) * 60);
                lastUpdate = millis();    // Reset Timer
            }
        }
    }
    if (demoRunning) // Check for DEMO ON/OFF
    {
        if (millis() - lastUpdate >= updateInterval)    // Update Interval
        {
            lastUpdate = millis();    // Reset Timer
            sweepMinute += 15;        // Advances simulation by 15 minutes
            if (sweepMinute >= 60)
            {
                sweepMinute -= 60;
                sweepHour++;
            }
            float sunset = weeklySunset[currentWeek];        // Check to see if simulation has reached sunset
            int sunsetHour = (int)sunset;
            int sunsetMinuteTime = (int)((sunset - sunsetHour) * 60);

            if (sweepHour > sunsetHour || (sweepHour == sunsetHour && sweepMinute > sunsetMinuteTime))  // Checks for sunset
            {
                currentWeek = (currentWeek + 1) % 52;            // Advances to next week
                sweepHour = (int)weeklySunrise[currentWeek];
                sweepMinute = (int)((weeklySunrise[currentWeek] - sweepHour) * 60);
            }
            updateSolarPosition(currentWeek, sweepHour, sweepMinute); // Move/Update Motors
        }
    }
}