cmake_minimum_required(VERSION 3.30)

# Set the project name and version
project(Parameters VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set AAX SDK path
set(AAX_SDK_PATH "D:/ALL_SDK/AAX_SDK" CACHE PATH "Path to AAX SDK")

# Fetch JUCE
include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.10
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(JUCE)

# Create the audio plugin target
juce_add_plugin(${PROJECT_NAME}
    
    COMPANY_NAME "Wev Company"                 # Specify the name of the plugin's author
    IS_SYNTH FALSE                              # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE           # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE                # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Weva               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Para                            # A unique four-character plugin id with exactly one upper-case character                                            # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS VST3 AAX Standalone                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    VST3_CATEGORIES Fx 
    AAX_CATEGORY AAX_ePlugInCategory_Effect 
    PRODUCT_NAME "${PROJECT_NAME}") 

    # Disable VST2/VST3 compatibility warnings (not building VST2)
target_compile_definitions(${PROJECT_NAME} 
    PUBLIC 
        JUCE_VST3_CAN_REPLACE_VST2=0)
    

    target_sources(${PROJECT_NAME}
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginProcessor.h
        src/PluginEditor.cpp
        src/PluginEditor.h)

    target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)    