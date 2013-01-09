# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "UNIT_TESTS=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) -I../../Libs/re2
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -L../../Libs/re2/obj -lre2
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "UNIT_TESTS=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  TARGET := Synth-A-ModelerGUI
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS) -I../../Libs/re2
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -L../../Libs/re2/obj -lre2
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  TARGET := Synth-A-ModelerGUI
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/ForceBasedFlowAlgorithm_2d9e5b9.o \
  $(OBJDIR)/CircleFlowAlgorithm_8478b2f1.o \
  $(OBJDIR)/GraphUtils_6816740d.o \
  $(OBJDIR)/ForceDirectedFlowAlgorithm_3998fbd8.o \
  $(OBJDIR)/DirectedGraph_c0af84a6.o \
  $(OBJDIR)/Node_98b30e4c.o \
  $(OBJDIR)/CommandLine_e08eafe1.o \
  $(OBJDIR)/SAMLookAndFeel_df6eec67.o \
  $(OBJDIR)/MainWindow_3e8328d1.o \
  $(OBJDIR)/Application_7b202ed2.o \
  $(OBJDIR)/Main_70df61c1.o \
  $(OBJDIR)/SAMRegex_b144d836.o \
  $(OBJDIR)/RegularExpression_7b2cba50.o \
  $(OBJDIR)/IdManager_97f65d0e.o \
  $(OBJDIR)/ResourceLoader_4c525f0f.o \
  $(OBJDIR)/MiscUtilities_c4d90fa.o \
  $(OBJDIR)/MDLWriter_7bd6d284.o \
  $(OBJDIR)/Alerts_379eb1e5.o \
  $(OBJDIR)/MDLParser_674db430.o \
  $(OBJDIR)/StoredSettings_125117b4.o \
  $(OBJDIR)/MDLController_4aad0d67.o \
  $(OBJDIR)/ObjController_e2cf3d29.o \
  $(OBJDIR)/SAMCompiler_55c40662.o \
  $(OBJDIR)/ObjectFactory_a170b74f.o \
  $(OBJDIR)/SAMCmd_cf7547a1.o \
  $(OBJDIR)/MDLFile_5f0f8395.o \
  $(OBJDIR)/RedrawOptionsPanel_f391d648.o \
  $(OBJDIR)/CommentEditor_abeb432b.o \
  $(OBJDIR)/CommentComponent_3199b309.o \
  $(OBJDIR)/SnapGridPainter_bcfdcdba.o \
  $(OBJDIR)/AudioOutConnector_995cd494.o \
  $(OBJDIR)/BaseObjectComponent_eac867ec.o \
  $(OBJDIR)/LinkComponent_6a391d22.o \
  $(OBJDIR)/VariablesPanel_8104ce78.o \
  $(OBJDIR)/ObjectPropertiesPanel_99e3c0d1.o \
  $(OBJDIR)/ObjectComponent_96e7177d.o \
  $(OBJDIR)/OutputWindow_f103bf7c.o \
  $(OBJDIR)/PrefsPanel_2006a45f.o \
  $(OBJDIR)/ContentComp_99fa8a27.o \
  $(OBJDIR)/ObjectsHolder_b334705f.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_core_5f5a3bad.o \
  $(OBJDIR)/juce_cryptography_55e5c007.o \
  $(OBJDIR)/juce_data_structures_7e517fa9.o \
  $(OBJDIR)/juce_events_88dd09a1.o \
  $(OBJDIR)/juce_graphics_29b23085.o \
  $(OBJDIR)/juce_gui_basics_a9efcd81.o \
  $(OBJDIR)/juce_gui_extra_a9396b49.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking Synth-A-ModelerGUI
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning Synth-A-ModelerGUI
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

