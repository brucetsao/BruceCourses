
DrillMotorMountBack();

module DrillMotorMountBack()
{
	difference()
	{
		union()
		{
			// body mount
			translate([0,13,24/2])
				cube([48,26,24],center=true);
			translate([0,0,12])
				cylinder(r=48/2,h=24, center=true);

			// mounting tab
			translate([0,24,24/2])
				cube([76.5,5,24],center=true);

			translate([24,10,3/2]) rotate([0,0,-28])
				cube([14,34,3],center=true);
			translate([-24,10,3/2]) rotate([0,0,28])
				cube([14,34,3],center=true);
		}

		// through hole
		translate([0,0,10])
			cylinder(r=38.5/2,h=30, center=true, $fn=100);

		// mounting holes on tabs
		translate([31,20,24/2]) rotate([90,0,0])
			cylinder(r=2.8,h=20, center=true);
		translate([-31,20,24/2]) rotate([90,0,0])
			cylinder(r=2.8,h=20, center=true);

		// cut off excess
		translate([0,24+5,24/2])
			cube([80,5.1,26],center=true);

		translate([39,24,22]) rotate([0,36,0])
			cube([30,5.2,10],center=true);
		translate([-39,24,22]) rotate([0,-36,0])
			cube([30,5.2,10],center=true);
	}
}
