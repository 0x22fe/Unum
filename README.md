<img src="https://github.com/0x22fe/Unum/blob/master/icon.png" width="64" height="auto" alt="Unum Language Logo"/>

# Unum
A programming language designed for simplicity, concurrency, and flow.

# Status
The project is currently in active development and an initial working version is almost ready.

# Examples

The usual "Hello World" example:

```
# A comment

@function main ->
@parameters { args: String } ->
@result int ->
@body
(
    io.print "Hello, World!";
    @return 0;
)

```

More examples coming soon!

# Planned features

Features that are planned or already in development include:

* Internal memory pool
* Unified error reporting and handling
* Full unicode character support
* Sockets and threads support
* File handling support
* Graphics/UI support
* Compiling

And more!

# Dependencies
The only dependency of the project is [plibsys](https://github.com/saprykin/plibsys), which will be automatically downloaded by CMake on build.

# License
All code is under the MIT License.
