# The basics of creating / adding effects
------

As you hopefully remember from 12 seconds ago, we create/declare the effects we want to use at the top of program.

``` C
// Create/declare one echo effect and configure it
fx_delay   my_echo_1(1000.0, 0.7);    
```

The first word (which in this case is `fx_delay`) is the *type* of effect we want to create.  The API docs (and the next section) contain the complete list of the effects that are available.

We then provide a name for our effect object (which in the example above is `my_echo_1`).  This needs to be a unique word with no spaces (just characters and underscores really).

And finally, we provide the initial parameters for that effect (i.e. where the knobs are set initially).

Again, the The API docs contain more details on this.

What's neat is that this *object* then becomes its own stand-alone effect.  We can create multiple objects of the same type in our program (i.e. multiple delays in this case) that each have their own parameters and which are each wired-in in their own ways.
 
``` C
// Create/declare one echo effect and configure it
fx_delay   my_echo_1(1000.0, 0.7);    
fx_delay   my_echo_2(2000.0, 0.8);    // Totally legit!    
```

Just make sure each object you create/declare in your system has a unique name even if they are different effect types.  For example, don't do this:
 
``` C
// Create/declare one echo effect and configure it
fx_delay        ricky_bobby(1000.0, 0.7);    
fx_pitch_shift  ricky_bobby(0.8);    // BAD! DON'T DO THIS, ricky_bobby already exists!   
```

Oh yeah, this is important: in some cases an effect will have a few different ways you can initialize it.  Most effects have a *simple* initializer that you just need to pass one or two values to.  And, they may have a more *advanced* initializer that allows you to do ever more things with that effect. Usually the advanced initializer is a super-set of the simple initializer.

Here's an example of us initializing two `fx_amplitude_mod` objects with both the simple and advanced initializer functions:

``` C
fx_amplitude_mod    tremelo_1(1.0,     // Rate is 1Hz
                              0.5);    // Depth is 0.5 (0->1)
fx_amplitude_mod    tremelo_2(1.0,      // Rate is 1Hz
                              0.5,      // Depth is 0.5 (0->1)
                              OSC_TRI,  // Modulation waveform is triangle instead of sine
                              false);   // Not using an external signal as our modulator                           
```

The beauty of this is that we have a DSP platform with tons of memory and lots of DSP processing power so you can create effects that incorporate several different individual effect objects / instances.

