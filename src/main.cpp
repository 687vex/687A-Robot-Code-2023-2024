#include "main.h"
#include <iostream>
#include <ostream>
#include <cmath>
#include "pros/misc.h"

pros::Motor right_mtr1(1);
pros::Motor right_mtr2(2);
pros::Motor right_mtr3(3);

pros::Motor left_mtr1(8);
pros::Motor left_mtr2(9);
pros::Motor left_mtr3(10);

pros::Motor_Group motor_leftsidegroup ({left_mtr1, left_mtr2, left_mtr3});
pros::Motor_Group motor_rightsidegroup ({right_mtr1, right_mtr2, right_mtr3});

void extendWings(bool state){
	pros::ADIDigitalOut pistonLeft (11, state);
	pros::ADIDigitalOut pistonRight (12, state);
	pistonLeft.set_value(state);
	pistonRight.set_value(state);

}
void retractWings(bool state){
	pros::ADIDigitalOut pistonLeft (11, state);
	pros::ADIDigitalOut pistonRight (12, state);
	pistonRight.set_value(state);
	pistonLeft.set_value(state);

}

void startDrive(int i) {
	motor_leftsidegroup.move_voltage(12000);
	motor_rightsidegroup.move_voltage(12000);
	pros::delay(i);
}
/*
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "HELLO!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it`
 * from where it left off.
 */
void autonomous() {
	// pros::Motor left_mtr1(11,true);
	// pros::Motor left_mtr2(22,true);
	// pros::Motor right_mtr1(19,true);
	// pros::Motor right_mtr2(20,true);
	// pros::Motor wings1(2,true);
	// pros::Motor wings2(3,true);

	startDrive(5);


}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	//pros::Motor left_mtr1(1,true);
	//pros::Motor left_mtr2(2,false);
	//pros::Motor left_mtr3(3,true);
	//pros::Motor right_mtr1(8,false);
	//pros::Motor right_mtr2(9,true);
	//pros::Motor right_mtr3(10,false);
	pros::Motor flystick(4,false);
	pros::Motor intake(5, false);


	while (true) {
		int left = master.get_analog(ANALOG_LEFT_Y); //joystick
		int right = master.get_analog(ANALOG_RIGHT_Y); //joystick

		//intake
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			intake.move(100);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			intake.move(-100);
		} else {
			intake.brake();
		}

		//FLYWHEEL
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
			flystick.move(128);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			flystick.move(100);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
			flystick.move(80);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
			flystick.move(60);
		} else {
			flystick.brake();
		}

		//wings
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			extendWings(true);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			retractWings(false);
		}

		if(left < 10 && left > -10){
			left = 0;
		}

		motor_leftsidegroup.move(left);
		//left_mtr1.move(left);
		//left_mtr2.move(left);
		//left_mtr3.move(left);

		if(right < 10 && right > -10){
			right = 0;
		}

		motor_rightsidegroup.move(right);
		//right_mtr1.move(right);
		//right_mtr2.move(right);
		//right_mtr3.move(right);

		pros::delay(20);
	}
}

