set execDir=%CD%

if not exist %execDir%/Dependencies/openal-soft/nul (
	git clone https://github.com/kcat/openal-soft.git %execDir%/Dependencies/openal-soft
)

if not exist %execDir%/Dependencies_build/openal-soft/nul (
	mkdir %execDir%/Dependencies_build/openal-soft
	cmake -S%execDir%/Dependencies/openal-soft -B%execDir%/Dependencies_build/openal-soft -DALSOFT_EXAMPLES=False
	cmake --build %execDir%/Dependencies_build/openal-soft --config Release
)

if not exist %execDir%/Dependencies/vcpkg/nul (
	git clone --branch 2025.06.13 https://github.com/microsoft/vcpkg %execDir%/Dependencies/vcpkg 
)
