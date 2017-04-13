ballDiameter = 16.5;
holeSize = 3;
wallThickness = 2.5;
gapWidth = 5;
baseThickness = 3;

baseHeight = ballDiameter+wallThickness*2;
baseWidth = wallThickness*4+ballDiameter;

module mainShape(){
	translate([-baseWidth/2, -wallThickness-holeSize/2]) cube([baseWidth, wallThickness*2+holeSize, baseThickness]);
	
	translate([baseHeight/2+wallThickness,0,0]) cylinder(baseThickness, holeSize/2+wallThickness,holeSize/2+wallThickness,$fn=40);
	translate([-baseHeight/2-wallThickness,0,0]) cylinder(baseThickness, holeSize/2+wallThickness,holeSize/2+wallThickness,$fn=40);

	cylinder(baseThickness+ballDiameter*0.7,baseHeight/2, baseHeight/2, $fn=50);
}

module cutout(){
	translate([0,0,ballDiameter/2+holeSize]) sphere(ballDiameter/2, $fn=50);
	translate([-baseHeight/2-2,-gapWidth/2,baseThickness]) cube([baseHeight+4,gapWidth,ballDiameter]);
	translate([-baseHeight/2-2,0,baseThickness+ballDiameter*0.6]) rotate([0,90]) cylinder(baseHeight+4,ballDiameter*0.3,ballDiameter*0.3,$fn=6);
	translate([0,0,baseThickness]) cylinder(ballDiameter/4, ballDiameter/2, ballDiameter/2, $fn=40);
}

module mountingHoles(){
	translate([ballDiameter/2+wallThickness*2,0,-1]) cylinder(baseThickness+2, holeSize/2, holeSize/2, $fn=30);
	translate([-(ballDiameter/2+wallThickness*2),0,-1]) cylinder(baseThickness+2, holeSize/2, holeSize/2, $fn=30);
}

difference(){
	mainShape();
	cutout();
	mountingHoles();
}