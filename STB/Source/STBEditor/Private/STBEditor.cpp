// Search for a Star 2023

#include "STBEditor.h"

#include "FProgressionDetails.h"

#define LOCTEXT_NAMESPACE "FSTBEditorModule"

const FName FSTBEditorModule::ProgressionClassName = FName("ProgressionData");

void FSTBEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(ProgressionClassName, FOnGetDetailCustomizationInstance::CreateStatic(&FProgressionDetails::MakeInstance));
}

void FSTBEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(ProgressionClassName);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSTBEditorModule, STBEditor)