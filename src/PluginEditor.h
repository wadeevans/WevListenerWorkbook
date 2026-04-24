/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

// #include <JuceHeader.h>

// #include <JuceHeader.h> // old Projucer method of pulling in JUCE files

// new CMake method of pulling in JUCE files
// #include <juce_audio_processors/juce_audio_processors.h>
// #include <juce_audio_devices/juce_audio_devices.h>
// #include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"

//==============================================================================
/**
 */
class ParametersAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
  ParametersAudioProcessorEditor(ParametersAudioProcessor &);
  ~ParametersAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  ParametersAudioProcessor &audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametersAudioProcessorEditor)
};
