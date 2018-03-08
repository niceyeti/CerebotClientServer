Old undergraduate code dump of a linux side client server for remote robotic control

The intent was a protocol for controlling a cerebot upright robot, forking processes
for maintaining the complete sensor state of the robot to different consoles, with
another console for inputing commands. These procs used unix domain sockets to communicate.

None of this was well-architected, but its practical linux code and thought I would
keep it. The unix sockets for interprocess communication are useful for other things.
Wouldn't use C++ for this; use python instead (cross platform and faster dev).

