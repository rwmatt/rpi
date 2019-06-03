# atlast

atlast is adapted from John Walker's public domain 
[Forth](https://www.fourmilab.ch/atlast/).

I have targetted it towards mcus (microcontrollers), but with
a little massaging, it should work just fine as a Forth on
the desktop.

Advantages:
* pure C implementation, so it should be highly portable and
more readable
* compiles on Arduino. So no more faffing around getting
obscure toolchains working. If you can upload a sketch to your mcu,
then you should be able to get this working, subject to some
caveats.

Disdvantages:
* probably slower than an assembly-written Forth
* VERY rough around the edges. It's a proof of concept, which seems
to have merit.
* It doesn't have the libraries you really want, like fiddling with
GPIO pins. HOWEVER, the Forth is in place, so it's a fairly simple
matter of adding in the hooks to get all the hardware stuff going
* It uses malloc/free in places, which isn't ideal. Maybe there's a way
around that.
