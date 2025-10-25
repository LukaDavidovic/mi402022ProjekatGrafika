
if (NOT EXISTS "/home/matfrg/mi402022ProjekatGrafika/cmake-build-default/engine/libs/glfw/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"/home/matfrg/mi402022ProjekatGrafika/cmake-build-default/engine/libs/glfw/install_manifest.txt\"")
endif()

file(READ "/home/matfrg/mi402022ProjekatGrafika/cmake-build-default/engine/libs/glfw/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("/home/matfrg/Programs/CLion-2024.2.2.tar/clion-2024.2.2/bin/cmake/linux/x64/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("/home/matfrg/Programs/CLion-2024.2.2.tar/clion-2024.2.2/bin/cmake/linux/x64/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

