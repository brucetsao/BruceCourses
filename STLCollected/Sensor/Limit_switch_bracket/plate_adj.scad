// MakerSlide limit switch mounting plate. Measurments adjusted for
// clearance on one particular printer. YMMV.

module switch()
{
    difference() {
        union() {
            cube([21, 10, 6.5]);
            cube([10.6, 11, 6.5]);
        }
        translate([0, 3.0, -1])
            union() {
                translate([5.4, 0, 0])
                    cylinder(d=1.5, h=10);
                translate([15.8, 0, 0])
                    cylinder(d=1.5, h=10);
            }
    }
    translate([2.7, 11.5, 0]) {
        union() {
            cube([16.2, 0.3, 4.3]);
            translate([14, 3.2, 0])
                cylinder(d=5.0, h=3.0);
        }
    }
}


module plate(thickness=4.0, width=39.0, height=15.0)
{
    difference() {
        union() {
            // Main plate
            cube([width, height, thickness]);
            // Retention body
            translate([width-24, height-10])
                difference() {
                    cube([24, 10, thickness+8]);
                    translate([2, 0, thickness+2])
                        cube([20, 10, thickness+8]);
                }
        }
        // Screw hole.
        translate([10, height/2, -0.1])
            cylinder(d=6.5, h=thickness+0.2);
        // Switch
        translate([width-22.5, height-10, thickness])
            switch();
    }
}


plate();


// Test fit into extrusion.
/*
translate([0, 50, 0]) rotate([90, 0, 0])
    linear_extrude(height=50, convexity=10)
        scale([25.4, 25.4, 25.4])
            import(file="b17022_rev_2_recenter.dxf");
*/
