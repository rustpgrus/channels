include(FetchContent)

message(STATUS "Fetch libcoro...")
FetchContent_Declare(
  libcoro
  GIT_REPOSITORY https://github.com/jbaldwin/libcoro.git
  GIT_TAG        v0.4
)

set(LIBCORO_BUILD_TESTS OFF)
set(LIBCORO_BUILD_EXAMPLES OFF)
FetchContent_MakeAvailable(libcoro)

message(STATUS "Fetch googletest...")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
