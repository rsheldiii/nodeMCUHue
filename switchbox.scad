
thickness = [4, 4, 4];
total_size = [14,14,14] * 4 + thickness;


switch_hole = [14,14,2] * 4;

module box() {
    difference() {
        cube(total_size);
        translate(thickness / 2) cube(total_size - thickness + [0,0,0.1 + 2]);
        translate([-0.01,25.145,2.66]) cube([thickness[0] / 2 + .02, 9.9065, 6.3175]);
    }

    translate([25.547, 30.2309, 3.426]) import("node_mcu_holder_square.stl");
}

module plate() {
    difference() {
        translate([-3.2, -3.2]) cube([total_size[0] + 6.4, total_size[1] + 6.4, 12.8]);

        translate([0,0,5.6]) cube(total_size + [0.3,0.3,0.3]);
        //translate(thickness / 2) cube(total_size - thickness + [0,0,0.1 + 2]);
        //translate([-0.01,25.145,2.66]) cube([thickness[0] / 2 + .02, 9.9065, 6.3175]);
    }
}

box();