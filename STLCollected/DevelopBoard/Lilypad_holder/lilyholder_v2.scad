use <lily.scad>
difference(){
lily();
for ( i = [0 : 22] ){
rotate( i * 360 / 22, [0, 0, 1])
translate([0, 38, -1])
cylinder(h=1, r=3.1, center=true, $fn=128);
}
for ( i = [0 : 22] ){
rotate( i * 360 / 22, [0, 0, 1])
translate([0, 38, -2])
cylinder(h=4, r=1.8, center=true, $fn=128);
}
}
