#include "NRL.h"

// Author: Dhruv (lostluffyz)
// BasicDrive (TELEOP) - a minimal public example: tank drive + one arm servo.
// Anyone can copy this file and have a driving robot in minutes.
//
// Mapping:
//   Left stick Y  = drive forward / back
//   Right stick X = turn left / right
//   D-pad UP      = arm up a little (one press = 5 degrees)
//   D-pad DOWN    = arm down a little (one press = 5 degrees)
//   X             = arm to the middle (90 degrees)

static HexaDCMotor leftMotor{{ .dirPin = MOTOR_L_DIR, .pwmPin = MOTOR_L_PWM }};
static HexaDCMotor rightMotor{{ .dirPin = MOTOR_R_DIR, .pwmPin = MOTOR_R_PWM, .flipped = true }};
static HexaServo arm{{ .signalPin = SERVO_1, .startAngle = 90.0f }};
static TankDrive drive(leftMotor, rightMotor);

class BasicDrive : public NRLOpMode {
public:
    void init() override {
        // Runs once when INIT is pressed. Start the hardware here.
        leftMotor.begin();
        rightMotor.begin();
        arm.begin();
    }

    void loop() override {
        // Runs at 100 Hz until STOP.

        // Drive. If pushing the stick up drives you backwards, put a
        // minus sign in front of gamepad1.leftY(). Same idea for
        // turning with gamepad1.rightX().
        drive.drive(gamepad1.leftY(), gamepad1.rightX());

        // Arm: read where it is now, step it, keep it inside 0-180.
        float angle = arm.getPosition();
        if (gamepad1.justPressed(BTN_DPAD_UP)) {
            angle += 5.0f;  // one press = 5 degrees up
        }
        if (gamepad1.justPressed(BTN_DPAD_DOWN)) {
            angle -= 5.0f;  // one press = 5 degrees down
        }
        if (gamepad1.justPressed(BTN_X)) {
            angle = 90.0f;  // middle
        }
        if (angle < 0.0f) {
            angle = 0.0f;
        }
        if (angle > 180.0f) {
            angle = 180.0f;
        }
        arm.setPosition(angle);

        // Show the arm angle on telemetry so you can see it move.
        telemetry.addData("arm", angle);
    }

    void stop() override {
        // Runs once on STOP. Always stop everything here.
        drive.stop();
        arm.detach();
    }
};

REGISTER_OPMODE(BasicDrive, "BasicDrive", TELEOP);