strip:
	@echo Stripping Synth-A-ModelerGUI
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/ForceBasedFlowAlgorithm_2d9e5b9.o: ../../Source/Graph/ForceBasedFlowAlgorithm.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ForceBasedFlowAlgorithm.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CircleFlowAlgorithm_8478b2f1.o: ../../Source/Graph/CircleFlowAlgorithm.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CircleFlowAlgorithm.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GraphUtils_6816740d.o: ../../Source/Graph/GraphUtils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GraphUtils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ForceDirectedFlowAlgorithm_3998fbd8.o: ../../Source/Graph/ForceDirectedFlowAlgorithm.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ForceDirectedFlowAlgorithm.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DirectedGraph_c0af84a6.o: ../../Source/Graph/DirectedGraph.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DirectedGraph.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Node_98b30e4c.o: ../../Source/Graph/Node.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Node.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CommandLine_e08eafe1.o: ../../Source/Application/CommandLine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CommandLine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SAMLookAndFeel_df6eec67.o: ../../Source/Application/SAMLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SAMLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_3e8328d1.o: ../../Source/Application/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Application_7b202ed2.o: ../../Source/Application/Application.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Application.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_70df61c1.o: ../../Source/Application/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SAMRegex_b144d836.o: ../../Source/Utilities/SAMRegex.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SAMRegex.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RegularExpression_7b2cba50.o: ../../Source/Utilities/RegularExpression.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RegularExpression.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/IdManager_97f65d0e.o: ../../Source/Utilities/IdManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling IdManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ResourceLoader_4c525f0f.o: ../../Source/Utilities/ResourceLoader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ResourceLoader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MiscUtilities_c4d90fa.o: ../../Source/Utilities/MiscUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MiscUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MDLWriter_7bd6d284.o: ../../Source/Utilities/MDLWriter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MDLWriter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Alerts_379eb1e5.o: ../../Source/Utilities/Alerts.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Alerts.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MDLParser_674db430.o: ../../Source/Utilities/MDLParser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MDLParser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StoredSettings_125117b4.o: ../../Source/Utilities/StoredSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StoredSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MDLController_4aad0d67.o: ../../Source/Controller/MDLController.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MDLController.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ObjController_e2cf3d29.o: ../../Source/Controller/ObjController.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ObjController.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SAMCompiler_55c40662.o: ../../Source/Models/SAMCompiler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SAMCompiler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ObjectFactory_a170b74f.o: ../../Source/Models/ObjectFactory.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ObjectFactory.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SAMCmd_cf7547a1.o: ../../Source/Models/SAMCmd.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SAMCmd.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MDLFile_5f0f8395.o: ../../Source/Models/MDLFile.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MDLFile.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RedrawOptionsPanel_f391d648.o: ../../Source/View/RedrawOptionsPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RedrawOptionsPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CommentEditor_abeb432b.o: ../../Source/View/CommentEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CommentEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CommentComponent_3199b309.o: ../../Source/View/CommentComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CommentComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SnapGridPainter_bcfdcdba.o: ../../Source/View/SnapGridPainter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SnapGridPainter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioOutConnector_995cd494.o: ../../Source/View/AudioOutConnector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioOutConnector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BaseObjectComponent_eac867ec.o: ../../Source/View/BaseObjectComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BaseObjectComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LinkComponent_6a391d22.o: ../../Source/View/LinkComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LinkComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/VariablesPanel_8104ce78.o: ../../Source/View/VariablesPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling VariablesPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ObjectPropertiesPanel_99e3c0d1.o: ../../Source/View/ObjectPropertiesPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ObjectPropertiesPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ObjectComponent_96e7177d.o: ../../Source/View/ObjectComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ObjectComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OutputWindow_f103bf7c.o: ../../Source/View/OutputWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OutputWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PrefsPanel_2006a45f.o: ../../Source/View/PrefsPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PrefsPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ContentComp_99fa8a27.o: ../../Source/View/ContentComp.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ContentComp.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ObjectsHolder_b334705f.o: ../../Source/View/ObjectsHolder.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ObjectsHolder.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_5f5a3bad.o: ../../../juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_55e5c007.o: ../../../juce/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_7e517fa9.o: ../../../juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_88dd09a1.o: ../../../juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_29b23085.o: ../../../juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_a9efcd81.o: ../../../juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_a9396b49.o: ../../../juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
