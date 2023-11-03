#include "main.h"
#include "pros/misc.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Good luck, have fun!");

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
	pros::Motor left_mtr1(11,true);
	pros::Motor left_mtr2(22,true);
	pros::Motor right_mtr1(19,true);
	pros::Motor right_mtr2(20,true);
	pros::Motor wings1(2,true);
	pros::Motor wings2(3,true);


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
	pros::Motor left_mtr1(11,true);
	pros::Motor left_mtr2(12,true);
	pros::Motor right_mtr1(19,false);
	pros::Motor right_mtr2(20,false);
	pros::Motor flystick(4,false);
	pros::Motor launcher(1,	false);
	pros::Motor wings1(2, true);
	pros::Motor wings2(3,false);


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y); //joystick
		int right = master.get_analog(ANALOG_RIGHT_Y); //joystick

		//wings1
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			wings1.move(100);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			wings1.move(-100);
		} else {
			wings1.brake();
		}

		//wings2
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			wings2.move(100);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			wings2.move(-100);
		} else {
			wings2.brake();
		}

		//FLYWHEEL
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
			launcher.move(128);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			launcher.move(100);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
			launcher.move(80);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
			launcher.move(60);
		} else {
			launcher.brake();
		}


		//flystick
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
			flystick.move(100);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
			flystick.move(-100);
		} else {
			flystick.brake();
		}
		

		if(left < 10 && left > -10){
			left = 0;
		}

		left_mtr1.move(left);
		left_mtr2.move(left);


		if(right < 10 && right > -10){
			right = 0;
		}
		right_mtr1.move(right);
		right_mtr2.move(right);

		pros::delay(20);
	}
}
