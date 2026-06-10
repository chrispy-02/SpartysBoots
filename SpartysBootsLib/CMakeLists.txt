project(SpartysBootsLib)

set(SOURCE_FILES
        MainFrame.cpp
        MainFrame.h
        pch.h
        Game.cpp
        Game.h
        GameView.cpp
        GameView.h
        Vector.cpp
        Vector.h
        Item.cpp
        Item.h
        AssetManager.cpp
        AssetManager.h
        Image.cpp
        Image.h
        Conveyor.cpp
        Conveyor.h
        ids.h
        ConveyorBackground.cpp
        ConveyorBackground.h
        ConveyorPanel.cpp
        ConveyorPanel.h
        Scoreboard.cpp
        Scoreboard.h
        Sparty.cpp
        Sparty.h
        Beam.cpp
        Beam.h
        Pin.cpp
        Pin.h
        Sensor.cpp
        Sensor.h
        Product.cpp
        Product.h
        Gate.cpp
        Gate.h
        GateOr.cpp
        GateOr.h
        GateAnd.cpp
        GateAnd.h
        LevelLoader.cpp
        LevelLoader.h
        GateNand.cpp
        GateNand.h
        GateNot.cpp
        GateNot.h
        PinOwner.h
        GateDFlipFlop.cpp
        GateDFlipFlop.h
        GateSRFlipFlop.cpp
        GateSRFlipFlop.h
        LogicState.h
        ItemVisitor.cpp
        ItemVisitor.h
        LastProcessedVisitor.cpp
        LastProcessedVisitor.h
        SensorRangeVisitor.cpp
        SensorRangeVisitor.h
        ScoringVisitor.cpp
        ScoringVisitor.h)

set(wxBUILD_PRECOMP OFF)
find_package(wxWidgets COMPONENTS core base xrc html xml REQUIRED)

include(${wxWidgets_USE_FILE})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})