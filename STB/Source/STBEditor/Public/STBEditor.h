// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSTBEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:
    static const FName ProgressionClassName;
};
