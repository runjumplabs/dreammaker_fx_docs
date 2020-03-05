# Debugging Sketches
------

There are a few different resources that are available for debugging a sketch that isn't working as intended.

When initializing a sketch, you can enable debug by calling `pedal.init(true)` instead of `pedal.init()`.  This will put the pedal in
"debug mode" which means it will send information to the Serial Monitor (Tools->Serial Monitor).  Open the Serial Monitor before downloading your sketch.  Once the sketch is running, you'll see status info and any errors that were encountered while processing the routing information.

``` C
// Include our library of effects routines
#include <dreammakerfx.h> 

// Create/declare one echo effect and configure it
fx_delay my_echo_1(1000.0,   // 1 second echo
                   0.7);     // 0.7 feedback ratio

void setup() {

   pedal.init(true); // Initialize the system

   // Connect our effect(s) to input and output jacks
   pedal.route_audio(pedal.instr_in, my_echo_1.input);
   pedal.route_audio(my_echo_1.output, pedal.amp_out);

   pedal.run(); // Run the effect

}


```

You can also generate a report of the final routing and parameters by adding a few additional commands before calling `pedal.run()`.  `print_instance_stack()` will show you all of the instances in the sketch.  `print_routing_table()` will show you how the audio and control are wired up.  And `print_param_tables()` will show you the parameters for each instance.

``` C
// Include our library of effects routines
#include <dreammakerfx.h> 

// Create/declare one echo effect and configure it
fx_delay my_echo_1(1000.0,   // 1 second echo
                   0.7);     // 0.7 feedback ratio
 

void setup() {

   pedal.init(true); // Initialize the system

   // Connect our effect(s) to input and output jacks
   pedal.route_audio(pedal.instr_in, my_echo_1.input);
   pedal.route_audio(my_echo_1.output, pedal.amp_out)

   // Print debug info to Serial Monitor
   pedal.print_instance_stack();
   pedal.print_routing_table();
   pedal.print_param_tables();

   pedal.run(); // Run the effect

}
```