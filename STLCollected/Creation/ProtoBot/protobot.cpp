/**********************************************************************
 *
 * This code is part of the OOML project
 * Authors: Alberto Valero-Gomez, Nieves Cubo, Mario Almagro
 *
 * OOML is licenced under the Common Creative License,
 * Attribution-ShareAlike 3.0
 *
 * You are free:
 *   - to Share - to copy, distribute and transmit the work
 *   - to Remix - to adapt the work
 *
 * Under the following conditions:
 *   - Attribution. You must attribute the work in the manner specified
 *     by the author or licensor (but not in any way that suggests that
 *     they endorse you or your use of the work).
 *   - Share Alike. If you alter, transform, or build upon this work,
 *     you may distribute the resulting work only under the same or
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs or
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 **********************************************************************/



#include <components/Cube.h>
#include <components/Cylinder.h>
#include <core/IndentWriter.h>
#include <core/Difference.h>
#include <core/Union.h>
#include <core/Intersection.h>
#include <components/RoundedTablet.h>
#include <components/Toroid.h>
#include <parts/FutabaS3003.h>
#include <parts/ArduinoUNO.h>
#include <parts/NineVoltBattery.h>
#include <parts/ServoWheel.h>
#include <parts/BallWheelHolder.h>

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
	 IndentWriter masterbot_writer;
	 IndentWriter wheel_writer;
	 IndentWriter spacer_writer;



///////////////////////////////////////////////////////////////
//****************    PROTOBOT BODY   ***********************/
///////////////////////////////////////////////////////////////

//ARDUINO UNO

	 // Use ArduinoUNO part.
	 Component arduino = ArduinoUNO(true,10,false);
	 arduino.rotate(0,180,90).translate(53.5,77,0);

//MAIN BODY

	 // Use the difference between body and body_hole.
	 Component body = RoundedTablet(54.5,98,22.5,10,true,true,true,true,100,false);
	 Component body_hole = RoundedTablet(44.5,88,30,6,true,true,true,true,100,false);
	 body_hole.translate(5,5,3);

         // Arduino needs to extend the main body for the two upper holes. This is achieved by using two cylinders.
	 Component ardu_support1 = Cylinder::create(5,3,50,false).translate(2.5,61.5,0)
				+ Cylinder::create(3.5,6,50,false).translate(2.5,61.5,0);
	 Component ardu_support2 = Cylinder::create(5,3,50,false).translate(51,62.5,0)
				+Cylinder::create(3.5,6,50,false).translate(51,62.5,0);

	 Component ardu_support = /*ardu_support1 +*/ ardu_support2;



	 // In order to save material and time, some parts are removed from our base.
	 Component removedPart1 = RoundedTablet(15,6,6,3,true,true,true,true,6,false).translate(22.2,5,0);
	 Component removedPart2 = RoundedTablet(44.5,51,6,6,true,true,true,true,100,false).translate(5,15,0);
	 Component removedPart3 = RoundedTablet(44.5,20,6,6,true,true,true,true,100,false).translate(5,70,0);
	 Component removedParts = removedPart1 + removedPart2 + removedPart3 ;

	 Component body_front_hole = RoundedTablet(37.5,44.5,10,6,true,true,true,true,100,false).rotate(90,0,0).translate(8.5,6,6);

	 // Protoboard will be above arduino, and it's linked to our design through that hole and one groove.
	 Component proto_hole = Cylinder::create(1.5,5,10,false).translate(27.25,98,-1);

	 //SERVOS
	 // Use FutabaS3003 part.
	 Component l_servo = FutabaS3003(true,false);
	 l_servo.rotate(0,-90,0);

	 // Create a mirrored copy FutabaS3003 in x direction.
	 Component r_servo = l_servo.mirroredCopy(1,0,0);
	 r_servo.translate(1,0,0);

	 // Translate both servos.
	 Component servos = l_servo + r_servo;
	 servos.translate(26.75,17,3);


//FREE WHEEL

	 // Create the part which contains a marble.
	 Component free_wheel = BallWheelHolder::create(38.5);
	 free_wheel.translate(27.25,98,0);
	 Component wheel_hole = Cylinder::create(7,40,65,false).translate(27.25,98,6);
	 Component wheel_nut = Cylinder::create(3.6,3.1,6,false).translate(27.25,98,3);

