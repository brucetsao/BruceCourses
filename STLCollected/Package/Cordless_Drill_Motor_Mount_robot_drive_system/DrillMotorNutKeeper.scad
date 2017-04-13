
DrillMotorNutKeeper();

module DrillMotorNutKeeper()
{
	difference()
	{
		union()
		{
			translate([0,0,12/2])
				cylinder(r=21/2,h=12, center=true);
		}

		translate([0,0,15])
			cylinder(r=18/2,h=20, center=true, $fn=6);

		translate([0,0,5])
			cylinder(r=6/2,h=20, center=true, $fn=40);
	}
}
