include(../tp_stub/krkrz.cmake)

krkrz_plugin(gamepad
    SOURCES
        Main.cpp
        src/GamePadDeviceInspector.cpp
        src/GamePadDirectInputMappingDB.cpp
        src/GamepadInputContext.cpp
        src/GamePadLog.cpp
        src/GamePadXInputDevice.cpp
        src/GamePadDirectInputDevice.cpp
        src/GamePadForceFeedbackDevice.cpp
        src/GamePadInputDevicePort.cpp
        src/GamePadUtil.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)