//BATTERY SUPPORT

	 double sz = 45.30;
	 double connector_r = 5;
	 double connector_lenght = 3.20;
	 double connect_width = 22;

         // Use NineVoltBattery part.
	 Component battery = NineVoltBattery();
	 Component connector_hole = RoundedTablet(connect_width+1 , 2*connector_r+1 , 20 , 2*connector_r+1);
	 connector_hole.translate( 0 , 0 , sz/2 + connector_lenght/2);

	 Component battery_hole = battery+connector_hole;

	 battery.rotate(0,90,0);
	 battery.translate(28.25,79.5,16);
	 battery_hole.rotate(0,90,0);
	 battery_hole.translate(28.25,79,16);

	 // Build a wall to hold the battery.
	 Component wall = Cube::create(54.5,5,22.5,false);
	 Component wall_hole = RoundedTablet(33.5,44.5,10,6,true,true,true,true,100,false).rotate(90,0,0).translate(10.5,6,3);
	 wall = wall - wall_hole;
	 wall.translate(0,65.5,0);

// FULL ROBOT

	 // body is made up by a support, the removed parts, the protoboard's hole, the arduino's support and the groove for the protoboard.
	 body = body
				- body_hole
				- body_front_hole
				- removedParts
				+ ardu_support
				+ free_wheel
				+ wall
				- proto_hole
				- servos
				- battery_hole.translate(-1,0,0)
				- wheel_hole
				- wheel_nut
				- arduino;

	 ///////////////////////////////////////////////////////////////
	 //****************    PROTOBOT WHEELS ***********************/
	 ///////////////////////////////////////////////////////////////

	 Component wheel = ServoWheel(28,4.2,6,10.7,2);
	 Component oring = Toroid(28,1.3).translate(0,0,3);

	 Component servo_wheel_hole = Cylinder::create(22,6.2,100,false)
				- Cylinder::create(12,6.3,100,false);
	 servo_wheel_hole.translate(0,0,-0.1);

	 Component radius1 = Cube::create(15,8,3,true);
	 radius1.translate(18.5,0,1.5);
	 Component radius2 = Cube::create(15,3,6,true);
	 radius2.translate(18.5,0,3);

	 Component one_radius = radius1 + radius2;

	 Component radius = Union::create();


	 for (int i=0;i<3;i++){
		  radius = radius + one_radius.rotatedCopy(0,0,120*i);
	 }


	 wheel = wheel - oring - servo_wheel_hole + radius;

	 ///////////////////////////////////////////////////////////////
	 //****************    PROTOBOT SPACERS ***********************/
	 ///////////////////////////////////////////////////////////////

	 Component small_spacer = Cylinder::create(5,7,30,false)
				- Cylinder::create(2,12,30,false).translate(0,0,-1);

	 Component big_spacer = Cylinder::create(7,3,100,false)
				+ Cylinder::create(5,14,30,false)
				+ Cylinder::create(5,10,12,100,false).translate(0,0,13.9)
				- Cylinder::create(1.5,30,30,false).translate(0,0,-1);

	 Component spacers = Union::create();

	 for (int i=0;i<3;i++){
		  spacers = spacers + small_spacer.translatedCopy(0,13*i,0);
	 }

	 spacers.translate(25,-10,0);
	 spacers = spacers + big_spacer;


	 //GENERATE OPENSCAD CODE

	 //Output file which contains the OpenSCAD code.
	 ofstream os;

	 //ProtoBot Body
	 os.open("protobot.scad");
	 if(os){
		  masterbot_writer << body;
		  os << masterbot_writer;
		  os.close();
	 }else{
		  cerr << "Error opening protobot.scad file";
	 }

	 //ProtoBot Wheel
	 os.open("wheel.scad");
	 if(os){
		  wheel_writer << wheel;
		  os << wheel_writer;
		  os.close();
	 }else{
		  cerr << "Error opening wheel.scad file";
	 }

	 //ProtoBot Spacers
	 os.open("spacers.scad");
	 if(os){
		  spacer_writer << spacers;
		  os << spacer_writer;
		  os.close();
	 }else{
		  cerr << "Error opening spacers.scad file";
	 }
}
