<img src="https://github.com/0x22fe/Unum/blob/master/icon.png" width="64" height="auto" alt="Unum Language Logo"/>

# Unum
## Version 1.0.0
A programming language designed for simplicity, concurrency, and flow. The interpreter is written in C99.

**The project has suspended development, however it is usable.**

# Reference
View the library [reference](#library-reference). 

# Examples

The usual "Hello World" example:

```
# A comment

@function $main ->
@parameters { args: String } ->
@result int ->
@body
(
    io.print "Hello, World!";
    @return 0
);

```

Integrating a function over a given range:

```
# Test function f(x) = x^2
@function $f ->
@parameters { x: f64 } ->
@result f64 ->
@body (math.pow x, 2.0 -> @return);

# Integrate function f(x) over [0.0, 5.0]

# Inputs
@variable $range_min, f64, 0.0;
@variable $range_max, f64, 5.0;

@variable $steps, f64, 10000.0;
@variable $STEPS, u64, 10000;

# Calculation
@variable $sum, f64, 0.0;
@variable $i, f64, range_min;
@variable $increment, f64, (math.sub range_max, range_min -> math.div steps);

@loop STEPS,
(
    math.mul increment, (f i) -> math.add sum -> sum;
    math.add i, increment -> i
);

# Print output
io.println sum

```

# Planned features

Features that are planned include:

* More efficient and cleaner codebase
* Better interface for native functions
* Internal memory pool
* Unified error reporting and handling
* Full unicode character support
* Sockets and threads support
* File handling support
* Graphics/UI support
* Compiling

(And more.)

# Dependencies
The project is built with CMake. The only other dependency of the project is [plibsys](https://github.com/saprykin/plibsys), which will be automatically downloaded by CMake on build.

# Library Reference
This section contains information on the core libraries of Unum.

Libraries
 * Math (math), since v1.0.0
 * I/O (io), since v1.0.0
 
# License
All code is under the MIT License.
