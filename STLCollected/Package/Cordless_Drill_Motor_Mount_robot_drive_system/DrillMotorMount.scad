
DrillMotorMount();

module DrillMotorMount()
{
	difference()
	{
		union()
		{
			difference()
			{
				union()
				{
					// body mount
					translate([0,13,24/2])
						cube([40,26,24],center=true);
					translate([0,0,12])
						cylinder(r=40/2,h=24, center=true);

					// mounting tab
					translate([0,24,24/2])
						cube([68.5,5,24],center=true);

					translate([20,10,3/2]) rotate([0,0,-28])
						cube([14,34,3],center=true);
					translate([-20,10,3/2]) rotate([0,0,28])
						cube([14,34,3],center=true);
				}

				// center hole
				cylinder(r=15/2,h=20, center=true);

				// mounting holes
				translate([24/2,0,0])
					cylinder(r=3.8/2,h=20, center=true);
				translate([-24/2,0,0])
					cylinder(r=3.8/2,h=20, center=true);

				translate([0,0,2.5+1.8])
					cube([30,8,5],center=true);

				// body mount cutout
				translate([0,0,15])
					cylinder(r=30.5/2,h=20, center=true, $fn=100);

				// mounting holes on tabs
				translate([27,20,24/2]) rotate([90,0,0])
					cylinder(r=2.8,h=20, center=true);
				translate([-27,20,24/2]) rotate([90,0,0])
					cylinder(r=2.8,h=20, center=true);

				// cut off excess
				translate([0,24+5,24/2])
					cube([70,5.1,26],center=true);

				translate([35,24,22]) rotate([0,36,0])
					cube([30,5.2,10],center=true);
				translate([-35,24,22]) rotate([0,-36,0])
					cube([30,5.2,10],center=true);
			}

			// keyed body mount
			translate([0,24.5/2+5/2,14])
				cube([20,5,20],center=true);
			translate([0,-24.5/2-5/2,14])
				cube([20,5,20],center=true);
		}

		// keyed body mount
		translate([-3,-24.5/2,16])
			cylinder(r=4/2,h=22, center=true, $fn=30);
	}
}
