#       --build-cache-path string      Builds of 'core.a' are saved into this path to be cached and reused.
#       --build-path string            Path where to save compiled files. If omitted, a directory will be created in the default temporary path of your OS.
#       --build-property stringArray   Override a build property with a custom value. Can be used multiple times for multiple properties.
#       --clean                        Optional, cleanup the build folder and do not use any cached build.
#   -e, --export-binaries              If set built binaries will be exported to the sketch folder.
#   -b, --fqbn string                  Fully Qualified Board Name, e.g.: arduino:avr:uno
#   -h, --help                         help for compile
#       --libraries strings            List of custom libraries paths separated by commas. Or can be used multiple times for multiple libraries paths.
#       --only-compilation-database    Just produce the compilation database, without actually compiling.
#       --optimize-for-debug           Optional, optimize compile output for debugging, rather than for release.
#       --output-dir string            Save build artifacts in this directory.
#   -p, --port string                  Upload port, e.g.: COM10 or /dev/ttyACM0
#       --preprocess                   Print32_t preprocessed code to stdout instead of compiling.
#   -P, --programmer string            Optional, use the specified programmer to upload.
#       --quiet                        Optional, suppresses almost every output.
#       --show-properties              Show all build properties used instead of compiling.
#   -u, --upload                       Upload the binary after the compilation.
#   -t, --verify                       Verify uploaded binary after the upload.
#       --vid-pid string               When specified, VID/PID specific build properties are used, if board supports them.
#       --warnings string              Optional, can be "none", "default", "more" and "all". Defaults to "none". Used to tell gcc which warning level to use (-W flag). (default "none")

all:
	arduino-cli compile -b arduino:avr:mega -u -p /dev/ttyUSB0 -t