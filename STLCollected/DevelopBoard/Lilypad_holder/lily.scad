
module lily(){
difference(){
union(){
translate([0,0,-2.4])
cylinder(h=1, r=43, center=true,$fn=100);
cylinder(h=4., r=30, center=true, $fn=100);
for ( i = [0 : 22] ){
rotate( i * 360 / 22, [0, 0, 1])
translate([0, 38, -1.4])
cylinder(h=1.5, r=4.5, center=true, $fn=100);
}
}
color([1,0,0])
translate([0,0,1.3])
cylinder(h=1.5, r=25.5, center=true, $fn=100);
translate([12,0,0])
cylinder(h=1.5, r=5, center=true, $fn=100);
for ( i = [0 : 22] ){
rotate( i * 360 / 22, [0, 0, 1])
translate([0, 21.5, -1])
cylinder(h=10, r=1.2, center=true, $fn=100);
}
for ( i = [0 : 22] ){
rotate( i * 360 / 22, [0, 0, 1])
translate([0, 32,-1])
cylinder(h=5, r=1, center=true, $fn=100);
}
}
}
lily();






